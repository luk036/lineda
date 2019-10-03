function [c,a_W1,a_W5,a_W6,a_W7,a_W8,a_I8]=mono_gain(W1,W5,W6,W7,W8,I8, gain)
Nsample = length(W1);

logW1 = log10(W1)';
logW5 = log10(W5)';
logW6 = log10(W6)';
logW7 = log10(W7)';
logW8 = log10(W8)';
logI8 = log10(I8)';

%logGain = log(10.^(gain/20))';
logGain = (gain/20)';

A = [ones(Nsample,1), logW1, logW5, logW6, logW7, logW8, logI8];
b = logGain;

x = A\b;

c = 10^(x(1));
a_W1 = x(2);
a_W5 = x(3);
a_W6 = x(4);
a_W7 = x(5);
a_W8 = x(6);
a_I8 = x(7);

