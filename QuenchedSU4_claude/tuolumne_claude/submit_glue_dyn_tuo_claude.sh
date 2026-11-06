#!/bin/bash
#FLUX: -N 1
#FLUX: -t 12h
#FLUX: --exclusive
#FLUX: --job-name=glue_dyn
#
# Glueballs on the DYNAMICAL 24c SU(4) ensembles (Chunk 7 of
# QuenchedSU4_claude/quenched_su4_glueball_impl_plan_claude.md): one node, one 24-rank measure-only run per
# ensemble, all ensembles of ENSEMBLES side by side (4 x 24 ranks fit on a 96-core node).
#   configs : /p/lustre5/matsumoto5/conf_nc4nf1_2448/conf_nc4nf1_2448_<ens>/conf_nc4nf1_2448_<ens>_lat.<traj>,
#             traj >= TMIN and traj % STRIDE == 0 (defaults 1000, 20)
#   output  : /p/lustre5/matsumoto5/obs_glue_2448_<ens>/glue_2448_<ens>_claude.h5 (+ .bak), next to the meson
#             (obs_nc4nf1_2448/) and baryon (obs_bvar_2448_*) observables
# Restart = resubmit: configurations already in the h5 are skipped. The config list and the input of an
# ensemble are written only if absent (delete them yourself to regenerate).
#
# Submit from a tuolumne login node (HIREP = repo clone with the tuolumne build):
#   HIREP=/path/to/hirep_su4 flux batch QuenchedSU4_claude/tuolumne_claude/submit_glue_dyn_tuo_claude.sh
#   HIREP=... ENSEMBLES="b10p990_m0p2000 b10p865_m0p1000" flux batch ...        (other ensembles)
#   chain:  prev=$(HIREP=... flux batch ...) ; HIREP=... flux batch --dependency=afterany:$prev ...
#
set -u

HIREP=${HIREP:?set HIREP to the repo clone}
ENSEMBLES=${ENSEMBLES:-b11p035_m0p3000 b11p045_m0p4000}
TMIN=${TMIN:-1000}
STRIDE=${STRIDE:-20}
NRANK=24
EXE=$HIREP/PureGauge/suN_glueballs_h5_claude
TEMPLATE=$HIREP/QuenchedSU4_claude/tuolumne_claude/input_su4_dyn24c_claude
CONFROOT=/p/lustre5/matsumoto5/conf_nc4nf1_2448
OBSROOT=/p/lustre5/matsumoto5

module load cray-hdf5/1.14.3.7
export HDF5_USE_FILE_LOCKING=FALSE
export OMP_NUM_THREADS=1

echo "job ${FLUX_JOB_ID:-?} on $(hostname) at $(date): ENSEMBLES=$ENSEMBLES TMIN=$TMIN STRIDE=$STRIDE"
if [ ! -x "$EXE" ]; then
    echo "NOT BUILT: $EXE (run QuenchedSU4_claude/tuolumne_claude/build_su4_tuo_claude.sh)"
    exit 1
fi

pids=""
for ens in $ENSEMBLES; do
    CDIR=$CONFROOT/conf_nc4nf1_2448_$ens
    ODIR=$OBSROOT/obs_glue_2448_$ens
    mkdir -p "$ODIR"
    LIST=$ODIR/configlist_claude.txt
    INPUT=$ODIR/input_glue_2448_${ens}_claude
    if [ ! -f "$LIST" ]; then
        ls -1 $CDIR/conf_nc4nf1_2448_${ens}_lat.* | awk -F. -v tmin=$TMIN -v st=$STRIDE \
            '{n = $NF + 0; if (n >= tmin && n % st == 0) print n, $0}' | sort -n | awk '{print $2}' > "$LIST"
    fi
    if [ ! -f "$INPUT" ]; then
        sed -e "s#CONFIGLIST#$LIST#" -e "s#H5FILE#$ODIR/glue_2448_${ens}_claude.h5#" "$TEMPLATE" > "$INPUT"
    fi
    echo "$ens: $(wc -l < "$LIST") configs in $LIST (first $(head -1 "$LIST" | sed 's/.*\.//'), last $(tail -1 "$LIST" | sed 's/.*\.//'))"
    cd "$ODIR"
    flux run -N 1 -n $NRANK -c 1 "$EXE" -i "$INPUT" -o out_glue_${FLUX_JOB_ID:-local} > flux_run_${FLUX_JOB_ID:-local}_claude.log 2>&1 &
    pids="$pids $!"
done
rc=0
for p in $pids; do
    wait $p || rc=1
done
echo "job ${FLUX_JOB_ID:-?} finished at $(date), rc=$rc"
for ens in $ENSEMBLES; do
    ODIR=$OBSROOT/obs_glue_2448_$ens
    grep -h "measure-only mode\|done:\|ERROR" $ODIR/out_glue_${FLUX_JOB_ID:-local}_0 2>/dev/null | sed "s/^/$ens: /"
done
exit $rc
