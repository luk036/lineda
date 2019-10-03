function [x,status] = gpellip(A,b,szs,x0,G,h,quiet)

% [x,status] = gpellip(A,b,szs,x0,G,h,quiet)
%
% solves the geometric program in convex form with a starting point
% using Ellipsoid method
%
%  minimize    lse(y0)
%  subject to  lse(yi) <= 0,   i=1,...,m,
%              Ai*x+bi = yi,   i=0,...,m,
%              G*x+h = 0,
%
% where lse is defined as  lse(y) = log sum_i exp yi,
%
% Input arguments:
%
% - A:         (sum_i n_i) x n matrix; A = [A0' A1' ... Am' ]'
% - b:         (sum_i n_i) vector;   b = [b0' b1' ... bm' ]'
% - szs:       dimensions of Ai and bi; szs = [Nob n1 ... nm]' 
%              where Ai is (ni x n) and bi is (ni x 1)
% - x0:        n-vector;
% - G:         p x n matrix
% - h:         p-vector
% - quiet:     boolean variable; suppress all the print messages if true
%
% Output arguments:
%
% - x:         n-vector; primal optimal point
% - status:    scalar;
%              2	Function converged to a solution x.
%             -1	Number of iterations exceeded MAXITERS.
%             -2        No feasible solution

%----------------------------------------------------------------------
%               INITIALIZATION
%----------------------------------------------------------------------

% PARAMETERS
EPS     = 1e-8;
MAXITER = 500;
INITR   = 100;  % initial radius of ellipsoid (sphere)

% DIMENSIONS
N   = size(A,1);
n   = size(A,2);
p   = size(G,1);
m   = length(szs)-1;
n0  = szs(1);

if (isempty(G)) 
  G = zeros(0,n);
  h = zeros(0,1);
end
warning off all;

% SPARSE ZERO MATRIX
Opxp = sparse(p,p);

% SUM MATRIX
% E is a matrix s.t. [1'*y0  1'*y1  ... 1'*ym ]' = E*y
E     = spalloc(m+1,N,N);
indsf = cumsum(szs)-szs+1;
indsl = cumsum(szs);
for i = 1:m+1
  E(i,indsf(i):indsl(i)) = ones(1,szs(i));
end;
x = x0;

% INITIAL ELLIPSOID
Ae = diag(INITR * ones(n,1));

%----------------------------------------------------------------------
%               MAIN LOOP
%----------------------------------------------------------------------
global errlist;
errlist = [];
A0 = A(indsf(1):indsl(1), :);
E0 = E(1,:);

for iters = 1:MAXITER
  y   = A*x+b;
  f   = log(E*exp(y));  
  feasible = 1;
  
  for j = 2:m+1,
    fj = f(j);
    if (fj > 0)     %  x infeasible
      % Compute the gradient
      Aj = A(indsf(j):indsl(j), :);
      yj = y(indsf(j):indsl(j));
      Ej = E(j,:);
      g = Aj' * (exp(yj)./sum(exp(yj)));
      Aeg = Ae*g;
      error2 = g'*Aeg;
      if (error2 < 0) flag = 1; return; end
      error = sqrt(error2);
      if (fj - error > 0)    % no feasible sol'n
        if (~quiet) disp(['No feasible solution.']); end
        flag = -2; 
        return; 
      end
      feasible = 0;
      break;
    end
  end

  % todo: take care of G, h

  if (feasible)   %  x feasible    
    % Compute the gradient
    y0 = y(indsf(1):indsl(1));
    g = A0' * (exp(y0)./(sum(exp(y0))));
    Aeg = Ae*g;
    error2 = g'*Aeg;
    if (error2 < 0) flag = 1; return; end
    error = sqrt(error2);
    errlist = [errlist, error];
    if (error < EPS)
      if (~quiet) disp(['Solved.']); end
      status = 2; 
      return;
    end
  end

  % Find min. volume ellipsoid containing feasible sol'ns.
  %gt = g / error;
  %Aegt = Ae*gt;
  Aeg = Aeg/error;
  x = x - Aeg/(n+1);  % update center
  Ae = n*n/(n*n-1)*(Ae - 2/(n+1)*Aeg*Aeg');
  
end
if (iters >= MAXITER)
  if (~quiet) disp(['Maxiters exceeded.']); end
  status = -1;
  return;
end
