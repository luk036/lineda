* two-stage Op Amp
.lib '~/share/sm124002-1c.hspice' typical
.include './exp.txt'
.include './simu_type.txt'

m1  2  9  1  1  pmos_3p3  l=length  w=w12 
m2  3  8  1  1  pmos_3p3  l=length  w=w12
m3  2  2  0  0  nmos_3p3  l=length  w=w34 
m4  3  2  0  0  nmos_3p3  l=length  w=w34 
m5  1  4  7  7  pmos_3p3  l=length  w=w5
m6  5  3  0  0  nmos_3p3  l=length  w=w6
m7  5  4  7  7  pmos_3p3  l=length  w=w7 
m8  4  4  7  7  pmos_3p3  l=length  w=w8 
Rc  3  6  r
Cc  5  6  c
Ib  4  0  Ibias
Vd  7  0  5

Cl  5  0  3p

.param  Vcm = 2.5

.op
.options ingold=2
.end
