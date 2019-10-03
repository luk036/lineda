function [design, flag] = gp_loop_phase(design,SPEC, MOS3p3)

global c_vod1 a_vod1_W1 a_vod1_W5 a_vod1_W8 a_vod1_I8
global c_vod3 a_vod3_W3 a_vod3_W5 a_vod3_W8 a_vod3_I8
global c_vod5 a_vod5_W8 a_vod5_I8
global c_vod6 a_vod6_W6 a_vod6_W7 a_vod6_W8 a_vod6_I8
global c_vod7 a_vod7_W8 a_vod7_I8
global c_vod8 a_vod8_W8 a_vod8_I8
%
global c_gm1 a_gm1_W1 a_gm1_W5 a_gm1_W8 a_gm1_I8
global c_gm6 a_gm6_W6 a_gm6_W7 a_gm6_W8 a_gm6_I8
global c_go1 a_go1_W5 a_go1_W8 a_go1_I8
global c_go3 a_go3_W5 a_go3_W8 a_go3_I8
global c_go6 a_go6_W7 a_go6_W8 a_go6_I8
global c_go7 a_go7_W7 a_go7_W8 a_go7_I8
%   
global c_gain a_gain_W1 a_gain_W5 a_gain_W6 a_gain_W7 a_gain_W8 a_gain_I8
global c_phase a_phase_W1 a_phase_W3 a_phase_W5 a_phase_W6
global a_phase_W7 a_phase_W8 a_phase_I8 a_phase_Cc  
global c_bdw a_bdw_W1 a_bdw_W3 a_bdw_W5 a_bdw_W6 
global a_bdw_W7 a_bdw_W8 a_bdw_I8 a_bdw_Cc

global QUIET;

global L
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
              Cc <= SPEC.Ccmax;
              I8 <= SPEC.Imax;
];

constr = [ constr;   % Eq. 7
    design.W1*0.95 <= W1; W1 <= design.W1*1.05;
    design.W3*0.95 <= W3; W3 <= design.W3*1.05;
    design.W5*0.95 <= W5; W5 <= design.W5*1.05;
    design.W6*0.95 <= W6; W6 <= design.W6*1.05;
    design.W7*0.95 <= W7; W7 <= design.W7*1.05;
    design.W8*0.95 <= W8; W8 <= design.W8*1.05;
    design.I8*0.95 <= I8; I8 <= design.I8*1.05;
    design.Cc*0.95 <= Cc; Cc <= design.Cc*1.05;       
];
%% systematic input offset
constr = [ constr;
           (W3/W6) / (W5/W7) == 1/2; ];  % Eq. 9

%% current
I5 = W5/W8 * I8;  % Eq. 10
I7 = W7/W8 * I8;
I1 = 0.5 * I5;       % Eq. 11

%% Saturation
Vod1 = c_vod1 * W1^a_vod1_W1 * W5^a_vod1_W5 * W8^a_vod1_W8 * I8^a_vod1_I8;
Vod3 = c_vod3 * W3^a_vod3_W3 * W5^a_vod3_W5 * W8^a_vod3_W8 * I8^a_vod3_I8;
Vod5 = c_vod5 * W8^a_vod5_W8 * I8^a_vod5_I8;
Vod6 = c_vod6 * W6^a_vod6_W6 * W7^a_vod6_W7 * W8^a_vod6_W8 * I8^a_vod6_I8;
Vod7 = c_vod7 * W8^a_vod7_W8 * I8^a_vod7_I8;
Vod8 = c_vod8 * W8^a_vod8_W8 * I8^a_vod8_I8;

constr = [constr;
    Vod3 <= SPEC.VCMmin - MOS3p3.Vth(1) - MOS3p3.Vth(2); % Eq. 12
    Vod1 + Vod5 <= SPEC.Vdd - SPEC.VCMmax + MOS3p3.Vth(2); % Eq. 13
    Vod6 <= SPEC.VOUTmin; % Eq. 14
    Vod7 <= SPEC.Vdd - SPEC.VOUTmax;
];

%% overdrive voltage
constr = [ constr;  % Eq. 16
    1/Vod1 <= 1/SPEC.VODmin;
    1/Vod3 <= 1/SPEC.VODmin; 
    1/Vod3 <= 1/0.4;%1/SPEC.VODmin;
    1/Vod5 <= 1/SPEC.VODmin;
    1/Vod6 <= 1/SPEC.VODmin;
    1/Vod7 <= 1/SPEC.VODmin;
    1/Vod8 <= 1/SPEC.VODmin;
    Vod8 <= SPEC.VODmax;
];

%% power
power = SPEC.Vdd*(I8 + I7 + I5); % Eq. 17

% constr = [constr;
%           power <= SPEC.power;
%          ]; 

Av = c_gain * W1^a_gain_W1 * W5^a_gain_W5 * W6^a_gain_W6...
     * W7^a_gain_W7 * W8^a_gain_W8 * I8^a_gain_I8; %% Eq. 20
phase = c_phase * W1^a_phase_W1 * W3^a_phase_W3 * W5^a_phase_W5...
        * W6^a_phase_W6 * W7^a_phase_W7 * W8^a_phase_W8...
        * I8^a_phase_I8 * Cc^a_phase_Cc;
bdw =  c_bdw * W1^a_bdw_W1 * W3^a_bdw_W3 * W5^a_bdw_W5...
       * W6^a_bdw_W6 * W7^a_bdw_W7 * W8^a_bdw_W8...
       * I8^a_bdw_I8 * Cc^a_bdw_Cc;
%p1 = gm1/(Av*Cc);  %% Eq. 21

constr = [constr;  
          1/Av <= 1/1000;
%          Av <= 5000;
          1/bdw <= 1/1e6;
%          phase <= 170;
          ];

%% Area
area = L*(2*W1 + 2*W3 + W5 + W6 + W7 + W8);

%% solve the GP
[phase_min, solution, status] = gpsolve(phase, constr, 'min')
if strcmp(status(1:2),'So') == 0,
    flag = 1; design = []; return, 
end
%% convert the GP variables to doubles, the optimal values found
assign(solution);

%solution(1,2)
I5 = W5/W8 * I8;  % Eq. 10
I7 = W7/W8 * I8; 

design = struct(...
    'L', L, 'W1', W1, 'W3', W3, 'W5', W5, 'W6', W6,...
    'W7', W7, 'W8', W8, 'Rc', design.Rc, 'Cc', Cc, 'I8', I8...
    );
flag = 0; % SUCCESS

gain_tmp = eval_gain(c_gain,a_gain_W1,a_gain_W5,a_gain_W6,...
                     a_gain_W7,a_gain_W8,a_gain_I8,...
                     W1,W5,W6,W7,W8,I8)
bdw_tmp = eval_bdw(c_bdw,a_bdw_W1,a_bdw_W3,a_bdw_W5,a_bdw_W6,...
                   a_bdw_W7,a_bdw_W8,a_bdw_I8,a_bdw_Cc,...
                   W1,W3,W5,W6,W7,W8,I8,Cc)
