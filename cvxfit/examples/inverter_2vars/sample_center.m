function sp = sample_center(width)
    NWn = 5;  NWp = 5; 
    Wn = linspace(width(1)*1e-6*.8, width(1)*1e-6*1.2, NWn);
    Wp = linspace(width(2)*1e-6*.8, width(2)*1e-6*1.2, NWp);
    Nsample = 0;
for ii = 1 : NWn
    for jj = 1 : NWp
        fid = fopen('exp.txt','w');
          fprintf(fid, '* Generated automatically.\n\n.param\n');
          fprintf(fid, '+  length = %6.4e\n', .35e-6);
          fprintf(fid, '+  wn = %6.4e\n', Wn(ii));
          fprintf(fid, '+  wp = %6.4e\n', Wp(jj));
          fprintf(fid, '+  Vdd = %6.4e\n', 3);
        fclose(fid);

        [status,result]=system('hspice -i inverter.sp -o inverter.lis');
        if status ~= 0, return, end

        fid = fopen('inverter.lis','r');
          while 1
              tline = fgetl(fid);
              if strfind(tline,'******  transient analysis')==3, break; end
          end
          for iline = 1 : 2, tline = fgetl(fid); end
          idx = strfind(tline,'='); 
          falltime(ii,jj) = str2num(tline(idx(1)+1:idx(2)-5));
        fclose(fid);
    end
end
xe6 = Wn*1e6; ye6= Wp*1e6; 
ze9_fall = falltime*1e9; % ze9_rise = z_rise*1e9;
kx = 3; ky = 3; 
sp_fall_e = spap2({kx,ky},[kx ky],{xe6,ye6},ze9_fall);
tol = 1e-6;
knots = fnbrk(sp_fall_e,'k'); 
sp = spcvx2d(knots, [kx, ky], xe6, ye6, ze9_fall);


