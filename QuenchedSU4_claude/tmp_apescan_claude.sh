#!/bin/bash
#
# Chunk 6 handoff: APE smearing-depth scan. Runs ALONGSIDE stream 1 segment 2 (user decision; 4 more cores).
# (New file name: tmp_claude.sh is the script of the running segment-2 job and must not be modified.)
#   PHASE 1  rebuild (gb_ape_iter in LibHR/Observables/glueballs.c, APEscan in the driver). The running
#            segment-2 binary is not affected (the linker replaces the file, the running process keeps its copy).
#   PHASE 2  8^3 x 16 tests in runs_claude/apescan_test (stops here if any fails):
#            A = fresh 12 updates with APEscan = 1,2:
#                /glue_ape1 == runs_claude/chunk3_fix3/A /glue bit for bit (default depth unchanged),
#                traj/plaq/tor/topo equal too; /glue_ape2 == /glue_ape1 at level 0 (raw links), differs at level >= 1
#            C = 8 + resume 4 with APEscan = 1,2: C == A for every dataset
#   PHASE 3  scan run in runs_claude/apescan_24c: copy the latest COMPLETE stream-1 checkpoint pair, resume
#            from it with input_su4_24c_apescan_claude (APEscan = 1,2,4,8, 500 updates = 25 measurements),
#            new h5 glue_su4_24c_b11p045_apescan_claude.h5. ~1 h per measurement -> ~25 h.
# Run detached:
#   nohup bash /mnt/baracuda_14/hirep_su4/QuenchedSU4_claude/tmp_apescan_claude.sh > /dev/null 2>&1 &
# I read back: /mnt/baracuda_14/hirep_su4/QuenchedSU4_claude/tmp_apescan_claude.log,
#              runs_claude/apescan_24c/out_apescan_claude_0 (progress of phase 3)
#
set -u
set -e
set -o pipefail

HERE=/mnt/baracuda_14/hirep_su4/QuenchedSU4_claude
EXE=/mnt/baracuda_14/hirep_su4/PureGauge/suN_glueballs_h5_claude
LOG=$HERE/tmp_apescan_claude.log
TBASE=$HERE/runs_claude/apescan_test
SDIR=$HERE/runs_claude/stream1_24c_b11p045
RDIR=$HERE/runs_claude/apescan_24c
H5=glue_c3_claude.h5

export PATH=/mnt/hdd_barracuda/opt/openmpi_cuda/bin:$PATH
export LD_LIBRARY_PATH=/mnt/hdd_barracuda/opt/openmpi_cuda/lib:/mnt/hdd_barracuda/opt/myhdfstuff/hdf5-2.1.0/lib:${LD_LIBRARY_PATH:-}
export HWLOC_COMPONENTS=-gl
export OMP_NUM_THREADS=1

echo "================ PHASE 1: rebuild ($(date)) ================" 2>&1 | tee "$LOG"
for d in "$TBASE" "$RDIR"; do
    if [ -e "$d" ]; then
        echo "$d exists: remove it yourself, then rerun" 2>&1 | tee -a "$LOG"
        exit 1
    fi
done
set +e
NJOBS=4 bash $HERE/build_su4_claude.sh > $HERE/tmp_build_su4_apescan_claude.log 2>&1
RC=$?
set -e
grep -nE "error|glueballs\.o|suN_glueballs_h5_claude" $HERE/tmp_build_su4_apescan_claude.log | head -20 2>&1 | tee -a "$LOG" || true
echo "build exit code: $RC" 2>&1 | tee -a "$LOG"
if [ "$RC" -ne 0 ]; then
    tail -30 $HERE/tmp_build_su4_apescan_claude.log 2>&1 | tee -a "$LOG"
    echo "BUILD FAILED" 2>&1 | tee -a "$LOG"
    exit 1
fi

echo "================ PHASE 2: APEscan tests ($(date)) ================" 2>&1 | tee -a "$LOG"
mkdir -p "$TBASE"/A "$TBASE"/C

# run <dir> <n_update> <tag> [CheckpointStart]
run() {
    local dir=$1
    local nup=$2
    local tag=$3
    local ck=${4:-}
    sed "s/NUPDATE/$nup/" $HERE/input_su4_chunk3_claude > "$dir"/input_$tag
    echo "APEscan = 1,2" >> "$dir"/input_$tag
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
    grep -hE "glueball dataset|H5\]|glueballs \(APE|done:|ERROR" out_${tag}_0 | head -12 2>&1 | tee -a "$LOG" || true
    if [ "$rc" -ne 0 ]; then
        tail -20 mpirun_$tag.log 2>&1 | tee -a "$LOG"
        echo "TEST RUN FAILED" 2>&1 | tee -a "$LOG"
        exit 1
    fi
}

run "$TBASE"/A 12 a12
run "$TBASE"/C 8 c8
run "$TBASE"/C 4 c4 8

set +e
python3 -I - "$TBASE" "$HERE/runs_claude/chunk3_fix3/A" "$H5" <<'EOF_PY' 2>&1 | tee -a "$LOG"
import sys
import h5py
import numpy as np
base = sys.argv[1]
ref = h5py.File("%s/%s" % (sys.argv[2], sys.argv[3]), "r")
a = h5py.File("%s/A/%s" % (base, sys.argv[3]), "r")
c = h5py.File("%s/C/%s" % (base, sys.argv[3]), "r")
ok = True
print("A datasets:", sorted(a.keys()), "APE depths:", [int(a.attrs["APE_depth_%d" % k]) for k in range(int(a.attrs["n_APE_depths"]))])
eq = np.array_equal(a["glue_ape1"][:], ref["glue"][:])
ok = ok and eq
print("A /glue_ape1 vs chunk3_fix3/A /glue:", "equal" if eq else "DIFFER")
for k in ["traj", "plaq", "tor", "topo"]:
    eq = np.array_equal(a[k][:], ref[k][:])
    ok = ok and eq
    print("A vs chunk3_fix3/A", k, "equal" if eq else "DIFFER")
g1 = a["glue_ape1"][:]
g2 = a["glue_ape2"][:]
eq0 = np.array_equal(g1[:, 0], g2[:, 0])
diff1 = not np.array_equal(g1[:, 1], g2[:, 1])
ok = ok and eq0 and diff1
print("ape2 vs ape1: level 0", "equal" if eq0 else "DIFFER", "; level 1", "differs" if diff1 else "EQUAL (unexpected)")
for k in ["traj", "plaq", "glue_ape1", "glue_ape2", "tor", "topo", "topo_traj"]:
    eq = np.array_equal(a[k][:], c[k][:])
    ok = ok and eq
    print("A vs C", k, "equal" if eq else "DIFFER")
print("ALL APESCAN TESTS PASSED" if ok else "APESCAN TESTS FAILED")
sys.exit(0 if ok else 1)
EOF_PY
RC=${PIPESTATUS[0]}
set -e
if [ "$RC" -ne 0 ]; then
    echo "stopping before the scan run" 2>&1 | tee -a "$LOG"
    exit 1
fi

echo "================ PHASE 3: 24c APE depth scan ($(date)) ================" 2>&1 | tee -a "$LOG"
# latest COMPLETE checkpoint pair of stream 1 (lat 679477276 bytes, rng 1688 bytes for NP_T = 4)
CK=""
for f in $(ls -1 $SDIR/ckpoint_lat.* | sed 's/.*ckpoint_lat\.//' | sort -n -r); do
    if [ -f $SDIR/ckpoint_rng.$f ] && [ "$(stat -c %s $SDIR/ckpoint_lat.$f)" = "679477276" ] && [ "$(stat -c %s $SDIR/ckpoint_rng.$f)" = "1688" ]; then
        CK=$f
        break
    fi
done
if [ -z "$CK" ]; then
    echo "no complete stream-1 checkpoint pair found" 2>&1 | tee -a "$LOG"
    exit 1
fi
mkdir -p "$RDIR"
cp $SDIR/ckpoint_lat.$CK $SDIR/ckpoint_rng.$CK "$RDIR"/
if [ "$(stat -c %s $RDIR/ckpoint_lat.$CK)" != "679477276" ] || [ "$(stat -c %s $RDIR/ckpoint_rng.$CK)" != "1688" ]; then
    echo "checkpoint copy incomplete" 2>&1 | tee -a "$LOG"
    exit 1
fi
cp $HERE/input_su4_24c_apescan_claude "$RDIR"/
cd "$RDIR"
echo "copied stream-1 checkpoint $CK; scan = updates $((CK + 1))..$((CK + 500)); started $(date)" 2>&1 | tee -a "$LOG"
T0=$SECONDS
set +e
mpirun -np 4 -x CheckpointStart=$CK "$EXE" -i input_su4_24c_apescan_claude -o out_apescan_claude < /dev/null > mpirun_apescan_claude.log 2>&1
RC=$?
set -e
echo "scan exit code: $RC   wall: $((SECONDS - T0)) s" 2>&1 | tee -a "$LOG"
grep -hE "H5\]|done:|ERROR" out_apescan_claude_0 2>&1 | tee -a "$LOG" || true
echo "DONE ($(date))" 2>&1 | tee -a "$LOG"
