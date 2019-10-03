clear all
% problem data
Vdd = 5.0;          % positive supply voltage (V)
Vss = 0.0;          % negative supply voltage (V)
CL = 3e-12;         % loading capacitance (F)
% Specification
Lmax = 10e-6;       % max device length (m)
Wmax = 1000e-6;     % max device width  (m)
Lmin = 0.8e-6;      % min device length (m)
Wmin = 2e-6;        % min device width  (m)
Amax = 10000e-12;   % maximum area (m^2)

Vcmmax = 0.5*Vdd;   % max common-mode input voltage (V)
Vcmmin = 0.5*Vdd;   % min common-mode input voltage (V)
Voutmin = 0.1*Vdd;  % max output voltage (V)
Voutmax = 0.9*Vdd;  % min output voltage (V)
Pmax = 15e-3;       % max quiescent power (W)
deltaVo = 0.1;      % overdrive voltage (V)

Gmin = 80;          % min open-loop gain (dB)
Omega = 60e6;       % Unity Gain Bandwidth              
PMmin = pi/3;       % min phase margin (degree)

SRmin = 10e6;       % min slew rate (V/s)
CMRRmin = 60;       % min common-mode rejection ratio (dB)
FreqPower = 50;     % Power Noise Frequency (Hz)
NPSRRmin = 80;      % min neg. power supply rejection ratio (dB)
PPSRRmin = 80;      % min pos. power supply rejection ratio (dB)
Irsn = 300e-9;      % Input-referred spot noise; 1kHz (nV/sqrt(Hz))
 
%% HSPICE Level 1 parameters for the technology
Temp = 300;         % Room Temperature (K)
KBnoise = 1.38e-23; % Boltsman Coeeficient (J/K)
TOX = 20e-9;        % Oxide thickness (m)
LD = 0.2e-6;        % Lateral diffusion (m)
COX = 3.45e-11/TOX; % oxide capacitance per unit area (F/m^2)

VTN = 0.7;          % NMOS threshold voltage (V)
LAMBDAN = 0.03;     % NMOS channel length modulation (V^-1)
UN = 600e-4;        % NMOS carrier mobility (m^2/(Vs))
KN = UN*COX;        % NMOS intrinsic transconductance parameter (A/V^2)
GAMMAN = 0.298;     % NMOS body effect factor (V^0.5)
PHIN = 0.688;       % NMOS surface inversion potential
RSHN = 40;          % NMOS diffusion sheet resistance (ohm per unit area)
CJN = 271e-6;       % NMOS zero-bias bulk junction capacitace (F/m^2)
CJSWN = 600e-6;     % NMOS zero-bias sidewall bulk junction capacitance (F/m)
MJN = 0.5;          % NMOS bulk junction grading coefficient (1)
PBN = 0.904;        % NMOS bulk junction contact potential (V)
CGDON = 346e-6;     % NMOS gate-grain overlap capacitance (F/m)
CGSON = 346e-6;     % NMOS gate-source overlap capacitance (F/m)
KFN = 4e-24;        % NMOS flicker noise coefficient

VTP = -0.9;         % PMOS threshold voltage (V)
LAMBDAP = 0.06;     % PMOS channel length modulation (V^-1)
UP = 200e-4;        % PMOS carrier mobility (m^2/(Vs))
KP = UP*COX  ;      % PMOS intrinsic transconductance parameter (A/V^2)
GAMMAP = 0.471;     % PMOS body effect factor (V^0.5)
PHIP = 0.730;       % PMOS surface inversion potential
RSHP = 60;          % PMOS diffusion sheet resistance (ohm per unit area)
CJP = 423e-6;       % PMOS zero-bias bulk junction capacitace (F/m^2)
CJSWP = 1.2e-9;     % PMOS zero-bias sidewall bulk junction capacitance (F/m)
MJP = 0.5;          % PMOS bulk junction grading coefficient (1)
PBP = 0.928;        % PMOS bulk junction contact potential (V)
CGDOP = 346e-6;     % PMOS gate-grain overlap capacitance (F/m)
CGSOP = 346e-6;     % PMOS gate-source overlap capacitance (F/m)
KFP = 2e-24;        % PMOS flicker noise coefficient
  
%% GP variables
gpvar W1 W3 W5 W6 W7 W8 L1 L3 L5 L6 Cc Ibias; 

%% object function is the area of the chip
area = 2*W1*L1 + 2*W3*L3 + W5*L5 + W6*L6 + W7*L5 + W8*L5;

W2 = W1; W4 = W3; L2 = L1; L4 = L3; L7 = L5; L8 = L5;
%% W1 = W2, W3 = W4, L1 = L2, L3 = L4, L5 = L7 = L8
I5 = (W5/L5)/(W8/L8) * Ibias; %% eqn(10)
I7 = (W7/L5)/(W8/L5) * Ibias;
I1 = 0.5 * I5;                %% eqn(11)
Gm1 = sqrt(2*KP*I1*W1/L1);
Gm3 = sqrt(2*KN*I1*W3/L3);
Gm6 = sqrt(2*KN*I7*W6/L6);    %% eqn(51)
Go2 = LAMBDAP*I1;
Go4 = LAMBDAN*I1;
Go6 = LAMBDAN*I7;             %% eqn(52)
Rc = 1/Gm6;                   %% eqn(18)
Av = 2*COX/(LAMBDAN+LAMBDAP)/(LAMBDAN+LAMBDAP) ...
     *sqrt(UN*UP*W2*W6/(L2*L6*I1*I7)); 
                              %% eqn(20)
Ap_inv = (Go2+Go4)*Go6/(Gm1*Gm6);
                              %% eqn(40)
p1 = Gm1/(Av*Cc);             %% eqn(21)
Cgs3 = (2/3*W3*L3*COX + W3*LD*COX); 
Cgs4 = (2/3*W4*L4*COX + W4*LD*COX); 
Cgs6 = (2/3*W6*L6*COX + W6*LD*COX);
                              %% eqn(53)
Cgd1 = COX*W1*LD;
Cgd2 = COX*W2*LD;
Cgd4 = COX*W4*LD;
Cgd6 = COX*W6*LD;
Cgd7 = COX*W7*LD;             %% eqn(57)
C1 = Cgs6 + Cgd2 + Cgd4;      %% eqn(23)
C2 = Cgs3 + Cgs4 + Cgd1;      %% eqn(26)
CTL = CL + Cgd6 + Cgd7;       %% eqn(24)
p2_inv = (C1*Cc + C1*CTL + Cc*CTL) / (Gm6*Cc); %% eqn(22)
p3_inv = C2/Gm3;              %% eqn(25)
p4_inv = C1/Gm6;              %% eqn(27)
alpha = 2*KP/COX/W1/L1*(1 + KN*UN*L1*L1/(KP*UP*L3*L3));
                              %% eqn(43)
beta = 16*KBnoise*Temp/3/sqrt(2*KP*W1/L1*I1)* ...
       (1 + sqrt(UN*W3/L3/(UP*W1/L1))); 
                              %% eqn(43)
%% set of constraints expressed as an array
constr = ...
   [
%% eqn(7,8)
%%           W1 <= Wmax; W3 <= Wmax; W5 <= Wmax; 
%%           W6 <= Wmax; W7 <= Wmax; W8 <= Wmax;
%%           L1 <= Lmax; L3 <= Lmax; L5 <= Lmax; L6 <= Lmax;
     Wmin <= W1; Wmin <= W3; Wmin <= W5; 
     Wmin <= W6; Wmin <= W7; Wmin <= W8;
     Lmin <= L1; Lmin <= L3; Lmin <= L5; Lmin <= L6;
     %% eqn(9)
     ((W3/L3)/(W6/L6))/((W5/L5)/(W7/L7)) == 0.5;
     %% eqn(12)
     sqrt( I1 / (0.5*KN*(W3/L3))) <= Vcmmin - Vss - VTP - VTN;
     %% eqn(13)
     sqrt(I1/(0.5*KP*(W1/L1))) + sqrt(I5/(0.5*KP*(W5/L5))) ...  
     <= Vdd - Vcmmax + VTP;
     %% eqn(14) 
     sqrt(I7/(0.5*KN*(W6/L6))) <= Voutmin - Vss;
     %% eqn(15)
     sqrt(I7/(0.5*KP*(W7/L7))) <= Vdd - Voutmax;
     %% eqn(16), over-drive
     deltaVo <= sqrt(I1/(0.5*KP*W1/L1)); %% M1
     deltaVo <= sqrt(I1/(0.5*KN*W3/L3)); %% M3
     deltaVo <= sqrt(I5/(0.5*KP*W5/L5)); %% M5
     deltaVo <= sqrt(I7/(0.5*KN*W6/L6)); %% M6 
     deltaVo <= sqrt(I7/(0.5*KP*W7/L7)); %% M7
     deltaVo <= sqrt(Ibias/(0.5*KP*W8/L8)); %% M8, above are over-drive voltage,  
     %% eqn(17)
     (Ibias + I5 + I7)*(Vdd-Vss) <= Pmax;
     %% eqn(20)
     10^(Gmin/20) <= Av;  
     %% eqn(31) 3-dB bandwidth
     1e-12 <= Cc;  
     %% eqn(32) 
     p1*p2_inv <= 0.1;
     p1*p3_inv <= 0.1;
     p1*p4_inv <= 0.1;
     %% eqn(34)
     Omega <= Gm1/Cc;
     %% eqn(36), Phase Margin
     Omega*p2_inv + Omega*p3_inv + Omega*p4_inv <= pi/2 - PMmin;
     %% eqn(37), slew rate
     Cc/(2*I1) <= 1/SRmin;
     (Cc + CTL)/I7 <= 1/SRmin;
     %% eqn(38), CMRR
     10^(CMRRmin/20) <= COX/LAMBDAP/(LAMBDAN+LAMBDAP)* ...
      sqrt(UN*UP*W1*W3/(L1*L3*I5*I5)); 
     %% eqn(40)
     Ap_inv <= 1/10^(NPSRRmin/20);
          %% eqn(41)
%          Ap_inv*Ap_inv*(1 + FreqPower*FreqPower/p1/p1)* ...
%          (1 + FreqPower*FreqPower*p2_inv*p2_inv) ...
%          <= 1/(10^(NPSRRmin/20))^2;
          %% eqn(43)
%          alpha/100000 + beta <= Irsn*Irsn;
   ];

%% solve the GP
[min_area, solution, status] = gpsolve(area, constr, 'min')

%% convert the GP variables to doubles, the optimal values found
assign(solution);


