function [x, error, iter, flag] = ellipsoid(fct, gradfct, m, Ae, x, max_it, tol)

% -- Ellipsoid method for sovling convex programming
%     minimize      fct(x,0)
%     subject to    fct(x,j) <= 0
%       where fct(x,i)'s are convex
%
% input   fct(0)        objective function
%	  gradfct(0)    gradient of objective function
%	  fct(1..m)     constraint function (only allow one constraint)
%         gradfct(1..m) gradient of constraint function 
%         m             number of constraints
%	  E(Ae,x)       ellipsoid containing x*
%         max_it        maximum number of iterations
%         tol           error tolerance                  
%
% output  x             solution vector
%         error         error norm
%         iter          number of iterations performed
%         flag          0 = solution found to tolerance
%                       1 = no convergence given max_it
%                       2 = no feasible sol'n
 
global errlist;

iter = 0;
flag = 0;
errlist = [];
n = size(x,1);

for iter = 1:max_it
  feasible = 1;
  for j = 1:m,
    fj = fct(x,j);
    if (fj > 0)     %  x infeasible
      g = gradfct(x,j);
      Aeg = Ae*g;
      error2 = g'*Aeg;
      if (error2 < 0) flag = 1; return; end
      error = sqrt(error2);
      if (fj - error > 0) flag = 2; return; end   % no feasible sol'n
      feasible = 0;
      break;
    end
  end

  if (feasible)   %  x feasible
    g = gradfct(x,0);
    Aeg = Ae*g;
    error2 = g'*Aeg;
    if (error2 < 0) flag = 1; return; end
    error = sqrt(error2);
    errlist = [errlist, error];
    if (error < tol) return, end
  end
  % Find min. volume ellipsoid containing feasible sol'ns.
  %gt = g / error;
  %Aegt = Ae*gt;
  Aegt = Aeg/error;
  x = x - Aegt/(n+1);  % update center
  Ae = n*n/(n*n-1)*(Ae - 2/(n+1)*Aegt*Aegt');
end

if (error > tol) flag = 1; end

% END of ellipsoid
