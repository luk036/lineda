function gm1 = read_gm1()
    
fid = fopen('opamp2.lis','r');

while 1
    tline = fgetl(fid);
    if strfind(tline,'region') == 2
        break;
    end
end

for ii = 1 : 11
    tline = fgetl(fid);
end

gm1 = str2num(tline(11:20));

fclose(fid);
    
