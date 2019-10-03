close all, clear all

load('inv_Tfall_samples.mat');
k1 = 3; k2 = 3; % Quadratic spline Least-Squares Approx
xWn = Wn_nogrid'*1e6; xWp = Wp_nogrid'*1e6; yTf = falltime_nogrid*1e9;
knots_Wn = augknt(linspace(min(xWn),max(xWn),8)',k1);
knots_Wp = augknt(linspace(min(xWp),max(xWp),8)',k2);

[WnWn,WpWp]=meshgrid(Wn_li*1e6,Wp_li*1e6);
figure, mesh(WnWn,WpWp,falltime_grid'*1e9);  view(135,30); axis tight;
xlabel('Wn \times 10^6'); ylabel('Wp \times 10^6');
zlabel('Tfall \times 10^9');
%saveas('INV_Tfall_org','pdf');

sp_fall_auto = spcvx2d_gen_H({knots_Wn,knots_Wp},[k1 k2],[xWn xWp],yTf);
figure
fnplt(sp_fall_auto);  view(135,30); axis tight;
xlabel('Wn \times 10^6'); ylabel('Wp \times 10^6');
zlabel('Tfall \times 10^9');
%saveas('INV_Tfall_cvx','pdf');


