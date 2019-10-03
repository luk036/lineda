% a fake dataset to make the bumps with
n = 20;   % number of points
%%m = 28;    % number pf support sites for x(s)
s_begin = 1;
s_end = 10;
sdkern = 0.5;  % width of kernel
var = 2.0;     % standard derivation
tau = 0.1;    % standard derivation of white noise
N = 500;  % number of samples

% create sites s
%%s = [s_begin:(s_end-s_begin)/(n-1):s_end]';
seq = [1:n-1];
seq2 = [0,seq.*(seq+1)/2];
s = s_begin + (s_end-s_begin)/seq2(n)*seq2';

% locations of support points
%%dw = (s_end - s_begin + 4) / (m-1);
%%w = [(s_begin-2):dw:(s_end+2)]';

Sig = ones(n,n);
for i=1:n-1,
  for j=i+1:n,
    d = s(j,:)' - s(i,:)';
    corr1 = exp(-0.5*sqrt(d'*d)/sdkern);
    corr2 = exp(-0.5*(d'*d)/(sdkern*sdkern)/2);
    Sig(i,j) = max([corr1, corr2]);
    %%Sig(i,j) = exp(-0.5*sqrt(d'*d)/(sdkern*sdkern)/2);
    Sig(j,i) = Sig(i,j);
  end
end

%%[U,Lambda,V] = svd(Sig);
%%A = U*sqrt(Lambda);
A = sqrtm(Sig);
Ys = [];

for k=1:N
  x = var * randn(n,1);
  y = A*x;
  y = y + tau*randn(n,1);
  Ys = [Ys, y(:)];
end


Y = cov(Ys',1);
