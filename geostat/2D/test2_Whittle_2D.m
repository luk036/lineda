clear all;
close all;
%% Spectral density method for intra-die spatial correlation extraction
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
nx=21;       % no. of measurement sites along x direction on each chip
yrange=10;   % measurement range along y direction
ny=21;       % no. of measurement sites along y direction on each chip

var_G=0.5;   % actual variance of inter-die global variation component
var_S=1;     % actual variance of intra-die spatial variation component
kappa=0;     % ratio of residual variance to variance of intra-die spatial variation component

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
%% Step 1: Generate measurement data
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
%% Step 2: Compute the covariance matrix
tic;  % calculate the elapsed time start
% S=M'*M/nchip; % the biased covariance matrix
S=cov(M); % the unbiased covariance matrix
if 0
    par0=[1 1 1];
    options=optimset('LargeScale','off');
    lb=[0.01 1 0.1];
    ub=[10 10 10];
    disp('<== Optimization using ML starts ==>');
    [par,loglik]=fmincon(@obj_mle,par0,[],[],[],[],lb,ub,[],options,D,S);
    
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

    h=0:0.01:range;
    % the actual spatial correlation
    rou=corr_matern(alpha,nu,h);
    % the fitted spatial correlation
    rou_fit=corr_matern(alpha_fit,nu_fit,h);
    
    err_var_S3=abs(var_s-var_S) / var_S;
    disp('The relative error of the spatial-variation component err(var_S):'); disp(err_var_S3);
    err_rou3=norm(rou_fit-rou) / norm(rou);
    disp('The relative error of the spatial-correlation function err(rou(h)):'); disp(err_rou3);
end

%% Step 3:
disp('-****************************************************-');
tic;  % calculate the elapsed time start
dx=xrange/(nx-1);     % sampling interval along x direction
nyquist_x=0.5*(1/dx); % nyquist frequency along x direction (defined as matlab help)
dy=yrange/(ny-1);     % sampling interval along y direction
nyquist_y=0.5*(1/dy); % nyquist frequency along y direction (defined as matlab help)

nwx=fix(nx/2+1);
nwy=fix(ny/2+1);
nw=nwx*nwy;

P=zeros(nchip,nw);
% for i=1:nchip
%     X=fft2(reshape(M(i,:),ny,nx));
%     X_tmp=X(1:nwx,1:nwy); % leave only the first quadrant
%     P(i,:)=reshape(X_tmp.*conj(X_tmp)/nsite/(4*pi*pi),1,nw);
% end
Taper=kron(tukeywin(ny,0.75),tukeywin(nx,0.75)');
den=(4*pi*pi*sum(sum(Taper.*Taper)));
for i=1:nchip
    X=fft2(Taper.*reshape(M(i,:),ny,nx));
    X_tmp=X(1:nwx,1:nwy); % leave only the first quadrant
    P(i,:)=reshape( X_tmp.*conj(X_tmp)/den,1,nw );
end
% P(:,1)=0;

[wx,wy]=meshgrid(2*pi*linspace(0,nyquist_x,nwx),2*pi*linspace(0,nyquist_y,nwy));
% plot for test
figure;
surf(wx,wy,reshape(P(1,:),nwy,nwx));
figure;
surf(wx,wy,reshape(mean(P),nwy,nwx));

omega_x=reshape(wx,1,nw);
omega_y=reshape(wy,1,nw);
% omega_x(1)=[];
% omega_y(1)=[];
% plot for test
% figure;
% plot3(omega_x,omega_y,vec_P,'.');

omega_x2=omega_x.^2;
omega_y2=omega_y.^2;
mp=mean(P);
par0=[1; 1];
options=optimset('LargeScale','off','TolFun',1e-12,'MaxFunEvals',2000,'MaxIter',1000,'TolX',1e-8);
lb=[1; 0.1];
ub=[10; 10];
disp('<== Optimization using Whittle likelihood starts ==>');
[par,loglik]=fmincon(@test2_obj_Whittle,par0,[],[],[],[],lb,ub,[],options,...
    omega_x2,omega_y2,mp);

t_WhiMLE=toc; % calculate the elapsed time
disp('Runtime of optimization for Whittle likelihood based extraction: '); disp(t_WhiMLE);

alpha_fit=par(1);
nu_fit=par(2);
disp('Extracted parameter alpha in Matern function:'); disp(alpha_fit);
disp('Extracted parameter nu in Matern function:'); disp(nu_fit);

var_s=pi*sum( (alpha_fit^2+omega_x2+omega_y2).^(nu_fit+1).*mp ) ...
    /(nsite*alpha_fit^(2*nu_fit)*nu_fit);

disp('Extracted intrachip spatial variance Var[s]:'); disp(var_s);

%% Step 4: Extract fitted rou(h) by plugging in extracted parameters
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

%% Step 5: Report the extraction error
% calculate the relative error of the spatial correlation function
err_var_S4=abs(var_s-var_S) / var_S;
disp('The relative error of the spatial-variation component err(var_S):'); disp(err_var_S4);
err_rou4=norm(rou_fit-rou) / norm(rou);
disp('The relative error of the spatial-correlation function err(rou(h)):'); disp(err_rou4);
