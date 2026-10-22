# Handoff: Grid HMC + HiRep multi-channel glueballs (SU(2))

Status as of 2026-10-22. Written for the incoming collaborator and their Claude agents.
Reading order:

1. this file (status, results, open work);
2. `README_claude.md` (setup: repositories, building and linking Grid + HiRep, first run, troubleshooting);
3. `glueball_generator_audit_claude.md` (glueball operator generator, operator table, shift-phase bug);
4. `glueball_mass_test_impl_plan_claude.md` (small-lattice mass test: estimators, results, history);
5. `checkpointer_impl_plan_claude.md` (checkpointing design).

## 1. Goal

Generate pure-gauge SU(2) configurations with **Grid's HMC** and, in the same process, measure
**HiRep's glueball (and torelon) 1-point functions** per timeslice in many symmetry channels. No
configuration files are written and re-read between generation and measurement. The 1-point
functions $O_i(t)$ are the input for offline 2-point correlators

$$
C_{ij}(\Delta t) = \frac{1}{T} \sum_t \left\langle O_i^*(t+\Delta t)\, O_j(t) \right\rangle - \left\langle O_i^* \right\rangle \left\langle O_j \right\rangle
$$

and a variational (GEVP) analysis per channel (M. Luscher and U. Wolff, Nucl. Phys. B339 (1990) 222).

## 2. How it is put together

```
HiRep driver (C)  PureGauge/hmc_glueballs_grid_claude.c
  setup_process()                  HiRep owns MPI_Init / MPI_Finalize
  grid_hmc_init(...)               -> Grid (C++) via extern "C" bridge, opaque struct HmcState*
  grid_hmc_seed(S, GridSeed, ...)  seed Grid's RNGs from a string, draw the start configuration
  loop n = start+1 .. start+n_traj
    grid_hmc_step(S, buf, metro)   Grid HMC trajectory, gauge field read out into buf
    copy_to_ugauge(buf)            buf -> HiRep u_gauge (+ halo exchange, BCs)
    assert(|plaq_HiRep - plaq_Grid| < 1e-6)
    every ObsInterval: measure_1pt_glueballs / measure_1pt_torellons
    grid_hmc_save_checkpoint(S, n) ckpoint_{lat,rng}.n, previous pair pruned
```

- HMC (S. Duane, A. D. Kennedy, B. J. Pendleton, D. Roweth, Phys. Lett. B195 (1987) 216) with
  Grid's `MinimumNorm2` second-order minimum-norm integrator (I. P. Omelyan, I. M. Mryglod, R. Folk,
  Comput. Phys. Commun. 151 (2003) 272), single timescale; Wilson fundamental + adjoint action
  ($\beta_F$, $\beta_A$); $\delta\tau = $ `trajL/nMD`. The first `NoMetropolisUntilRoutine`
  trajectories are MD-only (thermalization).
- Bridges `GridBridge_claude/grid_hirep_hmc_claude.{cpp,h}` (HMC) and `grid_hirep_io_claude.{cpp,h}`
  (checkpoint reader), compiled with Grid's own `grid-config` flags and linked by HiRep's ninja build
  with `LINK = $CXX`. Building and linking rules: `README_claude.md` section 5.
- Direction convention: Grid is $(x,y,z,t)$, HiRep $(t,x,y,z)$; the readout maps Grid $\mu$ to HiRep
  slot $(\mu+1) \bmod 4$.
- Seeding: input key `GridSeed`. `auto` = date+time string `YYYYMMDD-HHMMSS` made on rank 0,
  broadcast and logged; any other string is used verbatim, so a logged seed replays exactly the same
  Markov chain. Grid's serial and parallel RNGs get `SeedUniqueString(GridSeed + "_serial")` and
  `(GridSeed + "_parallel")`. On resume (`CheckpointStart`) the checkpoint RNG state is used.

## 3. Status and results

### 3.1 Pipeline validation (2026-10-19, PI workstation)

$8^3 \times 32$, `NP_T = 4`, $\beta_F = 2.4$, $\beta_A = 0$, `nMD = 16`, `trajL = 1`, hot start,
input `input_hmc_glueballs_grid_100_claude`.

| Check | Fresh run (traj 1..100) | Resume (`CheckpointStart=100`, traj 101..200) |
|-------|-------------------------|-----------------------------------------------|
| Exit code / wall time | 0 / 82 s | 0 / 83 s |
| Acceptance (Metropolis traj) | 83/90 = 0.92 | 95/100 = 0.95 |
| $\langle e^{-\Delta H} \rangle$ | 1.048 (includes early thermalization) | 1.006 |
| $\max \vert P_\text{HiRep} - P_\text{Grid} \vert$ | 0 at printed precision (10 digits) | same |
| $\langle P \rangle$, last 50 traj | 0.6302 | 0.6308 |

The plaquette agrees with the standard SU(2) value at $\beta = 2.4$ (about 0.630).

### 3.2 Small-lattice mass test with the original 15-operator set (2026-10-20)

`run_small_8x16_claude.sh`, input `input_hmc_glueballs_grid_small_claude`: $8^3 \times 16$,
`NP_T = 1` (every timeslice printed), 4 threads, 2000 trajectories, `ObsInterval = 1`, blocking
levels 0-2, `GridSeed = 20261020-131546`.

- 1.23 s per trajectory (0.18 s of it the measurement); acceptance 1892/1980 = 0.956; HiRep and Grid
  plaquettes agree on every trajectory.
- **Nontrivial exponent:** single-trace $A_1$ at $|\vec P| = 2\pi/L$, plaquette perpendicular to
  $\vec P$ (one operator in the six directions $\pm \hat e_i$), blocking level 2:
  $C(1)/C(0) = 0.1527(28)$, $a\,m_\text{eff}(t=1) = 1.377(81)$ (binned jackknife, bin 40; flat within
  errors to $t = 3$). The lattice dispersion relation
  $\sinh^2(aE/2) = \sinh^2(am/2) + \sin^2(ap/2)$ turns it into $a m_{0^{++}} \approx 1.20(10)$.
  A reference value $a m_{0^{++}} \approx 0.94$-$1.0$ at $\beta = 2.4$ (quoted from memory, **not
  checked**) would predict $aE \approx 1.17$-$1.22$, about $2\sigma$ below; possible reasons:
  excited states (no GEVP yet), the small box ($L\sqrt\sigma \approx 2$), or the reference value.
- **No signal at $\vec P = 0$**: that set has no single-glueball operator at rest (section 5.2).

### 3.3 Current operator set (installed, commit `ed1ee388`)

The 38-operator `su2rest` set (section 5.1) was generated with the patched Mathematica generator
(section 4) and is installed in `LibHR/Observables/glueballs_op.c` and
`Include/Observables/glueballs.h`. It adds single-glueball $A_1^{++}$ (8 loop shapes), $E^{++}$ and
$T_2^{++}$ operators at rest. Validation status:

- generated code checked (all 38 slots written, no empty operator; spot check of the $E^{++}$
  plaquette doublet); Mathematica 15 reproduces the original 15-operator files exactly;
- HiRep's generator tests (`TestProgram/Utils/check_utils_3`, `TestProgram/PureGauge/check_puregauge_5`,
  NG = 3 only) were **not run** (PI decision);
- **Same-chain rerun (2026-10-22, done):** `run_small_8x16_su2rest_claude.sh` reran the
  $8^3 \times 16$ test of 3.2 with the new set and the same `GridSeed` (2000 trajectories, 12501 s;
  about 5.6 s per trajectory when the machine is otherwise idle, 2.5 s of it the single-threaded
  measurement; about 78 KB of output per trajectory). All 2000 plaquettes are identical to 3.2, and
  of the 672000 shared moving-frame operator values 671860 are bit-identical; the other 140 (ops 1 / 37,
  the same plaquette traversed in opposite orientation) differ by at most $10^{-8}$. So the new code
  reproduces the old measurements wherever they overlap.

**Results at rest** (run dir `runs_claude/small_8x16_su2rest`, local only; 1800 configurations after
200 for thermalization, blocking level 2, binned jackknife with bin 20, errors about 10 % larger with
bin 40; single-operator effective masses $m_\text{eff}(t) = \ln[C(t)/C(t+1)]$, no GEVP yet):

| Channel (loop shapes `res[k]`) | $C(1)/C(0)$ | $a\,m_\text{eff}(t=1)$ | $a\,m_\text{eff}(t=2)$ |
|---|---|---|---|
| $A_1^{++}$ (3, 5, 8) | 0.16-0.23 | 0.84-0.88 ($\pm 0.09$-$0.11$) | 0.97-1.12 ($\pm 0.29$-$0.31$) |
| $A_1^{++}$ (1, 11, 12) | 0.14-0.28 | 1.00-1.03 ($\pm 0.10$-$0.14$) | 1.01-1.06 ($\pm 0.27$-$0.39$) |
| $E^{++}$ (1, 2, 4) | 0.08-0.14 | 1.47-1.51 ($\pm 0.13$-$0.24$) | noisy |
| $T_2^{++}$ (3, 4) | 0.08-0.10 | 1.57-1.69 ($\pm 0.22$-$0.23$) | noisy |
| two-glueball products ($A_1^{+}$, $E^{+}$) | about 0.02 | noise | noise |
| moving frame $A_1$, $\vert\vec P\vert = 2\pi/L$ (3.2) | 0.1527(28) | 1.377(73) | 1.43(26) |

- $E^{++}$ and $T_2^{++}$ agree within errors, as they should: both carry the $2^{++}$ glueball.
- $A_1^{++}$ at rest ($\approx 0.85$-$1.0$) lies below the $a m_{0^{++}} \approx 1.20(10)$ inferred from
  the moving frame. On this small box ($L\sqrt\sigma \approx 2$) a torelon pair (estimated
  $2 a m_T \approx 0.8$, `glueball_mass_test_impl_plan_claude.md`) can be the lowest $A_1^{++}$ state,
  so the rest-frame number may not be the glueball. A GEVP over shapes and levels, the torelon
  channel, and a larger $L$ are needed to separate them. Literature values to compare with are still
  to be looked up (the $0^{++}$ value quoted in 3.2 is from memory).
- Per-channel tables: `jk_<channel>_lev<k>.txt` in the run dir; full output in
  `tmp_small_8x16_su2rest_claude.log` on the PI's machine.

## 4. Setup and regeneration

**Setup** (repositories, Grid with `--enable-Nc=2`, machine config `MKFLAGS`, bridge + HiRep build,
linking rules, first run, troubleshooting): `README_claude.md`. Do not copy the PI's machine files
(`MkFlags_claude.ini`, `build_grid_ubuntu_claude.sh`, the `run_small_8x16*` scripts' paths); use the
examples `MkFlags_example_claude.ini`, `build_grid_example_claude.sh`, `run_example_slurm_claude.sh`.

PI-machine notes (only if you hit the same problems): after an OS upgrade to GCC 15, HiRep's header
generator `Make/Utils/autosun` stopped compiling (its `class complex` clashed with libstdc++ 15). That
is now fixed upstream (`2a54d8b4`, merged into this fork in `dd4ec8d7`), so a current clone builds
with GCC 15. The PI's machine still builds with clang 21 pinned to the GCC 13 libstdc++ (`OMPI_CC` /
`OMPI_CXX` = `clang[++] --gcc-install-dir=/usr/lib/gcc/x86_64-linux-gnu/13`, `BRIDGE_CXX` for the
bridges), the setup the results above were produced with; a plain GCC 15 build there has not been
tried yet. The PI also sets `HWLOC_COMPONENTS=-gl` because hwloc's GL plugin hung `mpirun` on the
desktop session.

**Regenerating the glueball operator code.** `glueballs_op.c`, `torellons_op.c`, `glueballs.h` and
`TestProgram/Utils/check_utils_3_{gb,tor}_functions.c` are generated by A. Rago's Mathematica package
`Make/Utils/Math_glueballs/OpProjection.m`. Always use the **patched copy**
`glueball_gen_claude/OpProjection_claude.m` (shift-phase fix, section 6 item 3):
`LibHR/Observables/glueballs.c` uses the variable `gb_shift_unit` that only the patched generator
defines, so regenerating with the original package or the notebook gives a link error.

```bash
<wolframscript> -activate                              # once per machine, interactive
WOLFRAMSCRIPT=/path/to/wolframscript CLANG_FORMAT=/path/to/clang-format \
    bash GridBridge_claude/glueball_gen_claude/run_gen_glueball_ops_claude.sh
```

- `gen_glueball_ops_claude.wls` (headless replacement of the notebook) has two modes: `github`
  (the original 15-operator CI set, to check a Mathematica version against commit `44939eac`) and
  `su2rest` (the installed set; edit its block to change the basis). Each runs in a staging tree
  `glueball_gen_claude/stage_<mode>/`, because `GenerateCcode[]` deletes and rewrites its outputs.
- `CLANG_FORMAT` = clang-format 17 (`pip install clang-format==17.0.6`); with HiRep's
  `.clang-format` it reproduces the committed formatting byte for byte. Regenerating overwrites the
  tracked `stage_su2rest/` files: format before committing.
- To install a regenerated set, copy `stage_su2rest/{LibHR,Include,TestProgram}` files over the
  real ones and rebuild. For SU(2) use $C = +$ operators only (section 5.2).

## 5. Output format and operators

Output `out_<name>_0`, written by rank 0 only:

- channel table at startup: `[INIT Measure ML] 1pt_glb Irrep multiplets ...`
  (`report_gb_group_setup`, `LibHR/Observables/glueballs_op.c:12224`);
- per measurement, one `[step]` line per local timeslice $t$:
  `t  (Re O_0 +I*(Im O_0))  ...`, i.e. `total_n_glue_op * nblocking` complex numbers ordered
  `[level][op]`: the first `total_n_glue_op` are blocking level `nblkstart` (level 0 = raw links),
  the next ones the next level (APE smear + one spatial blocking step), and so on
  (`LibHR/Observables/glueballs.c:10-56`); then `Glueballs & Torellons 1pt traj <n>` and `Plaquette`.

### 5.1 Installed set: 38 operators (`total_n_glue_op 38`, `Include/Observables/glueballs.h:45`)

| Indices | $\vec P$ | Channel |
|---|---|---|
| 1 / 2 / 3 / 35 / 36 / 37 | $-\hat x$ / $-\hat y$ / $-\hat z$ / $+\hat z$ / $+\hat y$ / $+\hat x$ | $A_1$ ($D_4$): plaquette perpendicular to $\vec P$ |
| 0 | $-\hat x$ | $A_1$ ($D_4$): $P_{xy} + P_{xz}$ (planes containing $\vec P$) |
| 4-11 | 0 | $A_1^{++}$, 8 loop shapes |
| 12 | 0 | $A_1^{+}$ two-glueball product |
| 13-16 / 18-21 | 0 | $E^{++}$ components 1 / 2, 4 shapes |
| 17 / 22 | 0 | $E^{+}$ two-glueball products |
| 23-34 | 0 | $T_2^{++}$, 4 shapes x 3 components (23, 27, 31 = shape 1, ...) |

Shapes and details: `glueball_generator_audit_claude.md` section 5. The analysis script's
`CHANNEL_MAPS` (keyed by `total_n_glue_op`) groups these into channels.

### 5.2 Original 15-operator set (commit `44939eac` and earlier; historical)

The generator's minimal CI ("github") set: $A_1$ / $A_2$ at $|\vec P| = 1$, $A_2^{+}$ (op 5) and
$T_1^{+-}$ (ops 8-10) at rest, and ops 4, 6, 7, which are **two-glueball** products
$O_{-\hat e_i} \cdot O_{+\hat e_i}$ projected to $A_1^{+}$ / $E^{+}$. It has no single-glueball
$0^{++}$ / $2^{++}$ operator at rest. **$C = -$ operators (ops 8-10) are identically zero for SU(2)**
($C = -$ takes $\mathrm{Im}\,\mathrm{tr}$ of the loops, and SU(2) traces are real).

## 6. Known issues and open work

1. **Only rank 0's timeslices are printed.** With `NP_T > 1` the output covers rank 0's slices
   only; use `NP_T = 1` until a gather exists. Options: gather all slices to rank 0 before printing, or
   HiRep's `collect_1pt_glueball_functions` (in `glueballs_op.c`), which gathers and prints per
   channel; if you use it, note `PureGauge/suN_utils.c:225-226` sets `t1` twice (the second
   assignment probably meant `t2`).
2. **No GEVP yet.** The variational analysis over loop shapes and blocking levels is the next step
   for the $0^{++}$ / $2^{++}$ masses at rest. Blocking levels: `nblkend = 2` on $L = 8$ ran fine;
   the link length doubles per level, so presumably $2^{\text{nblkend}} < L$ (not verified).
3. **Fixed 2026-10-22: shift phases of moving operators at blocking levels >= 1.** The generator
   hard-coded momentum shift phases for 1-site steps; spatial blocking makes a step $2^b$ sites.
   Affected only $\vec P \ne 0$ operators with `c` phase constants (op 13 of the 15-operator set);
   $\vec P = 0$ operators, torelons and the installed 38-operator set never were. Fix: patched generator
   (phases scale with `gb_shift_unit`) + `glueballs.c` setting `gb_shift_unit = 1 << level`
   (`LibHR/Observables/glueballs.c:32,49,54`). Details: `glueball_generator_audit_claude.md` section 6.
   Not tested numerically (the installed set has no shifted operator).
4. **Fixed 2026-10-19:** the per-timeslice print loop printed slice 0 for every $t$; the July data
   (`out_glueballs_100_claude_0`) is unusable. The same bug remains in
   `PureGauge/mk_1pt_glueballs_from_grid_claude.c` and upstream `PureGauge/mk_1pt_glueballs.c`.
5. **HDF5 variant writes one file from every rank:** `hmc_glueballs_grid_hdf5_claude.c` has every rank
   `H5Fcreate(..., H5F_ACC_TRUNC)` the same `glueballs.<traj>.h5`; with `NP_T > 1` ranks overwrite each
   other.
6. Torelon 1-point functions are measured but never printed (also true upstream).
7. Measurements start at trajectory `ObsInterval` (global index); discard thermalization in the
   analysis. The first 200 measurements are dropped by default (`--ntherm`).
8. Only the glueball driver has `GridSeed` and checkpointing; `hmc_grid_claude` and the HDF5 driver
   use fixed Grid seeds.
9. Measurement cost: the 38-operator set takes about 14 times longer to measure than the 15-operator
   set (240 vs 17 loop paths, single-threaded HiRep code); consider `ObsInterval > 1` or fewer shapes
   for long runs.

## 7. For Claude agents working in this repo

- Repo conventions (from the PI): files Claude creates or edits get a `_claude` suffix before the
  extension; copy an original to `*_claude.*` before modifying it. HiRep core files that the build
  references by name are the exception (edited in place, e.g. `LibHR/Observables/glueballs.c`).
  When replacing a line in a reference implementation, keep the old line commented directly above
  the new one. ASCII only in files (math in LaTeX); cite published methods in plans and code.
- Never edit the Grid source tree; only read `grid-config`, `libGrid.a`, `Config.h` out of it.
- Never set `NDEBUG`: the per-trajectory plaquette `assert` is the main guard against a broken
  Grid -> HiRep transfer.
- Never call `Grid_finalize()`; HiRep's `finalize_process()` owns `MPI_Finalize`. Keep
  `NP_X = NP_Y = NP_Z = 1`.
- Do not commit `GridLibrary/` contents (`libGrid.a` exceeds GitHub's 50 MB limit) or run outputs
  (`ckpoint_*`, `out_*claude*`, `glueballs.*.h5`, `runs_claude/`). HiRep's `.gitignore` re-includes
  `*.[hc]`, so add files explicitly rather than with `git add -A`.
- Sanity numbers for a quick regression: section 3.1 (plaquette about 0.630, acceptance about 0.9
  at `nMD = 16`, `trajL = 1`).
