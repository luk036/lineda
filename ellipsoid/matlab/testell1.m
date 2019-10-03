% initialize E(A,x)
global errlist;
A = diag(50 * ones(3,1));
x = [0, 0, 0]';
% Use basic ellipsoid method for unconstraint problem
[x, error, iter, flag] = ellipsoid1(@(x)gquadratic(x,0), A, x, 1000, 1e-6)
