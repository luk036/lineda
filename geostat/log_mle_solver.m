function Sig = log_mle_solver(Y);
n = size(Y,1);
cvx_quiet(false);
cvx_begin sdp
  variable S(n,n) symmetric
  maximize(log_det(S) - trace(S*Y))
  subject to 
     S >= 0;
cvx_end
Sig = inv(S);
