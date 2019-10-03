close all, clear all

load('sample_wire_sizing_4_400_20080123.mat');
k1 = 3; k2 = 3; k3 = 3; k4 = 3; % Quadratic spline Least-Squares Approx
xR1 = R1*1e-3; xR2 = R2*1e-3; xC1 = C1*1e12; xC2 = C2*1e12;
yTr = risetime*1e9;

knots_R1 = augknt(linspace(min(xR1),max(xR1),8)',k1);
knots_R2 = augknt(linspace(min(xR2),max(xR2),8)',k2);
knots_C1 = augknt(linspace(min(xC1),max(xC1),8)',k3);
knots_C2 = augknt(linspace(min(xC2),max(xC2),8)',k4);

%autogen_spcvx_H(4);
sp_rise_auto = spcvx4d_gen_H({knots_R1,knots_R2,knots_C1,knots_C2},...
                             [k1 k2 k3 k4],[xR1' xR2' xC1' xC2'],yTr);
[Qnew,Q,B,C] = rank_1([xR1;xR2;xC1;xC2],yTr',1000);

for ii = 1:length(R1),
    Tr_cvx(1,ii) = fnval(sp_rise_auto,...
                         [R1(ii)*1e-3;R2(ii)*1e-3;C1(ii)*1e12;C2(ii)*1e12]);
    Tr_rnk(1,ii) = fit_rank_1(Qnew,Q,B,C,...
                              [R1(ii)*1e-3;R2(ii)*1e-3;C1(ii)*1e12;C2(ii)*1e12]);
end

error_cvx = norm(Tf_cvx - risetime*1e9)
error_rank = norm(Tf_rnk - risetime*1e9)

error_cvx_100 = abs((Tf_cvx - risetime*1e9)./(risetime*1e9))*100;
error_rnk_100 = abs((Tf_rnk - risetime*1e9)./(risetime*1e9))*100;

AE_cvx = mean(error_cvx_100);
SE_cvx = std(error_cvx_100);

AE_rnk = mean(error_rnk_100);
SE_rnk = std(error_rnk_100);

fprintf('Convex: AE=%6.4f%%, SE=%6.4f%%\n',AE_cvx,SE_cvx);
fprintf('Rank-1: AE=%6.4f%%, SE=%6.4f%%\n',AE_rnk,SE_rnk);


