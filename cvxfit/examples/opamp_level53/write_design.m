function write_design(design)
    
DateNum = now;
DateStr = datestr(DateNum,0);
fid = fopen('exp.txt','w');
fprintf(fid, '* Generated on %s.\n.param\n',DateStr);
fprintf(fid, '+  length = %6.4e\n', design.L);
fprintf(fid, '+  w12 = %6.4e\n', design.W1);
fprintf(fid, '+  w34 = %6.4e\n', design.W3);
fprintf(fid, '+  w5 = %6.4e\n', design.W5);
fprintf(fid, '+  w6 = %6.4e\n', design.W6);
fprintf(fid, '+  w7 = %6.4e\n', design.W7);
fprintf(fid, '+  w8 = %6.4e\n', design.W8);
fprintf(fid, '+  r = %6.4e\n', design.Rc);
fprintf(fid, '+  c = %6.4e\n', design.Cc);
fprintf(fid, '+  Ibias = %6.4e\n', design.I8);
fclose(fid);

