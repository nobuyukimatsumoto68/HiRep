# Grid-in-HiRep bridge — build & install

Self-contained setup for driving Grid's pure-gauge HMC from HiRep and measuring
observables (plaquette, glueballs) on the generated configs. This directory holds the
single source of truth: the `extern "C"` bridge sources, the HiRep build config, and the
build scripts. Everything is copied into the HiRep tree by `build_grid_hirep_claude.sh`.

## Critical requirement: Grid `Nc` must equal HiRep `NG`

Grid and HiRep are compiled with a fixed number of colours. **They must match.** The gauge
field is transferred through a flat buffer sized `2*NG*NG` doubles per site per direction on
the HiRep side; the Grid readout writes `2*Nc*Nc`. If `Nc != NG` the readout overruns the
buffer -> segfault, and the physics is the wrong gauge group. On $8^3\times32$ at
$\beta_F=2.4$ this shows up as `Total H` $\approx 727450$ for SU(4) vs $\approx 334234$ for
SU(2). This project targets **SU(2): `NG = 2`, so build Grid with `--enable-Nc=2`.**

## Prerequisites (system libraries)

Grid needs: GMP, MPFR, HDF5 (with C++), OpenMPI (with `mpi-cxx`), FFTW (double + float),
c-lime, OpenSSL. Install recipes are in the commented header of
`build_grid_ubuntu_claude.sh`. On this machine they live under
`/mnt/hdd_barracuda/opt/{openmpi_cuda,myhdfstuff/hdf5-2.1.0}` etc. — the paths baked into
`MkFlags_claude.ini` `GRIDLIBS` and the Grid `CXXFLAGS/LDFLAGS`. Adjust for your box.

## Step 1 — clone the Grid fork and build with `Nc=2`

```bash
cd /where/you/want/grid            # a real path; do NOT rely on symlinks
bash /path/to/HiRep/GridBridge_claude/build_grid_ubuntu_claude.sh
```

`build_grid_ubuntu_claude.sh`:
- clones upstream `https://github.com/paboyle/Grid.git` (if absent; no fork needed),
- bootstraps, configures into `./build` with `--enable-simd=AVX --enable-gen-simd-width=256
  --enable-comms=mpi-auto --enable-openmp --enable-Nc=2 --disable-gparity
  --disable-fermion-reps`, then `make -j` + `make install`.

After it finishes, the Grid `--prefix` build dir (call it `$GRID_BUILD`, e.g.
`/where/you/want/grid/build`) holds `grid-config`, `lib/libGrid.a`,
`include/Grid/Config.h`, `configure.log`. Verify the colour count:

```bash
grep '^Nc' $GRID_BUILD/configure.log      # must print: Nc : 2
```

## Step 2 — build the bridge and HiRep

```bash
GRID_BUILD=/where/you/want/grid/build \
    bash /path/to/HiRep/GridBridge_claude/build_grid_hirep_claude.sh
```

This script (read-only w.r.t. Grid):
0. resolves `$GRID_BUILD` and **aborts if Grid `Nc` != HiRep `NG`**;
1. derives Grid's exact `grid-config` flags and records them to
   `HiRep/GridLibrary/grid_flags_claude.txt`;
2. compiles `grid_hirep_hmc_claude.cpp` with `grid-config --cxx` (`g++ -std=c++17`) +
   `--cxxflags` — the bridge `.o` MUST use Grid's own flags or it is ABI-incompatible with
   `libGrid.a`;
3. **captures** `libGrid.a` + `Config.h` out of Grid into `HiRep/GridLibrary` (so the HiRep
   build cannot drift when the Grid tree changes);
4. stages `MkFlags_claude.ini`, `build_claude.pl`, `ScriptForMake`, and the input file into
   the HiRep tree;
5. runs `ScriptForMake` (`nj PureGauge_grid`), linking the captured `libGrid.a`.

## Step 3 — run

```bash
cd /path/to/HiRep
export LD_LIBRARY_PATH=/mnt/hdd_barracuda/opt/openmpi_cuda/lib:/mnt/hdd_barracuda/opt/myhdfstuff/hdf5-2.1.0/lib:$LD_LIBRARY_PATH
mpirun -np 4 PureGauge/hmc_grid_claude -i PureGauge/input_hmc_grid_4node_claude -o out_claude
```

`input_hmc_grid_4node_claude`: $8^3\times32$, `NP_T=4` (`NP_X=NP_Y=NP_Z=1`), `nMD=16`,
`trajL=1.0`, `n_traj=10`, `betaF=2.4`, `betaA=0`. Sanity: `Total H before` $\approx 329146$
and the plaquette thermalizes toward $\approx 0.59$.

## Directory contents

| File | Role |
|------|------|
| `README_claude.md` | this file |
| `build_grid_ubuntu_claude.sh` | clone + build Grid fork with `--enable-Nc=2` |
| `build_grid_hirep_claude.sh` | build bridge `.o`, capture Grid libs, build HiRep (`GRID_BUILD=...`) |
| `grid_hirep_hmc_claude.cpp` / `.h` | Grid HMC bridge (`grid_hmc_init/step/finalize`, `extern "C"`) |
| `grid_hirep_io_claude.cpp` / `.h` | Grid NERSC-checkpoint -> HiRep IO bridge |
| `MkFlags_claude.ini` | HiRep build config: `NG=2`, `CXX=mpicxx`, `GRIDLIBS` (captured lib + grid-config flags) |
| `build_claude.pl` | custom `PureGauge_grid` link rules (`LINK=$CXX`) |
| `ScriptForMake_claude` | `cp MkFlags/build.pl` + `nj PureGauge_grid` |
| `input_hmc_grid_4node_claude` | example 4-rank bare-HMC input |
| `input_hmc_glueballs_grid_100_claude` | glueball run input (100 traj, ObsInterval=10) |
| `checkpointer_impl_plan_claude.md` | checkpointer design/decisions |

## Design notes / gotchas

- **`LINK = $CXX`** in `build_claude.pl`: HiRep links these executables with the C++
  compiler (`mpicxx`) because Grid drags in libstdc++/C++ symbols; a plain C link fails.
- **Never call `Grid_finalize()`** — HiRep's `finalize_process()` owns `MPI_Finalize`.
- **`NP_X = NP_Y = NP_Z = 1`** (only `NP_T > 1`), required by the spatial-transform module.
- Grid's `Grid.h` includes `<H5Cpp.h>`, so the bridge compile needs the HDF5 C++ include
  (already in `grid-config --cxxflags`).
- **Direction remap (fixed):** Grid indexes Lorentz as $(x,y,z,t)=(0,1,2,3)$, HiRep as
  $(t,x,y,z)=(0,1,2,3)$. The readout in `grid_hmc_step` stores Grid direction `mu` at HiRep
  slot `hmu = (mu+1) % 4`, so `out[]` is already in HiRep direction order and every driver's
  `copy_to_ugauge` is correct with no change. Verified: HiRep `avr_plaquette` matches Grid's
  `avgPlaquette` to ~1e-8.
- **Transfer assert:** `hmc_grid_claude.c` asserts
  `fabs(avr_plaquette() - grid_hmc_plaquette(S)) < 1e-6` after each `copy_to_ugauge`.
  Requires `NDEBUG` unset in `MkFlags_claude.ini` (it is removed here).

## Checkpointing & resume (glueball driver)

`hmc_glueballs_grid_claude` checkpoints the Grid HMC state every trajectory, using Grid's
conventional NERSC naming in the run dir:
- `ckpoint_lat.<n>` — gauge field (64-bit, IEEE64BIG)
- `ckpoint_rng.<n>` — serial + parallel RNG (exact)

Only the **latest** pair is kept: after writing `<n>`, the previous pair is deleted
(save-before-delete, so a valid checkpoint always exists). Bridge API:
`grid_hmc_save_checkpoint(S,n)` / `grid_hmc_load_checkpoint(S,n)` (via `NerscIO`).

**Resume** with the `CheckpointStart` env var (propagated through mpirun with `-x`):
```bash
CheckpointStart=100 bash run_glueballs_100_claude.sh   # loads ckpoint_*.100, runs 100 MORE configs -> 101..200
```
`n_traj` is **additive**: it runs `n_traj` MORE configs, i.e. `(CheckpointStart+1) .. (CheckpointStart+n_traj)`,
so no input edit is needed on resume. Config indexing is global, so a resumed run does not re-thermalize
(`NoMetropolisUntilRoutine` and `ObsInterval` are gated on the global config index `n`). Missing
`ckpoint_*.<n>` gives a clean `error()` (existence-checked with `access`). Design:
`checkpointer_impl_plan_claude.md`.

Input knobs (all drivers): `coldStart` (1 cold / 0 hot), `NoMetropolisUntilRoutine` (MD-only
thermalization trajectories before Metropolis); glueball driver also `ObsInterval` (measurement
stride). Run-output files (`ckpoint_*`, `out_*claude*`, `glueballs.*.h5`) are gitignored.
