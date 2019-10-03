%%  Xin Li, pp.16, Algo 2.2   
% Write for comparison by XXL 
function [Qnew,Q,B,C] = rank_p(X,f,p,iter)
%%  Approximate function by rank-p projection  
%   X, f are sampled function input and function value 
%   iter is number of iteration    
[Ndim,Nsample] = size(X);
fe = f;

fprintf('Rank-%d: ',p);
ip = 1;
[Qnew(:,ip),Q(:,ip),B,C] = rank_1(X,fe,iter);
f1 = fit_rank_p(Qnew(:,ip),Q(:,ip),B,C,X,1);

if p < 1.5, return, end

for ip = 2 : p
    %fprintf('Rank-%d: ',ip);
    fe = fe - f1;
    Qnew(:,ip) = ones(Ndim,1);
    k = 0;
    err = ones(Nsample,1);
    while 1
        k=k+1;
        Q(:,ip) = Qnew(:,ip)/norm(Qnew(:,ip));
        Mod = [];
        for ii = 1 : length(f)
            Mod(ii, :) = (X(:,ii)'*Q(:,ip)) * X(:,ii)';  % eqn.2-6
        end
        Qnew(:,ip) = Mod\fe;
        
        % err_new = norm(f1 - fe);
        % if(k < 2) tol = err_bs/1e10; end
        if(k > iter) break; end
        
        Qnew_norm = Qnew(:,ip)/norm(Qnew(:,ip));    %% stop criterion 1
        if norm(Qnew_norm - Q(:,ip)) < 1e-2, break, end
    
        % if norm(err_new - err)/norm(err) < 1e-10, break, end 
        % err = err_new;                %% stop criterion 2
    end
    
    for ii = 1 : Nsample
        f1(ii,1) = (X(:,ii)'*Q(:,ip))*(Qnew(:,ip)'*X(:,ii));
    end    
    
    %fprintf(1,'Iteration: %d\n',k);
end

return
