%% Author: XXLiu 
%% Date: Dec 2, 2007
%% Fitness function is set to be the sum of transistors' widths,
%% which stands for the area.
%% Objective is to find a minimal area
function f = objfun(x)
fprintf('obj: W1=%6.4e  W3=%6.4e W5=%6.4e W6=%6.4e W7=%6.4e W8=%6.4e\n',...
        x(1),x(2),x(3),x(4),x(5),x(6));
fprintf('     I8=%6.4e\n-------------------------\n',x(7));
f = 2*(x(1)+x(2)) + x(3) + x(4) + x(5) + x(6);
%% x(1) x(2) x(3) x(4) x(5) x(6)
%%  W1   W3   W5   W6   W7   W8
%% For the MOSFETs, 
%% W1=W2, W3=W4, therefore they are doubled in the equation above. 





