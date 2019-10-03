function [c,a_W7,a_W8,a_I8]=...
        mono_go6(W7,W8,I8, go6)
Nsample = length(W7);

logW7 = log(W7)';
logW8 = log(W8)';
logI8 = log(I8)';

logGo6 = log(go6)';

A = [ones(Nsample,1), logW7, logW8, logI8];
b = logGo6;

x = A\b;

c = exp(x(1));
a_W7 = x(2);
a_W8 = x(3);
a_I8 = x(4);
