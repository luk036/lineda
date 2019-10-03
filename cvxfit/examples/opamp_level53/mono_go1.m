function [c,a_W,a_I]=mono_go(W,I, go)
Nsample = length(W);

logW = log(W)';
logI = log(I)';

logGo = log(go)';

A = [ones(Nsample,1), logW, logI];
b = logGo;

x = A\b;

c = exp(x(1));
a_W = x(2);
a_I = x(3);
