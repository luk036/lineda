clear all;
n=50;
A1 = rand(n,n);
A = triu(A1,1) + diag(ones(n,1)) + triu(A1,1)';
%%A = [1 0.0615 0.75;
%%     0.0615 1 0.75;
%%     0.75 0.75 1];
%% A = (A1 + A1')/2;  % make it symmetric
%% W1 = rand(n,n);
%% W = W1'*W1; % make it s.p.d.
%%W = eye(n);
maxit = 1000;

% Modified alternative-projection algorithm
% Given a symetric matrix A with elements a(i,j) between 0 and 1,
% find a correlation matrix Omega that is mostly close to A.
dS = zeros(n,n); Y = A;
for k=1:maxit,
    R = Y - dS;
    
    %% X = PS(R);
    [Q,D] = eig(R);
    lambda = diag(D);
    ind = find(lambda<0);
        
    lambda(ind) = 0;
    X = Q*diag(lambda)*Q';
    
    dS = X - R;
    
    %% Y = PU(X);
    Y2 = triu(X,1) + diag(ones(n,1)) + tril(X,-1);

    if (norm(Y2-Y,'fro') <= 1e-8)
        break;
    end
    Y = Y2;
end
