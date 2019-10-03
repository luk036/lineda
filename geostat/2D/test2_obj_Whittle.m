function loglik=test2_obj_Whittle(par,omega_x2,omega_y2,mp)

alpha=par(1);
nu=par(2);

nvar=length(omega_x2);

temp=alpha^2+omega_x2+omega_y2;
loglik1=nvar*log( sum( temp.^(nu+1).*mp ) );
loglik2=-(nu+1)*sum( log(temp) );
loglik=loglik1+loglik2;