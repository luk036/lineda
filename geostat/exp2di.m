I = imread('luk_test.png');
I2 = im2double(I);
[nx, ny, dummy] = size(I2); % dummy = 3
s_beg = [0, 0];
s_end = [nx-1, ny-1];

sdkern = 4;  % width of kernel (isotropic)
var = 0.2;     % standard derivation
tau = 0.1;     % standard derivation of white noise
N = 4;        % number of samples

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
    %%corr1 = exp(-0.5*(d'*d)/(sdkern*sdkern)/2);
    %%corr2 = exp(-0.5*sqrt(d'*d)/sdkern);
    %%Sig(i,j) = max([corr1, corr2]);
    Sig(i,j) = exp(-0.5*(d'*d)/(sdkern*sdkern)/2);
    Sig(j,i) = Sig(i,j);
  end
end

disp('Performing sqrtm ...');

%%[U,Lambda,V] = svd(Sig);
%%A = U*sqrt(Lambda);
A = real(sqrtm(Sig));

disp('Performing plot ...');
Ys = [];
for k=1:N
  x = var * randn(n,1);
  y = A*x + tau*randn(n,1);
  Ys = [Ys, y(:)];
end

figure
for i=1:4,
  subplot(2,2,i);
  Yi = reshape(Ys(:,i),nx, ny);
  I3(:,:,1) = I2(:,:,1) + Yi;
  I3(:,:,2) = I2(:,:,2) + Yi;
  I3(:,:,3) = I2(:,:,3) + Yi;
  imshow(I3);
end
%
%% Y = cov(Ys',1);
