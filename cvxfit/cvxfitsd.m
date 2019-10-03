% This example is taken from MATLAB Spline Toolbox. We describe how to
% handle scattered data.

clear all
n = 65; t = linspace(0,2*pi,n+1); 
x = rand(2,n);
%%x = [x, [0;0], [0;1], [1;0], [1;1]];

y = (x(1,:)+.5).^2 + (x(2,:)+.5).^2;
y = y + 2*(rand(size(y))-.5);
%% subplot(2,2,1);
%% plot3(x(1,:),x(2,:),y,'wo','markerfacecolor','k')
%% title('Data from MATLAB')

subplot(2,2,1);
st = tpaps(x,y);
fnplt(st); hold on
plot3(x(1,:),x(2,:),y,'wo','markerfacecolor','k')
title('Fitting by tpaps');
hold off
%%view(2)

xx = linspace(0,1,20);
yy = linspace(0,1,20);
[xxx,yyy] = ndgrid(xx,yy);
subplot(2,2,2);
contour(xxx,yyy,reshape(fnval(st,[xxx(:) yyy(:)].'),...
    [length(xx(:)),length(yy(:))]));
title('Contour of the thin-plate spline''s surface');


% In this example, knot sequence is specified explicitly.
kx = 4; knotsx = augknt([0:.2:1],kx);
ky = 4; knotsy = augknt([0:.2:1],ky);

eps = 1e-6;

xx = aveknt(knotsx,kx)';  % site location
yy = aveknt(knotsy,ky)';
nx = size(xx,1);
ny = size(yy,1);

sp4 = spcvxsd({knotsx,knotsy}, [kx,ky], x, y);
subplot(2,2,3);
fnplt(sp4); hold on;
plot3(x(1,:),x(2,:),y,'wo','markerfacecolor','k')
title('Fitting by spcvx');
hold off
%% view(2);
% Check
%%isempty(find(diff(diff(p(:,3))./diff(xx))' < eps))
%%isempty(find(diff(diff(p(3,:))./diff(yy)') < eps))

xx = linspace(0,1,20);
yy = linspace(0,1,20);
[xxx,yyy] = ndgrid(xx,yy);
subplot(2,2,4);
pp2dir = fndir(sp4,eye(2));
grads = reshape(fnval(pp2dir,[xxx(:) yyy(:)].'),...
    [2,length(xx(:)),length(yy(:))]);
contour(xxx,yyy,reshape(fnval(sp4,[xxx(:) yyy(:)].'),...
    [length(xx(:)),length(yy(:))]));
title('Contour of the cvx B-spline''s surface');

