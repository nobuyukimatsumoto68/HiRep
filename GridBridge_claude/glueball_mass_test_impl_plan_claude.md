# Small-lattice mass test: implementation plan

## Sources (methods used)

- APE smearing: M. Albanese et al. (APE Collaboration), Phys. Lett. B192 (1987) 163.
- Spatial blocking of glueball operators: M. Teper, Phys. Lett. B183 (1987) 345.
- Variational / GEVP analysis: M. Luscher and U. Wolff, Nucl. Phys. B339 (1990) 222.
- Torelon (flux tube winding around the lattice) energy, Luscher term:
  M. Luscher, K. Symanzik, P. Weisz, Nucl. Phys. B173 (1980) 365; M. Luscher, Nucl. Phys. B180 (1981) 317.
- Glueball and k-string spectroscopy with smeared/blocked operators (reference numbers):
  B. Lucini, M. Teper, U. Wenger, JHEP 06 (2004) 012, hep-lat/0404008.
- Jackknife error estimate: M. H. Quenouille, Biometrika 43 (1956) 353;
  J. W. Tukey, Ann. Math. Statist. 29 (1958) 614.
- Binning (blocking) of autocorrelated Monte Carlo data: H. Flyvbjerg and H. G. Petersen,
  J. Chem. Phys. 91 (1989) 461.

Implemented in `GridBridge_claude/analyze_glueball_1pt_claude.py`.

## Goal

Show that the Grid-HMC + HiRep pipeline produces correlators with **nontrivial exponential decay**,
i.e. effective masses that plateau at sensible values, on a lattice small enough to run locally.
Two observables, in order of how easy the signal is:

1. **Torelon** (Polyakov loop winding around a spatial direction): light on small $L$, so a long,
   clean exponential; the mass has a parameter-free prediction (below).
2. **Glueballs**: originally planned in "$A_1^{++}$" (op 4), "$E^{++}$" (ops 6, 7), $T_1^{+-}$
   (ops 8-10) at $\vec P = 0$ and $A_1$ at $|\vec P| = 1$ (ops 0-3, 11, 12, 14; op 13 is $A_2$). As found
   below, ops 4, 6, 7 are two-glueball products and ops 8-10 vanish for SU(2); the single-glueball
   channels at rest came only with the regenerated 38-operator set (chunks 5-6).

## Current state (what exists, what does not)

- No correlator is written anywhere. The driver prints only **1-point** functions $O_i(t)$ as text
  `[step]` lines in `out_*_0`, from rank 0 only, so with `NP_T = 4` only $t = 0..7$ of 32.
- Checkpoints (`ckpoint_lat.<n>`, `ckpoint_rng.<n>`) hold gauge field and RNG state only, and only
  the latest pair is kept. Sizes at $8^3 \times 32$: 4.19 MB (lat, full 64-bit $2\times2$ complex
  links) + 1.70 MB (rng).
- Text size of the 1-pt data: about 650 bytes per timeslice row (15 ops x 1 blocking level).
- The full Grid log is the largest output: about 160 KB per trajectory (16 MB per 100 trajectories).
- Torelon 1-pt functions are computed (`measure_1pt_torellons`) but never printed.
- (Fixed 2026-10-20, `GridSeed` key + `grid_hmc_seed`.) Grid RNG seeds were hard-coded
  (now `grid_hirep_hmc_claude.cpp:184,186`); `rlx_seed` only seeds HiRep, so every fresh run was the
  same Markov chain.
- (Fixed 2026-10-20 in `build_hmc`.) Grid printed `Number of MD steps : 10` from an unused default in `HMCparameters::MD`
  (`Grid/qcd/hmc/HMC.h:90`). The integrator really uses `nMD` (verified: 16 kicks of
  $(1-2\lambda)\epsilon$ per trajectory at `nMD = 16`). Cosmetic only.

## Estimators

1-point functions $O_i(t)$ (complex), measured per configuration $c$. Time-averaged correlator,
using time-translation invariance over all $T$ slices:

$$
C_{ij}(\Delta t) = \frac{1}{T} \sum_{t=0}^{T-1} \left\langle O_i^*(t+\Delta t)\, O_j(t) \right\rangle
  - \left\langle O_i^* \right\rangle \left\langle O_j \right\rangle
$$

The vacuum subtraction matters only for $A_1^{++}$ at $\vec P = 0$ (the other channels have
$\langle O \rangle = 0$ by symmetry, which is itself a check). Multiplet components (the 2 of $E$,
the 3 of $T_1$, and the momenta related by cubic rotations) are averaged.

Effective mass, periodic in $T$:

$$
\frac{C(t)}{C(t+1)} = \frac{\cosh\left[m_\text{eff}(t)\,(t - T/2)\right]}{\cosh\left[m_\text{eff}(t)\,(t + 1 - T/2)\right]}
$$

which reduces to $m_\text{eff}(t) = \ln[C(t)/C(t+1)]$ for $t \ll T/2$. With several blocking
levels $b$, solve the GEVP $C(t)\, v = \lambda(t, t_0)\, C(t_0)\, v$ per channel and use
$m_\text{eff}$ of the leading eigenvalue (Luscher-Wolff).

Errors: jackknife over configurations, after binning to absorb HMC autocorrelation (bin size
increased until the error stops growing).

### Expected numbers (to verify against the literature before comparing)

SU(2) at $\beta = 2.4$: $a\sqrt{\sigma} \approx 0.26$ and $a\, m_{0^{++}} \approx 0.9$ to $1.0$
(check against Lucini-Teper-Wenger and the SU(2) tables cited there). Torelon of length $L$:

$$
a\, m_T(L) \approx \sigma a^2 L - \frac{\pi}{3L}
$$

For $L = 8$: $\sigma a^2 L \approx 0.55$, $\pi/(3L) \approx 0.13$, so $a\, m_T \approx 0.42$. A
clean plateau near this value is the target "nontrivial exponent".

Caveat for $A_1^{++}$ on small $L$: a pair of torelons has energy $\approx 2 m_T \approx 0.8$,
below the $0^{++}$ glueball, so on $L = 8$ the lowest $A_1^{++}$ level may be torelon-pair-like.
That is still a nontrivial exponent, but not the glueball mass.

## Proposed run (no MPI gather needed)

- $8^3 \times 16$, `NP_T = 1` (one rank holds all timeslices, so rank-0 printing is complete),
  `nOMP = 4`.
- $\beta_F = 2.4$, $\beta_A = 0$, `nMD = 16`, `trajL = 1` (acceptance about 0.9 at $8^3 \times 32$).
- `ObsInterval = 1` (a measurement costs about 30 ms vs about 0.8 s per trajectory).
- `nblkstart = 0`, `nblkend = 2`, `APEsmear` 0.33: three smearing/blocking levels, so 45 glueball
  operators per row (blocking limits on $L = 8$ to be checked in `spatial_transformations.c`).
- `NoMetropolisUntilRoutine = 20`, about 2000 trajectories, then discard about 200 for
  thermalization. Expected output: about 2000 x 16 x 2 KB, roughly 60 MB of text, plus the Grid log
  (filter it to the HMC summary lines, or it is about 300 MB).

## Implementation chunks

1. **Driver and bridge fixes**
   - print torelon 1-pt rows next to the glueball rows;
   - make the Grid seeds input-driven (new `extern "C"` setter, so the other drivers keep compiling);
   - set `Params.MD = MD` so Grid's log shows the real `nMD`.

   Files: `PureGauge/hmc_glueballs_grid_claude.c`, `GridBridge_claude/grid_hirep_hmc_claude.{cpp,h}`.
2. **Small-lattice input and run script** (you run it; the Grid log is filtered on the fly).
   Files: `GridBridge_claude/input_hmc_glueballs_grid_small_claude`, `tmp_claude.sh`.
3. **Analysis**: parse `[step]` rows into $O_i(t, c)$, build $C(\Delta t)$, binned jackknife,
   effective masses, GEVP over blocking levels.
   Files: one notebook or Julia script (to decide).
4. **Later, for production**: gather all timeslices to rank 0 (or use
   `collect_1pt_glueball_functions`) so `NP_T > 1` works, and binary output (HDF5, one writer).
   Files: drivers, possibly `suN_utils.c` (note the double `t1` assignment at lines 225-226).

## First run results (2026-10-20, `HiRep/runs_claude/small_8x16`)

Run: $8^3 \times 16$, 1 rank x 4 threads, 2000 trajectories in 2466 s (1.23 s/traj, of which
0.18 s is the measurement with 3 blocking levels). `GridSeed = 20261020-131546`. Grid log now
shows `Number of MD steps : 16`, step 0.0625. Acceptance 1892/1980 = 0.956. HiRep and Grid
plaquettes agree on all 2000 trajectories. Output: 62.5 MB text (32000 rows x 45 ops),
checkpoints 2.10 MB (lat) + 0.85 MB (rng).

Analysis (`analyze_glueball_1pt_claude.py`; 1800 configs after 200 thermalization, bins of 20, jackknife,
$m_\text{eff}(t) = \ln[C(t)/C(t+1)]$):

| Channel (ops) | Best level | $C(1)/C(0)$ | $a\,m_\text{eff}$ at $t = 1, 2, 3$ |
|---|---|---|---|
| $A_1$, $\vert\vec P\vert = 1$, plaquette perpendicular to $\vec P$ (1, 2, 3, 11, 12, 14) | 2 | 0.1527(28) | 1.377(73), 1.43(26), 1.5(1.4) |
| $A_1$, $\vec P = -\hat x$, $P_{xy}+P_{xz}$ (0) | 2 | 0.155(5) | 1.39(14), 1.22(39), - |
| $A_2^{+}$, $\vec P = 0$ (5) | 2 | 0.054 | noise |
| $T_1^{+-}$, $\vec P = 0$ (8-10) | - | - | identically zero for SU(2) ($C=-$, $\vert O\vert < 10^{-12}$) |
| "$A_1^{++}$", "$E^{+}$", $\vec P = 0$ (4; 6, 7) | 2 | 0.02, 0.03 | noise |

(Corrected 2026-10-22: a first version averaged op 0 with ops 2, 3, 11, 12, 14 and quoted
1.359(66); op 0 is a different operator, see `glueball_generator_audit_claude.md` section 5.)

Bin-size scan (perpendicular-plaquette $A_1$, $|\vec P| = 1$, level 2, error of $m_\text{eff}(1)$):
0.061 (bin 1), 0.069 (10), 0.073 (20), 0.081 (40), 0.092 (80, only 22 bins). Mild autocorrelation;
quote bin 40, $a\,m_\text{eff}(1) = 1.377(81)$.

**Operator content (important).** In `glueballs_op.c:1189-1205` (15-operator file, commit
`44939eac`), ops 4, 6, 7 are not
single-trace glueball operators: each is a sum of products
$O_{\vec P = -\hat e_i} \cdot O_{\vec P = +\hat e_i}$, i.e. **two-glueball (scattering) operators**
projected to $A_1^{+}$ / $E^{+}$ at total $\vec P = 0$. The only single-trace $\vec P = 0$ operators
are $A_2^{+}$ (op 5) and $T_1^{+-}$ (ops 8-10), whose glueballs are heavy, so no signal there is
expected at these statistics. The set has no single-trace $\vec P = 0$ $A_1^{++}$ or $E^{++}$ operator.

**Moving-frame check.** The lowest state in $A_1$ at $\vec p = (2\pi/L)\hat e_i$ should be the
$0^{++}$ glueball, with lattice dispersion relation

$$
\sinh^2\left(\frac{a E}{2}\right) = \sinh^2\left(\frac{a m}{2}\right) + \sin^2\left(\frac{a p}{2}\right),
\qquad a p = \frac{2\pi}{8}
$$

For $a m_{0^{++}} = 0.94$ to $1.0$ (reference value from memory, to be checked) this gives
$a E \approx 1.17$ to $1.22$. The measured $a E = 1.377(81)$ is about $2\sigma$ above; inverted, it
corresponds to $a m_{0^{++}} \approx 1.20(10)$. Possible reasons: excited-state contamination at $t = 1$ (no GEVP
yet), the small box ($L\sqrt\sigma \approx 2.1$ shifts the glueball mass), or the reference value
(to be checked). So a nontrivial, flat exponent is seen, in the right range, in this channel.

## Chunk 5: regenerate the operator set (decision 2026-10-20: option (b), the Mathematica generator)

Recipe and risks: `glueball_generator_audit_claude.md`. Mathematica 15.0.1 at
`/mnt/hdd_barracuda/mathematica` (installed 2026-10-22; needs one-time activation).

- 5a. Headless driver `glueball_gen_claude/gen_glueball_ops_claude.wls`, two modes, run in staging
  trees `glueball_gen_claude/stage_<mode>/` (copy `OpProjection_claude.m`; the real `LibHR/`,
  `Include/`, `TestProgram/` are never written).
  - `github`: reproduce the committed CI set; compare with the committed files ignoring whitespace
    and braces (clang-format `InsertBraces`). Tests whether Mathematica 15 matches 11.3.
  - `su2rest`: $A_1^{++}$ (8 "run" shapes), $E^{++}$ (`res` 1, 2, 4, 5), $T_2^{++}$ (`res` 3, 4, 5, 6)
    at rest, $C = +$ only, plus the github moving-frame $A_1$, two-trace and torelon operators.

  Files: `glueball_gen_claude/gen_glueball_ops_claude.wls`, `tmp_claude.sh`.
- 5b. Inspect the su2rest output: channel table, operator count, no operator projecting to zero.
  **5a + 5b done 2026-10-22**: `github` reproduces the committed files exactly; `su2rest` gives 38
  operators, none empty (channel table in `glueball_generator_audit_claude.md` section 5).
- 5c. Install the generated files into `LibHR/Observables/`, `Include/Observables/`,
  `TestProgram/Utils/` (they keep their names: the build references them; git keeps the
  originals), validate on NG = 3 (`check_utils_3`, `check_puregauge_5`), rebuild NG = 2, rerun
  the $8^3 \times 16$ test.
- 5d. Analysis: channel maps (`CHANNEL_MAPS` in `analyze_glueball_1pt_claude.py`, keyed by
  `total_n_glue_op`, 15 and 38) done; reading the table from the out file is not done.

### 5c plan (decided 2026-10-22: NG = 3 validation only; NG = 2 rebuild and SU(2) rerun postponed)

> **SUPERSEDED the same day:** the PI dropped the NG = 3 validation and the worktree ("No validation
> is needed. Use NG=2"). The set was installed directly in the NG = 2 tree (commit `ed1ee388`), and
> the check is the same-`GridSeed` rerun of chunk 6d. The text below is kept as a record only.

Install and test in a **separate git worktree** so the working NG = 2 tree (`HiRep/`, with the
Grid-linked SU(2) executables and its `build/`) is not touched.

- 5c-1. Worktree `/mnt/baracuda_14/hirep_workspace/HiRep_ng3_claude` on a new branch
  `glueball-ops-su2rest-claude` from `HiRep-CUDA` HEAD (`git worktree add`, run by the PI).
  Copy the three formatted files from `GridBridge_claude/glueball_gen_claude/stage_su2rest/` over
  `LibHR/Observables/glueballs_op.c`, `Include/Observables/glueballs.h`,
  `TestProgram/Utils/check_utils_3_gb_functions.c` in the worktree. The torelon files are unchanged.
  Files: worktree copies of those three files.
- 5c-2. In the worktree: `Make/write_mkflags.pl -f Make/MkFlags.ini -n 3 -r FUND --mpi --no-ndebug`,
  clang toolchain via `OMPI_CC` / `OMPI_CXX` (as for the bridge), `HWLOC_COMPONENTS=-gl`; build only
  `TestProgram/Utils/check_utils_3` and `TestProgram/PureGauge/check_puregauge_5`; run each directly
  with `mpirun -np 2 <test> -i <test>.in -o out_<test>_claude` (both inputs have `NP_T = 2`; HiRep's
  `test_wrapper.pl` is avoided because it deletes old outputs). Pass = exit code 0.
  Files: `tmp_claude.sh` (handoff), log `tmp_ng3_tests_claude.log`.
- What the tests check: `check_utils_3` compares every operator evaluated on a rotated
  configuration with the generated rotation table (`check_utils_3_gb_functions.c`), i.e. the
  irrep projections; `check_puregauge_5` checks gauge invariance of the measured operators.

Open: branch / worktree names OK? After the tests pass, merging into `HiRep-CUDA` and the NG = 2
rebuild + rerun are separate later steps.

## Chunk 6: fix the shift-phase / blocking bug (audit section 6; requested by PI 2026-10-22)

Goal: moving operators with shifted loop copies get the phase for the actual step length,
$c_N^{(b)} = \exp(i\pi k_N 2^b / L)$ at blocking level $b$. The original `OpProjection.m` (A. Rago)
stays untouched; the fix lives in a committed patched copy used by our generator runner.

- 6a. Patched generator `glueball_gen_claude/OpProjection_claude.m` (copy of `OpProjection.m`):
  - `WriteShift` (`OpProjection.m:924`): for the glueball file only, emit
    `cN = cexp(I*PI*(<number>*gb_shift_unit)/GLB_X);` (torelons are never blocked: unchanged);
  - `eval_time_momentum_glueball_paths` (`OpProjection.m:1306-1312`): emit the `c` assignments
    on every call, not only inside `if (path_storage == NULL)` (cheap: a few `cexp` per call);
  - emit `int gb_shift_unit = 1;` in `glueballs_op.c` and
    `extern int gb_shift_unit; /* lattice sites per loop step = 2^(blocking level) */` in `glueballs.h`.

  Files: `glueball_gen_claude/OpProjection_claude.m`, `run_gen_glueball_ops_claude.sh` (copy the
  patched file into the stages instead of the original).
- 6b. Hand-written HiRep: `LibHR/Observables/glueballs.c`, `measure_1pt_glueballs`: set
  `gb_shift_unit = 1 << level` before evaluating each blocking level (absolute level
  `nblkstart + i`), reset to 1 afterwards. Edited in place (core file referenced by name).

  Files: `LibHR/Observables/glueballs.c`.
- 6c. Regenerate both modes, format with clang-format 17, check that the `github` output differs
  from the committed files only in the `c` lines, `gb_shift_unit`, and the header line; install
  the `su2rest` files (numerically unchanged: it has no shift phases).
- **6a-6c done 2026-10-22.** Regenerated `github` output differs from HEAD only by the `gb_shift_unit`
  definition/declaration and `c0 = cexp(I*PI*((2.)*gb_shift_unit)/GLB_X)` moved out of the one-time
  block; `su2rest` output gains only the variable (no shift phases). Fixed `su2rest` files installed.
- 6d. Rebuild NG = 2 and run the $8^3 \times 16$ test with the new operators and the same `GridSeed`
  (`20261020-131546`): identical configurations, so the plaquettes must reproduce exactly and the
  moving-frame $A_1$ values must agree. Note: new ops 1 / 37 (`yz-y-z`) and old ops 1 / 14
  (`zy-z-y`) are the same plaquette traversed in opposite orientation, so they agree only up to
  rounding (differences of order $10^{-8}$ in the last printed digit); the other shared operators are
  bit-identical.

Not covered: a numerical test that the fixed phases are right (needs a moving operator with
shifts in the measured set, e.g. old op 13, plus an independent evaluation); the NG = 3 rotation
test was dropped by PI decision.

## Decisions (2026-10-20)

1. Analysis: the faster option, a Python Jupyter notebook (numpy; one plot per cell). In practice
   a script was written first, `analyze_glueball_1pt_claude.py`; no notebook yet.
2. Lattice and statistics: $8^3 \times 16$, about 2000 trajectories, `NP_T = 1`.
3. Grid seed: a string key built from date+time. Input key `GridSeed` (default `auto`): with
   `auto`, rank 0 formats `YYYYMMDD-HHMMSS`, broadcasts it, and it is logged; an explicit string
   reproduces a run. New bridge function `grid_hmc_seed(S, seed, cold_start)` seeds the serial and
   parallel Grid RNGs with `SeedUniqueString(seed + "_serial")` / `(seed + "_parallel")` (SHA-256 of
   the string, Grid `Lattice_rng.h`) and redraws the start configuration. `grid_hmc_init` stays
   unchanged, so the other drivers keep their fixed seeds. On resume, the checkpoint RNG state wins.
4. No torelon printing. The torelon prediction above stays as background only; the test targets
   the glueball channels.
