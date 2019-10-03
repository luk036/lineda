function sp = spnonneg1d(knots,k,x,y)
%
% spnonneg1d: non-negative fitting with B-spline for univariate cases
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
ny = size(y, 1);
e = ones(ny,1);
z = zeros(nc, 1);
I = eye(nc);
A = [C, -e;              % C*p - t <= y
     -C, -e;             % -C*p - t <= -y
     -I, z];             % -p(i) <= 0 
f = [z; 1];
b = [y; -y; z];
pt = linprog(f, A, b);


% Formulate convex fitting problem as convex quadratic programming 
% with linear inequality constraints
%% cvx_quiet(false);
%% cvx_begin
%%   cvx_precision low
%%   variable p(nc);
%%   minimize( norm(C*p - y));
%%   subject to
%%     for i=1:nc
%%       % Enforce second divided difference >= 0
%%       p(i) >= 0;
%%     end
%% cvx_end

p = pt(1:nc),
sp = spmak(knots,p,size(p')); % construct the cvx B-spline

