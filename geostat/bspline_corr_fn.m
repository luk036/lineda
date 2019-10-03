%% exp1d_data;
%% addpath /export/home/xuexin/share/cvx
%% Y = (Ys*Ys')./N;

% Assume that the spatial correlation is isotopic: 
%   i.e. Sig_{ij} = Corr(s_j - s_i) forall i < j
% Use B-spline for representing the correlation function
% Request that the correlation function has to be nonincreasing
% Request that the correlation matrix has to be symmetric positive definite

% An example with a known Sig
Sig_k = zeros(n,n);
for i=1:n-1,
  for j=i+1:n,
    x = s(j) - s(i);
    Sig_k(i,j) = exp(-0.5*x*x/0.4/0.4/2);
    Sig_k(j,i) = Sig_k(i,j);
  end
end
for i=1:n,
  Sig_k(i,i) = 1;
end

% Setup B-spline parameters
kx = 3; % quadratic
knotsx = augknt([0:0.2:0.8,1:10],kx); % knot sequence
C0 = spcol(knotsx, kx, 0); % collocation matrix for zero distance
nx = size(C0,2);

F = zeros(n,n,nx);
for i=1:n-1,
  for j=i+1:n,
    x = s(j) - s(i);
    C = spcol(knotsx, kx, x);
    F(i,j,:) = C;
    F(j,i,:) = F(i,j,:); % symmetric
  end
end
for i=1:n,
  F(i,i,:) = C0;
end

cvx_quiet(false);

clear Sig;
cvx_begin sdp
  variable p(nx)  % coefficents of B-spline
  % The following statement is equivalent to Sig = sum( F(:,:,i)*p(i) )
  Sig = reshape(reshape(F,n*n,nx)*p, n, n);
  minimize(norm(Sig - Sig_k))

  subject to
    for i=1:nx-1,
      p(i) >= p(i+1); % nondecreasing
      p(i) >= 0; % nonnegative
    end
    Sig >= 0;

cvx_end

sp = spmak(knotsx, p, nx);
fnplt(sp,'r');
