exp2da_data;   % generate data
figure
subplot(2,2,1);
contourf(xx,yy,reshape(Ys(:,1),nx,ny));
subplot(2,2,2);
contourf(xx,yy,reshape(Ys(:,2),nx,ny));
subplot(2,2,3);
contourf(xx,yy,reshape(Ys(:,3),nx,ny));
subplot(2,2,4);
contourf(xx,yy,reshape(Ys(:,4),nx,ny));

[sp, tau2] = aniso_lsq_corr_fn(Y,s, nx, ny, 'x');
figure
subplot(1,2,1);
fnplt(sp);
subplot(1,2,2);
fnplt(sp);
view(2);
title('Anisotropic spatial correlation estimation');
