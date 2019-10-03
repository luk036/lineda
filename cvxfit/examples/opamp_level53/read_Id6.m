function Id6 = read_Id6()
    
fid = fopen('opamp2.lis','r');

while 1
    tline = fgetl(fid);
    if strfind(tline,'region') == 2
        break;
    end
end

for ii = 1 : 1
    tline = fgetl(fid);
end

Id6 = str2num(tline(66:75));

fclose(fid);
