 % Write for comparison by XXL
function fm = fit_rank_p_c(Qnew,Q,B,C,X,p)
%%  Compute function values based on fitting model and input variables  
%   Qnew = \lambda_1 P_1
%   Q = P_1 which is normalized    
%   Nsample is the number of sampling
%   p is the rank of approximation    
    [Ndim,Nsample] = size(X);
    fm = zeros(Nsample,1);

    for ip = 1 : p
        f = fit_rank_1(Qnew(:,ip),Q(:,ip),B(:,ip),C(ip),X);
        fm = fm + f;
    end

return    
        
