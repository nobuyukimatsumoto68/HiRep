#!/bin/bash
#
# Build the Grid-in-HiRep HMC bridge from this self-contained directory
# (HiRep/GridBridge_claude). Everything the bridge needs lives here; this script copies it
# into the HiRep tree, captures the Grid requisites out of a built Grid, and builds.
#
#   (0) resolve the built Grid ($GRID_BUILD); guard Nc == NG
#   (1) derive Grid's exact compiler/link flags from grid-config; record them
#   (2) compile grid_hirep_hmc_claude.cpp against Grid -> grid_hirep_hmc_claude.o
#   (3) capture out of Grid into HiRep/GridLibrary: libGrid.a, Config.h (+ .o, header)
#   (4) stage HiRep build config from THIS dir (MkFlags, build.pl, ScriptForMake, input)
#   (5) rebuild HiRep PureGauge_grid, linking the CAPTURED libGrid.a
#
# Read-only w.r.t. the Grid repo: we only READ $GRID_BUILD/... and copy OUT into HiRep.
#
# Usage:
#   GRID_BUILD=/abs/path/to/Grid/build  bash build_grid_hirep_claude.sh
# where $GRID_BUILD is the Grid --prefix build dir (holds grid-config, lib/libGrid.a,
# include/Grid/Config.h). Do NOT rely on symlinks -- give the real path.
#
set -u
set -e
set -o pipefail

SETUP_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
HIREP_ROOT="$(cd "$SETUP_DIR/.." && pwd)"
LOG="$SETUP_DIR/build_grid_hirep_claude.log"

# Point this at your real (non-symlink) Grid build --prefix dir.
GRID_BUILD="${GRID_BUILD:-/mnt/baracuda_14/grid_mpi/build}"
GRIDCONFIG="$GRID_BUILD/grid-config"
GRID_LIB_SRC="$GRID_BUILD/lib/libGrid.a"
GRID_CONFIG_H="$GRID_BUILD/include/Grid/Config.h"
GRID_CONFIGLOG="$GRID_BUILD/configure.log"
CAPTURE="$HIREP_ROOT/GridLibrary"
FLAGS_REC="$CAPTURE/grid_flags_claude.txt"

export PATH=/mnt/hdd_barracuda/opt/openmpi_cuda/bin:$PATH

echo "================ (0) resolve Grid + guard Nc == NG ================" 2>&1 | tee "$LOG"
echo "SETUP_DIR  : $SETUP_DIR" 2>&1 | tee -a "$LOG"
echo "HIREP_ROOT : $HIREP_ROOT" 2>&1 | tee -a "$LOG"
echo "GRID_BUILD : $GRID_BUILD" 2>&1 | tee -a "$LOG"
for f in "$GRIDCONFIG" "$GRID_LIB_SRC" "$GRID_CONFIG_H"; do
    if [ ! -e "$f" ]; then
        echo "MISSING Grid requisite: $f  (build Grid first, see README_claude.md)" 2>&1 | tee -a "$LOG"
        exit 1
    fi
done
NG="$(grep -E '^NG *= *[0-9]+' "$SETUP_DIR/MkFlags_claude.ini" | grep -oE '[0-9]+' | head -1)"
GRID_NC=""
if [ -f "$GRID_CONFIGLOG" ]; then
    GRID_NC="$(grep -E '^Nc' "$GRID_CONFIGLOG" | grep -oE '[0-9]+' | head -1)"
fi
echo "HiRep NG   : $NG" 2>&1 | tee -a "$LOG"
echo "Grid Nc    : ${GRID_NC:-unknown (no configure.log)}" 2>&1 | tee -a "$LOG"
if [ -n "$GRID_NC" ] && [ "$GRID_NC" != "$NG" ]; then
    echo "FATAL: Grid Nc=$GRID_NC != HiRep NG=$NG. Rebuild Grid with --enable-Nc=$NG" 2>&1 | tee -a "$LOG"
    echo "  (mismatch causes readout buffer overflow -> segfault + wrong SU(Nc) physics)." 2>&1 | tee -a "$LOG"
    exit 1
fi
echo "(0) OK" 2>&1 | tee -a "$LOG"

echo "================ (1) derive + record Grid flags ================" 2>&1 | tee -a "$LOG"
GRID_CXX="$("$GRIDCONFIG" --cxx)"
GRID_CXXFLAGS="$("$GRIDCONFIG" --cxxflags)"
GRID_LDFLAGS="$("$GRIDCONFIG" --ldflags)"
GRID_LIBS="$("$GRIDCONFIG" --libs)"
GRID_GIT="$("$GRIDCONFIG" --git 2>/dev/null || true)"
{
    echo "# Grid flags captured $(date) from $GRIDCONFIG"
    echo "# git: $GRID_GIT   Nc: ${GRID_NC:-unknown}"
    echo "CXX      = $GRID_CXX"
    echo "CXXFLAGS = $GRID_CXXFLAGS"
    echo "LDFLAGS  = $GRID_LDFLAGS"
    echo "LIBS     = $GRID_LIBS"
} > "$FLAGS_REC"
cat "$FLAGS_REC" 2>&1 | tee -a "$LOG"
echo "(1) OK" 2>&1 | tee -a "$LOG"

echo "================ (2) compile Grid bridge .o ================" 2>&1 | tee -a "$LOG"
$GRID_CXX $GRID_CXXFLAGS \
    -c "$SETUP_DIR"/grid_hirep_hmc_claude.cpp \
    -o "$SETUP_DIR"/grid_hirep_hmc_claude.o 2>&1 | tee -a "$LOG"
echo "(2) OK" 2>&1 | tee -a "$LOG"

echo "================ (3) capture Grid requisites into HiRep/GridLibrary ================" 2>&1 | tee -a "$LOG"
mkdir -p "$CAPTURE"
cp "$SETUP_DIR"/grid_hirep_hmc_claude.o "$CAPTURE"/grid_hirep_hmc_claude.o
cp "$SETUP_DIR"/grid_hirep_hmc_claude.h "$HIREP_ROOT"/Include/grid_hirep_hmc_claude.h
cp "$GRID_LIB_SRC"  "$CAPTURE"/libGrid.a
cp "$GRID_CONFIG_H" "$CAPTURE"/Config.h
echo "captured:" 2>&1 | tee -a "$LOG"
ls -la --time-style=full-iso "$CAPTURE"/libGrid.a "$CAPTURE"/Config.h "$CAPTURE"/grid_hirep_hmc_claude.o 2>&1 | tee -a "$LOG"
echo "(3) OK" 2>&1 | tee -a "$LOG"

echo "================ (4) stage HiRep build config from this dir ================" 2>&1 | tee -a "$LOG"
cp "$SETUP_DIR"/MkFlags_claude.ini                  "$HIREP_ROOT"/Make/MkFlags_claude.ini
cp "$SETUP_DIR"/build_claude.pl                    "$HIREP_ROOT"/Make/build_claude.pl
cp "$SETUP_DIR"/ScriptForMake_claude               "$HIREP_ROOT"/ScriptForMake
cp "$SETUP_DIR"/input_hmc_grid_4node_claude        "$HIREP_ROOT"/PureGauge/input_hmc_grid_4node_claude
cp "$SETUP_DIR"/input_hmc_glueballs_grid_100_claude "$HIREP_ROOT"/PureGauge/input_hmc_glueballs_grid_100_claude
echo "staged: MkFlags_claude.ini, build_claude.pl, ScriptForMake, input_hmc_grid_4node_claude, input_hmc_glueballs_grid_100_claude" 2>&1 | tee -a "$LOG"
echo "(4) OK" 2>&1 | tee -a "$LOG"

echo "================ (5) rebuild HiRep PureGauge_grid (links CAPTURED libGrid.a) ================" 2>&1 | tee -a "$LOG"
cd "$HIREP_ROOT"
bash ScriptForMake 2>&1 | tee -a "$LOG"
if [ ! -x PureGauge/hmc_grid_claude ]; then
    echo "BUILD FAILED: PureGauge/hmc_grid_claude not found" 2>&1 | tee -a "$LOG"
    exit 1
fi
echo "(5) OK" 2>&1 | tee -a "$LOG"
echo "BUILD DONE" 2>&1 | tee -a "$LOG"
