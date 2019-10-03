close all, clear all

load inverter_Wn30_Wp20_sample.mat;
PLOT_TRUE = 0;

x = Wn; y = Wp; z_fall = falltime; z_rise = risetime;
[WnWn,WpWp] = meshgrid(Wn,Wp);
kx = 3; ky = 3; % Quadratic spline Least-Squares Approx
sp_fall=spap2({kx,ky},[kx ky],{x,y},z_fall);
sp_rise=spap2({kx,ky},[kx ky],{x,y},z_rise);
if PLOT_TRUE
    figure, fnplt(fncmb(sp_fall,1)); view(45,30); grid on,
      xlabel('Wn'), ylabel('Wp'), zlabel('Fall time')
      title('Fall time - LS fit');
    figure, fnplt(fncmb(sp_rise,1)); view(135,30); grid on,
      xlabel('Wn'), ylabel('Wp'), zlabel('Rise time')
      title('Rise time - LS fit');
end

% scaled for sake of computational stability 
xe6 = x*1e6; ye6= y*1e6; ze9_fall = z_fall*1e9; ze9_rise = z_rise*1e9;
p0 = ones(5,1);
options = optimset('Display','iter','TolFun',1e-8,'TolX',1e-3,...
                   'MaxFunEvals',10000,'LargeScale','off');
% Posynomial fit
[p_fall,fval,exitflag] = fminsearch(@(p) fitgp_fall_obj(p,xe6,ye6,ze9_fall),...
                                    p0,options)
[p_rise,fval,exitflag] = fminsearch(@(p) fitgp_rise_obj(p,xe6,ye6,ze9_rise),...
                                    p0,options)
for ii = 1 : length(Wn)
    for jj = 1 : length(Wp)
        zfit_fall(ii,jj) = fitgp_fall(p_fall,xe6(ii),ye6(jj));
        zfit_rise(ii,jj) = fitgp_rise(p_rise,xe6(ii),ye6(jj));
    end
end
if PLOT_TRUE
    figure, mesh(WnWn,WpWp,1e-9*zfit_fall'); view(45,30)
      xlabel('Wn'), ylabel('Wp'), zlabel('Fall time')
      title('Fall time - General Posynomial')
    figure, mesh(WnWn,WpWp,falltime'); view(45,30)
      xlabel('Wn'), ylabel('Wp'), zlabel('Fall time')
      title('Fall time - Original')
    figure, mesh(WnWn,WpWp,1e-9*zfit_rise'); view(135,30)
      xlabel('Wn'), ylabel('Wp'), zlabel('Rise time')
      title('Rise time - General Posynomial')
    figure, mesh(WnWn,WpWp,risetime'); view(135,30)
      xlabel('Wn'), ylabel('Wp'), zlabel('Rise time')
      title('Rise time - Original')
end
% Optimization for a sizing which will minimize fall transition time
% and make Trise equal Tfall
L = .35e-6;
Wn_max = 20*L; Wp_max = 30*L;
width0 = ones(2,1);
lwidth = [L, L]'*1e6;  uwidth = [Wn_max, Wp_max]'*1e6;
options = optimset('Display','iter','TolFun',1e-8,'TolX',1e-3,...
                   'MaxFunEvals',10000,'LargeScale','off');
[width,fval,exitflag] = fmincon(@(width) fitgp_fall(p_fall,width(1),width(2)),...
                                width0, [],[],[],[],lwidth,uwidth,...
                                @(width) optInvcon(p_fall,p_rise,width),options)

% function call, fit the convex B-spline surface
sp_fall_e_cvx = FallCvxFit(kx,ky,xe6,ye6,ze9_fall,PLOT_TRUE)
sp = sp_fall_e_cvx;
for ii = 1 : 5
[width,fval,exitflag] = fmincon(@(width) optCvxObj(width),width0,[],[],[],[],lwidth,uwidth,...
                                @(width) optCvxCon(sp,width),options)
fall_time_opt = fnval(sp, {width(1), width(2)})
width0 = width;

sp = sample_center(width0);
lwidth = [width(1)*.8  width(2)*.8];  uwidth = [width(1)*1.2  width(2)*1.2];
%reply = input('Do you want more? Y/N [Y]: ', 's');
end

