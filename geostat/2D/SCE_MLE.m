clear all;
close all;
%% MLE of MVN based method for intra-die spatial correlation extraction
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
DEBUG_INFO='re';
%%%%!!!! set the synthetic process parameters for all the programs here %%%%
pt_gen='equd';         % generation method of measurement point
HIF_gen='chol';        % generation method of HIF
samp_sche='ransamp';   % sampling scheme of measurement data
noise_type='white';    % noise type of measurement error

nchip=500;   % no. of sampled chips 
xrange=10;   % measurement range along x direction
nx=11;       % no. of measurement sites along x direction on each chip
yrange=10;   % measurement range along y direction
ny=11;       % no. of measurement sites along y direction on each chip

var_G=0.1;   % actual variance of inter-die global variation component
var_S=1;     % actual variance of intra-die spatial variation component
kappa=0.1;     % ratio of residual variance to variance of intra-die spatial variation component

alpha=2;     % range/distance parameter of Matern correlation function
nu=3;        % smoothness parameter of Matern correlation function
%%%% end of setting the synthetic process parameters %%%%

% receive all the parameters to four structures
ChipInfo=struct('ChipNo',nchip,'XSiteNo',nx,'YSiteNo',ny,'XRange',xrange,'YRange',yrange);
VarInfo=struct('VarG',var_G,'VarS',var_S,'kappa',kappa);
CorrPara=struct('Par1',alpha,'Par2',nu);
% options for measurement data generation
GenOpt=struct('PtGen',pt_gen,'HIFGen',HIF_gen,'SampSche',samp_sche, ...
    'NoiseType',noise_type);

nsite=nx*ny;
range=sqrt(xrange^2+yrange^2);
%% Generate measurement data
% Measurement data are from ChipNo of sample chips and SiteNo of 
% measurement sites on each chip
[M,D,Px,Py]=data_gen_2D(ChipInfo,VarInfo,CorrPara,GenOpt);
% plot for test
% for i=1:nchip
%     figure;
%     surf(Px,Py,reshape(M(i,:),ny,nx));
% end
% figure;
% surf(Px,Py,reshape(mean(M),ny,nx));

if 0    %% Roubust extraction of spatial correlation function
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
    
    %%% for inner control
    if (strcmp(DEBUG_INFO, 'debug'))
        h=0:0.01:range;
        rou=corr_matern(alpha,nu,h);  % the actual spatial correlation function
        figure;
        plot(h,rou,'LineWidth',2);
        hold on;
    
        plot(xdata,ydata,'k.');
        title('Figure: computed covariance from the simulated data');
        xlabel('Distance');
        ylabel('Covariance');
        legend('actual spatial correlation', 'computed covariance');
    end
    %%% end inner control

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
    exitflag, output
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
    h=0:0.01:range;
    % the actual spatial correlation
    rou=corr_matern(alpha,nu,h);
    % the fitted spatial correlation
    rou_fit=corr_matern(alpha_fit,nu_fit,h);

    figure;
    plot(h,rou,'k');
    hold on;
    plot(h,rou_fit,'b--');
    title('Figure: actual v.s. fitted spatial correlation');
    xlabel('Distance');
    ylabel('Spatial correlation');
    legend('actual spatial correlation', 'fitted spatial correlation');

    %% Step 1.3: Extract random variation var_r by eqn.(36)
    var_r = var_fc - var_s;
    disp('Extracted random residual variance Var[r]:'); disp(var_r);

%     %% Step 1.4: Extract overall process correlation by eqn.(25)
%     % the actual process correlation
%     pro_rou=(var_G+rou*var_S)/(var_G+var_S+kappa*var_S);
%     % the fitted process correlation
%     pro_rou_fit=(var_g+rou_fit*var_s)/(var_g+var_s+var_r);
% 
%     figure;
%     plot(h,pro_rou,'g','LineWidth',2);
%     hold on;
%     plot(h,pro_rou_fit,'r','LineWidth',2);
% 
%     proc_corr=corrcoef(M) ;  % process correlation computed via simulated artificial data
%     for i=1:(nsite-1)
%         for j=(i+1):nsite
%             plot(D(i,j),proc_corr(i,j),'k.');
%         end
%     end
% 
%     title('Figure: actual, fitted, v.s. computed process correlation');
%     xlabel('Distance');
%     ylabel('Process correlation');
%     legend('actual process correlation', 'fitted process correlation', 'computed process correlaton');

    %% Step 1.5: Report the extraction error
    err_var_G=abs(var_g-var_G) / var_G;
    err_var_S=abs(var_s-var_S) / var_S;
    err_rou=norm(rou_fit-rou) / norm(rou);

    disp('The relative error of the glabal-variation component err(var_G):'); disp(err_var_G);
    disp('The relative error of the spatial-variation component err(var_S):'); disp(err_var_S);
    disp('The relative error of the spatial-correlation function err(rou(h)):'); disp(err_rou);
end

%% MLE of MVN based spatial correlation function for multiple samples
disp('2. MLEMVN based extraction of spatial correlation function for multiple samples starts');
%% Step 2.1: Compute the covariance matrix
tic;  % calculate the elapsed time start
% S=M'*M/nchip; % the biased covariance matrix
S=cov(M); % the unbiased covariance matrix
% Mx=M-repmat(mean(M),nchip,1); % "cov" in Matlab removes the mean
% Sx=Mx'*Mx/(nchip-1);
% if isequal(S,Sx)
%     disp('ok');
% end
%% Step 2.2: Optimization
par0=[1 1 1];
options=optimset('LargeScale','off');
lb=[0.01 1 0.1];
ub=[10 10 10];
disp('<== Optimization using ML starts ==>');
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
h=0:0.01:range;
% the actual spatial correlation
rou=corr_matern(alpha,nu,h);
% the fitted spatial correlation
rou_fit=corr_matern(alpha_fit,nu_fit,h);

figure;
plot(h,rou,'k',h,rou_fit,'r--');
title('Figure: actual v.s. fitted spatial correlation');
xlabel('Distance');
ylabel('Spatial correlation');
legend('actual spatial correlation', 'fitted spatial correlation');

%% Step 2.4: Report the extraction error
% calculate the relative error of the spatial correlation function
err_var_S=abs(var_s-var_S) / var_S;
disp('The relative error of the spatial-variation component err(var_S):'); disp(err_var_S);
err_rou=norm(rou_fit-rou) / norm(rou);
disp('The relative error of the spatial-correlation function err(rou(h)):'); disp(err_rou);

%% MLE of MVN based spatial correlation function for sample mean
disp('3. MLEMVN based extraction of spatial correlation function for sample mean starts');
%% Step 3.1: Compute the covariance matrix
tic;  % calculate the elapsed time start
mn=mean(M);
B=mn'*mn;

%% Step 3.2: Optimization
par0=[1 1 1];
options=optimset('LargeScale','off');
lb=[0.01 1 0.1];
ub=[10 10 10];
disp('<== Optimization using ML starts ==>');
[par,loglik]=fmincon(@obj_mle,par0,[],[],[],[],lb,ub,[],options,D,B);

t_MLEMVN2=toc; % calculate the elapsed time
disp('Runtime of optimization for MLEMVN based extraction: '); disp(t_MLEMVN2);

kappa_fit=par(1);
alpha_fit=par(2);
nu_fit=par(3);
disp('Extracted parameter alpha in Matern function:'); disp(alpha_fit);
disp('Extracted parameter nu in Matern function:'); disp(nu_fit);
disp('Extracted parameter kappa:'); disp(kappa_fit);

R=corr_matern(alpha_fit,nu_fit,D);
V=R+kappa_fit*eye(nsite);
var_s=trace(B*inv(V))/nsite;
var_r=kappa*var_s;

disp('Extracted intrachip spatial variance Var[s]:'); disp(var_s);
disp('Extracted random residual variance Var[r]:'); disp(var_r);

%% Step 3.3: Extract fitted rou(h) by plugging in extracted parameters
% the fitted spatial correlation
rou_fit2=corr_matern(alpha_fit,nu_fit,h);

figure;
plot(h,rou,'k',h,rou_fit2,'r--');
title('Figure: actual v.s. fitted spatial correlation');
xlabel('Distance');
ylabel('Spatial correlation');
legend('actual spatial correlation', 'fitted spatial correlation');

%% Step 3.4: Report the extraction error
% calculate the relative error of the spatial correlation function
err_var_S2=abs(var_s-var_S) / var_S;
disp('The relative error of the spatial-variation component err(var_S):'); disp(err_var_S2);
err_rou2=norm(rou_fit2-rou) / norm(rou);
disp('The relative error of the spatial-correlation function err(rou(h)):'); disp(err_rou2);
