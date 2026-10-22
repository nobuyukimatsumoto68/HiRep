# Handoff: Grid HMC + HiRep multi-channel glueballs (SU(2))

Status as of 2026-10-22. Written for the incoming collaborator and their Claude agents; read this
first, then `README_claude.md` (install details), `checkpointer_impl_plan_claude.md` (checkpoint
design), `glueball_mass_test_impl_plan_claude.md` (small-lattice mass test: estimators, results)
and `glueball_generator_audit_claude.md` (operator generator: audit and regeneration recipe).

## 1. Goal

Generate pure-gauge SU(2) configurations with **Grid's HMC** and, in the same process, measure
**HiRep's glueball (and torelon) 1-point functions** per timeslice in many symmetry channels. No
configuration files are written and re-read between generation and measurement. The 1-point
functions $O_i(t)$ are the input for offline 2-point correlators
$C_{ij}(\Delta t) = \langle O_i(t+\Delta t) O_j(t) \rangle - \langle O_i \rangle \langle O_j \rangle$
and a variational (GEVP) analysis per channel.

## 2. How it is put together

```
HiRep driver (C)  PureGauge/hmc_glueballs_grid_claude.c
  setup_process()                  HiRep owns MPI_Init / MPI_Finalize
  grid_hmc_init(...)               -> Grid (C++) via extern "C" bridge, opaque struct HmcState*
  loop n = start+1 .. start+n_traj
    grid_hmc_step(S, buf, metro)   Grid HMC trajectory, gauge field read out into buf
    copy_to_ugauge(buf)            buf -> HiRep u_gauge (+ halo exchange, BCs)
    assert(|plaq_HiRep - plaq_Grid| < 1e-6)
    every ObsInterval: measure_1pt_glueballs / measure_1pt_torellons
    grid_hmc_save_checkpoint(S, n) ckpoint_{lat,rng}.n, previous pair pruned
```

- Bridge sources: `GridBridge_claude/grid_hirep_hmc_claude.{cpp,h}`, compiled separately into
  `GridLibrary/grid_hirep_hmc_claude.o` with Grid's own `grid-config` flags, then linked by HiRep's
  ninja build with `LINK = $CXX` (custom block "PureGauge with Grid I/O" in `build_claude.pl`).
- HMC: `MinimumNorm2` (Omelyan) integrator, single timescale, Wilson fundamental + adjoint action
  ($\beta_F$, $\beta_A$), $\delta\tau = $ `trajL/nMD`. The first `NoMetropolisUntilRoutine`
  trajectories are MD-only (thermalization).
- Direction convention: Grid is $(x,y,z,t)$, HiRep is $(t,x,y,z)$; the readout maps Grid $\mu$ to
  HiRep slot $(\mu+1) \bmod 4$. Already done in the bridge, so drivers need no change.

## 3. Validated state (2026-10-19, local workstation)

Run: $8^3 \times 32$, `NP_T=4` (4 MPI ranks), $\beta_F = 2.4$, $\beta_A = 0$, `nMD=16`, `trajL=1`,
hot start, `ObsInterval=10`, input `PureGauge/input_hmc_glueballs_grid_100_claude`.

| Check | Fresh run (traj 1..100) | Resume (`CheckpointStart=100`, traj 101..200) |
|-------|-------------------------|-----------------------------------------------|
| Exit code / wall time | 0 / 82 s | 0 / 83 s |
| Acceptance (Metropolis traj) | 83/90 = 0.92 | 95/100 = 0.95 |
| $\langle e^{-\Delta H} \rangle$ | 1.048 (includes early thermalization) | 1.006 |
| $\max \vert P_\text{HiRep} - P_\text{Grid} \vert$ | 0 at printed precision (10 digits) | same |
| $\langle P \rangle$, last 50 traj | 0.6302 | 0.6308 |
| Measurements | traj 10, 20, ..., 100 | traj 110, ..., 200 |
| Checkpoints left | `ckpoint_*.100` | `ckpoint_*.200` |

The plaquette is in line with the standard SU(2) value at $\beta = 2.4$ ($\approx 0.630$). Each
measurement prints 15 operators per timeslice, and the rows now differ between timeslices (see
section 6, item 1, for the bug that was fixed).

### Small-lattice mass test (2026-10-20)

Run dir `runs_claude/small_8x16/` (outputs gitignored), input
`GridBridge_claude/input_hmc_glueballs_grid_small_claude`: $8^3 \times 16$, `NP_T=1` x 4 threads
(one rank, so every timeslice is printed), $\beta_F = 2.4$, `nMD=16`, `trajL=1`, 2000 trajectories,
`ObsInterval=1`, blocking levels 0..2 (45 operators per row), `GridSeed=auto` (got `20261020-131546`).

- 2466 s total, 1.23 s/traj (0.18 s of it the 3-level measurement); acceptance 1892/1980 = 0.956;
  HiRep and Grid plaquettes agree on every trajectory.
- Output 62.5 MB text (32000 rows), checkpoints 2.10 MB (lat) + 0.85 MB (rng).
- **Nontrivial exponent seen** in the single-trace $A_1$ channel at $|\vec P| = 2\pi/L$ (ops 0, 2,
  3, 11, 12, 14 averaged), blocking level 2: $C(1)/C(0) = 0.153(3)$, $C(2)/C(0) = 0.039(3)$,
  $C(3)/C(0) = 0.010(3)$, i.e. $a\,m_\text{eff} = 1.36(7), 1.34(24), 1.3(1.0)$ at $t = 1, 2, 3$.
  The lattice dispersion relation with $a m_{0^{++}} \approx 0.94$-$1.0$ predicts
  $aE \approx 1.17$-$1.22$; the $2\sigma$ excess is plausibly excited states (no GEVP yet) or the
  small box. Figure: `runs_claude/small_8x16/corr_A1_P1_claude.png` (local only).
- **No signal at $\vec P = 0$** ($C(1)/C(0) \le 0.05$ at every level): see section 5 for why.

## 4. Build on a new machine

1. Build Grid with **`--enable-Nc=2`** (must equal HiRep `NG=2` in `MkFlags_claude.ini`; a mismatch
   overflows the transfer buffer and simulates the wrong group). Recipe:
   `GridBridge_claude/build_grid_ubuntu_claude.sh`; check `grep '^Nc' $GRID_BUILD/configure.log`.
2. Build the bridge and HiRep (always pass `GRID_BUILD`; the script's built-in default points at
   a different Grid tree on the original machine):
   ```
   GRID_BUILD=/abs/path/to/grid/build bash GridBridge_claude/build_grid_hirep_claude.sh
   ```
   It aborts if Grid `Nc` != HiRep `NG`, copies `libGrid.a` and `Config.h` into `GridLibrary/`
   (gitignored, never commit them), stages the config, and runs `nj PureGauge_grid`.
3. Run (from the HiRep root):
   ```
   export HWLOC_COMPONENTS=-gl
   mpirun -np 4 PureGauge/hmc_glueballs_grid_claude -i PureGauge/input_hmc_glueballs_grid_100_claude -o out_run < /dev/null
   CheckpointStart=100 mpirun -x CheckpointStart -np 4 ... # resume: runs n_traj MORE trajectories
   ```
   `n_traj` is additive on resume; config numbering is global, so a resume does not re-thermalize.
   Run each job in its own directory (checkpoints go to the working directory and the driver
   deletes the previous pair). Grid's stdout is about 160 KB per trajectory; filter it (see the
   `PATTERN` in the PI's `tmp_claude.sh`: `Total H|dH|etropolis|ccept|unique string|Seed SHA256|MD steps`).
4. Seeding: input key `GridSeed` (glueball driver only). `auto` (default) = date+time string
   `YYYYMMDD-HHMMSS` made on rank 0 and broadcast; any other string is used verbatim, so a logged
   seed reproduces a run. Grid's serial and parallel RNGs get `SeedUniqueString(GridSeed + "_serial")`
   and `(GridSeed + "_parallel")` via `grid_hmc_seed()`. On resume the checkpoint RNG state is used.

### Toolchain pitfalls hit on 2026-10-19 (after an OS upgrade to GCC 15)

- **HiRep's header generator fails with libstdc++ 15.** `Make/Utils/autosun/main.cc` has
  `using namespace std;` and its own `class complex`; libstdc++ 15 forward-declares `std::complex`
  in `bits/stl_pair.h`, so `complex` becomes ambiguous ("reference to 'complex' is ambiguous").
  libstdc++ 12-14 are fine. Workaround used here (no code change): build everything with clang
  pinned to an older libstdc++:
  ```
  GCC13_DIR=/usr/lib/gcc/x86_64-linux-gnu/13
  export OMPI_CC="clang --gcc-install-dir=$GCC13_DIR"
  export OMPI_CXX="clang++ --gcc-install-dir=$GCC13_DIR"
  export BRIDGE_CXX="clang++ --gcc-install-dir=$GCC13_DIR -std=c++17"   # read by build_grid_hirep_claude.sh
  ```
  Needs `libstdc++-13-dev` (or 12/14) and `libomp-<ver>-dev` for clang OpenMP. Pin to the libstdc++
  that your `libGrid.a` was built against. Alternative: patch `autosun/main.cc` to use explicit
  `using std::...` declarations (not done here).
- **GCC 15 compiles C as C23 by default**, which can break older HiRep C code; clang 21 still
  defaults to `gnu17`. If you stay on GCC 15, add `-std=gnu17` to `CFLAGS` in `MkFlags_claude.ini`.
- **`mpirun` hangs before launching any rank** (even `mpirun -np 1 hostname`) with hwloc 2.13: its
  GL plugin probes X displays `:0`, `:1`, ... and one display socket (gnome-shell on Wayland) never
  accepts. Fix: `export HWLOC_COMPONENTS=-gl` before `mpirun`. Only relevant on desktop sessions.
- OpenMP: with clang the executable uses `libomp`, which also provides the `GOMP_*` entry points
  of a GCC-built `libGrid.a`. Check `ldd` shows `libomp` and not also `libgomp`.

### Regenerating the glueball operator code (Mathematica, 2026-10-22)

The operator code (`LibHR/Observables/glueballs_op.c`, `torellons_op.c`,
`Include/Observables/glueballs.h`, `TestProgram/Utils/check_utils_3_{gb,tor}_functions.c`) is
generated by `Make/Utils/Math_glueballs/OpProjection.m` (A. Rago). Background, audit and risks:
`glueball_generator_audit_claude.md`. The notebook is replaced by a headless script:

- `glueball_gen_claude/gen_glueball_ops_claude.wls <stage>/Make/Utils/Math_glueballs <mode>`
  - `github`: the committed CI set (15 glueball + 8 torelon operators);
  - `su2rest`: single-trace $C=+$ operators at rest for SU(2): $A_1^{++}$ (8 loop shapes, the
    notebook's "run" list), $E^{++}$ (4 shapes x 2 components), $T_2^{++}$ (4 shapes x 3
    components), plus the github moving-frame $A_1$, two-glueball and torelon operators
    (38 glueball operators). Edit the `su2rest` block to change the basis.
- `glueball_gen_claude/run_gen_glueball_ops_claude.sh` runs both modes, each in its own **staging
  tree** `glueball_gen_claude/stage_<mode>/` (a mirror of the HiRep layout holding an unmodified
  copy `OpProjection_claude.m`). `GenerateCcode[]` deletes and rewrites its output files, so never
  run it inside the real tree. The runner then compares the `github` output with the committed
  files (ignoring whitespace and braces, since the committed files were clang-formatted with
  `InsertBraces`) and prints the `su2rest` channel table.

  ```
  <wolframscript> -activate          # once per machine, interactive
  WOLFRAMSCRIPT=/path/to/wolframscript bash GridBridge_claude/glueball_gen_claude/run_gen_glueball_ops_claude.sh
  ```
- Verified on the PI's workstation (Mathematica 15.0.1): `github` reproduces all five committed
  files exactly; `su2rest` produces 38 operators, every slot filled and no operator without loop
  terms; each mode takes about 4 s.
- **Not done yet** (plan chunk 5c): copy `stage_su2rest/` files into the real tree on a branch,
  clang-format them (not installed on the PI's workstation), validate on an NG = 3 build
  (`TestProgram/Utils/check_utils_3`, `TestProgram/PureGauge/check_puregauge_5`; both are skipped
  for NG = 2), rebuild NG = 2, rerun the small-lattice test, update the analysis channel map.
- The generated `su2rest` operator code is committed, formatted with clang-format 17 and HiRep's
  `.clang-format` (the same recipe turns the `github` output into the committed files byte for
  byte), in `glueball_gen_claude/stage_su2rest/`: `LibHR/Observables/glueballs_op.c`,
  `Include/Observables/glueballs.h`, `TestProgram/Utils/check_utils_3_gb_functions.c`. The torelon
  files are unchanged from the committed ones. The rest of the staging trees is reproducible in
  seconds and is not committed (HiRep's `.gitignore` re-includes `*.[hc]`, so add files explicitly).

## 5. Output format (`out_*_0`, rank 0 only)

- Channel table at startup: lines `[INIT Measure ML] 1pt_glb Irrep multiplets ...` (from
  `report_gb_group_setup`, `LibHR/Observables/glueballs_op.c:1543`).
- Per measurement: one `[step]` line per local timeslice $t$:
  `t  (Re O_0 +I*(Im O_0))  ...`, i.e. `total_n_glue_op * nblocking` complex numbers, ordered
  `[level][op]`: the first 15 are blocking level `nblkstart` (level 0 = raw links, no smearing),
  the next 15 the next level (APE smear + one spatial blocking step), and so on
  (`LibHR/Observables/glueballs.c:28-49`). Followed by `Glueballs & Torellons 1pt traj <n>` and
  `Plaquette`.
- Operator index -> channel (lattice irrep, momentum, charge conjugation):

| Index | $\vec P$ | Irrep | C |
|-------|----------|-------|---|
| 0, 1 | $(-1,0,0)$ | $A_1$ ($D_{4}$) | + |
| 2 | $(0,-1,0)$ | $A_1$ ($D_{4}$) | + |
| 3 | $(0,0,-1)$ | $A_1$ ($D_{4}$) | + |
| 4 | $(0,0,0)$ | $A_1^{+}$ ($O_h$), **two-glueball** product | + |
| 5 | $(0,0,0)$ | $A_2^{+}$ | + |
| 6, 7 | $(0,0,0)$ | $E^{+}$ (2 components), **two-glueball** products | + |
| 8, 9, 10 | $(0,0,0)$ | $T_1^{+}$ (3 components) | - |
| 11 | $(0,0,1)$ | $A_1$ ($D_{4}$) | + |
| 12 | $(0,1,0)$ | $A_1$ ($D_{4}$) | + |
| 13 | $(0,1,0)$ | $A_2$ ($D_{4}$) | + |
| 14 | $(1,0,0)$ | $A_1$ ($D_{4}$) | + |

**Ops 4, 6, 7 are not single-glueball operators.** In `glueballs_op.c:1189-1205` each is a sum of
products $O_{\vec P = -\hat e_i} \cdot O_{\vec P = +\hat e_i}$ of the moving single-trace operators
(1, 2, 3 with 14, 12, 11), projected to $A_1^{+}$ / $E^{+}$ at total $\vec P = 0$: two-glueball
(scattering) operators. The only single-trace operators at $\vec P = 0$ are op 5 ($A_2^{+}$) and
ops 8-10 ($T_1^{+-}$). **Ops 8-10 are identically zero for SU(2)** ($C = -$ takes
$\mathrm{Im}\,\mathrm{tr}$, and SU(2) traces are real; measured $|O| < 10^{-12}$). **There is no
single-trace $0^{++}$ or $2^{++}$ operator at rest in this set**: it is the generator's minimal CI
test set ("github"), not its production ("run") set; see `glueball_generator_audit_claude.md`.

The operator set is fixed at compile time (`total_n_glue_op 15`, `Include/Observables/glueballs.h:44`);
the code in `glueballs_op.c` is generated by the Mathematica sources in `Make/Utils/Math_glueballs/`.
Changing channels or adding operator shapes means regenerating it there.

## 6. Known issues and open work (prioritized for the multi-channel analysis)

1. **Fixed:** the per-timeslice print loop indexed `one_point_gb[i]` without the slice offset, so
   every row was slice 0. Fixed in `PureGauge/hmc_glueballs_grid_claude.c` (old line kept commented).
   **Data in `out_glueballs_100_claude_0` (July runs) is affected and unusable.** The same bug is
   still in `mk_1pt_glueballs_from_grid_claude.c` and in upstream `PureGauge/mk_1pt_glueballs.c`.
2. **Only rank 0's timeslices are printed.** `lprintf` writes from rank 0 only, so with `NP_T=4`
   the output covers $t = 0..7$ of 32. Options: gather all slices to rank 0 (MPI_Gather on the
   time communicator) before printing, or use HiRep's `collect_1pt_glueball_functions`
   (`glueballs_op.c`), which already gathers across ranks and prints per channel. If you use the
   latter, note `PureGauge/suN_utils.c:225-226` sets `t1` twice (the second assignment probably
   meant `t2`); check before trusting its correlator pairs.
3. **No single-trace $0^{++}$ / $2^{++}$ operator at $\vec P = 0$ in the committed set** (section 5).
   Being fixed by regeneration (PI decision 2026-10-20): the new 38-operator set is generated and
   checked in staging (section 4, "Regenerating the glueball operator code"); installing and
   validating it is the next step. Variational basis: several
   blocking levels now work (`nblkend = 2` on $L = 8$ ran fine; from a reading of
   `spatial_transformations.c` the link length appears to double per level, so presumably
   $2^{\text{nblkend}} < L$, not verified); a GEVP over levels is not written yet.
4. **HDF5 variant writes one file from every rank.** `hmc_glueballs_grid_hdf5_claude.c` has every
   rank `H5Fcreate(..., H5F_ACC_TRUNC)` the same `glueballs.<traj>.h5`, so with `NP_T > 1` ranks
   overwrite each other. Needs a gather or parallel HDF5.
5. Torelon 1-point functions are measured but never printed (also true upstream).
6. Measurement schedule: measurements start at traj `ObsInterval` and are gated on the global
   config index; discard those taken during thermalization in the analysis.
7. `mk_1pt_glueballs_from_grid_claude` (reads NERSC checkpoints from disk) uses an older IO bridge
   object (`grid_hirep_io_claude.o`, May 2026); rebuild it with the current flags before use.
8. **Fixed 2026-10-20:** Grid RNG seeds were hard-coded (every fresh run was the same chain); now
   `GridSeed` (section 4). The other two drivers (`hmc_grid_claude`, `..._hdf5_claude`) still use
   the fixed seeds.
9. **Fixed 2026-10-20 (log only):** Grid printed `Number of MD steps : 10` from an unused default
   in `HMCparameters::MD`; the integrator always used `nMD`. `build_hmc` now copies the
   integrator's parameters so the log is right.
10. Analysis: `GridBridge_claude/analyze_glueball_1pt_claude.py` reads the `[step]` rows (needs all
    timeslices, i.e. `NP_T = 1`), builds connected time-averaged correlators per channel and level
    (vev re-subtracted inside each jackknife sample), and prints $C(t)/C(0)$ and $m_\text{eff}$ with
    binned jackknife errors; `--binscan` checks autocorrelation, `--save PREFIX` writes tables.
    Example: `python3 analyze_glueball_1pt_claude.py out_small_8x16_claude_0 --T 16 --nlev 3 --binscan`.
    Not yet: GEVP over blocking levels, plots (planned notebook, chunk 3 of the plan).

Suggested order: 3 (a real $0^{++}$/$2^{++}$ operator at rest), GEVP + plots on top of the analysis
script (10), then 2
(full time extent with `NP_T > 1`) for larger lattices and a production run.

## 7. For Claude agents working in this repo

- Repo conventions (from the PI): files Claude creates or edits get a `_claude` suffix before the
  extension; copy an original to `*_claude.*` before modifying it. HiRep core files that the build
  references by name are the exception (edited in place, e.g. a past fix in
  `LibHR/Observables/polyakov.c`). When replacing a line in a reference implementation, keep the
  old line commented directly above the new one.
- Never edit the Grid source tree; only read `grid-config`, `libGrid.a`, `Config.h` out of it.
- Never set `NDEBUG` in `MkFlags_claude.ini`: the per-trajectory plaquette `assert` is the main
  guard against a broken Grid -> HiRep transfer.
- Never call `Grid_finalize()`; HiRep's `finalize_process()` owns `MPI_Finalize`. Keep
  `NP_X = NP_Y = NP_Z = 1` (only `NP_T > 1` is supported by the spatial-transform module).
- Do not commit `GridLibrary/` contents (`libGrid.a` exceeds GitHub's 50 MB limit) or run outputs
  (`ckpoint_*`, `out_*claude*`, `glueballs.*.h5`); they are gitignored.
- Sanity numbers for a quick regression: section 3 (plaquette about 0.630, acceptance about 0.9 at
  `nMD=16`, `trajL=1`).
