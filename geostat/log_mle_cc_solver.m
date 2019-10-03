function [Sig,S] = log_mle_cc_solver(Ys)
[n,N] = size(Ys);
Y = cov(Ys');

% Assume that y (vector) is Gaussian with zero mean, i.e., y ~ N(0,Sig)
% Request that Sig has to be symmetric positive definite
% Use maximum likelihood estimation.
% log-likelihood function is given by:
%   log det Sig + Trace(Sig^{-1} * Y)
% The first term is concave whereas the second term is convex
% Use convex-concave procedure: successively solve a sequence of convexified 
% problems

%% % Successively solve the convex-concave problem
V = eye(n);
I = eye(n);

maxiter = 100;
cvx_quiet(true);
for k=1:maxiter,
  %%V_k = Sig; % covariance matrix at the kth iteration
  %% t1 = log(det(Sig_k)); 
  S_k = inv(Sig_k);
  clear Sig;
  k,
  cvx_begin sdp
    %%variable S(n,n) symmetric
    variable Sig(n,n) symmetric
    variable K(n,n)

    % minimize the convexified negative log-likehood function
    % Note: CVX does not accept sum(matrix_frac(Ys(:,i),Sig)) or
    %   trace(Sig^{-1}*Y)
    minimize(trace(S_k*Sig) + trace_inv(K))
    subject to
      Sig >= 0;
      Sig*inv(Y) == K;
  cvx_end
  err = norm(Sig - Sig_k,'fro'),
  disp(log(det(Sig)) + trace(inv(Sig)*Y));
  if (err < 1e-4),
    disp('converge!');
    break;
  end 
end
disp(log(det(Sig)) + trace(inv(Sig)*Y));
disp(log(det(1.2*Sig)) + trace(inv(1.2*Sig)*Y));
disp(log(det(0.8*Sig)) + trace(inv(0.8*Sig)*Y));
