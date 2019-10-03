
Ntest = 100;
for ii = 1 : Ntest
    [R_test,Cap_test] = write_random();
    x_test = [R_test];%; Cap_test.*1e12];
    d_test(ii,1) = 1e-9*fit_rank_p_c(Qnew,Q,B,C,x_test,ip);
    [status,result]=system('hspice -i wire_sizing.sp -o wire_sizing.lis');
    %fprintf(1, '%s',result);
    if status ~= 0, return, end
    
    d3_test(ii,1) = read_lis();
end

relative_error = (d_test - d3_test)./d3_test;
pos_rela_err = abs(relative_error);
max_err = max(pos_rela_err)
mean_err = mean(pos_rela_err)
