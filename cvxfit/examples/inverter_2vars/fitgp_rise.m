% compute the rise transistion time from Wn, Wp
% posynomial coefs are computed by minimizing the 
% norm of fitting errors 
function  f=fitgp_rise(p,Wn,Wp)
        f = ((Wn + p(1))^p(2) * (1/Wp + p(3))^p(4))*p(5);
