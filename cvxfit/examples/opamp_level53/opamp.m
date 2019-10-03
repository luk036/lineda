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

% --------------------------------------------
global c_vod1 a_vod1_W1 a_vod1_W5 a_vod1_W8 a_vod1_I8
global c_vod3 a_vod3_W3 a_vod3_W5 a_vod3_W8 a_vod3_I8
global c_vod5 a_vod5_W8 a_vod5_I8
global c_vod6 a_vod6_W6 a_vod6_W7 a_vod6_W8 a_vod6_I8
global c_vod7 a_vod7_W8 a_vod7_I8
global c_vod8 a_vod8_W8 a_vod8_I8
%
global c_gm1 a_gm1_W1 a_gm1_I1
global c_gm6 a_gm6_W6 a_gm6_I6
global c_go1 a_go1_W1 a_go1_I1
global c_go3 a_go3_W3 a_go3_I1
global c_go6 a_go6_W6 a_go6_I6
global c_go7 a_go7_W7 a_go7_I7
%
global c_gain a_gain_W1 a_gain_W5 a_gain_W6 a_gain_W7 a_gain_W8 a_gain_I8
global c_phase a_phase_W1 a_phase_W3 a_phase_W5 a_phase_W6
global a_phase_W7 a_phase_W8 a_phase_I8 a_phase_Cc
global c_bdw a_bdw_W1 a_bdw_W3 a_bdw_W5 a_bdw_W6 
global a_bdw_W7 a_bdw_W8 a_bdw_I8 a_bdw_Cc

global QUIET
QUIET = 1;

[design, flag] = initial(SPEC, MOS3p3);
%design.Cc = 5e-12;
if flag == 0, write_design(design); end

[status,result]=system('hspice -i opamp2.sp -o opamp2.lis');
if status ~= 0, return, end
design.Rc = 1/read_gm6();
write_design(design);
[status,result]=system('hspice -i opamp2.sp -o opamp2.lis');

radius = 0.01;
%return
%% make phase margin satisfy the 60 degree constraint
% for ii = 1 : 1
%     fprintf('--------------------------\nIteration-%d\n',ii);
%     [W,I8,Cc,Id1,Id6,Vod1,Vod3,Vod5,Vod6,Vod7,Vod8,gm1,gm6,...
%      go1,go3,go6,go7,gain,bw,phase]=sample_flow(design,radius);
%     fit_performances(W,I8,Cc,Id1,Id6,Vod1,Vod3,Vod5,Vod6,Vod7,Vod8,...
%                      gm1,gm6,go1,go3,go6,go7,gain,bw,phase);
%     [design, flag] = gp_loop_phase(design, SPEC, MOS3p3);
%     if flag == 0, write_design(design);
%     else, break
%     end
% 
%     [status,result]=system('hspice -i opamp2.sp -o opamp2.lis');
%     if status ~= 0, return, end
% 
%     design.Rc = 1/read_gm6();
%     write_design(design);
%     [status,result]=system('hspice -i opamp2.sp -o opamp2.lis');
% 
%     [gain_tmp,bdw_tmp,phase_tmp]=read_lis();
%     fprintf('Gain (dB): %4.2f\n',gain_tmp);
%     if phase_tmp >= -120, break; end
% end
%% maximize gain with constraints phase margin

[gain_tmp,bdw_tmp,phase_tmp]=read_lis();
% for ii = 1 : 60
%     fprintf('--------------------------\ngp_loop_gain Iteration-%d\n',ii);
%     [W,I8,Cc,Id1,Id6,Vod1,Vod3,Vod5,Vod6,Vod7,Vod8,gm1,gm6,go1,go3,go6,go7,...
%      gain,bw,phase]=sample_flow(design,radius);
%     fit_performances(W,I8,Cc,Id1,Id6,Vod1,Vod3,Vod5,Vod6,Vod7,Vod8,gm1,gm6,...
%                      go1,go3,go6,go7,gain,bw,phase);
%     [design_new, flag] = gp_loop_gain(design, SPEC, MOS3p3);
%     if flag == 0, 
%         design = design_new;
%         write_design(design);
%     else,
%         break;
%     end
% 
%     [status,result]=system('hspice -i opamp2.sp -o opamp2.lis');
%     if status ~= 0, return, end
% 
%     design.Rc = 1/read_gm6();
%     write_design(design);
%     [status,result]=system('hspice -i opamp2.sp -o opamp2.lis');
% 
%     [gain_tmp_new,bdw_tmp,phase_tmp]=read_lis();
%     fprintf('Gain (dB): %4.2f\n',gain_tmp_new);
%     if abs(gain_tmp_new - gain_tmp) < 0.01, break; end
%     gain_tmp = gain_tmp_new;
% end

%% maximize bandwidth with constraints phase margin and gain
for ii = 1 : 40
    fprintf('--------------------------\ngp_loop_bdw Iteration-%d\n',ii);
    [W,I8,Cc,Id1,Id6,Vod1,Vod3,Vod5,Vod6,Vod7,Vod8,gm1,gm6,go1,go3,go6,go7,...
     gain,bw,phase]=sample_flow(design,radius);
    fit_performances(W,I8,Cc,Id1,Id6,Vod1,Vod3,Vod5,Vod6,Vod7,Vod8,gm1,gm6,...
                     go1,go3,go6,go7,gain,bw,phase);
    [design_new, flag] = gp_loop_bdw(design, SPEC, MOS3p3);
    if flag == 0, 
        design = design_new;
        write_design(design);
    else,
        break;
    end

    [status,result]=system('hspice -i opamp2.sp -o opamp2.lis');
    if status ~= 0, return, end

    design.Rc = 1/read_gm6();
    write_design(design);
    [status,result]=system('hspice -i opamp2.sp -o opamp2.lis');

    [gain_tmp,bdw_tmp_new,phase_tmp]=read_lis();
    fprintf('Gain (dB): %4.2f\n',gain_tmp);
    if abs(bdw_tmp_new - bdw_tmp) < 1e3, break; end
    bdw_tmp = bdw_tmp_new;
end

write_design(design);
[status,result]=system('hspice -i opamp2.sp -o opamp2.lis');

