function [c,a_W3,a_W5,a_W8,a_Ibias]=mono_vdsat3(W3,W5,W8,Ibias,Vod3)
Nsample = length(W3);

logW3 = log(W3)';
logW5 = log(W5)';
logW8 = log(W8)';
logIbias = log(Ibias)';

logVod3 = log(Vod3)';

A = [ones(Nsample,1), logW3, logW5, logW8, logIbias];
b = logVod3;

x = A\b;

c = exp(x(1));
a_W3 = x(2);
a_W5 = x(3);
a_W8 = x(4);
a_Ibias = x(5);


