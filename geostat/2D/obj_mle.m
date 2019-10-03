function loglik=obj_mle(par,D,S)

kappa=par(1);
alpha=par(2);
nu=par(3);

nvar=size(S,1);
R=corr_matern(alpha,nu,D);

V=R+kappa*eye(nvar);
[L,U]=lu(V);

% sigma2,alpha,nu,
% loglik1=sum(log(abs(diag(U)))),
% loglik2=n*log(trace(S*inv(V))),
% loglik=loglik1+loglik2;

loglik=sum(log(abs(diag(U)))) + nvar*log(trace(S*inv(V)));