% Use cvxfit package to create an example
%
% See also spline.m, gspline.m
%

%%cvxfit2d;
%%sp2 = fncmb(sp4,-1);
%%save sp2 sp2

clear all
global errlist;
global sp2;
load sp2

% initialize E(A,x)
A = diag([1, 1]);
x = [0.5 0.5]';

% Use Ellipsoid method
[x, error, iter, flag] = ellipsoid(@spline, @gspline, 1, A, x, 200, 1e-6)

