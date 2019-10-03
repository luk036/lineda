Boyd Tutorial, pp.63
.inc './exp.txt'

Vin  1  0  pulse  0  1  10n  1n  1n  100n  200n  
R1  1  2  R1
R2  2  3  R2

C1  2  0  C1
C2  3  0  C2

.options ingold=2
.tran  1n  100n
*.print  tran  v(1) v(3)

.measure  tran  d2  trig  v(1)  val=0.5  rise=1
+  targ  v(2)  val=0.5  rise=1

.measure  tran  d3  trig  v(1)  val=0.5  rise=1
+  targ  v(3)  val=0.5  rise=1

.end



