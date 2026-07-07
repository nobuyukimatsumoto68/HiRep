#ifndef GRID_HIREP_IO_H
#define GRID_HIREP_IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

void grid_init(int NP_T, int NP_X, int NP_Y, int NP_Z,
               int Nt,   int Nx,   int Ny,   int Nz);
void grid_read_config(const char* filename,
                      double*     out,
                      int Nc, int Nt, int Nx, int Ny, int Nz);

#ifdef __cplusplus
}
#endif

// Extracts the trajectory number from a Grid checkpoint filename of the form
// ckpoint_lat.NNNN — returns NNNN as an int.
static inline int confid_retriever(const char *cnfg_filename) {
    int confid;
    const char *dot = strrchr(cnfg_filename, '.');
    if (dot == NULL || sscanf(dot, ".%d", &confid) != 1) {
        fprintf(stderr, "[confid_retriever] Malformed configuration name "
                "(not ending by ...<number>): %s\n", cnfg_filename);
        exit(EXIT_FAILURE);
    }
    return confid;
}

#endif
