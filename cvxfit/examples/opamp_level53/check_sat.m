function [satCheck]=check_sat()
    
fid = fopen('opamp2.lis','r');
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


