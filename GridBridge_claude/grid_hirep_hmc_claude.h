#pragma once
#ifdef __cplusplus
extern "C" {
#endif

struct HmcState; /* opaque - C caller only stores and passes the pointer */

/*
 * grid_hmc_init - initialize Grid, build HMC state.
 *   NP_{T,X,Y,Z}  : MPI process grid (must match HiRep's NP_*)
 *   N{t,x,y,z}    : global lattice extent
 *   betaF, betaA  : fundamental / adjoint Wilson coupling
 *   n_mdSteps     : MD steps per trajectory (MinimumNorm2 / Omelyan integrator)
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
 * grid_hmc_seed - reseed Grid's serial and parallel RNGs from a string and redraw the start
 * configuration. Call right after grid_hmc_init (which seeds with fixed integers).
 *   seed       : any string, IDENTICAL on all ranks (e.g. date+time broadcast from rank 0).
 *                Serial RNG <- SeedUniqueString(seed + "_serial"),
 *                parallel RNG <- SeedUniqueString(seed + "_parallel") (SHA-256 of the string).
 *   cold_start : 1 = cold (unit links), 0 = hot (random links from the reseeded parallel RNG)
 * A later grid_hmc_load_checkpoint overrides both the RNG state and the gauge field.
 */
void grid_hmc_seed(struct HmcState* S, const char* seed, int cold_start);

/*
 * grid_hmc_step - run one HMC trajectory, then write U into out[] using HiRep in-memory
 * layout: T-outermost site order (t,x,y,z), 4 directions in HiRep order (T,X,Y,Z),
 * Nc x Nc complex row-major. out must be Nt*Nx*Ny*Nz * 4 * 2*Nc*Nc doubles.
 *   metropolis : 1 = MD + Metropolis accept/reject; 0 = MD only, always accept (used for
 *                the first NoMetropolisUntilRoutine thermalization trajectories).
 */
void grid_hmc_step(struct HmcState* S, double *out, int metropolis);

/*
 * grid_hmc_plaquette - Grid's average plaquette (ordered -> 1) of the last grid_hmc_step
 * readout field. Assert this equals HiRep avr_plaquette() after copy_to_ugauge to verify
 * the gauge-field transfer.
 */
double grid_hmc_plaquette(struct HmcState* S);

/*
 * grid_hmc_save_checkpoint - write gauge+RNG to Grid NERSC checkpoints ckpoint_lat.<traj> /
 * ckpoint_rng.<traj> (64-bit), then delete the previous pair (keep only the latest).
 * grid_hmc_load_checkpoint - restore gauge+RNG from ckpoint_lat.<traj> / ckpoint_rng.<traj>.
 */
void grid_hmc_save_checkpoint(struct HmcState* S, int traj);
void grid_hmc_load_checkpoint(struct HmcState* S, int traj);

/*
 * grid_hmc_finalize - destroy the HMC state. Does NOT call Grid_finalize(): that would call
 * MPI_Finalize(), which HiRep's finalize_process() owns.
 */
void grid_hmc_finalize(struct HmcState* S);  /* deletes S; Grid_finalize() deliberately not called */

#ifdef __cplusplus
}
#endif
