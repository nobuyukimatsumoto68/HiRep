#!/bin/bash
#SBATCH --job-name=grid_hirep_first_run
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=4
#SBATCH --time=00:20:00
##SBATCH --partition=<partition>
##SBATCH --account=<account>
#
# First-run example for the Grid-in-HiRep glueball pipeline on a SLURM cluster.
# Input: GridBridge_claude/input_first_run_example_claude (SU(2), 4^3 x 8, 1 rank, 50 trajectories,
# blocking levels 0-1, fixed GridSeed). Takes about a minute.
#
# Submit from the HiRep root after building (GridBridge_claude/README_claude.md):
#   cd /path/to/HiRep && sbatch GridBridge_claude/run_example_slurm_claude.sh
# or run it inside an interactive allocation:  bash GridBridge_claude/run_example_slurm_claude.sh
#
# Pass criteria (printed at the end):
#   - exit code 0 (each trajectory asserts HiRep plaquette == Grid plaquette; a failed transfer aborts)
#   - "GridSeed = first-run-example" in the HiRep output
#   - Metropolis acceptance well above 0.5 at nMD = 16, trajL = 1
#   - average plaquette near 0.63 (beta = 2.4; a 4^3 x 8 box shifts it slightly)
#   - the analysis script reads all 8 timeslices per measurement
#
set -u
set -e
set -o pipefail

# ---- environment: same modules as for the build ----------------------------------------------
# module load gcc openmpi hdf5 gmp mpfr fftw openssl
HIREP=${HIREP:-${SLURM_SUBMIT_DIR:-$(pwd)}}            # the HiRep root (sbatch copies this script elsewhere)
MPI_LAUNCH=${MPI_LAUNCH:-srun -n 1}                     # or "mpirun -np 1"
export OMP_NUM_THREADS=${SLURM_CPUS_PER_TASK:-4}
# hwloc 2.13 GL plugin can hang mpirun on desktops with an X/Wayland session; harmless elsewhere
export HWLOC_COMPONENTS=-gl

EXE=$HIREP/PureGauge/hmc_glueballs_grid_claude
INPUT_SRC=$HIREP/GridBridge_claude/input_first_run_example_claude
ANALYSIS=$HIREP/GridBridge_claude/analyze_glueball_1pt_claude.py
RUNDIR=$HIREP/runs_claude/first_run_example
OUT=out_first_run_example_claude

if [ ! -x "$EXE" ]; then
    echo "not built: $EXE (run GridBridge_claude/build_grid_hirep_claude.sh first)"
    exit 1
fi
mkdir -p "$RUNDIR"
cp "$INPUT_SRC" "$RUNDIR"/input_first_run_example_claude
cd "$RUNDIR"

echo "==== run: $MPI_LAUNCH $EXE ($(date)) ===="
set +e
$MPI_LAUNCH "$EXE" -i input_first_run_example_claude -o "$OUT" < /dev/null > grid_stdout_claude.log 2>&1
RC=$?
set -e
echo "exit code: $RC"
grep -E "GridSeed|betaF=" "$OUT"_0 || true
echo "accepted: $(grep -c 'Metropolis_test -- ACCEPTED' grid_stdout_claude.log || true)  rejected: $(grep -c 'Metropolis_test -- REJECTED' grid_stdout_claude.log || true)"
grep -E "Trajectory [0-9]+ +plaq" "$OUT"_0 | tail -3 || true
if [ "$RC" -ne 0 ]; then
    echo "FAILED: see $RUNDIR/grid_stdout_claude.log and ${OUT}_0"
    exit 1
fi

echo "==== analysis ===="
# T = GLB_T, nlev = nblkend - nblkstart + 1; short run, so few bins (numbers are only a sanity check)
python3 -I "$ANALYSIS" "$OUT"_0 --T 8 --nlev 2 --ntherm 10 --bin 5 --tmax 2
echo "DONE"
