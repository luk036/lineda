clear all
kx = 4; 
ky = 4; 

x = [0.1:0.1:1];
y = [0.1:0.1:1];
[xx,yy] = ndgrid(x,y); 
z = simple_fn(xx,yy);
%z = z + 0.04*rand(size(z));

kx = 4; 
ky = 4; 

%sp3=spap2({6,6},[kx ky],{log(x),log(y)}, log(z));
sp3=spap2({6,6},[kx ky],{x,y}, z);
%subplot(2,1,1);
fnplt(sp3); 
view(150,50);
title('Data fitting by spap2');

xd = [xx(:), yy(:)]';
f = z(:)';
[q1,q0,b,c] = rank1fit(xd,f,1e-4,100)

x = [0.1:0.02:1];
y = [0.1:0.02:1];
[xx,yy] = ndgrid(x,y); 
xd = [xx(:), yy(:)]';
inv_x = 1./xd;
x2 = [inv_x; xd];
ff = ((q1'*x2).*(q0'*x2) + b'*x2 + c);
zz = reshape(ff, [length(x), length(y)]);
figure
%mesh(log(x),log(y),log(zz.')), view(150,50)
mesh(x,y,zz.'), view(150,50)

