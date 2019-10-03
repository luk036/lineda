
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
                              
%%  Display Information
disp(['Eqn(9), Systematic Input Offset Vlotage: ', ...
      num2str((W3/L3)/(W6/L6)/((W5/L5)/(W7/L5)))]);
disp(['------- Saturating:']);
disp(['Eqn(12), Transistor M1: ', ...
      num2str(sqrt( I1 / (0.5*KN*(W3/L3)))), ' <= ', ...
      num2str(Vcmmin - Vss - VTP - VTN)]);
disp(['Eqn(13), Transistor M5: ', ...
      num2str(sqrt( I1 / (0.5*KP*(W1/L1))) + sqrt( I5 / (0.5*KP*(W5/L5)))), ...
      ' <= ', num2str(Vdd - Vcmmax + VTP)]);
disp(['Eqn(14), Transistor M6: ', ...
      num2str(sqrt( I7 / (0.5*KN*(W6/L6)))), ' <= ', ...
      num2str(Voutmin - Vss)]);
disp(['Eqn(15), Transistor M7: ', ...
      num2str(sqrt( I7 / (0.5*KP*(W7/L5)))), ' <= ', ...
      num2str(Vdd - Voutmax)]);
disp(['------- Over drive voltage:']);
disp(['Eqn(16), Transistor M1: ', ...
      num2str(sqrt( I1 /(0.5*KP*W1/L1))), ' >= ', ...
      num2str(deltaVo)]);
disp(['Eqn(16), Transistor M3: ', ...
      num2str(sqrt( I1 /(0.5*KN*W3/L3))), ' >= ', ...
      num2str(deltaVo)]);
disp(['Eqn(16), Transistor M5: ', ...
      num2str(sqrt( I5 /(0.5*KP*W5/L5))), ' >= ', ...
      num2str(deltaVo)]);
disp(['Eqn(16), Transistor M6: ', ...
      num2str(sqrt( I7 /(0.5*KN*W6/L6))), ' >= ', ...
      num2str(deltaVo)]);
disp(['Eqn(16), Transistor M7: ', ...
      num2str(sqrt( I7 /(0.5*KP*W7/L5))), ' >= ', ...
      num2str(deltaVo)]);
disp(['Eqn(16), Transistor M8: ', ...
      num2str(sqrt( Ibias /(0.5*KP*W8/L5))), ' >= ', ...
      num2str(deltaVo)]);
disp(['Av: ', num2str(Av)]);
disp(['Rc: ', num2str(Rc)]);
