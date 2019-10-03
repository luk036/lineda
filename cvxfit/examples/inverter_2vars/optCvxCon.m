function [c,ceq] = optCvxCon(sp,width)
    ceq = [2*width(1) - width(2);];
    c = [fnval(sp,{width(1), width(2)}) - .1;];
