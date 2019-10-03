function [sp, tau2] = log_mle_cc_corr_fn(Y, s, opt)

% Assume that y (vector) is Gaussian with zero mean, 
%   i.e., y ~ N(0,Sig+tau^2*I)
% Assume that the spatial correlation is isotopic: 
%   i.e. Sig_{ij} = rho(|| s_j - s_i ||)
% Use B-spline for representing the correlation function rho:
%   i.e. rho(x) = sum_i p_i * B_i(x)
% Request that rho has to be nonincreasing: i.e. p_i >= p_{i+1)
% Request that Sig has to be symmetric positive definite
% Use maximum likelihood estimation.
% log-likelihood function is given by:
%   log det Sig + Trace(Sig^{-1} * Y)
% The first term is concave whereas the second term is convex
% Use convex-concave procedure: successively solve a sequence of convexified 
% problems

n = size(s,1);
d = size(s,2);  % dimension
s_end = s(n,:);
s_begin = s(1,:);
dmax = norm(s_end - s_begin);

% Setup B-spline parameters
kx = 3; % quadratic
deltax = dmax/(n^(1/d))/sqrt(d);     % decide the resolution. better idea???
site = [deltax:deltax:dmax]; % decide the site location
knotsx = augknt([-site,0,site], kx); % knot sequence
C0 = spcol(knotsx, kx, 0);   % collocation matrix for zero
xx = aveknt(knotsx,kx)';     % get the site locations
nn = size(xx,1);

F = zeros(n,n,nn);
for i=1:n,
  F(i,i,:) = C0;
end
for i=1:n-1,
  for j=i+1:n,
    dist = norm(s(j,:) - s(i,:));
    C = spcol(knotsx, kx, dist); % get the collocation matrix of b-spline
    F(i,j,:) = C;
    C = spcol(knotsx, kx, -dist);
    F(j,i,:) = C; % symmetric
  end
end

% Convert to sparse matrices, make cvx solver more efficient???
Fs = cell(nn,1);
for i=1:nn
  Fs{i} = sparse(F(:,:,i));
end
clear F;

% Fourier transform of B-spline
d2 = 0:0.2:dmax;
C2 = spcol(knotsx, kx, d2);
F2 = real(fftshift(C2));
nf = size(d2,2);

% Successively solve the convex-concave problem
I = speye(n);

[sp,tau2,Sig] = lsq_corr_fn(Y,s,opt);
%%Sig= I;

maxiter = 100;
cvx_quiet(true);
c = zeros(nn,1);
%%tau2 = 0;
S = inv(Sig + tau2*I);

% Start convex-concave procedure
disp('|  k | err | logL | tau2');
for k=1:maxiter,
  Sig_k = Sig;

  for i=1:nn,
   c(i) = trace(S*Fs{i});
  end
  t = trace(S);
 
  clear p Sig S tau2;
  cvx_begin
    variable p2(nn/2) % coefficents of B-spline
    variable S(n,n) symmetric
    variable tau2

    p = [p2(nn/2:-1:1); p2];
    % The following statement is equiv. to 
    %    Sig = sum( F(:,:,i)*p(i) ) + tau2*I
    Sig = 0;
    for i=1:nn
      Sig = Sig + Fs{i}*p(i);
    end

    minimize(c'*p + t*tau2 + trace(S*Y))

    subject to
      if (~isempty(strfind(opt, 'm'))), % monotonic decreasing
        for i=1:nn/2-1,
          p2(i) >= p2(i+1);
        end
      end

      if (~isempty(strfind(opt, 'n'))), % nonnegative
        p2 >= 0;
      end

      if (~isempty(strfind(opt, 'f'))), % Fourier transform
        F2*p >= 0;
      end

      % Make S inverse of Sig
      [Sig+tau2*I  I;
        I          S ] == semidefinite(2*n);

      Sig == semidefinite(n);      
      tau2 >= 0;
  cvx_end

  err = norm(Sig - Sig_k,'fro'); 
  %%log_lik = -log(det(Sig)) - trace(inv(Sig)*Y);
  fprintf('| %2d | %g | %g | %g \n', k, err, -cvx_optval, tau2);
  if (err < 1e-4),
    disp('converge!');
    break;
  end 
end

sp = spmak(knotsx, p, nn);

