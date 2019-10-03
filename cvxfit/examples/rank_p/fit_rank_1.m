function fm = fit_rank_1(Qnew,Q,B,C,X)
    [Ndim,Nsample] = size(X);
    for ii = 1 : Nsample
        fm(ii,1) = X(:,ii)'*Q*Qnew'*X(:,ii) + B'*X(:,ii) + C;
    end
    
return
    
