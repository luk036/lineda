function [LSx,LSy,jmax,LSymax,prob]=period(x,y,ofac,hifac)

% np = number of frequencies examined (= length of output freqs file)
% ofac = oversampling parameter (typically >= 4 for best results)
% hifac = input parameter defined as fhi/fc, where:
% fhi is the highest frequency examined, and
% fc is the Nyquist frequency (=N/(2T)).

% ofac=4;
% hifac=2;

xmin=min(x);
xmax=max(x);
n=length(x);
T=xmax-xmin;

% fc=n/(2*T);
% fhi=hifac*fc;
nout=floor(ofac*hifac*n/2);

ave=mean(y);
variance=var(y);
xave=0.5*(xmax+xmin);
pnow=1/(T*ofac);

% for j=1:n
%     arg=2*pi*((x(j)-xave)*pnow);
%     wpr(j)=-2*sin(0.5*arg)^2;
%     wpi(j)=sin(arg);
%     wr(j)=cos(arg);
%     wi(j)=wpi(j);
% end
arg=2*pi*(x-xave)*pnow;
wpr=-2*sin(0.5*arg).^2;
wpi=sin(arg);
wr=cos(arg);
wi=wpr;

LSx=zeros(nout,1);
LSy=zeros(nout,1);
for i=1:nout
    LSx(i)=pnow;
    sumsh=sum(wi.*wr);
    sumc=sum((wr-wi).*(wr+wi));
    wtau=0.5*atan2(2*sumsh,sumc);
    swtau=sin(wtau);
    cwtau=cos(wtau);
    
    ss=wi*cwtau-wr*swtau;
    cc=wr*cwtau+wi*swtau;
    sums=sum(ss.*ss);
    sumc=sum(cc.*cc);
    yy=y-ave;
    sumsy=sum(yy.*ss);
    sumcy=sum(yy.*cc);
    wtemp=wr;
    wr=wr.*wpr-wi.*wpi+wr;
    wi=wi.*wpr+wtemp.*wpi+wi;
    
    LSy(i)=0.5*(sumcy^2/sumc+sumsy^2/sums)/variance;
    pnow=pnow+1/(ofac*T);
end

% for i=1:nout
%     LSx(i)=pnow;
%     sumsh=0;
%     sumc=0;
%     for j=1:n
%         c=wr(j);
%         s=wi(j);
%         sumsh=sumsh+s*c;
%         sumc=sumc+(c-s)*(c+s);
%     end
%     wtau=0.5*atan2(2*sumsh,sumc);
%     swtau=sin(wtau);
%     cwtau=cos(wtau);
%     sums=0;
%     sumc=0;
%     sumsy=0;
%     sumcy=0;
%     for j=1:n
%         s=wi(j);
%         c=wr(j);
%         ss=s*cwtau-c*swtau;
%         cc=c*cwtau+s*swtau;
%         sums=sums+ss^2;
%         sumc=sumc+cc^2;
%         yy=y(j)-ave;
%         sumsy=sumsy+yy*ss;
%         sumcy=sumcy+yy*cc;
%         wtemp=wr(j);
%         wr(j)=(wr(j)*wpr(j)-wi(j)*wpi(j))+wr(j);
%         wi(j)=(wi(j)*wpr(j)+wtemp*wpi(j))+wi(j);
%     end
%     LSy(i)=0.5*(sumcy^2/sumc+sumsy^2/sums)/variance;
%     pnow=pnow+1/(ofac*T);
% end

LSymax=max(LSy);
jmax=find(LSy==LSymax);

exLSy=exp(-LSymax);
% effm is an estimate of the no. of indenpendent frequencues
effm=2*nout/ofac;
prob=1-(1-exLSy)^effm;    
