%% read_ma.m  Dec 2nd, 2007
%% Author: XXLiu
%% Measured terms are read out from HSPICE's ".ma0" file
%% These measurements are feeded to optimization engine.
function [gain,bw,phase] = read_ma(filename)
    
fid = fopen([filename,'.ma0'],'r');
for ii = 1:4, tline = fgetl(fid); end
data_array = str2num(tline);

bw = data_array(1);
phase = data_array(2);
gain = data_array(3);

fclose(fid);



