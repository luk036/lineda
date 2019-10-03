function sp = spcvx2d(knots,k,x,y,z)
%
% spcvx2d: convex fitting with tensor-product B-spline for bivariate
% cases with rectangular gridded data
%
% Input:
%   knots: knot sequence on the interval to be approximated
%   k: B-spline's order: 2-linear, 3-quadratic, 4-cubic, 5-quartic
%   (x,y): sampled data in rectangular grid
%   z: gridded data
%
% Output
%   sp: the tensor-product B-spline surface
%
% Requires Spline Toolbox and CVX package.
%
% References:
%
% [1] Michael S. Floater. A weak condition for the convexity of
% tensor-product Bezier and B-spline surfaces. Advances in Comp. Math.,
% 2:67-80, 1994
u = knots{1};
v = knots{2};
m = k(1);
n = k(2);
xx = aveknt(u,m)';  % site location
yy = aveknt(v,n)';
nx = size(xx,1);
ny = size(yy,1);
Cx = spcol(u,m,x); % collocation matrix
Cy = spcol(v,n,y);
C = kron(Cy, Cx); % Kronecker product
% Make C to be a sparse matrix
C = sparse(C);
zz = z(:);
nn = nx*ny;

cvx_quiet(true);
% Least squares fitting with linear matrix inequality (LMI) constraints
cvx_begin sdp
  variable pp(nn)
  minimize( norm(C*pp - zz) );
  p = reshape(pp, [nx, ny]);
  subject to
    % a_{ij} >= 0
    for i=1:nx
      for j=3:ny
        (p(i,j) - p(i,j-1))/(v(j+n-1) - v(j-1)) - ...
          (p(i,j-1) - p(i,j-2))/(v(j+n-2) - v(j-2)) >= 0
      end
    end
    % c_{ij} >= 0
    for i=3:nx
      for j=1:ny
        (p(i,j) - p(i-1,j))/(u(i+m-1) - u(i-1)) - ...
          (p(i-1,j) - p(i-2,j))/(u(i+m-2) - u(i-2)) >= 0;
      end
    end
    % LMI inequality: [a b; b c] >= 0
    for i=3:nx
      for j=3:ny
        for k=0:1
          for l=0:1
            for r=0:1
              for s=0:1
                [m*(m-1)*((p(i,j-k-r) - p(i-1,j-k-r))/(u(i+m-1) - u(i-1)) - ...
                  (p(i-1,j-k-r) - p(i-2,j-k-r))/(u(i+m-2) - u(i-2))) / (u(m+i-2) - u(i-1)), ...
                 m*n*(p(i-l,j-k) - p(i-l,j-k-1) - p(i-l-1,j-k) + p(i-l-1,j-k-1)) ...
                  /(u(i-l+m-1) - u(i-l-1)) / (v(j-k+n-1) - v(j-k-1));
                 m*n*(p(i-l,j-k) - p(i-l,j-k-1) - p(i-l-1,j-k) + p(i-l-1,j-k-1)) ...
                  /(u(i-l+m-1) - u(i-l-1)) / (v(j-k+n-1) - v(j-k-1)), ...
                 n*(n-1)*((p(i-l-s,j) - p(i-l-s,j-1))/(v(j+n-1) - v(j-1)) - ...
                  (p(i-l-s,j-1) - p(i-l-s,j-2))/(v(j+n-2) - v(j-2))) / (v(n+j-2) - v(j-1)) 
                 ] >= 0;
              end
            end
          end
        end
      end
    end
cvx_end

p = reshape(pp, [nx, ny]);
sp = spmak({u,v},p,[nx, ny]);
