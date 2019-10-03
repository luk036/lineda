function [R,d2,d3]=sample_flow(Rcenter, Rradius)
Nsample = 100;
for ii = 1 : Nsample
    [R(:,ii),Cap(:,ii)] = write_random(Rcenter,Rradius);
    
    [status,result]=system('hspice -i wire_sizing.sp -o wire_sizing.lis');
    if status ~= 0, return, end
    
    [d2(ii,1),d3(ii,1)] = read_lis();
end


