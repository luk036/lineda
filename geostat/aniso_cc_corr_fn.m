function [sp,tau2] = aniso_cc_corr_fn(Y, s, nx, ny, opt)

% Assume that y (vector) is Gaussian with zero mean, i.e., y ~ N(0,Sig)
% Use maximum likelihood estimation.
% log-likelihood function is given by:
%   log det Sig + Trace(Sig^{-1} * Y)
% The first term is concave whereas the second term is convex
% Use convex-concave procedure: successively solve a sequence of convexified 
% problems
% Assume 2-dimension

n = size(s,1);
d = size(s,2);
s_end = s(n,:);
s_begin = s(1,:);
dmax = s_end - s_begin;
delta_x = dmax(1)/nx;
delta_y = dmax(2)/ny;

% Setup B-spline parameters
kx = 3; % quadratic
ky = 3;
sitex = [delta_x:delta_x:dmax(1)];
sitey = [delta_y:delta_y:dmax(2)];
knotsx = augknt([-sitex,0,sitex],kx); % knot sequence
knotsy = augknt([-sitey,0,sitey],ky);
%% C0 = spcol(knotsx, kx, 0); % collocation matrix for zero
%% xx = aveknt(knotsx,kx)';   % get the site locations
%% nx = size(C0,2);

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
    %% F(j,i,:) = F(i,j,:); % symmetric
  end
end
% Note: in fact all F's should be sparse

% Convert to sparse matrices
Fs = cell(nn,1);
for i=1:nn
  Fs{i} = sparse(F(:,:,i));
end
clear F;


I = eye(n);
Sig = Y;

maxiter = 100;
cvx_quiet(true);
disp('|  k | err | loglik | tau2');
c = zeros(nn,1);
tau2 = 0;
ncx = floor(nnx/2);
ncy = floor(nny/2);

dx2 = -dmax(1):0.5:dmax(1);
dy2 = -dmax(2):0.5:dmax(2);
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

for k=1:maxiter,
  Sig_k = Sig; % covariance matrix at the kth iteration
  S_k = inv(Sig_k);
  %%[blk,At,C,b] = sdpt3_cc(Y,S_k,Fs);

  for i=1:nn,
    c(i) = trace(S_k*Fs{i});
  end
  t = trace(S_k);

  clear pp p q Sig S;
  cvx_begin 
    variables p2(nn/2) % coefficents of B-spline
    variable S(n,n) symmetric
    %%variable Sig(n,n) symmetric
    variable tau2

    p = [p2(nn/2:-1:1); p2]; % symmetric

    % minimize the convexified negative log-likehood function
    % Note: CVX does not accept sum(matrix_frac(Ys(:,i),Sig)) or
    %   trace(Sig^{-1}*Y)

    minimize(c'*p + t'*tau2 + trace(S*Y))

    subject to
      %% for i=1:nn/2
      %%   p2(i) >= 0;
      %% end

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

      F2*p >= 0        % Fourier transform
    
      % The following statement is equivalent to Sig = sum( F(:,:,i)*p(i) )
      %% Sig == reshape(reshape(F,n*n,nn)*p, n, n) + tau2*I;
      Sig = tau2*I;
      %%Sig = zeros(n,n);
      for i=1:nn
        Sig = Sig + Fs{i}*p(i);
      end

      [Sig  I;
        I   S ] == semidefinite(2*n);
      tau2 >= 0;
  cvx_end

  %%Sig = reshape(reshape(F,n*n,nn)*p, n, n) + tau2*I;
  err = norm(Sig - Sig_k,'fro'); 
  log_lik = -log(det(Sig)) - trace(inv(Sig)*Y);
  fprintf('| %2d | %g | %g | %g \n', k, err, log_lik, tau2);
  if (err < 1e-4),
    disp('converge!');
    break;
  end 
end

sp = spmak({knotsx, knotsy}, p, [nnx, nny]);
disp(-log(det(Sig)) - trace(inv(Sig)*Y));
disp(-log(det(1.2*Sig)) - trace(inv(1.2*Sig)*Y));
disp(-log(det(0.7*Sig)) - trace(inv(0.7*Sig)*Y));
