%% check the rusult of opamp1.m

%% open file
fid = fopen('result1.txt','w');

%% geometry size
L01;       W01;
L02 = L01; W02 = W01;
L03 = L13; W03 = 0.5*A*W13;
L04 = L13; W04 = 0.5*A*W13;
L05 = L08; W05 = A*W08;
L06 = L13; W06 = B*W13;
L07 = L08; W07 = B*W08;
L08;       W08;
L09 = L08; W09 = W08;
L10;       W10;
L11 = L10; W11 = W10;
L12 = L13; W12 = 4*W13;
L13;       W13;
L14 = L10; W14 = L10*B*sqrt(W10/L10*W13/L13)*Cc/(Cc+Cload);
A;         B;
Rb = 1/sqrt(2*KN*W13/L13*Ibias);
Cc = Cload/3;

%% current
Ibias;
I01 = 0.5 * A * Ibias;
I02 = 0.5 * A * Ibias;
I03 = 0.5 * A * Ibias;
I04 = 0.5 * A * Ibias;
I05 = A * Ibias;
I06 = B * Ibias;
I07 = B * Ibias;
I08 = Ibias;
I09 = Ibias;
I10 = Ibias;
I11 = Ibias;
I12 = Ibias;
I13 = Ibias;

%% overdrive voltage
Vod01 = sqrt(  2 * I01 / ( KP*W01/L01 ) );
Vod02 = sqrt(  2 * I02 / ( KP*W02/L02 ) );
Vod03 = sqrt(  2 * I03 / ( KN*W03/L03 ) );
Vod04 = sqrt(  2 * I04 / ( KN*W04/L04 ) );
Vod05 = sqrt(  2 * I05 / ( KP*W05/L05 ) );
Vod06 = sqrt(  2 * I06 / ( KN*W06/L06 ) );
Vod07 = sqrt(  2 * I07 / ( KP*W07/L07 ) );
Vod08 = sqrt(  2 * I08 / ( KP*W08/L08 ) );
Vod09 = sqrt(  2 * I09 / ( KP*W09/L09 ) );
Vod10 = sqrt(  2 * I10 / ( KN*W10/L10 ) );
Vod11 = sqrt(  2 * I11 / ( KN*W11/L11 ) );
Vod12 = sqrt(  2 * I12 / ( KN*W12/L12 ) );
Vod13 = sqrt(  2 * I13 / ( KN*W13/L13 ) );

%% Gm
Gm01 = sqrt( 2 * KP * W01 / L01 * I01 );
Gm02 = sqrt( 2 * KP * W02 / L02 * I02 );
Gm03 = sqrt( 2 * KN * W03 / L03 * I03 );
Gm04 = sqrt( 2 * KN * W04 / L04 * I04 );
Gm05 = sqrt( 2 * KP * W05 / L05 * I05 );
Gm06 = sqrt( 2 * KN * W06 / L06 * I06 );
Gm07 = sqrt( 2 * KP * W07 / L07 * I07 );
Gm08 = sqrt( 2 * KP * W08 / L08 * I08 );
Gm09 = sqrt( 2 * KP * W09 / L09 * I09 );
Gm10 = sqrt( 2 * KN * W10 / L10 * I10 );
Gm11 = sqrt( 2 * KN * W11 / L11 * I11 );
Gm12 = sqrt( 2 * KN * W12 / L12 * I12 );
Gm13 = sqrt( 2 * KN * W13 / L13 * I13 );

%% Go
Go01 = LAMBDAP * I01;
Go02 = LAMBDAP * I02;
Go03 = LAMBDAN * I03;
Go04 = LAMBDAN * I04;
Go05 = LAMBDAP * I05;
Go06 = LAMBDAN * I06;
Go07 = LAMBDAP * I07;
Go08 = LAMBDAP * I08;
Go09 = LAMBDAP * I09;
Go10 = LAMBDAN * I10;
Go11 = LAMBDAN * I11;
Go12 = LAMBDAN * I12;
Go13 = LAMBDAN * I13;

%% Cgs
Cgs01 = (2/3*W01*L01*COX + W01*LD*COX); 
Cgs02 = (2/3*W02*L02*COX + W02*LD*COX); 
Cgs03 = (2/3*W03*L03*COX + W03*LD*COX); 
Cgs04 = (2/3*W04*L04*COX + W04*LD*COX); 
Cgs05 = (2/3*W05*L05*COX + W05*LD*COX); 
Cgs06 = (2/3*W06*L06*COX + W06*LD*COX);
Cgs07 = (2/3*W07*L07*COX + W07*LD*COX); 
Cgs08 = (2/3*W08*L08*COX + W08*LD*COX); 
Cgs09 = (2/3*W09*L09*COX + W09*LD*COX); 
Cgs10 = (2/3*W10*L10*COX + W10*LD*COX); 
Cgs11 = (2/3*W11*L11*COX + W11*LD*COX); 
Cgs12 = (2/3*W12*L12*COX + W12*LD*COX); 
Cgs13 = (2/3*W13*L13*COX + W13*LD*COX); 

%% Cgd
Cgd01 = COX*W01*LD;
Cgd02 = COX*W02*LD;
Cgd03 = COX*W03*LD;
Cgd04 = COX*W04*LD;
Cgd05 = COX*W05*LD;
Cgd06 = COX*W06*LD;
Cgd07 = COX*W07*LD;
Cgd08 = COX*W08*LD;
Cgd09 = COX*W09*LD;
Cgd10 = COX*W10*LD;
Cgd11 = COX*W11*LD;
Cgd12 = COX*W12*LD;
Cgd13 = COX*W13*LD;

%% Area
AREA = 100e-12 + 1000*Cc + 8*( 2*W01*L01 + 2*W10*L10 +...
       W14*L14 + (2+A+B)*W08*L08 + (5+A+B)*W13*L13 );

%% Power
POWER = (2+A+B)*Ibias*(Vdd-Vss);

%% Open Loop Gain
AVV = 20*log10( 2 / (LAMBDAN+LAMBDAP)^2 ...
                * sqrt(KP*KN*W01/L01*W06/L06/I01/I06) ); 

%% CMRR
CMRR = 20*log10(1 / LAMBDAP / (LAMBDAN+LAMBDAP) ...
                * sqrt(KP*KN*W01/L01*W03/L03/I01/I03) );

%% nagetive PSRR
NPSRR = 20*log10(1 / LAMBDAN / (LAMBDAN+LAMBDAP) ...
                 * sqrt(KP*KN*W01/L01*W06/L06/I01/I06) );

%% equivalent capacitance
C1 = Cgs06 + Cgd02 + Cgd04;   
C2 = Cgs03 + Cgs04 + Cgd01;   
Ctl = Cload + Cgd06 + Cgd07;  
C3 = (C1*Cc+C1*Ctl+Cc*Ctl)/Cc;

%% four poles
WP1     = Gm01/Cc/AVV;           
WP2_inv = C3/Gm06;               
WP3_inv = C2/Gm03;               
WP4_inv = C1/Gm06*Cc/(Cc+Cload); 

%% Unitity Gain Bandwidth
WPU = Gm01/Cc;
GBW = WPU/2/pi;

%% Phase Margin
PM = 180/pi*(pi/2 -atan(WPU*WP3_inv)-atan(WPU*WP4_inv));

%% Slew Rate
SR = min( (I05/Cc), (I07/(Cc+Ctl)) );

%% Input-Referred Spot Noise
alpha = 2*KFP/COX/W01/L01*( 1 + KFN*UN*L01*L01/(KFP*UP*L03*L03) );
beta  = (16/3)*KBOLT*TEMP/sqrt(2*KP*W01/L01*I01)* ...
        (1 + sqrt( UN*W03/L03/(UP*W01/L01) ) );
IRSN  = sqrt(alpha/1000 + beta);

%% print messages
fprintf(fid,'============================================================\n');
fprintf(fid,'HSPICE Level 1 Parameters:\n');
fprintf(fid,'KBOLT   = %4.3e\n', KBOLT   );
fprintf(fid,'TEMP    = %4.3e\n', TEMP    );
fprintf(fid,'QE      = %4.3e\n', QE      );
fprintf(fid,'VT      = %4.3e\n', VT      );
fprintf(fid,'TOX     = %4.3e\n', TOX     );
fprintf(fid,'COX     = %4.3e\n', COX     );
fprintf(fid,'LD      = %4.3e\n', LD      );
fprintf(fid,'\n');
fprintf(fid,'VTN     = %4.3e\tVTP     = %4.3e\n', VTN     ,VTP     );
fprintf(fid,'UN      = %4.3e\tUP      = %4.3e\n', UN      ,UP      );
fprintf(fid,'KN      = %4.3e\tKP      = %4.3e\n', KN      ,KP      );
fprintf(fid,'LAMBDAN = %4.3e\tLAMBDAP = %4.3e\n', LAMBDAN ,LAMBDAP );
fprintf(fid,'KFN     = %4.3e\tKFP     = %4.3e\n', KFN     ,KFP     );
fprintf(fid,'KAN     = %4.3e\tKAN     = %4.3e\n', KAN     ,KAN     );
fprintf(fid,'CGDON   = %4.3e\tCGDOP   = %4.3e\n', CGDON   ,CGDOP   );
fprintf(fid,'CGSON   = %4.3e\tCGSOP   = %4.3e\n', CGSON   ,CGSOP   );
fprintf(fid,'CJN     = %4.3e\tCJP     = %4.3e\n', CJN     ,CJP     );
fprintf(fid,'CJSWN   = %4.3e\tCJSWP   = %4.3e\n', CJSWN   ,CJSWP   );
fprintf(fid,'GAMMAN  = %4.3e\tGAMMAP  = %4.3e\n', GAMMAN  ,GAMMAP  );
fprintf(fid,'PHIN    = %4.3e\tPHIP    = %4.3e\n', PHIN    ,PHIP    );
fprintf(fid,'RSHN    = %4.3e\tRSHP    = %4.3e\n', RSHN    ,RSHP    );
fprintf(fid,'PBN     = %4.3e\tPBP     = %4.3e\n', PBN     ,PBP     );
fprintf(fid,'MJN     = %4.3e\tMJP     = %4.3e\n', MJN     ,MJP     );
fprintf(fid,'MJSWN   = %4.3e\tMJSWP   = %4.3e\n', MJSWN   ,MJSWP   );
fprintf(fid,'\n');
fprintf(fid,'============================================================\n');
fprintf(fid,'Specification:\n');
fprintf(fid,'Vdd     = %4.3e\n', Vdd     );
fprintf(fid,'Vss     = %4.3e\n', Vss     );
fprintf(fid,'Cload   = %4.3e\n', Cload   );
fprintf(fid,'Lmax    = %4.3e\n', Lmax    );
fprintf(fid,'Lmin    = %4.3e\n', Lmin    );
fprintf(fid,'Wmax    = %4.3e\n', Wmax    );
fprintf(fid,'Wmin    = %4.3e\n', Wmin    );
fprintf(fid,'VCMmax  = %4.3e\n', VCMmax  );
fprintf(fid,'VCMmin  = %4.3e\n', VCMmin  );
fprintf(fid,'VOUTmin = %4.3e\n', VOUTmin );
fprintf(fid,'VOUTmax = %4.3e\n', VOUTmax );
fprintf(fid,'VODmax  = %4.3e\n', VODmax  );
fprintf(fid,'VODmin  = %4.3e\n', VODmin  );
fprintf(fid,'\n');
fprintf(fid,'============================================================\n');
fprintf(fid,'Optimization results:\n');
fprintf(fid,'A   = %4.3e\n', A  );
fprintf(fid,'B   = %4.3e\n', B  );
fprintf(fid,'Rb  = %4.3e\n', Rb );
fprintf(fid,'C1  = %4.3e\n', C1 );
fprintf(fid,'C2  = %4.3e\n', C2 );
fprintf(fid,'C3  = %4.3e\n', C3 );
fprintf(fid,'Ctl = %4.3e\n', Ctl);
fprintf(fid,'GBW = %4.3e\n', GBW);
fprintf(fid,'FP1 = %4.3e\n', WP1/2/pi);
fprintf(fid,'FP2 = %4.3e\n', 1/2/pi/WP2_inv);
fprintf(fid,'FP3 = %4.3e\n', 1/2/pi/WP3_inv);
fprintf(fid,'FP4 = %4.3e\n', 1/2/pi/WP4_inv);
fprintf(fid,'\n');
fprintf(fid,'============================================================\n');
fprintf(fid,'Specification:          Optimization results:\n');
fprintf(fid,'AREA    = %4.3e    %4.3e\n', AREAmax , AREA  );
fprintf(fid,'POWER   = %4.3e    %4.3e\n', POWERmax, POWER );
fprintf(fid,'GBW     = %4.3e    %4.3e\n', GBWmin  , GBW   );
fprintf(fid,'PM      = %4.3e    %4.3e\n', PMmin   , PM    );
fprintf(fid,'AVV     = %4.3e    %4.3e\n', AVVmin  , AVV   );
fprintf(fid,'CMRR    = %4.3e    %4.3e\n', CMRRmin , CMRR  );
fprintf(fid,'NPSRR   = %4.3e    %4.3e\n', NPSRRmin, NPSRR );
fprintf(fid,'PPSRR   = %4.3e         \n', PPSRRmin        );
fprintf(fid,'SR      = %4.3e    %4.3e\n', SRmin   , SR    );
fprintf(fid,'IRSN    = %4.3e    %4.3e\n', IRSNmax , IRSN  );
fprintf(fid,'\n');
fprintf(fid,'============================================================\n');
fprintf(fid,'Optimization results:\n');
fprintf(fid,'Param   MOS_01      MOS_02      MOS_03');
fprintf(fid,'        MOS_04      MOS_05      MOS_06\n');
fprintf(fid,'W       %4.3e  %4.3e  %4.3e  ',W01, W02, W03);
fprintf(fid,'%4.3e  %4.3e  %4.3e\n',W04, W05, W06);
fprintf(fid,'L       %4.3e  %4.3e  %4.3e  ',L01, L02, L03);
fprintf(fid,'%4.3e  %4.3e  %4.3e\n',L04, L05, L06);
fprintf(fid,'Id      %4.3e  %4.3e  %4.3e  ',I01, I02, I03);
fprintf(fid,'%4.3e  %4.3e  %4.3e\n',I04, I05, I06);
fprintf(fid,'Vdsat   %4.3e  %4.3e  %4.3e  ',Vod01, Vod02, Vod03);
fprintf(fid,'%4.3e  %4.3e  %4.3e\n',Vod04, Vod05, Vod06);
fprintf(fid,'Gm      %4.3e  %4.3e  %4.3e  ',Gm01, Gm02, Gm03);
fprintf(fid,'%4.3e  %4.3e  %4.3e\n',Gm04, Gm05, Gm06);
fprintf(fid,'Gds     %4.3e  %4.3e  %4.3e  ',Go01, Go02, Go03);
fprintf(fid,'%4.3e  %4.3e  %4.3e\n',Go04, Go05, Go06);
fprintf(fid,'Cgs     %4.3e  %4.3e  %4.3e  ',Cgs01, Cgs02, Cgs03);
fprintf(fid,'%4.3e  %4.3e  %4.3e\n',Cgs04, Cgs05, Cgs06);
fprintf(fid,'Cgd     %4.3e  %4.3e  %4.3e  ',Cgd01, Cgd02, Cgd03);
fprintf(fid,'%4.3e  %4.3e  %4.3e\n',Cgd04, Cgd05, Cgd06);
fprintf(fid,'\n');
fprintf(fid,'Param   MOS_07      MOS_08      MOS_09');
fprintf(fid,'        MOS_10      MOS_11      MOS_12\n');
fprintf(fid,'W       %4.3e  %4.3e  %4.3e  ',W07, W08, W09);
fprintf(fid,'%4.3e  %4.3e  %4.3e\n',W10, W11, W12);
fprintf(fid,'L       %4.3e  %4.3e  %4.3e  ',L07, L08, L09);
fprintf(fid,'%4.3e  %4.3e  %4.3e\n',L10, L11, L12);
fprintf(fid,'Id      %4.3e  %4.3e  %4.3e  ',I07, I08, I09);
fprintf(fid,'%4.3e  %4.3e  %4.3e\n',I10, I11, I12);
fprintf(fid,'Vdsat   %4.3e  %4.3e  %4.3e  ',Vod07, Vod08, Vod09);
fprintf(fid,'%4.3e  %4.3e  %4.3e\n',Vod10, Vod11, Vod12);
fprintf(fid,'Gm      %4.3e  %4.3e  %4.3e  ',Gm07, Gm08, Gm09);
fprintf(fid,'%4.3e  %4.3e  %4.3e\n',Gm10, Gm11, Gm12);
fprintf(fid,'Gds     %4.3e  %4.3e  %4.3e  ',Go07, Go08, Go09);
fprintf(fid,'%4.3e  %4.3e  %4.3e\n',Go10, Go11, Go12);
fprintf(fid,'Cgs     %4.3e  %4.3e  %4.3e  ',Cgs07, Cgs08, Cgs09);
fprintf(fid,'%4.3e  %4.3e  %4.3e\n',Cgs10, Cgs11, Cgs12);
fprintf(fid,'Cgd     %4.3e  %4.3e  %4.3e  ',Cgd07, Cgd08, Cgd09);
fprintf(fid,'%4.3e  %4.3e  %4.3e\n',Cgd10, Cgd11, Cgd12);
fprintf(fid,'\n');
fprintf(fid,'Param   MOS_13      MOS_14\n');
fprintf(fid,'W       %4.3e  %4.3e\n',W13, W14);
fprintf(fid,'L       %4.3e  %4.3e\n',L13, L14);
fprintf(fid,'Id      %4.3e\n',I13);
fprintf(fid,'Vdsat   %4.3e\n',Vod13);
fprintf(fid,'Gm      %4.3e\n',Gm13);
fprintf(fid,'Gds     %4.3e\n',Go13);
fprintf(fid,'Cgs     %4.3e\n',Cgs13);
fprintf(fid,'Cgd     %4.3e',Cgd13);
fprintf(fid,'\n');

%% close file
fclose(fid);
