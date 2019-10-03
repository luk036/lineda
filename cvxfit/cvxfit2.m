x = sort([(0:10)/10,.03 .07, .93 .97]);
y = sort([(0:6)/6,.03 .07, .93 .97]);
[xx,yy] = ndgrid(x,y); z = franke(xx,yy);

% NOTE the use of NDGRID in preference to MESHGRID.
mesh(x,y,z.'), view(150,50)
title('Data from Franke function')
pause;

ky = 3; knotsy = augknt([0,.25,.5,.75,1],ky);
%sp = spap2(knotsy,ky,y,z);
coefs = spcvx1d(knotsy,ky,y,-z);
sp = spmak(knotsy,-coefs);
yy = 0:.05:1;  vals = fnval(sp,yy);
mesh(x,yy,vals.'), view(150,50)
title('Simultaneous approximation to all curves in y-direction');
pause;

coefsy = fnbrk(sp,'c');
kx = 4; knotsx = augknt(0:.2:1,kx);
%sp2 = spap2(knotsx,kx,x,coefsy.');
coefs = spcvx1d(knotsx,kx,x,-coefsy.');
sp2 = spmak(knotsx.', -coefs);

coefs = fnbrk(sp2,'c').';
xv = 0:.025:1; yv = 0:.025:1;
values = spcol(knotsx,kx,xv)*coefs*spcol(knotsy,ky,yv).';
mesh(xv,yv,values.'), view(150,50)
%fnplt(sp2); view(150,50);
title('The spline approximant with convex constraints')
pause;

%sp3=spap2({knotsx,knotsy},[kx ky],{x,y},z);
%fnplt(sp3); view(150,50);
%title('The spline approximation');
