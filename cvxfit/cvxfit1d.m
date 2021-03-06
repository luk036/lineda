% Example of univariate case
clear all; close all;
k = 3;      % B-spline's order: 2-linear, 3-quadratic, 4-cubic, 5-quartic
npp = 16;   % number of piecewise polynomials, should <= length(x)-k+1
            % It could be less than the number of data points.

% Construct an example
%%x = linspace(0,2*pi,51)';
%%y = cos(x) + .2*(rand(size(x))-.5);
x = [1:0.2:5]';
y = -atan(x);
y = y + .15*(rand(size(x))-.5);  % add some noise
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

%%% % Formulate convex fitting problem as convex quadratic programming 
%%% % with linear inequality contraints
%%% cvx_quiet(true);
%%% cvx_begin
%%%   variable p(nc);
%%%   minimize( norm(C*p - y));
%%%   subject to
%%%     for i=2:nc-1
%%%       % Enforce second divided difference >= 0
%%%       (p(i+1) - p(i))/(xx(i+1) - xx(i)) >= (p(i) - p(i-1))/(xx(i) - xx(i-1));
%%%     end
%%% cvx_end
%%% 
%%% %plot(xx,p,'o');

%%% sp2 = spmak(knots,p,size(p')); % construct the cvx B-spline
sp2 = spcvx1d(knots, k, x, y);
fnplt(sp2,'r');
plot(xx,p,'ko')
%xlabel('x value','FontWeight','bold');
%ylabel('y value','FontWeight','bold');
legend('data points','Fitting by spap2','Fitting by spcvx')
%title('Convex Fitting','FontSize',12,'FontWeight','bold')
hold off

%% Check 
resulting_B_spline_is_convex = ...
  isempty(find(diff(diff(fnval(sp2,xx))./diff(xx))' < 0))
