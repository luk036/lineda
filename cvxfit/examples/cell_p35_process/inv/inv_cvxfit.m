close all, clear all

%load('inv_Tfall_samples.mat');
% load('sample_inv_20080128.mat'); % most recently used before vacation
load('sample_inv_p18_20080220.mat');

k1 = 3; k2 = 3; % Quadratic spline Least-Squares Approx
xWn = Wn*1e6; xWp = Wp*1e6; yTf = risetime*1e9;
knots_Wn = augknt(linspace(min(xWn),max(xWn),8)',k1);
knots_Wp = augknt(linspace(min(xWp),max(xWp),8)',k2);


st = tpaps([xWn;xWp],yTf);
figure, fnplt(st);  view(135,30); axis tight;
xlabel('Wn \times 10^6'); ylabel('Wp \times 10^6');
zlabel('Tfall \times 10^9');
%saveas('INV_Tfall_org','pdf');
tic;
%sp_fall_auto = spcvx2d_gen_H({knots_Wn,knots_Wp},[k1 k2],[xWn' xWp'],yTf);
sp_fall_auto = spcvxsd({knots_Wn,knots_Wp},[k1 k2],[xWn; xWp],yTf);
t_cvx = toc;
tic;
[Qnew,Q,B,C] = rank_1([xWn;xWp],yTf',1000);
t_rnk = toc;
% --------------------------------------------------------
tic;
posy_coef = fit_posy({xWn,xWp},yTf,2);
t_psy = toc;
% --------------------------------------------------------

Wn_li = linspace(1.1*min(Wn),0.9*max(Wn),20); 
Wp_li = linspace(1.1*min(Wp),0.9*max(Wp),19);
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
mesh(WnWn(2:end-1,2:end-1),WpWp(2:end-1,2:end-1),...
     falltime_cvx(2:end-1,2:end-1)); 
view(135,30); axis tight;
xlabel('Wn \times 10^6'); ylabel('Wp \times 10^6');
zlabel('CVX: Tfall \times 10^9');
%saveas('INV_Tfall_cvx','pdf');

figure, 
mesh(WnWn(2:end-1,2:end-1),WpWp(2:end-1,2:end-1),...
     falltime_rank(2:end-1,2:end-1)); 
view(135,30); axis tight;
xlabel('Wn \times 10^6'); ylabel('Wp \times 10^6');
zlabel('Rank-1: Tfall \times 10^9');

figure, 
mesh(WnWn(2:end-1,2:end-1),WpWp(2:end-1,2:end-1),...
     falltime_pos(2:end-1,2:end-1)); 
view(135,30); axis tight;
xlabel('Wn \times 10^6'); ylabel('Wp \times 10^6');
zlabel('Posy: Tfall \times 10^9');

for ii = 1:length(xWn),
    Tf_cvx(1,ii) = fnval(sp_fall_auto,[xWn(ii);xWp(ii)]);
    Tf_rnk(1,ii) = fit_rank_1(Qnew,Q,B,C,[xWn(ii);xWp(ii)]);
    Tf_psy(1,ii) = fposy_val({xWn(ii),xWp(ii)},posy_coef);
end

error_cvx_100 = abs((Tf_cvx - yTf)./(yTf))*100;
error_rnk_100 = abs((Tf_rnk - yTf)./(yTf))*100;
error_psy_100 = abs((Tf_psy - yTf)./(yTf))*100;

AE_cvx = mean(error_cvx_100);
SE_cvx = std(error_cvx_100);

AE_rnk = mean(error_rnk_100);
SE_rnk = std(error_rnk_100);

AE_psy = mean(error_psy_100);
SE_psy = std(error_psy_100);

fprintf('Convex: AE=%6.4f%%, SE=%6.4f%%\n',AE_cvx,SE_cvx);
fprintf('Posynl: AE=%6.4f%%, SE=%6.4f%%\n',AE_psy,SE_psy);
fprintf('Rank-1: AE=%6.4f%%, SE=%6.4f%%\n',AE_rnk,SE_rnk);

fprintf('Time Convex: %6.4f\n',t_cvx);
fprintf('Time Posynl: %6.4f\n',t_psy);
fprintf('Time Rank-1: %6.4f\n',t_rnk);



