close all;
clear all;
addpath ..

alpha=1;
nu=1;
h=0:0.01:4;

R_exp=exp(-alpha*h);
R_Gau=exp(-alpha^2*(h.^2));
R_Mat=corr_matern(alpha,nu,h);

figure; hold on;
plot(h,R_exp,'b--',h,R_Gau,'g-.',h,R_Mat,'r-','LineWidth', 3);
string_mat(1) = {'Exponential: \alpha=1'};
string_mat(2) = {'Gaussian: \alpha=1'};
string_mat(3) = {'Matern: \alpha=1, \nu=1'};
legend(string_mat);

sigma2=1;
tau2=0.5;

R=R_Mat*sigma2/(sigma2+tau2);
R(1)=1;

figure;
plot(h,R,'r-','LineWidth', 3);
str={'Matern: \alpha=1, \nu=1'};
legend(str);