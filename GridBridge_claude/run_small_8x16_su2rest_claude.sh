#!/bin/bash
#
# Small-lattice glueball test with the 38-operator su2rest set (A1++ x8, E++, T2++ at rest; plan chunk 6d)
# and the shift-phase fix. Same GridSeed as the first run (20261020-131546), hence the SAME Markov chain:
# PHASE 4 checks that the plaquettes and the moving-frame A1 operators reproduce runs_claude/small_8x16.
#   8^3 x 16, NP_T=1 (one rank holds all timeslices), nOMP=4, betaF=2.4, nMD=16, trajL=1,
#   n_traj=2000, ObsInterval=1, nblkstart=0..nblkend=2 (45 glueball ops per row), GridSeed=auto.
#
#   PHASE 0  environment + toolchain
#   PHASE 1  rebuild bridge .o + HiRep PureGauge_grid (grid_hmc_seed, GridSeed key, Params.MD log fix)
#   PHASE 1b MPI smoke test
#   PHASE 2  production run in HiRep/runs_claude/small_8x16 (own dir: its checkpoints cannot touch
#            other runs' ckpoint_* files). Grid stdout is filtered on the fly to HMC summary lines.
#
# Run it yourself:  bash HiRep/GridBridge_claude/run_small_8x16_su2rest_claude.sh  (paths below are for the PI machine)
# I read back:      tmp_small_8x16_claude.log (summary)
#                   HiRep/runs_claude/small_8x16_su2rest/grid_hmc_summary_claude.log (filtered Grid log)
#                   HiRep/runs_claude/small_8x16_su2rest/out_small_8x16_su2rest_claude_0 (HiRep output: plaq, glueball rows)
#
set -u
set -e
set -o pipefail

WORKDIR=/mnt/baracuda_14/hirep_workspace
HIREP=$WORKDIR/HiRep
LOG=$WORKDIR/tmp_small_8x16_su2rest_claude.log
GRID_BUILD=$WORKDIR/build
RUNDIR=$HIREP/runs_claude/small_8x16_su2rest
OLDRUN=$HIREP/runs_claude/small_8x16
INPUT_SRC=$HIREP/GridBridge_claude/input_hmc_glueballs_grid_small_su2rest_claude
INPUT=input_hmc_glueballs_grid_small_su2rest_claude
EXE=$HIREP/PureGauge/hmc_glueballs_grid_claude
SRC=$HIREP/PureGauge/hmc_glueballs_grid_claude.c
GRIDLOG=$RUNDIR/grid_hmc_summary_claude.log
# Grid lines kept: per-trajectory H/dH, Metropolis decision, seeding, MD parameters, errors
PATTERN="Total H|dH|etropolis|ccept|unique string|Seed SHA256|MD steps|Step size|ERROR|rror|ssert|egmentation|signal"

export PATH=/mnt/hdd_barracuda/opt/openmpi_cuda/bin:$PATH
export LD_LIBRARY_PATH=/mnt/hdd_barracuda/opt/openmpi_cuda/lib:/mnt/hdd_barracuda/opt/myhdfstuff/hdf5-2.1.0/lib:${LD_LIBRARY_PATH:-}

# LLVM toolchain (clang 21) pinned to the GCC 13 libstdc++ that built libGrid.a (GCC 13.3).
# See GridBridge_claude/HANDOFF_claude.md section 4 for why.
GCC13_DIR=/usr/lib/gcc/x86_64-linux-gnu/13
export OMPI_CC="clang --gcc-install-dir=$GCC13_DIR"
export OMPI_CXX="clang++ --gcc-install-dir=$GCC13_DIR"
export BRIDGE_CXX="clang++ --gcc-install-dir=$GCC13_DIR -std=c++17"

# hwloc 2.13 GL plugin hangs mpirun on this desktop (X display :1 never accepts).
export HWLOC_COMPONENTS=-gl

echo "================ PHASE 0: environment ================" 2>&1 | tee "$LOG"
echo "date       : $(date)" 2>&1 | tee -a "$LOG"
echo "host       : $(hostname)" 2>&1 | tee -a "$LOG"
echo "mpicxx     : $(mpicxx --showme | cut -c1-80)" 2>&1 | tee -a "$LOG"
echo "BRIDGE_CXX : $BRIDGE_CXX" 2>&1 | tee -a "$LOG"
echo "GRID_BUILD : $GRID_BUILD" 2>&1 | tee -a "$LOG"

echo "================ PHASE 1: build ================" 2>&1 | tee -a "$LOG"
GRID_BUILD="$GRID_BUILD" bash "$HIREP"/GridBridge_claude/build_grid_hirep_claude.sh 2>&1 | tail -25 | tee -a "$LOG"
if [ ! -x "$EXE" ]; then
    echo "BUILD FAILED: $EXE missing (full log: HiRep/GridBridge_claude/build_grid_hirep_claude.log)" 2>&1 | tee -a "$LOG"
    exit 1
fi
if [ ! "$EXE" -nt "$SRC" ]; then
    echo "BUILD STALE: $EXE is older than $SRC" 2>&1 | tee -a "$LOG"
    exit 1
fi
# (not grep -q: it exits early, nm gets SIGPIPE, and pipefail turns that into a false failure)
if ! nm "$EXE" | grep " T grid_hmc_seed" > /dev/null; then
    echo "BUILD STALE: grid_hmc_seed not linked into $EXE" 2>&1 | tee -a "$LOG"
    exit 1
fi
echo "PHASE 1 OK (grid_hmc_seed linked)" 2>&1 | tee -a "$LOG"

echo "================ PHASE 1b: MPI smoke test ================" 2>&1 | tee -a "$LOG"
mpirun -np 1 hostname < /dev/null 2>&1 | tee -a "$LOG"
echo "PHASE 1b OK" 2>&1 | tee -a "$LOG"

echo "================ PHASE 2: production run (mpirun -np 1, 2000 traj) ================" 2>&1 | tee -a "$LOG"
mkdir -p "$RUNDIR"
cp "$INPUT_SRC" "$RUNDIR"/"$INPUT"
cd "$RUNDIR"
echo "run dir    : $RUNDIR" 2>&1 | tee -a "$LOG"
cat "$INPUT" 2>&1 | tee -a "$LOG"
echo "----------------------------------------------------------------" 2>&1 | tee -a "$LOG"
echo "started    : $(date)" 2>&1 | tee -a "$LOG"
T0=$SECONDS
set +e
mpirun -np 1 "$EXE" -i "$INPUT" -o out_small_8x16_su2rest_claude < /dev/null 2>&1 | grep --line-buffered -E "$PATTERN" > "$GRIDLOG"
PS=("${PIPESTATUS[@]}")
set -e
T1=$SECONDS
RC=${PS[0]}
echo "finished   : $(date)" 2>&1 | tee -a "$LOG"
echo "mpirun exit code: $RC" 2>&1 | tee -a "$LOG"
echo "WALL TIME: $((T1 - T0)) s" 2>&1 | tee -a "$LOG"
echo "--- tail of filtered Grid log ---" 2>&1 | tee -a "$LOG"
tail -6 "$GRIDLOG" 2>&1 | tee -a "$LOG"
echo "--- HiRep output ---" 2>&1 | tee -a "$LOG"
grep -E "GridSeed|betaF=" out_small_8x16_su2rest_claude_0 2>&1 | tee -a "$LOG" || true
grep -E "Trajectory [0-9]+ +plaq" out_small_8x16_su2rest_claude_0 | tail -2 2>&1 | tee -a "$LOG" || true
ls -la "$RUNDIR" 2>&1 | tee -a "$LOG"
if [ "$RC" -ne 0 ]; then
    echo "PHASE 2 FAILED" 2>&1 | tee -a "$LOG"
    exit 1
fi
echo "PHASE 2 OK" 2>&1 | tee -a "$LOG"

echo "================ PHASE 3: jackknife analysis (analyze_glueball_1pt_claude.py) ================" 2>&1 | tee -a "$LOG"
# T and nlev must match the input (GLB_T, nblkend - nblkstart + 1); tables -> $RUNDIR/jk_*.txt
OMP_NUM_THREADS=4 OPENBLAS_NUM_THREADS=4 python3 -I "$HIREP"/GridBridge_claude/analyze_glueball_1pt_claude.py \
    out_small_8x16_su2rest_claude_0 --T 16 --nlev 3 --ntherm 200 --bin 20 --tmax 4 --binscan --save jk 2>&1 | tee -a "$LOG"
echo "PHASE 3 OK" 2>&1 | tee -a "$LOG"

echo "================ PHASE 4: same-chain check against runs_claude/small_8x16 ================" 2>&1 | tee -a "$LOG"
# Same GridSeed -> identical configurations. Expect: all plaquettes equal, and the 7 moving-frame A1
# operators (old set: 0,1,2,3,11,12,14 of 15; new set: 0,1,2,3,35,36,37 of 38) equal as printed, all levels.
python3 -I - "$OLDRUN"/out_small_8x16_claude_0 out_small_8x16_su2rest_claude_0 <<'PY' 2>&1 | tee -a "$LOG"
import re
import sys
num = re.compile(r"\(\s*\S+ \+I\*\(\S+\)\)")
plaq = re.compile(r"Trajectory (\d+)\s+plaq = (\S+)")
def load(path):
    p = {}
    rows = []
    for line in open(path):
        m = plaq.search(line)
        if m:
            p[int(m.group(1))] = m.group(2)
        if line.startswith("[step][0]"):
            rows.append(num.findall(line))
    return p, rows
p_old, r_old = load(sys.argv[1])
p_new, r_new = load(sys.argv[2])
common = sorted(set(p_old) & set(p_new))
nbad = sum(1 for k in common if p_old[k] != p_new[k])
print("plaquette: %d common trajectories, %d differ" % (len(common), nbad))
idx_old = [0, 1, 2, 3, 11, 12, 14]
idx_new = [0, 1, 2, 3, 35, 36, 37]
n = min(len(r_old), len(r_new))
nbad = 0
for i in range(n):
    for lev in range(3):
        for a, b in zip(idx_old, idx_new):
            if r_old[i][lev * 15 + a] != r_new[i][lev * 38 + b]:
                nbad += 1
print("moving-frame A1 values: %d rows compared, %d mismatches" % (n, nbad))
PY
echo "PHASE 4 done" 2>&1 | tee -a "$LOG"
echo "DONE" 2>&1 | tee -a "$LOG"
