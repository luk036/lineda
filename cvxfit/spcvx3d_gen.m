function sp = spcvx3d_gen(knots,k,x,y)

u = knots{1};
m = k(1);
xx = aveknt(u,m)';
nx = size(xx,1);

v = knots{2};
n = k(2);
yy = aveknt(v,n)';
ny = size(yy,1);

w = knots{3};
o = k(3);
zz = aveknt(w,o)';
nz = size(zz,1);

nn = nx*ny*nz;

cvx_quiet(true);
%% Least squares fitting with linear matrix inequality (LMI) constraints
cvx_begin sdp
  variable pp(nn)
  for iy=1:length(y)
    Cx = spcol(u,m,x(iy,1));
    Cy = spcol(v,n,x(iy,2));
    Cz = spcol(w,o,x(iy,3));
    C = kron(Cz,kron(Cy,Cx));
    err(iy) = C*pp - y(iy);
  end
  minimize( norm(err) );
  p = reshape(pp, [nx,ny,nz]);
  subject to
    for di=3:nx
      for dj=1:ny
        for dk=1:nz
          (p(di,dj,dk)-p(di-1,dj,dk))/(u(di+m-1)-u(di-1))-...
            (p(di-1,dj,dk)-p(di-2,dj,dk))/(u(di+m-2)-u(di-2))>=0;
        end
      end
    end
    for di=1:nx
      for dj=3:ny
        for dk=1:nz
          (p(di,dj,dk)-p(di,dj-1,dk))/(v(dj+n-1)-v(dj-1))-...
            (p(di,dj-1,dk)-p(di,dj-2,dk))/(v(dj+n-2)-v(dj-2))>=0;
        end
      end
    end
    for di=1:nx
      for dj=1:ny
        for dk=3:nz
          (p(di,dj,dk)-p(di,dj,dk-1))/(w(dk+o-1)-w(dk-1))-...
            (p(di,dj,dk-1)-p(di,dj,dk-2))/(w(dk+o-2)-w(dk-2))>=0;
        end
      end
    end


    for di=3:nx
      for dj=3:ny
        for dk=1:nz
          for k=0:1
            for l=0:1
              for r=0:1
                for s=0:1
                  [m*(m-1)*((p(di,dj-k-r,dk)-p(di-1,dj-k-r,dk))/(u(di+m-1)-u(di-1))-...
                    (p(di-1,dj-k-r,dk)-p(di-2,dj-k-r,dk))/(u(di+m-2)-u(di-2)))/(u(di+m-2)-u(di-1)),...
                  m*n*(p(di-l,dj-k,dk)-p(di-l,dj-k-1,dk)-p(di-l-1,dj-k,dk)+p(di-l-1,dj-k-1,dk))...
                    /(u(di-l+m-1)-u(di-l-1))/(v(dj-k+n-1)-v(dj-k-1));
                  m*n*(p(di-l,dj-k,dk)-p(di-l,dj-k-1,dk)-p(di-l-1,dj-k,dk)+p(di-l-1,dj-k-1,dk))...
                    /(u(di-l+m-1)-u(di-l-1))/(v(dj-k+n-1)-v(dj-k-1)),...
                  n*(n-1)*((p(di-l-s,dj,dk)-p(di-l-s,dj-1,dk))/(v(dj+n-1)-v(dj-1))-...
                    (p(di-l-s,dj-1,dk)-p(di-l-s,dj-2,dk))/(v(dj+n-2)-v(dj-2)))/(v(dj+n-2)-v(dj-1))
                  ] >= 0;
                end
              end
            end
          end
        end
      end
    end


    for di=3:nx
      for dj=1:ny
        for dk=3:nz
          for k=0:1
            for l=0:1
              for r=0:1
                for s=0:1
                  [m*(m-1)*((p(di,dj,dk-k-r)-p(di-1,dj,dk-k-r))/(u(di+m-1)-u(di-1))-...
                    (p(di-1,dj,dk-k-r)-p(di-2,dj,dk-k-r))/(u(di+m-2)-u(di-2)))/(u(di+m-2)-u(di-1)),...
                  m*o*(p(di-l,dj,dk-k)-p(di-l,dj,dk-k-1)-p(di-l-1,dj,dk-k)+p(di-l-1,dj,dk-k-1))...
                    /(u(di-l+m-1)-u(di-l-1))/(w(dk-k+o-1)-w(dk-k-1));
                  m*o*(p(di-l,dj,dk-k)-p(di-l,dj,dk-k-1)-p(di-l-1,dj,dk-k)+p(di-l-1,dj,dk-k-1))...
                    /(u(di-l+m-1)-u(di-l-1))/(w(dk-k+o-1)-w(dk-k-1)),...
                  o*(o-1)*((p(di-l-s,dj,dk)-p(di-l-s,dj,dk-1))/(w(dk+o-1)-w(dk-1))-...
                    (p(di-l-s,dj,dk-1)-p(di-l-s,dj,dk-2))/(w(dk+o-2)-w(dk-2)))/(w(dk+o-2)-w(dk-1))
                  ] >= 0;
                end
              end
            end
          end
        end
      end
    end


    for di=1:nx
      for dj=3:ny
        for dk=3:nz
          for k=0:1
            for l=0:1
              for r=0:1
                for s=0:1
                  [n*(n-1)*((p(di,dj,dk-k-r)-p(di,dj-1,dk-k-r))/(v(dj+n-1)-v(dj-1))-...
                    (p(di,dj-1,dk-k-r)-p(di,dj-2,dk-k-r))/(v(dj+n-2)-v(dj-2)))/(v(dj+n-2)-v(dj-1)),...
                  n*o*(p(di,dj-l,dk-k)-p(di,dj-l,dk-k-1)-p(di,dj-l-1,dk-k)+p(di,dj-l-1,dk-k-1))...
                    /(v(dj-l+n-1)-v(dj-l-1))/(w(dk-k+o-1)-w(dk-k-1));
                  n*o*(p(di,dj-l,dk-k)-p(di,dj-l,dk-k-1)-p(di,dj-l-1,dk-k)+p(di,dj-l-1,dk-k-1))...
                    /(v(dj-l+n-1)-v(dj-l-1))/(w(dk-k+o-1)-w(dk-k-1)),...
                  o*(o-1)*((p(di,dj-l-s,dk)-p(di,dj-l-s,dk-1))/(w(dk+o-1)-w(dk-1))-...
                    (p(di,dj-l-s,dk-1)-p(di,dj-l-s,dk-2))/(w(dk+o-2)-w(dk-2)))/(w(dk+o-2)-w(dk-1))
                  ] >= 0;
                end
              end
            end
          end
        end
      end
    end
cvx_end

p = reshape(pp,[nx,ny,nz]);
sp = spmak({u,v,w},p,[nx,ny,nz]);

