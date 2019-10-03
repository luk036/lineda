% compute the fall transistion time from Wn, Wp
% posynomial coefs are computed by minimizing the 
% norm of fitting errors 
function  f=fitgp_fall(p,Wn,Wp)
        f = ((1/Wn + p(1))^p(2) * (Wp + p(3))^p(4))*p(5);
