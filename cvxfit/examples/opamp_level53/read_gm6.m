function gm6 = read_gm6()
    
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

gm6 = str2num(tline(66:75));

fclose(fid);
    
