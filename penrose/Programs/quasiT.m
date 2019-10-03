% user defined data
n = 5;                        % dimension
%theta = zeros(n,1);           % data for offset p 
theta = rand(n,1);
x = zeros(n,n);
x(:,1) = cos(pi*[0:n-1]/n)';  % first vectors for plane E
x(:,2) = sin(pi*[0:n-1]/n)';  % second vectors for plane E
m = 4;                        % 2*m + 1 points in each dimension 
M = eye(n);                   % canonical basis of a lattice in column vectors

% offset inside the canonical basis
p = M*theta;

% Form a plane E that spanned by x1, x2
% Gram-Schmidt orthogonalization to make two orthogonal vectors in E
x(:,1) = x(:,1)/norm(x(:,1));
x(:,2) = x(:,2) - (x(:,2)'*x(:,1))*x(:,1);
x(:,2) = x(:,2)/norm(x(:,2));

% Produce orthogonal complement subspace again by Gram-Schmidt orthogonalization
for i=3:n,
  x(:,i) = rand(n,1);
  for j=i-1:-1:1,
    x(:,i) = x(:,i) - (x(:,i)'*x(:,j))*x(:,j);
    x(:,i) = x(:,i)/norm(x(:,i));
  end
end
vb = x(:,3:n);        % orthogonal vectors spanned the orthogonal complement

v = M*bingen(n)';     % vertices of unit "cube" 
vc = sum(v')'/2^n;    % vertex of centriod of unit "cube"

nn = 2^n;
% vertices in orthogonal complement subspace
ocv = [(v - vc(:,ones(nn,1)))'*vb]'; 

% generate the coordinates of vertices of the lattice
v1 = [-m:m]';
m3 = length(v1);      % = 2*m+1
B = v1;
for i=1:n-1,
   nn = m3^i;
   %D = [];
   D = zeros(m3^(i+1),i+1);
   for j=1:m3,
      vv = v1(j);
      C = [vv(ones(nn,1),:),B];
      % D((i-1)*nn+1:i*nn) = [D;C];
      D((j-1)*nn+1:j*nn,1:i+1) = C;
   end
   B = D;
end
V = M*B';     % vertices of all points
clear B C D

% perpendicularly project onto the orthogonal complement subspace
nn = m3^n;
ocp = (p'*vb)';
ocV = (V'*vb)' - ocp(:,ones(nn,1));

% only for n>=4 ----------------------------------------------------
% initial remove out not-possible points 
[temp, maxind] = max(sum(ocv .* ocv));
radius2 = norm(ocv(:,maxind)) + 0.0001;
nn=m3^n;
ind = find((sum(ocV .* ocV) - radius2) <= 0);
V = V(:,ind);
ocV = ocV(:,ind);

% prepare data for qhull
fid = fopen('ocv.in','w');
fprintf(fid,'%d for penrose \n',n-2);
fprintf(fid,'%d \n',2^n);
frmt = '%20.18g '; frmt = frmt(ones(n-2,1),:)'; frmt = frmt(:)';
fprintf(fid,[frmt,'\n'],ocv);
fclose(fid);

!qhull Qx n < ocv.in > ocv.out; rm ocv.in

% read data from qhull
fid = fopen('ocv.out','r');
size1 = fscanf(fid,'%d',1);
size2 = fscanf(fid,'%d',1);
data = fscanf(fid,'%g',[size1,size2]);
fclose(fid);

% the matrix A and the vector offset define the
% faces of the convex hull
A = data(1:n-2,:)';
offset = data(n-1,:)';
pos = sign(offset);
if any(pos >= 0), error('check offset'); end;

% find all points that are insides the convex hull
locV = size(ocV,2);
temp = sign(A*ocV + offset(:,ones(1,locV)));
ind2 = find(all(temp<=0));
V = V(:,ind2);
ocV = ocV(:,ind2);

displayonly; % for display only
%hold on
%plot(hv(:,1),hv(:,2),'c');
%hold off


% end only for ------------------------------------------------------

% generate the lattices in n-dimensional space.
mm = m3;
e = ones(mm,1);
S = spdiags([e e e], [-1:1], mm, mm);
I = speye(mm);
for i=1:n-1
   S = kron(S,I) + kron(I,S);
end
clear temp
S = S - diag(diag(S));
S = triu(S);

% perpendicularly project onto the plane (2D only)
S = S(ind,ind); 
S = S(ind2,ind2);
l = length(ind2);
%V = V - p(:,ones(l,1));
px = p'*x(:,1:2);
Vx = V'*x(:,1:2) - px(ones(l,1),:);
figure
gplot(S,Vx,'c');
axis('off')
axis('square');
