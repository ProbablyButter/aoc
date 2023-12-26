from numpy import *
import sympy

import scipy


def fun(Pr, Vr, ts, Ps, Vs):
    # Pr, Vr, ts
    return [
        Pr[0] + ts[0] * Vr[0] - Ps[0][0] - ts[0] * Vs[0][0],
        Pr[1] + ts[0] * Vr[1] - Ps[0][1] - ts[0] * Vs[0][1],
        Pr[2] + ts[0] * Vr[2] - Ps[0][2] - ts[0] * Vs[0][2],
        Pr[0] + ts[1] * Vr[0] - Ps[1][0] - ts[1] * Vs[1][0],
        Pr[1] + ts[1] * Vr[1] - Ps[1][1] - ts[1] * Vs[1][1],
        Pr[2] + ts[1] * Vr[2] - Ps[1][2] - ts[1] * Vs[1][2],
        Pr[0] + ts[2] * Vr[0] - Ps[2][0] - ts[2] * Vs[2][0],
        Pr[1] + ts[2] * Vr[1] - Ps[2][1] - ts[2] * Vs[2][1],
        Pr[2] + ts[2] * Vr[2] - Ps[2][2] - ts[2] * Vs[2][2],
    ]


def jac(Vr, ts, Vs):
    # Pr, Vr, ts
    return [
        [1, 0, 0, ts[0], 0, 0, Vr[0] - Vs[0][0], 0, 0],
        [0, 1, 0, 0, ts[0], 0, Vr[1] - Vs[0][1], 0, 0],
        [0, 0, 1, 0, 0, ts[0], Vr[2] - Vs[0][2], 0, 0],
        [1, 0, 0, ts[1], 0, 0, 0, Vr[0] - Vs[1][0], 0],
        [0, 1, 0, 0, ts[1], 0, 0, Vr[1] - Vs[1][1], 0],
        [0, 0, 1, 0, 0, ts[2], 0, Vr[2] - Vs[1][2], 0],
        [1, 0, 0, ts[2], 0, 0, 0, 0, Vr[0] - Vs[2][0]],
        [0, 1, 0, 0, ts[2], 0, 0, 0, Vr[1] - Vs[2][1]],
        [0, 0, 1, 0, 0, ts[2], 0, 0, Vr[2] - Vs[2][2]],
    ]


hails = []
with open("input.txt", "r") as f:
    for line in f:
        if len(line):
            tmp = line.split("@")
            tmp1 = tmp[0].split(",")
            tmp2 = tmp[1].split(",")
            hails.append(
                [
                    int(tmp1[0].strip()),
                    int(tmp1[1].strip()),
                    int(tmp1[2].strip()),
                    int(tmp2[0].strip()),
                    int(tmp2[1].strip()),
                    int(tmp2[2].strip()),
                ]
            )
hails = array(hails)
Ps = hails[:3, :3]
Vs = hails[:3, 3:]

# Ps = [[sympy.Symbol(f'P{i}{j}', real=True) for j in range(3)] for i in range(3)]
# Vs = [[sympy.Symbol(f'V{i}{j}', real=True) for j in range(3)] for i in range(3)]

Pr = [sympy.Symbol(f"Pr{i}", real=True) for i in range(3)]
Vr = [sympy.Symbol(f"Vr{i}", real=True) for i in range(3)]
ts = [sympy.Symbol(f"t{i}", real=True, positive=True) for i in range(3)]
system = fun(Pr, Vr, ts, Ps, Vs)
sol = sympy.solve(system, [*Pr, *Vr, *ts], dict=True)[0]
