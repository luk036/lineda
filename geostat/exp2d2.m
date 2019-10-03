exp2d_data;   % generate data
%%figure
%%contourf(xx,yy,reshape(Ys(:,N),nx,ny));
figure
hold on;
%%[sp2,tau2] = lsq_corr_fn(Y,s,'x');
%%fnplt(sp2,'g--');

[sp1,tau2] = lsq_corr_fn(Y,s,'f');
fnplt(sp1,'r');

[sp, tau2] = lsq_corr_fn(Y,s,'mf');
fnplt(sp,'b--');

[knotsx, kx] = fnbrk(sp, 'knots', 'order');
%%xx = aveknt(knotsx, kx)';
dend = knotsx(size(knotsx,2));
xx = 0:0.2:dend;
corr = var*var*exp(-0.5*(xx.*xx)/(sdkern*sdkern)/2);
%%plot(xx,corr,'k');
xlim([0,dend]);
xlabel('distance');
ylabel('variance');
legend('none','FFT only','Mono+FFT');
title('Different constraints');

hold off
disp('Relative error:');
disp(norm(fnval(sp,xx) - corr)/norm(corr));
var2 = fnval(sp,0);
var*var+tau*tau,
var2 + tau2,
