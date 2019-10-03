function [Qnew_gain,Q_gain,B_gain,C_gain,...
          Qnew_bw,  Q_bw,  B_bw,  C_bw,...
          Qnew_phase,Q_phase,B_phase,C_phase]=fit_test_XL(input,gain,bw,phase)

    x = input*1e6;
    f_gain = gain;
    f_bw = bw*1e-6;
    f_phase = phase;

    ip = 1;
    
    [Qnew_gain, Q_gain, B_gain, C_gain] = rank_p_c(x,f_gain,ip,800);
    % f_p_d3 = fit_rank_p_c(Qnew_d3,Q_d3,B_d3,C_d3,x,ip);
    % error(ip) = norm(f_p_d3-f_d3);
    % fprintf('error is %6.4e\n',error(ip));
    
    [Qnew_bw, Q_bw, B_bw, C_bw] = rank_p_c(x,f_bw,ip,800);
    % f_p_d4 = fit_rank_p_c(Qnew_d4,Q_d4,B_d4,C_d4,x,ip);
    % error(ip) = norm(f_p_d4-f_d4);
    % fprintf('error is %6.4e\n',error(ip));
    
    [Qnew_phase, Q_phase, B_phase, C_phase] = rank_p_c(x,f_phase,ip,800);
    % f_p_d5 = fit_rank_p_c(Qnew_d5,Q_d5,B_d5,C_d5,x,ip);
    % error(ip) = norm(f_p_d5-f_d5);
    % fprintf('error is %6.4e\n',error(ip));
    
    
