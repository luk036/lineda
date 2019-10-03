close all;
clear all;

% run for table 7

% The multirun experiment of spatial correlation extraction
% The relative errors of the spatial correlation function are gathered and 
% the mean is calculated.

n = 30;  % the multirun times of the experiment 
e_sigma2_RESCF=zeros(n,1);
e_rou_RESCF=zeros(n,1);
t_RESCF=zeros(n,1);

e_sigma2_MLEMVN=zeros(n,1);
e_rou_MLEMVN=zeros(n,1);
t_MLEMVN=zeros(n,1);

e_sigma2_MLEMVN2=zeros(n,1);
e_rou_MLEMVN2=zeros(n,1);
t_MLEMVN2=zeros(n,1);

%%%%!!!! set the synthetic process parameters for all the programs here %%%%
pt_gen='equd';           % generation method of measurement point
HIF_gen='chol';        % generation method of HIF
samp_sche='ransamp';   % sampling scheme of measurement data
noise_type='white';    % noise type of measurement error

nchip=500;   % no. of sampled chips 
xrange=20;   % measurement range along x direction
nx=21;       % no. of measurement sites along x direction on each chip
yrange=20;   % measurement range along y direction
ny=21;       % no. of measurement sites along y direction on each chip

var_G=0.5;   % actual variance of inter-die global variation component
var_S=1;     % actual variance of intra-die spatial variation component
kappa=0.1;   % ratio of residual variance to variance of intra-die spatial variation component

alpha=2;     % range/distance parameter of Matern correlation function
nu=3;        % smoothness parameter of Matern correlation function
%%%% end of setting the synthetic process parameters %%%%

% receive all the parameters to four structures
ChipInfo = struct('ChipNo',nchip,'XSiteNo',nx,'YSiteNo',ny,'XRange',xrange,'YRange',yrange);
VarInfo = struct('VarG',var_G,'VarS',var_S,'kappa',kappa);
CorrPara = struct('Par1',alpha,'Par2',nu);
% options for measurement data generation
GenOpt = struct('PtGen',pt_gen,'HIFGen',HIF_gen,'SampSche',samp_sche, ...
    'NoiseType',noise_type);

for i = 1:n
    disp('Extraction run no.: '); disp(i);
    [e_sigma2_RESCF(i),e_rou_RESCF(i),t_RESCF(i),e_sigma2_MLEMVN(i),e_rou_MLEMVN(i),t_MLEMVN(i)...
        e_sigma2_MLEMVN2(i),e_rou_MLEMVN2(i),t_MLEMVN2(i)]=fun_SCE_c0102(ChipInfo,VarInfo,CorrPara,GenOpt);
end
save table7_last-5.mat

VarInfo.kappa=0.5;
for i = 1:n
    disp('Extraction run no.: '); disp(i);
    [e_sigma2_RESCF(i),e_rou_RESCF(i),t_RESCF(i),e_sigma2_MLEMVN(i),e_rou_MLEMVN(i),t_MLEMVN(i)...
        e_sigma2_MLEMVN2(i),e_rou_MLEMVN2(i),t_MLEMVN2(i)]=fun_SCE_c0102(ChipInfo,VarInfo,CorrPara,GenOpt);
end
save table7_last-4.mat

VarInfo.kappa=1;
for i = 1:n
    disp('Extraction run no.: '); disp(i);
    [e_sigma2_RESCF(i),e_rou_RESCF(i),t_RESCF(i),e_sigma2_MLEMVN(i),e_rou_MLEMVN(i),t_MLEMVN(i)...
        e_sigma2_MLEMVN2(i),e_rou_MLEMVN2(i),t_MLEMVN2(i)]=fun_SCE_c0102(ChipInfo,VarInfo,CorrPara,GenOpt);
end
save table7_last-3.mat

ChipInfo.ChipNo=1000;
VarInfo.kappa=0.1;
for i = 1:n
    disp('Extraction run no.: '); disp(i);
    [e_sigma2_RESCF(i),e_rou_RESCF(i),t_RESCF(i),e_sigma2_MLEMVN(i),e_rou_MLEMVN(i),t_MLEMVN(i)...
        e_sigma2_MLEMVN2(i),e_rou_MLEMVN2(i),t_MLEMVN2(i)]=fun_SCE_c0102(ChipInfo,VarInfo,CorrPara,GenOpt);
end
save table7_last-2.mat

VarInfo.kappa=0.5;
for i = 1:n
    disp('Extraction run no.: '); disp(i);
    [e_sigma2_RESCF(i),e_rou_RESCF(i),t_RESCF(i),e_sigma2_MLEMVN(i),e_rou_MLEMVN(i),t_MLEMVN(i)...
        e_sigma2_MLEMVN2(i),e_rou_MLEMVN2(i),t_MLEMVN2(i)]=fun_SCE_c0102(ChipInfo,VarInfo,CorrPara,GenOpt);
end
save table7_last-1.mat

VarInfo.kappa=1;
for i = 1:n
    disp('Extraction run no.: '); disp(i);
    [e_sigma2_RESCF(i),e_rou_RESCF(i),t_RESCF(i),e_sigma2_MLEMVN(i),e_rou_MLEMVN(i),t_MLEMVN(i)...
        e_sigma2_MLEMVN2(i),e_rou_MLEMVN2(i),t_MLEMVN2(i)]=fun_SCE_c0102(ChipInfo,VarInfo,CorrPara,GenOpt);
end
save table7_last.mat