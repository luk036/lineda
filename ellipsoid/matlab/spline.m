function f = spline(x,j)
% example of a spline function
global sp2;
switch (j)
    case 0,
        f = valspline(sp2,x,0);  % objective function
    case 1,
        f = ([-1 -1]*x+0.3);       % first constraint function
end
