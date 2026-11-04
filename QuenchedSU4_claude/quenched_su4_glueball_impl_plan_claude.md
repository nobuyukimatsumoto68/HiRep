# Quenched SU(4) glueballs: HiRep heatbath + in-process glue operators -> one h5 per stream

Status: Chunks 1-4 DONE; stream 1 segment 2 RUNNING since 2026-11-04 17:15 (100 measurements, flow every 5th; machine load ~26 -> ETA ~60 h). Next: h5 analysis script. Repo: /mnt/baracuda_14/hirep_su4 (branch su4).

## Algorithm sources

- Pure-gauge update: Cabibbo-Marinari SU(2)-subgroup heatbath, N. Cabibbo and E. Marinari,
  Phys. Lett. B119 (1982) 387; SU(2) heatbath sampler in `LibHR/Random/random_su2.c` (Creutz,
  Phys. Rev. D21 (1980) 2308, or Kennedy-Pendleton, Phys. Lett. B156 (1985) 393 -- TO VERIFY which
  one HiRep implements before citing in code); overrelaxation, M. Creutz, Phys. Rev. D36 (1987) 515;
  F. R. Brown and T. J. Woch, Phys. Rev. Lett. 58 (1987) 2394. HiRep: `LibHR/Update/update_hb.c`,
  `cabmar.c` (C. Pica).
- Glueball operators: Wilson loops projected onto $O_h$ irreps (B. Berg and A. Billoire, Nucl. Phys.
  B221 (1983) 109; R. C. Johnson, Phys. Lett. B114 (1982) 147); moving frames on the little groups
  $D_4$, $D_2$, $D_3$; generator `OpProjection.m` (A. Rago), patched copy
  `HiRep/GridBridge_claude/glueball_gen_claude/OpProjection_claude.m` (shift-phase / blocking fix).
- APE smearing (M. Albanese et al., Phys. Lett. B192 (1987) 163) + spatial blocking (M. Teper,
  Phys. Lett. B183 (1987) 345), as in `LibHR/Observables/glueballs.c`.
- Analysis later: GEVP, M. Luscher and U. Wolff, Nucl. Phys. B339 (1990) 222.

## Goal / physics summary

Generate quenched SU(4) Wilson-action ensembles with HiRep's heatbath + overrelaxation and, in the
same process, measure the glueball and torelon 1-pt functions $O_i(t)$ (all timeslices, all
blocking levels). Store only:

1. one h5 file per stream (appended after every measurement, backup copy made before each append);
2. the latest gauge configuration + RNG state (HiRep format), for resuming.

No meson measurement yet. Grid is not used in this part (HB+OR is HiRep-native).

Parameters (user, 2026-10-27):

- $24^3 \times 48$, $N_c = 4$, Wilson action, $\beta = 11.045$ (same as the dynamical m0.4 ensemble
  `b11.045`). Normalisation is the same in both codes: Grid `WilsonGaugeAction`
  $S = \beta \sum_P (1 - \frac{1}{N} \mathrm{Re\,tr}\,U_P)$, HiRep $S = -\frac{\beta}{N} \sum_P
  \mathrm{Re\,tr}\,U_P$ + const. (Quenched at the same bare $\beta$ is not the same lattice spacing as
  the dynamical ensemble.)
- Operators: C=+ and C=-, at rest and in moving frames; torelons, also in moving frames.
- Run locally (i9-10900X, 10 cores / 20 hardware threads, 61 GB RAM).

## Cost estimate (to be replaced by the Chunk 4 benchmark)

$V = 24^3 \times 48 = 663552$ sites.

- **Update** (1 HB + 4 OR sweeps): about 40 kflop per site per sweep for SU(4) (12 4x4 matmuls for the
  staples + 6 Cabibbo-Marinari subgroups), so about $1.3 \times 10^{11}$ flop per update. Scalar HiRep C
  at 1.5-3 Gflop/s per core: **45-90 core-s per update**.
- **Measurement**: scaled from the measured SU(2) $8^3 \times 16$ run (2.5 s single-rank, 38 ops, levels
  0-2): volume x81, SU(4)/SU(2) matrix cost x8, larger op set x~1.5 (C=- reuses the same loops via
  Im tr, so cost follows the number of loop shapes and rotations, not the number of irreps):
  **about 2400 core-s per measurement** (range 1200-4000). Measurement code has no OpenMP; it
  parallelises only over the timeslices of each rank.
- MPI layout: only `NP_T` may be > 1 (HiRep spatial-transform module), `NP_T | 48`.

| Layout | slices/rank | update | measurement |
|---|---|---|---|
| `NP_T=8`, 1 rank/core (recommended) | 6 | 6-11 s | ~300 s (150-500) |
| `NP_T=16` on hyperthreads | 3 | ~5-9 s | ~240 s (HT gain ~1.2x, uncertain) |
| `NP_T=4` (local test limit, user) | 12 | 11-22 s | ~600 s |
| `NP_T=24` (tuolumne CPU) | 2 | 2-4 s | ~100 s |

So the measurement dominates. With `ObsInterval = N` updates between measurements, one cycle costs
about $8N + 300$ s with `NP_T=8`: N=1 gives about 280 measurements/day, N=10 about 230/day (far less
correlated). Choose N after the benchmark and after a look at the autocorrelation.

- Memory: one SU(4) double-precision gauge field = 680 MB; with APE/blocking work fields a few GB in
  total. Path storage of the generated code must be checked in the benchmark.
- Disk: checkpoint about 0.7 GB (only the latest is kept). h5: about 0.25 MB per measurement
  (~80 ops x 4 levels x 48 t x 16 B + torelons), so about 250 MB per 1000 measurements (x2 with the
  backup). Copying the backup each time costs about 1 s per GB, small next to the measurement.

## Design

### h5 layout (one file per stream, `glue_<stream>_claude.h5`)

```
/traj            int64   (N)                      update index of each measurement
/plaq            float64 (N)
/glue            float64 (N, nlev, n_glue_op, T, 2)   re/im, levels nblkstart..nblkend
/tor             float64 (N, n_tor_op, T, 2)
attrs: NG, beta, L, T, nhb, nor, ObsInterval, nblkstart, nblkend, APEsmear, rlx_seed,
       op/channel table (strings from report_gb_group_setup), generator mode + HiRep commit
```

Datasets are chunked and extendable along axis 0; rank 0 writes with the HDF5 C API (HighFive is C++
only). Each rank holds only its own timeslices, so rank 0 gathers all T slices (MPI gather along
T) before writing.

### Order of operations at a measurement point (traj n)

1. measure, gather to rank 0;
2. `cp glue.h5 glue.h5.bak` (rank 0, plain file copy in C);
3. open, extend by one row, write, close;
4. write checkpoint `conf.<n>` + `rng.<n>` (HiRep `write_gauge_field` + `write_ranlxd_state`);
5. delete the previous checkpoint pair only after step 4 succeeds (save-before-delete, as in the
   Grid-bridge driver).

Checkpoints are written only at measurement points, so the h5 and the checkpoint always agree.
**Resume** (`CheckpointStart=<n>`): load conf + rng; if the h5 has rows with traj > n (crash between
3 and 4), shrink it back to traj <= n; if the h5 cannot be opened, restore from `.bak` (and then shrink).
The RNG restore needs the same `NP_T` as the run that wrote it (to check).

## Files

New / modified (all `_claude`; repo /mnt/baracuda_14/hirep_su4; SU(4)-specific files in `QuenchedSU4_claude/`, driver in `PureGauge/`):

- `glueball_gen_claude/gen_glueball_ops_claude.wls` (new mode `su4`), output tree `stage_su4/`
- generated `LibHR/Observables/glueballs_op.c`, `torellons_op.c`, `Include/Observables/glueballs.h`,
  `TestProgram/Utils/check_utils_3_{gb,tor}_functions.c` (in the NG=4 tree only)
- `PureGauge/suN_glueballs_h5_claude.c` (new driver: HiRep `update()` + `measure_1pt_glueballs` /
  `measure_1pt_torellons` + h5 append + checkpoint)
- `Make/MkFlags_su4_claude.ini` (NG=4, no Grid), build rule in `Make/build_claude.pl`
- inputs `input_su4_*_claude`, run scripts `run_su4_*_claude.sh`
- analysis: h5 reader added to `analyze_glueball_1pt_claude.py` (or a new `_su4` copy)

## Chunks

### Chunk 0 -- where the NG=4 code lives
Decide Q1. Files: none (decision only).

### Chunk 1 -- operator set for SU(4)
New `su4` mode in the generator: C=+ and C=- at rest, moving frames, torelons at rest + moving.
Run in `stage_su4/`, clang-format, install into the NG=4 tree, check that all slots are written.
Files: `gen_glueball_ops_claude.wls`, `stage_su4/...`, the generated files listed above.

**Status 2026-10-27: DONE -- `half` set generated, clang-formatted (17.0.6), INSTALLED in the clone**
via `QuenchedSU4_claude/run_gen_su4_ops_claude.sh` (`MOMSET=half INSTALL=1`; staging tree
`QuenchedSU4_claude/stage_su4/`, logs `run_gen_su4_ops_claude.log`, `run_gen_su4_ops_wolfram_claude.log`).
Installed: `total_n_glue_op 1324`, `total_n_tor_op 288`; `gb_shift_unit` hook intact (9 shift
phases in moving-frame ops). Not compiled yet (Chunk 2 build). Generated test file
`TestProgram/Utils/check_utils_3_gb_functions.c` is 33 MB (tor 18 MB).

Earlier test generation (scratch):
`GridBridge_claude/glueball_gen_claude/gen_glueball_ops_claude.wls` mode `su4 [rep|half]`: shapes
res[1,2,3,4,5,6,8,11,12,16,20] (= the su2rest shape union), every irrep x component x C, torelons on
the 3 github paths, no two-trace ops; github/su2rest unchanged (two-trace + torelon lines now inside
`If[genMode != "su4", ...]`).

| momset | momenta | `total_n_glue_op` | `total_n_tor_op` | distinct paths | Mathematica time |
|---|---|---|---|---|---|
| `rep` | (0,0,0), (0,0,1), (0,1,1), (1,1,1) | 534 | 117 | 240 | 25 s |
| `half` | (0,0,0) + 13 directions (one per $\pm\vec P$ pair) | 1324 | 288 | 240 | 61 s |
| su2rest (ref) | | 38 | 8 | 241 | 3 s |

All 44 glueball channels (10 $O_h$ irreps + $D_4$ 5 + $D_2$ 4 + $D_3$ 3, each C=$\pm$) and all 44
torelon channels are populated (2-11 multiplets per glueball channel). The path count equals
su2rest's, so the measurement cost is set by the same 240 loop evaluations per site: revised estimate
$2.5\,\text{s} \times 81 \times 8 \approx 1600$ core-s per measurement (the x1.5 op factor dropped);
`NP_T=4` about 400 s, `NP_T=24` about 70 s. Output per measurement (4 levels, T=48, complex double):
`rep` 1.7 MB, `half` 4.3 MB.

Q7 RESOLVED 2026-10-27: momset `half`. User: "we can average over inside to reduce the file
size" -> see Q8 (averaging the 1-pt functions over directions loses the gain).

### Q8 RESOLVED 2026-10-27: option (a) -- store all `half` 1-pt functions unaveraged, average correlators offline

For one momentum class with $n$ equivalent directions $\vec P_1, \dots, \vec P_n$ (n = 3, 6, 4 for
$|\vec P|^2 = 1, 2, 3$; only one of each $\pm\vec P$ pair), with the operator components aligned by
the rotations that map $\vec P_a$ onto each other, the wanted estimator is the average of correlators,

$$
\bar C(t) = \frac{1}{n} \sum_{a=1}^{n} \langle O_{\vec P_a}^*(t)\, O_{\vec P_a}(0) \rangle ,
\qquad \text{noise} \sim \sigma/\sqrt{n} .
$$

Averaging the 1-pt functions first, $\bar O = \frac{1}{n}\sum_a O_{\vec P_a}$, gives

$$
\langle \bar O^*(t)\, \bar O(0) \rangle = \frac{1}{n^2} \sum_{a} \langle O_{\vec P_a}^* O_{\vec P_a} \rangle
  + \frac{1}{n^2} \sum_{a \ne b} \langle O_{\vec P_a}^* O_{\vec P_b} \rangle ,
$$

where the cross terms ($\vec P_a \ne \vec P_b$) vanish only in the ensemble average (translation
invariance), not per configuration. Signal $\bar C/n$, noise from all $n^2$ terms $\sim \sigma/n$:
signal-to-noise $\bar C/\sigma$, the SAME as one direction alone (`rep`). So averaging $O$ inside saves
disk but throws away exactly the statistics `half` was meant to add.

Options:
- (a) store all 1-pt functions of `half` unaveraged (4.3 MB / measurement; with 1 measurement per 20
  updates, about 4.3 GB per 1000 measurements); average correlators offline. Exact, everything kept.
- (b) average the per-configuration correlators over directions inside the driver
  ($C_{ij}(t) = \frac{1}{T}\sum_{t'} O_i^*(t+t') O_j(t')$, per channel, $i,j$ over shapes x levels for
  the GEVP). Larger than (a): about 13 MB / measurement for the full shape x level matrices, because the
  GEVP basis (up to 11 shapes x 4 levels) squared outgrows the 1-pt data.
- (c) (a) in single precision: 2.2 MB / measurement. Precision $6 \times 10^{-8}$ relative; vacuum-subtracted
  $A_1^{++}$ at rest loses about $\log_{10}(\langle O \rangle / \delta O)$ digits, to check.
- (d) `rep` instead of `half` (1.7 MB), which equals (averaged half) statistically.

### Q6 RESOLVED 2026-10-27: topological charge

Wilson flow to $t/a^2 = 2$ (user "\hat\tau = 2", to be readjusted after thermalisation), measured at
every glueball measurement. `WF_update_and_measure(RK3_ADAPTIVE, ..., STORE)` returns
$(t, E, E_\text{sym}, Q)$ at `WF:nmeas + 1` flow times (`LibHR/Utils/wilsonflow.c:364-410`) -> new
dataset `/topo` shape (N, nmeas+1, 4). Flow on a copy of the gauge field (as `suN.c` does). Cost
estimate: an RK3 step is about 3 staple evaluations, about 30 core-s on $24^3 \times 48$; tens of
adaptive steps to $t=2$ -> 1000-3000 core-s, i.e. comparable to one glueball measurement.

`ObsInterval = 20` updates (user: "every 20 or so"). Cycle at `NP_T=4`: 20 updates (220-440 s) + glue
(~400 s) + flow (250-750 s) = about 900-1600 s -> 55-95 measurements/day locally; about 6x more on
tuolumne at `NP_T=24`.

### Chunk 2 -- driver + build
`suN_glueballs_h5_claude.c`: input parsing (HiRep pure-gauge keys `beta nhb nor therm` + glueball keys
+ `ObsInterval n_update`), thermalisation, update loop, T gather, h5 create/append/backup, checkpoint
+ prune, resume + h5 shrink. NG=4 MkFlags, build rule.
Files: `PureGauge/suN_glueballs_h5_claude.c`, `Make/MkFlags_su4_claude.ini`, `Make/build_claude.pl`.

**Status 2026-10-27: code written, build + smoke run handed off (`QuenchedSU4_claude/tmp_claude.sh`,
log `tmp_build_smoke_claude.log`).**
- `PureGauge/suN_glueballs_h5_claude.c`: standalone (input-record types from `suN_utils.h`,
  `suN_utils.c` untouched). Keys: `beta nhb nor` + `n_update` (additive) `ObsInterval therm h5file`
  + glueball keys + `WF:*`. Measures at `n > therm && n % ObsInterval == 0`. Fresh start = `random_u`
  (hot); refuses to start fresh if the h5 exists. Resume = env `CheckpointStart=<n>`. Checkpoints
  `ckpoint_lat.<n>` (HiRep `write_gauge_field`) + `ckpoint_rng.<n>` (`write_ranlxd_state`), written
  only at measurement points; previous pair removed after the new one is written. h5 backup via
  `.bak.tmp` + rename; `/traj` appended last; resume shrinks every dataset to the rows with
  traj <= n (restores from `.bak` if the file does not open). Gather over T on `GLB_COMM` placed by
  each rank's `zerocoord[0]`. h5 layout as in the Design section; `/topo` (N, WF:nmeas+1, 4).
- `Make/MkFlags_su4_claude.ini` (NG=4, no Grid, HDF5 in INCLUDE/LDFLAGS); copied to the tracked
  `Make/MkFlags.ini` (was an identical copy of `MkFlags_claude.ini`, NG=2).
- `Make/build_claude.pl`: `suN_glueballs_h5_claude` added to the PureGauge exes; copied to the tracked
  `Make/build.pl` (was identical). Manifest generation checked (NG = 4, LINK = mpicc, -lhdf5).
- `QuenchedSU4_claude/build_su4_claude.sh` (checks MkFlags/build.pl are the SU(4) copies, does not
  overwrite them; clang pinned to GCC 13, openmpi_cuda), `input_su4_smoke_claude` (8^3 x 16, NP_T = 2,
  6 updates, ObsInterval 2, levels 0-2, WF to t = 2 with 10 measurements).
- Behaviour to note: updates after the last measurement point of a run are not checkpointed; a resume
  redoes them from the last checkpoint (same RNG state, so the same chain).
- **Build attempt 1 FAILED (2026-10-27)**: `torellons_op.c` malformed for every C=- torelon. Generator
  bug in `DyPolyGenerateCcode` (reordering rule of the straight-line factor), never triggered by C=+
  sets. FIXED in `GridBridge_claude/glueball_gen_claude/OpProjection_claude.m` (original rule kept as a
  comment); documented in `GridBridge_claude/glueball_generator_audit_claude.md` section 7. su2rest
  regeneration unchanged; `half` set regenerated + reinstalled (288 torelon functions, 130 C=-, all
  well formed). `glueballs_op.c` compiled in attempt 1. Rerun the same `tmp_claude.sh`.
- **Build attempt 2 PASSED + smoke run PASSED (2026-11-02)**, `runs_claude/smoke/` (8^3 x 16, NP_T = 2,
  6 updates, 3 measurements): exit 0, 172 s; h5 datasets/attrs as designed, traj [2 4 6], no empty
  timeslice (gather OK), `.bak` written, old checkpoint pairs pruned, Q along the flow stored.
  **Independent check:** momentum-projected plaquettes at P = (1,1,1), t = 0, recomputed in Python
  from `ckpoint_lat.6`: op 1302 = $-i\,2\sqrt3\,(M_{xy} - M_{yz})$ reproduces the h5 value
  77.84349581 - 47.28072613 i.
  **32 glue ops identically zero** (all rows, levels, t): indices
  0 2 33 35 44 55 634 665 684 694 710 757 782 813 822 833 866 885 908 928 1087 1119 1129 1139 1171 1199 1219 1228 1283 1293 1304 1315.
  All in the $|\vec P|^2 = 2, 3$ frames, shapes plaquette and res[3]. They are zero AS WRITTEN (e.g. op 0
  = -M0 + M0 - M1 + M1 - c0 M2 + c0 M2): the generator keeps two translated copies of a loop as distinct
  symbols when their shifts differ, but $e^{i\vec P\cdot\vec s}$ is the same for both, so the C code
  cancels exactly. Not a measurement bug. Torelons: none zero.
  Timings (2 ranks, 8^3 x 16, hot start): update 0.2 s, glueballs 8-10 s, torelons 0.25 s, Wilson flow
  45-48 s, h5 backup + append < 0.02 s. Scaled to 24^3 x 48 (x81): update ~32 core-s, glueballs ~1500
  core-s (matches the estimate), flow ~7500 core-s (3-7x above the estimate; hot configuration, to
  re-measure on a thermalised one in Chunk 4).
- **2026-11-03 (user: "check that they are zero, and comment out")**: all 32 verified SYMBOLICALLY zero
  (every coefficient cancels in the generated C; they are exactly the symbolically-zero functions of the
  set) in addition to numerically zero; each is a complete multiplet. Commented out at the source:
  `gen_glueball_ops_claude.wls` su4 block lists the 32 `Add1trOpCorrelators[...]` calls in a comment and
  skips them via `genSu4Skip`. Regenerated + installed: `total_n_glue_op 1292` (torelons 288 unchanged),
  no symbolically zero op left. Rebuild + smoke rerun handed off (`tmp_claude.sh`, run dir
  `runs_claude/smoke2`). The analysis channel map comes from the run log, so slot numbers of the 1324 set
  (smoke run 1) do not apply to new runs.
- **smoke2 PASSED (2026-11-03)**, `runs_claude/smoke2`: 1292 glue / 288 tor ops, no zero op, no empty
  timeslice; same seed as smoke 1 -> plaquette, Q, torelons, topo bit-identical, and the 1292 kept glue
  ops bit-identical to smoke 1 with its 32 zero slots removed. Chunk 2 closed.
- TestProgram `check_utils_3_{gb,tor}_functions.c` (generator test tables, NG=3 tests only): user "no need to
  commit if it can be easily reproduced" -> do not add; reproduced by `run_gen_su4_ops_claude.sh`. (Already
  in commit 1d6601d5.)

### Chunk 3 -- small-lattice checks ($8^3 \times 16$, minutes)
(a) plaquette sane; C=- operators nonzero; (b) resume test: 40 measurements straight vs 20 + resume
20 -> h5 bit-identical; (c) crash-recovery test: h5 one row ahead of the checkpoint -> shrunk on resume.
Files: `input_su4_small_claude`, `run_su4_small_claude.sh` (handed off via `tmp_claude.sh`).

**Status 2026-11-03: handed off** (`tmp_claude.sh`, log `tmp_chunk3_claude.log`, dirs
`runs_claude/chunk3/{A,C,S,D,E}`; input `input_su4_chunk3_claude`: ObsInterval 4, WF tmax 0.5 for speed).
A = straight 12; C = 8 + resume 4 (test a: C == A); D = ckpt 8 + C's 3-row h5 (test b: shrink to 2
rows, D == C); E = ckpt 8 + truncated h5 + 2-row .bak (test c: restore, E == C). Crash states built by
copies into new dirs only. Possible outcome to watch: `LibHR/Random/random_su2.c` keeps buffered random
numbers (NVEC) outside the ranlxd state, so (a) may differ beyond the first resumed update while (b)
and (c) (same resume path as C) stay identical.

**Run 1 result (2026-11-03, `runs_claude/chunk3`)**: (b) and (c) PASSED (shrink 3 -> 2 rows on all 5
datasets / restore from .bak, then bit-identical to C; one checkpoint pair left per dir). (a) FAILED:
identical through update 8 (plaq, Q at the checkpoint), diverged from update 9 (first resumed update).
Cause: `random_su2.c` pre-draws random numbers into per-thread buffers (vec1-3, y/u, v; refilled NVEC/NRAN
at a time) that are not part of the ranlxd state; HiRep's own heatbath programs have the same property
(the other generators in LibHR/Random keep no such buffers).
**Fix (user: "It should be exactly reproducible")**: new `reset_su2_rand()` in `LibHR/Random/random_su2.c`
(core file, edited in place; declared in `Include/Random/random_utils.h`) marks all buffers empty; the
driver calls it right before `write_ranlxd_state` in every checkpoint. Second condition, enforced at
startup: `ObsInterval * (nhb + nor)` must be a multiple of 10 (`update_all` re-projects every
PROJECT_INTERVAL = 10 sweeps, `update_hb.c:14`, with a counter that restarts on resume). Production
20 x 5 and the test 4 x 5 satisfy it. Rerun: rebuild + same tests in `runs_claude/chunk3_fix`.

**Run 2 result (2026-11-03, `runs_claude/chunk3_fix`)**: gauge field now exact (`ckpoint_lat.12`
IDENTICAL A/C/D/E); plaq, glue, tor bit-identical in all comparisons; (b), (c) fully identical. Only
`/topo` differed A vs C, and only its flow-time t = 0 row. **Bug found (our driver):** the t = 0 flow
measurement (E, Esym, Q) reads neighbour links of the copy `Vwf` before any flow step, and
`E_cpu`/`Esym_cpu`/`topo_cpu` (`LibHR/Update/gauge_observables.c`) do no halo exchange: the t = 0 row
used stale halo links (A: halo of the previous flowed field; resumed run: fresh field). Verified with
the exact identity $E(t=0) = 12 N_c (1 - \langle P \rangle)$: stored E(t=0) was 5-8 % too high in every
row of every run so far (smoke, smoke2, chunk3*); t > 0 rows were correct (each flow step exchanges
first). HiRep's own `suN.c` uses the same copy + `WF_update_and_measure` pattern (not tested).
**Fix:** `start_sendrecv_suNg_field(Vwf); complete_sendrecv_suNg_field(Vwf);` right after the copy in
the driver. Rerun: rebuild + tests in `runs_claude/chunk3_fix2`, with an added check
$\max |E(t=0) - 12 N_c (1 - \langle P \rangle)|$. Data of the earlier test runs: t = 0 row of `/topo` invalid.

**Central fix (user: "better fix it", 2026-11-03).** `copy_suNg_field` copies local sites only
(`LibHR/Inverters/TMPL/linear_algebra_base.c.tmpl:26`), so EVERY PureGauge program that flows a copy
(`suN.c`, `suN_multilevel*.c`, `mk_1pt_glueballs*.c`, our `hmc_glueballs_grid*_claude.c`,
`mk_1pt_glueballs_from_grid_claude.c`) has the stale-halo t = 0 bug with > 1 MPI rank, in all checkouts
(`hirep_su4` su4, `hirep_workspace/HiRep` HiRep-CUDA, `HiRep2`). Not affected: `WilsonFlow/WF_measure.c` and
`TestProgram/StoredConfs` (they flow `u_gauge` right after `read_gauge_field`, which exchanges). No SU(2)
result is affected (all hirep_workspace run inputs have `WF:make = false`). Fixed in
`LibHR/Utils/wilsonflow.c` `WF_update_and_measure`: `start_sendrecv_suNg_field(V);
complete_sendrecv_suNg_field(V);` before the t = 0 measurement (start_sendrecv completes any pending
exchange first, so safe for every caller), in BOTH `hirep_su4` (su4) and `hirep_workspace/HiRep`
(HiRep-CUDA, uncommitted; user commits). The driver-local exchange is commented out (kept in place).
`HiRep2` NOT touched (separate tree). Upstream claudiopica/HiRep has the same code: issue/PR = user's call.

**Run 3 (driver-local halo fix) PASSED 2026-11-03 13:05, `runs_claude/chunk3_fix2`:** `ckpoint_lat.12`
IDENTICAL A/C/D/E; traj, plaq, glue, tor, topo bit-identical in all comparisons (a), (b), (c);
$\max |E(t=0) - 12 N_c (1-\langle P\rangle)| = 8.9 \times 10^{-14}$ in all runs. Run 4 (central fix only)
handed off: `runs_claude/chunk3_fix3`.
**Run 4 (central fix only) PASSED 2026-11-03 20:11:** same all-identical result, E(t=0) identity 8.9e-14,
and every dataset + `ckpoint_lat.12` bit-identical to run 3. **CHUNK 3 DONE**: resume is exact.

### Chunk 4 -- benchmark on $24^3 \times 48$
Few updates + 2 measurements with `NP_T = 8` and `16`; memory high-water mark. Replace the estimate
table above; choose `NP_T`, `ObsInterval`, thermalisation length.
Files: `input_su4_24c_bench_claude`, `run_su4_24c_bench_claude.sh`.

**Status 2026-11-03: handed off as the FIRST SEGMENT OF PRODUCTION STREAM 1** (resume is exact):
`QuenchedSU4_claude/tmp_claude.sh` -> log `tmp_bench24c_claude.log`, run dir
`runs_claude/stream1_24c_b11p045/`, input `input_su4_24c_claude` (24^3 x 48, NP_T = 4, beta 11.045,
1 HB + 4 OR, therm 500, ObsInterval 20, n_update 540 -> measurements at 520, 540; levels 0-3, APE 0.33;
WF adaptive RK3 to t = 2, nmeas 10; rlx_seed 20261103; h5 `glue_su4_24c_b11p045_s1_claude.h5`).
Memory sampler (RSS per rank every 60 s). Machine load average ~10 at hand-off: timings pessimistic.
Continue the stream with `CheckpointStart=540` and a new `n_update`.

**Chunk 4 RESULT (2026-11-03 20:20-23:00, stream 1 segment 1 DONE, 9605 s wall, exit 0)**, NP_T = 4,
machine load average 7-10 from other jobs:

| step | wall (4 ranks) | core-s | estimate was |
|---|---|---|---|
| update (1 HB + 4 OR) | 9-11 s | ~38 | 45-90 (smoke scaling 32) |
| glueballs (1292 ops, levels 0-3) | 616-622 s | ~2470 | ~1500 |
| torelons (288 ops) | 10 s | 40 | -- |
| Wilson flow to t = 2 (adaptive RK3, delta 1e-5) | 1367-1563 s | ~5900 | ~7500 (hot) |
| h5 backup + append | < 0.01 s | -- | -- |

- Cycle (20 updates + 1 measurement) ~ 2290 s -> about 38 measurements/day locally; the flow is 64 % of it.
- Memory 1.55 GB per rank (6.2 GB total). Disk: checkpoint 0.68 GB, h5 4.2 MB per measurement.
- Thermalisation of the plaquette: 0.5799-0.5801 from update ~50 on (hot start). Q needs a longer history.
- Flow at t = 2: $t^2 E$ = 0.270, 0.272; Q = -2.69, 0.96 (not near-integer at t = 2).
- Checks: E(t=0) identity 7.5e-14; no zero op; no empty timeslice.
- Constraint for changes: `/topo` row shape is fixed at creation (WF:nmeas + 1), and WF/level attributes are
  written once; flow or blocking settings must be settled before the stream continues (or start a new h5).

**Decisions 2026-11-04 (after Chunk 4):** stay LOCAL for the first look (HiRep has no GPU heatbath /
flow / glueball code; tuolumne CPU deferred). First look = 100 measurements; Wilson flow every 5th
measurement. Driver: new input key `FlowInterval` (updates; multiple of ObsInterval; default =
ObsInterval), new dataset `/topo_traj` (update index of each `/topo` row); `/topo`, `/topo_traj` resume
by `topo_traj <= ckpt`; a legacy h5 (no `/topo_traj`) gets `/topo_traj = /traj` on resume (legal only
when the lengths agree). Segment 2 handed off (`tmp_claude.sh`, log `tmp_seg2_claude.log`): rebuild,
8^3 tests in `runs_claude/flowint_test` (fresh vs resume with FlowInterval 8, legacy upgrade), then
stream 1 resume from 540 with `input_su4_24c_seg2_claude` (n_update 2000 -> measurements 560..2540,
flows 600..2500); estimate ~31 h (100 x 820 s + 20 x 1470 s). Safeguard copy
`glue_su4_24c_b11p045_s1_seg1end_claude.h5` made first.

### Chunk 5 -- production stream(s) + reader
Run script (resumable, one directory per stream, own `rlx_seed`), h5 reader in the analysis script.
Files: `run_su4_24c_prod_claude.sh`, `input_su4_24c_prod_claude`, analysis script.

## Open questions

- **Q1 (code location) -- RESOLVED 2026-10-27.** User creates a separate clone on a separate branch
  for the SU(4) study (HiRep builds one NG per tree: `Make/nj` hardcodes `$rootdir/build`, one
  `Make/MkFlags.ini`, `autosun` writes NG-dependent headers into `Include/`). The SU(4) operator set
  is committed on that branch only; the SU(2) tree `HiRep/` and the collaborator's branch are not
  touched. Clone: `/mnt/baracuda_14/hirep_su4`, branch `su4` (tracks `origin/su4`), base `e11316cb`.
- **Q2 (operator list) -- RESOLVED 2026-10-27.** At rest: C=+ and C=- for all 10 $O_h$ irreps on the
  existing shape list. Moving frames $\vec P = (0,0,1), (0,1,1), (1,1,1)$: EVERY irrep of the little
  group ($D_4$, $D_2$, $D_3$), C=+ and C=-. NO two-trace products. Torelons: existing 8 + C=- copies
  + more moving frames. Generator limit: $p_i \in \{-1,0,1\}$.
- **Q3 (update mix) -- RESOLVED.** 1 HB + 4 OR per update, 500 thermalisation updates from a hot
  start. User: "we'll monitor the topo charge" -> topological-charge measurement needed (Q6).
- **Q4 (blocking) -- RESOLVED (as a start).** Levels 0-3 ($24 \to 12 \to 6 \to 3$), APE 0.33.
- **Q5 (streams) -- RESOLVED.** Single stream to begin with. Local tests use AT MOST 4 cores
  (`NP_T <= 4`; user does not want this machine's cores used up). If too slow, production moves to
  tuolumne (CPU ranks, e.g. `NP_T = 24`).
- **Q6 (topological charge, NEW, open).** HiRep's Wilson flow (`WF_update_and_measure`) prints $Q$
  along the flow. Open: flow time, how often (a flow on $24^3 \times 48$ SU(4) may cost several
  glueball measurements), and whether $Q$ goes into the same h5 (`/topo` dataset).
