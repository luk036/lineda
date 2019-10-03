function [M,D,X,corr] = data_gen_2D(ChipInfo,VarInfo,CorrPara,GenOpt)
% [M,D,corr] = data_gen_2D(ChipInfo,VarInfo,CorrPara,GenOpt)
% generate measurement data with measurement error
% Input: four structures containing synthetic process parameters
%   ChipInfo: chip information containing three elements
%     - ChipInfo.ChipNo is the no. of sampled chips;
%     - ChipInfo.XSiteNo is the no. of measurement sites along x direction on each chip;
%     - ChipInfo.YSiteNo is the no. of measurement sites along y direction on each chip;
%     - ChipInfo.XRange is the mesurement range along x direction;
%     - ChipInfo.YRange is the mesurement range along y direction.
%   VarInfo: the variance information containing four components
%     - VarInfo.VarG is the actual variance of inter-die global variation component;
%     - VarInfo.VarS is the actual variance of intra-die spatial variation component;
%     - VarInfo.VarR is the actual variance of purely random variation component;
%     - VarInfo.NoiseRatio is the ratio of measurement error noise variance 
%         to the sum of the three above variances.
%   CorrPara: parameters of the correlation function (Matern function now)
%     - CorrPara.Par1 is the range/distance parameter alpha;
%     - CorrPara.Par2 is the smoothness parameter nu.
%   GenOpt: options for measurement data generation
%     - GenOpt.PtGen is the generation method of measurement point;
%     - GenOpt.HIFGen is the generation method of HIF;
%     - GenOpt.SampSche is the sampling scheme of measurement data;
%     - GenOpt.NoiseType is the noise type of measurement error.
%
% Output:
%     M: the generated measurement data;
%     D: the distance matrix;
%     corr: the Matern correlation coefficient matrix.
%
% The generated measurement data are arranged as follows:
% along by row - same chip, indexed by sites
% along by col - same site, different chips
%
%     S1 S2 S3 S4 S5
% C1   -  -  -  -  -
% C2   -  -  -  -  -
% C3   -  -  -  -  -
% C4   -  -  -  -  -

global DEBUG_INFO;  % global sign controlling on/off of the debug information

randn('state', sum(100*clock));

% receiving the parameters
nchip = ChipInfo.ChipNo;  % no. of sampled chips
nx = ChipInfo.XSiteNo;    % no. of measurement sites along x direction on each chip
ny = ChipInfo.YSiteNo;    % no. of measurement sites along y direction on each chip
xrange = ChipInfo.XRange; % measurement range along x direction
yrange = ChipInfo.YRange; % measurement range along y direction
var_G = VarInfo.VarG;     % actual variance of inter-die global variation component
var_S = VarInfo.VarS;     % actual variance of intra-die spatial variation component
kappa = VarInfo.kappa;    % ratio of residual variance to variance of intra-die spatial variation component
alpha = CorrPara.Par1;    % range/distance parameter of Matern correlation function
nu = CorrPara.Par2;       % smoothness parameter of Matern correlation function
nsite = nx*ny;            % total no. of measurement sites
%% Step 1: Generate correlation matrix corr
% correlation function already known.
% generate the coordinates of measurement point
% Px: the x coordinates of the grids (ny,nx)
% Py: the y coordinates of the grids (ny,nx)
% point_x: x coordinate of measurement point (nsite,1)
% point_y: y coordinate of measurement point (nsite,1)
switch GenOpt.PtGen
    case 'equd'  % measurement points evenly distributed
        % generate a sequence of 2-d grids distributed evenly on a plane
        dx=xrange/(nx-1);   % interval between two neigbouring grids along x direction
        dy=yrange/(ny-1);   % interval between two neigbouring grids along y direction
        [Px,Py]=meshgrid(0:dx:xrange,0:dy:yrange);  % generate the mesh grids
    case 'MC'  % measurement points randomly distributed
        [Px,Py]=meshgrid(xrange*rand(nx,1),yrange*rand(ny,1));
    otherwise
        error('No this option');
end

% compress all the coordinates of the grids into a vector
point_x=reshape(Px,nsite,1);  % coordinates of the x axis of all the grids   
point_y=reshape(Py,nsite,1);  % coordinates of the y axis of all the grids
% calculate distance matrix
% Based on Peter Acklam distance matrix calculation without loop
X = [point_x,point_y];
xx = permute(X,[ 1 3 2 ]);
yy = permute(X,[ 3 1 2 ]);
D = sqrt(sum((xx(:,ones(1,nsite),:) - yy(ones(1,nsite),:,:)).^2,3));  
% D is the distance matrix
% D(1,1) D(1,2) D(1,3) D(1,4)
% D(2,1) D(2,2) D(2,3) D(2,4)
% D(3,1) D(3,2) D(3,3) D(3,4)
% D(4,1) D(4,2) D(4,3) D(4,4)

% Generate the correlation matrix corr via Matern class
corr = corr_matern(alpha,nu,D);
% Check if the correlation matrix is symmetric
% corr_t = corr';
% if ~isequal(corr_t, corr)
%     error('The correlaton matrix is not symmetric!')
% end
% disp('corr is symmetric');

%% Step 2: Generate the intra-die spatial variation component
% A Gaussian random field with correlation matrix corr is synthesized 
% to represent this component.
xmean = zeros(1,nsite); % zero mean now
xsd = repmat(sqrt(var_S),1,nsite); % homoscedasticity now
% generate random process according to GeoOpt.HIFGen and GeoOpt.SampSche
if (strcmp(GenOpt.HIFGen,'chol') && strcmp(GenOpt.SampSche,'ransamp'))
    M = ransamp(xmean,xsd,corr,nchip);
end

% plot the generated hif's and their mean for test
% for i=1:nchip
%     figure;
%     surf(Px,Py,reshape(M(i,:),ny,nx));
% end
% figure;
% surf(Px,Py,reshape(mean(M),ny,nx));

range=sqrt(xrange^2+yrange^2);
%%% for inner control
if (strcmp(DEBUG_INFO,'debug'))
    h = 0:0.01:(range+0.3*range);
    rou = corr_matern(alpha,nu,h);  % the actual spatial correlation function
    figure(1);
    plot(h,rou,'LineWidth',2);
    hold on;
   
    spat_corr = corrcoef(M);  % spatial correlation computed via simulated artificial data
    for i = 1:(nsite-1)
       for j = (i+1):nsite
           plot(D(i,j), spat_corr(i,j), 'k.');
       end
    end
    title('Figure 1: actual spatial correlation function and computed spatial correlation from synthetic data generated via the actual function');
    xlabel('Distance');
    ylabel('Spatial correlation');
    legend('actual spatial correlation function', 'computed spatial correlation via data');
end
%%% end inner control

%% Step 3: Add the inter-die global variation component
% U is a vector storing the intra-die global variation component
% assuming U is Gaussian distributed with zero mean now
U = sqrt(var_G) * rand(nchip,1);
% Add the inter-die global variation component U to M 
% (same observed value on same chip)
M = M + repmat(U,1,nsite);
%%% for inner control
if (strcmp(DEBUG_INFO,'debug'))
    h = 0:0.01:(range+0.3*range);
    rou = corr_matern(alpha,nu,h);  % the actual spatial correlation function
    figure(2);
    plot(h,rou,'LineWidth',2);
    hold on;
   
    corr = corrcoef(M);  % correlation computed via simulated artificial data
    for i = 1:(nsite-1)
        for j = (i+1):nsite
            plot(D(i,j), corr(i,j), 'k.');
        end
    end
    title('Figure 2: computed correlation after inter-die global variation component is added');
    xlabel('Distance');
    ylabel('Correlation');
end
%%% end inner control

%% Step 4: Add the random variation component and measurement error
% this component is modeled as Gaussian white noise
tau2=kappa*var_S;  % sum variance of intra-die random variation component and measurement error
M = M + sqrt(tau2) * randn(nchip, nsite);
%%% for inner control
if (strcmp(DEBUG_INFO, 'debug'))
    h = 0:0.01:(range+0.3*range);
    rou = corr_matern(alpha,nu,h);  % the actual spatial correlation function
    figure(3);
    plot(h,rou,'LineWidth',2);
    hold on;
   
    corr = corrcoef(M);  % correlation computed via simulated artificial data
    for i = 1:(nsite-1)
        for j = (i+1):nsite
           plot(D(i,j), corr(i,j), 'k.');
        end
    end
    title('Figure 3: computed correlation after global & random variations are added');
    xlabel('Distance');
    ylabel('Correlation');
end
%%% end inner control

disp('Variance of actual inter-die global variation var[G]:'); disp(var_G);
disp('Variance of actual intra-die spatial variation var[S]:'); disp(var_S);
disp('Variance of actual residual variation tau2:'); disp(tau2);