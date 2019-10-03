mm=2;
e = ones(mm,1);
T = spdiags([e e e], [-1:1], mm, mm);
I = speye(mm);
%T2 = kron(T,I) + kron(I,T);
%T3 = kron(T2,I) + kron(I,T2);
%S = kron(T3,I) + kron(I,T3);
for i=1:n-1
   T = kron(T,I) + kron(I,T);
end
S=T; 
%S = kron(T,kron(I,I)) + kron(I,kron(T,I)) + kron(I,kron(I,T));
S = S - diag(diag(S));
S = triu(S);

gplot(S,ocv([1 2],:)','g');
hold on
%plot(ocvc(1),ocvc(2),'ro');
plot(ocv(1,maxind),ocv(2,maxind),'co')
plot(ocV(1,:),ocV(2,:),'y.')

%g = bingen(n);
%text(ocv(1,:)',ocv(2,:)',setstr(g + '0'));
hold off

