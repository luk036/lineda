m = 100;
n = 2;
x = rand(m,1)*10 - 5;
y = rand(m,1)*10 - 5;
z = log(exp(x) + exp(y)); 
z = z + rand(m,1)*1.1; % add randomness

dt=DelaunayTri(x,y);
F = TriScatteredInterp(dt,z);

ti = -5:.25:5;
[qx,qy] = meshgrid(ti,ti);
qz = F(qx,qy);
mesh(qx, qy, qz);
hold on;
plot3(x,y,z,'o');


es = edges(dt);
ne = size(es,1);
SI = edgeAttachments(dt, es);


cvx_begin
  variable p(m);
  minimize( norm(z - p) );
  subject to

  for k=1:ne
    simplices = SI{k};
    if (size(simplices,2) < 2)
      continue;
    end
  
    e = es(k,:);
    S1 = dt.Triangulation(simplices(1),:);
    S2 = dt.Triangulation(simplices(2),:);
  
    for i=1:n+1
      if (isempty(find(e==S1(i))))
        i1 = S1(i);
        break;
      end
    end
  
    for j=1:n+1
      if (isempty(find(e==S2(j))))
        i2 = S2(j);
        break;
      end
    end
  
    A = [dt.X([i1,i2],:)' -dt.X(e,:)';
         1   1   0    0    ;
         0   0   1    1    ];
  
    b = [0 0 1 1]';
  
    c = A\b;
    
    [p([i1;i2])', -p(e)' ]*c >= 0;
  
  end

cvx_end

figure;

trisurf(dt.Triangulation,x,y,p, ...
    'FaceColor', 'cyan', 'FaceAlpha', 0.8);
%%trisurf(dt.Triangulation,x,y,p,'FaceAlpha', 0.8);
%%view(0,75)
shading interp
lightangle(-45,30)
set(gcf,'Renderer','zbuffer')
set(findobj(gca,'type','surface'),...
    'FaceLighting','phong',...
    'AmbientStrength',.3,'DiffuseStrength',.8,...
    'SpecularStrength',.9,'SpecularExponent',25,...
    'BackFaceLighting','unlit')
