%%  Xin Li, pp.14, Algo 2.1    
% Write for comparison by XXL
function [Qnew,Q,B,C] = rank_1(X,f,iter)
%%  Approximate function by rank-1 projection  
%   X, f are sampled function input and function value 
%   iter is number of iteration    
[l,m] = size(X); if l > m, X = X'; end
[l,m] = size(f); if l < m, f = f'; end

[Ndim,Nsample] = size(X);
Qnew = ones(Ndim,1);
k = 0;
err = ones(Nsample,1);
while 1
    k=k+1;
    Q = Qnew/norm(Qnew);
    Mod = [];
    for ii = 1 : length(f)
         Mod(ii, :) = [(X(:,ii)'*Q) * X(:,ii)'  X(:,ii)'  1];  % eqn.2-6
    end
    sol = Mod\f;
    Qnew = sol(1:Ndim); B = sol(Ndim+1:2*Ndim); C = sol(2*Ndim+1);

    % ---- err calculated from data sample
%     fm = fit_rank_p(Qnew,Q,B,C,X,1);
%     err_new = norm(fm - f);

    % ---- err calculated from dist between Qnew and Q
    Qnew_norm = Qnew/norm(Qnew);
    if norm(Qnew_norm - Q) < 1e-6, break, end
    
    %    if norm(err - err_new)/norm(err) < 1e-10, break, end
    %    err = err_new;
    %    if(k < 2) tol = err_bs/1e10; end

    %    if(abs(err_bs - err) < tol) break; end
    %    err = err_bs;
    %    Q = Qnew;
    if(k > iter) break; end

end

fprintf(1,'Iteration: %d\n',k);
