%% tran_ac_plot.m 12/05/07
%% Author: XXLiu
%% need filename such as 'opamp2' and type such as 'ac' or 'tran'
%% PLOT_TRUE is set to 1, i.e., true, in this function.
function  tran_ac_plot(filename,type)

if strcmp(type(1),'t') == 1 % if type == tran
    simu_type = struct('AC_TRUE',0, 'TR_TRUE',1, 'PLOT_TRUE',1);
else % if type == ac
    simu_type = struct('AC_TRUE',1, 'TR_TRUE',0, 'PLOT_TRUE',1);
end
write_simu_type(simu_type);

[status,result]=system(['hspice -i ',filename,'.sp -o ',filename,'.lis']);
fprintf(1, '%s',result);
if status ~= 0, return, end

fid = fopen([filename,'.lis'],'r');
if strcmp(type(1),'t') == 1 % if type == tran
    while 1
        tline = fgetl(fid);
        if strfind(tline,'******  transient analysis') == 3
            break;
        end
    end
    for ii = 1:6, tline = fgetl(fid); end
    ii = 1;
    while tline(1) ~= 'y'
        data_array = str2num(tline);
        time_tran(ii) = data_array(1);
        vin_tran(ii) = data_array(2);
        vout_tran(ii) = data_array(3);
        
        tline = fgetl(fid);
        ii = ii + 1;
    end
    figure, plot(time_tran,vin_tran, time_tran,vout_tran,'r')
    xlabel('Time'), ylabel('Volts'), legend('Vin','Vout');
else % if type == 'ac'
% ac analysis ----------------------------
    while 1
        tline = fgetl(fid);
        if strfind(tline,'******  ac analysis') == 3
            break;
        end
    end
    for ii = 1:6, tline = fgetl(fid); end
    ii = 1;
    while tline(1) ~= 'y'
        data_array = str2num(tline);
        freq_ac(ii) = data_array(1);
        vdb_ac(ii) = data_array(2);
        vp_ac(ii) = data_array(3);
    
        tline = fgetl(fid);
        ii = ii + 1;
    end
    figure, 
    subplot(2,1,1), semilogx(freq_ac,vdb_ac), grid on
    xlabel('Frequency'), ylabel('dB')
    subplot(2,1,2), semilogx(freq_ac,vp_ac), grid on
    xlabel('Frequency'), ylabel('degree')
end % end if type == 'ac' or 'tran'

fclose(fid);

