function g = gspline(x,j)
% example of a spline function
global sp2;
switch (j)
    case 0,
        g = valspline(sp2,x,1);    % objective function
    case 1,
        g = [-1 -1]';              % first constraint function
end
