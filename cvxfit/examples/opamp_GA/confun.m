%% Author: XXLiu
%% Date: Dec 2, 2007
%% Fitness function is set to be the sum of transistors' widths,
%% which stands for the area
%% Objective is to find a minimal area
%% Constraints:
%%      gain >= 60 dB
%%      bdw >= 10 MHz
%%      PM >= 60 degree
%% In case one population fall into infeasible region,
%% i.e., not all MOS are saturated, the three constraints are declared 
%% BROKEN, even if their measurements may satisfy the constraints.
function [c, ceq] = confun(x)

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
fprintf('con: W1=%6.4e  W3=%6.4e W5=%6.4e W6=%6.4e W7=%6.4e W8=%6.4e\n',...
        x(1),x(2),x(3),x(4),x(5),x(6));
fprintf('     I8=%6.4e %s',x(7),result);
saturati_unmet=check_sat(filename);
if saturati_unmet > 0
    gain = 0; phase = -180; bw = 0;
else
    [gain,bw,phase]=read_ma(filename);
end

% Nonlinear inequality constraints
c = [...
     60 - gain;
     -phase - 135;
     1e7 - bw;
    ];
% Nonlinear equality constraints
ceq = [];


