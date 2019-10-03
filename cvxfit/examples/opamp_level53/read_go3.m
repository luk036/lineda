function go3 = read_go3()
    
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

go3 = str2num(tline(33:42));

fclose(fid);
    
