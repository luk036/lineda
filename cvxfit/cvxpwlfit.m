m = 100;
n = 4;
X = rand(m,n)*10 - 5;
%%z = log(sum(exp(X),2)); 
z = sum(X.*X,2);
%%z = z + rand(m,1)*5; % add randomness

dt=DelaunayTri(X);
%% F = TriScatteredInterp(dt,z);

%% ti = -5:.25:5;
%% [qx,qy] = meshgrid(ti,ti);
%% qz = F(qx,qy);
%% mesh(qx, qy, qz);
%% hold on;
%% plot3(x,y,z,'o');


%% es = edges(dt);
%% ne = size(es,1);
%% SI = edgeAttachments(dt, es);
SN = neighbors(dt);
ns = size(dt.Triangulation,1);

cvx_begin
  variable p(m);
  minimize( norm(z - p) );
  subject to

  for k=1:ns
    %% simplices = SI{k};
    %% if (size(simplices,2) < 2)
    %%   continue;
    %% end
    %% 
    %% e = es(k,:);
    S1 = dt.Triangulation(k,:);

    for l=1:n+1
      if (isnan(SN(k,l)))
        continue;
      end
      if (SN(k,l) < k)
        continue;  % make sure only constraint once
      end

      S2 = dt.Triangulation(SN(k,l),:);
      
      for i=1:n+1
        if (isempty(find(S2==S1(i))))
          i1 = S1(i);
          break;
        end
      end
      
      for j=1:n+1
        if (isempty(find(S1==S2(j))))
          i2 = S2(j);
          break;
        end
      end
      
      e = [S1(1:i-1), S1(i+1:n+1)];

      A = [dt.X([i1,i2],:)' -dt.X(e,:)';
           1   1   zeros(1,n)   ;
           0   0   ones(1,n)   ];
      
      b = [zeros(1,n) 1 1]';
      
      c = A\b;
      
      [p([i1;i2])', -p(e)' ]*c >= 0;
  
    end
  end

cvx_end

trisurf(dt.Triangulation,X(:,1),X(:,2),p, ...
    'FaceColor', 'cyan', 'FaceAlpha', 0.5);
%% 
%% figure
%% 
%% trisurf(dt.Triangulation,X(:,1),X(:,2),z, ...
%%    'FaceColor', 'cyan', 'FaceAlpha', 0.5);
