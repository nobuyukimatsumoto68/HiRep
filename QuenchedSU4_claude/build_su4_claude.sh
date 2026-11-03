#!/bin/bash
#
# Build PureGauge/suN_glueballs_h5_claude with NG = 4 (no Grid).
# Make/MkFlags.ini and Make/build.pl must be the SU(4) versions (copies of Make/MkFlags_su4_claude.ini
# and Make/build_claude.pl); this script only checks that, it does not overwrite them.
# Toolchain = the PI workstation's tested setup: clang pinned to the GCC 13 libstdc++, openmpi_cuda.
#
# Usage: [NJOBS=4] bash QuenchedSU4_claude/build_su4_claude.sh
#
set -u
set -e
set -o pipefail

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
HIREP="$(cd "$HERE/.." && pwd)"
NJOBS=${NJOBS:-4}

export PATH=/mnt/hdd_barracuda/opt/openmpi_cuda/bin:$PATH
export LD_LIBRARY_PATH=/mnt/hdd_barracuda/opt/openmpi_cuda/lib:/mnt/hdd_barracuda/opt/myhdfstuff/hdf5-2.1.0/lib:${LD_LIBRARY_PATH:-}
GCC13_DIR=/usr/lib/gcc/x86_64-linux-gnu/13
export OMPI_CC="clang --gcc-install-dir=$GCC13_DIR"
export OMPI_CXX="clang++ --gcc-install-dir=$GCC13_DIR"

cd "$HIREP"
if ! cmp -s Make/MkFlags.ini Make/MkFlags_su4_claude.ini; then
    echo "Make/MkFlags.ini is not Make/MkFlags_su4_claude.ini: copy it yourself, then rerun"
    exit 1
fi
if ! cmp -s Make/build.pl Make/build_claude.pl; then
    echo "Make/build.pl is not Make/build_claude.pl: copy it yourself, then rerun"
    exit 1
fi
grep -E "^NG" Make/MkFlags.ini
echo "mpicc: $(which mpicc) -> $(mpicc --version | head -1)"
Make/nj -j "$NJOBS" PureGauge/suN_glueballs_h5_claude
ls -la PureGauge/suN_glueballs_h5_claude
