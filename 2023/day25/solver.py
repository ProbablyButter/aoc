import sympy

Ps = [sympy.Symbol(f'P{i}') for i in range(2)]
ts = [sympy.Symbol(f't{i}') for i in range(2)]
Vs = [sympy.Symbol(f'V{i}') for i in range(2)]

Pr = sympy.Symbol(f'Pr')
Vr = sympy.Symbol(f'Vr')

system = [Ps[i] + ts[i] * Vs[i] - (Pr + ts[i] * Vr) for i in range(2)]
sol = sympy.solve(system, [Pr, Vr], dict=True)

from numpy import *

hail = []
with open('input.txt','r') as f:
    for line in f:
        if(len(line)):
            pos,vel=line.split('@')
            hail.append([[int(v) for v in pos.split(',')], [int(v) for v in vel.split(',')]])

hail = array(hail)

min_pos = amin(hail[:,0,:],axis=0)
max_pos = amax(hail[:,0,:],axis=0)


