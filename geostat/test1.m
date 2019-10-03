exp1d2_data
[sp,sq] = log_mle_cc_corr_fn(Y,s);
fnplt(sp,'r');
C0 = fnval(sp,0);
hold on
%%% corr = exp(-0.5*(s.*s)/sdkern/sdkern/2)*(C0-0.4*0.4);
%%% plot(s,corr,'b');
%%% hold off
%%% %% 
%%% Sig = ones(n,n);
%%% for i=1:n-1,
%%%   for j=i+1:n,
%%%     d = s(j) - s(i);
%%%     Sig(i,j) = exp(-0.5*(d'*d)/sdkern/sdkern/2);
%%%     Sig(j,i) = Sig(i,j);
%%%   end
%%% end
%%% 
%%% Sig2 = Sig*(C0-0.4*0.4) + 0.4*0.4*eye(n);
%%% disp(log(det(Sig2)) + trace(inv(Sig2)*Y));
