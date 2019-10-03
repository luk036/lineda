function s=ransamp(xmean,xsd,corr,nsample)
% s=ransamp(xmean,xsd,corr,nsample)
% random sampling with correlation
% Input:
%   xmean     : mean of data (1,nvar)
%   xsd       : std.dev of data (1,nvar)
%   corr      : correlation matrix of the variables (nvar,nvar)
%   nsample   : no. of samples
% Output:
%   s       : random sample (nsample,nvar) - nsample rows, nvar cols
% Uses Matlab function: randn
%   Budiman (2003)
% Small modification by FQ (2008)

nvar=size(corr,1);
% random sampling 
[L,p] = chol(corr,'lower');
if (p>0)  % the traditional Cholesky decomposition failed
    disp('The correlation matrix is not quite positive definite,');
    disp('using modified Cholesky decomposition instead.');
    [L,D,E] = mchol(corr);
    L = L*sqrt(D);
end

z = randn(nvar,nsample);
y = L*z ;  % y: (nvar,nsample), y': (nsample,nvar)
s = (y'.*repmat(xsd,nsample,1))+ repmat(xmean,nsample,1);
