#!/bin/bash
#
# Build PureGauge/suN_glueballs_h5_claude (NG = 4, CPU) on a tuolumne LOGIN node.
# Plan: QuenchedSU4_claude/quenched_su4_glueball_impl_plan_claude.md (Chunk 7).
# Before the first build, in the repo clone on tuolumne, do by hand (this script never overwrites them):
#   cp Make/MkFlags_su4_tuo_claude.ini Make/MkFlags.ini
#   cp Make/build_claude.pl Make/build.pl        (normally already identical)
# Usage (from the repo root or anywhere):  bash QuenchedSU4_claude/tuolumne_claude/build_su4_tuo_claude.sh
# Log: QuenchedSU4_claude/tuolumne_claude/build_su4_tuo_claude.log
#
set -u
set -e
set -o pipefail

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
HIREP="$(cd "$HERE/../.." && pwd)"
LOG=$HERE/build_su4_tuo_claude.log
NJOBS=${NJOBS:-8}

module load cray-hdf5/1.14.3.7

echo "================ build on $(hostname) ($(date)) ================" 2>&1 | tee "$LOG"
module list 2>&1 | tee -a "$LOG"
cd "$HIREP"
if ! cmp -s Make/MkFlags.ini Make/MkFlags_su4_tuo_claude.ini; then
    echo "Make/MkFlags.ini is not Make/MkFlags_su4_tuo_claude.ini: copy it yourself (see the header), then rerun" 2>&1 | tee -a "$LOG"
    exit 1
fi
if ! cmp -s Make/build.pl Make/build_claude.pl; then
    echo "Make/build.pl is not Make/build_claude.pl: copy it yourself, then rerun" 2>&1 | tee -a "$LOG"
    exit 1
fi
if ! command -v ninja > /dev/null; then
    echo "ninja not found in PATH (needed by Make/nj): e.g. pip install --user ninja, then rerun" 2>&1 | tee -a "$LOG"
    exit 1
fi
echo "cc: $(which cc)   ninja: $(which ninja) $(ninja --version)" 2>&1 | tee -a "$LOG"
grep -E "^NG|^CC|^MPICC" Make/MkFlags.ini 2>&1 | tee -a "$LOG"
set +e
Make/nj -j "$NJOBS" PureGauge/suN_glueballs_h5_claude 2>&1 | tee -a "$LOG"
RC=${PIPESTATUS[0]}
set -e
echo "build exit code: $RC" 2>&1 | tee -a "$LOG"
ls -la PureGauge/suN_glueballs_h5_claude 2>&1 | tee -a "$LOG" || true
exit "$RC"
