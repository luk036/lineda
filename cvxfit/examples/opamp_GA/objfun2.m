%% objfun2.m   Dec 3rd, 2007
%% Author: XXLiu
%% Fitness function is set to be -1*BDW, thus finding maximal bandwidth
%% via the minimize formula of ga.
%% Therefore it reaches its minimum if bandwidth is maximized
%% In case one population falls in the infeasible region,
%% Fitness function is set to zero, i.e., very big, in order to force
%% a come back to feasible region.
function f = objfun2(x)
fprintf('obj: W1=%6.4e  W3=%6.4e W5=%6.4e W6=%6.4e W7=%6.4e W8=%6.4e\n',...
        x(1),x(2),x(3),x(4),x(5),x(6));
fprintf('     I8=%6.4e\n-------------------------\n',x(7));
% f = 2*(x(1)+x(2)) + x(3) + x(4) + x(5) + x(6);

global filename L;
design = struct(...
    'L',L, 'W1',x(1), 'W3',x(2), 'W5',x(3), 'W6',x(4),...
    'W7',x(5), 'W8',x(6), 'I8',x(7)...
    );
    
write_design(design);
simu_type = struct('AC_TRUE', 1, 'TR_TRUE', 0, 'PLOT_TRUE', 0);
[status,result]=system(['hspice -i ',filename,'.sp -o ',...
                    filename,'.lis']);
if status ~= 0, return, end
saturati_unmet=check_sat(filename);
if saturati_unmet > 0
    f = 0;
else
    [gain,bw,phase]=read_ma(filename);
    f = -1 * bw;
end



