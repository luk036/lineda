% Example of univariate case
clear all; close all;
k = 3;      % B-spline's order: 2-linear, 3-quadratic, 4-cubic, 5-quartic
npp = 16;   % number of piecewise polynomials, should <= length(x)-k+1
            % It could be less than the number of data points.

% Construct an example
%%x = linspace(0,2*pi,51)';
%%y = cos(x) + .2*(rand(size(x))-.5);
x = [0.5:0.5:10]';
y = abs(sin(x));
%y = y + .5*(rand(size(x))-.5);  % add some noise
plot(x,y,'x');
hold on

% Least square fitting using B-spline. In this example, we let the 
% toolbox determines the knot sequences for us. See also MATLAB's
% Spline Toolbox for other choices.
sp = spap2(npp,k,x,y);   % least squares approximation
fnplt(sp,'b--')          % function plot

% Extract the information of resulting B-spline
[coefs,knots] = fnbrk(sp, 'coefs', 'knots');
C = spcol(knots, k, x);  % obtain collocation matrix
xx = aveknt(knots,k)';   % get the site locations
nc = size(xx,1);         % number of sites (should equal to npp+k-1)

% Check correctness
eps = 1e-6;
p = C\y; % QR sol'n
collocation_matrix_construction_is_ok = ...
  norm(coefs(:) - p) < eps


sp2 = spnonneg1d(knots, k, x, y);
fnplt(sp2,'r');
%xlabel('x value','FontWeight','bold');
%ylabel('y value','FontWeight','bold');
legend('data points','Fitting by spap2','Fitting by spnonneg')
%title('Nonnegative Fitting','FontSize',12,'FontWeight','bold')
hold off

%% Check 
%% resulting_B_spline_is_convex = ...
%%  isempty(find(diff(diff(fnval(sp2,xx))./diff(xx))' < 0))
