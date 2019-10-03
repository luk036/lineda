%%  Xin Li, pp.16, Algo 2.2   
% Write for comparison by XXL 
function [Qnew,Q,B,C] = rank_p_c(X,f,p,iter)
%%  Approximate function by rank-p projection  
%   X, f are sampled function input and function value 
%   iter is number of iteration    
[Ndim,Nsample] = size(X);
fe = f;
f1 = zeros(Nsample,1);
fprintf('Rank-%d: \n',p);
for ip = 1 : p
    fe = fe - f1; 
    [Qnew(:,ip),Q(:,ip),B(:,ip),C(ip)] = rank_1(X,fe,iter);
    f1 = fit_rank_p_c(Qnew(:,ip),Q(:,ip),B(:,ip),C(ip),X,1);
end

return
