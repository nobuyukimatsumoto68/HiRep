#!/bin/bash
#
# Example: build upstream Grid for the Grid-in-HiRep bridge on a module-based cluster (no root).
#   Grid repo: https://github.com/paboyle/Grid.git (upstream; no fork needed)
#
# REQUIRED: --enable-Nc=2, so Grid's Nc equals HiRep's NG = 2 (MkFlags_claude.ini). A mismatch
# overflows the gauge-field transfer buffer (segfault) and simulates the wrong group.
#
# Usage (edit the module lines and variables first, or export the variables before running):
#   GRID_ROOT=$HOME/grid SIMD=AVX2 bash build_grid_example_claude.sh
# Result: $GRID_ROOT/build holds grid-config, lib/libGrid.a, include/Grid/Config.h, configure.log.
# Next:   GRID_BUILD=$GRID_ROOT/build bash GridBridge_claude/build_grid_hirep_claude.sh
#
# Building on a login node is usually fine; if the cluster forbids long compiles there, run this
# inside an interactive allocation. Compiler: its libstdc++ should be older than 15 (HiRep's
# Make/Utils/autosun header generator does not compile with libstdc++ 15; see README_claude.md).
#
set -u
set -e
set -o pipefail

# ---- 1. environment: adapt to the cluster ---------------------------------------------------
# module load gcc openmpi hdf5 gmp mpfr fftw openssl      # names/versions differ per cluster
# Dependency prefixes: most module systems export them (e.g. $HDF5_DIR, $GMP_ROOT); set explicitly
# if not. HDF5 must include the C++ API (H5Cpp.h, libhdf5_cpp): Grid.h includes H5Cpp.h.
GRID_ROOT=${GRID_ROOT:-$HOME/grid}             # Grid clone goes to $GRID_ROOT/Grid, build to $GRID_ROOT/build
SIMD=${SIMD:-AVX2}                             # AVX2 / AVX512 / ... for the compute nodes' CPUs
MAKE_JOBS=${MAKE_JOBS:-8}
MPICC=${MPICC:-mpicc}
MPICXX=${MPICXX:-mpicxx}
HDF5_PREFIX=${HDF5_PREFIX:-${HDF5_DIR:-}}
GMP_PREFIX=${GMP_PREFIX:-}
MPFR_PREFIX=${MPFR_PREFIX:-}
FFTW_PREFIX=${FFTW_PREFIX:-}
OPENSSL_PREFIX=${OPENSSL_PREFIX:-}
LIME_PREFIX=${LIME_PREFIX:-$GRID_ROOT/lime}     # c-lime is rarely a module; built below if absent
LIME_VERSION=1.3.2

GRID_SRC=$GRID_ROOT/Grid
GRID_BUILD=$GRID_ROOT/build
mkdir -p "$GRID_ROOT"
LOG=$GRID_ROOT/build_grid_example_claude.log
echo "GRID_ROOT=$GRID_ROOT SIMD=$SIMD MPICC=$MPICC MPICXX=$MPICXX" 2>&1 | tee "$LOG"
echo "HDF5=$HDF5_PREFIX GMP=$GMP_PREFIX MPFR=$MPFR_PREFIX FFTW=$FFTW_PREFIX OPENSSL=$OPENSSL_PREFIX LIME=$LIME_PREFIX" 2>&1 | tee -a "$LOG"

# ---- 2. c-lime (only if not installed) --------------------------------------------------------
if [ ! -f "$LIME_PREFIX/include/lime.h" ]; then
    echo "==== building c-lime $LIME_VERSION into $LIME_PREFIX ====" 2>&1 | tee -a "$LOG"
    cd "$GRID_ROOT"
    if [ ! -d lime-$LIME_VERSION ]; then
        wget -q http://usqcd-software.github.io/downloads/c-lime/lime-$LIME_VERSION.tar.gz
        tar xzf lime-$LIME_VERSION.tar.gz
    fi
    cd lime-$LIME_VERSION
    ./configure --prefix="$LIME_PREFIX" CC=gcc 2>&1 | tee -a "$LOG"
    make -j "$MAKE_JOBS" 2>&1 | tee -a "$LOG"
    make install 2>&1 | tee -a "$LOG"
fi

# ---- 3. Grid source --------------------------------------------------------------------------
cd "$GRID_ROOT"
if [ ! -d "$GRID_SRC" ]; then
    git clone https://github.com/paboyle/Grid.git "$GRID_SRC"
fi
cd "$GRID_SRC"
if [ ! -x configure ]; then
    ./bootstrap.sh 2>&1 | tee -a "$LOG"
fi

# ---- 4. configure (Nc = 2) and build ---------------------------------------------------------
WITH_OPTS="--with-lime=$LIME_PREFIX"
if [ -n "$HDF5_PREFIX" ]; then
    WITH_OPTS="$WITH_OPTS --with-hdf5=$HDF5_PREFIX"
fi
if [ -n "$GMP_PREFIX" ]; then
    WITH_OPTS="$WITH_OPTS --with-gmp=$GMP_PREFIX"
fi
if [ -n "$MPFR_PREFIX" ]; then
    WITH_OPTS="$WITH_OPTS --with-mpfr=$MPFR_PREFIX"
fi
if [ -n "$FFTW_PREFIX" ]; then
    WITH_OPTS="$WITH_OPTS --with-fftw=$FFTW_PREFIX"
fi
if [ -n "$OPENSSL_PREFIX" ]; then
    WITH_OPTS="$WITH_OPTS --with-openssl=$OPENSSL_PREFIX"
fi

mkdir -p "$GRID_BUILD"
cd "$GRID_BUILD"
echo "==== configure Grid ====" 2>&1 | tee -a "$LOG"
# The configure output is also saved as $GRID_BUILD/configure.log: it must report "Nc : 2", and
# build_grid_hirep_claude.sh reads it to check Grid Nc == HiRep NG.
"$GRID_SRC"/configure \
    --prefix="$GRID_BUILD" \
    $WITH_OPTS \
    MPICC="$MPICC" MPICXX="$MPICXX" \
    CXXFLAGS="-std=c++17 -fopenmp" \
    --enable-comms=mpi-auto \
    --enable-simd="$SIMD" \
    --enable-openmp \
    --enable-Nc=2 \
    --enable-unified=no \
    --disable-gparity \
    --disable-fermion-reps \
    2>&1 | tee "$GRID_BUILD"/configure.log | tee -a "$LOG"

echo "==== make + install ====" 2>&1 | tee -a "$LOG"
make -j "$MAKE_JOBS" 2>&1 | tee -a "$LOG"
make install 2>&1 | tee -a "$LOG"

echo "==== check ====" 2>&1 | tee -a "$LOG"
grep -E '^Nc' "$GRID_BUILD"/configure.log 2>&1 | tee -a "$LOG"
ls -la "$GRID_BUILD"/grid-config "$GRID_BUILD"/lib/libGrid.a "$GRID_BUILD"/include/Grid/Config.h 2>&1 | tee -a "$LOG"
echo "DONE: next run  GRID_BUILD=$GRID_BUILD bash GridBridge_claude/build_grid_hirep_claude.sh" 2>&1 | tee -a "$LOG"
