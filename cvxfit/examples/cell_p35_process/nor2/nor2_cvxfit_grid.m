close all, clear all

load('sample_nor2_grid_20080125.mat');
k1 = 3; k2 = 3; % Quadratic spline Least-Squares Approx
xWn = Wn*1e6; xWp = Wp*1e6; yTf_grid = falltime*1e9; 
xWn_v = Wn_v*1e6; xWp_v = Wp_v*1e6; yTf_v = falltime_v*1e9;
knots_Wn = augknt(xWn,k1);
knots_Wp = augknt(xWp,k2);

sp_ls = spap2({8,8},[k1 k2],{xWn,xWp},yTf_grid);
figure, fnplt(sp_ls);  view(135,30); axis tight;
xlabel('Wn \times 10^6'); ylabel('Wp \times 10^6');
zlabel('Tfall \times 10^9');
%saveas('INV_Tfall_org','pdf');
print -f1 -depsc 'nor2_Tfall_spap2.eps';

sp_fall_auto = spcvx2d_gen_H(sp_ls.knots,[k1 k2],[xWn_v' xWp_v'],yTf_v);
[Qnew,Q,B,C] = rank_1([xWn_v;xWp_v],yTf_v,1000);
% --------------------------------------------------------
posy_coef = fit_posy({xWn_v,xWp_v},yTf_v,2);
% --------------------------------------------------------

Wn_li = linspace(3e-6,9e-6,20); Wp_li = linspace(3e-6,9e-6,19);
[WnWn,WpWp]=meshgrid(Wn_li*1e6,Wp_li*1e6);
[l,m] = size(WnWn);
for ii = 1:l,
    for jj = 1:m,
        falltime_cvx(ii,jj) = fnval(sp_fall_auto,[WnWn(ii,jj);WpWp(ii,jj)]);
        falltime_rank(ii,jj) = fit_rank_1(Qnew,Q,B,C,...
                                        [WnWn(ii,jj); WpWp(ii,jj)]);
        falltime_pos(ii,jj) = fposy_val({WnWn(ii,jj),WpWp(ii,jj)},posy_coef);
    end
end
figure, 
mesh(WnWn,WpWp,falltime_cvx); view(135,30); axis tight;
xlabel('Wn \times 10^6'); ylabel('Wp \times 10^6');
zlabel('CVX: Tfall \times 10^9');
%saveas('INV_Tfall_cvx','pdf');
print -f2 -depsc 'nor2_Tfall_cvx.eps';

figure, 
mesh(WnWn,WpWp,falltime_rank); view(135,30); axis tight;
%hold on; plot3(xWn,xWp,yTf,'.');
xlabel('Wn \times 10^6'); ylabel('Wp \times 10^6');
zlabel('Rank-1: Tfall \times 10^9');
print -f3 -depsc 'nor2_Tfall_rnk.eps';

figure, 
mesh(WnWn,WpWp,falltime_pos); view(135,30); axis tight;
xlabel('Wn \times 10^6'); ylabel('Wp \times 10^6');
zlabel('Posynomial: Tfall \times 10^9');
print -f4 -depsc 'nor2_Tfall_pos.eps';

for ii = 1:length(Wn_v),
    Tf_cvx(1,ii) = fnval(sp_fall_auto,[Wn_v(ii);Wp_v(ii)]*1e6);
    Tf_rnk(1,ii) = fit_rank_1(Qnew,Q,B,C,[Wn_v(ii);Wp_v(ii)]*1e6);
    Tf_psy(1,ii) = fposy_val({Wn_v(ii)*1e6,Wp_v(ii)*1e6},posy_coef);
end

% error_cvx = norm(Tf_cvx - falltime_v*1e9);
% error_rank = norm(Tf_rnk - falltime_v*1e9);

error_cvx_100 = abs((Tf_cvx - falltime_v*1e9)./(falltime_v*1e9))*100;
error_rnk_100 = abs((Tf_rnk - falltime_v*1e9)./(falltime_v*1e9))*100;
error_psy_100 = abs((Tf_psy - falltime_v*1e9)./(falltime_v*1e9))*100;

AE_cvx = mean(error_cvx_100);
SE_cvx = std(error_cvx_100);

AE_rnk = mean(error_rnk_100);
SE_rnk = std(error_rnk_100);

AE_psy = mean(error_psy_100);
SE_psy = std(error_psy_100);

fprintf('Convex: AE=%6.4f%%, SE=%6.4f%%\n',AE_cvx,SE_cvx);
fprintf('Posynl: AE=%6.4f%%, SE=%6.4f%%\n',AE_psy,SE_psy);
fprintf('Rank-1: AE=%6.4f%%, SE=%6.4f%%\n',AE_rnk,SE_rnk);

save('fit_result_nor2_grid_080126.mat','sp_ls','sp_fall_auto',...
     'Qnew','Q','B','C','posy_coef');


