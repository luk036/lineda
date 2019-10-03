function [c, ceq] = confun(x)
    global A_d2 B_d2 C_d2
    xx = [x(1) x(2)]';
% Nonlinear inequality constraints
c = [50 - xx;
    1.5 - (xx'*A_d2*xx + B_d2'*xx + C_d2)];

ceq = []; % Nonlinear equality constraints


