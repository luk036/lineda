function [c,a_W8,a_Ibias]=mono_vdsat5(W8,Ibias,Vod5)
Nsample = length(Ibias);

logW8 = log(W8)';
logIbias = log(Ibias)';

logVod5 = log(-1*Vod5)';

A = [ones(Nsample,1), logW8, logIbias];
b = logVod5;

x = A\b;

c = exp(x(1));
a_W8 = x(2);
a_Ibias = x(3);


