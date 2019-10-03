% a fake dataset to make the bumps with
n = 30;   % number of points
s_begin = 1;
s_end = 20;
sdkern = 2.0;  % width of kernel
var = 2.0;     % standard derivation
N = 500;  % number of samples
tau = 2.0;    % standard derivation of white noise

% create sites s
s = [s_begin:(s_end-s_begin)/(n-1):s_end]';

Sig = ones(n,n);
for i=1:n-1,
  for j=i+1:n,
    d = s(j) - s(i);
    Sig(i,j) = exp(-0.5*(d'*d)/sdkern/sdkern/2);
    Sig(j,i) = Sig(i,j);
  end
end

[U,Lambda,V] = svd(Sig);
A = U*sqrt(Lambda);

Ys = [];

for k=1:N
  x = var * randn(n,1);
  y = A*x;
  y = y + tau*randn(n,1);
  Ys = [Ys, y(:)];
end

%plot(s,Ys)
%hold off
Y = (Ys*Ys')./N;
