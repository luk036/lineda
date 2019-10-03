function [Qnew_d2,Q_d2,B_d2,C_d2,Qnew_d3,Q_d3,B_d3,C_d3]=fit_test_XL(R,d2,d3)
x = [R]; 
f_d2 = d2.*1e9; % scaling is necessary
f_d3 = d3.*1e9;

ip = 1;
[Qnew_d2, Q_d2, B_d2, C_d2] = rank_p_c(x,f_d2,ip,800);
f_p_d2 = fit_rank_p_c(Qnew_d2,Q_d2,B_d2,C_d2,x,ip);
error(ip) = norm(f_p_d2-f_d2);
fprintf('error is %6.4e\n',error(ip));

[Qnew_d3, Q_d3, B_d3, C_d3] = rank_p_c(x,f_d3,ip,800);
f_p_d3 = fit_rank_p_c(Qnew_d3,Q_d3,B_d3,C_d3,x,ip);
error(ip) = norm(f_p_d3-f_d3);
fprintf('error is %6.4e\n',error(ip));
    
    

