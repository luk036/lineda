function go1 = read_go1()
    
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

go1 = str2num(tline(11:20));

fclose(fid);
    
