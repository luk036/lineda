function [c, ceq] = confun(x)
    global A_gain B_gain C_gain A_phase B_phase C_phase
    global c_vod1 a_vod1_W1 a_vod1_W5 a_vod1_W8 a_vod1_Ibias
    global c_vod3 a_vod3_W3 a_vod3_W5 a_vod3_W8 a_vod3_Ibias
    global c_vod5 a_vod5_W8 a_vod5_Ibias
    
    global SPEC MOS3p3
% Nonlinear inequality constraints
    W1 = x(1);  W3 = x(2);  W5 = x(3);  W6 = x(4);
    W7 = x(5);  W8 = x(6);
    I8 = x(7); 
    xn = 1e6*x;
c = [
     eval_vdsat3(c_vod3,a_vod3_W3,a_vod3_W5,a_vod3_W8,a_vod3_Ibias,...
                 W3,W5,W8,I8)...
     - SPEC.VCMmin + MOS3p3.Vth(2) + MOS3p3.Vth(1); 
     %%
     eval_vdsat1(c_vod1,a_vod1_W1,a_vod1_W5,a_vod1_W8,a_vod1_Ibias,...
                 W1,W5,W8,I8)...
     + eval_vdsat5(c_vod5,a_vod5_W8,a_vod5_Ibias,W8,I8)
     - SPEC.Vdd + SPEC.VCMmax - MOS3p3.Vth(2); 
     %%
    
%     60 - (xn'*A_gain*xn + B_gain'*xn + C_gain); % gain
%     (xn'*A_gain*xn + B_gain'*xn + C_gain) - 70;
%     100 - (xn'*A_phase*xn + B_phase'*xn + C_phase); % phase
%     (xn'*A_phase*xn + B_phase'*xn + C_phase) - 130;
    ];
% Nonlinear equality constraints
ceq = [];
