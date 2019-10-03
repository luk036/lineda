clear all, close all
% coefs matrices in quadratic approx. model
global A_d2 A_d3 B_d2 B_d3 C_d2 C_d3

tic
Rc = [100,100]'; % initial center
Rr = 50; % searching radius at the start point
for ii = 1 : 3
    [R,d2,d3]=sample_flow(Rc,Rr);
    [Qnew_d2,Q_d2,B_d2,C_d2,Qnew_d3,Q_d3,B_d3,C_d3]=fit_test_XL(R,d2,d3);

    A_d2 = Qnew_d2*Q_d2';
    A_d3 = Qnew_d3*Q_d3';

    x0 = Rc;     % Make a starting guess at the solution
    lb = [50,50]';     % Set lower bounds
    ub = [ ];          % No upper bounds
    options = optimset('LargeScale','off','MaxFunEvals',500);
    [x,fval] = ... 
        fmincon(@objfun,x0,[],[],[],[],lb,ub,@confun,options)

    Rc = x;    % new center
    Rr = Rr/5; % reduce the searching radius
end

t = toc;
