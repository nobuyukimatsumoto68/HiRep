#!/usr/bin/env python3
"""
Glueball 2-point correlators and effective masses from the 1-point rows ([step] lines) that
PureGauge/hmc_glueballs_grid_claude writes to out_*_0, with binned jackknife errors.

Methods:
  jackknife      : M. H. Quenouille, Biometrika 43 (1956) 353;
                   J. W. Tukey, Ann. Math. Statist. 29 (1958) 614.
  binning        : H. Flyvbjerg and H. G. Petersen, J. Chem. Phys. 91 (1989) 461.
  (estimators written out in glueball_mass_test_impl_plan_claude.md)

Row layout: "[step][0]<t>  (Re +I*(Im))  ...", total_n_glue_op * nblocking complex numbers per
row, ordered [level][op] (level 0 = raw links). One row per timeslice; all T timeslices must be
present, i.e. run with NP_T = 1 (the driver prints rank 0's slices only).

Correlator per operator O, time-averaged and connected (vev re-subtracted in every jackknife sample):
  C(dt) = (1/T) sum_t < Re[ O^*(t+dt) O(t) ] > - |< O >|^2
Effective mass m_eff(t) = ln[ C(t) / C(t+1) ] (fine for t << T/2: the backward-propagating term is
negligible there). Multiplet components / equivalent momenta are averaged at the correlator level.

Usage:
  python3 analyze_glueball_1pt_claude.py OUTFILE [--T 16] [--nlev 3] [--ntherm 200] [--bin 20]
                                         [--tmax 4] [--binscan] [--save PREFIX]
"""
import argparse
import re
import sys

import numpy as np

# Channel -> operator indices (glueballs_op.c report_gb_group_setup / eval_all_glueball_ops), one map
# per generated operator set, selected by total_n_glue_op (inferred from the row length).
# "2gb" = products O_{-e_i} O_{+e_i} (two-glueball operators), not single-trace.
CHANNEL_MAPS = {
    # committed CI ("github") set, total_n_glue_op = 15
    15: {
        "A1_P1": [0, 2, 3, 11, 12, 14],  # single-trace A1 (D4), |P| = 2 pi / L, first operator
        "A1_P1_n2": [1],                 # second A1 operator at P = (-1,0,0)
        "A2_P1": [13],                   # A2 (D4), P = (0,1,0)
        "A2p_P0": [5],                   # A2+, P = 0, single trace
        # "T1pm_P0": [8, 9, 10],         # T1+-, P = 0: C = - takes Im tr, identically 0 for SU(2)
        "A1pp_2gb_P0": [4],              # A1+, P = 0, two-glueball product
        "Epp_2gb_P0": [6, 7],            # E+, P = 0, two-glueball products
    },
    # su2rest set (glueball_gen_claude), total_n_glue_op = 38; one channel per loop shape,
    # multiplet components averaged. Shapes res[k] of the generator notebook.
    38: {
        "A1_P1": [0, 2, 3, 35, 36, 37],  # single-trace A1 (D4), |P| = 2 pi / L, plaquette
        "A1_P1_n2": [1],                 # second plaquette orientation at P = (-1,0,0)
        "A1pp_r1": [4],                  # A1++ at rest, res[1] plaquette
        "A1pp_r3": [5],                  # res[3]  6-link
        "A1pp_r5": [6],                  # res[5]  8-link
        "A1pp_r8": [7],                  # res[8]  8-link
        "A1pp_r11": [8],                 # res[11] 8-link
        "A1pp_r12": [9],                 # res[12] 10-link
        "A1pp_r16": [10],                # res[16] 10-link
        "A1pp_r20": [11],                # res[20] 10-link
        "A1pp_2gb_P0": [12],
        "Epp_r1": [13, 18],              # E++ at rest, components 1, 2; res[1] plaquette
        "Epp_r2": [14, 19],              # res[2] 2x1 rectangle
        "Epp_r4": [15, 20],              # res[4] 6-link
        "Epp_r5": [16, 21],              # res[5] 8-link
        "Epp_2gb_P0": [17, 22],
        "T2pp_r3": [23, 27, 31],         # T2++ at rest, components 1-3; res[3] 6-link
        "T2pp_r4": [24, 28, 32],         # res[4] 6-link
        "T2pp_r5": [25, 29, 33],         # res[5] 8-link
        "T2pp_r6": [26, 30, 34],         # res[6] 8-link
    },
}

NUM = re.compile(r"\(\s*(\S+) \+I\*\((\S+)\)\)")


def read_rows(path, T, nlev):
    """Return O[cfg, t, level, op] (complex) from the [step] rows of a HiRep out file.

    The number of operators per level (total_n_glue_op) is inferred from the first row.
    """
    nop = None
    rows = []
    tcol = []
    for line in open(path):
        if not line.startswith("[step][0]"):
            continue
        t = int(line[len("[step][0]"):].split()[0])
        vals = NUM.findall(line)
        if nop is None:
            if len(vals) % nlev != 0:
                sys.exit("row has %d ops, not a multiple of nlev = %d (check --nlev)" % (len(vals), nlev))
            nop = len(vals) // nlev
        if len(vals) != nlev * nop:
            sys.exit("row has %d ops, expected %d (inconsistent rows)" % (len(vals), nlev * nop))
        tcol.append(t)
        rows.append([complex(float(a), float(b)) for a, b in vals])
    if len(rows) % T != 0:
        sys.exit("%d rows is not a multiple of T = %d (were all timeslices printed? NP_T must be 1)" % (len(rows), T))
    tcol = np.array(tcol).reshape(-1, T)
    if not np.all(tcol == np.arange(T)[None, :]):
        sys.exit("timeslice column is not 0..T-1 per measurement (NP_T > 1 or truncated file?)")
    return np.array(rows).reshape(-1, T, nlev, nop)


def per_config_sums(X, tmax):
    """For X[cfg, t] (one operator): per-config time-averaged product P[cfg, dt] and mean M[cfg]."""
    ncfg, T = X.shape
    P = np.zeros((ncfg, tmax + 2))
    for dt in range(tmax + 2):
        P[:, dt] = np.mean((np.conj(np.roll(X, -dt, axis=1)) * X).real, axis=1)
    M = X.mean(axis=1)
    return P, M


def jackknife_channel(O, lev, ops, nbin_size, tmax):
    """Binned jackknife for one channel at one blocking level.

    Returns (C, dC, R, dR, meff, dmeff, nbin) with R = C(t)/C(0).
    """
    ncfg = O.shape[0]
    nbin = ncfg // nbin_size
    ncut = nbin * nbin_size
    # bin the per-config pieces of every operator in the channel
    Pb = []
    Mb = []
    for op in ops:
        P, M = per_config_sums(O[:ncut, :, lev, op], tmax)
        Pb.append(P.reshape(nbin, nbin_size, -1).mean(axis=1))
        Mb.append(M.reshape(nbin, nbin_size).mean(axis=1))
    Pb = np.array(Pb)  # [op, bin, dt]
    Mb = np.array(Mb)  # [op, bin]
    # jackknife samples: leave one bin out, rebuild the connected correlator (vev inside the sample)
    Pjk = (Pb.sum(axis=1, keepdims=True) - Pb) / (nbin - 1)
    Mjk = (Mb.sum(axis=1, keepdims=True) - Mb) / (nbin - 1)
    Cjk = (Pjk - np.abs(Mjk)[:, :, None] ** 2).mean(axis=0)  # average over ops -> [bin, dt]
    Cfull = (Pb.mean(axis=1) - np.abs(Mb.mean(axis=1))[:, None] ** 2).mean(axis=0)
    with np.errstate(divide="ignore", invalid="ignore"):
        Rjk = Cjk / Cjk[:, :1]
        R = Cfull / Cfull[0]
        mjk = np.log(Cjk[:, :-1] / Cjk[:, 1:])
        meff = np.log(Cfull[:-1] / Cfull[1:])
    fac = (nbin - 1) / nbin
    dC = np.sqrt(fac * ((Cjk - Cjk.mean(axis=0)) ** 2).sum(axis=0))
    dR = np.sqrt(fac * ((Rjk - Rjk.mean(axis=0)) ** 2).sum(axis=0))
    dm = np.sqrt(fac * ((mjk - np.nanmean(mjk, axis=0)) ** 2).sum(axis=0))
    return Cfull, dC, R, dR, meff, dm, nbin


def fmt(v, e):
    if not np.isfinite(v) or not np.isfinite(e):
        return "      ---      "
    return "%8.4f(%5.4f)" % (v, e)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("outfile")
    ap.add_argument("--T", type=int, default=16)
    ap.add_argument("--nlev", type=int, default=3, help="nblkend - nblkstart + 1")
    ap.add_argument("--ntherm", type=int, default=200, help="measurements dropped at the start")
    ap.add_argument("--bin", type=int, default=20, help="bin size in measurements")
    ap.add_argument("--tmax", type=int, default=4)
    ap.add_argument("--binscan", action="store_true", help="error of m_eff(1) vs bin size, A1_P1 best level")
    ap.add_argument("--save", default=None, help="write PREFIX_<channel>_lev<k>.txt tables")
    args = ap.parse_args()

    O = read_rows(args.outfile, args.T, args.nlev)
    nmeas = O.shape[0]
    nop = O.shape[3]
    if nop not in CHANNEL_MAPS:
        sys.exit("no channel map for total_n_glue_op = %d (add one to CHANNEL_MAPS)" % nop)
    channels = CHANNEL_MAPS[nop]
    O = O[args.ntherm:]
    print("file %s: %d measurements, %d after ntherm=%d; T=%d, levels=%d, ops/level=%d, bin=%d"
          % (args.outfile, nmeas, O.shape[0], args.ntherm, args.T, args.nlev, nop, args.bin))

    for lev in range(args.nlev):
        print("\n==== blocking level %d ====" % lev)
        print("%-12s %5s %16s %16s" % ("channel", "t", "C(t)/C(0)", "a m_eff(t)"))
        for name, ops in channels.items():
            C, dC, R, dR, meff, dm, nbin = jackknife_channel(O, lev, ops, args.bin, args.tmax)
            for t in range(args.tmax + 1):
                label = name if t == 0 else ""
                print("%-12s %5d %16s %16s" % (label, t, fmt(R[t], dR[t]), fmt(meff[t], dm[t])))
            if args.save is not None:
                ts = np.arange(args.tmax + 1)
                table = np.column_stack([ts, C[: args.tmax + 1], dC[: args.tmax + 1], R[: args.tmax + 1],
                                         dR[: args.tmax + 1], meff[: args.tmax + 1], dm[: args.tmax + 1]])
                fname = "%s_%s_lev%d.txt" % (args.save, name, lev)
                np.savetxt(fname, table, fmt="%.8e", header="t C dC C/C0 dC/C0 meff dmeff  (nbin=%d)" % nbin)

    if args.binscan:
        lev = args.nlev - 1
        print("\n==== bin-size scan: A1_P1, level %d, error of m_eff(t=1) ====" % lev)
        for b in (1, 2, 5, 10, 20, 40, 80):
            if O.shape[0] // b < 10:
                break
            C, dC, R, dR, meff, dm, nbin = jackknife_channel(O, lev, channels["A1_P1"], b, args.tmax)
            print("bin %3d  nbin %4d  m_eff(1) = %s" % (b, nbin, fmt(meff[1], dm[1])))


if __name__ == "__main__":
    main()
