% 2d example

x = sort([(0:10)/10,.03 .07, .93 .97]);
y = sort([(0:6)/6,.03 .07, .93 .97]);
[xx,yy] = ndgrid(x,y); z = franke(xx,yy);
z = z + 0.4*rand(size(z));
subplot(2,2,1);
mesh(x,y,z.'), view(150,50)
title('Data from Franke function')

kx = 4; knotsx = augknt(0:.2:1,kx);
ky = 3; knotsy = augknt([0,.25,.5,.75,1],ky);

sp3=spap2({knotsx,knotsy},[kx ky],{x,y},z);
subplot(2,2,2);
fnplt(sp3); view(150,50);
title('The original least squares approximation');

eps = 1e-6;
knots = fnbrk(sp3,'k');
zz = z(:);
coefs = squeeze(fnbrk(sp3,'c')); % coefficient matrix
knotsx = knots{1};
knotsy = knots{2};

A = [];
nx = size(coefs,1);
ny = size(coefs,2);
Ix = speye(nx);
Iy = speye(ny);

%% Construct constraint: \partial_{xx} <= 0
xx = aveknt(knotsx,kx)';  % site location
ncx = size(xx,1);
%dd = diff(xx);
%Ax = spdiags([-dd(2:ncx-1), dd(2:ncx-1)+dd(1:ncx-2), -dd(1:ncx-2)], ...
%     [0:2], ncx-2,ncx);
ddx1 = ones(ncx-1,1)./diff(xx);  % divided difference
Adx1 = spdiags([-ddx1 ddx1], [0:1], ncx-1, ncx);
ddx2 = ones(ncx-2,1)./(xx(3:ncx) - xx(1:ncx-2));  % divided difference
Adx2 = spdiags([-ddx2 ddx2], [0:1], ncx-2, ncx-1);
Axx = Adx2*Adx1;
A = [A; kron(Iy, Axx)]; % append 

%% Construct constraint: \partial_{yy} <= 0
yy = aveknt(knotsy,ky)';
ncy = size(yy,1);
%dd = diff(yy);
%Ay = spdiags([-dd(2:ncy-1), dd(2:ncy-1)+dd(1:ncy-2), -dd(1:ncy-2)], ...
%     [0:2], ncy-2,ncy);
ddy1 = ones(ncy-1,1)./diff(yy);  % divided difference
Ady1 = spdiags([-ddy1 ddy1], [0:1], ncy-1, ncy);
ddy2 = ones(ncy-2,1)./(yy(3:ncy) - yy(1:ncy-2));  % divided difference
Ady2 = spdiags([-ddy2 ddy2], [0:1], ncy-2, ncy-1);
Ayy = Ady2*Ady1;
A = [A; kron(Ayy, Ix)]; % append 

b = zeros(size(A,1),1);


%% Construct constraint: \partial_{xy}
ddx = ones(ncx-1,1)./diff(xx);  % divided difference
Ad1 = spdiags([-ddx ddx], [0:1], ncx-1, ncx);
ddy = ones(ncy-1, 1)./diff(yy);
Ad2 = spdiags([-ddy ddy], [0:1], ncy-1, ncy);
Axy = kron(Ad2, Ad1);
%beq = zeros(size(Axy,1),1);

Cx = spcol(knotsx,kx,x); % collocation matrix
Cy = spcol(knotsy,ky,y);
C = kron(Cy, Cx);

%% Check correctness
pp = C\zz; % least square sol'n
norm(coefs(:) - pp) < eps

% Objective: min (zz - C*pp)'*(zz - C*pp)
%%H = C'*C;
%%f = -zz'*C;

options = optimset('MaxIter', 1000);

% Least squares approximation with linear inequality constraints 
% (not sufficient) 

%pp = quadprog(H,f, full(A), b, [], [], [], [], pp, options);
n = size(pp(:));
cvx_begin
  variable pp(n)
  minimize( norm(C*pp - zz) );
  subject to
    A * pp <= 0;
cvx_end

p = reshape(pp, size(coefs));
sp4 = spmak({knotsx,knotsy},p,size(coefs));
subplot(2,2,3);
fnplt(sp4); view(150,50);
title('Convex fitting method 1');
%% Check
diff(diff(p(:,3))./diff(xx))' < eps
diff(diff(p(3,:))./diff(yy)') < eps

% Constraint: \partial_xx <= +/- \partial_xy
A = [A;  kron(Ad2,Ad1(1:ncx-2,:)) + kron(speye(ncy-1,ncy), Axx)];
A = [A; -kron(Ad2,Ad1(1:ncx-2,:)) + kron(speye(ncy-1,ncy), Axx)];

% Constraint: \partial_yy <= +/- \partial_xy
A = [A;  kron(Ad2(1:ncy-2,:), Ad1) + kron(Ayy, speye(ncx-1,ncx))];
A = [A; -kron(Ad2(1:ncy-2,:), Ad1) + kron(Ayy, speye(ncx-1,ncx))];
b = zeros(size(A,1),1);

% Least squares approximation with additional linear constraints
% (sufficient but not necessary)
%pp = quadprog(H,f, full(A), b, [], [], [], [], pp, options);
cvx_begin
  variable pp(n)
  minimize( norm(C*pp - zz) );
  subject to
    A * pp <= 0;
cvx_end

p = reshape(pp, size(coefs));
sp4 = spmak({knotsx,knotsy},p,size(coefs));
subplot(2,2,4);
fnplt(sp4); view(150,50);
title('Convex fitting method 2');
%% Check
diff(diff(p(:,3))./diff(xx))' < eps
diff(diff(p(3,:))./diff(yy)') < eps
