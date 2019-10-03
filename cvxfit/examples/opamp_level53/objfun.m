function f = objfun(x)
    global A_bw B_bw C_bw
    f = -1*(x'*A_bw*x + B_bw'*x + C_bw);

