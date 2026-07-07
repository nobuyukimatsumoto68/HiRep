#pragma once
#ifdef __cplusplus
extern "C" {
#endif

struct HmcState; /* opaque — C caller only stores and passes the pointer */

/*
 * grid_hmc_init — initialize Grid, build HMC state.
 *   NP_{T,X,Y,Z}  : MPI process grid (must match HiRep's NP_*)
 *   N{t,x,y,z}    : global lattice extent
 *   betaF, betaA  : fundamental / adjoint Wilson coupling
 *   n_mdSteps     : leapfrog steps per trajectory
 *   trajL         : MD trajectory length
 *   n_omp         : OpenMP threads per MPI rank (passed as --threads to Grid_init)
 *   cold_start    : 1 = cold (unit links), 0 = hot (random links)
 * Returns an opaque pointer to be passed to grid_hmc_step / grid_hmc_finalize.
 */
struct HmcState* grid_hmc_init(int NP_T, int NP_X, int NP_Y, int NP_Z,
                               int Nt,   int Nx,   int Ny,   int Nz,
                               double betaF, double betaA,
                               int n_mdSteps, double trajL, int n_omp,
                               int cold_start);

/*
 * grid_hmc_step — run one HMC trajectory, then write U into out[] using HiRep in-memory
 * layout: T-outermost site order (t,x,y,z), 4 directions in HiRep order (T,X,Y,Z),
 * Nc×Nc complex row-major. out must be Nt*Nx*Ny*Nz * 4 * 2*Nc*Nc doubles.
 *   metropolis : 1 = MD + Metropolis accept/reject; 0 = MD only, always accept (used for
 *                the first NoMetropolisUntilRoutine thermalization trajectories).
 */
void grid_hmc_step(struct HmcState* S, double *out, int metropolis);

/*
 * grid_hmc_plaquette — Grid's average plaquette (ordered -> 1) of the last grid_hmc_step
 * readout field. Assert this equals HiRep avr_plaquette() after copy_to_ugauge to verify
 * the gauge-field transfer.
 */
double grid_hmc_plaquette(struct HmcState* S);

/*
 * grid_hmc_finalize — destroy HMC state and finalize Grid.
 */
void grid_hmc_finalize(struct HmcState* S);  /* deletes S, then finalizes Grid */

#ifdef __cplusplus
}
#endif
