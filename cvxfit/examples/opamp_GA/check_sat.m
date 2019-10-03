%% check_sat.m Oct 10th, 2007
%% Author: XXLiu
%% Read HSPICE report file ".lis"
%% If there is any MOS not working in saturation 
%% the returned value will be 1 or 2 according to Linear or Cutoff
function [satCheck]=check_sat(filename)
    
fid = fopen([filename,'.lis'],'r');
satCheck = 0;
while 1
    tline = fgetl(fid);
    if strfind(tline,'**** mosfets') == 2, break; end
end
while 1,
    tline = fgetl(fid);
    if strfind(tline,'******') == 2, break; end
    if strfind(tline,'region') == 2,
        if length(strfind(tline,'Linear')) ~= 0,
            satCheck = 1; break,
        elseif length(strfind(tline,'Cutoff')) ~= 0,
            satCheck = 2; break,
        else
            satCheck = 0;
        end
    end
end

fclose(fid);


