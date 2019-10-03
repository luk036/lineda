function gain=eval_gain(W1,W5,W6,W7,W8,I8)
% %
% global c_gm1 a_gm1_W1 a_gm1_I1
% global c_gm6 a_gm6_W6 a_gm6_I6
% global c_go1 a_go1_W1 a_go1_I1
% global c_go3 a_go3_W3 a_go3_I1
% global c_go6 a_go6_W6 a_go6_I6
% global c_go7 a_go7_W7 a_go7_I7
% %
% I5 = W5/W8 *I8;
% I1 = 0.5 *I5;
% I6 = W7/W8 *I8;
% 
% gm1 = eval_gm(c_gm1,a_gm1_W1,a_gm1_I1, W1, I1);
% gm6 = eval_gm(c_gm6,a_gm6_W6,a_gm6_I6, W6, I6);
% 
% go1 = eval_go(c_go1,a_go1_W1,a_go1_I1, W1, I1);
% go3 = eval_go(c_go3,a_go3_W3,a_go3_I1, W3, I1);
% go6 = eval_go(c_go6,a_go6_W6,a_go6_I6, W6, I6);
% go7 = eval_go(c_go7,a_go7_W7,a_go7_I7, W7, I6);
% 
% gain = gm1*gm6/(go1+go3)/(go6+go7);

global c_gain a_gain_W1 a_gain_W5 a_gain_W6 a_gain_W7 a_gain_W8 a_gain_I8
gain = c_gain * W1^a_gain_W1 * W5^a_gain_W5 * W6^a_gain_W6...
     * W7^a_gain_W7 * W8^a_gain_W8 * I8^a_gain_I8; %% Eq. 20
