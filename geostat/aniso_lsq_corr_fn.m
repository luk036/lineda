function [sp,tau2] = aniso_lsq_corr_fn(Y, s, nx, ny, opt)

% Assume that the spatial correlation is homogeneous
%   i.e. Sig_{ij} = rho(s_j - s_i)
% Use Tensor porduct B-spline for representing the correlation function rho:
%   i.e. rho(h) = sum_i p_ij * B_i(hx) * B_j(hy)
% Optional Constraints:
%   'm' : monotonic in x- and y-direction
% Use least squares estimation.

n = size(s,1);
s_end = s(n,:);
s_beg = s(1,:);
dmax = s_end - s_beg;
delta_x = dmax(1)/(nx-1); % better idea ???
delta_y = dmax(2)/(ny-1); % better idea ???

% Setup B-spline parameters
kx = 3; % quadratic
ky = 3;
sitex = [delta_x:delta_x:dmax(1)];
sitey = [delta_y:delta_y:dmax(2)];
knotsx = augknt([-sitex,0,sitex],kx); % knot sequence
knotsy = augknt([-sitey,0,sitey],ky);

knots = {knotsx, knotsy};
k = [kx, ky];
u = knots{1};
v = knots{2};
xx = aveknt(u,kx)'; % site location
yy = aveknt(v,ky)';
nnx = size(xx,1);
nny = size(yy,1);
nn = nnx*nny;

F = zeros(n,n,nn);
for i=1:n,
  for j=1:n,
    dist = s(j,:) - s(i,:);
    Cx = spcol(u, kx, dist(1));
    Cy = spcol(v, ky, dist(2));
    C = kron(Cy, Cx); % Kronecker product
    F(i,j,:) = C;
  end
end

% Convert to sparse matrices
Fs = cell(nn,1);
for i=1:nn
  Fs{i} = sparse(F(:,:,i));
end
clear F;

I = eye(n);
ncx = floor(nnx/2);
ncy = floor(nny/2);

% Setup for Fast Fourier Transform
dx2 = -dmax(1):delta_x/2:dmax(1);   % better idea ???
dy2 = -dmax(2):delta_y/2:dmax(2);   % better idea ???
szdx2 = size(dx2,2);
szdy2 = size(dy2,2);
Cx2 = spcol(knotsx, kx, dx2);
Cy2 = spcol(knotsy, ky, dy2);
C2 = kron(Cy2,Cx2);
C2 = sparse(C2);
F2 = zeros(szdx2*szdy2,nn);
for i=1:nn
  T = real(fftshift(full(reshape(C2(:,i),szdx2,szdy2))));
  F2(:,i) = T(:);
end

cvx_begin 
  variables p2(nn/2) % coefficents of B-spline
  variable tau2

  p = [p2(nn/2:-1:1); p2]; % symmetric
  % The following statement is equivalent to Sig = sum( F(:,:,i)*p(i) )
  %% Sig == reshape(reshape(F,n*n,nn)*p, n, n) + tau2*I;
  Sig = 0;
  for i=1:nn
    Sig = Sig + Fs{i}*p(i);
  end

  minimize( norm(Sig+tau2*I - Y,'fro') )
  subject to

    % Monotonicity
    if (~isempty(strfind(opt, 'm'))), % monotonic decreasing
      pp = reshape(p, [nnx, nny]);
      for i=1:ncx-1
        for j=1:nny
          pp(i,j) <= pp(i+1,j);
        end
      end
      for i=1:nnx
        for j=1:ncy-1
          pp(i,j) <= pp(i,j+1);
        end
      end
    end

    F2*p >= 0        % Fourier transform
  
    Sig == semidefinite(n);
    tau2 >= 0;
cvx_end
sp = spmak({knotsx, knotsy}, p, [nnx, nny]);
