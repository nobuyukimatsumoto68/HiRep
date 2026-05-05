/***************************************************************************\
* Copyright (c) 2025
* All rights reserved.
\***************************************************************************/

/*
 * Grid HMC + HiRep glueball/torellon 1-point functions.
 * Runs Grid HMC trajectories in memory; after each trajectory copies the
 * gauge field into u_gauge and measures glueball/torellon operators.
 */

#define MAIN_PROGRAM
#include "libhr.h"
#include "suN_utils.h"
#include "grid_hirep_hmc_claude.h"
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

/* HMC parameters read from input file */
typedef struct input_hmc_grid {
    double betaF, betaA;
    int nMD, n_traj, nOMP;
    double trajL;
    input_record_t read[7];
} input_hmc_grid;

#define init_input_hmc_grid(varname)                                                   \
    {                                                                                  \
        .betaF = 5.0, .betaA = 0.0, .nMD = 10, .n_traj = 4, .nOMP = 1, .trajL = 1.0, \
        .read = {                                                                      \
            { "betaF", "betaF = %lf", DOUBLE_T, &(varname).betaF },                   \
            { "betaA", "betaA = %lf", DOUBLE_T, &(varname).betaA },                   \
            { "nMD",   "nMD = %d",   INT_T,    &(varname).nMD },                      \
            { "n_traj","n_traj = %d",INT_T,    &(varname).n_traj },                   \
            { "trajL", "trajL = %lf",DOUBLE_T, &(varname).trajL },                    \
            { "nOMP",  "nOMP = %d",  INT_T,    &(varname).nOMP },                     \
            { NULL, NULL, INT_T, NULL }                                                \
        }                                                                              \
    }

static input_hmc_grid hmc_par = init_input_hmc_grid(hmc_par);
pg_flow_glueballs_measure flow = init_pg_flow_glueballs_measure(flow);

/*
 * Copy global gauge field from Grid out[] buffer into HiRep u_gauge.
 * out[] layout: T-outermost (t,x,y,z), 4 dirs, NG×NG complex row-major.
 */
static void copy_to_ugauge(const double *out)
{
    int T = GLB_T / NP_T, X = GLB_X / NP_X;
    int Y = GLB_Y / NP_Y, Z = GLB_Z / NP_Z;

    for (int lt = 0; lt < T; lt++)
    for (int lx = 0; lx < X; lx++)
    for (int ly = 0; ly < Y; ly++)
    for (int lz = 0; lz < Z; lz++) {
        int gt = COORD[0] * T + lt, gx = COORD[1] * X + lx;
        int gy = COORD[2] * Y + ly, gz = COORD[3] * Z + lz;
        int site = ipt(lt, lx, ly, lz);
        for (int mu = 0; mu < 4; mu++) {
            suNg *u = pu_gauge(site, mu);
            const double *p = out
                + (((gt * GLB_X + gx) * GLB_Y + gy) * GLB_Z + gz) * 4 * 2 * NG * NG
                + mu * 2 * NG * NG;
            for (int i = 0; i < NG * NG; i++) {
                __real__ u->c[i] = p[2 * i];
                __imag__ u->c[i] = p[2 * i + 1];
            }
        }
    }
}

int main(int argc, char *argv[])
{
    struct timeval start, end, etime;

    setup_process(&argc, &argv);
    setup_gauge_fields();

    init_mk_glueballs(&flow, get_input_filename());
    read_input(hmc_par.read, get_input_filename());

    int nblocking = flow.pg_v->nblkend - flow.pg_v->nblkstart + 1;

    hr_complex *one_point_gb = (hr_complex *)amalloc(sizeof(hr_complex) * total_n_glue_op * nblocking * n_active_slices, ALIGN);
    error(one_point_gb == NULL, 1, __func__, "Could not allocate memory space for field structure");
    hr_complex *one_point_tor = (hr_complex *)amalloc(sizeof(hr_complex) * total_n_tor_op * n_active_slices, ALIGN);
    error(one_point_tor == NULL, 1, __func__, "Could not allocate memory space for field structure");
    hr_complex **polyf = (hr_complex **)amalloc(sizeof(hr_complex *) * 3, ALIGN);
    error(polyf == NULL, 1, __func__, "Could not allocate memory space for field structure");
    polyf[0] = (hr_complex *)amalloc(sizeof(hr_complex) * Y * Z * T, ALIGN);
    error(polyf[0] == NULL, 1, __func__, "Could not allocate memory space for field structure");
    polyf[1] = (hr_complex *)amalloc(sizeof(hr_complex) * X * Z * T, ALIGN);
    error(polyf[1] == NULL, 1, __func__, "Could not allocate memory space for field structure");
    polyf[2] = (hr_complex *)amalloc(sizeof(hr_complex) * X * Y * T, ALIGN);
    error(polyf[2] == NULL, 1, __func__, "Could not allocate memory space for field structure");

    size_t buf_len = (size_t)GLB_T * GLB_X * GLB_Y * GLB_Z * 4 * 2 * NG * NG;
    double *buf = malloc(buf_len * sizeof(double));
    error(buf == NULL, 1, "main", "Cannot allocate gauge field buffer");

    struct HmcState *S = grid_hmc_init(
        NP_T, NP_X, NP_Y, NP_Z,
        GLB_T, GLB_X, GLB_Y, GLB_Z,
        hmc_par.betaF, hmc_par.betaA, hmc_par.nMD, hmc_par.trajL, hmc_par.nOMP);

    lprintf("MAIN", 0, "betaF=%.4f betaA=%.4f nMD=%d trajL=%.4f n_traj=%d nOMP=%d\n",
            hmc_par.betaF, hmc_par.betaA, hmc_par.nMD, hmc_par.trajL, hmc_par.n_traj, hmc_par.nOMP);

    for (int traj = 0; traj < hmc_par.n_traj; traj++) {
        lprintf("HMC", 0, "Starting trajectory %d\n", traj);

        grid_hmc_step(S, buf);
        copy_to_ugauge(buf);
        start_sendrecv_suNg_field(u_gauge);
        complete_sendrecv_suNg_field(u_gauge);
        apply_BCs_on_fundamental_gauge_field();

        lprintf("HMC", 0, "Trajectory %d  plaq = %.10e\n", traj, avr_plaquette());

        gettimeofday(&start, 0);

#if total_n_glue_op > 0
        for (int i = 0; i < total_n_glue_op * nblocking * n_active_slices; i++) {
            one_point_gb[i] = 0.;
        }
        measure_1pt_glueballs(flow.pg_v->nblkstart, flow.pg_v->nblkend, &(flow.pg_v->APEsmear), one_point_gb);
        for (int j = 0; j < n_active_slices; j++) {
            lprintf("step", 0, "%d ", zerocoord[0] + j);
            for (int i = 0; i < total_n_glue_op * nblocking; i++) {
                lprintf("step", 0, " (%.10e +I*(%.10e)) ", creal(one_point_gb[i]), cimag(one_point_gb[i]));
            }
            lprintf("step", 0, "\n");
        }
#endif

#if total_n_tor_op > 0
        for (int i = 0; i < total_n_tor_op * n_active_slices; i++) {
            one_point_tor[i] = 0.;
        }
        for (int i = 0; i < Y * Z * T; i++) { polyf[0][i] = 0.; }
        for (int i = 0; i < X * Z * T; i++) { polyf[1][i] = 0.; }
        for (int i = 0; i < X * Y * T; i++) { polyf[2][i] = 0.; }
        measure_1pt_torellons(&(flow.pg_v->APEsmear), one_point_tor, polyf);
#endif

        gettimeofday(&end, 0);
        timeval_subtract(&etime, &end, &start);
        lprintf("MAIN", 0, "Glueballs & Torellons 1pt traj %d: generated in [%ld sec %ld usec]\n",
                traj, etime.tv_sec, etime.tv_usec);
        lprintf("MAIN", 0, "Plaquette %1.18e\n", avr_plaquette());

        if (strcmp(flow.wf->make, "true") == 0) {
            static suNg_field *Vwf = NULL;
            if (Vwf == NULL) { Vwf = alloc_suNg_field(&glattice); }
            gettimeofday(&start, 0);
            copy_suNg_field(Vwf, u_gauge);
            WF_update_and_measure(RK3_ADAPTIVE, Vwf, &(flow.wf->tmax), &(flow.wf->eps),
                                  &(flow.wf->delta), flow.wf->nmeas, DONTSTORE);
            gettimeofday(&end, 0);
            timeval_subtract(&etime, &end, &start);
            lprintf("MAIN", 0, "WF Measure traj %d: generated in [%ld sec %ld usec]\n",
                    traj, etime.tv_sec, etime.tv_usec);
        }

        if (strcmp(flow.poly->make, "true") == 0) {
            gettimeofday(&start, 0);
            polyakov();
            gettimeofday(&end, 0);
            timeval_subtract(&etime, &end, &start);
            lprintf("MAIN", 0, "Polyakov Measure traj %d: generated in [%ld sec %ld usec]\n",
                    traj, etime.tv_sec, etime.tv_usec);
        }
    }

    grid_hmc_finalize(S);
    free(buf);
    afree(one_point_gb);
    afree(one_point_tor);
    afree(polyf[0]);
    afree(polyf[1]);
    afree(polyf[2]);
    afree(polyf);

    finalize_process();
    return 0;
}
