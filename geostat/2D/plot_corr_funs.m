figure;
plot(h,rou,'k',h,rou_fit1,'b--',h,rou_fit2,'g:',h,rou_fit3,'r-.','LineWidth',4);
title('Figure: actual v.s. fitted spatial correlation');
xlabel('Distance');
ylabel('Spatial correlation');
legend('actual spatial correlation', 'fitted spatial correlation by RESCF', ...
    'fitted spatial correlation by MLEMVN','fitted spatial correlation by MLEMVN-M');