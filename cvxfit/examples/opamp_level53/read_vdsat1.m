function vod = read_vdsat1()
    
fid = fopen('opamp2.lis','r');

while 1
    tline = fgetl(fid);
    if strfind(tline,'region') == 2
        break;
    end
end

for ii = 1 : 8
    tline = fgetl(fid);
end

idx = strfind(tline,'='); 
vod = str2num(tline(11:20));

fclose(fid);
    