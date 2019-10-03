% To approximate the transition time surface
% by convex B-spline, will call spcvx2d therefore
% addpath(lineda/cvxfit) if necessary
function sp_fall_e_cvx=FallCvxFit(kx,ky,xe6,ye6,ze9_fall,PLOT_TRUE)

sp_fall_e = spap2({kx,ky},[kx ky],{xe6,ye6},ze9_fall);
tol = 1e-6;
knots = fnbrk(sp_fall_e,'k'); 
coefs = squeeze(fnbrk(sp_fall_e,'c')); % coefficient matrix
knotsx = knots{1}; knotsy = knots{2};

Cx = spcol(knotsx,kx,xe6); % collocation matrix
Cy = spcol(knotsy,ky,ye6);
C = kron(Cy, Cx); % Kronecker product

% Make C to be a sparse matrix
C = sparse(C);

% function call, addpath lineda/cvxfit if never done before
sp_fall_e_cvx = spcvx2d(knots, [kx, ky], xe6, ye6, ze9_fall);
zfit_fall_cvx = fnval(sp_fall_e_cvx,{xe6,ye6});
if PLOT_TRUE
    figure, mesh(WnWn,WpWp,falltime'); view(45,30)
      xlabel('Wn'), ylabel('Wp'), zlabel('Fall time')
      title('Fall time - Original')
    figure, mesh(WnWn,WpWp,1e-9*zfit_fall'); view(45,30)
      xlabel('Wn'), ylabel('Wp'), zlabel('Fall time')
      title('Fall time - General Posynomial')
    figure, mesh(WnWn,WpWp,1e-9*zfit_fall_cvx'); view(45,30);
      xlabel('Wn'), ylabel('Wp'), zlabel('Fall time')
      title('Fall time - CVX fit');
end

