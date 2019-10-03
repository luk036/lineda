%% n = 3;
%% N = 100;
%% A = rand(n,n);
%% Y = A'*A;
%% I = eye(n);
exp1d_data;

cvx_begin
  variable W(n,n) symmetric
  variable V(n,n) symmetric
  minimize (-log_det(W) - n*log(trace(W*Y)/n))
  subject to
      W == semidefinite(n);
cvx_end
