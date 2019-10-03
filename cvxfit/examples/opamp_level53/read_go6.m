function go6 = read_go6()
    
fid = fopen('opamp2.lis','r');

while 1
    tline = fgetl(fid);
    if strfind(tline,'region') == 2
        break;
    end
end

for ii = 1 : 12
    tline = fgetl(fid);
end

go6 = str2num(tline(66:75));

fclose(fid);
    
