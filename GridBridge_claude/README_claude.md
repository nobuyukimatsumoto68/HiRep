# Grid-in-HiRep bridge: setup guide

Grid's pure-gauge Hybrid Monte Carlo generates SU(2) configurations, and HiRep measures observables
(plaquette, glueball and torelon 1-point functions) on each one in the same process, with no
configuration files in between. This file explains how to get the two codes, build them and link
them on a new machine. Project status, physics results and open work: `HANDOFF_claude.md`.

Methods: Hybrid Monte Carlo (S. Duane, A. D. Kennedy, B. J. Pendleton, D. Roweth, Phys. Lett. B195
(1987) 216) with Grid's `MinimumNorm2` second-order minimum-norm integrator (I. P. Omelyan,
I. M. Mryglod, R. Folk, Comput. Phys. Commun. 151 (2003) 272).

## 1. Repositories

| Code | Repository | Notes |
|---|---|---|
| HiRep (with this bridge) | `https://github.com/nobuyukimatsumoto68/HiRep.git`, branch `HiRep-CUDA` (fork of `claudiopica/HiRep`) | the bridge lives in `GridBridge_claude/` |
| Grid | `https://github.com/paboyle/Grid.git` (upstream) | no fork needed; must be configured with `--enable-Nc=2` |

```bash
git clone -b HiRep-CUDA https://github.com/nobuyukimatsumoto68/HiRep.git   # read-only, no account needed
# (with push access: git clone -b HiRep-CUDA git@github.com:nobuyukimatsumoto68/HiRep.git)
git clone https://github.com/paboyle/Grid.git        # or let build_grid_example_claude.sh clone it
```

## 2. Requirements

- C/C++ compiler with C++17. GCC 15 / libstdc++ 15 is fine since upstream HiRep commit `2a54d8b4`
  ("complex type fix for gcc 15", merged into this fork in `dd4ec8d7`); older clones need an older
  libstdc++ (see troubleshooting).
- MPI with C and C++ wrappers (`mpicc`, `mpicxx`).
- HDF5 **with the C++ API** (`H5Cpp.h`, `libhdf5_cpp`): Grid's headers include it.
- GMP, MPFR, FFTW (double and float), OpenSSL, zlib, and c-lime (built by the Grid example script if
  missing).
- Python 3 with numpy, for `analyze_glueball_1pt_claude.py`.
- Optional: Mathematica / wolframscript (only to regenerate the glueball operator code) and
  clang-format 17 (`pip install clang-format==17.0.6`, to format generated code).

## 3. Build Grid with Nc = 2

`GridBridge_claude/build_grid_example_claude.sh` is a commented example for a module-based cluster.
Edit its `module load` line and dependency prefixes (or export them), then:

```bash
GRID_ROOT=$HOME/grid SIMD=AVX2 bash GridBridge_claude/build_grid_example_claude.sh
```

It clones upstream Grid into `$GRID_ROOT/Grid`, builds c-lime if needed, and configures and
installs into `$GRID_ROOT/build` (call it `$GRID_BUILD`). The options that matter:

| Option | Why |
|---|---|
| `--enable-Nc=2` | **required**: must equal HiRep `NG = 2` |
| `--enable-comms=mpi-auto` | MPI through the `mpicc`/`mpicxx` wrappers |
| `--enable-simd=<AVX2/AVX512/...>` | the compute nodes' CPUs |
| `--enable-openmp` | Grid threads (`nOMP` in the input) |
| `--with-hdf5/gmp/mpfr/fftw/lime/openssl=<prefix>` | dependency locations |
| `--disable-gparity --disable-fermion-reps` | not needed for pure gauge; faster build |

Check: `grep Config_Nc $GRID_BUILD/include/Grid/Config.h` must print `#define Config_Nc 2`.

**Tested so far (2026-10-22):** sections 1, 4, 5 and 6 were run from a fresh clone on the PI's
workstation (reusing its existing Grid build; first run with `MPI_LAUNCH="mpirun -np 1"`).
`build_grid_example_claude.sh` and the SLURM lines of `run_example_slurm_claude.sh` have not been run
anywhere yet: expect to adapt module names, prefixes and batch options.
(`build_grid_ubuntu_claude.sh` is the PI's workstation script, kept as a reference only.)

## 4. HiRep build configuration for your machine

HiRep's build reads `Make/MkFlags.ini`; the bridge build script stages it from a file you give it.
The committed `GridBridge_claude/MkFlags_claude.ini` is the PI's machine. Instead:

```bash
cd /path/to/HiRep
cp GridBridge_claude/MkFlags_example_claude.ini GridBridge_claude/MkFlags_mymachine_claude.ini
# fill in INCLUDE (HDF5 C headers) and GRIDLIBS; GRIDLIBS is generated from YOUR Grid:
echo "GRIDLIBS = -L$PWD/GridLibrary $($GRID_BUILD/grid-config --ldflags) $($GRID_BUILD/grid-config --libs) -fopenmp"
```

Paste the printed `GRIDLIBS` line into your file. The MkFlags file must not contain double quotes or
backslashes anywhere, comments included: HiRep embeds it in a C string (`Make/Utils/cinfo.sh`). Keep `NG = 2`, `CXX = mpicxx`, and keep `NDEBUG`
unset (no `NDEBUG` macro): the drivers `assert` every trajectory that the gauge field arrived in
HiRep correctly.

## 5. Build the bridge and HiRep

```bash
MKFLAGS=$PWD/GridBridge_claude/MkFlags_mymachine_claude.ini \
GRID_BUILD=$GRID_ROOT/build \
    bash GridBridge_claude/build_grid_hirep_claude.sh
# log: GridBridge_claude/build_grid_hirep_claude.log
```

What it does (it never writes into the Grid tree):

0. Checks that Grid's `Nc` (from `Config.h`) equals `NG` in your MkFlags file; aborts otherwise.
1. Records Grid's compiler and flags from `grid-config` in `GridLibrary/grid_flags_claude.txt`.
2. Compiles the two bridges, `grid_hirep_hmc_claude.cpp` (HMC) and `grid_hirep_io_claude.cpp`
   (checkpoint reader), with `grid-config --cxx` and `--cxxflags`. Set `BRIDGE_CXX` to override the
   compiler while keeping Grid's flags.
3. Copies the bridge objects, `libGrid.a` and `Config.h` into `GridLibrary/` (gitignored) and the
   bridge headers into `Include/`.
4. Stages your MkFlags file, `build_claude.pl` and `ScriptForMake` into the HiRep tree.
5. Runs `ScriptForMake`, i.e. `Make/nj PureGauge_grid`, which builds four executables in
   `PureGauge/`:

| Executable | Purpose |
|---|---|
| `hmc_glueballs_grid_claude` | HMC + glueball/torelon measurement + checkpointing (main tool) |
| `hmc_grid_claude` | bare HMC (timing, transfer check) |
| `hmc_glueballs_grid_hdf5_claude` | as the main tool, also writes `glueballs.<traj>.h5` |
| `mk_1pt_glueballs_from_grid_claude` | glueballs on Grid NERSC checkpoints read from disk |

### Linking rules (why the build is set up this way)

- **Grid `Nc` = HiRep `NG`.** The gauge field crosses through a buffer of `2*NG*NG` doubles per site
  and direction; Grid writes `2*Nc*Nc`. A mismatch overflows it (segfault) and simulates the wrong
  group.
- **Bridge objects use Grid's own compiler and flags** (`grid-config`); hand-picked flags give
  ABI-incompatible objects (crashes or a wrong `Total H`). Rebuild the bridge after rebuilding Grid.
- **C/C++ boundary:** Grid code exists only in the two bridge `.cpp` files, behind `extern "C"`
  functions and an opaque `struct HmcState*`; the HiRep drivers are plain C.
- **Linker = C++ MPI wrapper.** `build_claude.pl` (block "PureGauge with Grid I/O") links these
  executables with `LINK = $CXX` (`mpicxx`) and `$GRIDLIBS`, with `-L.../GridLibrary` first so the
  captured `libGrid.a` is used.
- **MPI ownership:** HiRep's `setup_process` initialises MPI, then the bridge calls `Grid_init` with a
  generated argument list (`--grid`, `--mpi`, `--threads`). Never call `Grid_finalize()`: HiRep's
  `finalize_process()` owns `MPI_Finalize`.
- **Process grid:** only `NP_T > 1` is allowed (`NP_X = NP_Y = NP_Z = 1`; HiRep's spatial transforms).
- **Directions:** Grid orders $(x,y,z,t)$, HiRep $(t,x,y,z)$; the bridge readout maps Grid $\mu$ to
  HiRep slot $(\mu+1) \bmod 4$, so the drivers' `copy_to_ugauge` needs no change.

## 6. First run

`GridBridge_claude/run_example_slurm_claude.sh` with `input_first_run_example_claude`: SU(2),
$4^3 \times 8$, one rank, 50 trajectories, about a minute. From the HiRep root:

```bash
sbatch GridBridge_claude/run_example_slurm_claude.sh                 # or, in an allocation:
MPI_LAUNCH="mpirun -np 1" bash GridBridge_claude/run_example_slurm_claude.sh
```

Pass: exit code 0 (the per-trajectory HiRep/Grid plaquette `assert` held), `GridSeed =
first-run-example` in the output, Metropolis acceptance well above 0.5, plaquette near 0.63, and
the analysis script reading all 8 timeslices. Output in `runs_claude/first_run_example/`.

## 7. Production runs

- **Driver:** `PureGauge/hmc_glueballs_grid_claude -i <input> -o <name>` (output `<name>_0`). Run
  each job in its own directory: checkpoints are written to the working directory, and the
  driver deletes the previous pair after writing a new one.
- **Input keys** (one file): lattice and process grid (`GLB_T/X/Y/Z`, `NP_T/X/Y/Z`), HMC (`betaF`,
  `betaA`, `nMD`, `trajL`, `n_traj`, `nOMP` = Grid threads, `coldStart`,
  `NoMetropolisUntilRoutine` = MD-only thermalization trajectories), measurement (`ObsInterval`,
  `nblkstart`, `nblkend`, `APEsmear`), and `GridSeed` (`auto` = date+time string, logged; an
  explicit string replays exactly the same Markov chain).
- **All timeslices:** glueball rows are printed by rank 0 only, so use `NP_T = 1` when the analysis
  needs every timeslice.
- **Resume:** `CheckpointStart=<n>` in the environment (pass it to the ranks, e.g. `mpirun -x
  CheckpointStart`) loads `ckpoint_lat.<n>` / `ckpoint_rng.<n>`; `n_traj` then counts additional
  trajectories.
- **Cost on $8^3 \times 16$, one rank, 4 threads, 3 blocking levels, `ObsInterval = 1`:** about
  5.6 s per trajectory with the installed 38-operator set (2.5 s of it is the single-threaded
  measurement) and about 78 KB of output per trajectory. Grid's own stdout is about 160 KB per
  trajectory; filter it (see `run_small_8x16_claude.sh`).
- **Analysis:** `python3 GridBridge_claude/analyze_glueball_1pt_claude.py <name>_0 --T <GLB_T>
  --nlev <nblkend-nblkstart+1> [--binscan] [--save PREFIX]`.
- `run_small_8x16_claude.sh` and `run_small_8x16_su2rest_claude.sh` are the PI's complete
  build+run+analysis scripts. Use them as templates; change `WORKDIR`, `GRID_BUILD`, `PATH`,
  `LD_LIBRARY_PATH`, the compiler variables (`GCC13_DIR`, `OMPI_CC`, `OMPI_CXX`, `BRIDGE_CXX`) and the
  launcher.

## 8. Checkpoint reader (glueballs on stored configurations)

`PureGauge/mk_1pt_glueballs_from_grid_claude` measures the same observables on Grid NERSC
checkpoints. Examples: `input_glue_grid_example_claude` (input) and
`listconfs_grid_example_claude.txt` (one checkpoint path per line, **no comments**: they would be read
as file names; the trajectory number is taken from the `.<n>` suffix).

## 9. Glueball operator code

`LibHR/Observables/glueballs_op.c` and `Include/Observables/glueballs.h` are generated by a
Mathematica package. The installed set has 38 operators; regeneration needs the patched generator
`glueball_gen_claude/OpProjection_claude.m` (not the original in `Make/Utils/Math_glueballs/`),
because `LibHR/Observables/glueballs.c` uses the `gb_shift_unit` variable it defines. See
`HANDOFF_claude.md` section 4 and `glueball_generator_audit_claude.md`.

## 10. Troubleshooting

| Symptom | Cause | Fix |
|---|---|---|
| `Make/Utils/autosun/...: reference to 'complex' is ambiguous` | a clone older than `dd4ec8d7`: libstdc++ 15 forward-declares `std::complex`, which clashed with autosun's own `class complex` | pull this branch (upstream fix `2a54d8b4` renames it `complex_t`), or use a compiler with an older libstdc++ (the PI used `OMPI_CC`/`OMPI_CXX` = `clang --gcc-install-dir=/usr/lib/gcc/x86_64-linux-gnu/13`) |
| build script: `FATAL: Grid Nc=... != HiRep NG=...` | Grid configured with another `--enable-Nc` | reconfigure Grid with `--enable-Nc=2` |
| segfault in the first trajectory, or `Total H` far from about 334000 on $8^3 \times 32$ | Grid `Nc` != HiRep `NG`, or bridge `.o` and `libGrid.a` from different Grid builds | rebuild Grid cleanly, then rerun the bridge build |
| `H5Cpp.h: No such file` compiling the bridge | HDF5 without the C++ API, or not in Grid's flags | HDF5 with C++ support; reconfigure Grid with `--with-hdf5` |
| ninja: `GridLibrary/grid_hirep_*.o missing` | bridge objects not built | run `build_grid_hirep_claude.sh` (it builds both) |
| `LibHR/Utils/cinfo.c: error: expected ';' after top level declarator` | a double quote or backslash in your MkFlags file: HiRep embeds it as a C string (`Make/Utils/cinfo.sh` escapes only newlines) | remove `"` and `\` from the MkFlags file, comments included |
| link: `cannot find -lmpi_cxx` | a `GRIDLIBS` line copied from another machine | regenerate `GRIDLIBS` from your `grid-config` (section 4) |
| `mpirun` hangs before any output, even `mpirun -np 1 hostname` | hwloc 2.13 GL plugin probing an X/Wayland display that never answers | `export HWLOC_COMPONENTS=-gl` |
| `assert(fabs(avr_plaquette() - grid_hmc_plaquette(S)) < 1e-6)` fails | gauge-field transfer broken (layout, Nc, ABI) | as for the segfault row |

## 11. Files in this directory

| File | Role |
|---|---|
| `README_claude.md` | this setup guide |
| `HANDOFF_claude.md` | status, results, open work; start here |
| `build_grid_example_claude.sh` | example Grid build for a module cluster (Nc = 2) |
| `build_grid_ubuntu_claude.sh` | the PI's Grid build (reference only) |
| `build_grid_hirep_claude.sh` | builds the bridges, copies Grid's library, builds HiRep (`GRID_BUILD`, `MKFLAGS`) |
| `MkFlags_example_claude.ini` | template HiRep build config (fill in `INCLUDE`, `GRIDLIBS`) |
| `MkFlags_claude.ini` | the PI's build config |
| `build_claude.pl`, `ScriptForMake_claude` | HiRep build rules with the Grid link block; build driver |
| `grid_hirep_hmc_claude.cpp` / `.h` | HMC bridge (`grid_hmc_init/seed/step/plaquette/save_checkpoint/load_checkpoint/finalize`) |
| `grid_hirep_io_claude.cpp` / `.h` | checkpoint-reader bridge (`grid_init`, `grid_read_config`) |
| `run_example_slurm_claude.sh`, `input_first_run_example_claude` | first run (section 6) |
| `run_small_8x16_claude.sh`, `run_small_8x16_su2rest_claude.sh` | the PI's $8^3 \times 16$ build+run+analysis scripts |
| `input_hmc_glueballs_grid_small_claude`, `input_hmc_glueballs_grid_small_su2rest_claude` | their inputs |
| `input_hmc_glueballs_grid_100_claude`, `input_hmc_grid_4node_claude` | older inputs ($8^3 \times 32$, 4 ranks) |
| `input_glue_grid_example_claude`, `listconfs_grid_example_claude.txt` | checkpoint-reader examples |
| `analyze_glueball_1pt_claude.py` | correlators, binned jackknife, effective masses |
| `glueball_gen_claude/` | operator generator: patched `OpProjection_claude.m`, `gen_glueball_ops_claude.wls`, `run_gen_glueball_ops_claude.sh`, generated `stage_su2rest/` files |
| `glueball_generator_audit_claude.md` | generator audit, operator table, shift-phase bug |
| `glueball_mass_test_impl_plan_claude.md` | small-lattice mass test: estimators, results, history |
| `checkpointer_impl_plan_claude.md` | checkpointing design |
| `setup_bootstrap_impl_plan_claude.md` | plan for this setup guide |
