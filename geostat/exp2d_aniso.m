% a fake dataset to make the bumps with
% 2d example, anisotopic
n1 = 20;  % number of points in first dimension
n2 = 24;  % number of points in second dimension
n = n1*n2;

m1 = 7;   % number pf support sites for x1(s)
m2 = 8;   % number of support sites for x2(s)
sdk1 = 1.0;  % width of kernel in first dimension
sdk2 = 1.3;  % width of kernel in second dimension
var = 1.2;     % ???
N = 100;  % number of samples

% create sites s
s1 = [1:19/(n1-1):20]';
s2 = [1:23/(n2-1):24]';
s = ndgrid(s1,s2); 


% locations of support points
w1 = [(1-2):23/(m1-1):(20+2)]';
w2 = [(1-2):25/(m2-1):(22+2)]';
[ww1,ww2] = ndgrid(w1,w2);

z = zeros(n1,n2);
% moving average method
Ys = [];

for k=1:N,
  for i=1:n1,
    for j=1:n2,
      x = var * randn(m1,m2);
      [xx1,xx2] = ndgrid(s1(i) - w1, s2(j) - w2);
      kern = exp(-0.5*(xx1.*xx1/(sdk1*sdk1) + xx2.*xx2/(sdk2*sdk2)));
      z(i,j) = sum(x(:) .* kern(:));
    end
  end
  Ys = [Ys, z(:)];
end

mesh(s1,s2,z.');
%%hold off
Y = (Ys*Ys')./N;
