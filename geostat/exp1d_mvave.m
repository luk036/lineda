% a fake dataset to make the bumps with
n = 30;   % number of points
m = 28;    % number pf support sites for x(s)
s_begin = 1;
s_end = 20;
sdkern = 2.0;  % width of kernel
var = 1.0;     % standard derivation
tau = 1.0;    % standard derivation of white noise
N = 500;  % number of samples

% create sites s
s = [s_begin:(s_end-s_begin)/(n-1):s_end]';

% locations of support points
dw = (s_end - s_begin + 4) / (m-1);
w = [(s_begin-2):dw:(s_end+2)]';

y = zeros(n,1);
% moving average method
Ys = [];

for k=1:N
  x = var * randn(m,1);
  for i=1:n
    s2 = s(i) - w;
    kern = exp(-0.5*(s2.*s2)/(sdkern*sdkern));
    y(i) = sum(x .* kern) * dw;
  end
  y = y + tau*randn(n,1);
  Ys = [Ys, y(:)];
end

Y = cov(Ys');
