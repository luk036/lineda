function sp = spcvx2d_gen(knots,k,x,y)

u = knots{1};
m = k(1);
xx = aveknt(u,m)';
nx = size(xx,1);

v = knots{2};
n = k(2);
yy = aveknt(v,n)';
ny = size(yy,1);

nn = nx*ny;

cvx_quiet(true);
%% Least squares fitting with linear matrix inequality (LMI) constraints
cvx_begin sdp
  variable pp(nn)
  for iy=1:length(y)
    Cx = spcol(u,m,x(iy,1));
    Cy = spcol(v,n,x(iy,2));
    C = kron(Cy,Cx);
    err(iy) = C*pp - y(iy);
  end
  minimize( norm(err) );
  p = reshape(pp, [nx,ny]);
  subject to
    for di=3:nx
      for dj=1:ny
        (p(di,dj)-p(di-1,dj))/(u(di+m-1)-u(di-1))-...
          (p(di-1,dj)-p(di-2,dj))/(u(di+m-2)-u(di-2))>=0;
      end
    end
    for di=1:nx
      for dj=3:ny
        (p(di,dj)-p(di,dj-1))/(v(dj+n-1)-v(dj-1))-...
          (p(di,dj-1)-p(di,dj-2))/(v(dj+n-2)-v(dj-2))>=0;
      end
    end


    for di=3:nx
      for dj=3:ny
        for k=0:1
          for l=0:1
            for r=0:1
              for s=0:1
                [m*(m-1)*((p(di,dj-k-r)-p(di-1,dj-k-r))/(u(di+m-1)-u(di-1))-...
                  (p(di-1,dj-k-r)-p(di-2,dj-k-r))/(u(di+m-2)-u(di-2)))/(u(di+m-2)-u(di-1)),...
                m*n*(p(di-l,dj-k)-p(di-l,dj-k-1)-p(di-l-1,dj-k)+p(di-l-1,dj-k-1))...
                  /(u(di-l+m-1)-u(di-l-1))/(v(dj-k+n-1)-v(dj-k-1));
                m*n*(p(di-l,dj-k)-p(di-l,dj-k-1)-p(di-l-1,dj-k)+p(di-l-1,dj-k-1))...
                  /(u(di-l+m-1)-u(di-l-1))/(v(dj-k+n-1)-v(dj-k-1)),...
                n*(n-1)*((p(di-l-s,dj)-p(di-l-s,dj-1))/(v(dj+n-1)-v(dj-1))-...
                  (p(di-l-s,dj-1)-p(di-l-s,dj-2))/(v(dj+n-2)-v(dj-2)))/(v(dj+n-2)-v(dj-1))
                ] >= 0;
              end
            end
          end
        end
      end
    end
cvx_end

p = reshape(pp,[nx,ny]);
sp = spmak({u,v},p,[nx,ny]);

