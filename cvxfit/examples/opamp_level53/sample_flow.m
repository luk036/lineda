function [W,I8,Cc,Id1,Id6,vod1,vod3,vod5,vod6,vod7,vod8,gm1,gm6,go1,go3,go6,go7,gain,bw,phase]...
    =sample_flow(Rcenter, Rradius)
Nsample = 10;
fprintf('HSpice sampling start...\n');
ii = 1;
while ii <= Nsample
    design_random = write_random(Rcenter,Rradius);    
    [status,result]=system('hspice -i opamp2.sp -o opamp2.lis');
    % fprintf(1, '%s',result);
    if status ~= 0, return, end
    if check_sat > 0.5, 
        fprintf('Not SAT. Skip this sample.\n'); continue, 
    end
    
    W1(1,ii) = design_random.W1;
    W3(1,ii) = design_random.W3;
    W5(1,ii) = design_random.W5;
    W6(1,ii) = design_random.W6;
    W7(1,ii) = design_random.W7;
    W8(1,ii) = design_random.W8;
    I8(1,ii) = design_random.I8;
    Cc(1,ii) = design_random.Cc;

    [gain(ii,1),bw(ii,1),phase(ii,1)] = read_lis();
    vod1(1,ii) = read_vdsat1();
    vod3(1,ii) = read_vdsat3();
    vod5(1,ii) = read_vdsat5();
    vod6(1,ii) = read_vdsat6();
    vod7(1,ii) = read_vdsat7();
    vod8(1,ii) = read_vdsat8();
    
    gm1(1,ii) = read_gm1();
    gm6(1,ii) = read_gm6();
    go1(1,ii) = read_go1();
    go3(1,ii) = read_go3();
    go6(1,ii) = read_go6();
    go7(1,ii) = read_go7();
    
    Id1(1,ii) = read_Id1();
    Id6(1,ii) = read_Id6();
    
    design_random.Rc = 1/gm6(1,ii);
    write_design(design_random);
    [status,result]=system('hspice -i opamp2.sp -o opamp2.lis');
    % fprintf(1, '%s',result);
    if status ~= 0, return, end
    [gain(ii,1),bw(ii,1),phase(ii,1)] = read_lis();
    
    ii = ii + 1;
end

W = [W1; W3; W5; W6; W7; W8];

fprintf('%n HSpice sampling finished.\n\n',Nsample);

