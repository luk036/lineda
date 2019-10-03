inverter

.lib '~/share/sm124002-1c.hspice' typical
.inc './exp.txt'


.subckt  inv  in  out  dd
m_p  out  in  dd  dd  pmos_3p3  l=length  w=wp
m_n  out  in  0   0   nmos_3p3  l=length  w=wn
.ends

x1  in  out  dd  inv
* x2  mid  out  dd  inv
Cl  out  0  0.01p

Vd  dd  0  Vdd
Vi  in  0  pulse  0  Vdd  0.2n  0.1n  0.1n  2n  50n

.options  ingold=2
.tran  0.01n  2n $ sweep  monte=10000
*.print  tran  v(in)  v(out)

.measure  tran  T_fall  trig  v(out)  val=2.4  fall=1
+   targ  v(out)  val=0.6  fall=1

* .measure  tran  D_fall  trig  v(in)  val=1.65  rise=1
* +    targ  v(out)  val=1.65  fall=1
* 
* .measure  tran  D_rise  trig  v(in)  val=1.65  fall=1
* +    targ  v(out)  val=1.65  rise=1

.end
