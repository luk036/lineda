%% read_v5.m  Dec 5th, 2007
%% Author: XXLiu
%% For a broad output swing, the DC voltage at node 5
%% should be Vdd/2.
function v5=read_v5(filename)
    
fid = fopen([filename,'.lis'],'r');

while 1
    tline = fgetl(fid);
    if strfind(tline,'0:5       =') > 0
        break;
    end
end
v5 = str2num(tline(36:45));

fclose(fid);
