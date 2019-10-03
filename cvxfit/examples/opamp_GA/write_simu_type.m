%% write_simu_type.m   12/05/07
%% Author: XXLiu
%% Data structure: simu_type as an input structure
%% simu_type = struct('AC_TRUE', 0,...
%%                    'TR_TRUE', 1,...
%%                    'PLOT_TRUE', 1)
function write_simu_type(simu_type)
    
fid = fopen('simu_type.txt','w'); % Open the file for write
if simu_type.AC_TRUE > 0,
    fprintf(fid, 'Vin1  9  0  Vcm \n');
    fprintf(fid, 'Vin2  8  0  dc=Vcm ac=1 \n');
    fprintf(fid, '.ac  dec  20  1  1000x \n');

    if simu_type.PLOT_TRUE > 0,
        fprintf(fid, '.print ac vdb(5) vp(5) \n\n');
    else
        fprintf(fid, '.measure  ac  bandwidth  when  vdb(5)=0\n');
        fprintf(fid, '.measure  ac  phase_mar  find  vp(5)  at=bandwidth\n');
        fprintf(fid, '.measure  ac  gain  find  vdb(5)  at=1\n');
    end
end

if simu_type.TR_TRUE > 0,
    fprintf(fid, 'Vin1  9  0  Vcm \n');
    fprintf(fid, 'Vin2  8  0  sin(Vcm 100u 1) \n');
    fprintf(fid, '.tran 0.1 3 \n');
    if simu_type.PLOT_TRUE > 0,
        fprintf(fid, '.print tran v(8) v(5) \n\n');
    end
end

fclose(fid); % writing completed, close the file

