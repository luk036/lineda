% Synthesize 2D isotropic Gaussian random field data
nx = 10;	 % number of points in x and y direction
ny = 10;
s_beg = [1 1];
s_end = [11 11];

sdkern = 1.5;  % width of kernel (isotropic)
var = 2.0;     % standard derivation
tau = 0.5;     % standard derivation of white noise
N = 500;        % number of samples


% Create sites s in a uniform grid
sx = [s_beg(1):(s_end(1)-s_beg(1))/(nx-1):s_end(1)]';
sy = [s_beg(2):(s_end(2)-s_beg(2))/(ny-1):s_end(2)]';
[xx,yy] = ndgrid(sx, sy); 
s = [xx(:), yy(:)];
n = size(s,1);	      % equal to nx * ny

Sig = ones(n,n);
for i=1:n-1,
  for j=i+1:n,
    d = s(j,:)' - s(i,:)';
    corr1 = exp(-0.5*(d'*d)/(sdkern*sdkern)/2);
    corr2 = exp(-0.5*sqrt(d'*d)/sdkern);
    Sig(i,j) = max([corr1, corr2]);
    %%Sig(i,j) = exp(-0.5*(d'*d)/(sdkern*sdkern)/2);
    Sig(j,i) = Sig(i,j);
  end
end

%%[U,Lambda,V] = svd(Sig);
%%A = U*sqrt(Lambda);
A = sqrtm(Sig);

Ys = [];
for k=1:N
  x = var * randn(n,1);
  y = A*x + tau*randn(n,1);
  Ys = [Ys, y(:)];
end

Y = cov(Ys',1);
