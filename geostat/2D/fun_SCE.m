function [err_var_S1,err_rou1,t_RESCF,err_var_S2,err_rou2,t_MLEMVN,err_var_S3,err_rou3,t_MLEMVN2]...
    =fun_SCE(ChipInfo,VarInfo,CorrPara,GenOpt)
%% Intra-die spatial correlation extraction
% optional measurement point generation method: {'MC','equd'}
%   - MC: Monte Carlo distance
%   - equd: equal distance distributed
% optional HIF generation method: {'chol'}
%   - chol: Cholesky decomposition
% optional sampling scheme: {'ransamp'}
%   - random sampling with correlation
% optional noise type as measurement error: {'white','high_fre'}
%   - white: white noise
%   - high_fre: frequency components mainly concentrate on high frequncy range

global DEBUG_INFO;  % global sign controlling on/off of the debug information
DEBUG_INFO = 're';

nchip=ChipInfo.ChipNo;    % no. of sampled chips 
xrange=ChipInfo.XRange;   % measurement range along x direction
nx=ChipInfo.XSiteNo;      % no. of measurement sites along x direction on each chip
yrange=ChipInfo.YRange;   % measurement range along y direction
ny=ChipInfo.YSiteNo;      % no. of measurement sites along y direction on each chip

var_G=VarInfo.VarG;   % actual variance of inter-die global variation component
var_S=VarInfo.VarS;   % actual variance of intra-die spatial variation component
kappa=VarInfo.kappa;  % ratio of residual variance to variance of intra-die spatial variation component

alpha=CorrPara.Par1;  % range/distance parameter of Matern correlation function
nu=CorrPara.Par2;     % smoothness parameter of Matern correlation function

nsite=nx*ny;
range=sqrt(xrange^2+yrange^2);
%% Generate measurement data
% Measurement data are from ChipNo of sample chips and SiteNo of 
% measurement sites on each chip
[M,D,Px,Py] = data_gen_2D(ChipInfo,VarInfo,CorrPara,GenOpt);
% plot for test
% for i=1:nchip
%     figure;
%     surf(Px,Py,reshape(M(i,:),ny,nx));
% end
% figure;
% surf(Px,Py,reshape(mean(M),ny,nx));

%% 1. Roubust extraction of spatial correlation function
disp('1. Roubust extraction of spatial correlation function');
tic;  % calculate the elapsed time start
M_2=M.^2;
% calculate overall chip variance var_f using eqn.(26)
var_f=sum(M_2) - (sum(M).^2)/nchip;         % sum along the col (i.e. same site)
var_f=sum(var_f) / (nsite*(nchip-1));       % sum along the row
% calculate overall within-chip variation var_fc using eqn.(28)
var_fc=sum(M_2, 2) - (sum(M, 2).^2)/nsite;  % sum along the row (i.e. same chip)
var_fc=var_fc/(nsite-1);
var_fc=sum(var_fc)/nchip;
% do not calculate interchip global variation var_g using eqn.(29)
% instead, treat it as unknown and get it in the optimization procedure
disp('Extracted overall chip variance Var[f]:'); disp(var_f);
disp('Extracted overall within-chip variance Var[fc]:'); disp(var_fc);
    
%% Step 1.1: Extract intra-chip spatial variation var_s and alpha and nu
% calculate the covariance of two different sets at two different sites that are h distance apart, cov(h), 
% using eqn.(31)
cova=cov(M);   % the unbiased sample covariance
% obtain var_s and alpha and nu using nonlinear optimization as eqn.(35)
data_len=(nsite^2 - nsite) / 2;  % the size of the fitting data
xdata=zeros(data_len,1);
ydata=zeros(data_len,1);
ind=1;  % temp index of fitting data 
for i=1:(nsite-1)
    for j=(i+1):nsite
        xdata(ind)=D(i,j);
        ydata(ind)=cova(i,j);
        ind=ind+1;
    end
end

% constraint bounds for lsqcurfit:
% - alpha should greater than 0;
% - nu should greater than 0;
% - var_s should be greater than 0 and less than var_fc
% - var_g should be greater than 0 and less than var_f
lb=[1 0.1 1e-4 1e-4];
ub=[10 10 var_fc var_f];
x0=[1 1 1 1];  % starting guess at the solution
%options=optimset('TolFun',1e-12,'MaxFunEvals',2000,'MaxIter',1000,'TolX',1e-8);
options=[];
disp('<== Optimization using robust extraction starts ==>');
[x,resnorm,residual,exitflag,output] = lsqcurvefit(@obj_corr_fun,x0,xdata,ydata,lb,ub,options);

t_RESCF=toc; % calculate the elapsed time
disp('Runtime of optimization for robust extraction: ');disp(t_RESCF);
    
alpha_fit=x(1);  % the fitted parameter alpha in Matern class
nu_fit=x(2);  % the fitted parameter nu in Matern class
var_s=x(3);
var_g=x(4);

disp('Extracted interchip global variance Var[g]:'); disp(var_g);
disp('Extracted intrachip spatial variance Var[s]:'); disp(var_s);
disp('Extracted parameter alpha in Matern function:'); disp(alpha_fit);
disp('Extracted parameter nu in Matern function:'); disp(nu_fit);

%% Step 1.2: Extract fitted rou(h) by plugging fitted alpha and nu
% the fitted spatial correlation
h=0:0.01:range;
rou=corr_matern(alpha,nu,h);
rou_fit=corr_matern(alpha_fit,nu_fit,h);

%% Step 1.3: Extract random variation var_r by eqn.(36)
var_r = var_fc - var_s;
disp('Extracted random residual variance Var[r]:'); disp(var_r);

%% Step 1.4: Extract overall process correlation by eqn.(25)

%% Step 1.5: Report the extraction error
err_var_G=abs(var_g-var_G) / var_G;
err_var_S1=abs(var_s-var_S) / var_S;
err_rou1=norm(rou_fit-rou) / norm(rou);

disp('The relative error of the glabal-variation component err(var_G):'); disp(err_var_G);
disp('The relative error of the spatial-variation component err(var_S):'); disp(err_var_S1);
disp('The relative error of the spatial-correlation function err(rou(h)):'); disp(err_rou1);

%% 2. MLE of MVN based spatial correlation function
disp('2. MLEMVN based extraction of spatial correlation function');
%% Step 2.1: Compute the biased covariance matrix
tic;  % calculate the elapsed time start
S=M'*M/nchip;

%% Step 2.2: Optimization
par0=[1 1 1];
options=optimset('LargeScale','off');
lb=[0.01 1 0.1];
ub=[10 10 10];
disp('<== Optimization using typical ML starts ==>');
[par,loglik]=fmincon(@obj_mle,par0,[],[],[],[],lb,ub,[],options,D,S);
t_MLEMVN=toc; % calculate the elapsed time
disp('Runtime of optimization for MLEMVN based extraction: '); disp(t_MLEMVN);

kappa_fit=par(1);
alpha_fit=par(2);
nu_fit=par(3);
disp('Extracted parameter alpha in Matern function:'); disp(alpha_fit);
disp('Extracted parameter nu in Matern function:'); disp(nu_fit);
disp('Extracted parameter kappa:'); disp(kappa_fit);

R=corr_matern(alpha_fit,nu_fit,D);
V=R+kappa_fit*eye(nsite);
var_s=trace(S*inv(V))/nsite;
var_r=kappa*var_s;

disp('Extracted intrachip spatial variance Var[s]:'); disp(var_s);
disp('Extracted random residual variance Var[r]:'); disp(var_r);

%% Step 2.3: Extract fitted rou(h) by plugging in extracted parameters
% the fitted spatial correlation
rou_fit=corr_matern(alpha_fit,nu_fit,h);

%% Step 2.4: Report the extraction error
% calculate the relative error of the spatial correlation function
err_var_S2=abs(var_s-var_S) / var_S;
disp('The relative error of the spatial-variation component err(var_S):'); disp(err_var_S2);
err_rou2=norm(rou_fit-rou) / norm(rou);
disp('The relative error of the spatial-correlation function err(rou(h)):'); disp(err_rou2);

%% 3. MLE of MVN based spatial correlation function with modified scatter matrix S
disp('3. MLEMVN based extraction of spatial correlation function with modified scatter matrix');
%% Step 3.1: Compute the unbiased covariance matrix
tic;  % calculate the elapsed time start
% calculate the covariance of two different sets at two different sites that are h distance apart, cov(h), 
% using eqn.(31)
cova=cov(M);

%% Step 3.2: Optimization
disp('<== Optimization using typical ML with modified scatter matrix starts ==>');
[par,loglik]=fmincon(@obj_mle,par0,[],[],[],[],lb,ub,[],options,D,cova);

t_MLEMVN2=toc; % calculate the elapsed time
disp('Runtime of optimization for MLEMVN with modified scatter matrix based extraction: '); disp(t_MLEMVN2);

kappa_fit=par(1);
alpha_fit=par(2);
nu_fit=par(3);
disp('Extracted parameter alpha in Matern function:'); disp(alpha_fit);
disp('Extracted parameter nu in Matern function:'); disp(nu_fit);
disp('Extracted parameter kappa:'); disp(kappa_fit);

R=corr_matern(alpha_fit,nu_fit,D);
V=R+kappa_fit*eye(nsite);
var_s=trace(S*inv(V))/nsite;
var_r=kappa*var_s;

disp('Extracted intrachip spatial variance Var[s]:'); disp(var_s);
disp('Extracted random residual variance Var[r]:'); disp(var_r);

%% Step 3.3: Extract fitted rou(h) by plugging in extracted parameters
% the fitted spatial correlation
rou_fit=corr_matern(alpha_fit,nu_fit,h);

%% Step 3.3: Report the extraction error
% calculate the relative error of the spatial correlation function
err_var_S3=abs(var_s-var_S) / var_S;
disp('The relative error of the spatial-variation component err(var_S):'); disp(err_var_S3);
err_rou3=norm(rou_fit-rou) / norm(rou);
disp('The relative error of the spatial-correlation function err(rou(h)):'); disp(err_rou3);
