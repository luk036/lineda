function [c,a_W5,a_W8,a_I8]=...
        mono_go3(W5,W8,I8, go3)
Nsample = length(W5);

logW5 = log(W5)';
logW8 = log(W8)';
logI8 = log(I8)';

logGo3 = log(go3)';

A = [ones(Nsample,1), logW5, logW8, logI8];
b = logGo3;

x = A\b;

c = exp(x(1));
a_W5 = x(2);
a_W8 = x(3);
a_I8 = x(4);
