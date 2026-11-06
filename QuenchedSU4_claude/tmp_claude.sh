#!/bin/bash
#
# Stream 1 segment 2 handoff (FlowInterval):
#   PHASE 1  rebuild the driver (FlowInterval, /topo_traj)
#   PHASE 2  quick 8^3 x 16 tests in runs_claude/flowint_test (stops here if any fails):
#            A = fresh 12 updates, C = 8 + resume 4 (ObsInterval 4, FlowInterval 8): C == A incl. /topo_traj;
#            L = legacy h5 (chunk3_fix3/C, no /topo_traj) resumed +4: /topo_traj created, flows at 4,8,12,16
#   PHASE 3  stream 1: resume from 540, n_update 2000 (100 measurements, flow every 100 = 20 flows), ~31 h.
#            The stream h5 is first COPIED to glue_..._seg1end_claude.h5 (new file) as a safeguard.
# Run detached:
#   nohup bash /mnt/baracuda_14/hirep_su4/QuenchedSU4_claude/tmp_claude.sh > /dev/null 2>&1 &
# I read back: /mnt/baracuda_14/hirep_su4/QuenchedSU4_claude/tmp_seg2_claude.log (phases 1-2 + start of 3),
#              runs_claude/stream1_24c_b11p045/out_stream1_seg2_claude_0 (progress of phase 3)
#
set -u
set -e
set -o pipefail

HERE=/mnt/baracuda_14/hirep_su4/QuenchedSU4_claude
EXE=/mnt/baracuda_14/hirep_su4/PureGauge/suN_glueballs_h5_claude
LOG=$HERE/tmp_seg2_claude.log
TBASE=$HERE/runs_claude/flowint_test
SDIR=$HERE/runs_claude/stream1_24c_b11p045
SH5=glue_su4_24c_b11p045_s1_claude.h5
H5=glue_c3_claude.h5

export PATH=/mnt/hdd_barracuda/opt/openmpi_cuda/bin:$PATH
export LD_LIBRARY_PATH=/mnt/hdd_barracuda/opt/openmpi_cuda/lib:/mnt/hdd_barracuda/opt/myhdfstuff/hdf5-2.1.0/lib:${LD_LIBRARY_PATH:-}
export HWLOC_COMPONENTS=-gl
export OMP_NUM_THREADS=1

echo "================ PHASE 1: rebuild ($(date)) ================" 2>&1 | tee "$LOG"
set +e
NJOBS=4 bash $HERE/build_su4_claude.sh > $HERE/tmp_build_su4_claude.log 2>&1
RC=$?
set -e
grep -nE "error|suN_glueballs_h5_claude" $HERE/tmp_build_su4_claude.log | head -20 2>&1 | tee -a "$LOG" || true
echo "build exit code: $RC" 2>&1 | tee -a "$LOG"
if [ "$RC" -ne 0 ]; then
    tail -30 $HERE/tmp_build_su4_claude.log 2>&1 | tee -a "$LOG"
    echo "BUILD FAILED" 2>&1 | tee -a "$LOG"
    exit 1
fi

echo "================ PHASE 2: FlowInterval tests ($(date)) ================" 2>&1 | tee -a "$LOG"
if [ -e "$TBASE" ]; then
    echo "$TBASE exists: remove it yourself, then rerun" 2>&1 | tee -a "$LOG"
    exit 1
fi
if [ -e "$SDIR/${SH5%.h5}_seg1end_claude.h5" ]; then
    echo "$SDIR/${SH5%.h5}_seg1end_claude.h5 exists: segment 2 was already started; resume by hand" 2>&1 | tee -a "$LOG"
    exit 1
fi
mkdir -p "$TBASE"/A "$TBASE"/C "$TBASE"/L

# run <dir> <n_update> <tag> [CheckpointStart]
run() {
    local dir=$1
    local nup=$2
    local tag=$3
    local ck=${4:-}
    sed "s/NUPDATE/$nup/" $HERE/input_su4_chunk3_claude > "$dir"/input_$tag
    echo "FlowInterval = 8" >> "$dir"/input_$tag
    cd "$dir"
    local xflag=""
    if [ -n "$ck" ]; then
        xflag="-x CheckpointStart=$ck"
    fi
    local t0=$SECONDS
    set +e
    mpirun -np 2 $xflag "$EXE" -i input_$tag -o out_$tag < /dev/null > mpirun_$tag.log 2>&1
    local rc=$?
    set -e
    echo "---- $dir [$tag] n_update=$nup CheckpointStart=${ck:-none}: exit $rc, $((SECONDS - t0)) s" 2>&1 | tee -a "$LOG"
    grep -hE "FlowInterval|H5\]|Wilson flow|done:|ERROR" out_${tag}_0 2>&1 | tee -a "$LOG" || true
    if [ "$rc" -ne 0 ]; then
        tail -20 mpirun_$tag.log 2>&1 | tee -a "$LOG"
        echo "TEST RUN FAILED" 2>&1 | tee -a "$LOG"
        exit 1
    fi
}

run "$TBASE"/A 12 a12
run "$TBASE"/C 8 c8
run "$TBASE"/C 4 c4 8
cp $HERE/runs_claude/chunk3_fix3/C/ckpoint_lat.12 $HERE/runs_claude/chunk3_fix3/C/ckpoint_rng.12 "$TBASE"/L/
cp $HERE/runs_claude/chunk3_fix3/C/$H5 "$TBASE"/L/$H5
run "$TBASE"/L 4 l4 12

set +e
python3 -I - "$TBASE" "$H5" <<'EOF_PY' 2>&1 | tee -a "$LOG"
import sys
import h5py
import numpy as np
base = sys.argv[1]
name = sys.argv[2]
ok = True
a = h5py.File("%s/A/%s" % (base, name), "r")
c = h5py.File("%s/C/%s" % (base, name), "r")
l = h5py.File("%s/L/%s" % (base, name), "r")
print("A traj", a["traj"][:], "topo_traj", a["topo_traj"][:], "topo rows", a["topo"].shape[0])
for k in ["traj", "plaq", "glue", "tor", "topo", "topo_traj"]:
    eq = np.array_equal(a[k][:], c[k][:])
    ok = ok and eq
    print("A vs C", k, "equal" if eq else "DIFFER")
ok = ok and list(a["topo_traj"][:]) == [8]
print("L traj", l["traj"][:], "topo_traj", l["topo_traj"][:], "topo rows", l["topo"].shape[0])
ok = ok and list(l["traj"][:]) == [4, 8, 12, 16] and list(l["topo_traj"][:]) == [4, 8, 12, 16] and l["topo"].shape[0] == 4
NG = int(a.attrs["NG"])
for f, tag in [(a, "A"), (l, "L")]:
    rows = [list(f["traj"][:]).index(t) for t in f["topo_traj"][:]]
    d = np.abs(f["topo"][:, 0, 1] - 12 * NG * (1 - f["plaq"][:][rows])).max()
    ok = ok and d < 1e-10
    print("E(t=0) check %s: %.3e" % (tag, d))
print("ALL FLOWINTERVAL TESTS PASSED" if ok else "FLOWINTERVAL TESTS FAILED")
sys.exit(0 if ok else 1)
EOF_PY
RC=${PIPESTATUS[0]}
set -e
if [ "$RC" -ne 0 ]; then
    echo "stopping before production" 2>&1 | tee -a "$LOG"
    exit 1
fi

echo "================ PHASE 3: stream 1 segment 2 ($(date)) ================" 2>&1 | tee -a "$LOG"
cd "$SDIR"
if [ ! -f ckpoint_lat.540 ] || [ ! -f ckpoint_rng.540 ]; then
    echo "ckpoint_*.540 missing in $SDIR" 2>&1 | tee -a "$LOG"
    exit 1
fi
cp $SH5 ${SH5%.h5}_seg1end_claude.h5
cp $HERE/input_su4_24c_seg2_claude .
echo "safeguard copy: ${SH5%.h5}_seg1end_claude.h5; started $(date); progress: $SDIR/out_stream1_seg2_claude_0" 2>&1 | tee -a "$LOG"
T0=$SECONDS
set +e
mpirun -np 4 -x CheckpointStart=540 "$EXE" -i input_su4_24c_seg2_claude -o out_stream1_seg2_claude < /dev/null > mpirun_stream1_seg2_claude.log 2>&1
RC=$?
set -e
echo "segment 2 exit code: $RC   wall: $((SECONDS - T0)) s" 2>&1 | tee -a "$LOG"
grep -hE "H5\]|done:|ERROR" out_stream1_seg2_claude_0 2>&1 | tee -a "$LOG" || true
grep -h "Wilson flow" out_stream1_seg2_claude_0 | tail -3 2>&1 | tee -a "$LOG" || true
echo "DONE ($(date))" 2>&1 | tee -a "$LOG"
