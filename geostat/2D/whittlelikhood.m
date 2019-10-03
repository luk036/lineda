close all;
a=1:0.1:5;
b=0.1:0.1:5;

na=length(a);
nb=length(b);

T=zeros(na,nb);
% sigma2_fit=zeros(na,nb);
% figure;
for i=1:length(a)
    for j=1:length(b)
        T(i,j)=test_obj_Whittle([a(i) b(j)],omega_x2,omega_y2,P);
%         sigma2_fit(i,j)=pi*a(i)^(-2*b(j))/(nw-1)/b(j)*...
%             sum((a(i)^2+omega_x2+omega_y2).^(b(j)+1).*vec_P);
%         plot3(a(i),b(j),T(i,j),'.');
%         hold on;
    end
end
figure;
[Px,Py]=meshgrid(a,b);
meshz(Px,Py,T');
% figure;
% meshz(Px,Py,sigma2_fit');
