% initialize E(A,x)
global errlist;
A = diag(10 * ones(3,1));
x = 50*[1 1 1]';

% Use Ellipsoid method
[x, error, iter, flag] = ellipsoid(@linfrac, @glinfrac, 1, A, x, 200, 1e-6)
