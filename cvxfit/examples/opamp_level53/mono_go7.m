function [c,a_W7,a_W8,a_I8]=...
        mono_go7(W7,W8,I8, go7)
Nsample = length(W7);

logW7 = log(W7)';
logW8 = log(W8)';
logI8 = log(I8)';

logGo7 = log(go7)';

A = [ones(Nsample,1), logW7, logW8, logI8];
b = logGo7;

x = A\b;

c = exp(x(1));
a_W7 = x(2);
a_W8 = x(3);
a_I8 = x(4);
