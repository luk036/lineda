function loglik=obj_Whittle(par,omega_x2,omega_y2,P)

tau2=par(1);
alpha=par(2);
nu=par(3);

nvar=length(omega_x2);

loglik1=nvar*log( sum( (alpha^2+omega_x2+omega_y2).^(nu+1).*sum(P-tau2/(4*pi*pi)) ) )
loglik2=-(nu+1)*sum( log(alpha^2+omega_x2+omega_y2) )
loglik=loglik1+loglik2;