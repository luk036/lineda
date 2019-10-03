function [gain,bw,phase] = read_lis()
    
fid = fopen('opamp2.lis','r');

while 1
    tline = fgetl(fid);
    if strfind(tline,'******  ac analysis') == 3
        break;
    end
end

tline = fgetl(fid);
tline = fgetl(fid);
idx = strfind(tline,'='); 
bw = str2num(tline(idx(1)+1:idx(1)+12));

tline = fgetl(fid);
idx = strfind(tline,'='); 
phase = str2num(tline(idx(1)+1:idx(1)+12));

tline = fgetl(fid);
idx = strfind(tline,'='); 
gain = str2num(tline(idx(1)+1:idx(1)+12));

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
