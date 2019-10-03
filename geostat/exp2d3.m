figure
xx = 0:0.2:knotsx(size(knotsx,2));
corr = var*var*exp(-0.5*(xx.*xx)/(sdkern*sdkern)/2);
plot(xx,corr,'o-');
hold on
for i=1:5,
  exp2d_data;
  [sp, tau2] = lsq_corr_fn(Y,s,'mf');
  fnplt(sp,'r--');
end
xlim([0,xx(size(xx,2))]);
legend('ideal','least squares');
xlabel('distance');
ylabel('variance');
title('Isotropic spatial correlation estimation with 50 samples');
hold off
