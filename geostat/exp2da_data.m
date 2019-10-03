% Synthesize 2D anisotropic Gaussian random field data
% References:
%  [1] ???

nx = 16;       % number of points in x and y direction
ny = 9;
s_beg = [1 1];
s_end = [16 9];

sdkern = 2.0;  % width of kernel
var = 2.0;     % standard derivation
N = 50;        % number of samples
tau = 1.0;     % standard derivation of white noise

% Geometric anisotropic parameters
alpha = 2;     % scaling factor
theta = pi/3;  % angle

% Create sites s in a uniform grid
sx = [s_beg(1):(s_end(1)-s_beg(1))/(nx-1):s_end(1)]';
sy = [s_beg(2):(s_end(2)-s_beg(2))/(ny-1):s_end(2)]';
[xx,yy] = ndgrid(sx, sy); 
s = [xx(:), yy(:)];
n = size(s,1);	      % equal to nx * ny

% Geometric anisotropic parameters
Sc = [1   0;
      0   alpha];
R = [sin(theta) cos(theta);
     -cos(theta) sin(theta)];
T = Sc*R;

Sig = ones(n,n);
for i=1:n-1,
  for j=i+1:n,
    dt = s(j,:)' - s(i,:)';
    d = T*dt;  % become isotropic after the location transformation
    Sig(i,j) = exp(-0.5*(d'*d)/(sdkern*sdkern)/2);
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
