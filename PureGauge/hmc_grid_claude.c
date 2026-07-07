#define MAIN_PROGRAM
#include "libhr.h"
#include "grid_hirep_hmc_claude.h"
#include <stdlib.h>
#include <assert.h>
#include <math.h>

typedef struct input_hmc_grid {
    double betaF, betaA;
    int nMD, n_traj, nOMP;
    double trajL;
    int coldStart, NoMetropolisUntilRoutine;
    input_record_t read[9];
} input_hmc_grid;

#define init_input_hmc_grid(varname)                                                    \
    {                                                                                   \
        .betaF = 5.0, .betaA = 0.0, .nMD = 8, .n_traj = 4, .nOMP = 1, .trajL = 1.0,  \
        .coldStart = 0, .NoMetropolisUntilRoutine = 10,                                \
        .read = {                                                                       \
            { "betaF", "betaF = %lf", DOUBLE_T, &(varname).betaF },                    \
            { "betaA", "betaA = %lf", DOUBLE_T, &(varname).betaA },                    \
            { "nMD",   "nMD = %d",   INT_T,    &(varname).nMD },                       \
            { "n_traj","n_traj = %d",INT_T,    &(varname).n_traj },                    \
            { "trajL", "trajL = %lf",DOUBLE_T, &(varname).trajL },                     \
            { "nOMP",  "nOMP = %d",  INT_T,    &(varname).nOMP },                      \
            { "coldStart", "coldStart = %d", INT_T, &(varname).coldStart },            \
            { "NoMetropolisUntilRoutine", "NoMetropolisUntilRoutine = %d", INT_T, &(varname).NoMetropolisUntilRoutine }, \
            { NULL, NULL, INT_T, NULL }                                                 \
        }                                                                               \
    }

static input_hmc_grid hmc_par = init_input_hmc_grid(hmc_par);

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
    read_input(hmc_par.read, get_input_filename());

    int buf_len = GLB_T * GLB_X * GLB_Y * GLB_Z * 4 * 2 * NG * NG;
    double *out = malloc(buf_len * sizeof(double));

    struct HmcState *S = grid_hmc_init(
        NP_T, NP_X, NP_Y, NP_Z,
        GLB_T, GLB_X, GLB_Y, GLB_Z,
        hmc_par.betaF, hmc_par.betaA, hmc_par.nMD, hmc_par.trajL, hmc_par.nOMP,
        hmc_par.coldStart);

    lprintf("MAIN", 0, "betaF=%.4f betaA=%.4f nMD=%d trajL=%.4f n_traj=%d nOMP=%d coldStart=%d NoMetropolisUntilRoutine=%d\n",
            hmc_par.betaF, hmc_par.betaA, hmc_par.nMD, hmc_par.trajL, hmc_par.n_traj, hmc_par.nOMP,
            hmc_par.coldStart, hmc_par.NoMetropolisUntilRoutine);

    for (int traj = 0; traj < hmc_par.n_traj; traj++) {
        int metropolis = (traj >= hmc_par.NoMetropolisUntilRoutine) ? 1 : 0;
        lprintf("HMC", 0, "Starting trajectory %d (metropolis=%d)\n", traj, metropolis);
        grid_hmc_step(S, out, metropolis);
        copy_to_ugauge(out);
        double p_hirep = avr_plaquette();
        double p_grid = grid_hmc_plaquette(S);
        lprintf("HMC", 0, "Trajectory %d done  plaquette = %.10f (Grid %.10f)\n", traj, p_hirep, p_grid);
        // Assert the gauge-field transfer is correct: HiRep u_gauge plaquette must equal
        // Grid's own. (Requires NDEBUG unset in MkFlags, else assert is compiled out.)
        assert(fabs(p_hirep - p_grid) < 1e-6);
    }

    grid_hmc_finalize(S);

    free(out);

    finalize_process();
    return 0;
}
