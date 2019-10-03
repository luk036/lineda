function fit_performances(W,I8,Cc,Id1,Id6,Vod1,Vod3,Vod5,Vod6,Vod7,Vod8,gm1,gm6,...
                          go1,go3,go6,go7,gain,bw,phase)

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

W1 = W(1,:); W3 = W(2,:); W5 = W(3,:); W6 = W(4,:);
W7 = W(5,:); W8 = W(6,:);
    
% Vod1
[c_vod1,a_vod1_W1,a_vod1_W5,a_vod1_W8,a_vod1_I8]=...
    mono_vdsat1(W1,W5,W8,I8,Vod1);
% Vod3
[c_vod3,a_vod3_W3,a_vod3_W5,a_vod3_W8,a_vod3_I8]=...
    mono_vdsat3(W3,W5,W8,I8,Vod3);
% Vod5
[c_vod5,a_vod5_W8,a_vod5_I8]=mono_vdsat5(W8,I8,Vod5);
% Vod6
[c_vod6,a_vod6_W6,a_vod6_W7,a_vod6_W8,a_vod6_I8]=...
    mono_vdsat6(W6,W7,W8,I8,Vod6);
% Vod7
[c_vod7,a_vod7_W8,a_vod7_I8]=mono_vdsat7(W8,I8,Vod7);
% Vod8
[c_vod8,a_vod8_W8,a_vod8_I8]=mono_vdsat8(W8,I8,Vod8);
% gm1
[c_gm1,a_gm1_W1,a_gm1_I1]=mono_gm1(W1,Id1, gm1);
% gm6
[c_gm6,a_gm6_W6,a_gm6_I6]=mono_gm6(W6,Id6, gm6);
% go1
[c_go1,a_go1_W1,a_go1_I1]=mono_go(W1,Id1, go1);
% go3
[c_go3,a_go3_W3,a_go3_I1]=mono_go(W3,Id1, go3);
% go6
[c_go6,a_go6_W6,a_go6_I6]=mono_go(W6,Id6, go6);
% go7
[c_go7,a_go7_W7,a_go7_I7]=mono_go(W7,Id6, go7);
% gain
[c_gain,a_gain_W1,a_gain_W5,a_gain_W6,a_gain_W7,a_gain_W8,a_gain_I8]=...
    mono_gain(W1,W5,W6,W7,W8,I8,gain');
% phase
[c_phase,a_phase_W1,a_phase_W3,a_phase_W5,a_phase_W6,...
 a_phase_W7,a_phase_W8,a_phase_I8,a_phase_Cc]=...
        mono_phase(W1,W3,W5,W6,W7,W8,I8,Cc, phase');
% bandwidth
[c_bdw,a_bdw_W1,a_bdw_W3,a_bdw_W5,a_bdw_W6,a_bdw_W7,a_bdw_W8,a_bdw_I8,a_bdw_Cc]=...
    mono_bdw(W1,W3,W5,W6,W7,W8,I8,Cc, bw');

