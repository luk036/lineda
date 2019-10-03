function go7 = read_go7()
    
fid = fopen('opamp2.lis','r');

while 1
    tline = fgetl(fid);
    if strfind(tline,'region') == 2
        break;
    end
end
while 1
    tline = fgetl(fid);
    if strfind(tline,'region') == 2
        break;
    end
end

for ii = 1 : 12
    tline = fgetl(fid);
end

go7 = str2num(tline(11:20));

fclose(fid);
