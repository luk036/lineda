function [c,a_W1,a_W5,a_W8,a_Ibias]=mono_vdsat1(W1,W5,W8,Ibias,Vod1)
Nsample = length(W1);

logW1 = log(W1)';
logW5 = log(W5)';
logW8 = log(W8)';
logIbias = log(Ibias)';

logVod1 = log(-1*Vod1)';

A = [ones(Nsample,1), logW1, logW5, logW8, logIbias];
b = logVod1;

x = A\b;

c = exp(x(1));
a_W1 = x(2);
a_W5 = x(3);
a_W8 = x(4);
a_Ibias = x(5);


