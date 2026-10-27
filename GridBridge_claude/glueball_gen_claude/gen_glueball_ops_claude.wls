#!/usr/bin/env wolframscript
(* ::Package:: *)

(*
  Headless driver for HiRep's glueball/torelon operator generator
  (Make/Utils/Math_glueballs/OpProjection.m, A. Rago), replacing the notebook
  generate_glueball_code.nb. Operators: Wilson loops projected onto irreps of the cubic group
  O_h and its little groups (B. Berg and A. Billoire, Nucl. Phys. B221 (1983) 109;
  R. C. Johnson, Phys. Lett. B114 (1982) 147). Audit: GridBridge_claude/glueball_generator_audit_claude.md.

  Usage (run inside a STAGING tree that mirrors HiRep; never in HiRep itself, because
  GenerateCcode[] deletes and rewrites <topdir>/LibHR/Observables/glueballs_op.c etc.):

    wolframscript -file gen_glueball_ops_claude.wls <stage>/Make/Utils/Math_glueballs <mode>

  <stage>/Make/Utils/Math_glueballs must contain OpProjection_claude.m, a copy of the PATCHED
  generator GridBridge_claude/glueball_gen_claude/OpProjection_claude.m (shift-phase / blocking fix;
  LibHR/Observables/glueballs.c needs the gb_shift_unit it defines, so do NOT regenerate with the
  original Make/Utils/Math_glueballs/OpProjection.m); its topdir="../../.." resolves to <stage>.

  <mode>:
    github  : the committed CI set (notebook CodeControl="github", TypeControl="all"); used to check
              that this Mathematica version reproduces the committed glueballs_op.c
    su2rest : single-trace C=+ operators at rest for SU(2) (A1++, E++, T2++), plus the github
              moving-frame A1, two-trace and torelon operators. C=- operators are omitted (they
              take Im tr and vanish identically for SU(2)).
    su4     : quenched SU(4) set (QuenchedSU4_claude/quenched_su4_glueball_impl_plan_claude.md, Q2):
              single-trace C=+ and C=- operators, every irrep and component, on the shapes
              res[1,2,3,4,5,6,8,11,12,16,20], at rest and in moving frames; torelons on the three
              github paths, every irrep, C=+ and C=-, at rest and in the same moving frames; NO
              two-trace operators. Zero projections are dropped by the generator.
              Optional 4th argument <momset>: rep  = (0,0,1),(0,1,1),(1,1,1) (default)
                                              half = all 13 directions, one of each +-P pair
              (a C-eigen operator at -P is the complex conjugate of the one at +P).
*)

genArgs = $ScriptCommandLine;
If[Length[genArgs] < 3,
  Print["usage: wolframscript -file gen_glueball_ops_claude.wls <stage>/Make/Utils/Math_glueballs <github|su2rest>"];
  Exit[1]
];
genStage = genArgs[[2]];
genMode = genArgs[[3]];
genMomSet = If[Length[genArgs] >= 4, genArgs[[4]], "rep"];
If[Not[MemberQ[{"github", "su2rest", "su4"}, genMode]],
  Print["unknown mode: ", genMode];
  Exit[1]
];
Print["Wolfram version: ", $VersionNumber, "  (", $Version, ")"];
Print["stage dir: ", genStage, "  mode: ", genMode, "  momset: ", genMomSet];
If[Not[MemberQ[{"rep", "half"}, genMomSet]],
  Print["unknown momset: ", genMomSet];
  Exit[1]
];

SetDirectory[genStage];
Get["OpProjection_claude.m"];
Print["output files: ", {OpProjection`opfilename, OpProjection`torfilename, OpProjection`headerfilename}];

(* Loop shapes, copied verbatim from generate_glueball_code.nb (cell lines 79-383) *)
res[1] = P[ax, ay, -ax, -ay];
res[2] = P[ax, ax, ay, -ax, -ax, -ay];
res[3] = P[ax, -ay, az, -ax, ay, -az];
res[4] = P[ax, ay, -ax, az, -ay, -az];
res[5] = P[ax, ax, -ay, -ax, -ax, az, ay, -az];
res[6] = P[ax, ax, -ay, -ax, ay, az, -ax, -az];
res[7] = P[ax, ax, -ay, -ax, az, ay, -ax, -az];
res[8] = P[ax, ax, -ay, -ax, az, -ax, ay, -az];
res[9] = P[ax, ax, ay, -ax, -az, -ax, az, -ay];
res[10] = P[ax, ax, ay, -az, -ax, -ax, az, -ay];
res[11] = P[ax, ay, -ax, -ay, -ax, az, ax, -az];
res[12] = P[ax, ax, ax, -ay, -ax, -ax, -ax, az, ay, -az];
res[13] = P[ax, ax, ax, -ay, -ax, -ax, ay, ay, -ax, -ay];
res[14] = P[ax, ax, ax, -ay, -ax, -ax, ay, az, -ax, -az];
res[15] = P[ax, ax, ax, -ay, -ax, -ax, az, -ax, ay, -az];
res[16] = P[ax, ax, ax, -ay, -ax, -ax, az, ay, -ax, -az];
res[17] = P[ax, ax, ax, -ay, -ax, ay, -ax, az, -ax, -az];
res[18] = P[ax, ax, ax, -ay, -ax, az, -ax, -ax, ay, -az];
res[19] = P[ax, ax, ax, -ay, -ax, az, -ax, ay, -ax, -az];
res[20] = P[ax, ax, ax, ay, -ax, -ax, ay, -ax, -ay, -ay];
res[21] = P[ax, ax, ax, ay, -ax, -ax, -az, -ax, az, -ay];
res[22] = P[ax, ax, ax, ay, -ax, ay, -ax, -ax, -ay, -ay];
res[23] = P[ax, ax, ax, ay, -ax, -az, -ax, -ax, az, -ay];
res[24] = P[ax, ax, -ay, -ax, -ax, ay, -ax, ay, ax, -ay];
res[25] = P[ax, ax, -ay, -ax, -ax, ay, -ax, az, ax, -az];
res[26] = P[ax, ax, -ay, -ax, -ax, ay, -az, ay, az, -ay];
res[27] = P[ax, ax, -ay, -ax, -ay, az, -ax, ay, ay, -az];
res[28] = P[ax, ax, -ay, -ax, -ay, az, ay, -ax, ay, -az];
res[29] = P[ax, ax, -ay, -ax, ay, -ax, -ax, ay, ax, -ay];
res[30] = P[ax, ax, -ay, -ax, ay, -ax, -ax, az, ax, -az];
res[31] = P[ax, ax, -ay, -ax, ay, -ax, -ay, az, ay, -az];
res[32] = P[ax, ax, -ay, -ax, ay, -ax, ay, az, -ay, -az];

(* Add1trOpCorrelators[px, py, pz, irrep, ev, C, path]
   irrep at P = 0 (OpProjection.m:148): 1 A1+, 2 A2+, 3 E+, 4 T1+, 5 T2+, 6 A1-, ... ; C = +1 / -1 *)

If[genMode == "github",
  (* single trace, notebook cell lines 463-561, CodeControl = "github" *)
  Add1trOpCorrelators[-1, 0, 0, 1, 1, 1, res[1]];
  Add1trOpCorrelators[0, 0, 0, 2, 1, 1, res[2]];
  Add1trOpCorrelators[0, 1, 0, 2, 1, 1, res[8]];
  Add1trOpCorrelators[0, 0, 0, 4, 1, -1, res[2]];
  Add1trOpCorrelators[0, 0, 0, 4, 2, -1, res[2]];
  Add1trOpCorrelators[0, 0, 0, 4, 3, -1, res[2]];
];

If[genMode == "su2rest",
  (* moving-frame A1 at P = (-1,0,0), as in the github set (dispersion check) *)
  Add1trOpCorrelators[-1, 0, 0, 1, 1, 1, res[1]];
  (* A1++ at rest: the notebook's "run" list (cell lines 463-561, CodeControl = "run") *)
  Do[Add1trOpCorrelators[0, 0, 0, 1, 1, 1, res[genK]], {genK, {1, 3, 5, 8, 11, 12, 16, 20}}];
  (* E++ at rest, 2 components; shapes from an O_h character estimate, see audit section 3 *)
  Do[Add1trOpCorrelators[0, 0, 0, 3, genE, 1, res[genK]], {genK, {1, 2, 4, 5}}, {genE, 1, 2}];
  (* T2++ at rest, 3 components; shapes from an O_h character estimate, see audit section 3 *)
  Do[Add1trOpCorrelators[0, 0, 0, 5, genE, 1, res[genK]], {genK, {3, 4, 5, 6}}, {genE, 1, 3}];
];

If[genMode == "su4",
  genSu4Shapes = {1, 2, 3, 4, 5, 6, 8, 11, 12, 16, 20};
  genSu4TorPaths = {P[ax], P[-ay, ax, ay], P[-ay, ax, az, ay, -az]};
  genSu4MomRep = {{0, 0, 1}, {0, 1, 1}, {1, 1, 1}};
  genSu4MomHalf = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1},
                   {1, 1, 0}, {1, -1, 0}, {1, 0, 1}, {1, 0, -1}, {0, 1, 1}, {0, 1, -1},
                   {1, 1, 1}, {1, 1, -1}, {1, -1, 1}, {-1, 1, 1}};
  genSu4Moms = Prepend[If[genMomSet == "half", genSu4MomHalf, genSu4MomRep], {0, 0, 0}];
  Print["su4 momenta: ", genSu4Moms];
  (* single trace: every irrep / component / charge of the little group of each momentum *)
  Do[
    genP = genSu4Moms[[genI]];
    genNirr = Length[bTOrthog[genP[[1]], genP[[2]], genP[[3]]]];
    Do[
      genDim = Length[bTOrthog[genP[[1]], genP[[2]], genP[[3]]][[genIr]]];
      Do[Add1trOpCorrelators[genP[[1]], genP[[2]], genP[[3]], genIr, genE, genC, res[genK]],
        {genK, genSu4Shapes}, {genE, 1, genDim}, {genC, {1, -1}}],
      {genIr, 1, genNirr}],
    {genI, 1, Length[genSu4Moms]}];
  (* torelons: same momenta, every irrep / component / charge, the three github paths *)
  Do[
    genP = genSu4Moms[[genI]];
    genNirr = Length[bTOrthog[genP[[1]], genP[[2]], genP[[3]]]];
    Do[
      genDim = Length[bTOrthog[genP[[1]], genP[[2]], genP[[3]]][[genIr]]];
      Do[AddTorCorrelators[genP[[1]], genP[[2]], genP[[3]], genIr, genE, genC, genTp],
        {genTp, genSu4TorPaths}, {genE, 1, genDim}, {genC, {1, -1}}],
      {genIr, 1, genNirr}],
    {genI, 1, Length[genSu4Moms]}];
];

If[genMode != "su4",
  (* two trace, notebook cell lines 1125-1271, CodeControl = "github" (github and su2rest) *)
  Add2trOpCorrelators[res[1], 0, 0, 1, 1, 1, res[1], 0, 0, 1, 1, 1, 0, 0, 0, 1, 1];
  Add2trOpCorrelators[res[1], 0, 0, 1, 1, 1, res[1], 0, 0, 1, 1, 1, 0, 0, 0, 3, 1];
  Add2trOpCorrelators[res[1], 0, 0, 1, 1, 1, res[1], 0, 0, 1, 1, 1, 0, 0, 0, 3, 2];

  (* torelons, notebook cell lines 2014-2147, CodeControl = "github" (github and su2rest) *)
  AddTorCorrelators[0, 0, 0, 1, 1, 1, P[ax]];
  AddTorCorrelators[0, 1, 0, 1, 1, 1, P[-ay, ax, ay]];
  AddTorCorrelators[1, 1, 1, 1, 1, 1, P[-ay, ax, ay]];
  AddTorCorrelators[0, 0, 0, 3, 1, 1, P[-ay, ax, az, ay, -az]];
  AddTorCorrelators[0, 0, 0, 3, 2, 1, P[-ay, ax, az, ay, -az]];
  AddTorCorrelators[0, 0, 0, 4, 1, 1, P[-ay, ax, az, ay, -az]];
  AddTorCorrelators[0, 0, 0, 4, 2, 1, P[-ay, ax, az, ay, -az]];
  AddTorCorrelators[0, 0, 0, 4, 3, 1, P[-ay, ax, az, ay, -az]];
];

GenerateCcode[];
Print["GenerateCcode done"];
Exit[0];
