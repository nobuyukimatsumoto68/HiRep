
#include "observables.h"
#include "libhr_core.h"
#include "io.h"
#include "utils.h"
#include <string.h>
#define npaths 241
static hr_complex *mom_def_Cp_tr_paths = NULL;
static hr_complex *mom_def_Cm_tr_paths = NULL;
static hr_complex *path_storage = NULL;
int **direct_spatial_rotations() {
    int i;
    int **res = malloc(sizeof(int *) * 48);
    int *res1 = malloc(sizeof(int *) * 48 * 4);
    for (i = 0; i < 48; i++) {
        res[i] = res1 + 4 * i;
    }

    res1[0] = 0;
    res1[1] = 1;
    res1[2] = 2;
    res1[3] = 3;
    res1[4] = 0;
    res1[5] = 2;
    res1[6] = 1;
    res1[7] = -3;
    res1[8] = 0;
    res1[9] = -2;
    res1[10] = -1;
    res1[11] = -3;
    res1[12] = 0;
    res1[13] = -1;
    res1[14] = 3;
    res1[15] = 2;
    res1[16] = 0;
    res1[17] = -1;
    res1[18] = -3;
    res1[19] = -2;
    res1[20] = 0;
    res1[21] = 3;
    res1[22] = -2;
    res1[23] = 1;
    res1[24] = 0;
    res1[25] = -3;
    res1[26] = -2;
    res1[27] = -1;
    res1[28] = 0;
    res1[29] = -2;
    res1[30] = -3;
    res1[31] = 1;
    res1[32] = 0;
    res1[33] = 3;
    res1[34] = -1;
    res1[35] = -2;
    res1[36] = 0;
    res1[37] = -3;
    res1[38] = -1;
    res1[39] = 2;
    res1[40] = 0;
    res1[41] = -2;
    res1[42] = 3;
    res1[43] = -1;
    res1[44] = 0;
    res1[45] = 3;
    res1[46] = 1;
    res1[47] = 2;
    res1[48] = 0;
    res1[49] = 2;
    res1[50] = 3;
    res1[51] = 1;
    res1[52] = 0;
    res1[53] = 2;
    res1[54] = -3;
    res1[55] = -1;
    res1[56] = 0;
    res1[57] = -3;
    res1[58] = 1;
    res1[59] = -2;
    res1[60] = 0;
    res1[61] = -2;
    res1[62] = 1;
    res1[63] = 3;
    res1[64] = 0;
    res1[65] = 2;
    res1[66] = -1;
    res1[67] = 3;
    res1[68] = 0;
    res1[69] = 3;
    res1[70] = 2;
    res1[71] = -1;
    res1[72] = 0;
    res1[73] = -3;
    res1[74] = 2;
    res1[75] = 1;
    res1[76] = 0;
    res1[77] = 1;
    res1[78] = 3;
    res1[79] = -2;
    res1[80] = 0;
    res1[81] = 1;
    res1[82] = -3;
    res1[83] = 2;
    res1[84] = 0;
    res1[85] = -1;
    res1[86] = -2;
    res1[87] = 3;
    res1[88] = 0;
    res1[89] = -1;
    res1[90] = 2;
    res1[91] = -3;
    res1[92] = 0;
    res1[93] = 1;
    res1[94] = -2;
    res1[95] = -3;
    res1[96] = 0;
    res1[97] = -1;
    res1[98] = -2;
    res1[99] = -3;
    res1[100] = 0;
    res1[101] = -2;
    res1[102] = -1;
    res1[103] = 3;
    res1[104] = 0;
    res1[105] = 2;
    res1[106] = 1;
    res1[107] = 3;
    res1[108] = 0;
    res1[109] = 1;
    res1[110] = -3;
    res1[111] = -2;
    res1[112] = 0;
    res1[113] = 1;
    res1[114] = 3;
    res1[115] = 2;
    res1[116] = 0;
    res1[117] = -3;
    res1[118] = 2;
    res1[119] = -1;
    res1[120] = 0;
    res1[121] = 3;
    res1[122] = 2;
    res1[123] = 1;
    res1[124] = 0;
    res1[125] = 2;
    res1[126] = 3;
    res1[127] = -1;
    res1[128] = 0;
    res1[129] = -3;
    res1[130] = 1;
    res1[131] = 2;
    res1[132] = 0;
    res1[133] = 3;
    res1[134] = 1;
    res1[135] = -2;
    res1[136] = 0;
    res1[137] = 2;
    res1[138] = -3;
    res1[139] = 1;
    res1[140] = 0;
    res1[141] = -3;
    res1[142] = -1;
    res1[143] = -2;
    res1[144] = 0;
    res1[145] = -2;
    res1[146] = -3;
    res1[147] = -1;
    res1[148] = 0;
    res1[149] = -2;
    res1[150] = 3;
    res1[151] = 1;
    res1[152] = 0;
    res1[153] = 3;
    res1[154] = -1;
    res1[155] = 2;
    res1[156] = 0;
    res1[157] = 2;
    res1[158] = -1;
    res1[159] = -3;
    res1[160] = 0;
    res1[161] = -2;
    res1[162] = 1;
    res1[163] = -3;
    res1[164] = 0;
    res1[165] = -3;
    res1[166] = -2;
    res1[167] = 1;
    res1[168] = 0;
    res1[169] = 3;
    res1[170] = -2;
    res1[171] = -1;
    res1[172] = 0;
    res1[173] = -1;
    res1[174] = -3;
    res1[175] = 2;
    res1[176] = 0;
    res1[177] = -1;
    res1[178] = 3;
    res1[179] = -2;
    res1[180] = 0;
    res1[181] = 1;
    res1[182] = 2;
    res1[183] = -3;
    res1[184] = 0;
    res1[185] = 1;
    res1[186] = -2;
    res1[187] = 3;
    res1[188] = 0;
    res1[189] = -1;
    res1[190] = 2;
    res1[191] = 3;
    return res;
}
int **inverse_spatial_rotations() {
    int i;
    int **res = malloc(sizeof(int *) * 48);
    int *res1 = malloc(sizeof(int *) * 48 * 4);
    for (i = 0; i < 48; i++) {
        res[i] = res1 + 4 * i;
    }
    res1[0] = 0;
    res1[1] = 1;
    res1[2] = 2;
    res1[3] = 3;
    res1[4] = 0;
    res1[5] = 2;
    res1[6] = 1;
    res1[7] = -3;
    res1[8] = 0;
    res1[9] = -2;
    res1[10] = -1;
    res1[11] = -3;
    res1[12] = 0;
    res1[13] = -1;
    res1[14] = 3;
    res1[15] = 2;
    res1[16] = 0;
    res1[17] = -1;
    res1[18] = -3;
    res1[19] = -2;
    res1[20] = 0;
    res1[21] = 3;
    res1[22] = -2;
    res1[23] = 1;
    res1[24] = 0;
    res1[25] = -3;
    res1[26] = -2;
    res1[27] = -1;
    res1[28] = 0;
    res1[29] = 3;
    res1[30] = -1;
    res1[31] = -2;
    res1[32] = 0;
    res1[33] = -2;
    res1[34] = -3;
    res1[35] = 1;
    res1[36] = 0;
    res1[37] = -2;
    res1[38] = 3;
    res1[39] = -1;
    res1[40] = 0;
    res1[41] = -3;
    res1[42] = -1;
    res1[43] = 2;
    res1[44] = 0;
    res1[45] = 2;
    res1[46] = 3;
    res1[47] = 1;
    res1[48] = 0;
    res1[49] = 3;
    res1[50] = 1;
    res1[51] = 2;
    res1[52] = 0;
    res1[53] = -3;
    res1[54] = 1;
    res1[55] = -2;
    res1[56] = 0;
    res1[57] = 2;
    res1[58] = -3;
    res1[59] = -1;
    res1[60] = 0;
    res1[61] = 2;
    res1[62] = -1;
    res1[63] = 3;
    res1[64] = 0;
    res1[65] = -2;
    res1[66] = 1;
    res1[67] = 3;
    res1[68] = 0;
    res1[69] = -3;
    res1[70] = 2;
    res1[71] = 1;
    res1[72] = 0;
    res1[73] = 3;
    res1[74] = 2;
    res1[75] = -1;
    res1[76] = 0;
    res1[77] = 1;
    res1[78] = -3;
    res1[79] = 2;
    res1[80] = 0;
    res1[81] = 1;
    res1[82] = 3;
    res1[83] = -2;
    res1[84] = 0;
    res1[85] = -1;
    res1[86] = -2;
    res1[87] = 3;
    res1[88] = 0;
    res1[89] = -1;
    res1[90] = 2;
    res1[91] = -3;
    res1[92] = 0;
    res1[93] = 1;
    res1[94] = -2;
    res1[95] = -3;
    res1[96] = 0;
    res1[97] = -1;
    res1[98] = -2;
    res1[99] = -3;
    res1[100] = 0;
    res1[101] = -2;
    res1[102] = -1;
    res1[103] = 3;
    res1[104] = 0;
    res1[105] = 2;
    res1[106] = 1;
    res1[107] = 3;
    res1[108] = 0;
    res1[109] = 1;
    res1[110] = -3;
    res1[111] = -2;
    res1[112] = 0;
    res1[113] = 1;
    res1[114] = 3;
    res1[115] = 2;
    res1[116] = 0;
    res1[117] = -3;
    res1[118] = 2;
    res1[119] = -1;
    res1[120] = 0;
    res1[121] = 3;
    res1[122] = 2;
    res1[123] = 1;
    res1[124] = 0;
    res1[125] = -3;
    res1[126] = 1;
    res1[127] = 2;
    res1[128] = 0;
    res1[129] = 2;
    res1[130] = 3;
    res1[131] = -1;
    res1[132] = 0;
    res1[133] = 2;
    res1[134] = -3;
    res1[135] = 1;
    res1[136] = 0;
    res1[137] = 3;
    res1[138] = 1;
    res1[139] = -2;
    res1[140] = 0;
    res1[141] = -2;
    res1[142] = -3;
    res1[143] = -1;
    res1[144] = 0;
    res1[145] = -3;
    res1[146] = -1;
    res1[147] = -2;
    res1[148] = 0;
    res1[149] = 3;
    res1[150] = -1;
    res1[151] = 2;
    res1[152] = 0;
    res1[153] = -2;
    res1[154] = 3;
    res1[155] = 1;
    res1[156] = 0;
    res1[157] = -2;
    res1[158] = 1;
    res1[159] = -3;
    res1[160] = 0;
    res1[161] = 2;
    res1[162] = -1;
    res1[163] = -3;
    res1[164] = 0;
    res1[165] = 3;
    res1[166] = -2;
    res1[167] = -1;
    res1[168] = 0;
    res1[169] = -3;
    res1[170] = -2;
    res1[171] = 1;
    res1[172] = 0;
    res1[173] = -1;
    res1[174] = 3;
    res1[175] = -2;
    res1[176] = 0;
    res1[177] = -1;
    res1[178] = -3;
    res1[179] = 2;
    res1[180] = 0;
    res1[181] = 1;
    res1[182] = 2;
    res1[183] = -3;
    res1[184] = 0;
    res1[185] = 1;
    res1[186] = -2;
    res1[187] = 3;
    res1[188] = 0;
    res1[189] = -1;
    res1[190] = 2;
    res1[191] = 3;
    return res;
}
static hr_complex path0(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path1(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static void OP_oneTr_p_m1_0_0_Ir_1_C_1_n_1(hr_complex *op_out) {
    *op_out = +(4.) * mom_def_Cp_tr_paths[0] + (4.) * mom_def_Cp_tr_paths[1];
}

static hr_complex path2(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static void OP_oneTr_p_m1_0_0_Ir_1_C_1_n_2(hr_complex *op_out) {
    *op_out = +(8.) * mom_def_Cp_tr_paths[2];
}

static void OP_oneTr_p_0_m1_0_Ir_1_C_1_n_1(hr_complex *op_out) {
    *op_out = +(8.) * mom_def_Cp_tr_paths[1];
}

static void OP_oneTr_p_0_0_m1_Ir_1_C_1_n_1(hr_complex *op_out) {
    *op_out = +(8.) * mom_def_Cp_tr_paths[0];
}

static void OP_oneTr_p_0_0_0_Ir_1_C_1_n_1(hr_complex *op_out) {
    *op_out = +(16.) * mom_def_Cp_tr_paths[0] + (16.) * mom_def_Cp_tr_paths[2] + (16.) * mom_def_Cp_tr_paths[1];
}

static hr_complex path3(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, *w2, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path4(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path6(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path5(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static void OP_oneTr_p_0_0_0_Ir_1_C_1_n_2(hr_complex *op_out) {
    *op_out = +(12.) * mom_def_Cp_tr_paths[3] + (12.) * mom_def_Cp_tr_paths[4] + (12.) * mom_def_Cp_tr_paths[6] +
              (12.) * mom_def_Cp_tr_paths[5];
}

static hr_complex path7(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path10(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path11(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path14(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path8(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path9(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path27(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path19(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path29(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path20(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path17(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path18(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path15(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path22(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path16(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path12(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path21(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path13(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path25(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path28(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path26(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path23(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path30(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path24(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static void OP_oneTr_p_0_0_0_Ir_1_C_1_n_3(hr_complex *op_out) {
    *op_out = +(2.) * mom_def_Cp_tr_paths[7] + (2.) * mom_def_Cp_tr_paths[10] + (2.) * mom_def_Cp_tr_paths[11] +
              (2.) * mom_def_Cp_tr_paths[14] + (2.) * mom_def_Cp_tr_paths[8] + (2.) * mom_def_Cp_tr_paths[9] +
              (2.) * mom_def_Cp_tr_paths[27] + (2.) * mom_def_Cp_tr_paths[19] + (2.) * mom_def_Cp_tr_paths[29] +
              (2.) * mom_def_Cp_tr_paths[20] + (2.) * mom_def_Cp_tr_paths[17] + (2.) * mom_def_Cp_tr_paths[18] +
              (2.) * mom_def_Cp_tr_paths[15] + (2.) * mom_def_Cp_tr_paths[22] + (2.) * mom_def_Cp_tr_paths[16] +
              (2.) * mom_def_Cp_tr_paths[12] + (2.) * mom_def_Cp_tr_paths[21] + (2.) * mom_def_Cp_tr_paths[13] +
              (2.) * mom_def_Cp_tr_paths[25] + (2.) * mom_def_Cp_tr_paths[28] + (2.) * mom_def_Cp_tr_paths[26] +
              (2.) * mom_def_Cp_tr_paths[23] + (2.) * mom_def_Cp_tr_paths[30] + (2.) * mom_def_Cp_tr_paths[24];
}

static hr_complex path31(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path42(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path44(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path46(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path37(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path61(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path49(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path32(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path50(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path71(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path33(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path65(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path36(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path55(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path66(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path77(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path45(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path52(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path34(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path35(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path47(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path75(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path56(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path58(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path60(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path62(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path69(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path40(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path51(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path70(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path48(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path73(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path41(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path38(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path39(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path59(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path63(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path68(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path53(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path43(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path54(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path67(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path57(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path64(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path72(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path74(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path76(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path78(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static void OP_oneTr_p_0_0_0_Ir_1_C_1_n_4(hr_complex *op_out) {
    *op_out = +mom_def_Cp_tr_paths[31] + mom_def_Cp_tr_paths[42] + mom_def_Cp_tr_paths[44] + mom_def_Cp_tr_paths[46] +
              mom_def_Cp_tr_paths[37] + mom_def_Cp_tr_paths[61] + mom_def_Cp_tr_paths[49] + mom_def_Cp_tr_paths[32] +
              mom_def_Cp_tr_paths[50] + mom_def_Cp_tr_paths[71] + mom_def_Cp_tr_paths[33] + mom_def_Cp_tr_paths[65] +
              mom_def_Cp_tr_paths[36] + mom_def_Cp_tr_paths[55] + mom_def_Cp_tr_paths[66] + mom_def_Cp_tr_paths[77] +
              mom_def_Cp_tr_paths[45] + mom_def_Cp_tr_paths[52] + mom_def_Cp_tr_paths[34] + mom_def_Cp_tr_paths[35] +
              mom_def_Cp_tr_paths[47] + mom_def_Cp_tr_paths[75] + mom_def_Cp_tr_paths[56] + mom_def_Cp_tr_paths[58] +
              mom_def_Cp_tr_paths[60] + mom_def_Cp_tr_paths[62] + mom_def_Cp_tr_paths[69] + mom_def_Cp_tr_paths[40] +
              mom_def_Cp_tr_paths[51] + mom_def_Cp_tr_paths[70] + mom_def_Cp_tr_paths[48] + mom_def_Cp_tr_paths[73] +
              mom_def_Cp_tr_paths[41] + mom_def_Cp_tr_paths[38] + mom_def_Cp_tr_paths[39] + mom_def_Cp_tr_paths[59] +
              mom_def_Cp_tr_paths[63] + mom_def_Cp_tr_paths[68] + mom_def_Cp_tr_paths[53] + mom_def_Cp_tr_paths[43] +
              mom_def_Cp_tr_paths[54] + mom_def_Cp_tr_paths[67] + mom_def_Cp_tr_paths[57] + mom_def_Cp_tr_paths[64] +
              mom_def_Cp_tr_paths[72] + mom_def_Cp_tr_paths[74] + mom_def_Cp_tr_paths[76] + mom_def_Cp_tr_paths[78];
}

static hr_complex path79(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path89(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path90(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path80(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path81(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path97(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path98(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path84(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path85(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path82(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path83(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path87(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path88(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path101(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path102(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path91(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path92(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path86(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path95(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path96(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path93(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path94(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path99(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path100(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static void OP_oneTr_p_0_0_0_Ir_1_C_1_n_5(hr_complex *op_out) {
    *op_out = +(2.) * mom_def_Cp_tr_paths[79] + (2.) * mom_def_Cp_tr_paths[89] + (2.) * mom_def_Cp_tr_paths[90] +
              (2.) * mom_def_Cp_tr_paths[80] + (2.) * mom_def_Cp_tr_paths[81] + (2.) * mom_def_Cp_tr_paths[97] +
              (2.) * mom_def_Cp_tr_paths[98] + (2.) * mom_def_Cp_tr_paths[84] + (2.) * mom_def_Cp_tr_paths[85] +
              (2.) * mom_def_Cp_tr_paths[82] + (2.) * mom_def_Cp_tr_paths[83] + (2.) * mom_def_Cp_tr_paths[87] +
              (2.) * mom_def_Cp_tr_paths[88] + (2.) * mom_def_Cp_tr_paths[101] + (2.) * mom_def_Cp_tr_paths[102] +
              (2.) * mom_def_Cp_tr_paths[91] + (2.) * mom_def_Cp_tr_paths[92] + (2.) * mom_def_Cp_tr_paths[86] +
              (2.) * mom_def_Cp_tr_paths[95] + (2.) * mom_def_Cp_tr_paths[96] + (2.) * mom_def_Cp_tr_paths[93] +
              (2.) * mom_def_Cp_tr_paths[94] + (2.) * mom_def_Cp_tr_paths[99] + (2.) * mom_def_Cp_tr_paths[100];
}

static hr_complex path103(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path106(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path107(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path110(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path104(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path105(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path123(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path115(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path125(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path116(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path113(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path114(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path111(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path118(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path112(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path108(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path117(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path109(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path121(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path124(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path122(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path119(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path126(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path120(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static void OP_oneTr_p_0_0_0_Ir_1_C_1_n_6(hr_complex *op_out) {
    *op_out = +(2.) * mom_def_Cp_tr_paths[103] + (2.) * mom_def_Cp_tr_paths[106] + (2.) * mom_def_Cp_tr_paths[107] +
              (2.) * mom_def_Cp_tr_paths[110] + (2.) * mom_def_Cp_tr_paths[104] + (2.) * mom_def_Cp_tr_paths[105] +
              (2.) * mom_def_Cp_tr_paths[123] + (2.) * mom_def_Cp_tr_paths[115] + (2.) * mom_def_Cp_tr_paths[125] +
              (2.) * mom_def_Cp_tr_paths[116] + (2.) * mom_def_Cp_tr_paths[113] + (2.) * mom_def_Cp_tr_paths[114] +
              (2.) * mom_def_Cp_tr_paths[111] + (2.) * mom_def_Cp_tr_paths[118] + (2.) * mom_def_Cp_tr_paths[112] +
              (2.) * mom_def_Cp_tr_paths[108] + (2.) * mom_def_Cp_tr_paths[117] + (2.) * mom_def_Cp_tr_paths[109] +
              (2.) * mom_def_Cp_tr_paths[121] + (2.) * mom_def_Cp_tr_paths[124] + (2.) * mom_def_Cp_tr_paths[122] +
              (2.) * mom_def_Cp_tr_paths[119] + (2.) * mom_def_Cp_tr_paths[126] + (2.) * mom_def_Cp_tr_paths[120];
}

static hr_complex path127(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path138(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path140(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path142(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path133(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path129(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path155(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path157(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path145(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path146(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path143(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path159(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path161(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path162(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path171(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path173(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path139(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path141(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path130(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path131(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path128(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path148(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path152(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path154(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path156(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path158(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path144(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path163(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path165(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path166(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path167(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path169(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path136(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path137(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path134(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path135(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path132(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path147(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path149(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path150(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path151(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path153(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path164(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path160(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path168(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path170(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path172(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path174(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static void OP_oneTr_p_0_0_0_Ir_1_C_1_n_7(hr_complex *op_out) {
    *op_out = +mom_def_Cp_tr_paths[127] + mom_def_Cp_tr_paths[138] + mom_def_Cp_tr_paths[140] + mom_def_Cp_tr_paths[142] +
              mom_def_Cp_tr_paths[133] + mom_def_Cp_tr_paths[129] + mom_def_Cp_tr_paths[155] + mom_def_Cp_tr_paths[157] +
              mom_def_Cp_tr_paths[145] + mom_def_Cp_tr_paths[146] + mom_def_Cp_tr_paths[143] + mom_def_Cp_tr_paths[159] +
              mom_def_Cp_tr_paths[161] + mom_def_Cp_tr_paths[162] + mom_def_Cp_tr_paths[171] + mom_def_Cp_tr_paths[173] +
              mom_def_Cp_tr_paths[139] + mom_def_Cp_tr_paths[141] + mom_def_Cp_tr_paths[130] + mom_def_Cp_tr_paths[131] +
              mom_def_Cp_tr_paths[128] + mom_def_Cp_tr_paths[148] + mom_def_Cp_tr_paths[152] + mom_def_Cp_tr_paths[154] +
              mom_def_Cp_tr_paths[156] + mom_def_Cp_tr_paths[158] + mom_def_Cp_tr_paths[144] + mom_def_Cp_tr_paths[163] +
              mom_def_Cp_tr_paths[165] + mom_def_Cp_tr_paths[166] + mom_def_Cp_tr_paths[167] + mom_def_Cp_tr_paths[169] +
              mom_def_Cp_tr_paths[136] + mom_def_Cp_tr_paths[137] + mom_def_Cp_tr_paths[134] + mom_def_Cp_tr_paths[135] +
              mom_def_Cp_tr_paths[132] + mom_def_Cp_tr_paths[147] + mom_def_Cp_tr_paths[149] + mom_def_Cp_tr_paths[150] +
              mom_def_Cp_tr_paths[151] + mom_def_Cp_tr_paths[153] + mom_def_Cp_tr_paths[164] + mom_def_Cp_tr_paths[160] +
              mom_def_Cp_tr_paths[168] + mom_def_Cp_tr_paths[170] + mom_def_Cp_tr_paths[172] + mom_def_Cp_tr_paths[174];
}

static hr_complex path175(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path182(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path187(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path195(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path176(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path184(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path178(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path192(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path183(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path177(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path180(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path188(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path190(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path197(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path185(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path194(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path191(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path179(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path193(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path186(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path181(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path189(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path196(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path198(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static void OP_oneTr_p_0_0_0_Ir_1_C_1_n_8(hr_complex *op_out) {
    *op_out = +(2.) * mom_def_Cp_tr_paths[175] + (2.) * mom_def_Cp_tr_paths[182] + (2.) * mom_def_Cp_tr_paths[187] +
              (2.) * mom_def_Cp_tr_paths[195] + (2.) * mom_def_Cp_tr_paths[176] + (2.) * mom_def_Cp_tr_paths[184] +
              (2.) * mom_def_Cp_tr_paths[178] + (2.) * mom_def_Cp_tr_paths[192] + (2.) * mom_def_Cp_tr_paths[183] +
              (2.) * mom_def_Cp_tr_paths[177] + (2.) * mom_def_Cp_tr_paths[180] + (2.) * mom_def_Cp_tr_paths[188] +
              (2.) * mom_def_Cp_tr_paths[190] + (2.) * mom_def_Cp_tr_paths[197] + (2.) * mom_def_Cp_tr_paths[185] +
              (2.) * mom_def_Cp_tr_paths[194] + (2.) * mom_def_Cp_tr_paths[191] + (2.) * mom_def_Cp_tr_paths[179] +
              (2.) * mom_def_Cp_tr_paths[193] + (2.) * mom_def_Cp_tr_paths[186] + (2.) * mom_def_Cp_tr_paths[181] +
              (2.) * mom_def_Cp_tr_paths[189] + (2.) * mom_def_Cp_tr_paths[196] + (2.) * mom_def_Cp_tr_paths[198];
}

static void OP_oneTr_p_0_0_0_Ir_3_C_1_n_1(hr_complex *op_out) {
    *op_out = +(16.) * mom_def_Cp_tr_paths[0] + (-8.) * mom_def_Cp_tr_paths[2] + (-8.) * mom_def_Cp_tr_paths[1];
}

static void OP_oneTr_p_0_0_0_Ir_3_C_1_n_2(hr_complex *op_out) {
    *op_out = +(13.8564064605510183) * mom_def_Cp_tr_paths[2] + (-13.8564064605510183) * mom_def_Cp_tr_paths[1];
}

static hr_complex path199(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path200(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path201(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path202(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path203(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path204(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static void OP_oneTr_p_0_0_0_Ir_3_C_1_n_3(hr_complex *op_out) {
    *op_out = +(8.) * mom_def_Cp_tr_paths[199] + (8.) * mom_def_Cp_tr_paths[200] + (8.) * mom_def_Cp_tr_paths[201] +
              (8.) * mom_def_Cp_tr_paths[202] + (-16.) * mom_def_Cp_tr_paths[203] + (-16.) * mom_def_Cp_tr_paths[204];
}

static void OP_oneTr_p_0_0_0_Ir_3_C_1_n_4(hr_complex *op_out) {
    *op_out = +(-13.8564064605510183) * mom_def_Cp_tr_paths[199] + (-13.8564064605510183) * mom_def_Cp_tr_paths[200] +
              (13.8564064605510183) * mom_def_Cp_tr_paths[201] + (13.8564064605510183) * mom_def_Cp_tr_paths[202];
}

static hr_complex path205(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path209(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path213(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path215(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path207(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path206(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path210(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path214(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path212(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path216(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path208(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path211(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static void OP_oneTr_p_0_0_0_Ir_3_C_1_n_5(hr_complex *op_out) {
    *op_out = +(-2.) * mom_def_Cp_tr_paths[205] + (4.) * mom_def_Cp_tr_paths[209] + (-2.) * mom_def_Cp_tr_paths[213] +
              (-2.) * mom_def_Cp_tr_paths[215] + (-2.) * mom_def_Cp_tr_paths[207] + (-2.) * mom_def_Cp_tr_paths[206] +
              (-2.) * mom_def_Cp_tr_paths[210] + (-2.) * mom_def_Cp_tr_paths[214] + (4.) * mom_def_Cp_tr_paths[212] +
              (-2.) * mom_def_Cp_tr_paths[216] + (4.) * mom_def_Cp_tr_paths[208] + (4.) * mom_def_Cp_tr_paths[211];
}

static void OP_oneTr_p_0_0_0_Ir_3_C_1_n_6(hr_complex *op_out) {
    *op_out = +(-3.46410161513775459) * mom_def_Cp_tr_paths[205] + (3.46410161513775459) * mom_def_Cp_tr_paths[213] +
              (3.46410161513775459) * mom_def_Cp_tr_paths[215] + (-3.46410161513775459) * mom_def_Cp_tr_paths[207] +
              (-3.46410161513775459) * mom_def_Cp_tr_paths[206] + (-3.46410161513775459) * mom_def_Cp_tr_paths[210] +
              (3.46410161513775459) * mom_def_Cp_tr_paths[214] + (3.46410161513775459) * mom_def_Cp_tr_paths[216];
}

static void OP_oneTr_p_0_0_0_Ir_3_C_1_n_7(hr_complex *op_out) {
    *op_out = +(2.) * mom_def_Cp_tr_paths[7] + (2.) * mom_def_Cp_tr_paths[10] + (2.) * mom_def_Cp_tr_paths[11] +
              (2.) * mom_def_Cp_tr_paths[14] + (2.) * mom_def_Cp_tr_paths[8] + (2.) * mom_def_Cp_tr_paths[9] +
              (-4.) * mom_def_Cp_tr_paths[27] + (2.) * mom_def_Cp_tr_paths[19] + (-4.) * mom_def_Cp_tr_paths[29] +
              (2.) * mom_def_Cp_tr_paths[20] + (2.) * mom_def_Cp_tr_paths[17] + (2.) * mom_def_Cp_tr_paths[18] +
              (2.) * mom_def_Cp_tr_paths[15] + (2.) * mom_def_Cp_tr_paths[22] + (2.) * mom_def_Cp_tr_paths[16] +
              (2.) * mom_def_Cp_tr_paths[12] + (2.) * mom_def_Cp_tr_paths[21] + (2.) * mom_def_Cp_tr_paths[13] +
              (-4.) * mom_def_Cp_tr_paths[25] + (-4.) * mom_def_Cp_tr_paths[28] + (-4.) * mom_def_Cp_tr_paths[26] +
              (-4.) * mom_def_Cp_tr_paths[23] + (-4.) * mom_def_Cp_tr_paths[30] + (-4.) * mom_def_Cp_tr_paths[24];
}

static void OP_oneTr_p_0_0_0_Ir_3_C_1_n_8(hr_complex *op_out) {
    *op_out = +(-3.46410161513775459) * mom_def_Cp_tr_paths[7] + (-3.46410161513775459) * mom_def_Cp_tr_paths[10] +
              (-3.46410161513775459) * mom_def_Cp_tr_paths[11] + (-3.46410161513775459) * mom_def_Cp_tr_paths[14] +
              (-3.46410161513775459) * mom_def_Cp_tr_paths[8] + (-3.46410161513775459) * mom_def_Cp_tr_paths[9] +
              (3.46410161513775459) * mom_def_Cp_tr_paths[19] + (3.46410161513775459) * mom_def_Cp_tr_paths[20] +
              (3.46410161513775459) * mom_def_Cp_tr_paths[17] + (3.46410161513775459) * mom_def_Cp_tr_paths[18] +
              (3.46410161513775459) * mom_def_Cp_tr_paths[15] + (3.46410161513775459) * mom_def_Cp_tr_paths[22] +
              (3.46410161513775459) * mom_def_Cp_tr_paths[16] + (-3.46410161513775459) * mom_def_Cp_tr_paths[12] +
              (3.46410161513775459) * mom_def_Cp_tr_paths[21] + (-3.46410161513775459) * mom_def_Cp_tr_paths[13];
}

static void OP_oneTr_p_0_0_0_Ir_5_C_1_n_1(hr_complex *op_out) {
    *op_out = +(4. + I * 4.) * mom_def_Cp_tr_paths[3] + (-4. + I * 4.) * mom_def_Cp_tr_paths[4] +
              (-4. - I * 4.) * mom_def_Cp_tr_paths[6] + (4. - I * 4.) * mom_def_Cp_tr_paths[5];
}

static void OP_oneTr_p_0_0_0_Ir_5_C_1_n_2(hr_complex *op_out) {
    *op_out = +(-5.6568542494923802) * mom_def_Cp_tr_paths[3] + (5.6568542494923802) * mom_def_Cp_tr_paths[4] +
              (-5.6568542494923802) * mom_def_Cp_tr_paths[6] + (5.6568542494923802) * mom_def_Cp_tr_paths[5];
}

static void OP_oneTr_p_0_0_0_Ir_5_C_1_n_3(hr_complex *op_out) {
    *op_out = +(4. - I * 4.) * mom_def_Cp_tr_paths[3] + (-4. - I * 4.) * mom_def_Cp_tr_paths[4] +
              (-4. + I * 4.) * mom_def_Cp_tr_paths[6] + (4. + I * 4.) * mom_def_Cp_tr_paths[5];
}

static void OP_oneTr_p_0_0_0_Ir_5_C_1_n_4(hr_complex *op_out) {
    *op_out = +(+I * 4.) * mom_def_Cp_tr_paths[205] + (4.) * mom_def_Cp_tr_paths[213] + (-4.) * mom_def_Cp_tr_paths[215] +
              (-I * 4.) * mom_def_Cp_tr_paths[207] + (+I * 4.) * mom_def_Cp_tr_paths[206] +
              (-I * 4.) * mom_def_Cp_tr_paths[210] + (-4.) * mom_def_Cp_tr_paths[214] + (4.) * mom_def_Cp_tr_paths[216];
}

static void OP_oneTr_p_0_0_0_Ir_5_C_1_n_5(hr_complex *op_out) {
    *op_out = +(5.6568542494923802) * mom_def_Cp_tr_paths[209] + (-5.6568542494923802) * mom_def_Cp_tr_paths[212] +
              (-5.6568542494923802) * mom_def_Cp_tr_paths[208] + (5.6568542494923802) * mom_def_Cp_tr_paths[211];
}

static void OP_oneTr_p_0_0_0_Ir_5_C_1_n_6(hr_complex *op_out) {
    *op_out = +(-I * 4.) * mom_def_Cp_tr_paths[205] + (4.) * mom_def_Cp_tr_paths[213] + (-4.) * mom_def_Cp_tr_paths[215] +
              (+I * 4.) * mom_def_Cp_tr_paths[207] + (-I * 4.) * mom_def_Cp_tr_paths[206] +
              (+I * 4.) * mom_def_Cp_tr_paths[210] + (-4.) * mom_def_Cp_tr_paths[214] + (4.) * mom_def_Cp_tr_paths[216];
}

static void OP_oneTr_p_0_0_0_Ir_5_C_1_n_7(hr_complex *op_out) {
    *op_out = +(+I * 2.) * mom_def_Cp_tr_paths[7] + (+I * 2.) * mom_def_Cp_tr_paths[8] + (-I * 2.) * mom_def_Cp_tr_paths[9] +
              (-2.) * mom_def_Cp_tr_paths[27] + (-2.) * mom_def_Cp_tr_paths[19] + (-I * 2.) * mom_def_Cp_tr_paths[29] +
              (2.) * mom_def_Cp_tr_paths[20] + (2.) * mom_def_Cp_tr_paths[15] + (-2.) * mom_def_Cp_tr_paths[16] +
              (-I * 2.) * mom_def_Cp_tr_paths[12] + (+I * 2.) * mom_def_Cp_tr_paths[25] + (2.) * mom_def_Cp_tr_paths[28] +
              (-I * 2.) * mom_def_Cp_tr_paths[26] + (2.) * mom_def_Cp_tr_paths[23] + (+I * 2.) * mom_def_Cp_tr_paths[30] +
              (-2.) * mom_def_Cp_tr_paths[24];
}

static void OP_oneTr_p_0_0_0_Ir_5_C_1_n_8(hr_complex *op_out) {
    *op_out = +(-2.8284271247461901) * mom_def_Cp_tr_paths[10] + (2.8284271247461901) * mom_def_Cp_tr_paths[11] +
              (-2.8284271247461901) * mom_def_Cp_tr_paths[14] + (-2.8284271247461901) * mom_def_Cp_tr_paths[17] +
              (2.8284271247461901) * mom_def_Cp_tr_paths[18] + (-2.8284271247461901) * mom_def_Cp_tr_paths[22] +
              (2.8284271247461901) * mom_def_Cp_tr_paths[21] + (2.8284271247461901) * mom_def_Cp_tr_paths[13];
}

static void OP_oneTr_p_0_0_0_Ir_5_C_1_n_9(hr_complex *op_out) {
    *op_out = +(-I * 2.) * mom_def_Cp_tr_paths[7] + (-I * 2.) * mom_def_Cp_tr_paths[8] + (+I * 2.) * mom_def_Cp_tr_paths[9] +
              (-2.) * mom_def_Cp_tr_paths[27] + (-2.) * mom_def_Cp_tr_paths[19] + (+I * 2.) * mom_def_Cp_tr_paths[29] +
              (2.) * mom_def_Cp_tr_paths[20] + (2.) * mom_def_Cp_tr_paths[15] + (-2.) * mom_def_Cp_tr_paths[16] +
              (+I * 2.) * mom_def_Cp_tr_paths[12] + (-I * 2.) * mom_def_Cp_tr_paths[25] + (2.) * mom_def_Cp_tr_paths[28] +
              (+I * 2.) * mom_def_Cp_tr_paths[26] + (2.) * mom_def_Cp_tr_paths[23] + (-I * 2.) * mom_def_Cp_tr_paths[30] +
              (-2.) * mom_def_Cp_tr_paths[24];
}

static hr_complex path217(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path227(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path228(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path218(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path219(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path222(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path223(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path220(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path221(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path225(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path226(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path229(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path230(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path224(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path231(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path232(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static void OP_oneTr_p_0_0_0_Ir_5_C_1_n_10(hr_complex *op_out) {
    *op_out = +(2.) * mom_def_Cp_tr_paths[217] + (+I * 2.) * mom_def_Cp_tr_paths[227] + (-I * 2.) * mom_def_Cp_tr_paths[228] +
              (-2.) * mom_def_Cp_tr_paths[218] + (2.) * mom_def_Cp_tr_paths[219] + (-2.) * mom_def_Cp_tr_paths[222] +
              (2.) * mom_def_Cp_tr_paths[223] + (2.) * mom_def_Cp_tr_paths[220] + (-2.) * mom_def_Cp_tr_paths[221] +
              (-I * 2.) * mom_def_Cp_tr_paths[225] + (+I * 2.) * mom_def_Cp_tr_paths[226] +
              (-I * 2.) * mom_def_Cp_tr_paths[229] + (+I * 2.) * mom_def_Cp_tr_paths[230] + (-2.) * mom_def_Cp_tr_paths[224] +
              (+I * 2.) * mom_def_Cp_tr_paths[231] + (-I * 2.) * mom_def_Cp_tr_paths[232];
}

static hr_complex path235(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path236(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path239(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path240(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 2);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path233(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path234(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path237(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static hr_complex path238(int in) {
    suNg *w1, *w2;
    suNg res, res1;
    int site = in;
    hr_complex p;

    w2 = pu_gauge_wrk(site, 3);

    site = iup_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg(res, *w2, *w1);

    site = iup_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg(res1, res, *w1);

    site = iup_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg(res, res1, *w1);

    site = iup_wrk(site, 3);
    site = idn_wrk(site, 1);
    w1 = pu_gauge_wrk(site, 1);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res, res1, *w1);

    site = idn_wrk(site, 3);
    w1 = pu_gauge_wrk(site, 3);
    _suNg_times_suNg_dagger(res1, res, *w1);

    site = idn_wrk(site, 2);
    w1 = pu_gauge_wrk(site, 2);
    _suNg_times_suNg_dagger(res, res1, *w1);

    _suNg_trace(p, res);
    return p;
}

static void OP_oneTr_p_0_0_0_Ir_5_C_1_n_11(hr_complex *op_out) {
    *op_out = +(-2.8284271247461901) * mom_def_Cp_tr_paths[235] + (2.8284271247461901) * mom_def_Cp_tr_paths[236] +
              (-2.8284271247461901) * mom_def_Cp_tr_paths[239] + (2.8284271247461901) * mom_def_Cp_tr_paths[240] +
              (2.8284271247461901) * mom_def_Cp_tr_paths[233] + (-2.8284271247461901) * mom_def_Cp_tr_paths[234] +
              (2.8284271247461901) * mom_def_Cp_tr_paths[237] + (-2.8284271247461901) * mom_def_Cp_tr_paths[238];
}

static void OP_oneTr_p_0_0_0_Ir_5_C_1_n_12(hr_complex *op_out) {
    *op_out = +(2.) * mom_def_Cp_tr_paths[217] + (-I * 2.) * mom_def_Cp_tr_paths[227] + (+I * 2.) * mom_def_Cp_tr_paths[228] +
              (-2.) * mom_def_Cp_tr_paths[218] + (2.) * mom_def_Cp_tr_paths[219] + (-2.) * mom_def_Cp_tr_paths[222] +
              (2.) * mom_def_Cp_tr_paths[223] + (2.) * mom_def_Cp_tr_paths[220] + (-2.) * mom_def_Cp_tr_paths[221] +
              (+I * 2.) * mom_def_Cp_tr_paths[225] + (-I * 2.) * mom_def_Cp_tr_paths[226] +
              (+I * 2.) * mom_def_Cp_tr_paths[229] + (-I * 2.) * mom_def_Cp_tr_paths[230] + (-2.) * mom_def_Cp_tr_paths[224] +
              (-I * 2.) * mom_def_Cp_tr_paths[231] + (+I * 2.) * mom_def_Cp_tr_paths[232];
}

static void OP_oneTr_p_0_0_1_Ir_1_C_1_n_1(hr_complex *op_out) {
    *op_out = +(8.) * mom_def_Cp_tr_paths[0];
}

static void OP_oneTr_p_0_1_0_Ir_1_C_1_n_1(hr_complex *op_out) {
    *op_out = +(8.) * mom_def_Cp_tr_paths[1];
}

static void OP_oneTr_p_1_0_0_Ir_1_C_1_n_1(hr_complex *op_out) {
    *op_out = +(8.) * mom_def_Cp_tr_paths[2];
}

static int last_t = -10;
void request_space_paths_evaluation() {
    last_t = -10;
}
void eval_time_momentum_glueball_paths(int t, int px, int py, int pz) {
    int nnx, nny, nnz, idx, in;
    hr_complex ce = 0.;
    if (path_storage == NULL) {
        path_storage = malloc(npaths * X * Y * Z * sizeof(hr_complex));
        mom_def_Cp_tr_paths = malloc(npaths * sizeof(hr_complex));
        mom_def_Cm_tr_paths = malloc(npaths * sizeof(hr_complex));
        for (in = 0; in < npaths * X * Y * Z; in++) {
            path_storage[in] = 0.;
        }
    }
    for (in = 0; in < npaths; in++) {
        mom_def_Cp_tr_paths[in] = 0.;
        mom_def_Cm_tr_paths[in] = 0.;
    }
    if (t != last_t) {
        last_t = t;
        for (nnx = 0; nnx < X; nnx++) {
            for (nny = 0; nny < Y; nny++) {
                for (nnz = 0; nnz < Z; nnz++) {
                    in = ipt(t, nnx, nny, nnz);
                    ce = cexp(I * 2.0 * PI * (double)(nnx * px + nny * py + nnz * pz) / GLB_X);
                    idx = npaths * (nnx + X * (nny + Y * nnz));
                    path_storage[0 + idx] = path0(in);
                    mom_def_Cp_tr_paths[0] += ce * creal(path_storage[0 + idx]);
                    mom_def_Cm_tr_paths[0] += I * ce * cimag(path_storage[0 + idx]);
                    path_storage[1 + idx] = path1(in);
                    mom_def_Cp_tr_paths[1] += ce * creal(path_storage[1 + idx]);
                    mom_def_Cm_tr_paths[1] += I * ce * cimag(path_storage[1 + idx]);
                    path_storage[2 + idx] = path2(in);
                    mom_def_Cp_tr_paths[2] += ce * creal(path_storage[2 + idx]);
                    mom_def_Cm_tr_paths[2] += I * ce * cimag(path_storage[2 + idx]);
                    path_storage[3 + idx] = path3(in);
                    mom_def_Cp_tr_paths[3] += ce * creal(path_storage[3 + idx]);
                    mom_def_Cm_tr_paths[3] += I * ce * cimag(path_storage[3 + idx]);
                    path_storage[4 + idx] = path4(in);
                    mom_def_Cp_tr_paths[4] += ce * creal(path_storage[4 + idx]);
                    mom_def_Cm_tr_paths[4] += I * ce * cimag(path_storage[4 + idx]);
                    path_storage[5 + idx] = path5(in);
                    mom_def_Cp_tr_paths[5] += ce * creal(path_storage[5 + idx]);
                    mom_def_Cm_tr_paths[5] += I * ce * cimag(path_storage[5 + idx]);
                    path_storage[6 + idx] = path6(in);
                    mom_def_Cp_tr_paths[6] += ce * creal(path_storage[6 + idx]);
                    mom_def_Cm_tr_paths[6] += I * ce * cimag(path_storage[6 + idx]);
                    path_storage[7 + idx] = path7(in);
                    mom_def_Cp_tr_paths[7] += ce * creal(path_storage[7 + idx]);
                    mom_def_Cm_tr_paths[7] += I * ce * cimag(path_storage[7 + idx]);
                    path_storage[8 + idx] = path8(in);
                    mom_def_Cp_tr_paths[8] += ce * creal(path_storage[8 + idx]);
                    mom_def_Cm_tr_paths[8] += I * ce * cimag(path_storage[8 + idx]);
                    path_storage[9 + idx] = path9(in);
                    mom_def_Cp_tr_paths[9] += ce * creal(path_storage[9 + idx]);
                    mom_def_Cm_tr_paths[9] += I * ce * cimag(path_storage[9 + idx]);
                    path_storage[10 + idx] = path10(in);
                    mom_def_Cp_tr_paths[10] += ce * creal(path_storage[10 + idx]);
                    mom_def_Cm_tr_paths[10] += I * ce * cimag(path_storage[10 + idx]);
                    path_storage[11 + idx] = path11(in);
                    mom_def_Cp_tr_paths[11] += ce * creal(path_storage[11 + idx]);
                    mom_def_Cm_tr_paths[11] += I * ce * cimag(path_storage[11 + idx]);
                    path_storage[12 + idx] = path12(in);
                    mom_def_Cp_tr_paths[12] += ce * creal(path_storage[12 + idx]);
                    mom_def_Cm_tr_paths[12] += I * ce * cimag(path_storage[12 + idx]);
                    path_storage[13 + idx] = path13(in);
                    mom_def_Cp_tr_paths[13] += ce * creal(path_storage[13 + idx]);
                    mom_def_Cm_tr_paths[13] += I * ce * cimag(path_storage[13 + idx]);
                    path_storage[14 + idx] = path14(in);
                    mom_def_Cp_tr_paths[14] += ce * creal(path_storage[14 + idx]);
                    mom_def_Cm_tr_paths[14] += I * ce * cimag(path_storage[14 + idx]);
                    path_storage[15 + idx] = path15(in);
                    mom_def_Cp_tr_paths[15] += ce * creal(path_storage[15 + idx]);
                    mom_def_Cm_tr_paths[15] += I * ce * cimag(path_storage[15 + idx]);
                    path_storage[16 + idx] = path16(in);
                    mom_def_Cp_tr_paths[16] += ce * creal(path_storage[16 + idx]);
                    mom_def_Cm_tr_paths[16] += I * ce * cimag(path_storage[16 + idx]);
                    path_storage[17 + idx] = path17(in);
                    mom_def_Cp_tr_paths[17] += ce * creal(path_storage[17 + idx]);
                    mom_def_Cm_tr_paths[17] += I * ce * cimag(path_storage[17 + idx]);
                    path_storage[18 + idx] = path18(in);
                    mom_def_Cp_tr_paths[18] += ce * creal(path_storage[18 + idx]);
                    mom_def_Cm_tr_paths[18] += I * ce * cimag(path_storage[18 + idx]);
                    path_storage[19 + idx] = path19(in);
                    mom_def_Cp_tr_paths[19] += ce * creal(path_storage[19 + idx]);
                    mom_def_Cm_tr_paths[19] += I * ce * cimag(path_storage[19 + idx]);
                    path_storage[20 + idx] = path20(in);
                    mom_def_Cp_tr_paths[20] += ce * creal(path_storage[20 + idx]);
                    mom_def_Cm_tr_paths[20] += I * ce * cimag(path_storage[20 + idx]);
                    path_storage[21 + idx] = path21(in);
                    mom_def_Cp_tr_paths[21] += ce * creal(path_storage[21 + idx]);
                    mom_def_Cm_tr_paths[21] += I * ce * cimag(path_storage[21 + idx]);
                    path_storage[22 + idx] = path22(in);
                    mom_def_Cp_tr_paths[22] += ce * creal(path_storage[22 + idx]);
                    mom_def_Cm_tr_paths[22] += I * ce * cimag(path_storage[22 + idx]);
                    path_storage[23 + idx] = path23(in);
                    mom_def_Cp_tr_paths[23] += ce * creal(path_storage[23 + idx]);
                    mom_def_Cm_tr_paths[23] += I * ce * cimag(path_storage[23 + idx]);
                    path_storage[24 + idx] = path24(in);
                    mom_def_Cp_tr_paths[24] += ce * creal(path_storage[24 + idx]);
                    mom_def_Cm_tr_paths[24] += I * ce * cimag(path_storage[24 + idx]);
                    path_storage[25 + idx] = path25(in);
                    mom_def_Cp_tr_paths[25] += ce * creal(path_storage[25 + idx]);
                    mom_def_Cm_tr_paths[25] += I * ce * cimag(path_storage[25 + idx]);
                    path_storage[26 + idx] = path26(in);
                    mom_def_Cp_tr_paths[26] += ce * creal(path_storage[26 + idx]);
                    mom_def_Cm_tr_paths[26] += I * ce * cimag(path_storage[26 + idx]);
                    path_storage[27 + idx] = path27(in);
                    mom_def_Cp_tr_paths[27] += ce * creal(path_storage[27 + idx]);
                    mom_def_Cm_tr_paths[27] += I * ce * cimag(path_storage[27 + idx]);
                    path_storage[28 + idx] = path28(in);
                    mom_def_Cp_tr_paths[28] += ce * creal(path_storage[28 + idx]);
                    mom_def_Cm_tr_paths[28] += I * ce * cimag(path_storage[28 + idx]);
                    path_storage[29 + idx] = path29(in);
                    mom_def_Cp_tr_paths[29] += ce * creal(path_storage[29 + idx]);
                    mom_def_Cm_tr_paths[29] += I * ce * cimag(path_storage[29 + idx]);
                    path_storage[30 + idx] = path30(in);
                    mom_def_Cp_tr_paths[30] += ce * creal(path_storage[30 + idx]);
                    mom_def_Cm_tr_paths[30] += I * ce * cimag(path_storage[30 + idx]);
                    path_storage[31 + idx] = path31(in);
                    mom_def_Cp_tr_paths[31] += ce * creal(path_storage[31 + idx]);
                    mom_def_Cm_tr_paths[31] += I * ce * cimag(path_storage[31 + idx]);
                    path_storage[32 + idx] = path32(in);
                    mom_def_Cp_tr_paths[32] += ce * creal(path_storage[32 + idx]);
                    mom_def_Cm_tr_paths[32] += I * ce * cimag(path_storage[32 + idx]);
                    path_storage[33 + idx] = path33(in);
                    mom_def_Cp_tr_paths[33] += ce * creal(path_storage[33 + idx]);
                    mom_def_Cm_tr_paths[33] += I * ce * cimag(path_storage[33 + idx]);
                    path_storage[34 + idx] = path34(in);
                    mom_def_Cp_tr_paths[34] += ce * creal(path_storage[34 + idx]);
                    mom_def_Cm_tr_paths[34] += I * ce * cimag(path_storage[34 + idx]);
                    path_storage[35 + idx] = path35(in);
                    mom_def_Cp_tr_paths[35] += ce * creal(path_storage[35 + idx]);
                    mom_def_Cm_tr_paths[35] += I * ce * cimag(path_storage[35 + idx]);
                    path_storage[36 + idx] = path36(in);
                    mom_def_Cp_tr_paths[36] += ce * creal(path_storage[36 + idx]);
                    mom_def_Cm_tr_paths[36] += I * ce * cimag(path_storage[36 + idx]);
                    path_storage[37 + idx] = path37(in);
                    mom_def_Cp_tr_paths[37] += ce * creal(path_storage[37 + idx]);
                    mom_def_Cm_tr_paths[37] += I * ce * cimag(path_storage[37 + idx]);
                    path_storage[38 + idx] = path38(in);
                    mom_def_Cp_tr_paths[38] += ce * creal(path_storage[38 + idx]);
                    mom_def_Cm_tr_paths[38] += I * ce * cimag(path_storage[38 + idx]);
                    path_storage[39 + idx] = path39(in);
                    mom_def_Cp_tr_paths[39] += ce * creal(path_storage[39 + idx]);
                    mom_def_Cm_tr_paths[39] += I * ce * cimag(path_storage[39 + idx]);
                    path_storage[40 + idx] = path40(in);
                    mom_def_Cp_tr_paths[40] += ce * creal(path_storage[40 + idx]);
                    mom_def_Cm_tr_paths[40] += I * ce * cimag(path_storage[40 + idx]);
                    path_storage[41 + idx] = path41(in);
                    mom_def_Cp_tr_paths[41] += ce * creal(path_storage[41 + idx]);
                    mom_def_Cm_tr_paths[41] += I * ce * cimag(path_storage[41 + idx]);
                    path_storage[42 + idx] = path42(in);
                    mom_def_Cp_tr_paths[42] += ce * creal(path_storage[42 + idx]);
                    mom_def_Cm_tr_paths[42] += I * ce * cimag(path_storage[42 + idx]);
                    path_storage[43 + idx] = path43(in);
                    mom_def_Cp_tr_paths[43] += ce * creal(path_storage[43 + idx]);
                    mom_def_Cm_tr_paths[43] += I * ce * cimag(path_storage[43 + idx]);
                    path_storage[44 + idx] = path44(in);
                    mom_def_Cp_tr_paths[44] += ce * creal(path_storage[44 + idx]);
                    mom_def_Cm_tr_paths[44] += I * ce * cimag(path_storage[44 + idx]);
                    path_storage[45 + idx] = path45(in);
                    mom_def_Cp_tr_paths[45] += ce * creal(path_storage[45 + idx]);
                    mom_def_Cm_tr_paths[45] += I * ce * cimag(path_storage[45 + idx]);
                    path_storage[46 + idx] = path46(in);
                    mom_def_Cp_tr_paths[46] += ce * creal(path_storage[46 + idx]);
                    mom_def_Cm_tr_paths[46] += I * ce * cimag(path_storage[46 + idx]);
                    path_storage[47 + idx] = path47(in);
                    mom_def_Cp_tr_paths[47] += ce * creal(path_storage[47 + idx]);
                    mom_def_Cm_tr_paths[47] += I * ce * cimag(path_storage[47 + idx]);
                    path_storage[48 + idx] = path48(in);
                    mom_def_Cp_tr_paths[48] += ce * creal(path_storage[48 + idx]);
                    mom_def_Cm_tr_paths[48] += I * ce * cimag(path_storage[48 + idx]);
                    path_storage[49 + idx] = path49(in);
                    mom_def_Cp_tr_paths[49] += ce * creal(path_storage[49 + idx]);
                    mom_def_Cm_tr_paths[49] += I * ce * cimag(path_storage[49 + idx]);
                    path_storage[50 + idx] = path50(in);
                    mom_def_Cp_tr_paths[50] += ce * creal(path_storage[50 + idx]);
                    mom_def_Cm_tr_paths[50] += I * ce * cimag(path_storage[50 + idx]);
                    path_storage[51 + idx] = path51(in);
                    mom_def_Cp_tr_paths[51] += ce * creal(path_storage[51 + idx]);
                    mom_def_Cm_tr_paths[51] += I * ce * cimag(path_storage[51 + idx]);
                    path_storage[52 + idx] = path52(in);
                    mom_def_Cp_tr_paths[52] += ce * creal(path_storage[52 + idx]);
                    mom_def_Cm_tr_paths[52] += I * ce * cimag(path_storage[52 + idx]);
                    path_storage[53 + idx] = path53(in);
                    mom_def_Cp_tr_paths[53] += ce * creal(path_storage[53 + idx]);
                    mom_def_Cm_tr_paths[53] += I * ce * cimag(path_storage[53 + idx]);
                    path_storage[54 + idx] = path54(in);
                    mom_def_Cp_tr_paths[54] += ce * creal(path_storage[54 + idx]);
                    mom_def_Cm_tr_paths[54] += I * ce * cimag(path_storage[54 + idx]);
                    path_storage[55 + idx] = path55(in);
                    mom_def_Cp_tr_paths[55] += ce * creal(path_storage[55 + idx]);
                    mom_def_Cm_tr_paths[55] += I * ce * cimag(path_storage[55 + idx]);
                    path_storage[56 + idx] = path56(in);
                    mom_def_Cp_tr_paths[56] += ce * creal(path_storage[56 + idx]);
                    mom_def_Cm_tr_paths[56] += I * ce * cimag(path_storage[56 + idx]);
                    path_storage[57 + idx] = path57(in);
                    mom_def_Cp_tr_paths[57] += ce * creal(path_storage[57 + idx]);
                    mom_def_Cm_tr_paths[57] += I * ce * cimag(path_storage[57 + idx]);
                    path_storage[58 + idx] = path58(in);
                    mom_def_Cp_tr_paths[58] += ce * creal(path_storage[58 + idx]);
                    mom_def_Cm_tr_paths[58] += I * ce * cimag(path_storage[58 + idx]);
                    path_storage[59 + idx] = path59(in);
                    mom_def_Cp_tr_paths[59] += ce * creal(path_storage[59 + idx]);
                    mom_def_Cm_tr_paths[59] += I * ce * cimag(path_storage[59 + idx]);
                    path_storage[60 + idx] = path60(in);
                    mom_def_Cp_tr_paths[60] += ce * creal(path_storage[60 + idx]);
                    mom_def_Cm_tr_paths[60] += I * ce * cimag(path_storage[60 + idx]);
                    path_storage[61 + idx] = path61(in);
                    mom_def_Cp_tr_paths[61] += ce * creal(path_storage[61 + idx]);
                    mom_def_Cm_tr_paths[61] += I * ce * cimag(path_storage[61 + idx]);
                    path_storage[62 + idx] = path62(in);
                    mom_def_Cp_tr_paths[62] += ce * creal(path_storage[62 + idx]);
                    mom_def_Cm_tr_paths[62] += I * ce * cimag(path_storage[62 + idx]);
                    path_storage[63 + idx] = path63(in);
                    mom_def_Cp_tr_paths[63] += ce * creal(path_storage[63 + idx]);
                    mom_def_Cm_tr_paths[63] += I * ce * cimag(path_storage[63 + idx]);
                    path_storage[64 + idx] = path64(in);
                    mom_def_Cp_tr_paths[64] += ce * creal(path_storage[64 + idx]);
                    mom_def_Cm_tr_paths[64] += I * ce * cimag(path_storage[64 + idx]);
                    path_storage[65 + idx] = path65(in);
                    mom_def_Cp_tr_paths[65] += ce * creal(path_storage[65 + idx]);
                    mom_def_Cm_tr_paths[65] += I * ce * cimag(path_storage[65 + idx]);
                    path_storage[66 + idx] = path66(in);
                    mom_def_Cp_tr_paths[66] += ce * creal(path_storage[66 + idx]);
                    mom_def_Cm_tr_paths[66] += I * ce * cimag(path_storage[66 + idx]);
                    path_storage[67 + idx] = path67(in);
                    mom_def_Cp_tr_paths[67] += ce * creal(path_storage[67 + idx]);
                    mom_def_Cm_tr_paths[67] += I * ce * cimag(path_storage[67 + idx]);
                    path_storage[68 + idx] = path68(in);
                    mom_def_Cp_tr_paths[68] += ce * creal(path_storage[68 + idx]);
                    mom_def_Cm_tr_paths[68] += I * ce * cimag(path_storage[68 + idx]);
                    path_storage[69 + idx] = path69(in);
                    mom_def_Cp_tr_paths[69] += ce * creal(path_storage[69 + idx]);
                    mom_def_Cm_tr_paths[69] += I * ce * cimag(path_storage[69 + idx]);
                    path_storage[70 + idx] = path70(in);
                    mom_def_Cp_tr_paths[70] += ce * creal(path_storage[70 + idx]);
                    mom_def_Cm_tr_paths[70] += I * ce * cimag(path_storage[70 + idx]);
                    path_storage[71 + idx] = path71(in);
                    mom_def_Cp_tr_paths[71] += ce * creal(path_storage[71 + idx]);
                    mom_def_Cm_tr_paths[71] += I * ce * cimag(path_storage[71 + idx]);
                    path_storage[72 + idx] = path72(in);
                    mom_def_Cp_tr_paths[72] += ce * creal(path_storage[72 + idx]);
                    mom_def_Cm_tr_paths[72] += I * ce * cimag(path_storage[72 + idx]);
                    path_storage[73 + idx] = path73(in);
                    mom_def_Cp_tr_paths[73] += ce * creal(path_storage[73 + idx]);
                    mom_def_Cm_tr_paths[73] += I * ce * cimag(path_storage[73 + idx]);
                    path_storage[74 + idx] = path74(in);
                    mom_def_Cp_tr_paths[74] += ce * creal(path_storage[74 + idx]);
                    mom_def_Cm_tr_paths[74] += I * ce * cimag(path_storage[74 + idx]);
                    path_storage[75 + idx] = path75(in);
                    mom_def_Cp_tr_paths[75] += ce * creal(path_storage[75 + idx]);
                    mom_def_Cm_tr_paths[75] += I * ce * cimag(path_storage[75 + idx]);
                    path_storage[76 + idx] = path76(in);
                    mom_def_Cp_tr_paths[76] += ce * creal(path_storage[76 + idx]);
                    mom_def_Cm_tr_paths[76] += I * ce * cimag(path_storage[76 + idx]);
                    path_storage[77 + idx] = path77(in);
                    mom_def_Cp_tr_paths[77] += ce * creal(path_storage[77 + idx]);
                    mom_def_Cm_tr_paths[77] += I * ce * cimag(path_storage[77 + idx]);
                    path_storage[78 + idx] = path78(in);
                    mom_def_Cp_tr_paths[78] += ce * creal(path_storage[78 + idx]);
                    mom_def_Cm_tr_paths[78] += I * ce * cimag(path_storage[78 + idx]);
                    path_storage[79 + idx] = path79(in);
                    mom_def_Cp_tr_paths[79] += ce * creal(path_storage[79 + idx]);
                    mom_def_Cm_tr_paths[79] += I * ce * cimag(path_storage[79 + idx]);
                    path_storage[80 + idx] = path80(in);
                    mom_def_Cp_tr_paths[80] += ce * creal(path_storage[80 + idx]);
                    mom_def_Cm_tr_paths[80] += I * ce * cimag(path_storage[80 + idx]);
                    path_storage[81 + idx] = path81(in);
                    mom_def_Cp_tr_paths[81] += ce * creal(path_storage[81 + idx]);
                    mom_def_Cm_tr_paths[81] += I * ce * cimag(path_storage[81 + idx]);
                    path_storage[82 + idx] = path82(in);
                    mom_def_Cp_tr_paths[82] += ce * creal(path_storage[82 + idx]);
                    mom_def_Cm_tr_paths[82] += I * ce * cimag(path_storage[82 + idx]);
                    path_storage[83 + idx] = path83(in);
                    mom_def_Cp_tr_paths[83] += ce * creal(path_storage[83 + idx]);
                    mom_def_Cm_tr_paths[83] += I * ce * cimag(path_storage[83 + idx]);
                    path_storage[84 + idx] = path84(in);
                    mom_def_Cp_tr_paths[84] += ce * creal(path_storage[84 + idx]);
                    mom_def_Cm_tr_paths[84] += I * ce * cimag(path_storage[84 + idx]);
                    path_storage[85 + idx] = path85(in);
                    mom_def_Cp_tr_paths[85] += ce * creal(path_storage[85 + idx]);
                    mom_def_Cm_tr_paths[85] += I * ce * cimag(path_storage[85 + idx]);
                    path_storage[86 + idx] = path86(in);
                    mom_def_Cp_tr_paths[86] += ce * creal(path_storage[86 + idx]);
                    mom_def_Cm_tr_paths[86] += I * ce * cimag(path_storage[86 + idx]);
                    path_storage[87 + idx] = path87(in);
                    mom_def_Cp_tr_paths[87] += ce * creal(path_storage[87 + idx]);
                    mom_def_Cm_tr_paths[87] += I * ce * cimag(path_storage[87 + idx]);
                    path_storage[88 + idx] = path88(in);
                    mom_def_Cp_tr_paths[88] += ce * creal(path_storage[88 + idx]);
                    mom_def_Cm_tr_paths[88] += I * ce * cimag(path_storage[88 + idx]);
                    path_storage[89 + idx] = path89(in);
                    mom_def_Cp_tr_paths[89] += ce * creal(path_storage[89 + idx]);
                    mom_def_Cm_tr_paths[89] += I * ce * cimag(path_storage[89 + idx]);
                    path_storage[90 + idx] = path90(in);
                    mom_def_Cp_tr_paths[90] += ce * creal(path_storage[90 + idx]);
                    mom_def_Cm_tr_paths[90] += I * ce * cimag(path_storage[90 + idx]);
                    path_storage[91 + idx] = path91(in);
                    mom_def_Cp_tr_paths[91] += ce * creal(path_storage[91 + idx]);
                    mom_def_Cm_tr_paths[91] += I * ce * cimag(path_storage[91 + idx]);
                    path_storage[92 + idx] = path92(in);
                    mom_def_Cp_tr_paths[92] += ce * creal(path_storage[92 + idx]);
                    mom_def_Cm_tr_paths[92] += I * ce * cimag(path_storage[92 + idx]);
                    path_storage[93 + idx] = path93(in);
                    mom_def_Cp_tr_paths[93] += ce * creal(path_storage[93 + idx]);
                    mom_def_Cm_tr_paths[93] += I * ce * cimag(path_storage[93 + idx]);
                    path_storage[94 + idx] = path94(in);
                    mom_def_Cp_tr_paths[94] += ce * creal(path_storage[94 + idx]);
                    mom_def_Cm_tr_paths[94] += I * ce * cimag(path_storage[94 + idx]);
                    path_storage[95 + idx] = path95(in);
                    mom_def_Cp_tr_paths[95] += ce * creal(path_storage[95 + idx]);
                    mom_def_Cm_tr_paths[95] += I * ce * cimag(path_storage[95 + idx]);
                    path_storage[96 + idx] = path96(in);
                    mom_def_Cp_tr_paths[96] += ce * creal(path_storage[96 + idx]);
                    mom_def_Cm_tr_paths[96] += I * ce * cimag(path_storage[96 + idx]);
                    path_storage[97 + idx] = path97(in);
                    mom_def_Cp_tr_paths[97] += ce * creal(path_storage[97 + idx]);
                    mom_def_Cm_tr_paths[97] += I * ce * cimag(path_storage[97 + idx]);
                    path_storage[98 + idx] = path98(in);
                    mom_def_Cp_tr_paths[98] += ce * creal(path_storage[98 + idx]);
                    mom_def_Cm_tr_paths[98] += I * ce * cimag(path_storage[98 + idx]);
                    path_storage[99 + idx] = path99(in);
                    mom_def_Cp_tr_paths[99] += ce * creal(path_storage[99 + idx]);
                    mom_def_Cm_tr_paths[99] += I * ce * cimag(path_storage[99 + idx]);
                    path_storage[100 + idx] = path100(in);
                    mom_def_Cp_tr_paths[100] += ce * creal(path_storage[100 + idx]);
                    mom_def_Cm_tr_paths[100] += I * ce * cimag(path_storage[100 + idx]);
                    path_storage[101 + idx] = path101(in);
                    mom_def_Cp_tr_paths[101] += ce * creal(path_storage[101 + idx]);
                    mom_def_Cm_tr_paths[101] += I * ce * cimag(path_storage[101 + idx]);
                    path_storage[102 + idx] = path102(in);
                    mom_def_Cp_tr_paths[102] += ce * creal(path_storage[102 + idx]);
                    mom_def_Cm_tr_paths[102] += I * ce * cimag(path_storage[102 + idx]);
                    path_storage[103 + idx] = path103(in);
                    mom_def_Cp_tr_paths[103] += ce * creal(path_storage[103 + idx]);
                    mom_def_Cm_tr_paths[103] += I * ce * cimag(path_storage[103 + idx]);
                    path_storage[104 + idx] = path104(in);
                    mom_def_Cp_tr_paths[104] += ce * creal(path_storage[104 + idx]);
                    mom_def_Cm_tr_paths[104] += I * ce * cimag(path_storage[104 + idx]);
                    path_storage[105 + idx] = path105(in);
                    mom_def_Cp_tr_paths[105] += ce * creal(path_storage[105 + idx]);
                    mom_def_Cm_tr_paths[105] += I * ce * cimag(path_storage[105 + idx]);
                    path_storage[106 + idx] = path106(in);
                    mom_def_Cp_tr_paths[106] += ce * creal(path_storage[106 + idx]);
                    mom_def_Cm_tr_paths[106] += I * ce * cimag(path_storage[106 + idx]);
                    path_storage[107 + idx] = path107(in);
                    mom_def_Cp_tr_paths[107] += ce * creal(path_storage[107 + idx]);
                    mom_def_Cm_tr_paths[107] += I * ce * cimag(path_storage[107 + idx]);
                    path_storage[108 + idx] = path108(in);
                    mom_def_Cp_tr_paths[108] += ce * creal(path_storage[108 + idx]);
                    mom_def_Cm_tr_paths[108] += I * ce * cimag(path_storage[108 + idx]);
                    path_storage[109 + idx] = path109(in);
                    mom_def_Cp_tr_paths[109] += ce * creal(path_storage[109 + idx]);
                    mom_def_Cm_tr_paths[109] += I * ce * cimag(path_storage[109 + idx]);
                    path_storage[110 + idx] = path110(in);
                    mom_def_Cp_tr_paths[110] += ce * creal(path_storage[110 + idx]);
                    mom_def_Cm_tr_paths[110] += I * ce * cimag(path_storage[110 + idx]);
                    path_storage[111 + idx] = path111(in);
                    mom_def_Cp_tr_paths[111] += ce * creal(path_storage[111 + idx]);
                    mom_def_Cm_tr_paths[111] += I * ce * cimag(path_storage[111 + idx]);
                    path_storage[112 + idx] = path112(in);
                    mom_def_Cp_tr_paths[112] += ce * creal(path_storage[112 + idx]);
                    mom_def_Cm_tr_paths[112] += I * ce * cimag(path_storage[112 + idx]);
                    path_storage[113 + idx] = path113(in);
                    mom_def_Cp_tr_paths[113] += ce * creal(path_storage[113 + idx]);
                    mom_def_Cm_tr_paths[113] += I * ce * cimag(path_storage[113 + idx]);
                    path_storage[114 + idx] = path114(in);
                    mom_def_Cp_tr_paths[114] += ce * creal(path_storage[114 + idx]);
                    mom_def_Cm_tr_paths[114] += I * ce * cimag(path_storage[114 + idx]);
                    path_storage[115 + idx] = path115(in);
                    mom_def_Cp_tr_paths[115] += ce * creal(path_storage[115 + idx]);
                    mom_def_Cm_tr_paths[115] += I * ce * cimag(path_storage[115 + idx]);
                    path_storage[116 + idx] = path116(in);
                    mom_def_Cp_tr_paths[116] += ce * creal(path_storage[116 + idx]);
                    mom_def_Cm_tr_paths[116] += I * ce * cimag(path_storage[116 + idx]);
                    path_storage[117 + idx] = path117(in);
                    mom_def_Cp_tr_paths[117] += ce * creal(path_storage[117 + idx]);
                    mom_def_Cm_tr_paths[117] += I * ce * cimag(path_storage[117 + idx]);
                    path_storage[118 + idx] = path118(in);
                    mom_def_Cp_tr_paths[118] += ce * creal(path_storage[118 + idx]);
                    mom_def_Cm_tr_paths[118] += I * ce * cimag(path_storage[118 + idx]);
                    path_storage[119 + idx] = path119(in);
                    mom_def_Cp_tr_paths[119] += ce * creal(path_storage[119 + idx]);
                    mom_def_Cm_tr_paths[119] += I * ce * cimag(path_storage[119 + idx]);
                    path_storage[120 + idx] = path120(in);
                    mom_def_Cp_tr_paths[120] += ce * creal(path_storage[120 + idx]);
                    mom_def_Cm_tr_paths[120] += I * ce * cimag(path_storage[120 + idx]);
                    path_storage[121 + idx] = path121(in);
                    mom_def_Cp_tr_paths[121] += ce * creal(path_storage[121 + idx]);
                    mom_def_Cm_tr_paths[121] += I * ce * cimag(path_storage[121 + idx]);
                    path_storage[122 + idx] = path122(in);
                    mom_def_Cp_tr_paths[122] += ce * creal(path_storage[122 + idx]);
                    mom_def_Cm_tr_paths[122] += I * ce * cimag(path_storage[122 + idx]);
                    path_storage[123 + idx] = path123(in);
                    mom_def_Cp_tr_paths[123] += ce * creal(path_storage[123 + idx]);
                    mom_def_Cm_tr_paths[123] += I * ce * cimag(path_storage[123 + idx]);
                    path_storage[124 + idx] = path124(in);
                    mom_def_Cp_tr_paths[124] += ce * creal(path_storage[124 + idx]);
                    mom_def_Cm_tr_paths[124] += I * ce * cimag(path_storage[124 + idx]);
                    path_storage[125 + idx] = path125(in);
                    mom_def_Cp_tr_paths[125] += ce * creal(path_storage[125 + idx]);
                    mom_def_Cm_tr_paths[125] += I * ce * cimag(path_storage[125 + idx]);
                    path_storage[126 + idx] = path126(in);
                    mom_def_Cp_tr_paths[126] += ce * creal(path_storage[126 + idx]);
                    mom_def_Cm_tr_paths[126] += I * ce * cimag(path_storage[126 + idx]);
                    path_storage[127 + idx] = path127(in);
                    mom_def_Cp_tr_paths[127] += ce * creal(path_storage[127 + idx]);
                    mom_def_Cm_tr_paths[127] += I * ce * cimag(path_storage[127 + idx]);
                    path_storage[128 + idx] = path128(in);
                    mom_def_Cp_tr_paths[128] += ce * creal(path_storage[128 + idx]);
                    mom_def_Cm_tr_paths[128] += I * ce * cimag(path_storage[128 + idx]);
                    path_storage[129 + idx] = path129(in);
                    mom_def_Cp_tr_paths[129] += ce * creal(path_storage[129 + idx]);
                    mom_def_Cm_tr_paths[129] += I * ce * cimag(path_storage[129 + idx]);
                    path_storage[130 + idx] = path130(in);
                    mom_def_Cp_tr_paths[130] += ce * creal(path_storage[130 + idx]);
                    mom_def_Cm_tr_paths[130] += I * ce * cimag(path_storage[130 + idx]);
                    path_storage[131 + idx] = path131(in);
                    mom_def_Cp_tr_paths[131] += ce * creal(path_storage[131 + idx]);
                    mom_def_Cm_tr_paths[131] += I * ce * cimag(path_storage[131 + idx]);
                    path_storage[132 + idx] = path132(in);
                    mom_def_Cp_tr_paths[132] += ce * creal(path_storage[132 + idx]);
                    mom_def_Cm_tr_paths[132] += I * ce * cimag(path_storage[132 + idx]);
                    path_storage[133 + idx] = path133(in);
                    mom_def_Cp_tr_paths[133] += ce * creal(path_storage[133 + idx]);
                    mom_def_Cm_tr_paths[133] += I * ce * cimag(path_storage[133 + idx]);
                    path_storage[134 + idx] = path134(in);
                    mom_def_Cp_tr_paths[134] += ce * creal(path_storage[134 + idx]);
                    mom_def_Cm_tr_paths[134] += I * ce * cimag(path_storage[134 + idx]);
                    path_storage[135 + idx] = path135(in);
                    mom_def_Cp_tr_paths[135] += ce * creal(path_storage[135 + idx]);
                    mom_def_Cm_tr_paths[135] += I * ce * cimag(path_storage[135 + idx]);
                    path_storage[136 + idx] = path136(in);
                    mom_def_Cp_tr_paths[136] += ce * creal(path_storage[136 + idx]);
                    mom_def_Cm_tr_paths[136] += I * ce * cimag(path_storage[136 + idx]);
                    path_storage[137 + idx] = path137(in);
                    mom_def_Cp_tr_paths[137] += ce * creal(path_storage[137 + idx]);
                    mom_def_Cm_tr_paths[137] += I * ce * cimag(path_storage[137 + idx]);
                    path_storage[138 + idx] = path138(in);
                    mom_def_Cp_tr_paths[138] += ce * creal(path_storage[138 + idx]);
                    mom_def_Cm_tr_paths[138] += I * ce * cimag(path_storage[138 + idx]);
                    path_storage[139 + idx] = path139(in);
                    mom_def_Cp_tr_paths[139] += ce * creal(path_storage[139 + idx]);
                    mom_def_Cm_tr_paths[139] += I * ce * cimag(path_storage[139 + idx]);
                    path_storage[140 + idx] = path140(in);
                    mom_def_Cp_tr_paths[140] += ce * creal(path_storage[140 + idx]);
                    mom_def_Cm_tr_paths[140] += I * ce * cimag(path_storage[140 + idx]);
                    path_storage[141 + idx] = path141(in);
                    mom_def_Cp_tr_paths[141] += ce * creal(path_storage[141 + idx]);
                    mom_def_Cm_tr_paths[141] += I * ce * cimag(path_storage[141 + idx]);
                    path_storage[142 + idx] = path142(in);
                    mom_def_Cp_tr_paths[142] += ce * creal(path_storage[142 + idx]);
                    mom_def_Cm_tr_paths[142] += I * ce * cimag(path_storage[142 + idx]);
                    path_storage[143 + idx] = path143(in);
                    mom_def_Cp_tr_paths[143] += ce * creal(path_storage[143 + idx]);
                    mom_def_Cm_tr_paths[143] += I * ce * cimag(path_storage[143 + idx]);
                    path_storage[144 + idx] = path144(in);
                    mom_def_Cp_tr_paths[144] += ce * creal(path_storage[144 + idx]);
                    mom_def_Cm_tr_paths[144] += I * ce * cimag(path_storage[144 + idx]);
                    path_storage[145 + idx] = path145(in);
                    mom_def_Cp_tr_paths[145] += ce * creal(path_storage[145 + idx]);
                    mom_def_Cm_tr_paths[145] += I * ce * cimag(path_storage[145 + idx]);
                    path_storage[146 + idx] = path146(in);
                    mom_def_Cp_tr_paths[146] += ce * creal(path_storage[146 + idx]);
                    mom_def_Cm_tr_paths[146] += I * ce * cimag(path_storage[146 + idx]);
                    path_storage[147 + idx] = path147(in);
                    mom_def_Cp_tr_paths[147] += ce * creal(path_storage[147 + idx]);
                    mom_def_Cm_tr_paths[147] += I * ce * cimag(path_storage[147 + idx]);
                    path_storage[148 + idx] = path148(in);
                    mom_def_Cp_tr_paths[148] += ce * creal(path_storage[148 + idx]);
                    mom_def_Cm_tr_paths[148] += I * ce * cimag(path_storage[148 + idx]);
                    path_storage[149 + idx] = path149(in);
                    mom_def_Cp_tr_paths[149] += ce * creal(path_storage[149 + idx]);
                    mom_def_Cm_tr_paths[149] += I * ce * cimag(path_storage[149 + idx]);
                    path_storage[150 + idx] = path150(in);
                    mom_def_Cp_tr_paths[150] += ce * creal(path_storage[150 + idx]);
                    mom_def_Cm_tr_paths[150] += I * ce * cimag(path_storage[150 + idx]);
                    path_storage[151 + idx] = path151(in);
                    mom_def_Cp_tr_paths[151] += ce * creal(path_storage[151 + idx]);
                    mom_def_Cm_tr_paths[151] += I * ce * cimag(path_storage[151 + idx]);
                    path_storage[152 + idx] = path152(in);
                    mom_def_Cp_tr_paths[152] += ce * creal(path_storage[152 + idx]);
                    mom_def_Cm_tr_paths[152] += I * ce * cimag(path_storage[152 + idx]);
                    path_storage[153 + idx] = path153(in);
                    mom_def_Cp_tr_paths[153] += ce * creal(path_storage[153 + idx]);
                    mom_def_Cm_tr_paths[153] += I * ce * cimag(path_storage[153 + idx]);
                    path_storage[154 + idx] = path154(in);
                    mom_def_Cp_tr_paths[154] += ce * creal(path_storage[154 + idx]);
                    mom_def_Cm_tr_paths[154] += I * ce * cimag(path_storage[154 + idx]);
                    path_storage[155 + idx] = path155(in);
                    mom_def_Cp_tr_paths[155] += ce * creal(path_storage[155 + idx]);
                    mom_def_Cm_tr_paths[155] += I * ce * cimag(path_storage[155 + idx]);
                    path_storage[156 + idx] = path156(in);
                    mom_def_Cp_tr_paths[156] += ce * creal(path_storage[156 + idx]);
                    mom_def_Cm_tr_paths[156] += I * ce * cimag(path_storage[156 + idx]);
                    path_storage[157 + idx] = path157(in);
                    mom_def_Cp_tr_paths[157] += ce * creal(path_storage[157 + idx]);
                    mom_def_Cm_tr_paths[157] += I * ce * cimag(path_storage[157 + idx]);
                    path_storage[158 + idx] = path158(in);
                    mom_def_Cp_tr_paths[158] += ce * creal(path_storage[158 + idx]);
                    mom_def_Cm_tr_paths[158] += I * ce * cimag(path_storage[158 + idx]);
                    path_storage[159 + idx] = path159(in);
                    mom_def_Cp_tr_paths[159] += ce * creal(path_storage[159 + idx]);
                    mom_def_Cm_tr_paths[159] += I * ce * cimag(path_storage[159 + idx]);
                    path_storage[160 + idx] = path160(in);
                    mom_def_Cp_tr_paths[160] += ce * creal(path_storage[160 + idx]);
                    mom_def_Cm_tr_paths[160] += I * ce * cimag(path_storage[160 + idx]);
                    path_storage[161 + idx] = path161(in);
                    mom_def_Cp_tr_paths[161] += ce * creal(path_storage[161 + idx]);
                    mom_def_Cm_tr_paths[161] += I * ce * cimag(path_storage[161 + idx]);
                    path_storage[162 + idx] = path162(in);
                    mom_def_Cp_tr_paths[162] += ce * creal(path_storage[162 + idx]);
                    mom_def_Cm_tr_paths[162] += I * ce * cimag(path_storage[162 + idx]);
                    path_storage[163 + idx] = path163(in);
                    mom_def_Cp_tr_paths[163] += ce * creal(path_storage[163 + idx]);
                    mom_def_Cm_tr_paths[163] += I * ce * cimag(path_storage[163 + idx]);
                    path_storage[164 + idx] = path164(in);
                    mom_def_Cp_tr_paths[164] += ce * creal(path_storage[164 + idx]);
                    mom_def_Cm_tr_paths[164] += I * ce * cimag(path_storage[164 + idx]);
                    path_storage[165 + idx] = path165(in);
                    mom_def_Cp_tr_paths[165] += ce * creal(path_storage[165 + idx]);
                    mom_def_Cm_tr_paths[165] += I * ce * cimag(path_storage[165 + idx]);
                    path_storage[166 + idx] = path166(in);
                    mom_def_Cp_tr_paths[166] += ce * creal(path_storage[166 + idx]);
                    mom_def_Cm_tr_paths[166] += I * ce * cimag(path_storage[166 + idx]);
                    path_storage[167 + idx] = path167(in);
                    mom_def_Cp_tr_paths[167] += ce * creal(path_storage[167 + idx]);
                    mom_def_Cm_tr_paths[167] += I * ce * cimag(path_storage[167 + idx]);
                    path_storage[168 + idx] = path168(in);
                    mom_def_Cp_tr_paths[168] += ce * creal(path_storage[168 + idx]);
                    mom_def_Cm_tr_paths[168] += I * ce * cimag(path_storage[168 + idx]);
                    path_storage[169 + idx] = path169(in);
                    mom_def_Cp_tr_paths[169] += ce * creal(path_storage[169 + idx]);
                    mom_def_Cm_tr_paths[169] += I * ce * cimag(path_storage[169 + idx]);
                    path_storage[170 + idx] = path170(in);
                    mom_def_Cp_tr_paths[170] += ce * creal(path_storage[170 + idx]);
                    mom_def_Cm_tr_paths[170] += I * ce * cimag(path_storage[170 + idx]);
                    path_storage[171 + idx] = path171(in);
                    mom_def_Cp_tr_paths[171] += ce * creal(path_storage[171 + idx]);
                    mom_def_Cm_tr_paths[171] += I * ce * cimag(path_storage[171 + idx]);
                    path_storage[172 + idx] = path172(in);
                    mom_def_Cp_tr_paths[172] += ce * creal(path_storage[172 + idx]);
                    mom_def_Cm_tr_paths[172] += I * ce * cimag(path_storage[172 + idx]);
                    path_storage[173 + idx] = path173(in);
                    mom_def_Cp_tr_paths[173] += ce * creal(path_storage[173 + idx]);
                    mom_def_Cm_tr_paths[173] += I * ce * cimag(path_storage[173 + idx]);
                    path_storage[174 + idx] = path174(in);
                    mom_def_Cp_tr_paths[174] += ce * creal(path_storage[174 + idx]);
                    mom_def_Cm_tr_paths[174] += I * ce * cimag(path_storage[174 + idx]);
                    path_storage[175 + idx] = path175(in);
                    mom_def_Cp_tr_paths[175] += ce * creal(path_storage[175 + idx]);
                    mom_def_Cm_tr_paths[175] += I * ce * cimag(path_storage[175 + idx]);
                    path_storage[176 + idx] = path176(in);
                    mom_def_Cp_tr_paths[176] += ce * creal(path_storage[176 + idx]);
                    mom_def_Cm_tr_paths[176] += I * ce * cimag(path_storage[176 + idx]);
                    path_storage[177 + idx] = path177(in);
                    mom_def_Cp_tr_paths[177] += ce * creal(path_storage[177 + idx]);
                    mom_def_Cm_tr_paths[177] += I * ce * cimag(path_storage[177 + idx]);
                    path_storage[178 + idx] = path178(in);
                    mom_def_Cp_tr_paths[178] += ce * creal(path_storage[178 + idx]);
                    mom_def_Cm_tr_paths[178] += I * ce * cimag(path_storage[178 + idx]);
                    path_storage[179 + idx] = path179(in);
                    mom_def_Cp_tr_paths[179] += ce * creal(path_storage[179 + idx]);
                    mom_def_Cm_tr_paths[179] += I * ce * cimag(path_storage[179 + idx]);
                    path_storage[180 + idx] = path180(in);
                    mom_def_Cp_tr_paths[180] += ce * creal(path_storage[180 + idx]);
                    mom_def_Cm_tr_paths[180] += I * ce * cimag(path_storage[180 + idx]);
                    path_storage[181 + idx] = path181(in);
                    mom_def_Cp_tr_paths[181] += ce * creal(path_storage[181 + idx]);
                    mom_def_Cm_tr_paths[181] += I * ce * cimag(path_storage[181 + idx]);
                    path_storage[182 + idx] = path182(in);
                    mom_def_Cp_tr_paths[182] += ce * creal(path_storage[182 + idx]);
                    mom_def_Cm_tr_paths[182] += I * ce * cimag(path_storage[182 + idx]);
                    path_storage[183 + idx] = path183(in);
                    mom_def_Cp_tr_paths[183] += ce * creal(path_storage[183 + idx]);
                    mom_def_Cm_tr_paths[183] += I * ce * cimag(path_storage[183 + idx]);
                    path_storage[184 + idx] = path184(in);
                    mom_def_Cp_tr_paths[184] += ce * creal(path_storage[184 + idx]);
                    mom_def_Cm_tr_paths[184] += I * ce * cimag(path_storage[184 + idx]);
                    path_storage[185 + idx] = path185(in);
                    mom_def_Cp_tr_paths[185] += ce * creal(path_storage[185 + idx]);
                    mom_def_Cm_tr_paths[185] += I * ce * cimag(path_storage[185 + idx]);
                    path_storage[186 + idx] = path186(in);
                    mom_def_Cp_tr_paths[186] += ce * creal(path_storage[186 + idx]);
                    mom_def_Cm_tr_paths[186] += I * ce * cimag(path_storage[186 + idx]);
                    path_storage[187 + idx] = path187(in);
                    mom_def_Cp_tr_paths[187] += ce * creal(path_storage[187 + idx]);
                    mom_def_Cm_tr_paths[187] += I * ce * cimag(path_storage[187 + idx]);
                    path_storage[188 + idx] = path188(in);
                    mom_def_Cp_tr_paths[188] += ce * creal(path_storage[188 + idx]);
                    mom_def_Cm_tr_paths[188] += I * ce * cimag(path_storage[188 + idx]);
                    path_storage[189 + idx] = path189(in);
                    mom_def_Cp_tr_paths[189] += ce * creal(path_storage[189 + idx]);
                    mom_def_Cm_tr_paths[189] += I * ce * cimag(path_storage[189 + idx]);
                    path_storage[190 + idx] = path190(in);
                    mom_def_Cp_tr_paths[190] += ce * creal(path_storage[190 + idx]);
                    mom_def_Cm_tr_paths[190] += I * ce * cimag(path_storage[190 + idx]);
                    path_storage[191 + idx] = path191(in);
                    mom_def_Cp_tr_paths[191] += ce * creal(path_storage[191 + idx]);
                    mom_def_Cm_tr_paths[191] += I * ce * cimag(path_storage[191 + idx]);
                    path_storage[192 + idx] = path192(in);
                    mom_def_Cp_tr_paths[192] += ce * creal(path_storage[192 + idx]);
                    mom_def_Cm_tr_paths[192] += I * ce * cimag(path_storage[192 + idx]);
                    path_storage[193 + idx] = path193(in);
                    mom_def_Cp_tr_paths[193] += ce * creal(path_storage[193 + idx]);
                    mom_def_Cm_tr_paths[193] += I * ce * cimag(path_storage[193 + idx]);
                    path_storage[194 + idx] = path194(in);
                    mom_def_Cp_tr_paths[194] += ce * creal(path_storage[194 + idx]);
                    mom_def_Cm_tr_paths[194] += I * ce * cimag(path_storage[194 + idx]);
                    path_storage[195 + idx] = path195(in);
                    mom_def_Cp_tr_paths[195] += ce * creal(path_storage[195 + idx]);
                    mom_def_Cm_tr_paths[195] += I * ce * cimag(path_storage[195 + idx]);
                    path_storage[196 + idx] = path196(in);
                    mom_def_Cp_tr_paths[196] += ce * creal(path_storage[196 + idx]);
                    mom_def_Cm_tr_paths[196] += I * ce * cimag(path_storage[196 + idx]);
                    path_storage[197 + idx] = path197(in);
                    mom_def_Cp_tr_paths[197] += ce * creal(path_storage[197 + idx]);
                    mom_def_Cm_tr_paths[197] += I * ce * cimag(path_storage[197 + idx]);
                    path_storage[198 + idx] = path198(in);
                    mom_def_Cp_tr_paths[198] += ce * creal(path_storage[198 + idx]);
                    mom_def_Cm_tr_paths[198] += I * ce * cimag(path_storage[198 + idx]);
                    path_storage[199 + idx] = path199(in);
                    mom_def_Cp_tr_paths[199] += ce * creal(path_storage[199 + idx]);
                    mom_def_Cm_tr_paths[199] += I * ce * cimag(path_storage[199 + idx]);
                    path_storage[200 + idx] = path200(in);
                    mom_def_Cp_tr_paths[200] += ce * creal(path_storage[200 + idx]);
                    mom_def_Cm_tr_paths[200] += I * ce * cimag(path_storage[200 + idx]);
                    path_storage[201 + idx] = path201(in);
                    mom_def_Cp_tr_paths[201] += ce * creal(path_storage[201 + idx]);
                    mom_def_Cm_tr_paths[201] += I * ce * cimag(path_storage[201 + idx]);
                    path_storage[202 + idx] = path202(in);
                    mom_def_Cp_tr_paths[202] += ce * creal(path_storage[202 + idx]);
                    mom_def_Cm_tr_paths[202] += I * ce * cimag(path_storage[202 + idx]);
                    path_storage[203 + idx] = path203(in);
                    mom_def_Cp_tr_paths[203] += ce * creal(path_storage[203 + idx]);
                    mom_def_Cm_tr_paths[203] += I * ce * cimag(path_storage[203 + idx]);
                    path_storage[204 + idx] = path204(in);
                    mom_def_Cp_tr_paths[204] += ce * creal(path_storage[204 + idx]);
                    mom_def_Cm_tr_paths[204] += I * ce * cimag(path_storage[204 + idx]);
                    path_storage[205 + idx] = path205(in);
                    mom_def_Cp_tr_paths[205] += ce * creal(path_storage[205 + idx]);
                    mom_def_Cm_tr_paths[205] += I * ce * cimag(path_storage[205 + idx]);
                    path_storage[206 + idx] = path206(in);
                    mom_def_Cp_tr_paths[206] += ce * creal(path_storage[206 + idx]);
                    mom_def_Cm_tr_paths[206] += I * ce * cimag(path_storage[206 + idx]);
                    path_storage[207 + idx] = path207(in);
                    mom_def_Cp_tr_paths[207] += ce * creal(path_storage[207 + idx]);
                    mom_def_Cm_tr_paths[207] += I * ce * cimag(path_storage[207 + idx]);
                    path_storage[208 + idx] = path208(in);
                    mom_def_Cp_tr_paths[208] += ce * creal(path_storage[208 + idx]);
                    mom_def_Cm_tr_paths[208] += I * ce * cimag(path_storage[208 + idx]);
                    path_storage[209 + idx] = path209(in);
                    mom_def_Cp_tr_paths[209] += ce * creal(path_storage[209 + idx]);
                    mom_def_Cm_tr_paths[209] += I * ce * cimag(path_storage[209 + idx]);
                    path_storage[210 + idx] = path210(in);
                    mom_def_Cp_tr_paths[210] += ce * creal(path_storage[210 + idx]);
                    mom_def_Cm_tr_paths[210] += I * ce * cimag(path_storage[210 + idx]);
                    path_storage[211 + idx] = path211(in);
                    mom_def_Cp_tr_paths[211] += ce * creal(path_storage[211 + idx]);
                    mom_def_Cm_tr_paths[211] += I * ce * cimag(path_storage[211 + idx]);
                    path_storage[212 + idx] = path212(in);
                    mom_def_Cp_tr_paths[212] += ce * creal(path_storage[212 + idx]);
                    mom_def_Cm_tr_paths[212] += I * ce * cimag(path_storage[212 + idx]);
                    path_storage[213 + idx] = path213(in);
                    mom_def_Cp_tr_paths[213] += ce * creal(path_storage[213 + idx]);
                    mom_def_Cm_tr_paths[213] += I * ce * cimag(path_storage[213 + idx]);
                    path_storage[214 + idx] = path214(in);
                    mom_def_Cp_tr_paths[214] += ce * creal(path_storage[214 + idx]);
                    mom_def_Cm_tr_paths[214] += I * ce * cimag(path_storage[214 + idx]);
                    path_storage[215 + idx] = path215(in);
                    mom_def_Cp_tr_paths[215] += ce * creal(path_storage[215 + idx]);
                    mom_def_Cm_tr_paths[215] += I * ce * cimag(path_storage[215 + idx]);
                    path_storage[216 + idx] = path216(in);
                    mom_def_Cp_tr_paths[216] += ce * creal(path_storage[216 + idx]);
                    mom_def_Cm_tr_paths[216] += I * ce * cimag(path_storage[216 + idx]);
                    path_storage[217 + idx] = path217(in);
                    mom_def_Cp_tr_paths[217] += ce * creal(path_storage[217 + idx]);
                    mom_def_Cm_tr_paths[217] += I * ce * cimag(path_storage[217 + idx]);
                    path_storage[218 + idx] = path218(in);
                    mom_def_Cp_tr_paths[218] += ce * creal(path_storage[218 + idx]);
                    mom_def_Cm_tr_paths[218] += I * ce * cimag(path_storage[218 + idx]);
                    path_storage[219 + idx] = path219(in);
                    mom_def_Cp_tr_paths[219] += ce * creal(path_storage[219 + idx]);
                    mom_def_Cm_tr_paths[219] += I * ce * cimag(path_storage[219 + idx]);
                    path_storage[220 + idx] = path220(in);
                    mom_def_Cp_tr_paths[220] += ce * creal(path_storage[220 + idx]);
                    mom_def_Cm_tr_paths[220] += I * ce * cimag(path_storage[220 + idx]);
                    path_storage[221 + idx] = path221(in);
                    mom_def_Cp_tr_paths[221] += ce * creal(path_storage[221 + idx]);
                    mom_def_Cm_tr_paths[221] += I * ce * cimag(path_storage[221 + idx]);
                    path_storage[222 + idx] = path222(in);
                    mom_def_Cp_tr_paths[222] += ce * creal(path_storage[222 + idx]);
                    mom_def_Cm_tr_paths[222] += I * ce * cimag(path_storage[222 + idx]);
                    path_storage[223 + idx] = path223(in);
                    mom_def_Cp_tr_paths[223] += ce * creal(path_storage[223 + idx]);
                    mom_def_Cm_tr_paths[223] += I * ce * cimag(path_storage[223 + idx]);
                    path_storage[224 + idx] = path224(in);
                    mom_def_Cp_tr_paths[224] += ce * creal(path_storage[224 + idx]);
                    mom_def_Cm_tr_paths[224] += I * ce * cimag(path_storage[224 + idx]);
                    path_storage[225 + idx] = path225(in);
                    mom_def_Cp_tr_paths[225] += ce * creal(path_storage[225 + idx]);
                    mom_def_Cm_tr_paths[225] += I * ce * cimag(path_storage[225 + idx]);
                    path_storage[226 + idx] = path226(in);
                    mom_def_Cp_tr_paths[226] += ce * creal(path_storage[226 + idx]);
                    mom_def_Cm_tr_paths[226] += I * ce * cimag(path_storage[226 + idx]);
                    path_storage[227 + idx] = path227(in);
                    mom_def_Cp_tr_paths[227] += ce * creal(path_storage[227 + idx]);
                    mom_def_Cm_tr_paths[227] += I * ce * cimag(path_storage[227 + idx]);
                    path_storage[228 + idx] = path228(in);
                    mom_def_Cp_tr_paths[228] += ce * creal(path_storage[228 + idx]);
                    mom_def_Cm_tr_paths[228] += I * ce * cimag(path_storage[228 + idx]);
                    path_storage[229 + idx] = path229(in);
                    mom_def_Cp_tr_paths[229] += ce * creal(path_storage[229 + idx]);
                    mom_def_Cm_tr_paths[229] += I * ce * cimag(path_storage[229 + idx]);
                    path_storage[230 + idx] = path230(in);
                    mom_def_Cp_tr_paths[230] += ce * creal(path_storage[230 + idx]);
                    mom_def_Cm_tr_paths[230] += I * ce * cimag(path_storage[230 + idx]);
                    path_storage[231 + idx] = path231(in);
                    mom_def_Cp_tr_paths[231] += ce * creal(path_storage[231 + idx]);
                    mom_def_Cm_tr_paths[231] += I * ce * cimag(path_storage[231 + idx]);
                    path_storage[232 + idx] = path232(in);
                    mom_def_Cp_tr_paths[232] += ce * creal(path_storage[232 + idx]);
                    mom_def_Cm_tr_paths[232] += I * ce * cimag(path_storage[232 + idx]);
                    path_storage[233 + idx] = path233(in);
                    mom_def_Cp_tr_paths[233] += ce * creal(path_storage[233 + idx]);
                    mom_def_Cm_tr_paths[233] += I * ce * cimag(path_storage[233 + idx]);
                    path_storage[234 + idx] = path234(in);
                    mom_def_Cp_tr_paths[234] += ce * creal(path_storage[234 + idx]);
                    mom_def_Cm_tr_paths[234] += I * ce * cimag(path_storage[234 + idx]);
                    path_storage[235 + idx] = path235(in);
                    mom_def_Cp_tr_paths[235] += ce * creal(path_storage[235 + idx]);
                    mom_def_Cm_tr_paths[235] += I * ce * cimag(path_storage[235 + idx]);
                    path_storage[236 + idx] = path236(in);
                    mom_def_Cp_tr_paths[236] += ce * creal(path_storage[236 + idx]);
                    mom_def_Cm_tr_paths[236] += I * ce * cimag(path_storage[236 + idx]);
                    path_storage[237 + idx] = path237(in);
                    mom_def_Cp_tr_paths[237] += ce * creal(path_storage[237 + idx]);
                    mom_def_Cm_tr_paths[237] += I * ce * cimag(path_storage[237 + idx]);
                    path_storage[238 + idx] = path238(in);
                    mom_def_Cp_tr_paths[238] += ce * creal(path_storage[238 + idx]);
                    mom_def_Cm_tr_paths[238] += I * ce * cimag(path_storage[238 + idx]);
                    path_storage[239 + idx] = path239(in);
                    mom_def_Cp_tr_paths[239] += ce * creal(path_storage[239 + idx]);
                    mom_def_Cm_tr_paths[239] += I * ce * cimag(path_storage[239 + idx]);
                    path_storage[240 + idx] = path240(in);
                    mom_def_Cp_tr_paths[240] += ce * creal(path_storage[240 + idx]);
                    mom_def_Cm_tr_paths[240] += I * ce * cimag(path_storage[240 + idx]);
                }
            }
        }
    } else {
        for (nnx = 0; nnx < X; nnx++) {
            for (nny = 0; nny < Y; nny++) {
                for (nnz = 0; nnz < Z; nnz++) {
                    ce = cexp(I * 2.0 * PI * (double)(nnx * px + nny * py + nnz * pz) / GLB_X);
                    idx = npaths * (nnx + X * (nny + Y * nnz));
                    for (int i = 0; i < 241; i++) {
                        mom_def_Cp_tr_paths[i] += ce * creal(path_storage[i + idx]);
                        mom_def_Cm_tr_paths[i] += I * ce * cimag(path_storage[i + idx]);
                    }
                }
            }
        }
    }
};
void eval_all_glueball_ops(int t, hr_complex *numerical_op_out) {
    static hr_complex *numerical_op = NULL;
    if (numerical_op == NULL) { numerical_op = malloc(total_n_glue_op * sizeof(hr_complex)); }
    request_space_paths_evaluation();
    eval_time_momentum_glueball_paths(t, -1, 0, 0);
    OP_oneTr_p_m1_0_0_Ir_1_C_1_n_1(numerical_op + 0);
    OP_oneTr_p_m1_0_0_Ir_1_C_1_n_2(numerical_op + 1);
    eval_time_momentum_glueball_paths(t, 0, -1, 0);
    OP_oneTr_p_0_m1_0_Ir_1_C_1_n_1(numerical_op + 2);
    eval_time_momentum_glueball_paths(t, 0, 0, -1);
    OP_oneTr_p_0_0_m1_Ir_1_C_1_n_1(numerical_op + 3);
    eval_time_momentum_glueball_paths(t, 0, 0, 0);
    OP_oneTr_p_0_0_0_Ir_1_C_1_n_1(numerical_op + 4);
    OP_oneTr_p_0_0_0_Ir_1_C_1_n_2(numerical_op + 5);
    OP_oneTr_p_0_0_0_Ir_1_C_1_n_3(numerical_op + 6);
    OP_oneTr_p_0_0_0_Ir_1_C_1_n_4(numerical_op + 7);
    OP_oneTr_p_0_0_0_Ir_1_C_1_n_5(numerical_op + 8);
    OP_oneTr_p_0_0_0_Ir_1_C_1_n_6(numerical_op + 9);
    OP_oneTr_p_0_0_0_Ir_1_C_1_n_7(numerical_op + 10);
    OP_oneTr_p_0_0_0_Ir_1_C_1_n_8(numerical_op + 11);
    OP_oneTr_p_0_0_0_Ir_3_C_1_n_1(numerical_op + 13);
    OP_oneTr_p_0_0_0_Ir_3_C_1_n_3(numerical_op + 14);
    OP_oneTr_p_0_0_0_Ir_3_C_1_n_5(numerical_op + 15);
    OP_oneTr_p_0_0_0_Ir_3_C_1_n_7(numerical_op + 16);
    OP_oneTr_p_0_0_0_Ir_3_C_1_n_2(numerical_op + 18);
    OP_oneTr_p_0_0_0_Ir_3_C_1_n_4(numerical_op + 19);
    OP_oneTr_p_0_0_0_Ir_3_C_1_n_6(numerical_op + 20);
    OP_oneTr_p_0_0_0_Ir_3_C_1_n_8(numerical_op + 21);
    OP_oneTr_p_0_0_0_Ir_5_C_1_n_1(numerical_op + 23);
    OP_oneTr_p_0_0_0_Ir_5_C_1_n_4(numerical_op + 24);
    OP_oneTr_p_0_0_0_Ir_5_C_1_n_7(numerical_op + 25);
    OP_oneTr_p_0_0_0_Ir_5_C_1_n_10(numerical_op + 26);
    OP_oneTr_p_0_0_0_Ir_5_C_1_n_2(numerical_op + 27);
    OP_oneTr_p_0_0_0_Ir_5_C_1_n_5(numerical_op + 28);
    OP_oneTr_p_0_0_0_Ir_5_C_1_n_8(numerical_op + 29);
    OP_oneTr_p_0_0_0_Ir_5_C_1_n_11(numerical_op + 30);
    OP_oneTr_p_0_0_0_Ir_5_C_1_n_3(numerical_op + 31);
    OP_oneTr_p_0_0_0_Ir_5_C_1_n_6(numerical_op + 32);
    OP_oneTr_p_0_0_0_Ir_5_C_1_n_9(numerical_op + 33);
    OP_oneTr_p_0_0_0_Ir_5_C_1_n_12(numerical_op + 34);
    eval_time_momentum_glueball_paths(t, 0, 0, 1);
    OP_oneTr_p_0_0_1_Ir_1_C_1_n_1(numerical_op + 35);
    eval_time_momentum_glueball_paths(t, 0, 1, 0);
    OP_oneTr_p_0_1_0_Ir_1_C_1_n_1(numerical_op + 36);
    eval_time_momentum_glueball_paths(t, 1, 0, 0);
    OP_oneTr_p_1_0_0_Ir_1_C_1_n_1(numerical_op + 37);
    *(numerical_op + 12) = +(0.408248290463863016) * ((*(numerical_op + 1))) * ((*(numerical_op + 37))) +
                           (0.408248290463863016) * ((*(numerical_op + 2))) * ((*(numerical_op + 36))) +
                           (0.408248290463863016) * ((*(numerical_op + 3))) * ((*(numerical_op + 35))) +
                           (0.408248290463863016) * ((*(numerical_op + 35))) * ((*(numerical_op + 3))) +
                           (0.408248290463863016) * ((*(numerical_op + 36))) * ((*(numerical_op + 2))) +
                           (0.408248290463863016) * ((*(numerical_op + 37))) * ((*(numerical_op + 1)));
    *(numerical_op + 17) = +(-0.288675134594812882) * ((*(numerical_op + 1))) * ((*(numerical_op + 37))) +
                           (-0.288675134594812882) * ((*(numerical_op + 2))) * ((*(numerical_op + 36))) +
                           (-0.288675134594812882) * ((*(numerical_op + 36))) * ((*(numerical_op + 2))) +
                           (-0.288675134594812882) * ((*(numerical_op + 37))) * ((*(numerical_op + 1))) +
                           (0.577350269189625765) * ((*(numerical_op + 3))) * ((*(numerical_op + 35))) +
                           (0.577350269189625765) * ((*(numerical_op + 35))) * ((*(numerical_op + 3)));
    *(numerical_op + 22) = +(-0.5) * ((*(numerical_op + 2))) * ((*(numerical_op + 36))) +
                           (-0.5) * ((*(numerical_op + 36))) * ((*(numerical_op + 2))) +
                           (0.5) * ((*(numerical_op + 1))) * ((*(numerical_op + 37))) +
                           (0.5) * ((*(numerical_op + 37))) * ((*(numerical_op + 1)));
    for (int i = 0; i < total_n_glue_op; i++) {
        *(numerical_op_out + i) += *(numerical_op + i);
    }
}

void collect_1pt_glueball_functions(cor_list *lcor, int nblocking, hr_complex *gb_storage, int logreport) {
#if total_n_glue_op > 0
    int n1, n2;
#endif
    int i;
    static hr_complex *gb1_bf;
    static int n_total_active_slices = 0;
    static int *listactive = NULL;
    if (listactive == NULL) {
        listactive = malloc(sizeof(int) * GLB_T);
        for (i = 0; i < GLB_T; i++) {
            listactive[i] = -1;
        }

        for (i = 0; i < lcor->n_entries; i++) {
            listactive[lcor->list[i].t1] = 1;
            listactive[lcor->list[i].t2] = 1;
        }

        for (i = 0; i < GLB_T; i++) {
            if (listactive[i] == 1) {
                listactive[i] = n_total_active_slices;
                n_total_active_slices++;
            }
        }
    }

#ifdef WITH_MPI
    static int *t_to_proc = NULL;
    static int *listsent = NULL;
    int t1, t2;

    if (t_to_proc == NULL) {
        listsent = malloc(sizeof(int) * GLB_T);

        gb1_bf = malloc(sizeof(hr_complex) * total_n_glue_op * nblocking * n_total_active_slices);

        t_to_proc = malloc(sizeof(int) * GLB_T);
        for (i = 0; i < GLB_T; i++) {
            int x[4] = { i, 0, 0, 0 }, p[4];
            glb_to_proc(x, p);
            t_to_proc[i] = p[0];
        }
    }

    for (i = 0; i < GLB_T; i++) {
        listsent[i] = -1;
    }

    static hr_complex *gb2;
    static hr_complex *gb1;
    MPI_Request req_1pt[GLB_T];

    for (int icor = 0; icor < lcor->n_entries; icor++) {
        t1 = glbT_to_active_slices[lcor->list[icor].t1];
        t2 = glbT_to_active_slices[lcor->list[icor].t2];
        gb1 = gb1_bf + total_n_glue_op * nblocking * listactive[lcor->list[icor].t1];
        gb2 = gb1_bf + total_n_glue_op * nblocking * listactive[lcor->list[icor].t2];

        if (listsent[lcor->list[icor].t1] == -1) {
            if (t1 != -1) {
                listsent[lcor->list[icor].t1] = 0;
                if (PID == 0) {
                    memcpy(gb1, gb_storage + t1 * total_n_glue_op * nblocking,
                           sizeof(hr_complex) * total_n_glue_op * nblocking);
                } else {
                    MPI_Isend((double *)(gb_storage + t1 * total_n_glue_op * nblocking), total_n_glue_op * nblocking * 2,
                              MPI_DOUBLE, 0, lcor->list[icor].t1, cart_comm, req_1pt + lcor->list[icor].t1);
                }
            }

            if (PID == 0 && t1 == -1) {
                MPI_Irecv((double *)(gb1), total_n_glue_op * nblocking * 2, MPI_DOUBLE, t_to_proc[lcor->list[icor].t1],
                          lcor->list[icor].t1, cart_comm, req_1pt + lcor->list[icor].t1);
                listsent[lcor->list[icor].t1] = 1;
            }
        }

        if (listsent[lcor->list[icor].t2] == -1) {
            listsent[lcor->list[icor].t2] = 0;
            if (t2 != -1) {
                if (PID == 0) {
                    memcpy(gb2, gb_storage + t2 * total_n_glue_op * nblocking,
                           sizeof(hr_complex) * total_n_glue_op * nblocking);
                } else {
                    MPI_Isend((double *)(gb_storage + t2 * total_n_glue_op * nblocking), total_n_glue_op * nblocking * 2,
                              MPI_DOUBLE, 0, lcor->list[icor].t2, cart_comm, req_1pt + lcor->list[icor].t2);
                }
            }

            if (PID == 0 && t2 == -1) {
                MPI_Irecv((double *)(gb2), total_n_glue_op * nblocking * 2, MPI_DOUBLE, t_to_proc[lcor->list[icor].t2],
                          lcor->list[icor].t2, cart_comm, req_1pt + lcor->list[icor].t2);
                listsent[lcor->list[icor].t2] = 1;
            }
        }
    }
    if (PID == 0) {
        for (i = 0; i < GLB_T; i++) {
            if (listsent[i] == 1) { MPI_Wait(req_1pt + i, MPI_STATUS_IGNORE); }
        }
    }
    MPI_Barrier(GLB_COMM);
#else
    gb1_bf = gb_storage;
#endif

    if (logreport == 0) {
        lprintf("Measure ML", 0, "\n1pt function P=(-1,0,0) Irrep=A1Dic4 Irrep ev=1/1 Charge=+ nop=%d\n", 2 * nblocking);
        lprintf("Measure ML", 0, "Op id= 0 1 (repeated nblocking times)\n");
        for (n1 = 0; n1 < GLB_T; n1++) {
            if (listactive[n1] > -1) {
                lprintf("Measure ML", 0, " t=%d", n1);
                for (n2 = 0; n2 < nblocking; n2++) {
                    for (i = 0; i < 2; i++) {
                        lprintf("Measure ML", 0, " ( %.10e %.10e )",
                                creal(gb1_bf[i + total_n_glue_op * (n2 + nblocking * listactive[n1])]),
                                cimag(gb1_bf[i + total_n_glue_op * (n2 + nblocking * listactive[n1])]));
                    }
                }
                lprintf("Measure ML", 0, "\n");
            }
        }

        lprintf("Measure ML", 0, "\n1pt function P=(0,-1,0) Irrep=A1Dic4 Irrep ev=1/1 Charge=+ nop=%d\n", 1 * nblocking);
        lprintf("Measure ML", 0, "Op id= 2 (repeated nblocking times)\n");
        for (n1 = 0; n1 < GLB_T; n1++) {
            if (listactive[n1] > -1) {
                lprintf("Measure ML", 0, " t=%d", n1);
                for (n2 = 0; n2 < nblocking; n2++) {
                    for (i = 2; i < 3; i++) {
                        lprintf("Measure ML", 0, " ( %.10e %.10e )",
                                creal(gb1_bf[i + total_n_glue_op * (n2 + nblocking * listactive[n1])]),
                                cimag(gb1_bf[i + total_n_glue_op * (n2 + nblocking * listactive[n1])]));
                    }
                }
                lprintf("Measure ML", 0, "\n");
            }
        }

        lprintf("Measure ML", 0, "\n1pt function P=(0,0,-1) Irrep=A1Dic4 Irrep ev=1/1 Charge=+ nop=%d\n", 1 * nblocking);
        lprintf("Measure ML", 0, "Op id= 3 (repeated nblocking times)\n");
        for (n1 = 0; n1 < GLB_T; n1++) {
            if (listactive[n1] > -1) {
                lprintf("Measure ML", 0, " t=%d", n1);
                for (n2 = 0; n2 < nblocking; n2++) {
                    for (i = 3; i < 4; i++) {
                        lprintf("Measure ML", 0, " ( %.10e %.10e )",
                                creal(gb1_bf[i + total_n_glue_op * (n2 + nblocking * listactive[n1])]),
                                cimag(gb1_bf[i + total_n_glue_op * (n2 + nblocking * listactive[n1])]));
                    }
                }
                lprintf("Measure ML", 0, "\n");
            }
        }

        lprintf("Measure ML", 0, "\n1pt function P=(0,0,0) Irrep=A1plusOhP Irrep ev=1/1 Charge=+ nop=%d\n", 9 * nblocking);
        lprintf("Measure ML", 0, "Op id= 4 5 6 7 8 9 10 11 12 (repeated nblocking times)\n");
        for (n1 = 0; n1 < GLB_T; n1++) {
            if (listactive[n1] > -1) {
                lprintf("Measure ML", 0, " t=%d", n1);
                for (n2 = 0; n2 < nblocking; n2++) {
                    for (i = 4; i < 13; i++) {
                        lprintf("Measure ML", 0, " ( %.10e %.10e )",
                                creal(gb1_bf[i + total_n_glue_op * (n2 + nblocking * listactive[n1])]),
                                cimag(gb1_bf[i + total_n_glue_op * (n2 + nblocking * listactive[n1])]));
                    }
                }
                lprintf("Measure ML", 0, "\n");
            }
        }

        lprintf("Measure ML", 0, "\n1pt function P=(0,0,0) Irrep=EplusOhP Irrep ev=1/2 Charge=+ nop=%d\n", 5 * nblocking);
        lprintf("Measure ML", 0, "Op id= 13 14 15 16 17 (repeated nblocking times)\n");
        for (n1 = 0; n1 < GLB_T; n1++) {
            if (listactive[n1] > -1) {
                lprintf("Measure ML", 0, " t=%d", n1);
                for (n2 = 0; n2 < nblocking; n2++) {
                    for (i = 13; i < 18; i++) {
                        lprintf("Measure ML", 0, " ( %.10e %.10e )",
                                creal(gb1_bf[i + total_n_glue_op * (n2 + nblocking * listactive[n1])]),
                                cimag(gb1_bf[i + total_n_glue_op * (n2 + nblocking * listactive[n1])]));
                    }
                }
                lprintf("Measure ML", 0, "\n");
            }
        }

        lprintf("Measure ML", 0, "\n1pt function P=(0,0,0) Irrep=EplusOhP Irrep ev=2/2 Charge=+ nop=%d\n", 5 * nblocking);
        lprintf("Measure ML", 0, "Op id= 18 19 20 21 22 (repeated nblocking times)\n");
        for (n1 = 0; n1 < GLB_T; n1++) {
            if (listactive[n1] > -1) {
                lprintf("Measure ML", 0, " t=%d", n1);
                for (n2 = 0; n2 < nblocking; n2++) {
                    for (i = 18; i < 23; i++) {
                        lprintf("Measure ML", 0, " ( %.10e %.10e )",
                                creal(gb1_bf[i + total_n_glue_op * (n2 + nblocking * listactive[n1])]),
                                cimag(gb1_bf[i + total_n_glue_op * (n2 + nblocking * listactive[n1])]));
                    }
                }
                lprintf("Measure ML", 0, "\n");
            }
        }

        lprintf("Measure ML", 0, "\n1pt function P=(0,0,0) Irrep=T2plusOhP Irrep ev=1/3 Charge=+ nop=%d\n", 4 * nblocking);
        lprintf("Measure ML", 0, "Op id= 23 24 25 26 (repeated nblocking times)\n");
        for (n1 = 0; n1 < GLB_T; n1++) {
            if (listactive[n1] > -1) {
                lprintf("Measure ML", 0, " t=%d", n1);
                for (n2 = 0; n2 < nblocking; n2++) {
                    for (i = 23; i < 27; i++) {
                        lprintf("Measure ML", 0, " ( %.10e %.10e )",
                                creal(gb1_bf[i + total_n_glue_op * (n2 + nblocking * listactive[n1])]),
                                cimag(gb1_bf[i + total_n_glue_op * (n2 + nblocking * listactive[n1])]));
                    }
                }
                lprintf("Measure ML", 0, "\n");
            }
        }

        lprintf("Measure ML", 0, "\n1pt function P=(0,0,0) Irrep=T2plusOhP Irrep ev=2/3 Charge=+ nop=%d\n", 4 * nblocking);
        lprintf("Measure ML", 0, "Op id= 27 28 29 30 (repeated nblocking times)\n");
        for (n1 = 0; n1 < GLB_T; n1++) {
            if (listactive[n1] > -1) {
                lprintf("Measure ML", 0, " t=%d", n1);
                for (n2 = 0; n2 < nblocking; n2++) {
                    for (i = 27; i < 31; i++) {
                        lprintf("Measure ML", 0, " ( %.10e %.10e )",
                                creal(gb1_bf[i + total_n_glue_op * (n2 + nblocking * listactive[n1])]),
                                cimag(gb1_bf[i + total_n_glue_op * (n2 + nblocking * listactive[n1])]));
                    }
                }
                lprintf("Measure ML", 0, "\n");
            }
        }

        lprintf("Measure ML", 0, "\n1pt function P=(0,0,0) Irrep=T2plusOhP Irrep ev=3/3 Charge=+ nop=%d\n", 4 * nblocking);
        lprintf("Measure ML", 0, "Op id= 31 32 33 34 (repeated nblocking times)\n");
        for (n1 = 0; n1 < GLB_T; n1++) {
            if (listactive[n1] > -1) {
                lprintf("Measure ML", 0, " t=%d", n1);
                for (n2 = 0; n2 < nblocking; n2++) {
                    for (i = 31; i < 35; i++) {
                        lprintf("Measure ML", 0, " ( %.10e %.10e )",
                                creal(gb1_bf[i + total_n_glue_op * (n2 + nblocking * listactive[n1])]),
                                cimag(gb1_bf[i + total_n_glue_op * (n2 + nblocking * listactive[n1])]));
                    }
                }
                lprintf("Measure ML", 0, "\n");
            }
        }

        lprintf("Measure ML", 0, "\n1pt function P=(0,0,1) Irrep=A1Dic4 Irrep ev=1/1 Charge=+ nop=%d\n", 1 * nblocking);
        lprintf("Measure ML", 0, "Op id= 35 (repeated nblocking times)\n");
        for (n1 = 0; n1 < GLB_T; n1++) {
            if (listactive[n1] > -1) {
                lprintf("Measure ML", 0, " t=%d", n1);
                for (n2 = 0; n2 < nblocking; n2++) {
                    for (i = 35; i < 36; i++) {
                        lprintf("Measure ML", 0, " ( %.10e %.10e )",
                                creal(gb1_bf[i + total_n_glue_op * (n2 + nblocking * listactive[n1])]),
                                cimag(gb1_bf[i + total_n_glue_op * (n2 + nblocking * listactive[n1])]));
                    }
                }
                lprintf("Measure ML", 0, "\n");
            }
        }

        lprintf("Measure ML", 0, "\n1pt function P=(0,1,0) Irrep=A1Dic4 Irrep ev=1/1 Charge=+ nop=%d\n", 1 * nblocking);
        lprintf("Measure ML", 0, "Op id= 36 (repeated nblocking times)\n");
        for (n1 = 0; n1 < GLB_T; n1++) {
            if (listactive[n1] > -1) {
                lprintf("Measure ML", 0, " t=%d", n1);
                for (n2 = 0; n2 < nblocking; n2++) {
                    for (i = 36; i < 37; i++) {
                        lprintf("Measure ML", 0, " ( %.10e %.10e )",
                                creal(gb1_bf[i + total_n_glue_op * (n2 + nblocking * listactive[n1])]),
                                cimag(gb1_bf[i + total_n_glue_op * (n2 + nblocking * listactive[n1])]));
                    }
                }
                lprintf("Measure ML", 0, "\n");
            }
        }

        lprintf("Measure ML", 0, "\n1pt function P=(1,0,0) Irrep=A1Dic4 Irrep ev=1/1 Charge=+ nop=%d\n", 1 * nblocking);
        lprintf("Measure ML", 0, "Op id= 37 (repeated nblocking times)\n");
        for (n1 = 0; n1 < GLB_T; n1++) {
            if (listactive[n1] > -1) {
                lprintf("Measure ML", 0, " t=%d", n1);
                for (n2 = 0; n2 < nblocking; n2++) {
                    for (i = 37; i < 38; i++) {
                        lprintf("Measure ML", 0, " ( %.10e %.10e )",
                                creal(gb1_bf[i + total_n_glue_op * (n2 + nblocking * listactive[n1])]),
                                cimag(gb1_bf[i + total_n_glue_op * (n2 + nblocking * listactive[n1])]));
                    }
                }
                lprintf("Measure ML", 0, "\n");
            }
        }
    }
}
void report_gb_group_setup() {
    lprintf("INIT Measure ML", 0, "\n1pt_glb Irrep multiplets Total P=(-1,0,0) Irrep=A1Dic4 Charge=+");
    lprintf("INIT Measure ML", 0, " |0=xy-x-y|1=yz-y-z|");
    lprintf("INIT Measure ML", 0, "\n1pt_glb Irrep multiplets Total P=(0,-1,0) Irrep=A1Dic4 Charge=+");
    lprintf("INIT Measure ML", 0, " |2=zx-z-x|");
    lprintf("INIT Measure ML", 0, "\n1pt_glb Irrep multiplets Total P=(0,0,-1) Irrep=A1Dic4 Charge=+");
    lprintf("INIT Measure ML", 0, " |3=xy-x-y|");
    lprintf("INIT Measure ML", 0, "\n1pt_glb Irrep multiplets Total P=(0,0,0) Irrep=A1plusOhP Charge=+");
    lprintf(
        "INIT Measure ML", 0,
        " |4=xy-x-y|5=x-yz-xy-z|6=xx-y-x-xzy-z|7=xx-y-xz-xy-z|8=xy-x-y-xzx-z|9=xxx-y-x-x-xzy-z|10=xxx-y-x-xzy-x-z|11=xxxy-x-xy-x-y-y|12=000_A1plusOhP+_1_001_A1Dic4+_xy-x-y_001_A1Dic4+_xy-x-y|");
    lprintf("INIT Measure ML", 0, "\n1pt_glb Irrep multiplets Total P=(0,0,0) Irrep=EplusOhP Charge=+");
    lprintf(
        "INIT Measure ML", 0,
        " |13=xy-x-y,18=yz-y-z|14=xxy-x-x-y,19=xxy-x-x-y|15=xy-xz-y-z,20=xy-xz-y-z|16=xx-y-x-xzy-z,21=xx-y-x-xzy-z|17=000_EplusOhP+_1_001_A1Dic4+_xy-x-y_001_A1Dic4+_xy-x-y,22=000_EplusOhP+_2_001_A1Dic4+_xy-x-y_001_A1Dic4+_xy-x-y|");
    lprintf("INIT Measure ML", 0, "\n1pt_glb Irrep multiplets Total P=(0,0,0) Irrep=T2plusOhP Charge=+");
    lprintf(
        "INIT Measure ML", 0,
        " |23=x-yz-xy-z,27=x-yz-xy-z,31=x-yz-xy-z|24=xy-xz-y-z,28=xyz-y-x-z,32=xy-xz-y-z|25=xx-y-x-xzy-z,29=xx-yzy-x-x-z,33=xx-y-x-xzy-z|26=xx-y-xyz-x-z,30=xz-x-yzy-z-z,34=xx-y-xyz-x-z|");
    lprintf("INIT Measure ML", 0, "\n1pt_glb Irrep multiplets Total P=(0,0,1) Irrep=A1Dic4 Charge=+");
    lprintf("INIT Measure ML", 0, " |35=xy-x-y|");
    lprintf("INIT Measure ML", 0, "\n1pt_glb Irrep multiplets Total P=(0,1,0) Irrep=A1Dic4 Charge=+");
    lprintf("INIT Measure ML", 0, " |36=zx-z-x|");
    lprintf("INIT Measure ML", 0, "\n1pt_glb Irrep multiplets Total P=(1,0,0) Irrep=A1Dic4 Charge=+");
    lprintf("INIT Measure ML", 0, " |37=yz-y-z|");
}
