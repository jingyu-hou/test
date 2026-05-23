// Rolling Soft.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Rolling Soft.h"
#include "math.h"
#include "fstream.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define  CALC_API(rtntype)   extern "C" rtntype PASCAL EXPORT   

//***************************************************************

double Km,Qp,Torque,Power,Ld;
double Xv,d;
//double StrainA=0;
double Force;
double Strain,StrainRate;
double Xfe;//奥氏体分数
double Xp;//珠光体分数
double Xb;//贝氏体分数
double dfe;//铁素体晶粒度
double DCurlSize;//卷取时晶粒尺寸
double Tb;//抗拉强度
double Ts;//屈服强度
double LenRatio;//延伸率
double T5;//在结晶发生50%的时间
double StrainC;//临界应变
//相变的温度与分数

 double l_Fe_Xv[100];
 double l_Fe_Temp[100];

 double l_P_Xv[100];
 double l_P_Temp[100];
/*
double Fe_Xv;
double Fe_Temp;

double *l_P_Xv;
double *l_P_Temp;
*/
//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CRollingSoftApp

BEGIN_MESSAGE_MAP(CRollingSoftApp, CWinApp)
//{{AFX_MSG_MAP(CRollingSoftApp)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRollingSoftApp construction

CRollingSoftApp::CRollingSoftApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CRollingSoftApp object

CRollingSoftApp theApp;
/////////////////////////////计算部分//////////////////////////////////////////////
//加热模块计算
//晶粒长大模型
extern "C" double  PASCAL EXPORT Reheating()
{
	double d0;//初始晶粒度
	double t;//时间
	const double R=8.314;
	double T;//温度（K)
	double d;//晶粒度
	double d1;
	d0=50.0;//um
	t=7200.0;
	T=1230.0+273.0;
	d1=pow(d0,2.6)+4.3*1.0e9*exp(-190000.0/(R*T))*t;
	d=pow(d1,(1.0/2.6));
	return d;
}

extern "C" double PASCAL EXPORT Reheatingphase(double Time,  /*加热保温时间min*/
											   double Temp   /*在炉温度C*/
											   )
{
	const double R=8.314;
	double d;//晶粒度
	double d1;
	double d0;
	d0=50.0;
	Time=Time*60.0; //转化为秒
	Temp=Temp+273.0; //转化为K
	d1=pow(d0,2.6)+4.3*1.0e9*exp(-190000.0/(R*Temp))*Time;
	d=pow(d1,(1.0/2.6));
	return d;
}

extern "C" int PASCAL EXPORT ReheatingDrawing(double Time, double Temp, double * pTime,
											   double * pTemp, double * pDgrain)
{
	int N_Step;
	int i;
	int j;
	int k;
	double D_Time;

	N_Step=95;
    Time=Time*60.0; //将分转化为秒
	D_Time=Time/double(N_Step-1);
	
	for(i=0;i<N_Step;i++)
	{
		pTime[i]=0.0;
		pTemp[i]=0.0;
		pDgrain[i]=0.0;
	}

	//求温度曲线
    if(Time<=120.0)
	{
		for(i=1;i<N_Step;i++)
		{
			pTime[i]=pTime[i-1]+D_Time;
			pTemp[i]=(Temp/Time)*pTime[i];
		}
	}
	if(Time>120.0&&Time<=8760.0)
	{
		for(i=1;i<N_Step;i++)
		{
			pTime[i]=pTime[i-1]+D_Time;
			if(pTime[i]<=120.0)
			{
				pTemp[i]=(350.0/120.0)*pTime[i];
			}
			if(pTime[i]>120.0&&pTime[i]<=8760.0)
			{
				pTemp[i]=350.0+(Temp-350.0)/(Time-120.0)*(pTime[i]-120.0);
			}
		}
	}
	if(Time>8760.0)
	{
		for(i=1;i<N_Step;i++)
		{
			pTime[i]=pTime[i-1]+D_Time;
            if(pTime[i]<=120.0)
			{
				pTemp[i]=(350.0/120.0)*pTime[i];
			}
			if(pTime[i]>120.0&&pTime[i]<=8760.0)
			{
				pTemp[i]=350.0+(1096.0-350.0)/(8760.0-120.0)*(pTime[i]-120.0);
			}
			if(pTime[i]>8760.0)
			{
				pTemp[i]=1096.0+(Temp-1096.0)/(Time-8760.0)*(pTime[i]-8760.0);
			}
		}
	}
	//奥氏体化后晶粒长大曲线

	for(i=1;i<N_Step;i++)
	{
		if(pTemp[i]>780.0)
		{
			k=i;
			break;
		}
	}

	/*for(i=1;i<N_Step;i++)
	{
		if(pTemp[i]>780.0)
		{
			pDgrain[i]=pow(pDgrain[i-1],3.7)+
				(5.6e+24)*exp(-494000.0/8.314/(pTemp[i]+273.0))*(pTime[i]-pTime[i-1]);
		    pDgrain[i]=pow(pDgrain[i],1.0/3.7);
		}	
	}*/

	for(i=k;i<N_Step;i++)
	{
		for(j=k;j<=i;j++)
		{
			pDgrain[i]=pDgrain[i]+
				(5.6e+24)*exp(-494000.0/8.314/(pTemp[j]+273.0))*(pTime[j]-pTime[j-1]);
		}
		pDgrain[i]=pow(pDgrain[i],1.0/3.7);
	}

	return 0;
}

//轧制模块计算

//Air Cooling DTair
extern "C" double PASCAL EXPORT AirCooling( double InitTemp,double Hcal,double Cp,double DTime)
{
	double DTair;
	const double Ep=0.67;//辐射率:0.67
	const double Sig=5.666*(1.0e-8);//Stenfan_Bolzman
	const double Gam=7856.0;
	//const double Lct=1.1; //Temperature Learning Coefficient
	//const double Cp; //Rolling:0.157,Finishing 0.175
	//double H;//Thickness
	double DT;
	double const1,const2;
	//Lct=1.1;
	const1=pow((InitTemp+273.0),-3.0);
	const2=6.0*Ep*Sig*DTime/(Cp*Gam*Hcal);
	DT=pow((const1+const2),(-1.0/3.0));
    DTair=(DT-(InitTemp+273.0));
	return DTair;
}
//考虑保温罩的空冷
extern "C" double PASCAL EXPORT AirCoolingR_F
( double InitTemp,double Hcal,double Cp,double DTime)
{
	double DTair;
    double Ep;//辐射率:0.67
	const double Sig=5.666*(1.0e-8);//Stenfan_Bolzman
	const double Gam=7856.0;
	//const double Lct=1.1; //Temperature Learning Coefficient
	//const double Cp; //Rolling:0.157,Finishing 0.175
	//double H;//Thickness
	double EPG=0.2;
	double DT;
	Ep=0.67-0.2*10.0/12.0;
	double const1,const2;
	const1=pow((InitTemp+273.0),-3.0);
	const2=6.0*Ep*Sig*DTime/(Cp*Gam*Hcal);
	DT=pow((const1+const2),(-1.0/3.0));
    DTair=(DT-(InitTemp+273.0));
	return DTair;
}

//WaterCooling
extern "C" double PASCAL EXPORT WaterCooling(double InitTemp,double DTime,double Cp,double H)
{
	double DTwater;
	double WaterTemp;
	const double Aah=290.278;//Heat Transmission Coefficient
    double Gam=7856.0;
	//double Lct=1.1;//Temperature Learning Coefficient
    WaterTemp=40.0;
	DTwater=((InitTemp-WaterTemp)*(exp((-2*Aah*DTime)/(Cp*Gam*H))-1));
	return DTwater;
}

//WaterCooling modified
extern "C" double PASCAL EXPORT WatCal(int Iz, double InitTemp,
											 double DTime,double Cp,double H)
{
	//Iz=0, Coolant, Iz=1, Descaler, Iz=2, Spray, 3, 4 FSB;
	double DTwater;
	double WaterTemp;
	double Aah;//Heat Transmission Coefficient
    double Gam=7856.0;
	//double Lct=1.1;//Temperature Learning Coefficient
	if(Iz==0)Aah=139.33;
	if(Iz==1)Aah=1741.67;
	if(Iz==2)Aah=290.28;
	if(Iz==3)Aah=2090.0;
	if(Iz==4)Aah=1451.39;
    WaterTemp=40.0;
	Cp=Cp*4180.0;
	DTwater=((InitTemp-WaterTemp)*(exp((-2*Aah*DTime)/(Cp*Gam*H))-1));
	return DTwater;
}

//Heat Tramission by Roll Contact
extern "C" double PASCAL EXPORT HeatContact(double EntryTemp,double Hpre,
											double Hcur,double Cp,double Ram,double byte1,double ContactTime)
{
	//byte Roughing:0.539,finishing 0.532
	//Ram:Heat Conduction Ratio Rolling 23,Finishing 23.4
	//double Lct;//精轧温度学习系数
	double DContactTemp;
    double RollTemp;
	double Pi=3.14159;
	double Ks;
    const double Gam=7856.0;
	//Lct=1.1;
    RollTemp=100.0;
    Ks=Ram/(Cp*Gam);
	DContactTemp=byte1*(RollTemp-EntryTemp)*3.0/(Hpre+2.0*Hcur)
		*sqrt((Ks*ContactTime)/Pi)*4.0;
	return 0.65*DContactTemp;
}

extern "C" double PASCAL EXPORT CalHeatCont(double EntryTemp,double Hpre,
											double Hcur,double Cp,double Ram,
											double byte1,double ContactTime,
											double LengthPerSeg, int iSeg)
{
	//byte Roughing:0.539,finishing 0.532
	//Ram:Heat Conduction Ratio Rolling 23,Finishing 23.4
	//double Lct;//精轧温度学习系数
	double DContactTemp;
    double RollTemp;
	double Pi=3.14159;
	double Ks;
    const double Gam=7856.0;
	double Length;
	double DeltTCont;
	double DeltT;
	//Lct=1.1;
	Length=LengthPerSeg*iSeg;
	if(Length<=30.0)
	{
		RollTemp=25.0+Length/30.0*(100.0-25.0);
	}
	else
	{
        RollTemp=100.0;
	}
    Ks=Ram/(Cp*Gam);
	DContactTemp=byte1*(RollTemp-EntryTemp)*3.0/(Hpre+2.0*Hcur)
		*sqrt((Ks*ContactTime)/Pi)*4.0;
	DeltTCont=byte1*(100.0-EntryTemp)*3.0/(Hpre+2.0*Hcur)
		*sqrt((Ks*ContactTime)/Pi)*4.0;

	DeltT=0.65*DContactTemp;

	return DeltT;
}

//Calculate Rd,Ld,Km
extern "C" void PASCAL EXPORT Phycal(double Hpre,double Hcur,double RRoll,double VRoll,
									 double Wmean,double Tmean,double CC, int iSeg, 
									 int iPass)
{
	double DH,R,RRollDz,RRollD,SigF;
	int IC;
	double Young=20600.0;
	const double Poiss=0.3;
	const double Pi=3.14159;
	double N;
	double C;
	double TD;
	double T;
	double G;
	double TX;
	double M;
	double AA;
	double C0;
	double P;

	Hpre=Hpre*1000.0;
	Hcur=Hcur*1000.0;
	RRoll=RRoll*1000.0;
	Wmean=Wmean*1000.0;
	//CC=CC/100.0;

    DH=Hpre-Hcur;
	R=DH/Hpre;
	RRollDz=0;
	RRollD=RRoll;
    IC=0;
	if(DH<=0.0)
	{
		Ld=0.0;
		Strain=0.0;
		StrainRate=0.0;
		Km=0.0;
		Qp=0.0;
		RRollD=0.0;
		Force=0.0;
		Torque=0.0;
		Power=0.0;
	}
	else
	{
	    while (fabs(RRollD-RRollDz)>0.1)
		{
		    IC=IC+1;
		    if(IC>5)
			    break;
		    //Projected Contact Length
		    Ld=sqrt(RRollD*DH*(1-(DH/(4*RRollD))));
            Strain=log(1/(1-R));
		    StrainRate=VRoll*1000.0/sqrt(RRollD*Hpre)/sqrt(R)*Strain;
		    N=0.41-0.07*CC;
		    TD=0.95*(CC+0.41)/(CC+0.32);
		    T=(Tmean+273.0)/1000.0;//?
		    if (T>=TD) 
			{
			    G=1.0;
			    TX=T;
			    M=(-0.019*CC+0.126)*T+(0.075*CC-0.050);
			}
		
		    else
			{
			    G=30.0*(CC+0.90)*pow((T-0.95*(CC+0.49)/(CC+0.42)),2.0)
				    +(CC+0.06)/(CC+0.09);
			    TX=TD;
			    M=(0.081*CC-0.154)*T+(-0.019*CC+0.207)+0.027/(CC+0.320);
			
			}
		
		    SigF=0.28*G*exp(5.0/TX-0.01/(CC+0.05));
		    //	AA=0.186*0.171/100.0+(-0.029)*0.44/100.0+0.988;
		    AA=0.186*0.171+(-0.029)*0.44+0.988;
		    Km=2.0/sqrt(3.0)*AA*SigF*(1.3*pow((Strain/0.2),N)-0.3*(Strain/0.2))*
			    pow((StrainRate/10.0),M);
		    //Rolling Force Function
		    if (R<=0.15)
			    C=0.052/sqrt(R)+0.016;
		    else 
			    C=0.2*R+0.12;
		    Qp=0.8+C*(sqrt(RRollD/Hpre)-0.5);
		
		    //Flattening Roll Radius
		    RRollDz=RRollD;
	    	C0=16.0*(1.0-pow(Poiss,2.0))/(Pi*Young);
		    P=Km*Ld*Qp;
		    RRollD=(1.0+C0/DH*P)*RRoll;
		}//End of While
	
	    //Rolling Force Calculation
	    Force=Km*Ld*Qp*Wmean/1000.0;
	
	    //Rolling Torque Calculation
	    Torque=0.5*Ld/1000.0*Force;
	
	    //Rolling Power Calculation 
	    Power=9.81*(1.0/0.9)*VRoll/(RRoll/1000.0)*Torque;
	}
	
	Ld=Ld/1000.0;
	Hpre=Hpre/1000.0;
	Hcur=Hcur/1000.0;
	RRoll=RRoll/1000.0;
	Wmean=Wmean/1000.0;
	//CC=CC*100.0;
}

//计算轧制力
extern "C" double PASCAL EXPORT  CalcForce()
{
	return Force;
}


//计算从加热炉到粗轧时间
//CALC_API(double)  Time1Cal(double Length)
extern "C" double PASCAL EXPORT Time1Cal(double Length)
{
	double t[11];
	double l4;
	double l3;
	int i;
    double Time1;
	Time1=0.0;
	/*
	t[0]=40;
	t[1]=1.0/1.1;
	t[2]=35.7875-24.0-1.0/2.2;
	*/
	t[0]=46.9735-35.7875;
	t[1]=1.0/1.1;
	t[2]=10.0;
	t[3]=0.3/1.1;
	t[4]=(50.9732-46.9735+Length)/0.3;
	t[5]=(3.0-0.3)/1.1;
	t[6]=(91.545475-50.9375-(9.0-0.09)/2.2)/3.0;
	t[7]=(5.0-3.0)/1.1;
	l3=(25.0-9.0)/2.2;
	t[8]=(141.155-91.5457-l3-Length)/5.0;

	t[9]=(1.44-5.0)/(-1.1);
	l4=(pow(1.44,2)-25.0)/(-2.2);
	t[10]=(154.3125-141.155-l4-6.15)/1.44;
    for(i=0;i<11;i++)
		Time1+=t[i];
	return Time1;
}



//计算粗轧时间
extern "C" double PASCAL EXPORT TimeCal(int i,double H0,double Length,double Hcur,double Vr)
{
	double L_bar;
	double t[6];
	int j;
	double Time;
	Time=0.0;
	L_bar=Length*H0/Hcur;
	if((i%2)==1)
	{
		t[0]=(L_bar+4.85)/Vr+2.0;
		t[1]=1.9/Vr;
		t[2]=1.35/Vr;
		t[3]=1.7/Vr;
		t[4]=1.2/Vr;
		t[5]=1.2/Vr;
	}
	else 
	{
		t[0]=(L_bar+3.0)/Vr+2.0;
		t[1]=1.9/Vr;
		t[2]=0.55/Vr;
		t[3]=0.55/Vr;
		t[4]=1.2/Vr;
		t[5]=1.2/Vr;
	}
	for(j=0;j<6;j++)
	{
		Time+=t[j];
	}
	return Time;
	
}

//粗精轧之间的时间
extern "C" double PASCAL EXPORT TimeRFCal(double Length)
{
	double t[5];
	double Time;
	int i;
	Time=0.0;
	t[0]=(Length-12.0)/5.0;
	t[1]=fabs((1.6-5.0)/2.133);
	t[2]=(346.1-246.5)-(Length-12.0)-
		(fabs(pow(1.6,2.0)-pow(5.0,2.0))/(2.0*2.133))-
		(fabs(pow(1.5,2.0)-pow(5.0,2.0))/(2.0*2.133));
	t[2]=t[2]/1.6;
	t[3]=fabs(1.5-1.6)/2.133;
	t[4]=12.0/5.0;
	for(i=0;i<5;i++)
	{
		Time+=t[i];
	}
	return Time;
}

//计算表面温度
extern "C" double PASCAL EXPORT FaceTempCal(double Temp,double Hcur,int idxRam)
{
	double QA;
	double RDT;
	double Ram;
	
	Ram = CalcRam(Temp , idxRam);
    QA=0.7 * 4.88* (1.0e-8) * pow( (Temp+15.0+273.0) , 4.0);
    RDT=Temp-(15.0+0.5 * QA * Hcur / (6.0*Ram) );
	/*
	char s[128]  ;
	sprintf(s,"Ram=%f4.2; QA=%f4.2; RDT=%f4.2",Ram, QA, RDT);
	::MessageBox(NULL,s,"Debug",MB_OK);
	*/
	return RDT;
}

extern "C" double PASCAL EXPORT LdCal()
{
	return Ld;
}
//Heat Generate by Deformation

extern "C" double PASCAL EXPORT HeatDeformation(double Cp,double Hpre,double Hcur)
{
	double DDeformationTemp;
	//const double Lct=1.1;//Temperature Learning Coefficient
	double Gam=7856.0;
	const double J1=1/427.0;//Conversion Constant
	DDeformationTemp=Km*log(Hpre/Hcur)*J1*(1.0e6)/(Cp*Gam);
	return 0.9*DDeformationTemp;
}

//Heat Gnenerate by Deformation modified model

extern "C" double PASCAL EXPORT HeatDefCal(int i, double Cp,double Hpre,double Hcur)
{
	double Lcf[7];//精轧各道次轧制力学习系数
	double DDeformationTemp;
	//const double Lct=1.1;//Temperature Learning Coefficient
	double Gam=7856.0;
	const double J1=1/427.0;//Conversion Constant
	Lcf[0]=0.76;
	Lcf[1]=0.77;
	Lcf[2]=0.77;
	Lcf[3]=0.69;
	Lcf[4]=0.68;
	Lcf[5]=0.66;
	Lcf[6]=0.66;
	DDeformationTemp=Lcf[i-1]*Km*log(Hpre/Hcur)*J1*(1.0e6)/(Cp*Gam);
	return 0.9*DDeformationTemp;
}

//Heate Generate by Friction
extern "C" double PASCAL EXPORT HeatFriction(double Vr,double Hpre,double Hcur,double Width,double byte1,double Tim)
{
	//double Lct;//精轧温度学习系数
	double DTFriction;
	double Gam=7856.0;
    const double J2=0.239;//Conversion Constant
	double Ha;
	const double CJ=2.3419*(1.0e-3);
	Ha=(Hpre+2.0*Hcur)/3.0;
    double myu=0.3;
	double red;
	double fs;
	double fb;
	double dvd;
	//Lct=1.1;
	red=(Hpre-Hcur)/Hpre;
	fs=red/4.0;
	fb=1.0-(1.0+fs)*(1.0-red);
	if (red!=0.0)
		dvd=Vr*(fs*fs+fb*fb)/(2.0*(fs+fb)*(1.0+fs));
	else 
		dvd=0.0;
		/*DTFriction=((Power/Hcur*Vr*Width)/(Cp*Gam)*J2-
	HeatDeformation( Cp, Hpre, Hcur))*byte1;*/
	DTFriction=2.0*CJ*myu*Km*dvd*Tim/(Ha*0.183*Gam)*(1.0e6);
	return 0.5*DTFriction;
}

//Recrysterlliztion
extern "C" void PASCAL EXPORT Recrysterllize(int iii, int jjj, double Strain,double StrainRate,
											 double Temp,double d0,double time)
{
    double Z;
	double dmrx;
	double dsrx;
    double d1;
	double const1;
    if(iii==12&&jjj==2)
	{
		iii=iii;
		/*MessageBox(NULL, "This is the 12th segment, and the 2nd pass.", 
			"Information", MB_OK|MB_ICONSTOP);*/
	}
	if(StrainRate==0.0||Strain==0.0)
	{
		Xv=0.0;
		d=d0;
		T5=0.0;
		d1=pow(d0,7.0)+8.2*(1.0e25)*(time-2.65*T5)*exp(-400000.0/(8.314*Temp));
	    d=pow(d1,1.0/7.0);
	}
	else
	{
        Z=StrainRate*exp(300000.0/(8.314*Temp));
	    const1=pow(Z,0.17);
	    StrainC=5.6*(1.0e-4)*const1*pow(d0,0.3);
        //StrainC=0.8*3.9*1e(-4)*pow(d0,0.5)*const1;
	    Xv=0;
	    // Strain=Strain+StrainA;
	
	    if (Strain>StrainC)   //动态再结晶 
		{
		    T5=1.1*pow(Z,-0.8)*exp(240000.0/(8.314*Temp));/////////////////
		    Xv=1.0 - exp(-0.693*pow((time/T5),1.5));
		    dmrx=2.6*(1.0e4)*pow(Z,-0.23);
            if (Xv<=0.95)
			{	
		        d=pow(Xv,3.5/3.0)*dmrx+pow((1.0-Xv),1.0)*d0;
		    //	d=pow(Xv,3.1/3.0)*dmrx+pow((1.0-Xv),1.0)*d0;
			
			}
		    if((time<=1.0)&&(Xv>0.95))
			{
			    d1=pow(dmrx,2.0)+1.2*(1.0e7)*(time-2.65*T5)*exp(-113000.0/(8.314*Temp));
                d=pow(d1,1.0/2.0);
			}
		    if ((time>1.0)&&(Xv>0.95))
			{
			    d1=pow(dmrx,7.0)+8.2*(1.0e25)*(time-2.65*T5)*exp(-400000.0/(8.314*Temp));
			    d=pow(d1,1.0/7.0);
			}
		}
	    else
		
		{
		    T5=2.3*(1.0e-15)*pow(Strain,-2.5)*pow(d0,2.0)*exp(230000.0/(8.314*Temp));
		    Xv=1.0-exp(-0.693*(time/T5));
		    dsrx=343.0*pow(d0,0.4)*pow(Strain,-0.5)*exp(-45000.0/(8.314*Temp));
		    if ((time<=1.0)&& (Xv>0.95))
			{
			    d1=pow(dsrx,2.0)+4.0*(1.0e7)*(time-4.32*T5)*exp(-113000.0/(8.314*Temp));
			    d=pow(d1,1.0/2.0);
			}
		    if((time>1.0)&&(Xv>0.95))
			{
			    d1=pow(dsrx,7.0)+1.5*(1.0e27)*(time-4.32*T5)*exp(-400000.0/(8.314*Temp));
			    d=pow(d1,1.0/7.0);
			}
		    if (Xv<=0.95)
			{
			
			    d=pow(Xv,3.5/3.0)*dsrx+pow((1.0-Xv),1.0)*d0;
		     //	d=pow(Xv,3.1/3.0)*dsrx+pow((1.0-Xv),1.0)*d0;
			
			}
		}
	}
}

//计算临界应变
extern "C" double PASCAL EXPORT CalcStrainC()
{
	return StrainC;
}
//计算T0.5
//计算临界应变
extern "C" double PASCAL EXPORT CalcThalf()
{
	return T5;
}

extern "C" double PASCAL EXPORT StrainCal()
{
	return Strain;
}
extern "C" double PASCAL EXPORT StrainRateCal()
{
	return StrainRate;
}
/*
extern "C" double PASCAL EXPORT StrainCal(double Hpre,double Hcal)
{
double r;
double Strain;
r=(Hpre-Hcur)/Hpre;
Strain=log(1/(1-r));
return Strain;
}
double StrainRate(double Vr,double 

*/
extern "C" double PASCAL EXPORT XvCal()
{
	return Xv;
}
extern "C" double	PASCAL EXPORT dCal()
{
	return d;
}
/*
//#define __CoolandPhrase_Write__

#ifdef  __CoolandPhrase_Write__
extern "C" int PASCAL EXPORT CoolandPhrase(double dy,double C,	double Mn,double Si,
											double RemStrain ,double * Fe_Xv,double * Fe_Temp,
											double *P_Temp,double * P_Xv,double CoolV )
#else
extern "C" void PASCAL EXPORT CoolandPhrase(double dy,double C,	double Mn,double Si,
											double RemStrain ,double CoolV )
#endif											
{
    CoolV=(884.0-600.0)/25.0;
	static double ca[100];//Fe-C 相图Ca线
	static double cacm[100];//Fe-C 相图ACm线
	static double wf[100];//平衡重量百分数
	static double xfc[100];//铁素体平衡体积百分数
	static double xpc[100];//珠光体平衡体积百分数
	static double temp[100];//铁素体温度
	static double tempp[100];//珠光体温度
	double delt;//时间步长
	static double xf[100];//铁素体分数
	static double xff[100];//归一化铁素体分数
	static double tf[100];//铁素体虚拟时间
	static double bf[100];//Avrami 方程参数
	double nf;//Avrami 方程参数
	static double xp[100];//珠光体分数
	static double xpp[100];//归一化珠光体分数
	static double tp[100];//珠光体虚拟时间
	static double bp[100];//Avrami 方程参数
	double np;//Avrami 方程参数
	double pf;//铁素体密度
	double pp;//珠光体密度
	int i,j,l,k;
	static double wf1[100];
	static double wf2[100];
	static double wp1[100];
	static double wp2[100];
	static double xfc1[100];
	static double xfc2[100];
	double tf1[100];
	static double xpc1[100];
	static double xpc2[100];
	static double tp1[100];
	//	double CoolV;////////////////////需传入////////////////////
	double coolp;
	static double deltxf[100];//铁素铁分数增量
	double sumf;//铁素铁分数
	double sump;//珠光体分数
	static double deltxp[100];//珠光体分数增量
	double pf1;
	static double pf2[100];
	double pf3;
	double pf4;
	double dr;
	static double ae3;
    double pp1;
	static double pp2[100];
	double pp3;
	double pp4;
	double ae1;
	static double pwf[100];
	static double pwp[100];
	static double cau[100];//奥氏体C浓度
    double tpe;//珠光体结束温度判据
	double temperature;
	double tem;

	static double Fe_Xv[100];
	static double Fe_Temp[100];
	static double P_Xv[100];
	static double P_Temp[100];
	pf=7.86;
	pp=7.81;
	nf=0.89;
	np=0.99;
	delt=1.0;
	tf[0]=0.0;
	tp[0]=0.0;
	sumf=0.0;
	sump=0.0;
	pf3=67.0;
	pf4=1.9;
	dr=dy/(1.0+0.5*RemStrain);
	pf1=2.0/(dr*(C+Mn/6.0));
	pp1=13.0/dr;
	pp3=47.0;
	pp4=2.2;
	pf2[0]=0.0;
	pp2[0]=0.0;
	///***********计算铁素体分数**********************************////
/*
	
	for(i=1;i<100;i++)
	{
		ae3=904.7899-374.25945*C+195.43806*pow(C,2.0);
		pf2[i]=ae3-215.0 + 28.15/(pow(dr,0.02)) - 0.0007*C;
		temperature=874.087+407.67788*C - 3724.46472*pow(C,2.0) + 8293.05941*pow(C,3.0) - 
			6277.1172*pow(C,4.0)-22.56*pow(CoolV,0.27);
		temp[i]=temperature-(i-1)*CoolV*delt;
		tem=temp[i];
		j=i;
		pwf[i]=pow(((temp[i]-pf2[i-1])/pf3),pf4);
		bf[i]=pf1*exp(-pwf[i]);
		xf[i]=1-exp(-bf[i]*pow((delt+tf[i-1]),nf));
		cacm[i]=-0.29729 + (4.57757/10000.0)*temp[i] + (1.37432/1000000.0)*pow(temp[i],2.0);
		cau[i]=(C-xf[i]*ca[i])/(1.0-xf[i]);
		if(cacm[i]<cau[i])
		{
			break;
		}
		ca[i]=0.04628+(1.27515/100000.0)*temp[i]-(1.695367/100000000.0)*pow(temp[i],2.0);
		wf1[i]=cacm[i]-C;
		wf2[i]=cacm[i]-ca[i];
		wf[i]=wf1[i]/wf2[i];
		xfc1[i]=wf[i]/pf;
		xfc2[i]=(1-wf[i])/pp;
		xfc[i]=xfc1[i]/(xfc1[i]+xfc2[i]);
		temp[i+1]=temperature-i*delt*CoolV;
		pwf[i+1]=pow((temp[i+1]-pf2[i])/pf3,pf4);
		bf[i+1]=pf1*exp(-pwf[i+1]);
		ca[i+1]=0.04628+(1.27515/100000.0)*temp[i+1]-(1.695367/100000000.0)*pow(temp[i+1],2.0);
		cacm[i+1]=-0.29729 + (4.57757/10000.0)*temp[i+1] + (1.37432/1000000.0)*pow(temp[i+1],2.0);
		wf1[i+1]=cacm[i+1]-C;
		wf2[i+1]=cacm[i+1]-ca[i+1];
		wf[i+1]=wf1[i+1]/wf2[i+1];
		xfc1[i+1]=wf[i+1]/pf;
		xfc2[i+1]=(1-wf[i+1])/pp;
		xfc[i+1]=xfc1[i+1]/(xfc1[i+1]+xfc2[i+1]);
		xff[i]=(xfc[i]/xfc[i+1]) * xf[i];
		tf1[i]=log(1.0/(1-xff[i]));
		tf[i]=pow((tf1[i]/bf[i+1]),(1.0/nf));
		deltxf[i]=xf[i]- xf[i-1];
		sumf=deltxf[i]+sumf;
		
		//	l_Fe_Xv[i]=sumf;
		//	l_Fe_Temp[i]=temp[i];
		
		Fe_Xv[i]=sumf;
		Fe_Temp[i]=temp[i];
		
		
	}
	
	//	l_Fe_Xv[0]=i-1;
	Fe_Temp[0]=i-1;//第0个元素保存有效数据个数
	//	Fe_Xv[i]=Fe_Temp[0];
	//	Xfe=0.0;
	//	Xfe=sumf;
	////////////////////计算珠光体//////////////////////////////////////////////////
	
	tpe=682.71-156.07*C-6.6809*Mn-9.0*pow(CoolV,0.12);//珠光体结束温度判据
	for(k=1;k<100;k++)
	{
		ae1=727.0;
		coolp=5.0;
		pp2[k]=ae1-175.0+(27.8/pow(dr,0.002))-22.0*C;
		if(fabs(CoolV-coolp)<4.5)
		{
			CoolV=coolp-(CoolV-coolp)/4.5;
		}
		tempp[k]=tem-(k-1)* CoolV*delt;
		l=k;
		pwp[k]=pow(((tempp[k]-pp2[k-1])/pp3),pp4);
		bp[k]=pp1*exp(-pwp[k]);
		xp[k]=1-exp(-bp[k]*pow((delt+tp[k-1]),np));
		if (tempp[k]<tpe)
		{
			break;
		}
		ca[k]=0.04628+(1.27515/100000.0)*temp[k]-(1.695367/100000000.0)*pow(temp[k],2.0);
		cacm[k]=-0.29729 + (4.57757/10000.0)*temp[k] + (1.37432/1000000.0)*pow(temp[k],2.0);
		wp1[k]=cacm[k]-C;
		wp2[k]=cacm[k]-ca[k];
		pwp[k]=wp1[k]/wp2[k];
		xpc1[k]=pwp[k]/pf;
		xpc2[k]=(1.0-pwp[k])/pp;
		xpc[k]=xpc1[k]/(xpc1[k]+xpc2[k]);
		tempp[k+1]=tem-(k)* CoolV*delt;
		pwp[k+1]=pow(((tempp[k+1]-pp2[k])/pp3),pp4);
		bp[k+1]=pp1*exp(-pwp[k+1]);
		ca[k+1]=0.04628+(1.27515/100000.0)*temp[k+1]-(1.695367/100000000.0)*pow(temp[k+1],2.0);
		cacm[k+1]=-0.29729 + (4.57757/10000.0)*temp[k+1] + (1.37432/1000000.0)*pow(temp[k+1],2.0);
		
		wp1[k+1]=cacm[k+1]-C;
		wp2[k+1]=cacm[k+1]-ca[k+1];
		pwp[k+1]=wp1[k+1]/wp2[k+1];
		
		xpc1[k+1]=pwp[k+1]/pf;
		xpc2[k+1]=(1.0-pwp[k+1])/pp;
		xpc[k+1]=xpc1[k+1]/(xpc1[k+1]+xpc2[k+1]);
		
		xpp[k]=((1-xpc[k])/(1-xpc[k+1]))* xp[k];
		tp1[k]=log(1.0/(1-xpp[k]));
		tp[k]=pow((tp1[k]/bp[k+1]),(1.0/np));
		deltxp[k]=xp[k]-xp[k-1];
		sump=deltxp[k]+sump;
		//	l_P_Temp[k]=tempp[k];
		//	l_P_Xv[k]=sump;
	}
	//l_P_Xv[0]=k-1;
	//l_P_Temp[0]=k-1;//第0个元素保存有效数据个数
	Xp=sump;
		   
	#ifdef  __CoolandPhrase_Write__
	#if 0
	for (i=0; i<l_P_Xv[0]; i++)
	{
	P_Xv[i]   = l_P_Xv[i]; 
	P_Temp[i] = l_P_Temp[i]; 
	}
	
	  for (i=0; i<l_Fe_Xv[0]; i++)
	  {
	  
		Fe_Xv[i]   = l_Fe_Xv[i]; 
		Fe_Temp[i] = l_Fe_Temp[i]; 
		}
		#else
		memcpy(Fe_Xv, l_Fe_Xv, sizeof(double)*100);
		memcpy(Fe_Temp, l_Fe_Temp, sizeof(double)*100);
		memcpy(P_Xv, l_P_Xv, sizeof(double)*100);
		memcpy(P_Temp, l_P_Temp, sizeof(double)*100);
		#endif
		#endif
	
	   Xb=1-Xp-Xfe;//贝氏体分数
	   double const1;
	   double Ceq;
	   double da0;
	   Ceq=C+Mn/6.0;
	   const1=pow(RemStrain,0.5);
	   if (Ceq<=0.35)
	   {
		   da0=(-0.4+6.4*Ceq)+(26.2-59.0*Ceq)*pow(CoolV,-0.5)+22.0*(1-exp(-0.015*dy));
	   }
	   else
	   {
		   da0=(23.6-57.0*Ceq)+(3.0 + 0.0*Ceq)*pow(CoolV,-0.5)+22.0*(1-exp(-0.015*dy));
	   }
	   dfe=da0*(1.0-0.45*sqrt(RemStrain));
	   
	   
	   #ifdef  __CoolandPhrase_Write__
	   return 0;
	   #endif
	   
}

//#ifndef  __CoolandPhrase_Write__
extern "C" void  PASCAL EXPORT Get_Fe_Xv(double * Fe_Xv)
{
memcpy(Fe_Xv, l_Fe_Xv, sizeof(double)*100);
}

  extern "C" void  PASCAL EXPORT Get_Fe_Temp(double * Fe_Temp)
  {
  memcpy(Fe_Temp, l_Fe_Temp, sizeof(double)*100);
  }
  
	extern "C" void  PASCAL EXPORT Get_P_Xv(double * P_Xv)
	{
	memcpy(P_Xv, l_P_Xv, sizeof(double)*100);
	}
	extern "C" void  PASCAL EXPORT Get_P_Temp(double * P_Temp)
	{
	memcpy(P_Temp, l_P_Temp, sizeof(double)*100);
	}
	//#endif											
	*/

extern "C" void PASCAL EXPORT CoolandPhrase(double dy, double C, double Mn, double Si,
											double FDT, double RemStrain , double * Fe_Xv,
											double * Fe_Temp, double *P_Temp,
											double * P_Xv, double CoolV )
										
{	
	//CoolV=(884.0-600.0)/25.0;
	static double l_Fe_Xv[1000];
	static double l_Fe_Temp[1000];

	static double l_P_Xv[1000];
	static double l_P_Temp[1000];

	static double ca[1000];//Fe-C 相图Ca线
	static double cacm[1000];//Fe-C 相图ACm线
	static double wf[1000];//平衡重量百分数
	static double xfc[1000];//铁素体平衡体积百分数
	static double xpc[1000];//珠光体平衡体积百分数
	static double temp[1000];//铁素体温度
	static double tempp[1000];//珠光体温度
	double delt;//时间步长
	static double xf[1000];//铁素体分数
	static double xff[1000];//归一化铁素体分数
	static double tf[1000];//铁素体虚拟时间
	static double bf[1000];//Avrami 方程参数
	double nf;//Avrami 方程参数
	static double xp[1000];//珠光体分数
	static double xpp[1000];//归一化珠光体分数
	static double tp[1000];//珠光体虚拟时间
	static double bp[1000];//Avrami 方程参数
	double np;//Avrami 方程参数
	double pf;//铁素体密度
	double pp;//珠光体密度
	int i,j,l,k;
	static double wf1[1000];
	static double wf2[1000];
	static double wp1[1000];
	static double wp2[1000];
	static double xfc1[1000];
	static double xfc2[1000];
	double tf1[1000];
	static double xpc1[1000];
	static double xpc2[1000];
	static double tp1[1000];
	//	double CoolV;////////////////////需传入////////////////////
	double coolp;
	static double deltxf[1000];//铁素铁分数增量
	static double sumf[1000];//铁素铁分数
	static double sump[1000];//珠光体分数
	static double deltxp[1000];//珠光体分数增量
	double pf1;
	//static double pf2[100];
	static double pf2;
	double pf3;
	double pf4;
	double dr;
	static double ae3;
    double pp1;
//	static double pp2[100];
	static double pp2;
	double pp3;
	double pp4;
	double ae1;
	static double pwf[1000];
	static double pwp[1000];
	static double cau[1000];//奥氏体C浓度
    double tpe;//珠光体结束温度判据
	double temperature;
	double tem;
	double Xaus[1000];
	double XausP[1000];
	int Sstop;

	Sstop=0;
	xf[0]=0.0;
	sumf[0]=0.0;
	xp[0]=0.0;
	sump[0]=0.0;
	Xaus[0]=1.0;
	pf=7.86;
	pp=7.81;
	nf=0.89;
	np=0.99;
	delt=0.1;
	tf[0]=0.0;
	tp[0]=0.0;
	//pf3=67.0;
	pf3=78.88;
	//pf4=1.9;
	pf4=2.46;
	dr=dy/(1.0+0.5*RemStrain);
	//pf1=2.0/(dr*(C+Mn/6.0));
	pf1=1.2/(dr*(C+Mn/6.0));
	pp1=13.0/dr;
	pp3=47.0;
	pp4=2.2;
//	pf2[0]=0.0;
	//pp2[0]=0.0;
	double Tempconst1,Tempconst2,Tempconst3,Tempconst4,Tempconst5,Tempconst6,Tempconst7;
	///***********计算铁素体分数**********************************////
	//ae3=904.7899-374.25945*C+195.43806*pow(C,2.0);
    ae3=904.7899-374.25945*C+195.43806*pow(C,2.0)-3.1635*(Mn/0.1)+2.3709*(Si/0.05);

	//pf2=ae3-215.0 + 28.15/(pow(dr,0.02)) - 0.0007*C;
    pf2=ae3-189.5 + 40.15/(pow(dr,0.02)) - 0.0007*C;
	for(i=1;i<1000;i++)
	{
	//	pf2[i]=ae3-215.0 + 28.15/(pow(dr,0.02)) - 0.0007*C;
        
		//temperature=874.087+407.67788*C - 3724.46472*pow(C,2.0) + 8293.05941*pow(C,3.0) - 
		//	6277.1172*pow(C,4.0)-22.56*pow(CoolV,0.27);
        temperature=874.087+407.67788*C - 3724.46472*pow(C,2.0) + 8293.05941*pow(C,3.0) - 
			6277.1172*pow(C,4.0)-22.56*pow(CoolV,0.27)-3.1635*(Mn/0.1);

        //if(FDT>=temperature)
		//{
		    temp[i]=temperature-(i-1)*CoolV*delt;
		//}
		//else
		//{
		//	temp[i]=FDT-(i-1)*CoolV*delt;
		///}

		tem=temp[i];
		j=i;
        //Tempconst1=(temp[i]-pf2[i-1])/pf3;
		Tempconst1=fabs((temp[i]-pf2)/pf3);
		pwf[i]=pow(Tempconst1,pf4);
		bf[i]=pf1*exp(-pwf[i]);
		Tempconst2=pow((delt+tf[i-1]),nf);
		xf[i]=1-exp(-bf[i]*Tempconst2);
		cacm[i]=-0.29729 + (4.57757/10000.0)*temp[i] + (1.37432/1000000.0)*pow(temp[i],2.0);
		cau[i]=(C-xf[i]*ca[i])/(1.0-xf[i]);
		if(cacm[i]<cau[i])
		{
			break;
		}
		ca[i]=0.04628+(1.27515/100000.0)*temp[i]-(1.695367/100000000.0)*pow(temp[i],2.0);
		wf1[i]=cacm[i]-C;
		wf2[i]=cacm[i]-ca[i];
		wf[i]=wf1[i]/wf2[i];
		xfc1[i]=wf[i]/pf;
		xfc2[i]=(1-wf[i])/pp;
		xfc[i]=xfc1[i]/(xfc1[i]+xfc2[i]);
		Xaus[i]=xfc[i]*xf[i];


		temp[i+1]=temperature-i*delt*CoolV;
		//Tempconst3=(temp[i+1]-pf2[i])/pf3;
		Tempconst3=fabs((temp[i+1]-pf2)/pf3);
		pwf[i+1]=pow(Tempconst3,pf4);
		bf[i+1]=pf1*exp(-pwf[i+1]);
		ca[i+1]=0.04628+(1.27515/100000.0)*temp[i+1]-(1.695367/100000000.0)*pow(temp[i+1],2.0);
		cacm[i+1]=-0.29729 + (4.57757/10000.0)*temp[i+1] + (1.37432/1000000.0)*pow(temp[i+1],2.0);
	
		wf1[i+1]=cacm[i+1]-C;
		wf2[i+1]=cacm[i+1]-ca[i+1];
		wf[i+1]=wf1[i+1]/wf2[i+1];
		xfc1[i+1]=wf[i+1]/pf;
		xfc2[i+1]=(1-wf[i+1])/pp;


		xfc[i+1]=xfc1[i+1]/(xfc1[i+1]+xfc2[i+1]);
		xff[i]=(xfc[i]/xfc[i+1]) * xf[i];
		tf1[i]=log(1.0/(1-xff[i]));
		tf[i]=pow((tf1[i]/bf[i+1]),(1.0/nf));
		deltxf[i]=xf[i]- xf[i-1];
		sumf[i]=deltxf[i]+sumf[i-1];
		l_Fe_Xv[i]=sumf[i];
		l_Fe_Temp[i]=temp[i];

	}


	l_Fe_Xv[0]=i-1;
	l_Fe_Temp[0]=i-1;//第0个元素保存有效数据个数
	Xfe=sumf[i-1];
	
//	Fe_Xv=l_Fe_Xv;
//	Fe_Temp=l_Fe_Temp;
	////////////////////计算珠光体//////////////////////////////////////////////////
	
	//tpe=682.71-156.07*C-6.6809*Mn-9.0*pow(CoolV,0.12);//珠光体结束温度判据
    tpe=l_Fe_Temp[i-1]-156.07*C-6.6809*Mn-9.0*pow(CoolV,0.12);//珠光体结束温度判据
	/*ae1=727.0;
	Tempconst4=pow(dr,0.02);
	pp2=ae1-175.0+(27.8/Tempconst4)-22.0*C;
	for(k=1;k<1000;k++)
	{
		//pp2[k]=ae1-175.0+(27.8/Tempconst4)-22.0*C;
		//if(fabs(CoolV-coolp)<4.5)
		//{
		//	CoolV=coolp-(CoolV-coolp)/4.5;
		//}
		tempp[k]=l_Fe_Temp[i-1]-(k-1)* CoolV*delt;///////////???????????????????????????????????????
		l=k;
		//Tempconst5=(tempp[k]-pp2[k-1])/pp3;
		Tempconst5=fabs((tempp[k]-pp2)/pp3);
		pwp[k]=pow(Tempconst5,pp4);
		bp[k]=pp1*exp(-pwp[k]);
		Tempconst6=pow((delt+tp[k-1]),np);
		xp[k]=1-exp(-bp[k]*Tempconst6);
		ca[k]=0.04628+(1.27515/100000.0)*temp[k]-(1.695367/100000000.0)*pow(temp[k],2.0);
		cacm[k]=-0.29729 + (4.57757/10000.0)*temp[k] + (1.37432/1000000.0)*pow(temp[k],2.0);
		wp1[k]=cacm[k]-C;
		wp2[k]=cacm[k]-ca[k];
		pwp[k]=wp1[k]/wp2[k];
		xpc1[k]=pwp[k]/pf;
		xpc2[k]=(1.0-pwp[k])/pp;
		xpc[k]=xpc1[k]/(xpc1[k]+xpc2[k]);
        XausP[k]=(1.0+xpc[k])*xp[k];
        
		tempp[k+1]=tem-k*CoolV*delt;
		//Tempconst7=(tempp[k+1]-pp2[k])/pp3;
		Tempconst7=fabs(tempp[k+1]-pp2)/pp3;
		Tempconst7=fabs(Tempconst7);
		pwp[k+1]=pow(Tempconst7,pp4);
		bp[k+1]=pp1*exp(-pwp[k+1]);
		ca[k+1]=0.04628+(1.27515/100000.0)*temp[k+1]-(1.695367/100000000.0)*pow(temp[k+1],2.0);
		cacm[k+1]=-0.29729 + (4.57757/10000.0)*temp[k+1] + (1.37432/1000000.0)*pow(temp[k+1],2.0);
		
		wp1[k+1]=cacm[k+1]-C;
		wp2[k+1]=cacm[k+1]-ca[k+1];
		pwp[k+1]=wp1[k+1]/wp2[k+1];
		
		xpc1[k+1]=pwp[k+1]/pf;
		xpc2[k+1]=(1.0-pwp[k+1])/pp;
		xpc[k+1]=xpc1[k+1]/(xpc1[k+1]+xpc2[k+1]);
		
		xpp[k]=((1-xpc[k])/(1-xpc[k+1]))* xp[k];
		tp1[k]=log(1.0/(1-xpp[k]));
		tp[k]=pow((tp1[k]/bp[k+1]),(1.0/np));
		deltxp[k]=xp[k]-xp[k-1];
		sump[k]=deltxp[k]+sump[k-1];
		l_P_Temp[k]=tempp[k];
		l_P_Xv[k]=sump[k];
		if ((l_Fe_Xv[i-1]+sump[k])>=1.0)//(tempp[k]<tpe)
		{
			break;
		}
		if(tempp[k]<tpe)break;
	}


	l_P_Xv[0]=k-1;
	l_P_Temp[0]=k-1;//第0个元素保存有效数据个数
	
     Xp=sump[k-1];*/
	l_P_Xv[0]=1;
	l_P_Temp[0]=1;
	l_P_Xv[1]=1.0-Xfe;
	l_P_Temp[1]=tpe;

	
	   memcpy(Fe_Xv, l_Fe_Xv, sizeof(double)*1000);
	   memcpy(Fe_Temp, l_Fe_Temp, sizeof(double)*1000);
	   memcpy(P_Xv, l_P_Xv, sizeof(double)*1000);
	   memcpy(P_Temp, l_P_Temp, sizeof(double)*1000);
   	   Xb=1-Xp-Xfe;//贝氏体分数
	   double const1;
	   double Ceq;
	   double da0;
	   Ceq=C+Mn/6.0;
	   const1=pow(RemStrain,0.5);
	   if (Ceq<=0.35)
	   {
		   //da0=(-0.4+6.4*Ceq)+(22.2-59.0*Ceq)*pow(CoolV,-0.5)+22.0*(1-exp(-0.015*dy));
		   da0=(-0.4+6.4*Ceq)+(24.2-59.0*Ceq)*pow(CoolV,-0.5)+22.0*(1-exp(-0.015*dy));
	   }
	   else
	   {
		   //da0=(19.6-57.0*Ceq)+(3.0 + 0.0*Ceq)*pow(CoolV,-0.5)+22.0*(1-exp(-0.015*dy));
		   da0=(22.6-97.0*Ceq)+(3.0 + 0.0*Ceq)*pow(CoolV,-0.5)+22.0*(1-exp(-0.015*dy));
	   }
	   dfe=da0*(1.0-0.45*sqrt(RemStrain));

}

extern "C" void PASCAL EXPORT PhaseFraction(double dy,double C,	double Mn,double Si,
											double RemStrain ,double * Fe_Xv,double * Fe_Temp,
											double *P_Temp,double * P_Xv,double CoolV,
											double * pCoolV, double * pTemperature, 
											double *pBankTime, int iSegment)
										
{	
	//CoolV=(884.0-600.0)/25.0;
	static double l_Fe_Xv[1000];
	static double l_Fe_Temp[1000];

	static double l_P_Xv[1000];
	static double l_P_Temp[1000];

	static double ca[1000];//Fe-C 相图Ca线
	static double cacm[1000];//Fe-C 相图ACm线
	static double wf[1000];//平衡重量百分数
	static double xfc[1000];//铁素体平衡体积百分数
	static double xpc[1000];//珠光体平衡体积百分数
	static double temp[1000];//铁素体温度
	static double tempp[1000];//珠光体温度
	double delt;//时间步长
	static double xf[1000];//铁素体分数
	static double xff[1000];//归一化铁素体分数
	static double tf[1000];//铁素体虚拟时间
	static double bf[1000];//Avrami 方程参数
	double nf;//Avrami 方程参数
	static double xp[1000];//珠光体分数
	static double xpp[1000];//归一化珠光体分数
	static double tp[1000];//珠光体虚拟时间
	static double bp[1000];//Avrami 方程参数
	double np;//Avrami 方程参数
	double pf;//铁素体密度
	double pp;//珠光体密度
	int i,j,l,k,s;
	static double wf1[1000];
	static double wf2[1000];
	static double wp1[1000];
	static double wp2[1000];
	static double xfc1[1000];
	static double xfc2[1000];
	double tf1[1000];
	static double xpc1[1000];
	static double xpc2[1000];
	static double tp1[1000];
	//	double CoolV;////////////////////需传入////////////////////
	double coolp;
	static double deltxf[1000];//铁素铁分数增量
	static double sumf[1000];//铁素铁分数
	static double sump[1000];//珠光体分数
	static double deltxp[1000];//珠光体分数增量
	double pf1;
	//static double pf2[100];
	static double pf2;
	double pf3;
	double pf4;
	double dr;
	static double ae3;
    double pp1;
//	static double pp2[100];
	static double pp2;
	double pp3;
	double pp4;
	double ae1;
	static double pwf[1000];
	static double pwp[1000];
	static double cau[1000];//奥氏体C浓度
    double tpe;//珠光体结束温度判据
	double temperature;
	double tem;
	
	pf=7.86;
	pp=7.81;
	nf=0.89;
	np=0.99;
	delt=0.1;
	tf[0]=0.0;
	tp[0]=0.0;
	//pf3=67.0;
	pf3=78.88;
	//pf4=1.9;
	pf4=2.46;
	dr=dy/(1.0+0.5*RemStrain);
	//pf1=2.0/(dr*(C+Mn/6.0));
	pf1=1.2/(dr*(C+Mn/6.0));
	pp1=13.0/dr;
	pp3=47.0;
	pp4=2.2;
	xf[0]=0.0;
	sumf[0]=0.0;
//	pf2[0]=0.0;
	//pp2[0]=0.0;
	double Tempconst1,Tempconst2,Tempconst3,Tempconst4,Tempconst5,Tempconst6,Tempconst7;
	///***********计算铁素体分数**********************************////
	ae3=904.7899-374.25945*C+195.43806*pow(C,2.0);
	//pf2=ae3-215.0 + 28.15/(pow(dr,0.02)) - 0.0007*C;
    pf2=ae3-189.5 + 40.15/(pow(dr,0.02)) - 0.0007*C;
	
    for(s=1;s<=15;s++)
	{
		temperature=874.087+407.67788*C - 3724.46472*pow(C,2.0) + 8293.05941*pow(C,3.0) - 
			6277.1172*pow(C,4.0)-22.56*pow(pCoolV[(iSegment-1)*15+s],0.27);
	    if(pTemperature[2*s+1]<=temperature)break;
	}

	for(i=1;i<1000;i++)
	{
	//	pf2[i]=ae3-215.0 + 28.15/(pow(dr,0.02)) - 0.0007*C;
        
		for(s=1;s<=15;s++)
		{
			if(pBankTime[(iSegment-1)*32+2*s+1]>=(i-1)*delt)break;
		}
		if(i==1)
		{
			temp[1]=temperature;
		}
		else
		{
		    temp[i]=temp[i-1]-pCoolV[(iSegment-1)*15+s]*delt;
		}
		tem=temp[i];
		j=i;
        //Tempconst1=(temp[i]-pf2[i-1])/pf3;
		Tempconst1=fabs((temp[i]-pf2)/pf3);
		pwf[i]=pow(Tempconst1,pf4);
		bf[i]=pf1*exp(-pwf[i]);
		Tempconst2=pow((delt+tf[i-1]),nf);
		xf[i]=1-exp(-bf[i]*Tempconst2);
		cacm[i]=-0.29729 + (4.57757/10000.0)*temp[i] + (1.37432/1000000.0)*pow(temp[i],2.0);
		cau[i]=(C-xf[i]*ca[i])/(1.0-xf[i]);
		if(cacm[i]<cau[i])
		{
			break;
		}
		ca[i]=0.04628+(1.27515/100000.0)*temp[i]-(1.695367/100000000.0)*pow(temp[i],2.0);
		wf1[i]=cacm[i]-C;
		wf2[i]=cacm[i]-ca[i];
		wf[i]=wf1[i]/wf2[i];
		xfc1[i]=wf[i]/pf;
		xfc2[i]=(1-wf[i])/pp;
		xfc[i]=xfc1[i]/(xfc1[i]+xfc2[i]);
		temp[i+1]=temp[i]-delt*pCoolV[(iSegment-1)*15+s];
		//Tempconst3=(temp[i+1]-pf2[i])/pf3;
		Tempconst3=fabs((temp[i+1]-pf2)/pf3);
		pwf[i+1]=pow(Tempconst3,pf4);
		bf[i+1]=pf1*exp(-pwf[i+1]);
		ca[i+1]=0.04628+(1.27515/100000.0)*temp[i+1]-(1.695367/100000000.0)*pow(temp[i+1],2.0);
		cacm[i+1]=-0.29729 + (4.57757/10000.0)*temp[i+1] + (1.37432/1000000.0)*pow(temp[i+1],2.0);
	
		wf1[i+1]=cacm[i+1]-C;
		wf2[i+1]=cacm[i+1]-ca[i+1];
		wf[i+1]=wf1[i+1]/wf2[i+1];
		xfc1[i+1]=wf[i+1]/pf;
		xfc2[i+1]=(1-wf[i+1])/pp;

		xfc[i+1]=xfc1[i+1]/(xfc1[i+1]+xfc2[i+1]);
		xff[i]=(xfc[i]/xfc[i+1]) * xf[i];
		tf1[i]=log(1.0/(1-xff[i]));
		tf[i]=pow((tf1[i]/bf[i+1]),(1.0/nf));
		deltxf[i]=xf[i]- xf[i-1];
		sumf[i]=deltxf[i]+sumf[i-1];
		l_Fe_Xv[i]=sumf[i];
		l_Fe_Temp[i]=temp[i];
	}

	l_Fe_Xv[0]=i-1;
	l_Fe_Temp[0]=i-1;//第0个元素保存有效数据个数
	Xfe=sumf[i-1];
	
//	Fe_Xv=l_Fe_Xv;
//	Fe_Temp=l_Fe_Temp;
	////////////////////计算珠光体//////////////////////////////////////////////////
	
	//tpe=682.71-156.07*C-6.6809*Mn-9.0*pow(CoolV,0.12);//珠光体结束温度判据
    l_P_Temp[0]=1;
	l_P_Temp[1]=l_Fe_Temp[i-1]-156.07*C-6.6809*Mn-9.0*pow(CoolV,0.12);//珠光体结束温度判据
	
	//ae1=727.0;
	//coolp=5.0;
	//Tempconst4=pow(dr,0.02);
	//pp2=ae1-175.0+(27.8/Tempconst4)-22.0*C;
	/*for(k=1;k<100;k++)
	{
		//pp2[k]=ae1-175.0+(27.8/Tempconst4)-22.0*C;
		if(fabs(CoolV-coolp)<4.5)
		{
			CoolV=coolp-(CoolV-coolp)/4.5;
		}
		tempp[k]=tem-(k-1)* CoolV*delt;///////////???????????????????????????????????????
		l=k;
		//Tempconst5=(tempp[k]-pp2[k-1])/pp3;
		Tempconst5=fabs((tempp[k]-pp2)/pp3);
		pwp[k]=pow(Tempconst5,pp4);
		bp[k]=pp1*exp(-pwp[k]);
		Tempconst6=pow((delt+tp[k-1]),np);
		xp[k]=1-exp(-bp[k]*Tempconst6);
		if (tempp[k]<tpe)
		{
			break;
		}
		ca[k]=0.04628+(1.27515/100000.0)*temp[k]-(1.695367/100000000.0)*pow(temp[k],2.0);
		cacm[k]=-0.29729 + (4.57757/10000.0)*temp[k] + (1.37432/1000000.0)*pow(temp[k],2.0);
		wp1[k]=cacm[k]-C;
		wp2[k]=cacm[k]-ca[k];
		pwp[k]=wp1[k]/wp2[k];
		xpc1[k]=pwp[k]/pf;
		xpc2[k]=(1.0-pwp[k])/pp;
		xpc[k]=xpc1[k]/(xpc1[k]+xpc2[k]);

		tempp[k+1]=tem-k*CoolV*delt;
		//Tempconst7=(tempp[k+1]-pp2[k])/pp3;
		Tempconst7=(tempp[k+1]-pp2)/pp3;
		Tempconst7=fabs(Tempconst7);
		pwp[k+1]=pow(Tempconst7,pp4);
		bp[k+1]=pp1*exp(-pwp[k+1]);
		ca[k+1]=0.04628+(1.27515/100000.0)*temp[k+1]-(1.695367/100000000.0)*pow(temp[k+1],2.0);
		cacm[k+1]=-0.29729 + (4.57757/10000.0)*temp[k+1] + (1.37432/1000000.0)*pow(temp[k+1],2.0);
		
		wp1[k+1]=cacm[k+1]-C;
		wp2[k+1]=cacm[k+1]-ca[k+1];
		pwp[k+1]=wp1[k+1]/wp2[k+1];
		
		xpc1[k+1]=pwp[k+1]/pf;
		xpc2[k+1]=(1.0-pwp[k+1])/pp;
		xpc[k+1]=xpc1[k+1]/(xpc1[k+1]+xpc2[k+1]);
		
		xpp[k]=((1-xpc[k])/(1-xpc[k+1]))* xp[k];
		tp1[k]=log(1.0/(1-xpp[k]));
		tp[k]=pow((tp1[k]/bp[k+1]),(1.0/np));
		deltxp[k]=xp[k]-xp[k-1];
		sump[k]=deltxp[k]+sump[k-1];
		l_P_Temp[k]=tempp[k];
		l_P_Xv[k]=sump[k];
	}*/
	//l_P_Xv[0]=k-1;
	//l_P_Temp[0]=k-1;//第0个元素保存有效数据个数
	//if(l_P_Temp[0]==0)l_P_Temp[1]=l_Fe_Temp[i-1]-30.0;   //Modified at 02.08.15
 //	P_Xv=l_P_Xv;
//	P_Temp=l_P_Temp;
	//Xp=sump[k-1];

	//////////6 ,9
//	l_P_Xv[k]=1-Xfe;
/*	   
#if 0
	for (i=0; i<l_P_Xv[0]; i++)
	{
		P_Xv[i]   = l_P_Xv[i]; 
		P_Temp[i] = l_P_Temp[i]; 
	}

	for (i=0; i<l_Fe_Xv[0]; i++)
	{
		
		Fe_Xv[i]   = l_Fe_Xv[i]; 
		Fe_Temp[i] = l_Fe_Temp[i]; 
	}
#else
	memcpy(Fe_Xv, l_Fe_Xv, sizeof(double)*100);
	memcpy(Fe_Temp, l_Fe_Temp, sizeof(double)*100);
	memcpy(P_Xv, l_P_Xv, sizeof(double)*100);
	memcpy(P_Temp, l_P_Temp, sizeof(double)*100);
#endif
*/
	   memcpy(Fe_Xv, l_Fe_Xv, sizeof(double)*1000);
	   memcpy(Fe_Temp, l_Fe_Temp, sizeof(double)*1000);
	   memcpy(P_Xv, l_P_Xv, sizeof(double)*1000);
	   memcpy(P_Temp, l_P_Temp, sizeof(double)*1000);
   	   Xb=1-Xp-Xfe;//贝氏体分数
	   double const1;
	   double Ceq;
	   double da0;
	   Ceq=C+Mn/6.0;
	   const1=pow(RemStrain,0.5);
	   if (Ceq<=0.35)
	   {
		   da0=(-0.4+6.4*Ceq)+(22.2-59.0*Ceq)*pow(CoolV,-0.5)+22.0*(1-exp(-0.015*dy));
	   }
	   else
	   {
		   da0=(19.6-57.0*Ceq)+(3.0 + 0.0*Ceq)*pow(CoolV,-0.5)+22.0*(1-exp(-0.015*dy));
	   }
	   dfe=da0*(1.0-0.45*sqrt(RemStrain));

}

extern "C" double PASCAL EXPORT CoolXfe()
{
	return Xfe;
}

extern "C" double PASCAL EXPORT CoolXp()
{
	return 1-Xfe;
}
extern "C" double PASCAL EXPORT FedCal(double C, double Mn, double dy, double RemStrain,
									   double CoolV)
{
	double Ceq;
	double Da0;
	double DFe;

	Ceq=C+Mn/6.0;
	if(Ceq<=0.35)
	{
		Da0=(-0.4+6.4*Ceq)+(38.2-59.0*Ceq)*pow(CoolV,-0.5)+22.0*(1-exp(-0.015*dy));
	}
	else
	{
		Da0=(8.694-5.70*Ceq)+(3.0 + 0.0*Ceq)*pow(CoolV,-0.5)+22.0*(1-exp(-0.015*dy));
	}
	DFe=Da0*(1.0-0.45*sqrt(RemStrain));
	return DFe;
}



//计算冷却之后的晶粒长大尺寸—卷曲过程


extern "C" double PASCAL EXPORT ConvertSeg(int NSegTal,int NSegCur,double Length,
										   double Height)
{
	const double r=0.381; //钢卷内径
	//计算钢卷外径
	double R;//钢卷外径
    double SArea;//
	double Position ;
	double LengthCur;
	double b;
	const double Pi=3.1415926;
	
	SArea = Length * Height;
	R = sqrt( SArea / Pi + pow(r,2.0) );
	LengthCur = NSegCur * Length / NSegTal;
	b = 2.0 * r - Height;
	Position = -b + sqrt( pow(b,2.0) + 4.0 * Height * LengthCur / Pi);
	Position = Position/ ( (R-r)*2.0 );
	return Position;
}


extern "C" double PASCAL EXPORT CalcCurlDSize(double Temp,double dSize,
						 int  NSegTal,int NSegCur,double Length,double Height)
{
	
	double	DistanceCur[23]={
		0.0,   2.0,   4.0,   7.0 ,  9.0 , 
			12.0,  14.0,  17.0,  19.0,  22.0, 
			24.0,  27.0,  29.0,  32.0,  34.0, 
			37.0,  39.0,  42.0,  44.0,  47.0, 
			49.0,  52.0,  55.0	};	
		
		double Time[23][6]=  {
			{0.0,  43.0,  92.0,  186.0,  324.0,  536.0},  
				
			{0.0,  424.0,  819.0,  1182.0,  1673.0,  2271.0},  
			
			{29.0,  1182.0,  1956.0,  2617.0,  3422.0,  4650.0},  
			
			{159.0,  2789.0,  4142.0,  5567.0,  6960.0,  8601.0},  
			
			{288.0,  4142.0,  5872.0,  7749.0,  9527.0,  11527.0},  
			
			{600.0,  6235.0,  8601.0,  11027.0,  13027.0,  15027.0},  
			
			{741.0,  7749.0,  10527.0,  12527.0,  15027.0,  17527.0},  
			
			{896.0,  9527.0,  12527.0,  15027.0,  17527.0,  20527.0},  
			
			{896.0,  10527.0,  13527.0,  16527.0,  19527.0,  22027.0},  
			
			{896.0,  11527.0,  15027.0,  18027.0,  21027.0,  23527.0},  
			
			{896.0,  12027.0,  15527.0,  18527.0,  21527.0,  24027.0},  
			
			{896.0,  12027.0,  15527.0,  19027.0,  21527.0,  24027.0},  
			
			{896.0,  12027.0,  15527.0,  18527.0,  21027.0,  24027.0},  
			
			{896.0,  11027.0,  14527.0,  17527.0,  20027.0,  22527.0},  
			
			{896.0,  10527.0,  13527.0,  16527.0,  19027.0,  21527.0},  
			
			{896.0,  9027.0,  12027.0,  14527.0,  16527.0,  19027.0},  
			
			{896.0,  8175.0,  10527.0,  13027.0,  15027.0,  17027.0}, 
			
			{663.0,  6235.0,  8601.0,   10527.0,  12027.0,  14027.0},
			
			{473.0,  4956.0,  6960.0,   8601.0,  10027.0,  12027.0},
			
			{213.0,  3000.0,  4396.0,  5872.0,  6960.0,  8601.0},
			
			{112.0,  1956.0,  3000.0,  4142.0,  4956.0,  6235.0},
			
			{0.0, 741.0,  1182.0,  1673.0,  2271.0,  3000.0},
			
			{0.0, 43.0,  92.0, 186.0, 324.0, 536.0}
		};
		
		double Position;	
		
		//将时间插值
		double TimeCur[6];
		double k;
		int N;
		int i , j ;
		
		i = 0 ;
		
		for (i=0;  i<23;  i++)
		{
			DistanceCur[i] = DistanceCur[i] / 55.0;
		}
		
		//将段数转化成相应的位置数
		Position=ConvertSeg(NSegTal,NSegCur,Length,Height);
		
		if (Position>1.0)	{  Position=1.0;  }
		
		for (i = 0  ; i < 23 ; i++ )
		{
			if ( Position <= DistanceCur[i] )
			{
				break ;
			}
		}
		
		N = i;
		
		if ( i >= 23 )
		{
			N = i - 1;
		}
		
		if( fabs( Position-DistanceCur[N]) < (1.0e-4) )
		{
			for (j=0;j<6;j++)
			{
				TimeCur[j]=Time[N][j];
			}
		}
		else
		{
			for(j=0;j<6;j++)
			{
				k=(Time[N][j]-Time[N-1][j])/(DistanceCur[N]-DistanceCur[N-1]);
				TimeCur[j]=k*(Position-DistanceCur[N-1])+Time[N-1][j];
			}
		}
		
		//计算晶粒度		
		
		//	double DCurlSize;
		DCurlSize=dSize;
		Temp=Temp+273;
		double const1,const2;
		const double q=3.5;
		const double R=8.314;
		const double Qgg=160.0*(1.0e3);
		const double kk=6.0*(1.0e8);
		i=0;
		for (i=0;i<6;i++)
		{
			if (i>0)
			    const2=kk*(TimeCur[i]-TimeCur[i-1])*exp(-Qgg/(R*Temp));
			else
				const2=kk*TimeCur[i]*exp(-Qgg/(R*Temp));
			const1=pow(DCurlSize,q)+const2;
			DCurlSize=pow(const1,1.0/3.5);
			Temp-=20.0;
		}
		return DCurlSize;
}

// 计算卷取后应力松驰时间
extern "C" double PASCAL EXPORT CalCurTime(int NSegTal,int NSegCur,
						double Length, double Height)
{
	double	DistanceCur[23]={
		     0.0,   2.0,   4.0,   7.0 ,  9.0 , 
			12.0,  14.0,  17.0,  19.0,  22.0, 
			24.0,  27.0,  29.0,  32.0,  34.0, 
			37.0,  39.0,  42.0,  44.0,  47.0, 
	     	49.0,  52.0,  55.0	};	
		
	double Time[23]={ 536.0,  2271.0,  4650.0,  8601.0,  11527.0,  15027.0,  
			17527.0,  20527.0,  22027.0,  23527.0,  24027.0,  24027.0,  24027.0,  
			22527.0,  21527.0,  19027.0,  17027.0, 14027.0,  12027.0,  8601.0,
			6235.0,  3000.0,  536.0};
		
		double Position;
		double DCurTime;
		double k;
		int M;
		int i;		
		for (i=0;  i<23;  i++)
		{
			DistanceCur[i] = DistanceCur[i] / 55.0;
		}		
		
		Position=ConvertSeg(NSegTal,NSegCur,Length,Height);
		
		if (Position>1.0)
		{  Position=1.0;  }
		
		for (i = 0  ; i < 23 ; i++ )
		{
			if ( Position <= DistanceCur[i] )
			{
				break ;
			}
		}		
		M = i;		
		if ( i >= 23 )
		{
			M = i - 1;
		}	   
		    if( fabs( Position-DistanceCur[M]) < (1.0e-4) )					    
			{
				DCurTime=Time[M];
			}		
		    else
			{			    
				    k=(Time[M]-Time[M-1])/(DistanceCur[M]-DistanceCur[M-1]);
				    DCurTime=k*(Position-DistanceCur[M-1])+Time[M-1];
			}
			return DCurTime;
}

// 计算组织性能
/*
extern "C" void PASCAL EXPORT PropertyCal(double Si,double Vf,
										  double Vp,double Vb,double dF)
{
	double Hf;
	double Hp;
	double Hb;
	double Tmf;
	double Tmb;
	Tmf=695.0;
	Tmb=577.0;
	Hf=361.0-0.357*Tmf+50.0*Si;
	Hp=175.0;
	Hb=508.0-0.588*Tmb+50.0*Si;
	Tb=200.0+60.0*Vf+118.0*(Vp+Vb)+19.9*pow(dF/1000.0,-1.0/2.0);
    Ts=97.0+64.90*Vf+15.1*(Vp+Vb)+17.25*pow(dF/1000.0,-1.0/2.0);
	LenRatio=11.0+3.0*Vf+1.53*Vp-0.03*Vb+1.8*pow(dF/1000.0,-1.0/2.0);
	
	
}
*/
//Modified on 2002-6-25
extern "C" void PASCAL EXPORT PropertyCal(double Si, double Mn ,double Vf,double Vp,
										     double dF,double h,double S, double P,
											 double FT7, double CT)
{
	double Tbmax;
	double Tbmin;
	double Tsmax;
	double Tsmin;
	double LenRatiomax;
	double LenRatiomin;

	//力学性能范围
	Tbmax=650.0;
	Tbmin=200.0;
	Tsmax=500.0;
	Tsmin=100.0;
	LenRatiomax=51.0;
	LenRatiomin=10.0;

	h=h*1000.0;                //将厚度转化为mm
	Tb=376.0 + 55.0*Si +52.0*Mn+393.0*P-313.0*S+
		3.7*pow(dF/1000.0,-1.0/2.0) + 40.0*(1.0-Vf);
	Ts=180.0 + 75.0*Si + 84.0*Mn +5.0*P-500.0*S+ 
		9.7*pow(dF/1000.0,-1.0/2.0) + 28.0*(1.0-Vf);
	LenRatio=57.82- 30.9*Si -20.2*Mn +30.0*P-108.0*S
		+0.8*log(h)+0.17*pow(dF/1000.0,-1.0/2.0)-6.0*Vf-1.86*log(Ts);
	/*修改屈服强度和延伸率*/
	/*Tb=1190.4+ 93.6*Si +8.1*Mn+242.7*P+49.1*S+
		1.1*pow(dF/1000.0,-1.0/2.0) -20.3*Vf-306.1*log(FT7)+144.5*log(CT)+
		70.3*log(FT7-CT);
	Ts=-420.4 + 54.7*Si + 14.4*Mn +280.5*P + 99.5*S+ 
		10.9*pow(dF/1000.0,-1.0/2.0) -89.0*Vf+181.6*log(FT7)-43.4*log(CT)-
		46.5*log(FT7-CT);
	LenRatio=-8528.2- 208.4*Si -13.1*Mn -382.1*P-10.7*S
		-5.0*log(h)+7.2*pow(dF/1000.0,-1.0/2.0)-16.2*Vf +1021.7*log(FT7) -
		448.0*log(CT)-239.7*log(FT7-CT)-291.0*log(Ts)+1231.5*log(Tb);*/
    //抗拉强度范围200.0-650.0MPa
	Tb=1006.81+ 72.85*Si +12.86*Mn+337.96*P+0.44*S+
		4.17*pow(dF/1000.0,-1.0/2.0)-31.11*Vf-112.28*log(FT7)+16.15*log(CT)
		+12.65*log(FT7-CT);
	//屈服强度范围100.0-500.0MPa
	Ts=12826.2 + 10.33*Si + 17.98*Mn +222.44*P + 516.91*S+ 
		13.28*pow(dF/1000.0,-1.0/2.0) -114.63*Vf-398.80*log(FT7)-3167.36*log(CT)
		+270.71*log(CT)*log(CT)-348.49*log(FT7-CT)+41.26*log(FT7-CT)*log(FT7-CT)
        -13.126*log(h)+3.975*log(h)*log(h);
	//延伸率10.0-51.0%
	LenRatio=-1779.92-0.57*Si -2.89*Mn -33.59*P+6.87*S
		-9.77*log(h)+2.01*log(h)*log(h)+2.04*pow(dF/1000.0,-1.0/2.0)+13.79*Vf 
		+99.89*log(FT7) +386.81*log(CT)-34.17*log(CT)*log(CT)+40.15*log(FT7-CT)
		-5.82*log(FT7-CT)*log(FT7-CT);

	if(Tb<Tbmin)
	{
		Tb=Tbmin;
	}
	if(Tb>Tbmax)
	{
		Tb=Tbmax;
	}
    if(Ts<Tsmin)
	{
		Ts=Tsmin;
	}
	if(Ts>Tsmax)
	{
		Ts=Tsmax;
	}
	if(LenRatio<LenRatiomin)
	{
		LenRatio=LenRatiomin;
	}
	if(LenRatio>LenRatiomax)
	{
		LenRatio=LenRatiomax;
	}
}	
extern "C" double PASCAL EXPORT TsCal()
{
	return Ts;
}
extern "C" double PASCAL EXPORT TbCal()
{
	return Tb;
}
extern "C" double PASCAL EXPORT LenRatioCal()
{
	return LenRatio;
}

extern "C" double PASCAL EXPORT LengthCal(double Hpre,
										  double Widepre,double Lengthpre,double Hcur,double Widecur)
{
	double Length;
	Length=Hpre*Widepre*Lengthpre/(Hcur*Widecur);
	return Length;
}


extern "C" int PASCAL EXPORT 
CalcCoolCurve(int    m_mode/*冷却方式:*/ , 
			   double m_FDT /*终轧温度:*/ , 
			   double m_CT  /*卷曲温度*/ ,
			   double * pXTime, /*回带时间结果数组*/
			   double * pYTemp, /*回带温度结果数组*/
			   int   arySize /*回带结果数组的大小*/) 
{
	// TODO: Add your command handler code here
	int i,j,k,m;
	int Yubn[12][2];
	int Isdflg[16];
	// Isused1[j][k][m]前12个水箱，Isused2[j][k][m],13,14,15个水箱.
	int Isused1[12][2][6];
	int Isused2[3][2][12];
	//**************************** 输出数据
	int Upperbnk[15];
	int Lowerbnk[15];
	// 时间数组
    double Time[16][2];
	// 温度数组
	double Temperature[16][2];
	//输出数据****************************// 
	double DTwbnk[15];
	double Posbnk[16][2];
	int Icase;
	double DTbnk0=12.0;
	double DTsd0=2.0;
	double DTsd;
	double DTlast;
	double DTair;
	double DTW;
	double m_T;
	
	//Upper=0;
	//Lower=0;
	DTW=0.0;
	Icase=0;
	DTair=40.2;
	m_T=m_FDT;
	
	for(i=0;i<16;i++)
	{
		Isdflg[i]=0;
	}
	
	for(i=0;i<15;i++)
	{
		Upperbnk[i]=0;
		Lowerbnk[i]=0;
	}
	
	for(i=0;i<12;i++)
	{
		for(j=0;j<2;j++)
		{
			for(m=0;m<6;m++)
			{
				Isused1[i][j][m]=0;
			}
		}
	}
	
	for(i=0;i<3;i++)
	{
		for(j=0;j<2;j++)
		{
			for(m=0;m<12;m++)
			{
				Isused2[i][j][m]=0;
			}
		}
	}
	
	for(i=0;i<12;i++)
	{
		for(j=0;j<2;j++)
		{
			Yubn[i][j]=0;
		}
	}
	
	for(i=0;i<15;i++)
	{
		DTwbnk[i]=0.0;
	}
	
	for(i=0;i<16;i++)
	{
		if(i==0)
		{
			Posbnk[i][0]=0.0;
			Posbnk[i][1]=0.0;
		}
		Posbnk[i][0]=1.4+(i-1)*5.6;
		Posbnk[i][1]=Posbnk[i][0]+4.4;
	}
	
	if(m_mode==1)
	{
		Yubn[0][0]=1;
		Yubn[0][1]=2;
		Yubn[1][0]=3;
		Yubn[1][1]=4;
		Yubn[2][0]=5;
		Yubn[2][1]=6;
		Yubn[3][0]=7;
		Yubn[3][1]=8;
		Yubn[4][0]=9;
		Yubn[4][1]=10;
		Yubn[5][0]=11;
		Yubn[5][1]=12;
        Yubn[6][0]=13;
		Yubn[6][1]=14;
		Yubn[7][0]=15;
		Yubn[7][1]=16;
		Yubn[8][0]=17;
		Yubn[8][1]=18;
		Yubn[9][0]=19;
		Yubn[9][1]=20;
		Yubn[10][0]=21;
		Yubn[10][1]=22;
		Yubn[11][0]=23;
		Yubn[11][1]=24;	
	}
	else if(m_mode==3)
	{
		Yubn[0][0]=23;
		Yubn[0][1]=24;
		Yubn[1][0]=21;
		Yubn[1][1]=22;
		Yubn[2][0]=19;
		Yubn[2][1]=20;
		Yubn[3][0]=17;
		Yubn[3][1]=18;
		Yubn[4][0]=15;
		Yubn[4][1]=16;
		Yubn[5][0]=13;
		Yubn[5][1]=14;
        Yubn[6][0]=1;
		Yubn[6][1]=2;
		Yubn[7][0]=3;
		Yubn[7][1]=4;
		Yubn[8][0]=5;
		Yubn[8][1]=6;
		Yubn[9][0]=7;
		Yubn[9][1]=8;
		Yubn[10][0]=9;
		Yubn[10][1]=10;
		Yubn[11][0]=11;
		Yubn[11][1]=12;
	}
	else if(m_mode==2)
	{
        Yubn[0][0]=1;
		Yubn[0][1]=2;
		Yubn[1][0]=13;
		Yubn[1][1]=14;
		Yubn[2][0]=3;
		Yubn[2][1]=4;
		Yubn[3][0]=15;
		Yubn[3][1]=16;
		Yubn[4][0]=5;
		Yubn[4][1]=6;
		Yubn[5][0]=17;
		Yubn[5][1]=18;
        Yubn[6][0]=7;
		Yubn[6][1]=8;
		Yubn[7][0]=19;
		Yubn[7][1]=20;
		Yubn[8][0]=9;
		Yubn[8][1]=10;
		Yubn[9][0]=21;
		Yubn[9][1]=22;
		Yubn[10][0]=11;
		Yubn[10][1]=12;
		Yubn[11][0]=23;
		Yubn[11][1]=24;
	}
	
	DTlast=m_FDT-m_CT-DTair;
	
	if(DTlast>=0.0)
	{
		for(i=1;i<=24;i++)
		{
			for(j=0;j<12;j++)
			{
				for(k=0;k<2;k++)
				{
					if(i==Yubn[j][k])
					{
						if(Isdflg[j]==0&&Isdflg[j+1]==0)
						{
							Icase=1;
							DTsd=DTsd0+DTsd0;
							Isdflg[j]=1;
							Isdflg[j+1]=1;
						}
						else if(Isdflg[j]==0)
						{
							Icase=2;
							DTsd=DTsd0;
							Isdflg[j]=1;
						}
						else if(Isdflg[j+1]==0)
						{
							Icase=3;
							DTsd=DTsd0;
							Isdflg[j+1]=1;
						}
						else
						{
							DTsd=0.0;
						}
						DTW=DTW+DTbnk0+DTsd;
						if(DTW>DTlast)
						{
							DTW=DTW-DTbnk0-DTsd;
							if(Icase==1||Icase==2)
							{
								Isdflg[j]=0;
							}
							if(Icase==1||Icase==3)
							{
								Isdflg[j+1]=0;
							}
							if(DTW+DTsd>DTlast)
							{
								goto outloop100;
							}
							DTW=DTW+DTsd;
							DTwbnk[j]=DTwbnk[j]+DTsd;
							for(m=0;m<6;m++)
							{
								DTW=DTW+DTbnk0/6.0;
								if(DTW>DTlast)
								{
									DTW=DTW-DTbnk0/6.0;
									goto outloop200;
								}
								Isused1[j][k][m]=1;
								DTwbnk[j]=DTwbnk[j]+DTbnk0/6.0;
							}
						}
						
						for(m=0;m<6;m++)
						{
							Isused1[j][k][m]=1;
						}
						
						DTwbnk[j]=DTwbnk[j]+DTbnk0+DTsd;
					}
				}
			}
		}
	}

outloop100:

	for(j=12;j<15;j++)
	{
		if(Isdflg[j]==0&&Isdflg[j+1]==0)
		{
			Icase=1;
			DTsd=DTsd0*2.0;
		}
		else if(Isdflg[j+1]==0)
		{
			Icase=3;
			DTsd=DTsd0;
		}
		DTW=DTW+DTsd;

		if(DTW>DTlast)
		{
			DTW=DTW-DTsd;
			break;
			DTwbnk[j]=DTwbnk[j]+DTsd;
		}
		for(k=0;k<2;k++)
		{
			for(m=0;m<12;m++)
			{
				DTW=DTW+DTbnk0/12.0;
				if(DTW>DTlast)
				{
					DTW=DTW-DTbnk0/12.0;
					goto outloop200;
				}
				Isused2[j-12][k][m]=1;
				DTwbnk[j]=DTwbnk[j]+DTbnk0/12.0;

			}
		}
	}

outloop200:

	for(i=0;i<12;i++)
	{
		for(j=0;j<2;j++)
		{
			for(m=0;m<6;m++)
			{
				if(j==0)
				{
					if(Isused1[i][j][m]==1)
					{
						Upperbnk[i]=Upperbnk[i]+1;
					}
				}
				else if(j==1)
				{
					if(Isused1[i][j][m]==1)
					{
						Lowerbnk[i]=Lowerbnk[i]+1;
					}
				}
			}
		}
	}

	for(i=0;i<3;i++)
	{
		for(j=0;j<2;j++)
		{
			for(m=0;m<12;m++)
			{
				if(j==0)
				{
					if(Isused2[i][j][m]==1)
					{
						Upperbnk[i+12]++;
					}
				}
				else if(j==1)
				{
					if(Isused2[i][j][m]==1)
					{
						Lowerbnk[i+12]++;
					}
				}
			}
		}
	}
	
    m_CT=m_FDT-DTW-DTair;

	
	for(i=1;i<=15;i++)
	{
		m_T=m_T-DTair/108.2*(Posbnk[i][0]-Posbnk[i-1][1]);
		Temperature[i-1][0]=m_T;
		Time[i-1][0]=Posbnk[i][0]/4.35;

		m_T=m_T-(DTwbnk[i-1]/4.4+DTair/108.2)*(Posbnk[i][1]-Posbnk[i][0]);
		Temperature[i-1][1]=m_T;
		Time[i-1][1]=Posbnk[i][1]/4.35;

		/*将结果数据回传到输出参数*/
		pXTime[ i * 2 - 1 ] = Time[i-1][0];
		pXTime[ i * 2 ] = Time[i-1][1];

		pYTemp[ i * 2 - 1 ] = Temperature[i-1][0];
		pYTemp[ i * 2 ] = Temperature[i-1][1];

	}
	Time[15][0]=Time[15][1]=108.2/4.35;
	m_T=m_T-DTair/108.2*(108.2-Posbnk[15][1]);
	Temperature[15][0]=Temperature[15][1]=m_T;

	/*将结果数据回传到输出参数*/
	pXTime[0 ] = 0;             pYTemp[0 ] = m_FDT;
	pXTime[31] = Time[15][1];   pYTemp[31] = Temperature[15][1];
	
	return 0 ;
}





extern "C" int PASCAL EXPORT 
CalcSprayer(int    m_mode/*冷却方式:*/ , 
		   double m_FDT /*终轧温度:*/ , 
		   double m_CT  /*卷曲温度*/ ,
		   char * buff_low,	/*[0-14]*/
		   char * buff_up ) /*[0-14]*/
{
	// TODO: Add your command handler code here
	int i,j,k,m;
	int Yubn[12][2];
	int Isdflg[16];
	// Isused1[j][k][m]前12个水箱，Isused2[j][k][m],13,14,15个水箱.
	int Isused1[12][2][6];
	int Isused2[3][2][12];
	//**************************** 输出数据
	int Upperbnk[15];
	int Lowerbnk[15];
	// 温度数组
	//输出数据****************************// 
	double DTwbnk[15];
	double Posbnk[16][2];
	int Icase;
	double DTbnk0=12.0;
	double DTsd0=2.0;
	double DTsd;
	double DTlast;
	double DTair;
	double DTW;
	double m_T;
	
	//Upper=0;
	//Lower=0;
	DTW=0.0;
	Icase=0;
	DTair=40.2;
	m_T=m_FDT;
	
	for(i=0;i<16;i++)
	{
		Isdflg[i]=0;
	}
	
	for(i=0;i<15;i++)
	{
		Upperbnk[i]=0;
		Lowerbnk[i]=0;
	}
	
	for(i=0;i<12;i++)
	{
		for(j=0;j<2;j++)
		{
			for(m=0;m<6;m++)
			{
				Isused1[i][j][m]=0;
			}
		}
	}
	
	for(i=0;i<3;i++)
	{
		for(j=0;j<2;j++)
		{
			for(m=0;m<12;m++)
			{
				Isused2[i][j][m]=0;
			}
		}
	}
	
	for(i=0;i<12;i++)
	{
		for(j=0;j<2;j++)
		{
			Yubn[i][j]=0;
		}
	}
	
	for(i=0;i<15;i++)
	{
		DTwbnk[i]=0.0;
	}
	
	for(i=0;i<16;i++)
	{
		if(i==0)
		{
			Posbnk[i][0]=0.0;
			Posbnk[i][1]=0.0;
		}
		Posbnk[i][0]=1.4+(i-1)*5.6;
		Posbnk[i][1]=Posbnk[i][0]+4.4;
	}
	
	if(m_mode==1)
	{
		Yubn[0][0]=1;
		Yubn[0][1]=2;
		Yubn[1][0]=3;
		Yubn[1][1]=4;
		Yubn[2][0]=5;
		Yubn[2][1]=6;
		Yubn[3][0]=7;
		Yubn[3][1]=8;
		Yubn[4][0]=9;
		Yubn[4][1]=10;
		Yubn[5][0]=11;
		Yubn[5][1]=12;
        Yubn[6][0]=13;
		Yubn[6][1]=14;
		Yubn[7][0]=15;
		Yubn[7][1]=16;
		Yubn[8][0]=17;
		Yubn[8][1]=18;
		Yubn[9][0]=19;
		Yubn[9][1]=20;
		Yubn[10][0]=21;
		Yubn[10][1]=22;
		Yubn[11][0]=23;
		Yubn[11][1]=24;	
	}
	else if(m_mode==3)
	{
		Yubn[0][0]=23;
		Yubn[0][1]=24;
		Yubn[1][0]=21;
		Yubn[1][1]=22;
		Yubn[2][0]=19;
		Yubn[2][1]=20;
		Yubn[3][0]=17;
		Yubn[3][1]=18;
		Yubn[4][0]=15;
		Yubn[4][1]=16;
		Yubn[5][0]=13;
		Yubn[5][1]=14;
        Yubn[6][0]=1;
		Yubn[6][1]=2;
		Yubn[7][0]=3;
		Yubn[7][1]=4;
		Yubn[8][0]=5;
		Yubn[8][1]=6;
		Yubn[9][0]=7;
		Yubn[9][1]=8;
		Yubn[10][0]=9;
		Yubn[10][1]=10;
		Yubn[11][0]=11;
		Yubn[11][1]=12;
	}
	else if(m_mode==2)
	{
        Yubn[0][0]=1;
		Yubn[0][1]=2;
		Yubn[1][0]=13;
		Yubn[1][1]=14;
		Yubn[2][0]=3;
		Yubn[2][1]=4;
		Yubn[3][0]=15;
		Yubn[3][1]=16;
		Yubn[4][0]=5;
		Yubn[4][1]=6;
		Yubn[5][0]=17;
		Yubn[5][1]=18;
        Yubn[6][0]=7;
		Yubn[6][1]=8;
		Yubn[7][0]=19;
		Yubn[7][1]=20;
		Yubn[8][0]=9;
		Yubn[8][1]=10;
		Yubn[9][0]=21;
		Yubn[9][1]=22;
		Yubn[10][0]=11;
		Yubn[10][1]=12;
		Yubn[11][0]=23;
		Yubn[11][1]=24;
	}
	
	DTlast=m_FDT-m_CT-DTair;
	
	if(DTlast>=0.0)
	{
		for(i=1;i<=24;i++)
		{
			for(j=0;j<12;j++)
			{
				for(k=0;k<2;k++)
				{
					if(i==Yubn[j][k])
					{
						if(Isdflg[j]==0&&Isdflg[j+1]==0)
						{
							Icase=1;
							DTsd=DTsd0+DTsd0;
							Isdflg[j]=1;
							Isdflg[j+1]=1;
						}
						else if(Isdflg[j]==0)
						{
							Icase=2;
							DTsd=DTsd0;
							Isdflg[j]=1;
						}
						else if(Isdflg[j+1]==0)
						{
							Icase=3;
							DTsd=DTsd0;
							Isdflg[j+1]=1;
						}
						else
						{
							DTsd=0.0;
						}
						DTW=DTW+DTbnk0+DTsd;
						if(DTW>DTlast)
						{
							DTW=DTW-DTbnk0-DTsd;
							if(Icase==1||Icase==2)
							{
								Isdflg[j]=0;
							}
							if(Icase==1||Icase==3)
							{
								Isdflg[j+1]=0;
							}
							if(DTW+DTsd>DTlast)
							{
								goto outloop100;
							}
							DTW=DTW+DTsd;
							DTwbnk[j]=DTwbnk[j]+DTsd;
							for(m=0;m<6;m++)
							{
								DTW=DTW+DTbnk0/6.0;
								if(DTW>DTlast)
								{
									DTW=DTW-DTbnk0/6.0;
									goto outloop200;
								}
								Isused1[j][k][m]=1;
								DTwbnk[j]=DTwbnk[j]+DTbnk0/6.0;
							}
						}
						
						for(m=0;m<6;m++)
						{
							Isused1[j][k][m]=1;
						}
						
						DTwbnk[j]=DTwbnk[j]+DTbnk0+DTsd;
					}
				}
			}
		}
	}

outloop100:

	for(j=12;j<15;j++)
	{
		if(Isdflg[j]==0&&Isdflg[j+1]==0)
		{
			Icase=1;
			DTsd=DTsd0*2.0;
		}
		else if(Isdflg[j+1]==0)
		{
			Icase=3;
			DTsd=DTsd0;
		}
		DTW=DTW+DTsd;

		if(DTW>DTlast)
		{
			DTW=DTW-DTsd;
			break;
			DTwbnk[j]=DTwbnk[j]+DTsd;
		}
		for(k=0;k<2;k++)
		{
			for(m=0;m<12;m++)
			{
				DTW=DTW+DTbnk0/12.0;
				if(DTW>DTlast)
				{
					DTW=DTW-DTbnk0/12.0;
					goto outloop200;
				}
				Isused2[j-12][k][m]=1;
				DTwbnk[j]=DTwbnk[j]+DTbnk0/12.0;
			}
		}
	}

outloop200:

	for(i=0;i<12;i++)
	{
		for(j=0;j<2;j++)
		{
			for(m=0;m<6;m++)
			{
				if(j==0)
				{
					if(Isused1[i][j][m]==1)
					{
						Upperbnk[i]=Upperbnk[i]+1;
					}
				}
				else if(j==1)
				{
					if(Isused1[i][j][m]==1)
					{
						Lowerbnk[i]=Lowerbnk[i]+1;
					}
				}
			}
		}
	}

	for(i=0;i<3;i++)
	{
		for(j=0;j<2;j++)
		{
			for(m=0;m<12;m++)
			{
				if(j==0)
				{
					if(Isused2[i][j][m]==1)
					{
						Upperbnk[i+12]++;
					}
				}
				else if(j==1)
				{
					if(Isused2[i][j][m]==1)
					{
						Lowerbnk[i+12]++;
					}
				}
			}
		}
	}
	

	sprintf( buff_up , "%d%d%d%d%d%d%d%d%d%d%d%d%x%x%x" , 
		Upperbnk[0] , Upperbnk[1] , Upperbnk[2], Upperbnk[3], Upperbnk[4] , Upperbnk[5], 
		Upperbnk[6] , Upperbnk[7] , Upperbnk[8], Upperbnk[9], Upperbnk[10], Upperbnk[11], 
		Upperbnk[12], Upperbnk[13], Upperbnk[14] ) ;
	
	sprintf( buff_low , "%d%d%d%d%d%d%d%d%d%d%d%d%x%x%x" , 
		Lowerbnk[0] , Lowerbnk[1] , Lowerbnk[2], Lowerbnk[3], Lowerbnk[4] , Lowerbnk[5], 
		Lowerbnk[6] , Lowerbnk[7] , Lowerbnk[8], Lowerbnk[9], Lowerbnk[10], Lowerbnk[11], 
		Lowerbnk[12], Lowerbnk[13], Lowerbnk[14] ) ;
	
	return 0 ;
}

extern "C" int PASCAL EXPORT
    CalCoolingTime(int stripNSeg, //带钢分段数
	               double slabLength, //钢坯长度
				   double slabHeight, //钢坯厚度
				   double slabWidth,  //钢坯宽度
				   double stripHeight,//带钢厚度
				   double stripWidth, //带钢宽度
				   double m_VF7,     //头部抛钢速度
				   double m_Acc1,    //No.1 Acceleration
				   double m_Acc2,    //No.2 Acceleration
				   double m_Vmax,    //最大速度
				   double m_Vout,    //尾部抛钢速度
				   double *pTime)    
{
    int i;
	int m_Modecooling; //判断是否达到最大速度
	double LengthperSeg;
	double Length;
	double t1; //匀速时间
	double t2; //No.1 Acceleration时间
	double t3; //No.2 Acceleration时间
	double t30;
	double t4; //Deceleration的时间
	double t5; //尾部匀速时间
	double m_V7; //第i个Segment出F7的速度
	double m_Vacc2; //第二加速时的初速度
	double m_Vdec; //减速时的初速度
	double m_Maxl; //达到最大速度时的距离
	double m_Ldece; //开始减速时的位置
	double m_Dece;  //减速度

	LengthperSeg=slabLength*slabHeight*slabWidth/(stripHeight*stripWidth*stripNSeg);
	m_Ldece=stripNSeg*LengthperSeg-30.0;
	if(m_Acc1==0.0&&m_Acc2==0.0)
	{
		for(i=0;i<stripNSeg;i++)
		{
			pTime[i]=126.85/m_VF7;
		}
	}
	else
	{
		if(m_Ldece>=150.0)
		{
        for(i=0;i<stripNSeg;i++)
		{
		    t1=0.0;
		    t2=0.0;
		    t3=0.0;
		    t4=0.0;
		    t5=0.0;
            m_Modecooling=0;
		
		    Length=(i + 1) * LengthperSeg;
		
		    if(Length<=50.0)
			{
			    t1=(50.0-Length)/m_VF7;
			    if((126.85+Length)<=150.0)
				{
				    t2=(-m_VF7+sqrt(m_VF7*m_VF7+2.0*m_Acc1*(126.85+Length-50.0)))/m_Acc1;
				}
			    else if(150.0<(126.85+Length)&&(126.85+Length)<=m_Ldece)
				{
				    t2=(-m_VF7+sqrt(m_VF7*m_VF7+2.0*m_Acc1*(150.0-50.0)))/m_Acc1;
				    m_Vacc2=m_VF7+m_Acc1*t2;
				    t3=(-m_Vacc2+sqrt(m_Vacc2*m_Vacc2+2.0*m_Acc2*(126.85+Length-150.0)))/m_Acc2;
				    m_Vdec=m_Vacc2+m_Acc2*t3;
				    if(m_Vdec>m_Vmax)
					{
					    m_Vdec=m_Vmax;
					    t3=(m_Vmax-m_Vacc2)/m_Acc2+(126.85+Length-150.0-(m_Vmax*m_Vmax-m_Vacc2*m_Vacc2)/(2.0*m_Acc2))/m_Vmax;
					}
				}
			    else if(m_Ldece<(126.85+Length)&&(126.85+Length)<=(m_Ldece+30.0))
				{
                    t2=(-m_VF7+sqrt(m_VF7*m_VF7+2.0*m_Acc1*(150.0-50.0)))/m_Acc1;
				    m_Vacc2=m_VF7+m_Acc1*t2;
				    t3=(-m_Vacc2+sqrt(m_Vacc2*m_Vacc2+2.0*m_Acc2*(m_Ldece-150.0)))/m_Acc2;
				    m_Vdec=m_Vacc2+m_Acc2*t3;
				    if(m_Vdec>m_Vmax)
					{
					    m_Vdec=m_Vmax;
					    t3=(m_Vmax-m_Vacc2)/m_Acc2+(m_Ldece-150.0-(m_Vmax*m_Vmax-m_Vacc2*m_Vacc2)/(2.0*m_Acc2))/m_Vmax;
					}
				    m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
				    t4=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(126.85+Length-m_Ldece)))/m_Dece;
				}
			    else if((m_Ldece+30.0)<(126.85+Length))
				{
                    t2=(-m_VF7+sqrt(m_VF7*m_VF7+2.0*m_Acc1*(150.0-50.0)))/m_Acc1;
				    m_Vacc2=m_VF7+m_Acc1*t2;
				    t3=(-m_Vacc2+sqrt(m_Vacc2*m_Vacc2+2.0*m_Acc2*(m_Ldece-150.0)))/m_Acc2;
				    m_Vdec=m_Vacc2+m_Acc2*t3;
				    if(m_Vdec>m_Vmax)
					{
					    m_Vdec=m_Vmax;
					    t3=(m_Vmax-m_Vacc2)/m_Acc2+(m_Ldece-150.0-(m_Vmax*m_Vmax-m_Vacc2*m_Vacc2)/(2.0*m_Acc2))/m_Vmax;
					}
				    m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
				    t4=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(m_Ldece+30.0-m_Ldece)))/m_Dece;
				    t5=(126.85+Length-(m_Ldece+30.0))/m_Vout;
				}

			    pTime[i]=t1+t2+t3+t4+t5;
			}
		    if(50.0<Length&&Length<=150.0)
			{
			    m_V7=sqrt(m_VF7*m_VF7+2.0*m_Acc1*(Length-50.0));
			    if((Length+126.85)<150.0)
				{
				    t2=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Acc1*(126.85+Length-Length)))/m_Acc1;
				    m_Vacc2=m_V7+m_Acc1*t2;
				    t3=0.0;
				    t4=0.0;
				    t5=0.0;
				}
			
			    if(150.0<(126.85+Length)&&(126.85+Length)<=m_Ldece)
				{
                    t2=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Acc1*(150.0-Length)))/m_Acc1;
			        m_Vacc2=m_V7+m_Acc1*t2;
				    t3=(-m_Vacc2+sqrt(m_Vacc2*m_Vacc2+2.0*m_Acc2*(126.85+Length-150.0)))/m_Acc2;
			 	    m_Vdec=m_Vacc2+m_Acc2*t3;
				    if(m_Vdec>m_Vmax)
					{
					    m_Vdec=m_Vmax;
					    t3=(m_Vmax-m_Vacc2)/m_Acc2+(126.85+Length-150.0-(m_Vmax*m_Vmax-m_Vacc2*m_Vacc2)/(2.0*m_Acc2))/m_Vmax;
					}
				    t4=0.0;
				}
			    else if(m_Ldece<(126.85+Length)&&(126.85+Length)<=(m_Ldece+30.0))
				{
                    t2=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Acc1*(150.0-Length)))/m_Acc1;
			        m_Vacc2=m_V7+m_Acc1*t2;
			        t3=(-m_Vacc2+sqrt(m_Vacc2*m_Vacc2+2.0*m_Acc2*(m_Ldece-150.0)))/m_Acc2;
				    m_Vdec=m_Vacc2+m_Acc2*t3;
                    if(m_Vdec>m_Vmax)
					{
					    m_Vdec=m_Vmax;
					    t3=(m_Vmax-m_Vacc2)/m_Acc2+(m_Ldece-150.0-(m_Vmax*m_Vmax-m_Vacc2*m_Vacc2)/(2.0*m_Acc2))/m_Vmax;
					}
				    m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
				    t4=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(126.85+Length-m_Ldece)))/m_Dece;
				}
			    else if((m_Ldece+30.0)<(126.85+Length))
				{
                    t2=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Acc1*(150.0-Length)))/m_Acc1;
			        m_Vacc2=m_V7+m_Acc1*t2;
				    t3=(-m_Vacc2+sqrt(m_Vacc2*m_Vacc2+2.0*m_Acc2*(m_Ldece-150.0)))/m_Acc2;
				    m_Vdec=m_Vacc2+m_Acc2*t3;
				    if(m_Vdec>m_Vmax)
					{
				    	m_Vdec=m_Vmax;
					    t3=(m_Vmax-m_Vacc2)/m_Acc2+(m_Ldece-150.0-(m_Vmax*m_Vmax-m_Vacc2*m_Vacc2)/(2.0*m_Acc2))/m_Vmax;
					}
				    m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
				    t4=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(m_Ldece+30.0-m_Ldece)))/m_Dece;
				    t5=(126.85+Length-(m_Ldece+30.0))/m_Vout;
				}

			    pTime[i]=t1+t2+t3+t4+t5;
			}
		    if(150.0<Length&&Length<=m_Ldece)
			{
			    m_Vacc2=sqrt(m_VF7*m_VF7+2.0*m_Acc1*(150.0-50.0));
			    t30=(-m_Vacc2+sqrt(m_Vacc2*m_Vacc2+2.0*m_Acc2*(Length-150.0)))/m_Acc2;
			    m_V7=m_Vacc2+m_Acc2*t30;
			    if(m_V7>m_Vmax)
				{
				    m_V7=m_Vmax;
				}
			    if((126.85+Length)<m_Ldece)
				{
				    t3=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Acc2*(126.85+Length-Length)))/m_Acc2;
				    m_Vdec=m_V7+m_Acc2*t3;
                    if(m_Vdec>m_Vmax)
					{
					    m_Vdec=m_Vmax;
					    t3=(m_Vmax-m_V7)/m_Acc2+(126.85+Length-Length-(m_Vmax*m_Vmax-m_V7*m_V7)/(2.0*m_Acc2))/m_Vmax;
					}
				    t4=0.0;
					t5=0.0;
				}
			    else if(m_Ldece<(126.85+Length)&&(126.85+Length)<=(m_Ldece+30.0))
				{
			        t3=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Acc2*(m_Ldece-Length)))/m_Acc2;
			        m_Vdec=m_V7+m_Acc2*t3;
                    if(m_Vdec>m_Vmax)
					{
					    m_Vdec=m_Vmax;
					    t3=(m_Vmax-m_V7)/m_Acc2+(m_Ldece-Length-(m_Vmax*m_Vmax-m_V7*m_V7)/(2.0*m_Acc2))/m_Vmax;
					}
			        m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
				    t4=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(126.85+Length-m_Ldece)))/m_Dece;
					t5=0.0;
				}
			    else if((m_Ldece+30.0)<(126.85+Length))
				{
				    t3=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Acc2*(m_Ldece-Length)))/m_Acc2;
				    m_Vdec=m_V7+m_Acc2*t3;
				    if(m_Vdec>m_Vmax)
					{
					    m_Vdec=m_Vmax;
					    t3=(m_Vmax-m_V7)/m_Acc2+(m_Ldece-Length-(m_Vmax*m_Vmax-m_V7*m_V7)/(2.0*m_Acc2))/m_Vmax;
					}
				    m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
				    t4=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(m_Ldece+30.0-m_Ldece)))/m_Dece;
				    t5=(126.85+Length-(m_Ldece+30.0))/m_Vout;
				}
			    pTime[i]=t1+t2+t3+t4+t5;
			}
		    if(m_Ldece<Length&&Length<(m_Ldece+30.0))
			{
			    m_Vdec=sqrt(m_VF7*m_VF7+2.0*m_Acc1*(150.0-50.0)+2.0*m_Acc2*(m_Ldece-150.0));
			    if(m_Vdec>m_Vmax)
				{
				    m_Vdec=m_Vmax;
				}
			    m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
			    t30=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(Length-m_Ldece)))/m_Dece;
			    m_V7=m_Vdec+m_Dece*t30;
			    if((126.85+Length)<=(m_Ldece+30.0))
				{
				    t4=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Dece*(126.85+Length-Length)))/m_Dece;
				    t5=0.0;
				}
			    else if((m_Ldece+30.0)<(126.85+Length))
				{
				    t4=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Dece*(m_Ldece+30.0-Length)))/m_Dece;
				    t5=(126.85+Length-(m_Ldece+30.0))/m_Vout;
				}

			    pTime[i]=t1+t2+t3+t4+t5;
			}
		    if((m_Ldece+30.0)<=Length)
			{
			    t5=126.85/m_Vout;
		        pTime[i]=t1+t2+t3+t4+t5;
			}
		}
		}
		else
		{
        for(i=0;i<stripNSeg;i++)
		{
		    t1=0.0;
		    t2=0.0;
		    t3=0.0;
		    t4=0.0;
		    t5=0.0;
            m_Modecooling=0;
		
		    Length=(i + 1) * LengthperSeg;
		
		    if(Length<=50.0)
			{
			    t1=(50.0-Length)/m_VF7;
			    if((126.85+Length)<=150.0)
				{
				    t2=(-m_VF7+sqrt(m_VF7*m_VF7+2.0*m_Acc1*(126.85+Length-50.0)))/m_Acc1;
				}
			    else if(150.0<(126.85+Length)&&(126.85+Length)<=m_Ldece)
				{
				    t2=(-m_VF7+sqrt(m_VF7*m_VF7+2.0*m_Acc1*(150.0-50.0)))/m_Acc1;
				    m_Vacc2=m_VF7+m_Acc1*t2;
				    t3=(-m_Vacc2+sqrt(m_Vacc2*m_Vacc2+2.0*m_Acc2*(126.85+Length-150.0)))/m_Acc2;
				    m_Vdec=m_Vacc2+m_Acc2*t3;
				    if(m_Vdec>m_Vmax)
					{
					    m_Vdec=m_Vmax;
					    t3=(m_Vmax-m_Vacc2)/m_Acc2+(126.85+Length-150.0-(m_Vmax*m_Vmax-m_Vacc2*m_Vacc2)/(2.0*m_Acc2))/m_Vmax;
					}
				}
			    else if(m_Ldece<(126.85+Length)&&(126.85+Length)<=(m_Ldece+30.0))
				{
                    t2=(-m_VF7+sqrt(m_VF7*m_VF7+2.0*m_Acc1*(m_Ldece-50.0)))/m_Acc1;
					t3=0.0;
				    m_Vdec=m_VF7+m_Acc1*t2;
				    m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
				    t4=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(126.85+Length-m_Ldece)))/m_Dece;
				}
			    else if((m_Ldece+30.0)<(126.85+Length))
				{
                    t2=(-m_VF7+sqrt(m_VF7*m_VF7+2.0*m_Acc1*(m_Ldece-50.0)))/m_Acc1;
				    m_Vdec=m_VF7+m_Acc1*t2;
				    m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
				    t4=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(m_Ldece+30.0-m_Ldece)))/m_Dece;
				    t5=(126.85+Length-(m_Ldece+30.0))/m_Vout;
				}

			    pTime[i]=t1+t2+t3+t4+t5;
			}
		    if(50.0<Length&&Length<=m_Ldece)
			{
			    m_V7=sqrt(m_VF7*m_VF7+2.0*m_Acc1*(Length-50.0));
			    if((126.85+Length)<m_Ldece)
				{
				    t3=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Acc1*(126.85+Length-Length)))/m_Acc1;
				    m_Vdec=m_V7+m_Acc1*t3;
				    t4=0.0;
					t5=0.0;
				}
			    else if(m_Ldece<(126.85+Length)&&(126.85+Length)<=(m_Ldece+30.0))
				{
			        t3=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Acc1*(m_Ldece-Length)))/m_Acc1;
			        m_Vdec=m_V7+m_Acc1*t3;
			        m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
				    t4=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(126.85+Length-m_Ldece)))/m_Dece;
					t5=0.0;
				}
			    else if((m_Ldece+30.0)<(126.85+Length))
				{
				    t3=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Acc1*(m_Ldece-Length)))/m_Acc1;
				    m_Vdec=m_V7+m_Acc1*t3;
				    m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
				    t4=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(m_Ldece+30.0-m_Ldece)))/m_Dece;
				    t5=(126.85+Length-(m_Ldece+30.0))/m_Vout;
				}
			    pTime[i]=t1+t2+t3+t4+t5;
			}
		    if(m_Ldece<Length&&Length<(m_Ldece+30.0))
			{
			    m_Vdec=sqrt(m_VF7*m_VF7+2.0*m_Acc1*(m_Ldece-50.0));
			    m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
			    t30=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(Length-m_Ldece)))/m_Dece;
			    m_V7=m_Vdec+m_Dece*t30;
			    if((126.85+Length)<=(m_Ldece+30.0))
				{
				    t4=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Dece*(126.85+Length-Length)))/m_Dece;
				    t5=0.0;
				}
			    else if((m_Ldece+30.0)<(126.85+Length))
				{
				    t4=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Dece*(m_Ldece+30.0-Length)))/m_Dece;
				    t5=(126.85+Length-(m_Ldece+30.0))/m_Vout;
				}

			    pTime[i]=t1+t2+t3+t4+t5;
			}
		    if((m_Ldece+30.0)<=Length)
			{
			    t5=126.85/m_Vout;
		        pTime[i]=t1+t2+t3+t4+t5;
			}
		}
		}
	}
	return 0;
}

extern "C" int PASCAL EXPORT
    CalBankTime(int stripNSeg, //带钢分段数
	               double slabLength, //钢坯长度
				   double slabHeight, //钢坯厚度
				   double slabWidth,  //钢坯宽度
				   double stripHeight,//带钢厚度
				   double stripWidth, //带钢宽度
				   double m_VF7,     //头部抛钢速度
				   double m_Acc1,    //No.1 Acceleration
				   double m_Acc2,    //No.2 Acceleration
				   double m_Vmax,    //最大速度
				   double m_Vout,    //尾部抛钢速度
				   double *pBankTime)    
{
    int i;             //分段数
	int j;             //Bank数
	int m_Modecooling; //判断是否达到最大速度
	double LengthperSeg;
	double Length;
	double t1; //匀速时间
	double t2; //No.1 Acceleration时间
	double t3; //No.2 Acceleration时间
	double t30;
	double t4; //Deceleration的时间
	double t5; //尾部匀速时间
	double m_V7; //第i个Segment出F7的速度
	double m_Vacc2; //第二加速时的初速度
	double m_Vdec; //减速时的初速度
	double m_Maxl; //达到最大速度时的距离
	double m_Ldece; //开始减速时的位置
	double m_Dece;  //减速度
	double m_PosBank[32];//Bank的位置

	LengthperSeg=slabLength*slabHeight*slabWidth/(stripHeight*stripWidth*stripNSeg);
	m_Ldece=stripNSeg*LengthperSeg-30.0;
    for(j=0;j<32;j++)
	{
		if(j==0)
		{
			m_PosBank[j]=0.0;
		}
		else if(j==1)
		{
			m_PosBank[j]=1.4;
		}
		else if(j==31)
		{
			m_PosBank[j]=108.2;
		}
		else
		{
			if(fmod(double(j),2.0)==0.0)
			{
				m_PosBank[j]=m_PosBank[j-1]+4.4;
			}
			else
			{
		        m_PosBank[j]=m_PosBank[j-1]+1.2;
			}
		}
	}

	if(m_Acc1==0.0&&m_Acc2==0.0)
	{
		for(i=0;i<stripNSeg;i++)
		{
			for(j=0;j<32;j++)
			{
			    pBankTime[i*32+j]=m_PosBank[j]/m_VF7;
			}
		}
	}
	else
	{
		if(m_Ldece>=150.0)
		{
        for(i=0;i<stripNSeg;i++)
		{
			for(j=0;j<32;j++)
			{
		    t1=0.0;
		    t2=0.0;
		    t3=0.0;
		    t4=0.0;
		    t5=0.0;
            m_Modecooling=0;
		
		    Length=(i + 1) * LengthperSeg;
		
		    if(Length<=50.0)
			{
				t1=(50.0-Length)/m_VF7;
				if((m_PosBank[j]+Length)<=50.0)
				{
					t1=m_PosBank[j]/m_VF7;
				}
			    if(50.0<(m_PosBank[j]+Length)&&(m_PosBank[j]+Length)<=150.0)
				{
				    t2=(-m_VF7+sqrt(m_VF7*m_VF7+2.0*m_Acc1*(m_PosBank[j]+Length-50.0)))/m_Acc1;
				}
			    else if(150.0<(m_PosBank[j]+Length)&&(m_PosBank[j]+Length)<=m_Ldece)
				{
				    t2=(-m_VF7+sqrt(m_VF7*m_VF7+2.0*m_Acc1*(150.0-50.0)))/m_Acc1;
				    m_Vacc2=m_VF7+m_Acc1*t2;
				    t3=(-m_Vacc2+sqrt(m_Vacc2*m_Vacc2+2.0*m_Acc2*(m_PosBank[j]+Length-150.0)))/m_Acc2;
				    m_Vdec=m_Vacc2+m_Acc2*t3;
				    if(m_Vdec>m_Vmax)
					{
					    m_Vdec=m_Vmax;
					    t3=(m_Vmax-m_Vacc2)/m_Acc2+(m_PosBank[j]+Length-150.0-(m_Vmax*m_Vmax-m_Vacc2*m_Vacc2)/(2.0*m_Acc2))/m_Vmax;
					}
				}
			    else if(m_Ldece<(m_PosBank[j]+Length)&&(m_PosBank[j]+Length)<=(m_Ldece+30.0))
				{
                    t2=(-m_VF7+sqrt(m_VF7*m_VF7+2.0*m_Acc1*(150.0-50.0)))/m_Acc1;
				    m_Vacc2=m_VF7+m_Acc1*t2;
				    t3=(-m_Vacc2+sqrt(m_Vacc2*m_Vacc2+2.0*m_Acc2*(m_Ldece-150.0)))/m_Acc2;
				    m_Vdec=m_Vacc2+m_Acc2*t3;
				    if(m_Vdec>m_Vmax)
					{
					    m_Vdec=m_Vmax;
					    t3=(m_Vmax-m_Vacc2)/m_Acc2+(m_Ldece-150.0-(m_Vmax*m_Vmax-m_Vacc2*m_Vacc2)/(2.0*m_Acc2))/m_Vmax;
					}
				    m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
				    t4=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(m_PosBank[j]+Length-m_Ldece)))/m_Dece;
				}
			    else if((m_Ldece+30.0)<(m_PosBank[j]+Length))
				{
                    t2=(-m_VF7+sqrt(m_VF7*m_VF7+2.0*m_Acc1*(150.0-50.0)))/m_Acc1;
				    m_Vacc2=m_VF7+m_Acc1*t2;
				    t3=(-m_Vacc2+sqrt(m_Vacc2*m_Vacc2+2.0*m_Acc2*(m_Ldece-150.0)))/m_Acc2;
				    m_Vdec=m_Vacc2+m_Acc2*t3;
				    if(m_Vdec>m_Vmax)
					{
					    m_Vdec=m_Vmax;
					    t3=(m_Vmax-m_Vacc2)/m_Acc2+(m_Ldece-150.0-(m_Vmax*m_Vmax-m_Vacc2*m_Vacc2)/(2.0*m_Acc2))/m_Vmax;
					}
				    m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
				    t4=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(m_Ldece+30.0-m_Ldece)))/m_Dece;
				    t5=(m_PosBank[j]+Length-(m_Ldece+30.0))/m_Vout;
				}

			    pBankTime[i*32+j]=t1+t2+t3+t4+t5;
			}
		    if(50.0<Length&&Length<=150.0)
			{
			    m_V7=sqrt(m_VF7*m_VF7+2.0*m_Acc1*(Length-50.0));
			    if((Length+m_PosBank[j])<150.0)
				{
				    t2=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Acc1*(m_PosBank[j]+Length-Length)))/m_Acc1;
				    m_Vacc2=m_V7+m_Acc1*t2;
				    t3=0.0;
				    t4=0.0;
				    t5=0.0;
				}
			
			    if(150.0<(m_PosBank[j]+Length)&&(m_PosBank[j]+Length)<=m_Ldece)
				{
                    t2=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Acc1*(150.0-Length)))/m_Acc1;
			        m_Vacc2=m_V7+m_Acc1*t2;
				    t3=(-m_Vacc2+sqrt(m_Vacc2*m_Vacc2+2.0*m_Acc2*(m_PosBank[j]+Length-150.0)))/m_Acc2;
			 	    m_Vdec=m_Vacc2+m_Acc2*t3;
				    if(m_Vdec>m_Vmax)
					{
					    m_Vdec=m_Vmax;
					    t3=(m_Vmax-m_Vacc2)/m_Acc2+(m_PosBank[j]+Length-150.0-(m_Vmax*m_Vmax-m_Vacc2*m_Vacc2)/(2.0*m_Acc2))/m_Vmax;
					}
				    t4=0.0;
				}
			    else if(m_Ldece<(m_PosBank[j]+Length)&&(m_PosBank[j]+Length)<=(m_Ldece+30.0))
				{
                    t2=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Acc1*(150.0-Length)))/m_Acc1;
			        m_Vacc2=m_V7+m_Acc1*t2;
			        t3=(-m_Vacc2+sqrt(m_Vacc2*m_Vacc2+2.0*m_Acc2*(m_Ldece-150.0)))/m_Acc2;
				    m_Vdec=m_Vacc2+m_Acc2*t3;
                    if(m_Vdec>m_Vmax)
					{
					    m_Vdec=m_Vmax;
					    t3=(m_Vmax-m_Vacc2)/m_Acc2+(m_Ldece-150.0-(m_Vmax*m_Vmax-m_Vacc2*m_Vacc2)/(2.0*m_Acc2))/m_Vmax;
					}
				    m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
				    t4=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(m_PosBank[j]+Length-m_Ldece)))/m_Dece;
				}
			    else if((m_Ldece+30.0)<(m_PosBank[j]+Length))
				{
                    t2=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Acc1*(150.0-Length)))/m_Acc1;
			        m_Vacc2=m_V7+m_Acc1*t2;
				    t3=(-m_Vacc2+sqrt(m_Vacc2*m_Vacc2+2.0*m_Acc2*(m_Ldece-150.0)))/m_Acc2;
				    m_Vdec=m_Vacc2+m_Acc2*t3;
				    if(m_Vdec>m_Vmax)
					{
				    	m_Vdec=m_Vmax;
					    t3=(m_Vmax-m_Vacc2)/m_Acc2+(m_Ldece-150.0-(m_Vmax*m_Vmax-m_Vacc2*m_Vacc2)/(2.0*m_Acc2))/m_Vmax;
					}
				    m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
				    t4=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(m_Ldece+30.0-m_Ldece)))/m_Dece;
				    t5=(m_PosBank[j]+Length-(m_Ldece+30.0))/m_Vout;
				}

			    pBankTime[i*32+j]=t1+t2+t3+t4+t5;
			}
		    if(150.0<Length&&Length<=m_Ldece)
			{
			    m_Vacc2=sqrt(m_VF7*m_VF7+2.0*m_Acc1*(150.0-50.0));
			    t30=(-m_Vacc2+sqrt(m_Vacc2*m_Vacc2+2.0*m_Acc2*(Length-150.0)))/m_Acc2;
			    m_V7=m_Vacc2+m_Acc2*t30;
			    if(m_V7>m_Vmax)
				{
				    m_V7=m_Vmax;
				}
			    if((m_PosBank[j]+Length)<m_Ldece)
				{
				    t3=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Acc2*(m_PosBank[j]+Length-Length)))/m_Acc2;
				    m_Vdec=m_V7+m_Acc2*t3;
                    if(m_Vdec>m_Vmax)
					{
					    m_Vdec=m_Vmax;
					    t3=(m_Vmax-m_V7)/m_Acc2+(m_PosBank[j]+Length-Length-(m_Vmax*m_Vmax-m_V7*m_V7)/(2.0*m_Acc2))/m_Vmax;
					}
				    t4=0.0;
					t5=0.0;
				}
			    else if(m_Ldece<(m_PosBank[j]+Length)&&(m_PosBank[j]+Length)<=(m_Ldece+30.0))
				{
			        t3=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Acc2*(m_Ldece-Length)))/m_Acc2;
			        m_Vdec=m_V7+m_Acc2*t3;
                    if(m_Vdec>m_Vmax)
					{
					    m_Vdec=m_Vmax;
					    t3=(m_Vmax-m_V7)/m_Acc2+(m_Ldece-Length-(m_Vmax*m_Vmax-m_V7*m_V7)/(2.0*m_Acc2))/m_Vmax;
					}
			        m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
				    t4=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(m_PosBank[j]+Length-m_Ldece)))/m_Dece;
					t5=0.0;
				}
			    else if((m_Ldece+30.0)<(m_PosBank[j]+Length))
				{
				    t3=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Acc2*(m_Ldece-Length)))/m_Acc2;
				    m_Vdec=m_V7+m_Acc2*t3;
				    if(m_Vdec>m_Vmax)
					{
					    m_Vdec=m_Vmax;
					    t3=(m_Vmax-m_V7)/m_Acc2+(m_Ldece-Length-(m_Vmax*m_Vmax-m_V7*m_V7)/(2.0*m_Acc2))/m_Vmax;
					}
				    m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
				    t4=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(m_Ldece+30.0-m_Ldece)))/m_Dece;
				    t5=(m_PosBank[j]+Length-(m_Ldece+30.0))/m_Vout;
				}
			    pBankTime[i*32+j]=t1+t2+t3+t4+t5;
			}
		    if(m_Ldece<Length&&Length<(m_Ldece+30.0))
			{
			    m_Vdec=sqrt(m_VF7*m_VF7+2.0*m_Acc1*(150.0-50.0)+2.0*m_Acc2*(m_Ldece-150.0));
			    if(m_Vdec>m_Vmax)
				{
				    m_Vdec=m_Vmax;
				}
			    m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
			    t30=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(Length-m_Ldece)))/m_Dece;
			    m_V7=m_Vdec+m_Dece*t30;
			    if((m_PosBank[j]+Length)<=(m_Ldece+30.0))
				{
				    t4=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Dece*(m_PosBank[j]+Length-Length)))/m_Dece;
				    t5=0.0;
				}
			    else if((m_Ldece+30.0)<(m_PosBank[j]+Length))
				{
				    t4=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Dece*(m_Ldece+30.0-Length)))/m_Dece;
				    t5=(m_PosBank[j]+Length-(m_Ldece+30.0))/m_Vout;
				}

			    pBankTime[i*32+j]=t1+t2+t3+t4+t5;
			}
		    if((m_Ldece+30.0)<=Length)
			{
			    t5=m_PosBank[j]/m_Vout;
		        pBankTime[i*32+j]=t1+t2+t3+t4+t5;
			}
			}
		}
		}
		else
		{
        for(i=0;i<stripNSeg;i++)
		{
			for(j=0;j<32;j++)
			{
		    t1=0.0;
		    t2=0.0;
		    t3=0.0;
		    t4=0.0;
		    t5=0.0;
            m_Modecooling=0;
		
		    Length=(i + 1) * LengthperSeg;
		
		    if(Length<=50.0)
			{
				t1=(50.0-Length)/m_VF7;
			    if((m_PosBank[j]+Length)<=50.0)
				{
					t1=m_PosBank[j]/m_VF7;
				}
			    if(50.0<(m_PosBank[j]+Length)&&(m_PosBank[j]+Length)<=150.0)
				{
				    t2=(-m_VF7+sqrt(m_VF7*m_VF7+2.0*m_Acc1*(m_PosBank[j]+Length-50.0)))/m_Acc1;
				}
			    else if(150.0<(m_PosBank[j]+Length)&&(m_PosBank[j]+Length)<=m_Ldece)
				{
				    t2=(-m_VF7+sqrt(m_VF7*m_VF7+2.0*m_Acc1*(150.0-50.0)))/m_Acc1;
				    m_Vacc2=m_VF7+m_Acc1*t2;
				    t3=(-m_Vacc2+sqrt(m_Vacc2*m_Vacc2+2.0*m_Acc2*(m_PosBank[j]+Length-150.0)))/m_Acc2;
				    m_Vdec=m_Vacc2+m_Acc2*t3;
				    if(m_Vdec>m_Vmax)
					{
					    m_Vdec=m_Vmax;
					    t3=(m_Vmax-m_Vacc2)/m_Acc2+(m_PosBank[j]+Length-150.0-(m_Vmax*m_Vmax-m_Vacc2*m_Vacc2)/(2.0*m_Acc2))/m_Vmax;
					}
				}
			    else if(m_Ldece<(m_PosBank[j]+Length)&&(m_PosBank[j]+Length)<=(m_Ldece+30.0))
				{
                    t2=(-m_VF7+sqrt(m_VF7*m_VF7+2.0*m_Acc1*(m_Ldece-50.0)))/m_Acc1;
					t3=0.0;
				    m_Vdec=m_VF7+m_Acc1*t2;
				    m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
				    t4=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(m_PosBank[j]+Length-m_Ldece)))/m_Dece;
				}
			    else if((m_Ldece+30.0)<(m_PosBank[j]+Length))
				{
                    t2=(-m_VF7+sqrt(m_VF7*m_VF7+2.0*m_Acc1*(m_Ldece-50.0)))/m_Acc1;
				    m_Vdec=m_VF7+m_Acc1*t2;
				    m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
				    t4=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(m_Ldece+30.0-m_Ldece)))/m_Dece;
				    t5=(m_PosBank[j]+Length-(m_Ldece+30.0))/m_Vout;
				}

			    pBankTime[i*32+j]=t1+t2+t3+t4+t5;
			}
		    if(50.0<Length&&Length<=m_Ldece)
			{
			    m_V7=sqrt(m_VF7*m_VF7+2.0*m_Acc1*(Length-50.0));
			    if((m_PosBank[j]+Length)<m_Ldece)
				{
				    t3=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Acc1*(m_PosBank[j]+Length-Length)))/m_Acc1;
				    m_Vdec=m_V7+m_Acc1*t3;
				    t4=0.0;
					t5=0.0;
				}
			    else if(m_Ldece<(m_PosBank[j]+Length)&&(m_PosBank[j]+Length)<=(m_Ldece+30.0))
				{
			        t3=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Acc1*(m_Ldece-Length)))/m_Acc1;
			        m_Vdec=m_V7+m_Acc1*t3;
			        m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
				    t4=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(m_PosBank[j]+Length-m_Ldece)))/m_Dece;
					t5=0.0;
				}
			    else if((m_Ldece+30.0)<(m_PosBank[j]+Length))
				{
				    t3=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Acc1*(m_Ldece-Length)))/m_Acc1;
				    m_Vdec=m_V7+m_Acc1*t3;
				    m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
				    t4=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(m_Ldece+30.0-m_Ldece)))/m_Dece;
				    t5=(m_PosBank[j]+Length-(m_Ldece+30.0))/m_Vout;
				}
			    pBankTime[i*32+j]=t1+t2+t3+t4+t5;
			}
		    if(m_Ldece<Length&&Length<(m_Ldece+30.0))
			{
			    m_Vdec=sqrt(m_VF7*m_VF7+2.0*m_Acc1*(m_Ldece-50.0));
			    m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
			    t30=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(Length-m_Ldece)))/m_Dece;
			    m_V7=m_Vdec+m_Dece*t30;
			    if((m_PosBank[j]+Length)<=(m_Ldece+30.0))
				{
				    t4=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Dece*(m_PosBank[j]+Length-Length)))/m_Dece;
				    t5=0.0;
				}
			    else if((m_Ldece+30.0)<(m_PosBank[j]+Length))
				{
				    t4=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Dece*(m_Ldece+30.0-Length)))/m_Dece;
				    t5=(m_PosBank[j]+Length-(m_Ldece+30.0))/m_Vout;
				}

			    pBankTime[i*32+j]=t1+t2+t3+t4+t5;
			}
		    if((m_Ldece+30.0)<=Length)
			{
			    t5=m_PosBank[j]/m_Vout;
		        pBankTime[i*32+j]=t1+t2+t3+t4+t5;
			}
			}
		}
		}
	}
	return 0;
}


extern "C" int PASCAL EXPORT
    CalF7Speed(int stripNSeg, //带钢分段数
	               double slabLength, //钢坯长度
				   double slabHeight, //钢坯厚度
				   double slabWidth,  //钢坯宽度
				   double stripHeight,//带钢厚度
				   double stripWidth, //带钢宽度
				   double m_VF7,     //头部抛钢速度
				   double m_Acc1,    //No.1 Acceleration
				   double m_Acc2,    //No.2 Acceleration
				   double m_Vmax,    //最大速度
				   double m_Vout,    //尾部抛钢速度
				   double *pTime, 
				   double *pV7)    
{
    int i;
	double LengthperSeg;
	double Length;
	double t1; //匀速时间
	double t2; //No.1 Acceleration时间
	double t3; //No.2 Acceleration时间
	double t30;
	double t4; //Deceleration的时间
	double t5; //尾部匀速时间
	double m_V7; //第i个Segment出F7的速度
	double m_Vacc2; //第二加速时的初速度
	double m_Vdec; //减速时的初速度
	double m_Maxl; //达到最大速度时的距离
	double m_Ldece; //开始减速时的位置
	double m_Dece;  //减速度

	LengthperSeg=slabLength*slabHeight*slabWidth/(stripHeight*stripWidth*stripNSeg);
	m_Ldece=stripNSeg*LengthperSeg-30.0;
	m_V7=0.0;
	if(m_Acc1==0.0&&m_Acc2==0.0)
	{
		for(i=0;i<stripNSeg;i++)
		{
			pTime[i]=125.6/m_VF7;
			m_V7=m_VF7;
			pV7[i]=m_V7;
		}
	}
	else
	{
        for(i=0;i<stripNSeg;i++)
		{
		    t1=0.0;
		    t2=0.0;
		    t3=0.0;
		    t4=0.0;
		    t5=0.0;
		
		    Length=(i + 1) * LengthperSeg;
		
		    if(Length<=50.0)
			{
				m_V7=m_VF7;
			    t1=(50.0-Length)/m_VF7;
			    if((126.85+Length)<=150.0)
				{
				    t2=(-m_VF7+sqrt(m_VF7*m_VF7+2.0*m_Acc1*(126.85-(50.0-Length))))/m_Acc1;
				}
			    else if(150.0<(126.85+Length)&&(126.85+Length)<=m_Ldece)
				{
				    t2=(-m_VF7+sqrt(m_VF7*m_VF7+2.0*m_Acc1*(150.0-50.0)))/m_Acc1;
				    m_Vacc2=m_VF7+m_Acc1*t2;
				    t3=(-m_Vacc2+sqrt(m_Vacc2*m_Vacc2+2.0*m_Acc2*(126.85+Length-150.0)))/m_Acc2;
				    m_Vdec=m_Vacc2+m_Acc2*t3;
				    if(m_Vdec>m_Vmax)
					{
					    m_Vdec=m_Vmax;
					    t3=(m_Vmax-m_Vacc2)/m_Acc2+(126.85+Length-150.0-(m_Vmax*m_Vmax-m_Vacc2*m_Vacc2)/(2.0*m_Acc2))/m_Vmax;
					}
				}
			    else if(m_Ldece<(126.85+Length)&&(126.85+Length)<=(m_Ldece+30.0))
				{
                    t2=(-m_VF7+sqrt(m_VF7*m_VF7+2.0*m_Acc1*(150.0-50.0)))/m_Acc1;
				    m_Vacc2=m_VF7+m_Acc1*t2;
				    t3=(-m_Vacc2+sqrt(m_Vacc2*m_Vacc2+2.0*m_Acc2*(m_Ldece-150.0)))/m_Acc2;
				    m_Vdec=m_Vacc2+m_Acc2*t3;
				    if(m_Vdec>m_Vmax)
					{
					    m_Vdec=m_Vmax;
					    t3=(m_Vmax-m_Vacc2)/m_Acc2+(m_Ldece-150.0-(m_Vmax*m_Vmax-m_Vacc2*m_Vacc2)/(2.0*m_Acc2))/m_Vmax;
					}
				    m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
				    t4=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(126.85+Length-m_Ldece)))/m_Dece;
				}
			    else if((m_Ldece+30.0)<(126.85+Length))
				{
                    t2=(-m_VF7+sqrt(m_VF7*m_VF7+2.0*m_Acc1*(150.0-50.0)))/m_Acc1;
				    m_Vacc2=m_VF7+m_Acc1*t2;
				    t3=(-m_Vacc2+sqrt(m_Vacc2*m_Vacc2+2.0*m_Acc2*(m_Ldece-150.0)))/m_Acc2;
				    m_Vdec=m_Vacc2+m_Acc2*t3;
				    if(m_Vdec>m_Vmax)
					{
					    m_Vdec=m_Vmax;
					    t3=(m_Vmax-m_Vacc2)/m_Acc2+(m_Ldece-150.0-(m_Vmax*m_Vmax-m_Vacc2*m_Vacc2)/(2.0*m_Acc2))/m_Vmax;
					}
				    m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
				    t4=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(m_Ldece+30.0-m_Ldece)))/m_Dece;
				    t5=(126.85+Length-(m_Ldece+30.0))/m_Vout;
				}

			    pTime[i]=t1+t2+t3+t4+t5;
				pV7[i]=m_V7;
			}
		    if(50.0<Length&&Length<=150.0)
			{
			    m_V7=sqrt(m_VF7*m_VF7+2.0*m_Acc1*(Length-50.0));
			    t2=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Acc1*(150.0-Length)))/m_Acc1;
			    m_Vacc2=m_V7+m_Acc1*t2;
			    if((Length+126.85)<150.0)
				{
				    t2=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Acc1*(126.85+Length-Length)))/m_Acc1;
				    m_Vacc2=m_V7+m_Acc2*t2;
				    t3=0.0;
				    t4=0.0;
				    t5=0.0;
				}
			
			    if(150.0<(126.85+Length)&&(126.85+Length)<=m_Ldece)
				{
				    t3=(-m_Vacc2+sqrt(m_Vacc2*m_Vacc2+2.0*m_Acc2*(126.85+Length-150.0)))/m_Acc2;
			 	    m_Vdec=m_Vacc2+m_Acc2*t2;
				    if(m_Vdec>m_Vmax)
					{
					    m_Vdec=m_Vmax;
					    t3=(m_Vmax-m_Vacc2)/m_Acc2+(126.85+Length-150.0-(m_Vmax*m_Vmax-m_Vacc2*m_Vacc2)/(2.0*m_Acc2))/m_Vmax;
					}
				    t4=0.0;
				}
			    else if(m_Ldece<(126.85+Length)&&(126.85+Length)<=(m_Ldece+30.0))
				{
			        t3=(-m_Vacc2+sqrt(m_Vacc2*m_Vacc2+2.0*m_Acc2*(m_Ldece-150.0)))/m_Acc2;
				    m_Vdec=m_Vacc2+m_Acc2*t3;
                    if(m_Vdec>m_Vmax)
					{
					    m_Vdec=m_Vmax;
					    t3=(m_Vmax-m_Vacc2)/m_Acc2+(m_Ldece-150.0-(m_Vmax*m_Vmax-m_Vacc2*m_Vacc2)/(2.0*m_Acc2))/m_Vmax;
					}
				    m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
				    t4=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(126.85+Length-m_Ldece)))/m_Dece;
				}
			    else if((m_Ldece+30.0)<(126.85+Length))
				{
				    t3=(-m_Vacc2+sqrt(m_Vacc2*m_Vacc2+2.0*m_Acc2*(m_Ldece-150.0)))/m_Acc2;
				    m_Vdec=m_Vacc2+m_Acc2*t3;
				    if(m_Vdec>m_Vmax)
					{
				    	m_Vdec=m_Vmax;
					    t3=(m_Vmax-m_Vacc2)/m_Acc2+(m_Ldece-150.0-(m_Vmax*m_Vmax-m_Vacc2*m_Vacc2)/(2.0*m_Acc2))/m_Vmax;
					}
				    m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
				    t4=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(m_Ldece+30.0-m_Ldece)))/m_Dece;
				    t5=(126.85+Length-(m_Ldece+30.0))/m_Vout;
				}

			    pTime[i]=t1+t2+t3+t4+t5;
				pV7[i]=m_V7;
			}
		    if(150.0<Length&&Length<=m_Ldece)
			{
			    m_Vacc2=sqrt(m_VF7*m_VF7+2.0*m_Acc1*(150.0-50.0));
			    t30=(-m_Vacc2+sqrt(m_Vacc2*m_Vacc2+2.0*m_Acc2*(Length-150.0)))/m_Acc2;
			    m_V7=m_Vacc2+m_Acc2*t30;
			    if(m_V7>m_Vmax)
				{
				    m_V7=m_Vmax;
				}
			    if((126.85+Length)<m_Ldece)
				{
				    t3=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Acc2*(126.85+Length-Length)))/m_Acc2;
				    m_Vdec=m_V7+m_Acc2*t3;
                    if(m_Vdec>m_Vmax)
					{
					    m_Vdec=m_Vmax;
					    t3=(m_Vmax-m_V7)/m_Acc2+(126.85+Length-Length-(m_Vmax*m_Vmax-m_V7*m_V7)/(2.0*m_Acc2))/m_Vmax;
					}
				    t4=0.0;
					t5=0.0;
				}
			    else if(m_Ldece<(126.85+Length)&&(126.85+Length)<=(m_Ldece+30.0))
				{
			        t3=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Acc2*(m_Ldece-Length)))/m_Acc2;
			        m_Vdec=m_V7+m_Acc2*t3;
                    if(m_Vdec>m_Vmax)
					{
					    m_Vdec=m_Vmax;
					    t3=(m_Vmax-m_V7)/m_Acc2+(m_Ldece-Length-(m_Vmax*m_Vmax-m_V7*m_V7)/(2.0*m_Acc2))/m_Vmax;
					}
			        m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
				    t4=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(126.85+Length-m_Ldece)))/m_Dece;
					t5=0.0;
				}
			    else if((m_Ldece+30.0)<(126.85+Length))
				{
				    t3=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Acc2*(m_Ldece-Length)))/m_Acc2;
				    m_Vdec=m_V7+m_Acc2*t3;
				    if(m_Vdec>m_Vmax)
					{
					    m_Vdec=m_Vmax;
					    t3=(m_Vmax-m_V7)/m_Acc2+(m_Ldece-Length-(m_Vmax*m_Vmax-m_V7*m_V7)/(2.0*m_Acc2))/m_Vmax;
					}
				    m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
				    t4=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(m_Ldece+30.0-m_Ldece)))/m_Dece;
				    t5=(126.85+Length-(m_Ldece+30.0))/m_Vout;
				}
			    pTime[i]=t1+t2+t3+t4+t5;
				pV7[i]=m_V7;
			}
		    if(m_Ldece<Length&&Length<(m_Ldece+30.0))
			{
			    m_Vdec=sqrt(m_VF7*m_VF7+2.0*m_Acc1*(150.0-50.0)+2.0*m_Acc2*(m_Ldece-150.0));
			    if(m_Vdec>m_Vmax)
				{
				    m_Vdec=m_Vmax;
				}
			    m_Dece=(m_Vout*m_Vout-m_Vdec*m_Vdec)/(2.0*30.0);
			    t30=(-m_Vdec+sqrt(m_Vdec*m_Vdec+2.0*m_Dece*(Length-m_Ldece)))/m_Dece;
			    m_V7=m_Vdec+m_Dece*t30;
			    if((126.85+Length)<=(m_Ldece+30.0))
				{
				    t4=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Dece*(126.85+Length-Length)))/m_Dece;
				    t5=0.0;
				}
			    else if((m_Ldece+30.0)<(126.85+Length))
				{
				    t4=(-m_V7+sqrt(m_V7*m_V7+2.0*m_Dece*(m_Ldece+30.0-Length)))/m_Dece;
				    t5=(126.85+Length-(m_Ldece+30.0))/m_Vout;
				}

			    pTime[i]=t1+t2+t3+t4+t5;
				pV7[i]=m_V7;
			}
		    if((m_Ldece+30.0)<=Length)
			{
				m_V7=m_Vout;
			    t5=126.85/m_Vout;
		        pTime[i]=t1+t2+t3+t4+t5;
				pV7[i]=m_V7;
			}
		}
	}
	return 0;
}

extern "C" double PASCAL EXPORT
    CalBpElongation(double Xf, double Si, double Mn, double P, double S, 
	     double FDT, double CT, double H, double D)
{
	//C,H,Si,Mn,P,S,D,Sigmas,Sigmab
	double Delta;  //Elongation
	double Neuron[3][19];
	double W[3][19][19];
	double SumNet, Temp, out;
	int LayerSize[3];
	int i, j, k;

	H=H*1000.0;  //单位从m-mm转换
	D=D;         //单位不用从um-mm转换
	Temp=0.9;
	LayerSize[0]=9;
	LayerSize[1]=18;
	LayerSize[2]=1;
    for(i=0;i<3;i++)
	{
		for(j=0;j<19;j++)
		{
			Neuron[i][j]=0.0;
		}
	}

	Neuron[0][0]=H/20.0;
    Neuron[0][1]=Si*2.0;
	Neuron[0][2]=Mn/2.0;
    Neuron[0][3]=P*10.0;
	Neuron[0][4]=S*40.0;
    Neuron[0][5]=D/20.0;
	Neuron[0][6]=Xf;
    Neuron[0][7]=FDT/400.0;
	Neuron[0][8]=CT/500.0;

	ifstream InFile("DeltaIn.txt",ios::in|ios::nocreate);//从文件读入权值

    for(i=1;i<3;i++)
	{
		for(j=0;j<LayerSize[i];j++)
		{
			for(k=0;k<=LayerSize[i-1];k++)
			{
				W[i][k][j]=0.0;
			}
		}
	}

	for(i=1;i<3;i++)
	{
		for(j=0;j<LayerSize[i];j++)
		{
			for(k=0;k<=LayerSize[i-1];k++)
			{
				InFile>>W[i][k][j];
			}
		}
	}

	for(i=1;i<3;i++)
	{
		Neuron[i-1][LayerSize[i-1]]=1.0;
		for(j=0;j<LayerSize[i];j++)
		{
			SumNet=0.0;
			for(k=0;k<=LayerSize[i-1];k++)
			{
				SumNet=SumNet+Neuron[i-1][k] * W[i][k][j];
			}
			out=1.0/(1.0+exp(-SumNet/Temp));
			Neuron[i][j]=out;
		}
	}
	Delta=out*50.0;
	return Delta;
}

extern "C" int PASCAL EXPORT
    CalBpTyandTs(double Si, double Mn, double P, double S, double FDT, double CT,
	double D, double Xf, double *Tbands)
{
    //H, Si, Mn, P, S, D, Xf
	double Neuron[3][19];
	double W[3][19][19];
	double SumNet, Temp, out;
	int LayerSize[3];
	int i, j, k;

	Temp=0.9;
	LayerSize[0]=8;
	LayerSize[1]=18;
	LayerSize[2]=2;
    for(i=0;i<3;i++)
	{
		for(j=0;j<19;j++)
		{
			Neuron[i][j]=0.0;
		}
	}

	Neuron[0][0]=Si*2.0;
    Neuron[0][1]=Mn/2.0;
	Neuron[0][2]=P*10.0;
    Neuron[0][3]=S*40.0;
	Neuron[0][4]=D/20.0;
	Neuron[0][5]=Xf;
    Neuron[0][6]=FDT/1000.0;
	Neuron[0][7]=CT/800.0;

	ifstream InFile("TbandsIn.txt",ios::in|ios::nocreate);//从文件读入权值

    for(i=1;i<3;i++)
	{
		for(j=0;j<LayerSize[i];j++)
		{
			for(k=0;k<=LayerSize[i-1];k++)
			{
				W[i][k][j]=0.0;
			}
		}
	}

	for(i=1;i<3;i++)
	{
		for(j=0;j<LayerSize[i];j++)
		{
			for(k=0;k<=LayerSize[i-1];k++)
			{
				InFile>>W[i][k][j];
			}
		}
	}

	for(i=1;i<3;i++)
	{
		Neuron[i-1][LayerSize[i-1]]=1.0;
		for(j=0;j<LayerSize[i];j++)
		{
			SumNet=0.0;
			for(k=0;k<=LayerSize[i-1];k++)
			{
				SumNet=SumNet+Neuron[i-1][k] * W[i][k][j];
			}
			out=1.0/(1.0+exp(-SumNet/Temp));
			Neuron[i][j]=out;
			if(i==2)
			{
				Tbands[j]=out;
			}
		}
	}
		Tbands[0]=	Tbands[0]*400.0;
        Tbands[1]=	Tbands[1]*500.0;

	return 0;
}
