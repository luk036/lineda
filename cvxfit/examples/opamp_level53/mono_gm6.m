function [c,a_W6,a_I6]=...
        mono_gm6(W6,I6, gm6)
Nsample = length(W6);

logW6 = log(W6)';
logI6 = log(I6)';

logGm6 = log(gm6)';

A = [ones(Nsample,1), logW6, logI6];
b = logGm6;

x = A\b;

c = exp(x(1));
a_W6 = x(2);
a_I6 = x(3);

