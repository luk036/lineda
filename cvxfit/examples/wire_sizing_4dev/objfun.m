function f = objfun(x)
    global A_d3 B_d3 C_d3
    xx = [x(1) x(2)]';
    f = xx'*A_d3*xx + B_d3'*xx + C_d3;
