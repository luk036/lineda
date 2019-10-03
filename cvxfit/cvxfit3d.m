% 3d example taken from ConvexFit web site
clear all; close all
az = 100; el = 50;
data; % read data into x1,x2,x3,z

[xx1,xx2] = ndgrid(x1,x2);
subplot(2,2,1);
mesh(x1,x2,z(:,:,6).'), view(az,el);
title('The original data');

n = 3; 
x = {x1, x2, x3};
kx = [4, 3, 4];   % order of B-spline

%%for i=1:n
%%  knotsx{i} = aptknt(x{i},kx(i));
%%end

sp3=spap2({4,2,4}, kx, x, z);
subplot(2,2,2);
fnplt(sp3); view(az,el);
title('The original spline approximation');

eps = 1e-6;
knots = fnbrk(sp3,'k');
zz = z(:);
coefs = squeeze(fnbrk(sp3,'c')); % coefficient matrix

C = 1;
for i=1:n
  %% Construct collocation matrix
  Cx = spcol(knots{i},kx(i),x{i});
  C = kron(Cx, C);      % append
end

%% Check correctness
pp = C\zz; % least square sol'n
norm(coefs(:) - pp) < eps
nx = size(pp(:));
error = zz - C*pp;
error'*error

sz = size(coefs);
% Least squares approximation with additional linear constraints
% (not sufficient)
cvx_begin
  variable pp(nx)
  minimize( norm(C*pp - zz) );
  pn = reshape(pp, sz);
  subject to
    for d=1:n
      xx = aveknt(knots{d},kx(d))';  % site location 
      p = pn(:,:);
      [n1,n2] = size(p);
      % \partial_{dd} >= 0
      for j=1:n2
        for i=2:n1-1
          (p(i,j) - p(i-1,j))/(xx(i) - xx(i-1)) <= ...
            (p(i+1,j) - p(i,j))/(xx(i+1) - xx(i));
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
title('Convex fitting method 1');
%% Check
%%%diff(diff(p(:,2,2)')./diff(x{1})) > -eps
%%%diff(diff(p(2,:,2))./diff(x{2})) > -eps

x1 = aveknt(knots{1},kx(1))';  % site location 
x2 = aveknt(knots{2},kx(2))';  % site location 
x3 = aveknt(knots{3},kx(3))';  % site location 

%B = repmat(0, ones(1,n)*3);
 
% Least squares approximation with additional constraints
% (sufficient and necessary)
cvx_begin
  variable pp(nx)
  minimize( norm(C*pp - zz) );
  p = reshape(pp, sz);
  subject to
    for d=1:n
      xx = aveknt(knots{d},kx(d))';  % site location 
      pn = p(:,:);
      [n1, n2] = size(pn);
      for j=1:n2
        for i=2:n1-1
          (pn(i,j) - pn(i-1,j))/(xx(i) - xx(i-1)) <= ...
            (pn(i+1,j) - pn(i,j))/(xx(i+1) - xx(i));
        end
      end
      p = shiftdim(p,1);
    end

    %H = zeros(n,n);
    % LMI inequality
    for ind=1:nx
      is_boundary = false;
      [i,j,k] = ind2sub(sz,ind);
      sub = [i,j,k];
      for d=1:n
        if (sub(d)==1 || sub(d)==sz(d))
          is_boundary = true;
          break;
        end;
      end;
      if (is_boundary) 
        continue; 
      end
 
      H11 = ((p(i+1,j,k) - p(i,j,k))/(x1(i+1) - x1(i)) - ...
       (p(i,j,k) - p(i-1,j,k))/(x1(i) - x1(i-1))) * (x1(i+1) - x1(i-1));
      H22 =  ((p(i,j+1,k) - p(i,j,k))/(x2(j+1) - x2(j)) - ...
       (p(i,j,k) - p(i,j-1,k))/(x2(j) - x2(j-1))) * (x2(j+1) - x2(j-1));
      H33 =  ((p(i,j,k+1) - p(i,j,k))/(x3(k+1) - x3(k)) - ...
       (p(i,j,k) - p(i,j,k-1))/(x3(k) - x3(k-1))) * (x3(k+1) - x3(k-1));

      H12 = (p(i+1,j+1,k)-p(i-1,j+1,k)-p(i+1,j-1,k)+p(i-1,j-1,k)); 
      H13 = (p(i+1,j,k+1)-p(i-1,j,k+1)-p(i+1,j,k-1)+p(i-1,j,k-1)); 
      H21 =  (p(i+1,j+1,k)-p(i-1,j+1,k)-p(i+1,j-1,k)+p(i-1,j-1,k)); 
      H23 =  (p(i,j+1,k+1)-p(i,j+1,k-1)-p(i,j-1,k+1)+p(i,j-1,k-1)); 
      H31 =  (p(i+1,j,k+1)-p(i-1,j,k+1)-p(i+1,j,k-1)+p(i-1,j,k-1)); 
      H32 =  (p(i,j+1,k+1)-p(i,j+1,k-1)-p(i,j-1,k+1)+p(i,j-1,k-1)); 

      [H11,H12,H13; H21,H22,H23; H31,H32,H33] == semidefinite(n);
    end      
cvx_end

error = zz - C*pp;
error'*error
p = reshape(pp, size(coefs));
sp4 = spmak(knots, p, size(coefs));
%figure;
subplot(2,2,4);
fnplt(sp4);view(az,el);
title('Convex fitting method 2');
% Check 
%%diff(diff(p(:,2,2)')./diff(x{1})) > -eps
%%diff(diff(p(2,:,2))./diff(x{2})) > -eps
