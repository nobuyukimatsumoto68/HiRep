# Small-lattice mass test: implementation plan

## Sources (methods used)

- APE smearing: M. Albanese et al. (APE Collaboration), Phys. Lett. B192 (1987) 163.
- Spatial blocking of glueball operators: M. Teper, Phys. Lett. B183 (1987) 345.
- Variational / GEVP analysis: M. Luscher and U. Wolff, Nucl. Phys. B339 (1990) 222.
- Torelon (flux tube winding around the lattice) energy, Luscher term:
  M. Luscher, K. Symanzik, P. Weisz, Nucl. Phys. B173 (1980) 365; M. Luscher, Nucl. Phys. B180 (1981) 317.
- Glueball and k-string spectroscopy with smeared/blocked operators (reference numbers):
  B. Lucini, M. Teper, U. Wenger, JHEP 06 (2004) 012, hep-lat/0404008.

## Goal

Show that the Grid-HMC + HiRep pipeline produces correlators with **nontrivial exponential decay**,
i.e. effective masses that plateau at sensible values, on a lattice small enough to run locally.
Two observables, in order of how easy the signal is:

1. **Torelon** (Polyakov loop winding around a spatial direction): light on small $L$, so a long,
   clean exponential; the mass has a parameter-free prediction (below).
2. **Glueballs** in $A_1^{++}$ (op 4), $E^{++}$ (ops 6, 7), $T_1^{+-}$ (ops 8-10) at $\vec P = 0$,
   and the $A_1$ operators at $|\vec P| = 1$ (ops 0-3, 11-14).

## Current state (what exists, what does not)

- No correlator is written anywhere. The driver prints only **1-point** functions $O_i(t)$ as text
  `[step]` lines in `out_*_0`, from rank 0 only, so with `NP_T = 4` only $t = 0..7$ of 32.
- Checkpoints (`ckpoint_lat.<n>`, `ckpoint_rng.<n>`) hold gauge field and RNG state only, and only
  the latest pair is kept. Sizes at $8^3 \times 32$: 4.19 MB (lat, full 64-bit $2\times2$ complex
  links) + 1.70 MB (rng).
- Text size of the 1-pt data: about 650 bytes per timeslice row (15 ops x 1 blocking level).
- The full Grid log is the largest output: about 160 KB per trajectory (16 MB per 100 trajectories).
- Torelon 1-pt functions are computed (`measure_1pt_torellons`) but never printed.
- Grid RNG seeds are hard-coded (`grid_hirep_hmc_claude.cpp:178,180`); `rlx_seed` only seeds
  HiRep. Every fresh run is therefore the same Markov chain.
- Grid prints `Number of MD steps : 10` from an unused default in `HMCparameters::MD`
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
- `nblkstart = 0`, `nblkend = 2`, `APEsmear` 0.5: three smearing/blocking levels, so 45 glueball
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

## Open questions

1. Analysis in a Jupyter notebook (Python, one plot per cell) or a Julia script?
2. Lattice and statistics: $8^3 \times 16$ with about 2000 trajectories, or something else?
3. Grid seed: derive from `rlx_seed`, or a separate input key?
4. Include the torelon printing (chunk 1), which gives the cleanest first signal?
