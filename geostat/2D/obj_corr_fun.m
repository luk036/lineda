function F=obj_corr_fun(x,xdata)
% F=obj_corr_fun(x,xdata)
% function handler for the optimization to extract parameters of the correlation function
% Input:
%   x(1) : alpha, parameter of Matern correlation function
%   x(2) : nu, parameter of Matern correlation function
%   x(3) : var_s, extracted variance of inter-chip global variation component
%   x(4) : var_g, extracted variance of intra-chip spatial variation component
alpha=x(1);
nu=x(2);
var_s=x(3);
var_g=x(4);

F = var_s*corr_matern(alpha,nu,xdata)+var_g;