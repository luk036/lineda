function autogen_spcvxsd(num_var)
    
filename = ['spcvx', num2str(num_var), 'd_gen.m']; % need change

fid = fopen(filename,'w');
alphabet_i = {'di','dj','dk'};
alphabet_u = {'u','v','w'};
alphabet_x = {'x','y','z'};
alphabet_m = {'m','n','o','p','q','r'};
alphabet_xx = {'xx','yy','zz'};
alphabet_nx = {'nx','ny','nz'};
alphabet_Cx = {'Cx','Cy','Cz'};


fprintf(fid,'function sp = spcvx%dd_gen(knots,k,x,y)\n\n',num_var);
for ii = 1:num_var
    fprintf(fid,'%s = knots{%d};\n',alphabet_u{ii},ii);
    fprintf(fid,'%s = k(%d);\n',alphabet_m{ii},ii);
    fprintf(fid,'%s = aveknt(%s,%s)\'';\n',alphabet_xx{ii},...
            alphabet_u{ii},alphabet_m{ii});
    fprintf(fid,'%s = size(%s,1);\n\n',alphabet_nx{ii},alphabet_xx{ii});
%%    fprintf(fid,'%s = spcol(%s,%s,%s);\n\n',alphabet_Cx{ii},alphabet_u(ii),...
%%            alphabet_m(ii),alphabet_x(ii));
end

fprintf(fid,'nn = nx');
if num_var > 1,
    for ii = 2:num_var, fprintf(fid,'*%s',alphabet_nx{ii}); end
end
fprintf(fid,';\n\n');
fprintf(fid,['cvx_quiet(true);\n%%%% Least squares fitting with linear ' ...
             'matrix inequality (LMI) constraints\n']);
fprintf(fid,'cvx_begin sdp\n  variable pp(nn)\n  for iy=1:length(y)\n');
for ii = 1:num_var
    fprintf(fid,'    %s = spcol(%s,%s,x(iy,%d));\n',alphabet_Cx{ii},...
            alphabet_u{ii},alphabet_m{ii},ii);
end
fprintf(fid,'    C = ');
for ii = 1:num_var-1
    fprintf(fid,'kron(%s,',alphabet_Cx{num_var-ii+1});
end
fprintf(fid,'%s',alphabet_Cx{1});
for ii = 1:num_var-1, fprintf(fid,')'); end
fprintf(fid,';\n    err(iy) = C*pp - y(iy);\n');
fprintf(fid,'  end\n  minimize( norm(err) );\n');
%% p = reshape(pp, [nx, ny]);
fprintf(fid,'  p = reshape(pp, [nx');
if num_var > 1,
    for ii = 2:num_var, fprintf(fid,',%s',alphabet_nx{ii}); end
end
fprintf(fid,']);\n  subject to\n');

for ii = 1:num_var %% print constraints a_ij and c_ij
    for jj = 1:num_var
        for ispace = 1:jj, fprintf(fid,'  '); end
        if ii == jj
            fprintf(fid,'  for %s=3:%s\n',alphabet_i{jj},alphabet_nx{jj});
        else 
            fprintf(fid,'  for %s=1:%s\n',alphabet_i{jj},alphabet_nx{jj});
        end
    end
    for ispace = 1:num_var, fprintf(fid,'  '); end
    fprintf(fid,'    (p(%s',alphabet_i{1});
    for jj = 2:num_var, fprintf(fid,',%s',alphabet_i{jj}); end
    fprintf(fid,')-p(');
    for jj = 1:num_var
        if ii == jj
            fprintf(fid,'%s-1',alphabet_i{jj});
        else
            fprintf(fid,'%s',alphabet_i{jj});
        end
        if jj ~= num_var, fprintf(fid,',');
        else fprintf(fid,'))/(%s(%s+%s-1)-%s(%s-1))-...\n',alphabet_u{ii},...
                     alphabet_i{ii},alphabet_m{ii},alphabet_u{ii}, ...
                     alphabet_i{ii}); 
        end
    end
     for ispace = 1:num_var, fprintf(fid,'  '); end
    fprintf(fid,'      (p(');
    for jj = 1:num_var
        if ii == jj
            fprintf(fid,'%s-1',alphabet_i{jj});
        else
            fprintf(fid,'%s',alphabet_i{jj});
        end
        if jj ~= num_var, fprintf(fid,',');
        else fprintf(fid,')-p('); end
    end
    for jj = 1:num_var
        if ii == jj
            fprintf(fid,'%s-2',alphabet_i{jj});
        else
            fprintf(fid,'%s',alphabet_i{jj});
        end        
        if jj ~= num_var, fprintf(fid,',');
        else fprintf(fid,'))/(%s(%s+%s-2)-%s(%s-2))>=0;\n',alphabet_u{ii},...
                     alphabet_i{ii},alphabet_m{ii},alphabet_u{ii}, ...
                     alphabet_i{ii}); 
        end
    end
    for jj = num_var:-1:1
        for ispace = 1:jj, fprintf(fid,'  '); end
        fprintf(fid,'  end\n');
    end
end
index_num = 1:num_var;
C_ij = nchoosek(index_num,2);
[LMI_num,temp] = size(C_ij);

for aa = 1:LMI_num
    fprintf(fid,'\n\n');
    for bb = 1:num_var
        for ispace = 1:bb, fprintf(fid,'  '); end
        if bb == C_ij(aa,1) || bb == C_ij(aa,2)
            fprintf(fid,'  for %s=3:%s\n',alphabet_i{bb},alphabet_nx{bb});
        else
            fprintf(fid,'  for %s=1:%s\n',alphabet_i{bb},alphabet_nx{bb});
        end
    end
    indent1 = num_var+2; indent2 = num_var+6;
    for jj = 1:indent1, fprintf(fid,'  '); end
    fprintf(fid,'for k=0:1\n');
    for jj = 1:indent1, fprintf(fid,'  '); end
    fprintf(fid,'  for l=0:1\n');
    for jj = 1:indent1, fprintf(fid,'  '); end
    fprintf(fid,'    for r=0:1\n');
    for jj = 1:indent1, fprintf(fid,'  '); end
    fprintf(fid,'      for s=0:1\n');
    %% constraint a
    for jj = 1:indent2, fprintf(fid,'  '); end
    fprintf(fid,'[%s*(%s-1)*((p(',alphabet_m{C_ij(aa,1)},...
            alphabet_m{C_ij(aa,1)});
    for bb = 1:num_var %% line 1 of a, item 1
        if bb == C_ij(aa,2)
            fprintf(fid,'%s-k-r',alphabet_i{bb});
        else
            fprintf(fid,'%s',alphabet_i{bb});
        end
        if bb < num_var, fprintf(fid,','); 
        else fprintf(fid,')-p('); end
    end
    for bb = 1:num_var %% line 1 of a, item 2
        if bb == C_ij(aa,1)
            fprintf(fid,'%s-1',alphabet_i{bb});
        elseif bb == C_ij(aa,2)
                fprintf(fid,'%s-k-r',alphabet_i{bb});
        else
            fprintf(fid,'%s',alphabet_i{bb});
        end
        if bb < num_var, fprintf(fid,','); 
        else fprintf(fid,'))/(%s(%s+%s-1)-%s(%s-1))-...\n',...
                     alphabet_u{C_ij(aa,1)},alphabet_i{C_ij(aa,1)},...
                     alphabet_m{C_ij(aa,1)},...
                     alphabet_u{C_ij(aa,1)},alphabet_i{C_ij(aa,1)}); 
        end
    end
    for jj = 1:indent2+1, fprintf(fid,'  '); end
    fprintf(fid,'(p(');
    for bb = 1:num_var %% line 2 of a, item 1
        if bb == C_ij(aa,1)
            fprintf(fid,'%s-1',alphabet_i{bb});
        elseif bb == C_ij(aa,2)
                fprintf(fid,'%s-k-r',alphabet_i{bb});
        else
            fprintf(fid,'%s',alphabet_i{bb});
        end
        if bb < num_var, fprintf(fid,','); 
        else fprintf(fid,')-p('); end
    end
    for bb = 1:num_var %% line 2 of a, item 2 
        if bb == C_ij(aa,1)
            fprintf(fid,'%s-2',alphabet_i{bb});
        elseif bb == C_ij(aa,2)
                fprintf(fid,'%s-k-r',alphabet_i{bb});
        else
            fprintf(fid,'%s',alphabet_i{bb});
        end
        if bb < num_var, fprintf(fid,','); 
        else fprintf(fid,['))/(%s(%s+%s-2)-%s(%s-2)))/',...
                          '(%s(%s+%s-2)-%s(%s-1)),...\n'],alphabet_u{C_ij(aa,1)},...
                     alphabet_i{C_ij(aa,1)},alphabet_m{C_ij(aa,1)},...
                     alphabet_u{C_ij(aa,1)},alphabet_i{C_ij(aa,1)},...
                     alphabet_u{C_ij(aa,1)},alphabet_i{C_ij(aa,1)},...
                     alphabet_m{C_ij(aa,1)},...
                     alphabet_u{C_ij(aa,1)},alphabet_i{C_ij(aa,1)});
        end
    end %% a finished
    %% constraint b
    for jj = 1:indent2, fprintf(fid,'  '); end
    fprintf(fid,'%s*%s*(p(',alphabet_m{C_ij(aa,1)},...
            alphabet_m{C_ij(aa,2)});
    for bb = 1:num_var %% line 1 of b, term 1
        if bb == C_ij(aa,1)
            fprintf(fid,'%s-l',alphabet_i{bb});
        elseif bb == C_ij(aa,2)
            fprintf(fid,'%s-k',alphabet_i{bb});
        else
            fprintf(fid,'%s',alphabet_i{bb});
        end
        if bb < num_var, fprintf(fid,','); 
        else fprintf(fid,')-p('); end
    end
    for bb = 1:num_var %% line 1 of b, term 2 
        if bb == C_ij(aa,1)
            fprintf(fid,'%s-l',alphabet_i{bb});
        elseif bb == C_ij(aa,2)
                fprintf(fid,'%s-k-1',alphabet_i{bb});
        else
            fprintf(fid,'%s',alphabet_i{bb});
        end
        if bb < num_var, fprintf(fid,','); 
        else fprintf(fid,')-p('); end
    end
    for bb = 1:num_var %% line 1 of b, term 3
        if bb == C_ij(aa,1)
            fprintf(fid,'%s-l-1',alphabet_i{bb});
        elseif bb == C_ij(aa,2)
                fprintf(fid,'%s-k',alphabet_i{bb});
        else
            fprintf(fid,'%s',alphabet_i{bb});
        end
        if bb < num_var, fprintf(fid,','); 
        else fprintf(fid,')+p('); end
    end
    for bb = 1:num_var %% line 1 of b, term 4
        if bb == C_ij(aa,1)
            fprintf(fid,'%s-l-1',alphabet_i{bb});
        elseif bb == C_ij(aa,2)
                fprintf(fid,'%s-k-1',alphabet_i{bb});
        else
            fprintf(fid,'%s',alphabet_i{bb});
        end
        if bb < num_var, fprintf(fid,','); 
        else %% line 2 of b
            fprintf(fid,'))...\n');
            for jj = 1:indent2+1, fprintf(fid,'  '); end
            fprintf(fid,'/(%s(%s-l+%s-1)-%s(%s-l-1))',alphabet_u{C_ij(aa,1)},...
                    alphabet_i{C_ij(aa,1)},alphabet_m{C_ij(aa,1)},...
                    alphabet_u{C_ij(aa,1)},alphabet_i{C_ij(aa,1)}); 
            fprintf(fid,'/(%s(%s-k+%s-1)-%s(%s-k-1));\n',alphabet_u{C_ij(aa,2)},...
                    alphabet_i{C_ij(aa,2)},alphabet_m{C_ij(aa,2)},...
                    alphabet_u{C_ij(aa,2)},alphabet_i{C_ij(aa,2)}); 
        end 
    end %% b finished
    %% constraint b, 2nd times
    for jj = 1:indent2, fprintf(fid,'  '); end
    fprintf(fid,'%s*%s*(p(',alphabet_m{C_ij(aa,1)},...
            alphabet_m{C_ij(aa,2)});
    for bb = 1:num_var %% line 1 of b, term 1, 2nd times
        if bb == C_ij(aa,1)
            fprintf(fid,'%s-l',alphabet_i{bb});
        elseif bb == C_ij(aa,2)
            fprintf(fid,'%s-k',alphabet_i{bb});
        else
            fprintf(fid,'%s',alphabet_i{bb});
        end
        if bb < num_var, fprintf(fid,','); 
        else fprintf(fid,')-p('); end
    end
    for bb = 1:num_var %% line 1 of b, term 2, 2nd times 
        if bb == C_ij(aa,1)
            fprintf(fid,'%s-l',alphabet_i{bb});
        elseif bb == C_ij(aa,2)
                fprintf(fid,'%s-k-1',alphabet_i{bb});
        else
            fprintf(fid,'%s',alphabet_i{bb});
        end
        if bb < num_var, fprintf(fid,','); 
        else fprintf(fid,')-p('); end
    end
    for bb = 1:num_var %% line 1 of b, term 3, 2nd times
        if bb == C_ij(aa,1)
            fprintf(fid,'%s-l-1',alphabet_i{bb});
        elseif bb == C_ij(aa,2)
                fprintf(fid,'%s-k',alphabet_i{bb});
        else
            fprintf(fid,'%s',alphabet_i{bb});
        end
        if bb < num_var, fprintf(fid,','); 
        else fprintf(fid,')+p('); end
    end
    for bb = 1:num_var %% line 1 of b, term 4, 2nd times
        if bb == C_ij(aa,1)
            fprintf(fid,'%s-l-1',alphabet_i{bb});
        elseif bb == C_ij(aa,2)
                fprintf(fid,'%s-k-1',alphabet_i{bb});
        else
            fprintf(fid,'%s',alphabet_i{bb});
        end
        if bb < num_var, fprintf(fid,','); 
        else %% line 2 of b, 2nd times
            fprintf(fid,'))...\n');
            for jj = 1:indent2+1, fprintf(fid,'  '); end
            fprintf(fid,'/(%s(%s-l+%s-1)-%s(%s-l-1))',alphabet_u{C_ij(aa,1)},...
                    alphabet_i{C_ij(aa,1)},alphabet_m{C_ij(aa,1)},...
                    alphabet_u{C_ij(aa,1)},alphabet_i{C_ij(aa,1)}); 
            fprintf(fid,'/(%s(%s-k+%s-1)-%s(%s-k-1)),...\n',alphabet_u{C_ij(aa,2)},...
                    alphabet_i{C_ij(aa,2)},alphabet_m{C_ij(aa,2)},...
                    alphabet_u{C_ij(aa,2)},alphabet_i{C_ij(aa,2)}); 
        end 
    end %% b finished, 2nd times
    %% constraint c
    for jj = 1:indent2, fprintf(fid,'  '); end
    fprintf(fid,'%s*(%s-1)*((p(',alphabet_m{C_ij(aa,2)},...
            alphabet_m{C_ij(aa,2)});
    for bb = 1:num_var %% line 1 of c, item 1
        if bb == C_ij(aa,1)
            fprintf(fid,'%s-l-s',alphabet_i{bb});
        else
            fprintf(fid,'%s',alphabet_i{bb});
        end
        if bb < num_var, fprintf(fid,','); 
        else fprintf(fid,')-p('); end
    end
    for bb = 1:num_var %% line 1 of c, item 2
        if bb == C_ij(aa,1)
            fprintf(fid,'%s-l-s',alphabet_i{bb});
        elseif bb == C_ij(aa,2)
                fprintf(fid,'%s-1',alphabet_i{bb});
        else
            fprintf(fid,'%s',alphabet_i{bb});
        end
        if bb < num_var, fprintf(fid,','); 
        else fprintf(fid,'))/(%s(%s+%s-1)-%s(%s-1))-...\n',...
                     alphabet_u{C_ij(aa,2)},alphabet_i{C_ij(aa,2)},...
                     alphabet_m{C_ij(aa,2)},...
                     alphabet_u{C_ij(aa,2)},alphabet_i{C_ij(aa,2)}); 
            for jj = 1:indent2+1, fprintf(fid,'  '); end
            fprintf(fid,'(p(');
        end
    end
    for bb = 1:num_var %% line 2 of c, item 1
        if bb == C_ij(aa,1)
            fprintf(fid,'%s-l-s',alphabet_i{bb});
        elseif bb == C_ij(aa,2)
                fprintf(fid,'%s-1',alphabet_i{bb});
        else
            fprintf(fid,'%s',alphabet_i{bb});
        end
        if bb < num_var, fprintf(fid,','); 
        else fprintf(fid,')-p('); end
    end
    for bb = 1:num_var %% line 2 of c, item 2 
        if bb == C_ij(aa,1)
            fprintf(fid,'%s-l-s',alphabet_i{bb});
        elseif bb == C_ij(aa,2)
                fprintf(fid,'%s-2',alphabet_i{bb});
        else
            fprintf(fid,'%s',alphabet_i{bb});
        end
        if bb < num_var, fprintf(fid,','); 
        else fprintf(fid,['))/(%s(%s+%s-2)-%s(%s-2)))/',...
                          '(%s(%s+%s-2)-%s(%s-1))\n'],alphabet_u{C_ij(aa,2)},...
                     alphabet_i{C_ij(aa,2)},alphabet_m{C_ij(aa,2)},...
                     alphabet_u{C_ij(aa,2)},alphabet_i{C_ij(aa,2)},...
                     alphabet_u{C_ij(aa,2)},alphabet_i{C_ij(aa,2)},...
                     alphabet_m{C_ij(aa,2)},...
                     alphabet_u{C_ij(aa,2)},alphabet_i{C_ij(aa,2)});
        end
    end %% c finished
    for jj = 1:num_var+6, fprintf(fid,'  '); end 
    fprintf(fid,'] >= 0;\n');
    for jj = 1:num_var+4, 
        for bb = 1:num_var+4-jj+2, fprintf(fid,'  '); end
        fprintf(fid,'end\n'); 
    end
end

fprintf(fid,'cvx_end\n\n');
fprintf(fid,'p = reshape(pp,[');
for ii = 1:num_var,
    fprintf(fid,'%s',alphabet_nx{ii});
    if ii < num_var, fprintf(fid,',');
    else fprintf(fid,']);\n'); end
end
fprintf(fid,'sp = spmak({');
for ii = 1:num_var,
    fprintf(fid,'%s',alphabet_u{ii});
    if ii < num_var, fprintf(fid,',');
    else fprintf(fid,'},p,['); end
end
for ii = 1:num_var,
    fprintf(fid,'%s',alphabet_nx{ii});
    if ii < num_var, fprintf(fid,',');
    else fprintf(fid,']);\n\n'); end
end
fclose(fid);

