#!/bin/bash
#
# Regenerate HiRep's glueball operator code with Mathematica 15 (headless), in STAGING trees only.
# Audit + recipe: HiRep/GridBridge_claude/glueball_generator_audit_claude.md
#
#   PHASE 0  check the Wolfram kernel starts (needs a one-time activation, see below)
#   PHASE 1  mode "github" : regenerate the original CI set -> compare with commit 44939eac (the last
#                            commit holding it; HEAD now holds the su2rest set)
#                            (whitespace-insensitive), i.e. does Mathematica 15 reproduce 11.3?
#   PHASE 2  mode "su2rest": new set (A1++ x 8 shapes, E++, T2++ at rest; C=+ only) -> summary
#
# Nothing under HiRep/LibHR, HiRep/Include or HiRep/TestProgram is written: each mode runs in
# HiRep/GridBridge_claude/glueball_gen_claude/stage_<mode>/, which mirrors that layout, with an
# copy of the PATCHED generator glueball_gen_claude/OpProjection_claude.m (shift-phase / blocking fix,
# glueball_generator_audit_claude.md section 6) whose topdir="../../.." resolves to the stage dir.
#
# Optional: CLANG_FORMAT=/path/to/clang-format (version 17, as HiRep CI; pip install clang-format==17.0.6)
# formats the stage outputs with HiRep/.clang-format, which reproduces the committed formatting.
# NOTE: regenerating overwrites the tracked stage_su2rest/ files; format them before committing.
#
# BEFORE the first run, activate Mathematica once, interactively (not inside this script):
#   <path to>/wolframscript -activate
#
# Usage (from anywhere):
#   WOLFRAMSCRIPT=/path/to/wolframscript bash HiRep/GridBridge_claude/glueball_gen_claude/run_gen_glueball_ops_claude.sh
# Log: HiRep/GridBridge_claude/glueball_gen_claude/gen_glueball_ops_claude.log
#
set -u
set -e
set -o pipefail

GEN="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
HIREP="$(cd "$GEN/../.." && pwd)"
LOG=$GEN/gen_glueball_ops_claude.log
# default: the PI's workstation install (Mathematica 15.0.1)
WS=${WOLFRAMSCRIPT:-/mnt/hdd_barracuda/mathematica/15.0/Executables/wolframscript}
WLS=$GEN/gen_glueball_ops_claude.wls
# SRC_M=$HIREP/Make/Utils/Math_glueballs/OpProjection.m
# patched copy with the shift-phase / blocking fix (glueball_generator_audit_claude.md section 6)
SRC_M=$GEN/OpProjection_claude.m
# generated files, relative to a HiRep-like root
GENFILES="LibHR/Observables/glueballs_op.c LibHR/Observables/torellons_op.c Include/Observables/glueballs.h TestProgram/Utils/check_utils_3_gb_functions.c TestProgram/Utils/check_utils_3_tor_functions.c"

echo "================ PHASE 0: Wolfram kernel ================" 2>&1 | tee "$LOG"
echo "date : $(date)" 2>&1 | tee -a "$LOG"
echo "wolframscript : $WS" 2>&1 | tee -a "$LOG"
"$WS" -code 'Print[$Version]; Print["licensed: ", $LicenseID]' < /dev/null 2>&1 | tee -a "$LOG"
echo "PHASE 0 OK" 2>&1 | tee -a "$LOG"

for MODE in github su2rest; do
    STAGE=$GEN/stage_$MODE
    echo "================ mode $MODE: stage $STAGE ================" 2>&1 | tee -a "$LOG"
    mkdir -p "$STAGE"/Make/Utils/Math_glueballs "$STAGE"/LibHR/Observables "$STAGE"/Include/Observables "$STAGE"/TestProgram/Utils
    cp "$SRC_M" "$STAGE"/Make/Utils/Math_glueballs/OpProjection_claude.m
    T0=$SECONDS
    set +e
    "$WS" -file "$WLS" "$STAGE"/Make/Utils/Math_glueballs "$MODE" < /dev/null 2>&1 | tee -a "$LOG"
    RC=${PIPESTATUS[0]}
    set -e
    echo "wolframscript exit code: $RC   time: $((SECONDS - T0)) s" 2>&1 | tee -a "$LOG"
    if [ "$RC" -ne 0 ]; then
        echo "MODE $MODE FAILED" 2>&1 | tee -a "$LOG"
        exit 1
    fi
    for F in $GENFILES; do
        if [ -f "$STAGE/$F" ]; then
            echo "  generated $F : $(wc -l < "$STAGE/$F") lines" 2>&1 | tee -a "$LOG"
        else
            echo "  MISSING   $F" 2>&1 | tee -a "$LOG"
        fi
    done
    grep -E "define +total_n_(glue|tor)_op" "$STAGE"/Include/Observables/glueballs.h 2>&1 | tee -a "$LOG" || true
done

echo "================ PHASE 1 check: github stage vs commit 44939eac (ignoring whitespace and braces) ================" 2>&1 | tee -a "$LOG"
# With the patched generator, glueballs_op.c and glueballs.h are EXPECTED to differ, only in the shift-phase
# lines (c<N> = ..., gb_shift_unit) and the extern declaration; the other files must be SAME.
# The committed files were clang-formatted (HiRep .clang-format: InsertBraces true, BreakStringLiterals
# false), so compare token streams with whitespace AND braces removed.
for F in $GENFILES; do
    mkdir -p "$GEN"/stage_github/committed_ref
    A=$GEN/stage_github/committed_ref/$(basename "$F")
    # 44939eac = last commit with the original 15-op CI set in LibHR/Include/TestProgram
    git -C "$HIREP" show 44939eac:"$F" > "$A"
    B=$GEN/stage_github/$F
    if cmp -s <(tr -d ' \t\n{}' < "$A") <(tr -d ' \t\n{}' < "$B"); then
        echo "  SAME    $F" 2>&1 | tee -a "$LOG"
    else
        echo "  DIFFER  $F" 2>&1 | tee -a "$LOG"
        diff <(tr -s ' \t' ' ' < "$A" | tr ';' '\n') <(tr -s ' \t' ' ' < "$B" | tr ';' '\n') | head -60 2>&1 | tee -a "$LOG" || true
    fi
done

echo "================ PHASE 2 summary: su2rest channel table ================" 2>&1 | tee -a "$LOG"
# string literals of report_gb_group_setup(): one line per multiplet, then its |index=path| list
awk '/void report_gb_group_setup/,/^}/' "$GEN"/stage_su2rest/LibHR/Observables/glueballs_op.c \
    | grep -oE '"[^"]*"' | grep -v '"INIT Measure ML"' 2>&1 | tee -a "$LOG" || true
if [ -n "${CLANG_FORMAT:-}" ]; then
    echo "================ clang-format stage outputs ($CLANG_FORMAT) ================" 2>&1 | tee -a "$LOG"
    for MODE in github su2rest; do
        for F in $GENFILES; do
            "$CLANG_FORMAT" --style=file:"$HIREP"/.clang-format -i "$GEN"/stage_$MODE/"$F"
        done
    done
    echo "formatted" 2>&1 | tee -a "$LOG"
fi
echo "DONE" 2>&1 | tee -a "$LOG"
