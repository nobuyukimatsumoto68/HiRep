# Fresh-machine bootstrap for the Grid + HiRep environment: implementation plan

> **Scope reduced by the PI (2026-10-22): documentation only.** "Pointing to the git repos and
> building & linking guidance is sufficient." No machine-config / preflight / smoke-test scripts.
> Deliverable: `README_claude.md` rewritten as the setup guide (repos, Grid configure, bridge build,
> what to edit for another machine, linking rules, short first run, troubleshooting), with the
> pre-handoff audit findings folded in. The design below is kept for reference only.

## Goal

The collaborator's Claude agent must be able to set up the whole environment from a fresh clone of
this repo, without the PI's machine-specific knowledge: install or locate dependencies, build Grid
with `Nc = 2`, build the bridge and HiRep, and confirm with a short run that the pipeline is
correct. Every step must be scripted, every decision point documented, and every known failure
mapped to its fix.

## What is machine-specific today (to remove)

| Where | What | Why it breaks elsewhere |
|---|---|---|
| `MkFlags_claude.ini` `GRIDLIBS`, `INCLUDE` | `/mnt/hdd_barracuda/opt/openmpi_cuda`, `.../hdf5-2.1.0`, `/mnt/baracuda_14/.../HiRep/GridLibrary` | absolute PI paths |
| `build_grid_hirep_claude.sh` | `GRID_BUILD` default `/mnt/baracuda_14/grid_mpi/build` (an Nc=4 tree) | wrong tree; path absent elsewhere |
| `run_small_8x16*_claude.sh`, `run_gen_glueball_ops_claude.sh` | `WORKDIR=/mnt/baracuda_14/...`, OpenMPI `PATH`, wolframscript default | absolute PI paths |
| compiler pin | `--gcc-install-dir=/usr/lib/gcc/x86_64-linux-gnu/13` | depends on installed GCC / libstdc++ versions |
| `HWLOC_COMPONENTS=-gl` | needed on the PI's GNOME/Wayland desktop | harmless elsewhere; keep as default |

## Design

1. **One machine config file**, `GridBridge_claude/machine_env_claude.sh` (template committed as
   `machine_env_claude.sh.example`; the real one is generated per machine and not committed).
   It holds every path and toolchain choice: MPI prefix, HDF5 prefix, extra dependency prefixes,
   `GRID_SRC` / `GRID_BUILD`, compiler wrappers (`OMPI_CC`, `OMPI_CXX`, `BRIDGE_CXX`), `HWLOC_COMPONENTS`.
   All build and run scripts `source` it; no absolute paths remain in scripts.
2. **`setup_preflight_claude.sh`**: detects the system (OS, compilers and versions, libstdc++
   versions, MPI wrappers, HDF5 with C++ API, GMP, MPFR, FFTW, LIME, OpenSSL, zlib, hwloc), writes a
   first `machine_env_claude.sh`, and prints a checklist of what is missing with the install command
   for Ubuntu (apt) and a note for module-based clusters. Read-only apart from writing that file.
3. **`MkFlags_claude.ini` generated**, not hand-edited: the bridge build script writes `INCLUDE` and
   `GRIDLIBS` from `grid-config` plus the machine config (the captured `libGrid.a` path is relative to
   the HiRep root).
4. **Toolchain rule, decided automatically by preflight:** if the default libstdc++ is version 15 or
   newer, HiRep's `Make/Utils/autosun` header generator does not compile (see `HANDOFF_claude.md`
   section 4). Either pin an older libstdc++ (clang `--gcc-install-dir`, or `g++-13/14`) or apply the
   portability patch (open question 1). C code is compiled as `gnu17` either way.
5. **`SETUP_claude.md`**: an agent-oriented runbook. Ordered steps, each with the exact command,
   the expected output, and a pass criterion; a troubleshooting table from symptom to fix (autosun
   `complex` ambiguity, mpirun hang, Grid `Nc` != HiRep `NG` (segfault, wrong `Total H`), ABI skew
   between the bridge `.o` and `libGrid.a`, missing `H5Cpp.h`).
6. **Smoke test** `setup_smoketest_claude.sh`: build check, then a short run ($4^3 \times 8$,
   1 rank, 30 trajectories, fixed `GridSeed`): exit code 0, the per-trajectory HiRep/Grid plaquette
   `assert` holds, acceptance above 0.7, plaquette in a sanity window, all timeslices printed. Then
   the analysis script runs on its output. Takes minutes, not hours.

## Files

New: `GridBridge_claude/SETUP_claude.md`, `machine_env_claude.sh.example`,
`setup_preflight_claude.sh`, `setup_smoketest_claude.sh`, `input_smoketest_claude`.
Modified: `build_grid_ubuntu_claude.sh`, `build_grid_hirep_claude.sh`, `MkFlags_claude.ini`
(becomes a template), `run_small_8x16_claude.sh`, `run_small_8x16_su2rest_claude.sh`,
`glueball_gen_claude/run_gen_glueball_ops_claude.sh`, `HANDOFF_claude.md`, `README_claude.md`,
`.gitignore` (machine config). Possibly `Make/Utils/autosun/main.cc` (open question 1).

## Chunks

1. Machine config + preflight. Files: `machine_env_claude.sh.example`, `setup_preflight_claude.sh`.
2. Scripts read the machine config; `MkFlags_claude.ini` generated. Files: the build and run scripts above.
3. Smoke test. Files: `setup_smoketest_claude.sh`, `input_smoketest_claude`.
4. Runbook and doc updates, folding in the pre-handoff audit findings. Files: `SETUP_claude.md`,
   `HANDOFF_claude.md`, `README_claude.md`.
5. Dry run on this machine: preflight + smoke test from a fresh clone in a scratch directory
   (run by the PI), to prove nothing depends on the PI's paths.

## Decisions (PI, 2026-10-22)

1. **Toolchain: no special handling.** Assume the collaborator's machine has a compiler whose
   libstdc++ is older than 15 (or an equivalent workaround) [moot since 2026-10-22: upstream commit
   2a54d8b4, merged into the fork in dd4ec8d7, fixes autosun for libstdc++ 15]. No autosun patch, no automatic pinning;
   the runbook only mentions the symptom and the PI-machine workaround in its troubleshooting table.
2. **Target: HPC cluster with environment modules, no root.** Dependencies come from modules
   (MPI, HDF5 with C++ API, GMP, MPFR, FFTW, OpenSSL, zlib); anything missing (typically c-lime) is
   built under `$HOME` or the project space. Jobs run through the batch system: the MPI launcher is a
   config variable (`MPI_LAUNCH`, e.g. `srun` or `mpirun`), and the smoke test can run in an
   interactive allocation or as a batch job.

## Revised design (supersedes items 2, 4 and 6 above where they conflict)

- `machine_env_claude.sh` (from `machine_env_claude.sh.example`): `module load` lines, dependency
  prefixes (taken from the module environment where possible, e.g. `$HDF5_DIR`), `GRID_SRC`,
  `GRID_BUILD`, `MPI_LAUNCH`, `SIMD` (e.g. `AVX2` / `AVX512` for the cluster CPUs). Sourced by all scripts.
- `setup_preflight_claude.sh`: after `source machine_env_claude.sh`, checks that every tool and
  library is visible (compiler and MPI wrappers, `H5Cpp.h` + libs, gmp, mpfr, fftw, lime, ssl, z),
  prints what is missing. No toolchain decisions.
- Grid build script generalised from `build_grid_ubuntu_claude.sh`: configure flags from the
  machine config (`--enable-Nc=2`, `--enable-simd=$SIMD`, `--enable-comms=mpi-auto`, `--with-*` prefixes),
  build into `$GRID_BUILD`; builds c-lime into a local prefix if it is not available.
- Bridge build: `MkFlags_claude.ini` written from `grid-config` + machine config.
- Smoke test: $4^3 \times 8$, 1 rank, 30 trajectories, fixed `GridSeed`, launched with `$MPI_LAUNCH`;
  pass criteria as above.
- `SETUP_claude.md`: cluster runbook (module discovery commands, where to build, interactive
  allocation vs batch, expected outputs, troubleshooting table).

Open: none blocking. The pre-handoff audit findings will be folded into chunk 4.
