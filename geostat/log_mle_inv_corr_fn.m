function sq = log_mle_inv_corr_fn(Y, s)
%%clear all
%%exp1d_data;
%%Y = (Ys*Ys')./N;

% Assume that y (vector) is Gaussian with zero mean, i.e., y ~ N(0,Sig)
% Assume that the spatial correlation is isotropic: 
%   i.e. Sig_{ij} = rho(|| s_j - s_i ||)
% Use B-spline for representing the inverse correlation function rho':
%   i.e. rho'(x) = sum_i q_i * B_i(x)
% Request that S = Sig^{-1} (and hence Sig) has to be s.p.d
% Use maximum likelihood estimation.
% Negative log-likelihood function is given by:
%   -log det S + Trace(S * Y)
% and is convex in term of S

% Assume isotropic

n = size(s,1);
s_end = s(n,:);
s_begin = s(1,:);
dmax = norm(s_end - s_begin);

% Setup B-spline parameters
kx = 3; % quadratic
site = [0:0.4:0.8, 1:2:dmax];
knotsx = augknt(site,kx); % knot sequence
C0 = spcol(knotsx, kx, 0); % collocation matrix for zero
nx = size(C0,2);

% F is used to construct the inverse correlation matrix 

F = zeros(n,n,nx);
for i=1:n,
  F(i,i,:) = C0;
end
for i=1:n-1,
  for j=i+1:n,
    d = norm(s(j,:) - s(i,:));
    C = spcol(knotsx, kx, d);
    F(i,j,:) = C;
    F(j,i,:) = F(i,j,:); % symmetric
  end
end
% Note: in fact all F's should be sparse

%%d2 = 0:0.5:dmax;
%%C2 = spcol(knotsx, kx, d2);
%%F2 = real(fft(C2));

%%I = eye(n);

cvx_quiet(false);
clear q;
clear S;
cvx_begin sdp
  variable q(nx)  % coefficents of B-spline
  variable S(n,n) symmetric
  S == reshape(reshape(F,n*n,nx)*q, n, n);
  minimize(-log_det(S) + trace(S*Y))
  subject to
    
    %%F2*q >= 0;
    S >= 0;
cvx_end
sq = spmak(knotsx, q, nx);
%%mesh(inv(S));
disp(-log(det(1.2*S)) + trace((1.2*S)*Y));
disp(-log(det(0.85*S)) + trace((0.85*S)*Y));
