function [c,a_W8,a_Ibias]=mono_vdsat8(W8,Ibias,Vod8)
Nsample = length(W8);

logW8 = log(W8)';
logIbias = log(Ibias)';

logVod8 = log(-1*Vod8)';

A = [ones(Nsample,1), logW8, logIbias];
b = logVod8;

x = A\b;

c = exp(x(1));
a_W8 = x(2);
a_Ibias = x(3);
