function sp = spmono1d(knots,k,x,y)
%
% spcvx1d: convex fitting with B-spline for univariate cases
%
% Input:
%   knots: knot sequence on the interval to be approximated
%   k : B-spline's order: 2-linear, 3-quadratic, 4-cubic, 5-quartic
%   (x,y): sampled data
%
% Output
%   sp: the B-spline curve
%
% Requires Spline Toolbox and CVX package.
C = spcol(knots, k, x);  % obtain collocation matrix
xx = aveknt(knots,k)';   % get the site locations
nc = size(xx,1);         % number of sites (should equal to npp+k-1)

% Formulate convex fitting problem as convex quadratic programming 
% with linear inequality constraints
cvx_quiet(false);
cvx_begin
  cvx_precision low
  variable p(nc);
  minimize( norm(C*p - y));
  subject to
    for i=1:nc-1
      p(i+1) >= p(i);
    end
cvx_end

sp = spmak(knots,p,size(p')); % construct the cvx B-spline

