function [c,a_W1,a_I1]=...
        mono_gm1(W1,I1, gm1)
Nsample = length(W1);

logW1 = log(W1)';
logI1 = log(I1)';

logGm1 = log(gm1)';

A = [ones(Nsample,1), logW1, logI1];
b = logGm1;

x = A\b;

c = exp(x(1));
a_W1 = x(2);
a_I1 = x(3);
