function loglik=obj_mle_ideal(par,D,S)

alpha=par(1);
nu=par(2);

nvar=size(S,1);
R=corr_matern(alpha,nu,D);

[L,U]=lu(R);

% sigma2,alpha,nu,
% loglik1=nvar*log(sigma2)+sum(log(abs(diag(U)))),
% loglik2=trace(S*inv(U)*inv(L))/sigma2,
% loglik=loglik1+loglik2;

loglik=sum(log(abs(diag(U)))) + nvar*log(trace(S*inv(R)));
