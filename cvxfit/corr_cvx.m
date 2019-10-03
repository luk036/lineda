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
%% W = eye(n);
%% maxit = 1000;

cvx_begin sdp
   variable Y(n,n);
   minimize(norm(A - Y,'fro'))
   subject to
      Y >= 0;
      for i=1:n
          Y(i,i) == 1;
      end
      
cvx_end
