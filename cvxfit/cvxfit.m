% 1d example
clear all; close all;

k = 3;                % 2-linear, 3-quadratic, 4-cubic
ncp = 6;              % number of control points, should <= length(x)-k+1
x = [1:0.5:5]';

if ncp > length(x)-k+1
    disp(sprintf('The number of polynomial pieces to be used must be positive'));
    disp(sprintf('but not larger than length(x)-k+1 = %d.', length(x)-k+1));
    return
end

y = -atan(x);
plot(x,y,'c:'), hold on         % original arctan curve
y = y + 0.1*randn(length(y),1); % add some noise
plot(x,y,'bx');

sp = spap2(ncp,k,x,y);   % least squares spline approximation
fnplt(sp,'b--')          % function plot
[coefs,knots] = fnbrk(sp, 'coefs', 'knots');
C = spcol(knots, k, x);  % obtain collocation matrix
xx = aveknt(knots,k)';   % get the site locations
nc = size(xx,1);         % number of sites (should equal to ncp+k-1)

eps = 1e-6;

%% Check correctness
p = C\y; % least square sol'n
norm(coefs(:) - p) < eps
n = size(coefs(:));

% Objective: min (y - C*p)'*(y - C*p)
H = C'*C;
f = -y'*C;

% Enforce second divided difference >= 0
dd = diff(xx);
A = spdiags([-dd(2:nc-1), dd(2:nc-1)+dd(1:nc-2), -dd(1:nc-2)], ...
            [0:2], nc-2,nc);
b = zeros(nc-2,1);

% min ||y - C p||2
% s.t. convex constraints
%p = quadprog(H,f, full(A), b);  
cvx_begin
  variable p(n);
  minimize( norm(C*p - y));
  subject to
    A * p <= 0;
cvx_end

plot(xx,p,'o');

sp2 = spmak(knots,p,size(p'));
fnplt(sp2,'r'); hold off
xlabel('x value','FontWeight','bold');
ylabel('y value','FontWeight','bold');
legend('arctan curve without noise','arctan points with noise','B-spline',...
       'control points','B-spline cvx')
title('Convex Fitting','FontSize',12,'FontWeight','bold')

%% Check 
-diff(diff(p)./diff(xx))' < eps

figure(2)
fnplt(sp), hold on
fnplt(fnder(sp,1),'r:')
fnplt(fnder(sp,2),'g:')
hold off
legend('spline','1st derivative','2nd derivative')
title('Least-squares spline approximation')
