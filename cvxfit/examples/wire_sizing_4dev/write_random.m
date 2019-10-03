function [R,C] = write_random(Rcenter,Rradius)
    R = Rcenter - Rradius + 2*Rradius*rand(2,1);
    C = 10e-12*ones(2,1);

    fid = fopen('exp.txt','w');
      fprintf(fid, '* \n');
      fprintf(fid, '.param  \n');
      fprintf(fid, '+ R1=%6.4e \n', R(1));
      fprintf(fid, '+ R2=%6.4e \n', R(2)); 
      fprintf(fid, '+ C1=%6.4e \n', C(1));
      fprintf(fid, '+ C2=%6.4e \n', C(2));
    fclose(fid);
return



