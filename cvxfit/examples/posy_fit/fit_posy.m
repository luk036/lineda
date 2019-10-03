function posy_coef=fit_posy(x,f,Km)
    dim = length(x); M = length(f);
    % Make input be vectors 
    for ii = 1:dim,
        [l,m] = size(x{ii});
        if l < m, 
            x{ii} = x{ii}'; 
        end
    end
    [l,m] = size(f);
    if l < m,
        f = f';
    end
    % ----------------------
    % x are real, positive. Scale x to [1,10].
    for ii = 1:dim,
        [x{ii},lin_log(ii),scale_factor{ii}] = lin_log_scale(x{ii});
    end
    [f,lin_log(end+1),scale_factor{end+1}] = lin_log_scale(f);
    %------------------------------------------------------------
    if Km < 1.5, % Monomial: Km = 1;
        A = ones(M,1);
        for ii = 1:dim,
            y{ii} = log(x{ii});
            A = [A  y{ii}];
        end
        g = log(f);

        alpha = A\g;
    else, % Posynomial: Km >= 2;
        coef0 = rand(Km*(dim+1),1);
    
        options = optimset('TolX',1e-20,'TolFun',1e-30,...
                           'TolCon',1e-30,'LargeScale','off',...
                           'MaxFunEvals',50000,'MaxIter',100000);
        [coef_vector,fval,exitflag] = ...
            fminsearch(@(coef)error_posy(f,x,coef,Km),...
                       coef0,options)
        alpha = reshape(coef_vector,[dim+1, Km]);
    end
    posy_coef = struct('Km',Km,'alpha',alpha,'lin_log',lin_log,...
                       'scale_factor',scale_factor);
    return;

%=================================================
function [v_scaled,lin_log,scale_factor]=lin_log_scale(v)
    scale_factor = [min(v), max(v)];
    lin_log = {'LIN'};
    if max(v)/min(v) > 100, 
        lin_log = {'LOG'};
        v = log(v);
    end
    v_scaled = 1 + 9*(v-scale_factor(1))/diff(scale_factor);
    return;
    
%====================================================================   
function error=error_posy(yorg,xin,coef_vector,Km)
    dim = length(xin); M = length(yorg);
    alpha = reshape(coef_vector,[dim+1, Km]);
    
    yfit = zeros(M,1);
    for ii = 1:Km,
        coef = alpha(:,ii);
        g = exp(ones(M,1).*coef(1));
        for jj = 1:dim,
            g = g .* (xin{jj}.^coef(jj+1));
        end
        yfit = yfit + g;
    end
    
    error = norm(yorg-yfit);
    return;

        
% function error=error_posy(yorg,xin,coef,Km)
%     yfit = fposy(xin,coef,Km);
%     error = norm(yorg-yfit);
%     return
% 
% function y=fposy(x,coef_vector,Km)
% % c = size(Km,1), 
% % a = size(dim,Km), where dim is the number of function variables
% % x = {x1, x2,...,x_dim}, x_i = size(M,1), M is number of sampling 
%     dim = length(x); M = length(x{1});
%     c = coef_vector(1:Km);
%     a = reshape(coef_vector(Km+1:end),[dim,Km]);
%     y = zeros(M,1);
%     for ii = 1:Km
%         y = y + c(ii)*x{1}^a(1,ii)*x{2}^a(2,ii);
%     end
%     return;


