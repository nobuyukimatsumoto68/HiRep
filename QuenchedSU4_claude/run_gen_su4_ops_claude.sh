#!/bin/bash
#
# Generate the quenched SU(4) glueball/torelon operator set (generator mode "su4", momset "half")
# in a STAGING tree, clang-format it, and (with INSTALL=1) copy it over the HiRep files.
# Plan: QuenchedSU4_claude/quenched_su4_glueball_impl_plan_claude.md (Chunk 1, Q2/Q7).
# Generator: A. Rago's OpProjection.m, patched copy
# GridBridge_claude/glueball_gen_claude/OpProjection_claude.m (shift-phase / blocking fix);
# operators = Wilson loops projected onto O_h irreps and little groups (B. Berg and A. Billoire,
# Nucl. Phys. B221 (1983) 109; R. C. Johnson, Phys. Lett. B114 (1982) 147).
#
# Usage:
#   WOLFRAMSCRIPT=... CLANG_FORMAT=... [MOMSET=half] [INSTALL=1] bash QuenchedSU4_claude/run_gen_su4_ops_claude.sh
# Log: QuenchedSU4_claude/run_gen_su4_ops_claude.log
#
set -u
set -e
set -o pipefail

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
HIREP="$(cd "$HERE/.." && pwd)"
GEN=$HIREP/GridBridge_claude/glueball_gen_claude
LOG=$HERE/run_gen_su4_ops_claude.log
WS=${WOLFRAMSCRIPT:-/mnt/hdd_barracuda/mathematica/15.0/Executables/wolframscript}
CF=${CLANG_FORMAT:-}
MOMSET=${MOMSET:-half}
INSTALL=${INSTALL:-0}
STAGE=$HERE/stage_su4
GENFILES="LibHR/Observables/glueballs_op.c LibHR/Observables/torellons_op.c Include/Observables/glueballs.h TestProgram/Utils/check_utils_3_gb_functions.c TestProgram/Utils/check_utils_3_tor_functions.c"

echo "================ generate su4 $MOMSET ($(date)) ================" 2>&1 | tee "$LOG"
mkdir -p "$STAGE"/Make/Utils/Math_glueballs "$STAGE"/LibHR/Observables "$STAGE"/Include/Observables "$STAGE"/TestProgram/Utils
cp "$GEN"/OpProjection_claude.m "$STAGE"/Make/Utils/Math_glueballs/OpProjection_claude.m
T0=$SECONDS
set +e
"$WS" -file "$GEN"/gen_glueball_ops_claude.wls "$STAGE"/Make/Utils/Math_glueballs su4 "$MOMSET" < /dev/null > "$HERE"/run_gen_su4_ops_wolfram_claude.log 2>&1
RC=$?
set -e
echo "wolframscript exit code: $RC   time: $((SECONDS - T0)) s (full output: run_gen_su4_ops_wolfram_claude.log)" 2>&1 | tee -a "$LOG"
if [ "$RC" -ne 0 ]; then
    tail -20 "$HERE"/run_gen_su4_ops_wolfram_claude.log 2>&1 | tee -a "$LOG"
    echo "GENERATION FAILED" 2>&1 | tee -a "$LOG"
    exit 1
fi
for F in $GENFILES; do
    if [ ! -f "$STAGE/$F" ]; then
        echo "MISSING $F" 2>&1 | tee -a "$LOG"
        exit 1
    fi
    echo "  generated $F : $(wc -l < "$STAGE/$F") lines" 2>&1 | tee -a "$LOG"
done
grep -E "define +total_n_(glue|tor)_op" "$STAGE"/Include/Observables/glueballs.h 2>&1 | tee -a "$LOG"

if [ -n "$CF" ]; then
    echo "================ clang-format ($("$CF" --version)) ================" 2>&1 | tee -a "$LOG"
    for F in $GENFILES; do
        "$CF" --style=file:"$HIREP"/.clang-format -i "$STAGE/$F"
    done
    echo "formatted" 2>&1 | tee -a "$LOG"
else
    echo "CLANG_FORMAT not set: stage files left unformatted" 2>&1 | tee -a "$LOG"
fi

if [ "$INSTALL" = "1" ]; then
    echo "================ install into $HIREP ================" 2>&1 | tee -a "$LOG"
    for F in $GENFILES; do
        cp "$STAGE/$F" "$HIREP/$F"
        echo "  installed $F" 2>&1 | tee -a "$LOG"
    done
fi
echo "DONE" 2>&1 | tee -a "$LOG"
