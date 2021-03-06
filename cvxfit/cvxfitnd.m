% 3d example taken from ConvexFit web site
clear all
az = 100; el = 50;
data; % read data into x1,x2,x3,z

[xx1,xx2] = ndgrid(x1,x2);
subplot(2,2,1);
mesh(x1,x2,z(:,:,6).'), view(az,el);
title('The original data in x3=6');

ndim = 3;
x = {x1, x2, x3};
kx = [4, 3, 4];  % cubic for x,z; quadratic for y
npp = {4, 2, 4}; % number of piecewise polynomials

sp3=spap2(npp, kx, x, z);
subplot(2,2,2);
fnplt(sp3); view(az,el);
title('The original spline approximation');

% Below should not specific to ndim=3
eps = 1e-6;
zz = z(:);
knots = fnbrk(sp3,'k'); % we use knot sequence same as spap2()
coefs = squeeze(fnbrk(sp3,'c')); % coefficient matrix

C = 1;
for i=1:ndim
  % Construct collocation matrix
  Cx = spcol(knots{i},kx(i),x{i});
  C = kron(Cx, C);      % append
end

% Make C to be a sparse matrix. Does it help?
C = sparse(C);

% Check correctness
pp = C\zz; % least square sol'n
norm(coefs(:) - pp) < eps,   % ans should be 1

nx = size(pp(:)); % total number of elements
sz = size(coefs); % number of elements for each dimension


% ------------------------------------------------------------
% Least squares approximation with linear constraints
% (not guarantee convex)
cvx_begin
  variable pp(nx)
  minimize( norm(C*pp - zz,1) ); % minimize 1-norm
  pn = reshape(pp, sz);
  subject to
    for d=1:ndim
      xxd = aveknt(knots{d},kx(d))';  % site location 
      p = pn(:,:);
      [n1,n2] = size(p);
      % \partial_{dd} >= 0
      for j=1:n2
        for i=2:n1-1
          (p(i,j) - p(i-1,j))/(xxd(i) - xxd(i-1)) <= ...
            (p(i+1,j) - p(i,j))/(xxd(i+1) - xxd(i));
        end
      end
      pn = shiftdim(pn,1);
    end
cvx_end

p = reshape(pp, size(coefs));
sp4 = spmak(knots, p, size(coefs));
%figure;
subplot(2,2,3);
fnplt(sp4);view(az,el);
title('Not guarantee convex');
% ---------------------------------------------------------------



xx = cell(d,1); % site location for each dimension
k = [1, cumprod(sz(1:ndim-1))]; % k is used for calculating index

for d=1:ndim
  xx{d} = aveknt(knots{d},kx(d));  % site location
end

% Least squares approximation with additional LMI constraints
cvx_begin
  variable pp(nx)
  minimize( norm(C*pp - zz) );
  p = reshape(pp, sz);
  
  subject to
    % Enforce some boundary conditions
    %% for d=1:ndim
    %%   xxd = xx{d};  % site location 
    %%   pn = p(:,:);
    %%   [n1, n2] = size(pn);
    %%   for j=[1,n2]
    %%     for i=2:n1-1
    %%       (pn(i,j) - pn(i-1,j))/(xxd(i) - xxd(i-1)) <= ...
    %%         (pn(i+1,j) - pn(i,j))/(xxd(i+1) - xxd(i));
    %%     end
    %%   end
    %%   p = shiftdim(p,1);
    %% end

    H = repmat(pp(1), ndim, ndim);  % any better method to declare H?
    % LMI inequality
    for ind=1:nx
      % Ignore data points on the boundary
      is_boundary = false;
      ndx = ind;
      for d=ndim:-1:1
        vi = rem(ndx-1,k(d)) + 1; 
        i = (ndx - vi)/k(d) + 1;
        % m = ?
        if (i==1 || i==sz(d)) % todo: need to enhance
          is_boundary = true;
          break;
        end
        ndx = vi;
      end       
    
      if (is_boundary) 
        continue; 
      end
    
      % Construct the "Hessian" matrix for each data point.
      % The matrix is re-scaled in order to avoid the numerical
      % problem of CVX ?!
      ndx1 = ind;
      for d1=ndim:-1:1
        vi = rem(ndx1-1,k(d1)) + 1; 
        i = (ndx1 - vi)/k(d1) + 1;
        x1 = xx{d1};  % site location
        % need to be enhance
        % m = ?
        % n = ?
        % a = ?
        % b = ?
        % c = ?
        % e = ?
        H(d1,d1) = ((pp(ind+k(d1)) - pp(ind))/(x1(i+1) - x1(i)) - ...
                    (pp(ind) - pp(ind-k(d1)))/(x1(i) - x1(i-1)))  ...
                     * (x1(i+1) - x1(i-1));

        ndx2 = ind;
        for d2=ndim:-1:(d1+1)
          vj = rem(ndx2-1,k(d2)) + 1; 
          j = (ndx2 - vj)/k(d2) + 1;
          %x2 = aveknt(knots{d2},kx(d2))';  % site location
          % need to be enhance
          H(d1,d2) = pp(ind+k(d1)+k(d2)) - pp(ind-k(d1)+k(d2)) - ... 
                      pp(ind+k(d1)-k(d2)) + pp(ind-k(d1)-k(d2));

          H(d2,d1) = H(d1,d2);
          ndx2 = vj;
        end
    
        ndx1 = vi;
      end
    
      H == semidefinite(ndim)
    end      
cvx_end

p = reshape(pp, size(coefs));
sp4 = spmak(knots, p, size(coefs));
%figure;
subplot(2,2,4);
fnplt(sp4);view(az,el);
title('cvx B-spline');


% How to check the correctness rigidly?
