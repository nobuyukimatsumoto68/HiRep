#pragma once
#ifdef __cplusplus
extern "C" {
#endif

struct HmcState; /* opaque — C caller only stores and passes the pointer */

/*
 * grid_hmc_init — initialize Grid, build HMC state (cold start).
 *   NP_{T,X,Y,Z}  : MPI process grid (must match HiRep's NP_*)
 *   N{t,x,y,z}    : global lattice extent
 *   betaF, betaA  : fundamental / adjoint Wilson coupling
 *   n_mdSteps     : leapfrog steps per trajectory
 *   trajL         : MD trajectory length
 * Returns an opaque pointer to be passed to grid_hmc_step / grid_hmc_finalize.
 */
struct HmcState* grid_hmc_init(int NP_T, int NP_X, int NP_Y, int NP_Z,
                               int Nt,   int Nx,   int Ny,   int Nz,
                               double betaF, double betaA,
                               int n_mdSteps, double trajL);

/*
 * grid_hmc_step — run one HMC trajectory (MD + Metropolis), then write U
 * into out[] using HiRep in-memory layout:
 *   T-outermost site order (t,x,y,z), 4 directions, Nc×Nc complex row-major.
 *   out must be pre-allocated to Nt*Nx*Ny*Nz * 4 * 2*Nc*Nc doubles.
 */
void grid_hmc_step(struct HmcState* S, double *out);

/*
 * grid_hmc_finalize — destroy HMC state and finalize Grid.
 */
void grid_hmc_finalize(struct HmcState* S);  /* deletes S, then finalizes Grid */

#ifdef __cplusplus
}
#endif
