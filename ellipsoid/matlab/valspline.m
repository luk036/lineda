function funcsp = valspline(f,x,j)
% evaluate the spline function f
switch (j)
    case 0,
        funcsp = fnval(f,x);     %  value of the spline function f at site x
    case 1,
        ppdir = fndir(f,eye(length(x)));
        funcsp = fnval(ppdir,x); %  gradient of the spline function f at site x
end
