function [q1,q0,b,c] = rank1fit(x, f, tol, max_iter)
%
% Rank-One Quadratic Posynomial Fitting by Implicit Power Iteration
%
% Input:
%   x, z: scattered data
%
% Output
%   q1,q0,b,c such that 
%      sum_i [(yi'*q1)*(q0'*yi) + b'*yi + c - fi]^2 
%   is minimized, where y = [1/x1, 1/x2, ..., 1/xN, x1, x2, ..., xN]
%
% Requires CVX package.
%
% References:
%
% [1] X. Li et al., Robust Analog/RF Circuit Design with Projection-Based 
%     Posynomial Modeling, 2004
%
N = size(x,1);
inv_x = 1./x;
y = [inv_x; x];
q0 = rand(2*N,1); % initialize randomly ???
min = 1e100;

cvx_quiet(true);

for k=1:max_iter,
  q0 = q0 / norm(q0); % normalize q0

  cvx_begin 
    variables q1(2*N) b(2*N) c;
    t = ((q1'*y).*(q0'*y) + b'*y + c) - f;
    minimize( t*t' );
    subject to
      q1 == nonnegative(2*N);
      b  == nonnegative(2*N);
      c  >= 0;
  cvx_end

  if (abs(cvx_optval - min) < tol) 
    break; 
  end

  min = cvx_optval;
  q0 = q1;
end

