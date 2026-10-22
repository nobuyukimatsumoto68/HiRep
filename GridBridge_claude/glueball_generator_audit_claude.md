# Glueball operator generator: audit and how to use it

Audit of `Make/Utils/Math_glueballs/` (`OpProjection.m`, `generate_glueball_code.nb`, author
A. Rago), 2026-10-20. Read-only: Mathematica is not installed on the PI's workstation, so nothing
was run. Items marked **verified** were re-checked by hand against the source or our data; the
rest come from a full read of the sources and are cited by line.

## Sources (methods the generator implements)

- Glueball operators from closed Wilson loops projected onto irreps of the cubic group $O_h$:
  B. Berg and A. Billoire, Nucl. Phys. B221 (1983) 109; R. C. Johnson, Phys. Lett. B114 (1982) 147.
- Moving frames use the little groups of $O_h$ ($D_4$ for $(0,0,1)$, $D_2$ for $(0,1,1)$,
  $D_3$ for $(1,1,1)$; `OpProjection.m:150-152`).

## 1. Key findings

1. **The committed 15-operator set is the CI test set, not a physics set (verified).** The notebook
   has two operator lists switched by `CodeControl = {"github", "run"}[[1]]`: the first is labelled
   `(* minimal set for github CI *)`, the second `(* Base op used for the runs *)`
   (`generate_glueball_code.nb:463-561`). The committed `glueballs_op.c` is the "github" one.
2. **The "run" list is the one you want:** 8 single-trace $A_1^{++}$ operators at rest built from
   loop shapes `res[1, 3, 5, 8, 11, 12, 16, 20]` (plaquette, 6-link non-planar, 8- and 10-link
   loops), plus 14 two-trace $A_1$ products, plus torelons. Still no $E^{++}$ / $T_2^{++}$
   ($2^{++}$); those must be added by hand (section 3).
3. **$C = -$ operators are identically zero for SU(2) (verified).** $C=-$ takes
   $\mathrm{Im}\,\mathrm{tr}$ of the loops (`OpProjection.m:435-442`), and SU(2) traces are real.
   In our $8^3 \times 16$ data ops 8-10 ($T_1^{+-}$) have $|O| < 10^{-12}$ at every level. For SU(2),
   generate only $C = +$ operators.
4. **`GenerateCcode[]` deletes and rewrites tracked files (verified):**
   `LibHR/Observables/glueballs_op.c`, `LibHR/Observables/torellons_op.c`,
   `Include/Observables/glueballs.h` (including `total_n_glue_op`), and
   `TestProgram/Utils/check_utils_3_{gb,tor}_functions.c`, all relative to `topdir = "../../.."`
   (`OpProjection.m:182-189, 787-788`).
5. Larger sets generated in the past exist on old branches: `origin/HiRep-COMP` (2019,
   `total_n_glue_op 105`: about 21 $A_1^{++}$, 18 $A_1^{+-}$, $A_1$ at $|\vec P| = 1, \sqrt2$) and
   `origin/HiRep-GPU-new` (2020, 30 ops). They predate the 2022 header reorganisation and
   `hr_complex`, and lack `report_gb_group_setup` / `collect_1pt_glueball_functions`, so they are not
   drop-in; regenerating is cleaner than porting.

## 2. How the generator works

- **Inputs** (notebook Input cells, in order): `SetDirectory[NotebookDirectory[]]`,
  `Get["OpProjection.m"]`, the loop shapes `res[1..32]` (nb:79-383; plaquette, 2x1 rectangle,
  6-, 8-, 10-link loops; all 32 in distinct $O_h$ orbits), the two switches (nb:389-417), the
  single-trace list (nb:463-561), the two-trace list (nb:1125-1271), the torelon list
  (nb:2014-2147), then `GenerateCcode[]` (nb:11833).
- **Path syntax:** `P[ax, ay, -ax, -ay]`, unit steps along $\pm x, \pm y, \pm z$; closed loops only.
- **Single-trace operator:** `Add1trOpCorrelators[px, py, pz, irrep, ev, C, res[k]]`, with
  $p_i \in \{-1, 0, 1\}$ (other momenta are silently dropped), `ev` = component of the irrep.
  Irrep index at $\vec P = 0$ (`OpProjection.m:148`): 1 $A_1^{+}$, 2 $A_2^{+}$, 3 $E^{+}$,
  4 $T_1^{+}$, 5 $T_2^{+}$, 6 $A_1^{-}$, 7 $A_2^{-}$, 8 $E^{-}$, 9 $T_1^{-}$, 10 $T_2^{-}$ (superscript =
  parity; $C$ is the separate argument).
- **Two-trace operator:** `Add2trOpCorrelators[...]`; Clebsch-Gordan tables exist only for
  $A_1 \times A_1 \to \vec P = 0$ with back-to-back $|\vec p| = 1, \sqrt2, \sqrt3$.
- **Projection** sums the 48 $O_h$ elements without normalisation (hence factors like 8 in the C
  code); no vacuum subtraction. Both are left to the analysis.
- **Output order** in the `[step]` rows: momentum, then irrep, then $C$, then component, then
  multiplet (`OpProjection.m:1353-1385`). The channel table is printed at startup by
  `report_gb_group_setup` (`[INIT Measure ML] 1pt_glb Irrep multiplets ...`).

## 3. Recipe: regenerate with single-glueball operators at rest

1. **Get a Wolfram kernel** with `wolframscript` (Mathematica 11.3 or later; uses SymbolicC). The
   free Wolfram Engine is an option; check its license terms for research use.
2. **Protect the tracked files.** Work on a branch or in a `git worktree`, or point `topdir`
   (`OpProjection.m:182`) at a scratch tree containing `LibHR/Observables`,
   `Include/Observables`, `TestProgram/Utils`.
3. **Run headless.** `NotebookDirectory[]` needs the front end, so write a `.wls` with:
   `SetDirectory["<abs path>/Make/Utils/Math_glueballs"]`, `Get["OpProjection.m"]`, the `res[1..32]`
   definitions copied from the notebook, the operator lists, `GenerateCcode[]`; run
   `wolframscript -file gen_claude.wls` in a fresh kernel (the generator keeps global, memoised
   state; never run it twice in one kernel).
4. **Operator lists for SU(2)** ($C = +$ only):
   - the "run" $A_1^{++}$ list: `Add1trOpCorrelators[0,0,0,1,1,1,res[k]]` for
     `k` in {1, 3, 5, 8, 11, 12, 16, 20};
   - $E^{++}$ (2 components): `Add1trOpCorrelators[0,0,0,3,e,1,res[k]]`, `e` = 1, 2;
   - $T_2^{++}$ (3 components): `Add1trOpCorrelators[0,0,0,5,e,1,res[k]]`, `e` = 1..3;
   - optionally $A_1$ at $|\vec P| = 1$ (dispersion check) and the existing two-trace and torelon lines.

   Which shapes contain $E^{++}$ / $T_2^{++}$ was estimated from an independent $O_h$ character
   calculation (not run through the generator): $E^{++}$ from `res` 1, 2, 4, 5; $T_2^{++}$ from
   `res` 3, 4, 5, 6. A shape that does not contain the irrep projects to zero, so check that each
   generated operator is non-zero. Where an irrep appears twice in one shape, a second (rotated)
   seed path is needed for the second copy.
5. **Format and validate.** clang-format the generated files; build an **NG = 3** tree and run
   `TestProgram/Utils/check_utils_3` (rotations) and `TestProgram/PureGauge/check_puregauge_5`
   (gauge invariance). Both are disabled for NG = 2 (`NOCOMPILE= NG==2`). Then rebuild the NG = 2
   tree with `GridBridge_claude/build_grid_hirep_claude.sh`.
6. **Update the analysis** channel map (`GridBridge_claude/analyze_glueball_1pt_claude.py`, `CHANNELS`)
   from the new `report_gb_group_setup` table. Better: parse that table from the out file.

Cost: about 48 terms per projection; generation should take seconds. Measurement cost grows with
the number of distinct loop paths per timeslice and blocking level (currently 0.18 s per
measurement for 45 operators on $8^3 \times 16$).

## 4. Risks and bugs found

| # | Issue | Status | Impact for us |
|---|---|---|---|
| 1 | Committed set is the CI test set | verified | why $\vec P = 0$ has no $0^{++}$ |
| 2 | $C=-$ operators vanish for SU(2); generator tests disabled for NG = 2 | verified | drop $C=-$; validate on NG = 3 |
| 3 | `GenerateCcode` deletes the tracked originals and header | verified | branch/worktree first |
| 4 | Shift phase `c0` assumes 1-site steps, but blocked links span $2^b$ sites (`spatial_transformations.c:122`) | **verified** (section 6) | wrong phases for $\vec P \ne 0$ operators with shifts at blocking level $\ge 1$ (committed op 13 only); $\vec P = 0$ unaffected |
| 5 | Momentum phase uses `GLB_X` for all directions | from audit | cubic lattices only |
| 6 | `Add2trOpCorrelators` uses `irrepidx1` for the second factor (`OpProjection.m:305`) | from audit | latent; harmless while both irreps are $A_1$ |
| 7 | Header declares `request_spatial_paths_evaluation`, code defines `request_space_paths_evaluation` | from audit | unused prototype |
| 8 | Momenta outside $\{-1,0,1\}^3$ silently dropped | from audit | check counts after generation |
| 9 | No normalisation, no vev subtraction | from audit | handled in `analyze_glueball_1pt_claude.py` |
| 10 | `total_n_tor_op = 0` build untested | from audit | keep some torelons |
| 11 | `last_t` path cache | from audit: safe | `request_space_paths_evaluation()` resets it per call |

## 5. Results of the first headless run (2026-10-22, Mathematica 15.0.1)

Driver `glueball_gen_claude/gen_glueball_ops_claude.wls`, runner
`glueball_gen_claude/run_gen_glueball_ops_claude.sh` (staging trees only).

- `github` mode reproduces all five committed files exactly (token comparison ignoring whitespace
  and braces). So Mathematica 15 generates the same code as the 11.3 used originally (risk 13 of the
  subagent audit: resolved).
- `su2rest` mode: `total_n_glue_op 38`, `total_n_tor_op 8`; 35 single-trace operator functions,
  each with 2 to 48 loop terms (none empty), all 38 slots of `eval_all_glueball_ops` written.
  Channel table (operator index -> channel):

| Indices | $\vec P$ | Channel |
|---|---|---|
| 0, 1 / 2 / 3 / 35 / 36 / 37 | $\pm \hat e_i$ | $A_1$ ($D_4$), plaquette-type (moving frame) |
| 4-11 | 0 | $A_1^{++}$, 8 loop shapes |
| 12 | 0 | $A_1^{+}$ two-glueball product |
| 13-16 / 18-21 | 0 | $E^{++}$ components 1 / 2, 4 shapes |
| 17 / 22 | 0 | $E^{+}$ two-glueball products |
| 23-34 | 0 | $T_2^{++}$, 4 shapes x 3 components (23, 27, 31 = shape 1, ...) |

- Spot check: the plaquette $E^{++}$ components are $8(2P_{xy} - P_{yz} - P_{zx})$ and
  $8\sqrt3\,(P_{zx} - P_{yz})$, the standard $E$ doublet.

## 6. Bug: momentum shift phases are wrong at blocking levels >= 1 (verified 2026-10-22)

**Mechanism.** A moving operator $O_{\vec P} = \sum_{\vec x} e^{i \vec P \cdot \vec x}\, \mathrm{tr}\,W(\vec x)$
is a sum of rotated copies of a loop. The generator refers some copies to a base point displaced
by a number of lattice *steps* and compensates with a phase. `WriteShift` (`OpProjection.m:915-927`,
line 924) writes each such phase into the C code as a constant
`cN = cexp(I*PI*(<number>/GLB_X))`, computed assuming **one step = one site**; it is set once, when
`path_storage` is first allocated (committed `glueballs_op.c:1077`).

Spatial blocking (`LibHR/Utils/spatial_transformations.c:122-123`, used by `measure_1pt_glueballs`
in `LibHR/Observables/glueballs.c`) replaces $U(x,\mu)$ by $U(x,\mu)U(x+\hat\mu,\mu)$ and doubles the
neighbour step, so at blocking level $b$ one step spans $2^b$ sites. The correct phase is then
$c_N^{(b)} = \exp(i \pi\, k_N 2^b / L)$ where the code uses $\exp(i \pi\, k_N / L)$. The rotated
copies are combined with wrong relative phases: the result is no longer a clean projection onto the
intended irrep and momentum.

**Who is affected.**

| Operators | Affected? | Why |
|---|---|---|
| $\vec P = 0$ (all irreps, e.g. old op 5 $A_2^{+}$, new $A_1^{++}$/$E^{++}$/$T_2^{++}$) | no | shifts dropped for zero momentum components (`OpProjection.m:429-431`) |
| $\vec P \ne 0$ without shifts (plaquette $A_1$ at $\vert\vec P\vert = 1$: old ops 0-3, 11, 12, 14; new 0-3, 35-37) | no | no `c` factors in their code |
| old op 13 ($A_2$ ($D_4$) at $\vec P = (0,1,0)$, 8-link `res[8]`) | **yes, at levels $\ge 1$** | uses `c0` (`glueballs_op.c:1060-1062`) |
| torelons | no | have 2 shift phases but are only APE-smeared, never blocked |
| new 38-operator set | no | contains no shift phases at all |

Level 0 (raw links) is always correct. The bug is latent for the current runs but will hit any
future moving operator built from non-plaquette loops (e.g. a moving-frame variational basis).

**Fix (applied 2026-10-22; regenerated `github` output verified to differ from HEAD only in these lines).** Make the step length explicit: the generator emits
`cN = cexp(I*PI*(<number>*gb_shift_unit)/GLB_X)` and recomputes the `c` constants on every
evaluation, and `measure_1pt_glueballs` sets `gb_shift_unit = 1 << level` before evaluating each
blocking level. See `glueball_mass_test_impl_plan_claude.md`.
