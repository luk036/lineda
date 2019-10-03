function [c,a_W1,a_W3,a_W5,a_W6,a_W7,a_W8,a_I8,a_Cc]=...
        mono_bdw(W1,W3,W5,W6,W7,W8,I8,Cc, bdw)
Nsample = length(W1);

logW1 = log(W1)';
logW3 = log(W3)';
logW5 = log(W5)';
logW6 = log(W6)';
logW7 = log(W7)';
logW8 = log(W8)';
logI8 = log(I8)';
logCc = log(Cc)';

logBdw = log(bdw)';

A = [ones(Nsample,1), logW1, logW3, logW5, logW6, logW7, logW8, logI8, logCc];
b = logBdw;

x = A\b;

c = exp(x(1));
a_W1 = x(2);
a_W3 = x(3);
a_W5 = x(4);
a_W6 = x(5);
a_W7 = x(6);
a_W8 = x(7);
a_I8 = x(8);
a_Cc = x(9);

