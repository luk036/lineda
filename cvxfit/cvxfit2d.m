% This 2d example is taken from MATLAB Spline Toolbox. We try to
% fit the function by a *concave* one.
% Require Spline Toolbox and the CVX package from Stanford University.
%
% References:
%
% [1] Michael S. Floater. A weak condition for the convexity of
% tensor-product Bezier and B-spline surfaces. Advances in Comp. Math.,
% 2:67-80, 1994
%
clear all
x = sort([(0:10)/10,.03 .07, .93 .97]);
y = sort([(0:6)/6,.03 .07, .93 .97]);
[xx,yy] = ndgrid(x,y); 
z = -franke(xx,yy);
z = z + 0.4*rand(size(z));
%% z = (xx-0.5).^4 + 0.5*(yy-0.5).^4;
%% subplot(2,2,1);
%% mesh(x,y,z.'), view(150,50)
%% title('Data from perturbed Franke function')

% In this example, knot sequence is specified explicitly.
kx = 4; knotsx = augknt(0:.2:1,kx);
ky = 3; knotsy = augknt([0,.25,.5,.75,1],ky);

sp3=spap2({knotsx,knotsy},[kx ky],{x,y},z);
subplot(2,2,1);
fnplt(fncmb(sp3,-1)); view(150,50);
title('Data fitting by spap2');

subplot(2,2,2);
pp2dir = fndir(sp3,eye(2));
grads = reshape(fnval(pp2dir,[xx(:) yy(:)].'),...
    [2,length(x),length(y)]);
contour(xx,yy,reshape(fnval(sp3,[xx(:) yy(:)].'),...
    [length(x),length(y)]));
hold on
quiver(xx,yy,squeeze(grads(1,:,:)),squeeze(grads(2,:,:)))
title('Contour of the B-spline''s surface');
hold off

eps = 1e-6;
knots = fnbrk(sp3,'k'); 
coefs = squeeze(fnbrk(sp3,'c')); % coefficient matrix
knotsx = knots{1};
knotsy = knots{2};

Cx = spcol(knotsx,kx,x); % collocation matrix
Cy = spcol(knotsy,ky,y);
C = kron(Cy, Cx); % Kronecker product

% Make C to be a sparse matrix
C = sparse(C);

%% Check correctness
pp = C\z(:); % QR sol'n
norm(coefs(:) - pp) < eps

sp4 = spcvx2d(knots, [kx, ky], x, y, z);
subplot(2,2,3);
fnplt(fncmb(sp4,-1)); view(150,50);
title('Data fitting by spcvx');

% Check if it is really concave-like
subplot(2,2,4);
pp2dir = fndir(sp4,eye(2));
grads = reshape(fnval(pp2dir,[xx(:) yy(:)].'),...
    [2,length(x),length(y)]);
contour(xx,yy,reshape(fnval(sp4,[xx(:) yy(:)].'),...
    [length(x),length(y)]));
hold on
quiver(xx,yy,squeeze(grads(1,:,:)),squeeze(grads(2,:,:)))
title('Contour of the cvx B-spline''s surface');
hold off
