function [c,a_W6,a_W7,a_W8,a_Ibias]=mono_vdsat6(W6,W7,W8,Ibias,Vod6)
Nsample = length(W6);

logW6 = log(W6)';
logW7 = log(W7)';
logW8 = log(W8)';
logIbias = log(Ibias)';

logVod6 = log(Vod6)';

A = [ones(Nsample,1), logW6, logW7, logW8, logIbias];
b = logVod6;

x = A\b;

c = exp(x(1));
a_W6 = x(2);
a_W7 = x(3);
a_W8 = x(4);
a_Ibias = x(5);
