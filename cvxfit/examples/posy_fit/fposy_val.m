function y=fposy_val(x,posy_coef)
% c = size(Km,1), 
% a = size(dim,Km), where dim is the number of function variables
% x = {x1, x2,...,x_dim}, x_i = size(M,1), M is number of sampling 
%     dim = length(x); M = length(x{1});
%     c = coef_vector(1:Km);
%     a = reshape(coef_vector(Km+1:end),[dim,Km]);
%     y = zeros(M,1);
%     for ii = 1:Km
%         y = y + c(ii).*(x{1}.^a(1,ii)).*(x{2}.^a(2,ii));
%     end
    M = length(x{1}); dim = length(x);
    y = zeros(M,1);
    % scale
    for ii = 1:dim,
        x{ii} = lin_log_scale(x{ii},posy_coef(ii).lin_log,...
                              posy_coef(ii).scale_factor);
    end
    
    for ii = 1:posy_coef(1).Km,
        coef = posy_coef(1).alpha(:,ii);
        g = exp(ones(M,1).*coef(1));
        for jj = 1:dim,
            g = g .* (x{jj}.^coef(jj+1));
        end
        y = y + g;
    end
    y = lin_log_unscale(y,posy_coef(end).lin_log,...
                        posy_coef(end).scale_factor);
    return;

%=========================================
function [v_scaled]=lin_log_scale(v,lin_log,scale_factor)
    if strcmp(lin_log,'LOG') == 1,
        v = log(v);
    end
    v_scaled = 1 + 9*(v-scale_factor(1))/diff(scale_factor);
    return;
%===============================================================
function v_unscaled=lin_log_unscale(v,lin_log,scale_factor)
    v = (v-1)/9*diff(scale_factor) + scale_factor(1);
    if strcmp(lin_log,'LOG') == 1,
        v = exp(v);
    end
    v_unscaled = v;
    return;

