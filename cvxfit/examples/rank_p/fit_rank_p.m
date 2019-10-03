 % Write for comparison by XXL
function fm = fit_rank_p(Qnew,Q,B,C,X,p)
%%  Compute function values based on fitting model and input variables  
%   Qnew = \lambda_1 P_1
%   Q = P_1 which is normalized    
%   Nsample is the number of sampling
%   p is the rank of approximation    
    [Ndim,Nsample] = size(X);
    fm = zeros(Nsample,1);
    ip = 1;
    for ii = 1 : Nsample
        f(ii,1) = (X(:,ii)'*Q(:,ip))*(Qnew(:,ip)'*X(:,ii)) +...
                  B'*X(:,ii) + C;
    end
    fm = fm + f;

    if p < 1.5, return, end
    
    for ip = 2 : p
        for ii = 1 : Nsample
            f(ii,1) = (X(:,ii)'*Q(:,ip))*(Qnew(:,ip)'*X(:,ii));
        end
        fm = fm + f;
    end
return    
        
