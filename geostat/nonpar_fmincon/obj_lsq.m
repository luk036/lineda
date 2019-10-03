function fnorm=obj_lsq(par,Fs,Sig_raw)

nvar=length(par);
ns=size(Sig_raw);

p=[fliplr(par(1:nvar-1));par(1:nvar-1)];
Sig=zeros(ns);
for i=1:(nvar-1)*2
    Sig=Sig+Fs{i}*p(i);
end

diff=Sig+par(nvar)*eye(ns(1))-Sig_raw;

fnorm=norm(diff,'fro');