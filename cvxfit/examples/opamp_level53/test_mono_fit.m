
% fit_vod1 = eval_vdsat1(c_vod1,a_vod1_W1,a_vod1_W5,a_vod1_W8,a_vod1_Ibias,...
%                        100e-6,182e-6,10e-6,5.44e-6)
% fit_vod3 = eval_vdsat3(c_vod3,a_vod3_W3,a_vod3_W5,a_vod3_W8,a_vod3_Ibias,...
%                        38e-6,182e-6,10e-6,5.44e-6)
% fit_vod5 = eval_vdsat5(c_vod5,a_vod5_W8,a_vod5_Ibias,...
%                        10e-6,5.44e-6)
clear all, close all

N = 20;
x1 = linspace(1e-8,1e-6,N);
x2 = linspace(1e14,1e14,N);

y1 = log(x1); y2 = log(x2);

idxf = 1;
for ix1 = 1 : N
    for ix2 = 1 : N
        f(idxf,1) = 3 * x1(ix1)^2 * x2(ix2)^-.5;

        A(idxf,1) = 1;
        A(idxf,2) = y1(ix1);
        A(idxf,3) = y2(ix2);
        
        idxf = idxf + 1;
    end
end
g = log(f);
coef = A\g
a0 = exp(coef(1))



