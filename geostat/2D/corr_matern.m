function rou = corr_matern(alpha,nu,H)
% rou = corr_matern(alpha,nu,H)
% generate Matern correlation function with alpha and nu as controlling parameters
% Input:
%   1/alpha : the correlation range
%   nu      : measures the degree of smoothness of the process,
%             the higher the value, the smoother the process would be
%   H       : the distance,
%             H can be a real number or a vector or a distance matrix.
% Output:
%   rou     : correlation of Matern

Hr=alpha .* H;
r1=2^(nu-1) * gamma(nu);
bes=besselk(nu,Hr);
rou=(1./r1).*(Hr.^nu).*bes;

i = find(H==0);
rou(i) = 1;