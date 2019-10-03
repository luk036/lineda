function vod = read_vdsat5()
    
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
vod = str2num(tline(55:64));

fclose(fid);
   

