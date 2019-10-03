%% initial.m  Oct 10th, 2007
%% Author: XXLiu
%% This MATLAB fuctions as an initial guess to the sizing of opamp.
%% For an opamp to be in operation, all MOS have to be in saturation.
%% In addition, low level approximation are given as constraints
function [design, flag] = initial(SPEC, MOS3p3)
global L    
global QUIET;

%% GP variables
gpvar W1  W3  W5  W6  W7  W8  I8  Cc; 

%% size limitation.
constr = [];
constr = [ constr;   % Eq. 7
    SPEC.Wmin <= W1; W1 <= SPEC.Wmax;
    SPEC.Wmin <= W3; W3 <= SPEC.Wmax;
    SPEC.Wmin <= W5; W5 <= SPEC.Wmax;
    SPEC.Wmin <= W6; W6 <= SPEC.Wmax;
    SPEC.Wmin <= W7; W7 <= SPEC.Wmax;
    SPEC.Wmin <= W8; W8 <= SPEC.Wmax;
              I8 <= SPEC.Imax;
              Cc <= SPEC.Ccmax;
];

%% systematic input offset
constr = [ constr;
           (W3/W6) / (W5/W7) == 1/2; ];  % Eq. 9

%% current
I5 = W5/W8 * I8;  % Eq. 10
I7 = W7/W8 * I8;
I1 = 0.5 * I5;       % Eq. 11

constr = [ constr;
           1/Cc <= 1/(0.22*SPEC.Cload);
           1/I5 <= 1/(SPEC.SR*Cc);
         ];

%% Saturation
Vod1 = sqrt(  2*I1*L / ( MOS3p3.KP*W1 ) );
Vod3 = sqrt(  2*I1*L / ( MOS3p3.KN*W3 ) );
Vod5 = sqrt(  2*I5*L / ( MOS3p3.KP*W5 ) );
Vod6 = sqrt(  2*I7*L / ( MOS3p3.KN*W6 ) );
Vod7 = sqrt(  2*I7*L / ( MOS3p3.KP*W7 ) );
Vod8 = sqrt(  2*I8*L / ( MOS3p3.KP*W8 ) );

constr = [constr;
    Vod3 <= SPEC.VCMmin - MOS3p3.Vth(1) - MOS3p3.Vth(2); % Eq. 12
    Vod1 + Vod5 <= SPEC.Vdd - SPEC.VCMmax + MOS3p3.Vth(2); % Eq. 13
    Vod6 <= SPEC.VOUTmin; % Eq. 14
    Vod7 <= SPEC.Vdd - SPEC.VOUTmax;
];

Vod_cur_mir = 0.1;
%% overdrive voltage
constr = [ constr;  % Eq. 16
    1/Vod1 <= 1/SPEC.VODmin;
    Vod1 <= 0.3;%SPEC.VODmax;       
    1/Vod3 <= 1/0.22;%SPEC.VODmin; % 
    1/Vod5 <= 1/Vod_cur_mir;%SPEC.VODmin;
                            %1/Vod6 <= 1/SPEC.VODmin;
    1/Vod7 <= 1/Vod_cur_mir;%SPEC.VODmin;
    1/Vod8 <= 1/Vod_cur_mir;%SPEC.VODmin;
                            %    Vod8 <= 0.2;%SPEC.VODmax;
];

%% power
power = SPEC.Vdd*(I8 + I7 + I5); % Eq. 17

constr = [constr;
          power <= SPEC.power;
         ]; 

%% Gm
gm1 = sqrt( 2 * MOS3p3.KP * W1 / L * I1 );
gm3 = sqrt( 2 * MOS3p3.KN * W3 / L * I1 );
gm6 = sqrt( 2 * MOS3p3.KN * W6 / L * I7 );

Rc = 1/gm6;  % Eq. 18

Av = 2*MOS3p3.Cox/(MOS3p3.lambda(1) + MOS3p3.lambda(2))^2 *...
     sqrt( MOS3p3.Uo(1)*MOS3p3.Uo(2)*W1*W6/(L*L*I1*I7) ); %% Eq. 20
p1 = gm1/(Av*Cc);  %% Eq. 21

constr = [constr;  
          gm1 == SPEC.UGF*Cc;
          10*gm1 == gm6;
          1/Av <= 1/1e4;
%          Av <= 10000;
%          1/p1 <= 1/1e8;
         ]; 

%% Area
area = L*(2*W1 + 2*W3 + W5 + W6 + W7 + W8);

%% solve the GP
[Av_max, solution, status] = gpsolve(Av, constr, 'max')
%[area_min, solution, status] = gpsolve(area, constr, 'min')
if strcmp(status(1:2),'So') == 0,
    flag = 1; design = []; return, 
end
%% convert the GP variables to doubles, the optimal values found
assign(solution);

%solution(1,2)
I5 = W5/W8 * I8;  % Eq. 10
I1 = 0.5 * I5;
I7 = W7/W8 * I8; 
gm6 = sqrt( 2 * MOS3p3.KN * W6 / L * I7 );
Rc = 1/gm6;  % Eq. 18

design = struct(...
    'L', L, 'W1', W1, 'W3', W3, 'W5', W5, 'W6', W6,...
    'W7', W7, 'W8', W8, 'I8', I8...
    );
flag = 0; % SUCCESS

