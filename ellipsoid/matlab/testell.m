% initialize E(A,x)
global errlist;
A = diag(100 * ones(3,1));
x = [0, 0, 0]';

% Use Ellipsoid method
[x, error, iter, flag] = ellipsoid(@quadratic, @gquadratic, 1, A, x, 200, 1e-6)
