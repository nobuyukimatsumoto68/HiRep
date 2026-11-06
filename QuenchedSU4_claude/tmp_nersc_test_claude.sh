#!/bin/bash
#
# Chunk 7 local test: NERSC reader + measure-only mode (grid_claude/16c SU(4) configs, 16^3 x 8, 2 ranks).
#   PHASE 1 rebuild (nersc_io_claude.c, measure-only mode)
#   PHASE 2 run 1: list of 3 configs; run 2: same list (must skip all); run 3: list + 1 (must measure only it)
#   PHASE 3 checks: h5 traj = [1000 2000 3000 4000]; per-plane plaquettes (HiRep log) vs an independent
#           Python reader of the same files (a direction mix-up would show here, not in the average plaquette)
# Run it yourself:  bash /mnt/baracuda_14/hirep_su4/QuenchedSU4_claude/tmp_nersc_test_claude.sh
# I read back:      /mnt/baracuda_14/hirep_su4/QuenchedSU4_claude/tmp_nersc_test_claude.log
#
set -u
set -e
set -o pipefail

HERE=/mnt/baracuda_14/hirep_su4/QuenchedSU4_claude
EXE=/mnt/baracuda_14/hirep_su4/PureGauge/suN_glueballs_h5_claude
LOG=$HERE/tmp_nersc_test_claude.log
TDIR=$HERE/runs_claude/nersc16c_test
CDIR=/mnt/baracuda_14/grid_claude/16c

export PATH=/mnt/hdd_barracuda/opt/openmpi_cuda/bin:$PATH
export LD_LIBRARY_PATH=/mnt/hdd_barracuda/opt/openmpi_cuda/lib:/mnt/hdd_barracuda/opt/myhdfstuff/hdf5-2.1.0/lib:${LD_LIBRARY_PATH:-}
export HWLOC_COMPONENTS=-gl
export OMP_NUM_THREADS=1

echo "================ PHASE 1: rebuild ($(date)) ================" 2>&1 | tee "$LOG"
if [ -e "$TDIR" ]; then
    echo "$TDIR exists: remove it yourself, then rerun" 2>&1 | tee -a "$LOG"
    exit 1
fi
set +e
NJOBS=4 bash $HERE/build_su4_claude.sh > $HERE/tmp_build_su4_nersc_claude.log 2>&1
RC=$?
set -e
grep -nE "error|warning|nersc_io_claude|suN_glueballs_h5_claude" $HERE/tmp_build_su4_nersc_claude.log | head -20 2>&1 | tee -a "$LOG" || true
echo "build exit code: $RC" 2>&1 | tee -a "$LOG"
if [ "$RC" -ne 0 ]; then
    tail -30 $HERE/tmp_build_su4_nersc_claude.log 2>&1 | tee -a "$LOG"
    echo "BUILD FAILED" 2>&1 | tee -a "$LOG"
    exit 1
fi

echo "================ PHASE 2: measure-only runs ($(date)) ================" 2>&1 | tee -a "$LOG"
mkdir -p "$TDIR"
cd "$TDIR"
printf "%s\n" $CDIR/ckpoint_lat.1000 $CDIR/ckpoint_lat.2000 $CDIR/ckpoint_lat.3000 > list3_claude.txt
printf "%s\n" $CDIR/ckpoint_lat.1000 $CDIR/ckpoint_lat.2000 $CDIR/ckpoint_lat.3000 $CDIR/ckpoint_lat.4000 > list4_claude.txt

# run <tag> <list>
run() {
    local tag=$1
    local list=$2
    sed "s#CONFIGLIST#$list#" $HERE/input_su4_nersc16c_claude > input_$tag
    local t0=$SECONDS
    set +e
    mpirun -np 2 "$EXE" -i input_$tag -o out_$tag < /dev/null > mpirun_$tag.log 2>&1
    local rc=$?
    set -e
    echo "---- [$tag] $list: exit $rc, $((SECONDS - t0)) s" 2>&1 | tee -a "$LOG"
    grep -hE "measure-only|NERSC|read: plaq|skipped|glueballs \(APE|done:|ERROR" out_${tag}_0 2>&1 | tee -a "$LOG" || true
    if [ "$rc" -ne 0 ]; then
        tail -20 mpirun_$tag.log 2>&1 | tee -a "$LOG"
        echo "TEST RUN FAILED" 2>&1 | tee -a "$LOG"
        exit 1
    fi
}

run r1 list3_claude.txt
run r2 list3_claude.txt
run r3 list4_claude.txt

echo "================ PHASE 3: checks ================" 2>&1 | tee -a "$LOG"
set +e
python3 -I - "$TDIR" "$CDIR" <<'EOF_PY' 2>&1 | tee -a "$LOG"
import re
import sys
import h5py
import numpy as np
tdir = sys.argv[1]
cdir = sys.argv[2]
ok = True
f = h5py.File("%s/glue_nersc16c_claude.h5" % tdir, "r")
traj = list(f["traj"][:])
print("h5 traj", traj, "glue_ape4", f["glue_ape4"].shape)
ok = ok and traj == [1000, 2000, 3000, 4000]
g = f["glue_ape4"][:]
nz = int((np.abs(g).max(axis=(0, 1, 3, 4)) == 0).sum())
print("ops identically zero:", nz, " empty timeslices:", int((np.abs(g).max(axis=(0, 1, 2, 4)) == 0).sum()))
ok = ok and nz == 0


def planes(path):
    raw = open(path, "rb").read()
    end = raw.index(b"END_HEADER\n") + len(b"END_HEADER\n")
    d = np.frombuffer(raw[end:], dtype=">f8")
    U = (d[0::2] + 1j * d[1::2]).reshape(8, 16, 16, 16, 4, 4, 4)
    ax = {0: 3, 1: 2, 2: 1, 3: 0}
    res = {}
    for mu in range(4):
        for nu in range(mu + 1, 4):
            a = U[..., mu, :, :]
            b = np.roll(U[..., nu, :, :], -1, axis=ax[mu])
            c = np.roll(U[..., mu, :, :], -1, axis=ax[nu])
            dd = U[..., nu, :, :]
            P = a @ b @ np.conj(np.swapaxes(c, -1, -2)) @ np.conj(np.swapaxes(dd, -1, -2))
            res[(mu, nu)] = np.trace(P, axis1=-2, axis2=-1).real.mean() / 4
    return res


grid_of_hirep = {0: 3, 1: 0, 2: 1, 3: 2}
for tag, cfgs in [("r1", [1000, 2000, 3000]), ("r3", [4000])]:
    txt = open("%s/out_%s_0" % (tdir, tag)).read()
    vals = re.findall(r"Plaq\((\d),(\d)\) = \( ([-0-9.]+) ,", txt)
    for k, c in enumerate(cfgs):
        py = planes("%s/ckpoint_lat.%d" % (cdir, c))
        worst = 0.0
        for j in range(6):
            a, b, v = vals[6 * k + j]
            gm = grid_of_hirep[int(a)]
            gn = grid_of_hirep[int(b)]
            key = (min(gm, gn), max(gm, gn))
            worst = max(worst, abs(float(v) - py[key]))
        print("config %d: max |plane plaquette HiRep - Python| = %.2e (HiRep prints 6 digits)" % (c, worst))
        ok = ok and worst < 2e-6
print("ALL NERSC TESTS PASSED" if ok else "NERSC TESTS FAILED")
sys.exit(0 if ok else 1)
EOF_PY
echo "DONE ($(date))" 2>&1 | tee -a "$LOG"
