% Object function
% Usage: in fminsearch, this function serves as function handle
%        Wn, Wp are transistor size, generated on grid, and 
%        rise is fall transistion time read from SPICE
% After optimization call fminsearch, the coefs of a posynomial model
% are solved out, saved in p, and return
function  err=fitgp_rise_obj(p,Wn,Wp,rise)
    err = 0;
    for ii = 1 : length(Wn)
        for jj = 1 : length(Wp)
            err = err + (rise(ii,jj) - fitgp_rise(p,Wn(ii),Wp(jj)))^2;
        end
    end
