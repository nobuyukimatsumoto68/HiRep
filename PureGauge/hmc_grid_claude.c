#define MAIN_PROGRAM
#include "libhr.h"
#include "grid_hirep_hmc_claude.h"
#include <stdlib.h>

/* Parameters — replace with input_file parsing as needed */
static double betaF  = 5.0;
static double betaA  = 0.0;
static int    n_traj = 4;
static int    nMD    = 8;
static double trajL  = 1.0;

/*
 * Copy the full global gauge field from the Grid out[] buffer into HiRep's
 * u_gauge.  Each MPI rank extracts its own local sites from the global array.
 * out[] layout: T-outermost (t,x,y,z), 4 dirs, NG×NG complex row-major.
 * COORD[0..3] = process coordinates in T,X,Y,Z.
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
    start_sendrecv_suNg_field(u_gauge);
    complete_sendrecv_suNg_field(u_gauge);
    apply_BCs_on_fundamental_gauge_field();
}

int main(int argc, char *argv[])
{
    setup_process(&argc, &argv);
    setup_gauge_fields();

    int buf_len = GLB_T * GLB_X * GLB_Y * GLB_Z * 4 * 2 * NG * NG;
    double *out = malloc(buf_len * sizeof(double));

    struct HmcState *S = grid_hmc_init(
        NP_T, NP_X, NP_Y, NP_Z,
        GLB_T, GLB_X, GLB_Y, GLB_Z,
        betaF, betaA, nMD, trajL);

    // for (int traj = 0; traj < n_traj; traj++) {
    int traj=0;
    lprintf("HMC", 0, "Starting trajectory %d\n", traj);
    grid_hmc_step(S, out);
    copy_to_ugauge(out);
    lprintf("HMC", 0, "Trajectory %d done  plaquette = %.10f\n", traj, avr_plaquette());
    // }

    grid_hmc_finalize(S);

    free(out);

    finalize_process();
    return 0;
}
