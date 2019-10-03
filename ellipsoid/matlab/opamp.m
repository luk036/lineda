format compact;
close all;
clear all;
clc;

%% Specification
Vdd   = 3.3;        % positive supply voltage (V)
Vss   = 0.0;        % negative supply voltage (V)
Cload = 3e-12;      % loading capacitance (F)

Lmax   = 2e-6;      % max device length (m)
Lmin   = 1e-6;      % min device length (m)
Wmax   = 1000e-6;   % max device width  (m)
Wmin   = 2e-6;      % min device width  (m)
VODmax = 0.3;       % max overdrive voltage (V)
VODmin = 0.2;       % min overdrive voltage (V)

VCMmax  = 0.45*Vdd; % max common-mode input voltage (V)
VCMmin  = 0.55*Vdd; % min common-mode input voltage (V)
VOUTmax = 0.9*Vdd;  % min output voltage (V)
VOUTmin = 0.1*Vdd;  % max output voltage (V)

AREAmax  = 1e-8;    % maximum area (m^2)
POWERmax = 5e-3;    % max quiescent power (W)
AVVmin   = 80;      % min open-loop gain (dB)
GBWmin   = 50e6;    % min unity gain bandwidth (Hz)
PMmin    = 60;      % min phase margin (degree)

CMRRmin  = 75;      % min common-mode rejection ratio (dB)
PPSRRmin = 80;      % min positive power supply rejection ratio (dB)
NPSRRmin = 80;      % min negative power supply rejection ratio (dB)

SRmin    = 50e6;    % min slew rate (V/s)
IRSNmax  = 1e-6;    % max input-referred spot noise;(nV/sqrt(Hz)@1kHz)

%% HSPICE Level 1 parameters for the technology
KBOLT = 1.38e-23;   % Boltsman Coeeficient (J/K)
TEMP  = 300;        % Room Temperature (K)
QE    = 1.60e-19;   % One electron charge (C)
VT = KBOLT*TEMP/QE; % Thermal voltage (V)

TOX = 20e-9;        % Oxide thickness (m)
COX = 34.53e-12/TOX;% oxide capacitance per unit area (F/m^2)
LD  = 0.2e-6;       % Lateral diffusion (m)

VTN     = 0.7;      % NMOS threshold voltage (V)
UN      = 600e-4;   % NMOS carrier mobility (m^2/(Vs))
KN      = UN*COX;   % NMOS intrinsic transconductance parameter (A/V^2)

LAMBDAN = 0.03;     % NMOS channel length modulation (1/V)
KFN     = 4e-24;    % NMOS flicker noise coefficient
KAN     = 1         % NMOS flicker noise exponent

CGDON   = 346e-12;  % NMOS gate-grain overlap capacitance (F/m)
CGSON   = 346e-12;  % NMOS gate-source overlap capacitance (F/m)
CJN     = 271e-6;   % NMOS zero-bias bulk junction capacitace (F/m^2)
CJSWN   = 0.6e-9;   % NMOS zero-bias sidewall bulk junction capacitance (F/m)

GAMMAN  = 0.298;    % NMOS body effect factor (V^0.5)
PHIN    = 0.688;    % NMOS surface inversion potential
RSHN    = 40;       % NMOS diffusion sheet resistance (ohm per unit area)

PBN     = 0.904;    % NMOS bulk junction contact potential (V)
MJN     = 0.5;      % NMOS bulk junction grading coefficient
MJSWN   = 0.5;      % NMOS bulk junction sidewall bulk grading coefficient

%%% TPGN = 1;
%%% JSN = 0.753e-6;
%%% NSUBN = 8e15;
%%% NLEVN = 2;

VTP     = -0.9;     % PMOS threshold voltage (V)
UP      = 200e-4;   % PMOS carrier mobility (m^2/(Vs))
KP      = UP*COX;   % PMOS intrinsic transconductance parameter (A/V^2)

LAMBDAP = 0.06;     % PMOS channel length modulation (1/V)
KFP     = 2e-24;    % PMOS flicker noise coefficient
KAN     = 1         % PMOS flicker noise exponent

CGDOP   = 346e-12;  % PMOS gate-grain overlap capacitance (F/m)
CGSOP   = 346e-12;  % PMOS gate-source overlap capacitance (F/m)
CJP     = 423e-6;   % PMOS zero-bias bulk junction capacitace (F/m^2)
CJSWP   = 1.2e-9;   % PMOS zero-bias sidewall bulk junction capacitance (F/m)

GAMMAP  = 0.471;    % PMOS body effect factor (V^0.5)
PHIP    = 0.730;    % PMOS surface inversion potential
RSHP    = 60;       % PMOS diffusion sheet resistance (ohm per unit area)

PBP     = 0.928;    % PMOS bulk junction contact potential (V)
MJP     = 0.5;      % PMOS bulk junction grading coefficient
MJSWP   = 0.5;      % PMOS bulk junction sidewall bulk grading coefficient

%%% TPGP = -1;
%%% JSP = 0.176e-6;
%%% NSUBP = 2e16;
%%% NLEVP = 2;

%% GP variables
gpvar W01 L01 W08 L08 W10 L10 W13 L13 Ibias A B; 

%% Cc and Rb
Rb = 1/sqrt(2*KN*W13/L13*Ibias);
Cc = Cload/3;

%% geometry size
L02 = L01; W02 = W01;
L03 = L13; W03 = 0.5*A*W13;
L04 = L13; W04 = 0.5*A*W13;
L05 = L08; W05 = A*W08;
L06 = L13; W06 = B*W13;
L07 = L08; W07 = B*W08;
L09 = L08; W09 = W08;
L11 = L10; W11 = W10;
L12 = L13; W12 = 4*W13;
L14 = L10; W14 = L10*B*sqrt(W10/L10*W13/L13)*Cc/(Cc+Cload);

%% current
I01 = 0.5 * A * Ibias;
I02 = 0.5 * A * Ibias;
I03 = 0.5 * A * Ibias;
I04 = 0.5 * A * Ibias;
I05 = A * Ibias;
I06 = B * Ibias;
I07 = B * Ibias;
I08 = Ibias;
I09 = Ibias;
I10 = Ibias;
I11 = Ibias;
I12 = Ibias;
I13 = Ibias;

%% overdrive voltage
Vod01 = sqrt(  2 * I01 / ( KP*W01/L01 ) );
Vod02 = sqrt(  2 * I02 / ( KP*W02/L02 ) );
Vod03 = sqrt(  2 * I03 / ( KN*W03/L03 ) );
Vod04 = sqrt(  2 * I04 / ( KN*W04/L04 ) );
Vod05 = sqrt(  2 * I05 / ( KP*W05/L05 ) );
Vod06 = sqrt(  2 * I06 / ( KN*W06/L06 ) );
Vod07 = sqrt(  2 * I07 / ( KP*W07/L07 ) );
Vod08 = sqrt(  2 * I08 / ( KP*W08/L08 ) );
Vod09 = sqrt(  2 * I09 / ( KP*W09/L09 ) );
Vod10 = sqrt(  2 * I10 / ( KN*W10/L10 ) );
Vod11 = sqrt(  2 * I11 / ( KN*W11/L11 ) );
Vod12 = sqrt(  2 * I12 / ( KN*W12/L12 ) );
Vod13 = sqrt(  2 * I13 / ( KN*W13/L13 ) );

%% Gm
Gm01 = sqrt( 2 * KP * W01 / L01 * I01 );
Gm02 = sqrt( 2 * KP * W02 / L02 * I02 );
Gm03 = sqrt( 2 * KN * W03 / L03 * I03 );
Gm04 = sqrt( 2 * KN * W04 / L04 * I04 );
Gm05 = sqrt( 2 * KP * W05 / L05 * I05 );
Gm06 = sqrt( 2 * KN * W06 / L06 * I06 );
Gm07 = sqrt( 2 * KP * W07 / L07 * I07 );
Gm08 = sqrt( 2 * KP * W08 / L08 * I08 );
Gm09 = sqrt( 2 * KP * W09 / L09 * I09 );
Gm10 = sqrt( 2 * KN * W10 / L10 * I10 );
Gm11 = sqrt( 2 * KN * W11 / L11 * I11 );
Gm12 = sqrt( 2 * KN * W12 / L12 * I12 );
Gm13 = sqrt( 2 * KN * W13 / L13 * I13 );

%% Go
Go01 = LAMBDAP * I01;
Go02 = LAMBDAP * I02;
Go03 = LAMBDAN * I03;
Go04 = LAMBDAN * I04;
Go05 = LAMBDAP * I05;
Go06 = LAMBDAN * I06;
Go07 = LAMBDAP * I07;
Go08 = LAMBDAP * I08;
Go09 = LAMBDAP * I09;
Go10 = LAMBDAN * I10;
Go11 = LAMBDAN * I11;
Go12 = LAMBDAN * I12;
Go13 = LAMBDAN * I13;

%% Cgs
Cgs01 = (2/3*W01*L01*COX + W01*LD*COX); 
Cgs02 = (2/3*W02*L02*COX + W02*LD*COX); 
Cgs03 = (2/3*W03*L03*COX + W03*LD*COX); 
Cgs04 = (2/3*W04*L04*COX + W04*LD*COX); 
Cgs05 = (2/3*W05*L05*COX + W05*LD*COX); 
Cgs06 = (2/3*W06*L06*COX + W06*LD*COX);
Cgs07 = (2/3*W07*L07*COX + W07*LD*COX); 
Cgs08 = (2/3*W08*L08*COX + W08*LD*COX); 
Cgs09 = (2/3*W09*L09*COX + W09*LD*COX); 
Cgs10 = (2/3*W10*L10*COX + W10*LD*COX); 
Cgs11 = (2/3*W11*L11*COX + W11*LD*COX); 
Cgs12 = (2/3*W12*L12*COX + W12*LD*COX); 
Cgs13 = (2/3*W13*L13*COX + W13*LD*COX); 
Cgs14 = (2/3*W14*L14*COX + W14*LD*COX); 

%% Cgd
Cgd01 = COX*W01*LD;
Cgd02 = COX*W02*LD;
Cgd03 = COX*W03*LD;
Cgd04 = COX*W04*LD;
Cgd05 = COX*W05*LD;
Cgd06 = COX*W06*LD;
Cgd07 = COX*W07*LD;
Cgd08 = COX*W08*LD;
Cgd09 = COX*W09*LD;
Cgd10 = COX*W10*LD;
Cgd11 = COX*W11*LD;
Cgd12 = COX*W12*LD;
Cgd13 = COX*W13*LD;
Cgd14 = COX*W14*LD;

%% Area
AREA1 = 100e-12;
AREA2 = 1000*Cc;
AREA3 = 8*(2*W01*L01+2*W10*L10+W14*L14+(2+A+B)*W08*L08+(5+A+B)*W13*L13);
AREA  = AREA1 + AREA2 + AREA3;

%% Quiescent Power
POWER = (2+A+B)*Ibias*(Vdd-Vss);

%% Open Loop Gain, eqn(20)
AVV = 2/(LAMBDAN+LAMBDAP)^2*sqrt(KP*KN*W01/L01*W06/L06/I01/I06); 

%% Unitity Gain Bandwidth
C1 = Cgs06 + Cgd02 + Cgd04;        % eqn(23)
C2 = Cgs03 + Cgs04 + Cgd01;        % eqn(26)
Ctl = Cload + Cgd06 + Cgd07;       % eqn(24)
C3 = (C1*Cc+C1*Ctl+Cc*Ctl)/Cc;     % eqn(22)
WP1     = Gm01/Cc/AVV;              % eqn(21)
WP2_inv = C3/Gm06;                  % eqn(22)
WP3_inv = C2/Gm03;                  % eqn(25)
WP4_inv = C1/Gm06*Cc/(Cc+Cload);    % eqn(27)
WPU = Gm01/Cc;
GBW = WPU/2/pi;

%% CMRR
CMRR = 1/LAMBDAP/(LAMBDAN+LAMBDAP)*sqrt(KP*KN*W01/L01*W03/L03/I01/I03);

%% nagetive PSRR
NPSRR = 1/LAMBDAN/(LAMBDAN+LAMBDAP)*sqrt(KP*KN*W01/L01*W06/L06/I01/I06);

%% input-referred spot noise, eqn(43)
alpha = 2*KFP/COX/W01/L01*( 1 + KFN*UN*L01*L01/(KFP*UP*L03*L03) );
beta  = (16/3)*KBOLT*TEMP/sqrt(2*KP*W01/L01*I01)* ...
        (1 + sqrt( UN*W03/L03/(UP*W01/L01) ) );
IRSN  = alpha/1000 + beta;


%% set of constraints expressed as an array
constr = [
    %% Area
    %% AREA2 <= (AREAmax - AREA1);
    
    %% Power, eqn(17)
    POWER <= POWERmax;
    
    % open loop gain, eqn(20)
    10^(AVVmin/20) <= AVV;  
    
    % unitity gain bandwith, eqn(34)
    GBWmin <= GBW;

    % Phase Margin, eqn(36)
    WPU*(WP3_inv + WP4_inv) <= pi/2 - PMmin*pi/180;

    % CMRR, eqn(38)
    10^(CMRRmin/20) <= CMRR; 

    % NPSRR, eqn(40)
    10^(NPSRRmin/20) <= NPSRR;
    
    % slew rate, eqn(37)
    Cc/I05 <= 1/SRmin;
    (Cc+Ctl)/I07 <= 1/SRmin;
    
    % input-referred spot noise, eqn(43)
    IRSN <= IRSNmax^2;

    %% size limitation.
    Wmin <= W01; W01 <= Wmax; Lmin <= L01; L01 <= Lmax;
    Wmin <= W02; W02 <= Wmax; Lmin <= L02; L02 <= Lmax;
    Wmin <= W03; W03 <= Wmax; Lmin <= L03; L03 <= Lmax;
    Wmin <= W04; W04 <= Wmax; Lmin <= L04; L04 <= Lmax;
    Wmin <= W05; W05 <= Wmax; Lmin <= L05; L05 <= Lmax;
    Wmin <= W06; W06 <= Wmax; Lmin <= L06; L06 <= Lmax;
    Wmin <= W07; W07 <= Wmax; Lmin <= L07; L07 <= Lmax;
    Wmin <= W08; W08 <= Wmax; Lmin <= L08; L08 <= Lmax;
    Wmin <= W09; W09 <= Wmax; Lmin <= L09; L09 <= Lmax;
    Wmin <= W10; W10 <= Wmax; Lmin <= L10; L10 <= Lmax;
    Wmin <= W11; W11 <= Wmax; Lmin <= L11; L11 <= Lmax;
    Wmin <= W12; W12 <= Wmax; Lmin <= L12; L12 <= Lmax;
    Wmin <= W13; W13 <= Wmax; Lmin <= L13; L13 <= Lmax;
    Wmin <= W14; W14 <= Wmax; Lmin <= L14; L14 <= Lmax;

    %% overdrive voltage, eqn(16)
    VODmin <= Vod01; Vod01 <= VODmax;
    VODmin <= Vod02; Vod02 <= VODmax;
    VODmin <= Vod03; Vod03 <= VODmax;
    VODmin <= Vod04; Vod04 <= VODmax;
    VODmin <= Vod05; Vod05 <= VODmax;
    VODmin <= Vod06; Vod06 <= VODmax;
    VODmin <= Vod07; Vod07 <= VODmax;
    VODmin <= Vod08; Vod08 <= VODmax;
    VODmin <= Vod09; Vod09 <= VODmax;
    VODmin <= Vod10; Vod10 <= VODmax;
    VODmin <= Vod11; Vod11 <= VODmax;
       0.1 <= Vod12; Vod12 <= VODmax;
    VODmin <= Vod13; Vod13 <= VODmax;

    %% input common voltage, eqn(12)(13)
    Vod01 <= VCMmin - Vss - VTP - VTN;
    Vod01 + Vod05 <= Vdd - VCMmax + VTP;
    
    %% output voltage, eqn(14)(15)
    Vod06 <= VOUTmin - Vss;
    Vod07 <= Vdd - VOUTmax;
    
    % 2,3,4-pole more than 10 times of 1-pole, eqn(32) 
    10 * WP1 * WP2_inv <= 1;
    10 * WP1 * WP3_inv <= 1;
    10 * WP1 * WP4_inv <= 1;
    
    % 2,3,4-pole more than 2.5 times of unitity gain bandwidth
    2.5 * WPU * WP2_inv <= 1;
    2.5 * WPU * WP3_inv <= 1;
    2.5 * WPU * WP4_inv <= 1;
    ];

%% solve the GP
[area_min, solution, status] = gpsolve(AREA, constr, 'min')

%% convert the GP variables to doubles, the optimal values found
assign(solution);
Rb = 1/sqrt(2*KN*W13/L13*Ibias) 
