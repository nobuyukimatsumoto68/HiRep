/***************************************************************************\
* Reader for Grid NERSC gauge configurations into HiRep u_gauge (no Grid needed).
* Plan: QuenchedSU4_claude/quenched_su4_glueball_impl_plan_claude.md (Chunk 7).
*
* File layout (verified on Grid SU(4) files with an independent Python reader, plaquette and checksum
* reproduced): ASCII header up to and including the line "END_HEADER", then the links as big-endian doubles,
* site order t (slowest), z, y, x (fastest); per site the 4 directions mu = x, y, z, t; per link the NG x NG
* complex matrix row-major, (re, im). CHECKSUM = sum of the big-endian 32-bit words of the binary part,
* mod 2^32. PLAQUETTE = (1/6) sum_{mu<nu} < Re tr U_{mu nu} > / NG (= HiRep avr_plaquette()).
* Grid direction mu (x,y,z,t) goes to the HiRep slot (mu + 1) % 4 (HiRep order t,x,y,z), as in the Grid bridge.
* Each rank reads its own block of timeslices (only NP_T > 1 is supported by the glueball code).
\***************************************************************************/

#include "libhr.h"
#include "nersc_io_claude.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef struct nersc_header_claude {
    long hdr_len;
    int dims[4];
    double plaq;
    unsigned long checksum;
    int ok_type;
    int ok_float;
} nersc_header_claude;

/* strip leading/trailing blanks and the newline in place */
static char *strip_claude(char *s) {
    while (*s == ' ' || *s == '\t') {
        s++;
    }
    char *e = s + strlen(s);
    while (e > s && (e[-1] == '\n' || e[-1] == '\r' || e[-1] == ' ' || e[-1] == '\t')) {
        e--;
    }
    *e = '\0';
    return s;
}

/* PID 0: parse the header */
static void parse_header_claude(const char *filename, nersc_header_claude *h) {
    FILE *fp = fopen(filename, "rb");
    error(fp == NULL, 1, "read_nersc_gauge_claude", "cannot open configuration file");
    char line[1024];
    char expected_type[64];
    snprintf(expected_type, sizeof(expected_type), "4D_SU%d_GAUGE_%dx%d", NG, NG, NG);
    h->hdr_len = -1;
    h->ok_type = 0;
    h->ok_float = 0;
    for (int i = 0; i < 4; i++) {
        h->dims[i] = -1;
    }
    error(fgets(line, sizeof(line), fp) == NULL, 1, "read_nersc_gauge_claude", "empty file");
    error(strncmp(line, "BEGIN_HEADER", 12) != 0, 1, "read_nersc_gauge_claude", "no BEGIN_HEADER");
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strncmp(line, "END_HEADER", 10) == 0) {
            h->hdr_len = ftell(fp);
            break;
        }
        char *eq = strchr(line, '=');
        if (eq == NULL) { continue; }
        *eq = '\0';
        char *key = strip_claude(line);
        char *val = strip_claude(eq + 1);
        if (strcmp(key, "DATATYPE") == 0) { h->ok_type = (strcmp(val, expected_type) == 0); }
        if (strcmp(key, "FLOATING_POINT") == 0) { h->ok_float = (strcmp(val, "IEEE64BIG") == 0); }
        if (strcmp(key, "PLAQUETTE") == 0) { h->plaq = atof(val); }
        if (strcmp(key, "CHECKSUM") == 0) { h->checksum = strtoul(val, NULL, 16); }
        for (int i = 0; i < 4; i++) {
            char dk[32];
            snprintf(dk, sizeof(dk), "DIMENSION_%d", i + 1);
            if (strcmp(key, dk) == 0) { h->dims[i] = atoi(val); }
        }
    }
    fclose(fp);
}

double read_nersc_gauge_claude(const char *filename) {
    nersc_header_claude h;
    memset(&h, 0, sizeof(h));
    if (PID == 0) { parse_header_claude(filename, &h); }
#ifdef WITH_MPI
    MPI_Bcast(&h.hdr_len, 1, MPI_LONG, 0, GLB_COMM);
    MPI_Bcast(h.dims, 4, MPI_INT, 0, GLB_COMM);
    MPI_Bcast(&h.plaq, 1, MPI_DOUBLE, 0, GLB_COMM);
    MPI_Bcast(&h.checksum, 1, MPI_UNSIGNED_LONG, 0, GLB_COMM);
    MPI_Bcast(&h.ok_type, 1, MPI_INT, 0, GLB_COMM);
    MPI_Bcast(&h.ok_float, 1, MPI_INT, 0, GLB_COMM);
#endif
    error(h.hdr_len < 0, 1, "read_nersc_gauge_claude", "no END_HEADER");
    error(!h.ok_type, 1, "read_nersc_gauge_claude", "DATATYPE is not 4D_SU<NG>_GAUGE_<NG>x<NG> for this NG");
    error(!h.ok_float, 1, "read_nersc_gauge_claude", "FLOATING_POINT is not IEEE64BIG");
    error(h.dims[0] != GLB_X || h.dims[1] != GLB_Y || h.dims[2] != GLB_Z || h.dims[3] != GLB_T, 1,
          "read_nersc_gauge_claude", "lattice dimensions of the file differ from the input");
    error(NP_X != 1 || NP_Y != 1 || NP_Z != 1, 1, "read_nersc_gauge_claude", "only NP_T > 1 is supported");

    /* this rank's block of timeslices */
    const size_t per_site = (size_t)4 * NG * NG * 2;
    const size_t nsites = (size_t)T * X * Y * Z;
    const size_t nbytes = nsites * per_site * sizeof(double);
    unsigned char *buf = malloc(nbytes);
    error(buf == NULL, 1, "read_nersc_gauge_claude", "cannot allocate read buffer");
    FILE *fp = fopen(filename, "rb");
    error(fp == NULL, 1, "read_nersc_gauge_claude", "cannot open configuration file");
    off_t offset = (off_t)h.hdr_len + (off_t)zerocoord[0] * X * Y * Z * per_site * sizeof(double);
    error(fseeko(fp, offset, SEEK_SET) != 0, 1, "read_nersc_gauge_claude", "seek failed");
    error(fread(buf, 1, nbytes, fp) != nbytes, 1, "read_nersc_gauge_claude", "short read");
    fclose(fp);

    /* NERSC checksum: sum of the big-endian 32-bit words (mod 2^32), summed over all ranks */
    uint64_t csum = 0;
    for (size_t w = 0; w < nbytes / 4; w++) {
        const unsigned char *b = buf + 4 * w;
        uint32_t word = ((uint32_t)b[0] << 24) | ((uint32_t)b[1] << 16) | ((uint32_t)b[2] << 8) | (uint32_t)b[3];
        csum += word;
    }
    unsigned long csum_loc = (unsigned long)(csum & 0xffffffffUL);
    unsigned long csum_glb = csum_loc;
#ifdef WITH_MPI
    MPI_Allreduce(&csum_loc, &csum_glb, 1, MPI_UNSIGNED_LONG, MPI_SUM, GLB_COMM);
#endif
    csum_glb = csum_glb & 0xffffffffUL;
    lprintf("NERSC", 0, "%s: checksum file %lx computed %lx, header plaquette %.10f\n", filename, h.checksum, csum_glb,
            h.plaq);
    error(csum_glb != h.checksum, 1, "read_nersc_gauge_claude", "NERSC checksum mismatch");

    /* big-endian doubles -> host order, then into u_gauge */
    double *d = (double *)buf;
    for (size_t k = 0; k < nbytes / 8; k++) {
        unsigned char *b = buf + 8 * k;
        uint64_t v = 0;
        for (int j = 0; j < 8; j++) {
            v = (v << 8) | b[j];
        }
        memcpy(&d[k], &v, sizeof(double));
    }
    for (int t = 0; t < T; t++) {
        for (int z = 0; z < Z; z++) {
            for (int y = 0; y < Y; y++) {
                for (int x = 0; x < X; x++) {
                    size_t s = (((size_t)t * Z + z) * Y + y) * X + x;
                    int site = ipt(t, x, y, z);
                    for (int mu = 0; mu < 4; mu++) {
                        suNg *u = pu_gauge(site, (mu + 1) % 4);
                        const double *p = d + s * per_site + (size_t)mu * NG * NG * 2;
                        for (int i = 0; i < NG * NG; i++) {
                            u->c[i] = p[2 * i] + I * p[2 * i + 1];
                        }
                    }
                }
            }
        }
    }
    free(buf);
    start_sendrecv_suNg_field(u_gauge);
    complete_sendrecv_suNg_field(u_gauge);
    apply_BCs_on_fundamental_gauge_field();
    represent_gauge_field();
    return h.plaq;
}
