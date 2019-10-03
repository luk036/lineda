function autogen_spcvx_H(num_var)

filename = ['spcvx', num2str(num_var), 'd_gen_H.m']; % need change
fprintf('%s\n',filename);
fid = fopen(filename,'w');
alphabet_i = {'d1','d2','d3','d4','d5'};
alphabet_u = {'u1','u2','u3','u4','u5'};
alphabet_x = {'x1','x2','x3','x4','x5'};
alphabet_m = {'m1','m2','m3','m4','m5'};
alphabet_xx = {'xx1','xx2','xx3','xx4','xx5'};
alphabet_nx = {'nx1','nx2','nx3','nx4','nx5'};
alphabet_Cx = {'Cx1','Cx2','Cx3','Cx4','Cx5'};


fprintf(fid,'function sp = spcvx%dd_gen(knots,k,x,y)\n\n',num_var);
for ii = 1:num_var
    fprintf(fid,'%s = knots{%d};\n',alphabet_u{ii},ii);
    fprintf(fid,'%s = k(%d);\n',alphabet_m{ii},ii);
    fprintf(fid,'%s = aveknt(%s,%s)\'';\n',alphabet_xx{ii},...
        alphabet_u{ii},alphabet_m{ii});
    fprintf(fid,'%s = size(%s,1);\n\n',alphabet_nx{ii},alphabet_xx{ii});
end

fprintf(fid,'nn = ');
for ii = 1:num_var, 
    fprintf(fid,'%s',alphabet_nx{ii}); 
    if ii < num_var, fprintf(fid,'*'); 
    else, fprintf(fid,';\n\n'); end
end
fprintf(fid,['cvx_quiet(false);\n%%%% Least squares fitting with linear ' ...
    'matrix inequality (LMI) constraints\n']);
fprintf(fid,'cvx_begin sdp\n  variable pp(nn)\n');

fprintf(fid,'  for iy=1:length(y)\n');
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
fprintf(fid,'  p = reshape(pp, [');
for ii = 1:num_var, 
    fprintf(fid,'%s',alphabet_nx{ii}); 
    if ii < num_var, fprintf(fid,',');
    else, fprintf(fid,']);\n\n'); end
end

fprintf(fid,'\n  subject to\n');

for ii = 1:num_var % Sxx, Syy, Szz
    for jj = 1:num_var
        for ispace = 1:jj, fprintf(fid,'  '); end
        if ii == jj
            fprintf(fid,'for %s=3:%s\n',alphabet_i{jj},alphabet_nx{jj});
        else
            fprintf(fid,'for %s=1:%s\n',alphabet_i{jj},alphabet_nx{jj});
        end
    end
    for ispace = 1:num_var+1, fprintf(fid,'  '); end

    %fprintf(fid,'%s*(%s-1)*',alphabet_m{ii},alphabet_m{ii});
    fprintf(fid,'((p(%s',alphabet_i{1}); % Line 1 of Sxx >= 0
    for jj = 2:num_var, fprintf(fid,',%s',alphabet_i{jj}); end % p_{i,j}
    fprintf(fid,')-p(');
    for jj = 1:num_var % p_{i-1,j}
        if ii == jj
            fprintf(fid,'%s-1',alphabet_i{jj});
        else
            fprintf(fid,'%s',alphabet_i{jj});
        end
        if jj ~= num_var, fprintf(fid,','); % u_{i+m-1} - u_{i-1}
        else,
            fprintf(fid,'))/(%s(%s+%s-1)-%s(%s-1))-...\n',alphabet_u{ii},...
                    alphabet_i{ii},alphabet_m{ii},alphabet_u{ii}, ...
                    alphabet_i{ii});
        end
    end
    for ispace = 1:num_var, fprintf(fid,'  '); end
    fprintf(fid,'           (p('); % Line 2 of Sxx >= 0
    for jj = 1:num_var % p_{i-1,j}
        if ii == jj
            fprintf(fid,'%s-1',alphabet_i{jj});
        else
            fprintf(fid,'%s',alphabet_i{jj});
        end
        if jj ~= num_var, fprintf(fid,',');
        else fprintf(fid,')-p('); end
    end
    for jj = 1:num_var % p_{i-2,j}
        if ii == jj,
            fprintf(fid,'%s-2',alphabet_i{jj});
        else,
            fprintf(fid,'%s',alphabet_i{jj});
        end
        if jj ~= num_var, fprintf(fid,','); % u_{i+m-2} - u_{i-2}
        else,
            fprintf(fid,'))/(%s(%s+%s-2)-%s(%s-2)))...\n',...
                    alphabet_u{ii},alphabet_i{ii},alphabet_m{ii},...
                    alphabet_u{ii},alphabet_i{ii});
        end        
    end
    
    for qqq = 1:num_var, fprintf(fid,'  '); end
    %fprintf(fid,'         /(%s(',alphabet_u{ii});
    %fprintf(fid,'%s+%s-2)-%s(%s-1)) 
    fprintf(fid,'>= 0;\n',...% u_{m+i-2}-u_{i-1}, Line 3 of Sxx
        alphabet_m{ii},alphabet_i{ii},alphabet_u{ii},alphabet_i{ii});
    for jj = num_var:-1:1
        for ispace = 1:jj, fprintf(fid,'  '); end
        fprintf(fid,'end\n');
    end
end

index_num = 1:num_var; % Sxy, Syz, Sxz,...
C_ij = nchoosek(index_num,2);
[LMI_num,temp] = size(C_ij);

% S = [Sxx, Sxy; Syx, Syy] construction
for i_Cij = 1:LMI_num,
    Srow = C_ij(i_Cij,1); Scol = C_ij(i_Cij,2);
        for ii = 1:num_var
            for ispace = 1:ii, fprintf(fid,'  '); end
            if ii == Srow || ii == Scol,
                fprintf(fid,'for %s=3:%s\n',...
                        alphabet_i{ii},alphabet_nx{ii});
            else,                
                fprintf(fid,'for %s=1:%s\n',...
                        alphabet_i{ii},alphabet_nx{ii});
            end
        end
        for jj = 1:num_var+1, fprintf(fid,'  '); end
        fprintf(fid,'for kk=0:1\n');
        for jj = 1:num_var+1, fprintf(fid,'  '); end
        fprintf(fid,'  for ll=0:1\n');
        for jj = 1:num_var+1, fprintf(fid,'  '); end
        fprintf(fid,'    for rr=0:1\n');
        for jj = 1:num_var+1, fprintf(fid,'  '); end
        fprintf(fid,'      for ss=0:1\n');
        for ii = 1:num_var, fprintf(fid,'  '); end
        fprintf(fid,'    [...\n');
        % Sxx
        for jj = 1:num_var+2, fprintf(fid,'  '); end
        fprintf(fid,' (%s)*(%s-1)*((p(',...
                alphabet_m{Srow},alphabet_m{Srow}); % Line 1 of Sxx >= 0
        for jj = 1:num_var, 
            if jj == Scol, 
                fprintf(fid,'%s-ll-ss',alphabet_i{jj}); 
            else, 
                fprintf(fid,'%s',alphabet_i{jj}); 
            end
            if jj < num_var, 
                fprintf(fid,','); 
            end
        end % p_{i,j}
        fprintf(fid,')-p(');
        for jj = 1:num_var % p_{i-1,j}
            if Srow == jj, 
                fprintf(fid,'%s-1',alphabet_i{jj});
            elseif Scol == jj, 
                    fprintf(fid,'%s-ll-ss',alphabet_i{jj});
            else, 
                fprintf(fid,'%s',alphabet_i{jj});
            end
            if jj ~= num_var, 
                fprintf(fid,','); % u_{i+m-1} - u_{i-1}
            else, 
                fprintf(fid,'))/(%s(%s+%s-1)-%s(%s))-...\n',...
                        alphabet_u{Srow},alphabet_i{Srow},alphabet_m{Srow},...
                        alphabet_u{Srow},alphabet_i{Srow});
            end
        end
        for ispace = 1:num_var, fprintf(fid,'  '); end
        fprintf(fid,'              (p('); % Line 2 of Sxx >= 0
        for jj = 1:num_var % p_{i-1,j}
            if Srow == jj,
                fprintf(fid,'%s-1',alphabet_i{jj});
            elseif Scol == jj,
                    fprintf(fid,'%s-ll-ss',alphabet_i{jj});
            else, 
                fprintf(fid,'%s',alphabet_i{jj}); 
            end
            if jj ~= num_var, 
                fprintf(fid,',');
            else, 
                fprintf(fid,')-p('); 
            end
        end
        for jj = 1:num_var % p_{i-2,j}
            if Srow == jj, 
                fprintf(fid,'%s-2',alphabet_i{jj});
            elseif Scol == jj,
                    fprintf(fid,'%s-ll-ss',alphabet_i{jj});
            else, 
                fprintf(fid,'%s',alphabet_i{jj});
            end
            if jj ~= num_var, fprintf(fid,','); % u_{i+m-2} - u_{i-2}
            else,
                fprintf(fid,'))/(%s(%s+%s-2)-%s(%s-1)))...\n',...
                        alphabet_u{Srow},alphabet_i{Srow},alphabet_m{Srow},...
                        alphabet_u{Srow},alphabet_i{Srow});
                for qqq = 1:num_var, fprintf(fid,'  '); end
                fprintf(fid,'            /(%s(',alphabet_u{Srow});
            end
        end
        fprintf(fid,'%s+%s-2)-%s(%s)),...\n',...% u_{m+i-2}-u_{i-1}, Line 3 of Sxx
                alphabet_m{Srow},alphabet_i{Srow},alphabet_u{Srow},alphabet_i{Srow});
        % Sxy
        for ispace = 1:num_var+2, fprintf(fid,'  '); end
        fprintf(fid,' (%s)*(%s)*(p(',alphabet_m{Srow},...
                alphabet_m{Scol}); % Line 1 of Sxy
        for jj = 1:num_var,  % item 1, Line 1 of Sxy
            if jj == Srow, 
                fprintf(fid,'%s-kk',alphabet_i{jj});
            elseif jj == Scol,
                    fprintf(fid,'%s-ll',alphabet_i{jj});
            else, 
                fprintf(fid,',%s',alphabet_i{jj});
            end
            if jj < num_var, fprintf(fid,','); end
        end
        fprintf(fid,')-p(');
        for jj = 1:num_var % item 2, Line 1 of Sxy
            if jj == Srow, fprintf(fid,'%s-1-kk',alphabet_i{jj});
            elseif jj == Scol, 
                    fprintf(fid,'%s-ll',alphabet_i{jj});
            else, fprintf(fid,'%s',alphabet_i{jj}); end
            if jj ~= num_var, fprintf(fid,',');
            else, fprintf(fid,')-p('); end
        end
        for jj = 1:num_var % item 3, Line 1 of Sxy
            if jj == Scol, fprintf(fid,'%s-1-ll',alphabet_i{jj});
            elseif jj == Srow, 
                    fprintf(fid,'%s-kk',alphabet_i{jj});
            else, fprintf(fid,'%s',alphabet_i{jj}); end
            if jj ~= num_var, fprintf(fid,',');
            else, fprintf(fid,')+p('); end
        end
        for jj = 1:num_var % item 4, Line 1 of Sxy
            if jj == Srow,
                fprintf(fid,'%s-1-kk',alphabet_i{jj});
            elseif jj == Scol,
                        fprintf(fid,'%s-1-ll',alphabet_i{jj});
            else, fprintf(fid,'%s',alphabet_i{jj}); end
            if jj ~= num_var, fprintf(fid,',');
            else, fprintf(fid,'))...\n'); end
        end
        for jj = 1:num_var, fprintf(fid,'  '); end
        fprintf(fid,'        /((%s(%s+%s-1-kk)-%s(%s-kk)) * ',...
                alphabet_u{Srow},alphabet_i{Srow},...
                alphabet_m{Srow},alphabet_u{Srow}, ...
                alphabet_i{Srow}); % u_{i+m-1}-u_{i-1}, Line 2 of Sxy
        fprintf(fid,'(%s(%s+%s-1-ll)-%s(%s-ll)));...\n',...
                alphabet_u{Scol},alphabet_i{Scol},...
                alphabet_m{Scol},alphabet_u{Scol},...
                alphabet_i{Scol}); % v_{j+n-1}-v_{j-1}, Line 2 of Sxy
        
        % Sxy
        for ispace = 1:num_var+2, fprintf(fid,'  '); end
        fprintf(fid,' (%s)*(%s)*(p(',alphabet_m{Srow},...
                alphabet_m{Scol}); % Line 1 of Sxy
        for jj = 1:num_var,  % item 1, Line 1 of Sxy
            if jj == Srow, fprintf(fid,'%s-kk',alphabet_i{jj});
            elseif jj == Scol,
                    fprintf(fid,'%s-ll',alphabet_i{jj});
            else, fprintf(fid,',%s',alphabet_i{jj});
            end
            if jj < num_var, fprintf(fid,','); end
        end
        fprintf(fid,')-p(');
        for jj = 1:num_var % item 2, Line 1 of Sxy
            if jj == Srow, fprintf(fid,'%s-1-kk',alphabet_i{jj});
            elseif jj == Scol, 
                        fprintf(fid,'%s-ll',alphabet_i{jj});
            else, fprintf(fid,'%s',alphabet_i{jj}); end
            if jj ~= num_var, fprintf(fid,',');
            else, fprintf(fid,')-p('); end
        end
        for jj = 1:num_var % item 3, Line 1 of Sxy
            if jj == Scol, fprintf(fid,'%s-1-ll',alphabet_i{jj});
            elseif jj == Srow, 
                    fprintf(fid,'%s-kk',alphabet_i{jj});
            else, fprintf(fid,'%s',alphabet_i{jj}); end
            if jj ~= num_var, fprintf(fid,',');
            else, fprintf(fid,')+p('); end
        end
        for jj = 1:num_var % item 4, Line 1 of Sxy
            if jj == Srow,
                fprintf(fid,'%s-1-kk',alphabet_i{jj});
            elseif jj == Scol,
                    fprintf(fid,'%s-1-ll',alphabet_i{jj});
            else, fprintf(fid,'%s',alphabet_i{jj}); end
            if jj ~= num_var, fprintf(fid,',');
            else, fprintf(fid,'))...\n'); end
        end
        for jj = 1:num_var, fprintf(fid,'  '); end
        fprintf(fid,'        /((%s(%s+%s-1-kk)-%s(%s-kk)) * ',...
                alphabet_u{Srow},alphabet_i{Srow},...
                alphabet_m{Srow},alphabet_u{Srow}, ...
                alphabet_i{Srow}); % u_{i+m-1}-u_{i-1}, Line 2 of Sxy
        fprintf(fid,'(%s(%s+%s-1-ll)-%s(%s-ll))),...\n',...
                alphabet_u{Scol},alphabet_i{Scol},...
                alphabet_m{Scol},alphabet_u{Scol},...
                alphabet_i{Scol}); % v_{j+n-1}-v_{j-1}, Line 2 of Sxy
        
        % Syy
        for jj = 1:num_var+2, fprintf(fid,'  '); end
        fprintf(fid,' (%s)*(%s-1)*((p(',...
                alphabet_m{Scol},alphabet_m{Scol}); % Line 1 of Sxx >= 0
        for jj = 1:num_var, 
            if jj == Srow, fprintf(fid,'%s-kk-rr',alphabet_i{jj}); 
            else, fprintf(fid,'%s',alphabet_i{jj}); 
            end
            if jj < num_var, fprintf(fid,','); 
            end % p_{i,j}
        end
        
        fprintf(fid,')-p(');
        for jj = 1:num_var % p_{i-1,j}
            if Srow == jj, fprintf(fid,'%s-kk-rr',alphabet_i{jj});
            elseif Scol == jj, 
                    fprintf(fid,'%s-1',alphabet_i{jj});
            else, fprintf(fid,'%s',alphabet_i{jj});
            end
            if jj ~= num_var, fprintf(fid,','); % u_{i+m-1} - u_{i-1}
            else fprintf(fid,'))/(%s(%s+%s-1)-%s(%s))-...\n',...
                         alphabet_u{Scol},alphabet_i{Scol},alphabet_m{Scol},...
                         alphabet_u{Scol},alphabet_i{Scol});
            end
        end
        for ispace = 1:num_var, fprintf(fid,'  '); end
        fprintf(fid,'              (p('); % Line 2 of Sxx >= 0
        for jj = 1:num_var % p_{i-1,j}
            if Srow == jj, fprintf(fid,'%s-kk-rr',alphabet_i{jj});
            elseif Scol == jj,
                    fprintf(fid,'%s-1',alphabet_i{jj});
            else, fprintf(fid,'%s',alphabet_i{jj}); end
            if jj ~= num_var, fprintf(fid,',');
            else, fprintf(fid,')-p('); end
        end
        for jj = 1:num_var % p_{i-2,j}
            if Srow == jj, fprintf(fid,'%s-kk-rr',alphabet_i{jj});
            elseif Scol == jj,
                    fprintf(fid,'%s-2',alphabet_i{jj});
            else, fprintf(fid,'%s',alphabet_i{jj}); end
            if jj ~= num_var, fprintf(fid,','); % u_{i+m-2} - u_{i-2}
            else fprintf(fid,'))/(%s(%s+%s-2)-%s(%s-1)))...\n',...
                         alphabet_u{Scol},alphabet_i{Scol},alphabet_m{Scol},...
                         alphabet_u{Scol},alphabet_i{Scol});
                for qqq = 1:num_var, fprintf(fid,'  '); end
                fprintf(fid,'            /(%s(',alphabet_u{Scol});
            end
        end
        fprintf(fid,'%s+%s-2)-%s(%s));...\n',...% u_{m+i-2}-u_{i-1}, Line 3 of Sxx
                alphabet_m{Scol},alphabet_i{Scol},alphabet_u{Scol},alphabet_i{Scol});

        for jj = 1:num_var+2, fprintf(fid,'  '); end
        fprintf(fid,'] >= 0;\n');
        
        for jj = num_var+4:-1:1
            for ispace = 1:jj, fprintf(fid,'  '); end
            fprintf(fid,'  end\n');
        end    
end

fprintf(fid,'\ncvx_end\n');

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


