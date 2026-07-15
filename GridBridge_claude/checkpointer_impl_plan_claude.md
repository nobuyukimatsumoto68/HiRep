# Checkpointer feature — implementation plan

## Goal

Per-trajectory checkpointing of the Grid HMC state (gauge + RNG), Grid's conventional naming
`ckpoint_lat.<n>` / `ckpoint_rng.<n>`, keeping only the latest pair (delete the previous on each
save). Plus `CheckpointStart`: resume from checkpoint `n` supplied via a shell/env variable.

Uses Grid `NerscIO` (same format as `NerscHmcCheckpointer`):
- save: `NerscIO::writeRNGState(sRNG, pRNG, rng)`,
  `NerscIO::writeConfiguration<GaugeStatistics<Gimpl>>(U, config, tworow, precision32)`
- load: `NerscIO::readRNGState(sRNG, pRNG, header, rng)`,
  `NerscIO::readConfiguration<GaugeStatistics<Gimpl>>(U, header, config)`

## Files to modify

| File | Change |
|------|--------|
| `grid_hirep_hmc_claude.cpp` (setup dir; sync -> root, Include) | add `HmcState::last_ckpoint`; `grid_hmc_save_checkpoint`, `grid_hmc_load_checkpoint` (extern C); include NerscIO |
| `grid_hirep_hmc_claude.h` (setup; sync -> root, Include) | declare the two new functions |
| `HiRep/PureGauge/hmc_grid_claude.c` | read `CheckpointStart` env; load on start; save + prune per trajectory; global-traj indexing |
| `HiRep/PureGauge/hmc_glueballs_grid_claude.c` | same |
| `HiRep/PureGauge/hmc_glueballs_grid_hdf5_claude.c` | same |

## Design

- Config index `n` = trajectory/config number. Fresh run (no CheckpointStart): initial config is 0
  (hot/cold); after trajectory i (i=0..) we produce config i+1 and save `ckpoint_*.(i+1)`.
- Resume: `CheckpointStart=n` (env) -> load `ckpoint_*.n` after `grid_hmc_init`, set `last_ckpoint=n`,
  continue producing configs n+1, n+2, ...
- Prune: `grid_hmc_save_checkpoint(S, n)` writes `ckpoint_*.n`, THEN (rank 0 only) `std::remove`s
  `ckpoint_lat.<last_ckpoint>` + `ckpoint_rng.<last_ckpoint>` if `last_ckpoint>=0`, then sets
  `last_ckpoint=n`. Save-before-delete so a valid checkpoint always exists.
- Global-traj indexing: metropolis + measurement stride use the GLOBAL config index (`start+i`), so a
  resumed run does NOT re-thermalize and measurements stay on the global stride.

## Driver loop (all 3 drivers)

```c
int start = 0;
const char *cks = getenv("CheckpointStart");
if (cks != NULL) { start = atoi(cks); grid_hmc_load_checkpoint(S, start); }  // sets last_ckpoint=start inside
for (int i = 0; i < hmc_par.n_traj; i++) {
    int n = start + i + 1;                                   // config produced this iteration
    int metropolis = ((start + i) >= hmc_par.NoMetropolisUntilRoutine) ? 1 : 0;
    grid_hmc_step(S, buf, metropolis);
    copy_to_ugauge(buf);
    ... assert, measure if (n % ObsInterval == 0) ...        // glueball drivers
    grid_hmc_save_checkpoint(S, n);                          // writes n, deletes previous
}
```

## Note on file deletion

`grid_hmc_save_checkpoint` deletes the PREVIOUS checkpoint via C++ `std::remove` (rank 0 only), only
the two files we ourselves wrote last (`ckpoint_lat.<last_ckpoint>`, `ckpoint_rng.<last_ckpoint>`) -
never arbitrary paths, and only after the new pair is written. This is the requested behavior; it is
in C++, not a shell script (the no-rm-in-scripts rule is about shell scripts).

## Decisions (RESOLVED, implemented)

1. **Env var** `CheckpointStart` (getenv), propagated through mpirun with `-x CheckpointStart`.
2. **n_traj = ABSOLUTE stop**: run configs `(start+1) .. n_traj`.
3. **Precision = 64-bit** (precision32=0, IEEE64BIG).
4. **Delete the resumed-from checkpoint** after the first new save (keep exactly one pair).
5. **Write location** = CWD (the run dir, `HiRep/`), Grid default naming.
6. **Glueball driver only** (`hmc_glueballs_grid_claude.c`). Bridge functions are shared/new (no
   signature break), so the bare + hdf5 drivers still compile unchanged.

## Status: IMPLEMENTED, bridge pre-flight-compiled OK. Pending rebuild + resume test.
