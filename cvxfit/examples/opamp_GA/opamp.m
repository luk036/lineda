%% opamp.m 12/02/07
%% Author: XXLiu
%% This file is the main MATLAB file in this directory.
close all; clear all;

Vdd = 5.0;    % max common-mode input voltage (V)
Vss = 0.0;    % min common-mode input voltage (V)
%% Specification ~/share/sm124002-1c.hspice
global L
L = 0.8e-6;
SPEC = struct(...
    'Vdd', Vdd, 'Vss', Vss,...
    'Cload', 3e-12,...      % loading capacitance (F)   
    'Ccmax', 10e-12,...
    'Lmax', 2e-6,...        % max device length (m)
    'Lmin', 1e-6,...        % min device length (m)
    'Wmax', 500e-6,...     % max device width  (m)
    'Wmin', 10e-6,...        % min device width  (m)
    'VODmax', 0.4,...       % max overdrive voltage (V)
    'VODmin', 0.2,...       % min overdrive voltage (V)
    'VCMmax', 0.5*Vdd,...  % max common-mode input voltage (V)
    'VCMmin', 0.5*Vdd,...  % min common-mode input voltage (V)
    'VOUTmax', 0.6*Vdd,...  % max output voltage (V)
    'VOUTmin', 0.4*Vdd,...  % min output voltage (V)
    'Imax', 20e-6,...
    'area', 1e-6,...    % maximum area (m^2)
    'power', 5e-3,...    % max quiescent power (W)
    'gain', 1000,...      % min open-loop gain (dB)
    'UGF', 80e6,...    % min unity gain bandwidth (Hz)
    'PM', 60,...      % min phase margin (degree)
    'CMRR', 75,...      % min common-mode rejection ratio (dB)
    'PPSRR', 80,...      % min positive power supply rejection ratio (dB)
    'NPSRR', 80,...      % min negative power supply rejection ratio (dB)
    'SR', 10e6,...    % min slew rate (V/s)
    'IRSN', 1e-6...    % max input-referred spot noise;(nV/sqrt(Hz)@1kHz)
); % end of SPEC

%% HSPICE Level 1 parameters for the technology
KBOLT = 1.38e-23;   % Boltsman Coeeficient (J/K)
TEMP  = 300;        % Room Temperature (K)
QE    = 1.60e-19;   % One electron charge (C)
VT = KBOLT*TEMP/QE; % Thermal voltage (V)

TOX = 7.5e-9;        % Oxide thickness (m)
COX = 34.53e-12/TOX;% oxide capacitance per unit area (F/m^2)
LD  = 0.2e-6;       % Lateral diffusion (m)
Uo_n = 438.8489e-4;  Uo_p = 137.9776e-4;
KN = Uo_n*COX; KP = Uo_p*COX;

MOS3p3 = struct(...
    'KN', KN,  'KP', KP,...
    'Tox', TOX,...    % Oxide thickness (m)
    'Cox', COX,...    % oxide capacitance per unit area (F/m^2)
    'LD', LD,...      % Lateral diffusion (m)
    'Vth', [0.64  -0.822],...    % NMOS threshold voltage (V)
    'Uo', [Uo_n  Uo_p],...% NMOS carrier mobility (m^2/(Vs))
    'lambda', [0.01  0.01],...% channel length modulation (1/V) 0.01  0.001
    'KF', [4e-24  2e-24],...  % flicker noise coefficient
    'KA', [1  1],...          % flicker noise exponent
    'Cgd0', [692e-12  692e-12],...% gate-grain overlap capacitance (F/m)
    'CgsO', [692e-12  692e-12],...% gate-source overlap capacitance (F/m)
    'Cj', [271e-6  423e-6],...    % zero-bias bulk junction capacitace (F/m^2)
    'Cjsw', [600e-12  1200e-12],...  % zero-bias sidewall bulk junction capacitance (F/m)
    'gamma', [0.298  0.471],...   % NMOS body effect factor (V^0.5)
    'phi', [0.688  0.730],...     % NMOS surface inversion potential
    'Rsh', [40  60],...       % NMOS diffusion sheet resistance (ohm per unit area)
    'PB', [0.904  0.928],...  % NMOS bulk junction contact potential (V)
    'Mj', [0.5  0.5],...      % NMOS bulk junction grading coefficient
    'Mjsw', [0.5  0.5]...     % NMOS bulk junction sidewall bulk grading coefficient
); % end of MOS3p3

%--------------------------------------------
global QUIET filename
QUIET = 1; filename = 'opamp2';

%% Call function initial which will give an initial point
%% for optimization flow to start with
%% In initial.m, MOSFET model and Op Amp performances
%% are approximated by level-1 model and quadratic equations.
[design, flag] = initial(SPEC, MOS3p3);
if flag == 0, write_design(design); end
simu_type = struct('AC_TRUE', 1, 'TR_TRUE', 0, 'PLOT_TRUE', 0);
write_simu_type(simu_type);
[status,result]=system('hspice -i opamp2.sp -o opamp2.lis');
if status ~= 0, return, end
% [gain,bw,phase]=read_ma(filename)
% return
xinitial = [design.W1, design.W3, design.W5, design.W6,...
            design.W7, design.W8, design.I8];

options = gaoptimset;
options = gaoptimset(options,'StallTimeLimit',1000);
options = gaoptimset(options,'InitialPopulation',xinitial);
%options = gaoptimset(options,'PlotFcns',@gaplotbestf);
objFun = @objfun3;
conFun = @confun2;
A = []; b = []; Aeq = []; beq = [];
lb = [1e-6*ones(1,6), 5e-6];
ub = [400e-6*ones(1,6), 10e-6];
tic
[results,fval,exitflag] = ga(objFun,7,A,b,Aeq,beq,lb,ub,conFun,options);
toc






