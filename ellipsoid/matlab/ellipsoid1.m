function [x, error, iter, flag] = ellipsoid1(grad, A, x, max_it, tol)

% -- Basic Ellipsoid method for solving uncontrainted convex programming
%
% input   grad     gradient function of f(x)
%	  E(A,x)   ellipsoid containing x*
%         max_it   maximum number of iterations
%         tol      error tolerance                  
%
% output  x        solution vector
%         error    error norm
%         iter     number of iterations performed
%         flag     0 = solution found to tolerance
%                  1 = no convergence given max_it

global errlist;

iter = 0;
flag = 0;
n = size(x,1);
errlist = [];

for iter = 1:max_it
  g = grad(x);
  error = sqrt(g'*A*g);
  errlist = [errlist, error];
  if ( error < tol ) return, end
  gt = g / error;
  Agt = A*gt;
  x = x - Agt/(n+1);  % update ellipsoid
  A = (n*n/(n*n-1))*(A - 2/(n+1)*Agt*Agt');
end

if (error > tol) flag = 1; end

% END of ellipsoid1
