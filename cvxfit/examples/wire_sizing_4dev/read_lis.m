function [d2,d3] = read_lis()
    
fid = fopen('wire_sizing.lis','r');

while 1
    tline = fgetl(fid);
    if strfind(tline,'******  transient analysis') == 3
        break;
    end
end

tline = fgetl(fid);
tline = fgetl(fid);

idx = strfind(tline,'='); 
d2 = str2num(tline(idx(1)+1:idx(2)-5));

tline = fgetl(fid);
idx = strfind(tline,'='); 
d3 = str2num(tline(idx(1)+1:idx(2)-5));

fclose(fid);

return
% tline = fgetl(fid); % fall delay
% idx = strfind(tline,'='); 
% falldelay = str2num(tline(idx(1)+1:idx(2)-5));
% 
% tline = fgetl(fid); % rise slew
% idx = strfind(tline,'='); 
% riseslew = str2num(tline(idx(1)+1:idx(2)-5));
% 
% tline = fgetl(fid); % fall slew
% idx = strfind(tline,'=');
% fallslew = str2num(tline(idx(1)+1:idx(2)-5));
