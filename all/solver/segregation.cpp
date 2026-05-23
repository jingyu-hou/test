// Macrosegregation.cpp: implementation of the CMacrosegregation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SS.h"
#include "ApiFileNameStr.h"
#include <math.h>
#include "Macrosegregation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMacrosegregation::CMacrosegregation()
{
	LT=0;
	MT=0;
	NT=0;
	L=0;
	M=0;
	N=0;
	DDL=0;
	DDM=0;
	DDN=0;
	DL=0;
	DM=0;
	DN=0;
	limitXY=0;
	limitX=0;
	limitY=0;
	limitZ=0;
	limitxy=0;
	limitx=0;
	limity=0;
	limitz=0;
	ii=0;
	m_place=0;
	m_spai=0;
	m_spaj=0;
	m_spak=0;
	
	Tem_coeff=0.0f;
	Con_coeff=0.0f;
	m_fcarbon=0.0f;
	m_ixielv=0.0f;
	m_fk=0.0f;
	TL=0.0f;
	TS=0.0f;
	DiffL=0.0f;
	DiffS=0.0f;
	fs_critical=0.0f;
	himax=0.0f;
	himin=0.0;
	Temp_c=0.0f;
	Temp_e=0.0f;
	
	DnstS=0.0f;
	
	SphtS=0.0f;
	L_heat=0.0f;

	dlt_tem=0.0f;
	dlt_flow=0.0f;
	Time_step=0.0f;
	Time_end=0.0f;

	for(int i=0;i<20;i++)
	{
		Dnst[i]=0.0f;
		Htcf[i]=0.0f;
		Spht[i]=0.0f;
		for(int j=0;j<10;j++)
		{
			Fhm[i][j]=0.0f;
		}
	}
	
	Ttold=NULL;
	Tt=NULL;
	TH=NULL;
	Att=NULL;
	
	
	Vv=NULL;
	Vu=NULL;
	Vw=NULL;
	Pp=NULL;
	
	subfsold=NULL;
	fsold=NULL;

	CL=NULL;
	Cmix=NULL;
	Cmixold=NULL;
	CLold=NULL;
	CCS=NULL;

	start_poro=NULL;
	cooling_rate=NULL;
	tem_grad=NULL;
	
	
}

CMacrosegregation::~CMacrosegregation()
{
	if(start_poro!=NULL)
	{
		free(start_poro);
		start_poro=NULL;
	}
	if(tem_grad!=NULL)
	{
		free(tem_grad);
		tem_grad=NULL;
	}
	if(cooling_rate!=NULL)
	{
		free(cooling_rate);
		cooling_rate=NULL;
	}

	if(Ttold!=NULL)
	{
		free(Ttold);
		Ttold=NULL;
	}

	if(Tt!=NULL)
	{
		free(Tt);
		Tt=NULL;
	}
	if(TH!=NULL)
	{
		free(TH);
		TH=NULL;
	}
	if(Att!=NULL)
	{
		free(Att);
		Att=NULL;
	}
	
	
	if(Pp!=NULL)
	{
		free(Pp);
		Pp=NULL;
	}
	if(Vv!=NULL)
	{
		free(Vv);
		Vv=NULL;
	}
	if(Vu!=NULL)
	{
		free(Vu);
		Vu=NULL;
	}
	if(Vw!=NULL)
	{
		free(Vw);
		Vw=NULL;
	}
	

	if(CL!=NULL)
	{
		free(CL);
		CL=NULL;
	}
	if(Cmix!=NULL)
	{
		free(Cmix);
		Cmix=NULL;
	}
	if(Cmixold!=NULL)
	{
		free(Cmixold);
		Cmixold=NULL;
	}

	if(CCS!=NULL)
	{
		free(CCS);
		CCS=NULL;
	}
	
	

	if(subfsold!=NULL)
	{
		free(subfsold);
		subfsold=NULL;
	}
	if(fsold!=NULL)
	{
		free(fsold);
		fsold=NULL;
	}
	
	if(CLold!=NULL)
	{
		free(CLold);
		CLold=NULL;
	}
}

void CMacrosegregation::ParameterAssign()
{
	/// inserting alloy rods: xiao sang experiment
	m_fcarbon=0.3;

	m_fk=0.169;
	m_ixielv=118.41;//84.53;
	TM=1530.5246;
	TL=TM-m_ixielv*m_fcarbon;
	TS=1438.0;//TM-m_ixielv*m_fcarbon1/m_fk;//1379.8f;
	CEE=0.6805;

	Tem_ref=TL;

	DiffL=0.00002f;
	DiffS=0.0000056f;
	DnstS=7.16;//7.79
	SphtS=0.715f;
	L_heat=309.0f;

	Tem_coeff=0.00006;
	Con_coeff=0.011;

	VsctyL=0.00805;

	fs_critical=0.7;
	fs_start=0.0f;
	fs_end=0.0f;
	
	
	T_ref_end=TS;
	Temp_c=TL+70.0;// TL+5.0
	Temp_mold=80.0;
	Temp_e=25.0f; //
	Space_step=2.0; //cm 
	Time_end=208605.0f; //s
	TIME=0.0f;


	ii=78;
	m_spai=78;
	m_place=0;
	
	dlt_flow=0.0;
	dlt_tem=0.0;
	Time_step=0.0;

	himax=30000.0;
	himin=0.008;//0.05;

///Fe-C// alloy
	Dnst[0]=6.91;
	Htcf[0]=0.29;
	Spht[0]=0.625;

	/// cast iron mold
	Dnst[2]=7.0;// density
	Htcf[2]=0.29;// conductivity
	Spht[2]=0.8;// specific heat

	//Dnst[2]=1.52;
	//Htcf[2]=0.008;
	//Spht[2]=1.07;

	///cold iron
	Dnst[4]=7.0;
	Htcf[4]=0.29;
	Spht[4]=0.8;


	/// insulating board top
	Dnst[6]=0.25;//
	Htcf[6]=0.002;//right
	Spht[6]=1.13;//

	/// 
	Dnst[7]=Dnst[6];
	Htcf[7]=Htcf[6];
	Spht[7]=Spht[6];
	
	/// insulating powder side
	Dnst[9]=0.21;//
	Htcf[9]=0.0018;//
	Spht[9]=0.4;//

	/// brick
	Dnst[8]=0.6;
	Htcf[8]=0.009;//right
	Spht[8]=1.13;
	

    //for multi-pouring
	Dnst[5]=0.0;
	Htcf[5]=0.0;
	Spht[5]=0.0;

	/// for Niyama criterion  from Beckermann paper
	Csdas=0.0144;//cm
	tem_critical=TS+0.1*(TL-TS);
	delt_P_critical=1.01;
    shrinkage_coeff=(DnstS-Dnst[0])/Dnst[0];

	allsuo=0.0;
	allmysuo=0.0;
	allsuo_solidk=0;
	solid_k=0;

	FSsuo=fs_critical;
	PIPEsuo=0.2;//0.3;//0.2
	Lsuo=0.0002;
	Ssuo=(DnstS-Dnst[0])/DnstS;

	
	pipe_hole=17;//air
	Dnst[pipe_hole]=Dnst[9];//0.0013;
	Htcf[pipe_hole]=Htcf[9];//0.0003;
	Spht[pipe_hole]=Spht[9];//1.1;

	shrinkage_hole=18;//air
	Dnst[shrinkage_hole]=Dnst[9];//0.0013;
	Htcf[shrinkage_hole]=Htcf[9];//0.0003;
	Spht[shrinkage_hole]=Spht[9];//1.1;
	

	////
	double hi1=Space_step*himax;//金属型和铸件的换热系数

	double hi86=Space_step*0.02;
	double hi89=Space_step*0.03;
	
//0
	Fhm[0][2]=1.0/((Htcf[0]+Htcf[2])/(2.0f*Htcf[0]*Htcf[2])+1.0f/hi1);
	Fhm[2][0]=1.0/((Htcf[0]+Htcf[2])/(2.0f*Htcf[0]*Htcf[2])+1.0f/hi1);

	Fhm[0][4]=1.0/((Htcf[0]+Htcf[4])/(2.0f*Htcf[0]*Htcf[4])+1.0f/hi1);
	Fhm[4][0]=1.0/((Htcf[0]+Htcf[4])/(2.0f*Htcf[0]*Htcf[4])+1.0f/hi1);

	//Fhm[0][6]=1.0/((Htcf[0]+Htcf[6])/(2.0f*Htcf[0]*Htcf[6])+1.0f/hi2);
	//Fhm[6][0]=1.0/((Htcf[0]+Htcf[6])/(2.0f*Htcf[0]*Htcf[6])+1.0f/hi2);
	Fhm[0][6]=1.0/((Htcf[0]+Htcf[6])/(2.0f*Htcf[0]*Htcf[6]));
	Fhm[6][0]=1.0/((Htcf[0]+Htcf[6])/(2.0f*Htcf[0]*Htcf[6]));
	
	Fhm[0][7]=1.0/((Htcf[0]+Htcf[7])/(2.0f*Htcf[0]*Htcf[7]));
	Fhm[7][0]=1.0/((Htcf[0]+Htcf[7])/(2.0f*Htcf[0]*Htcf[7]));

	Fhm[0][8]=1.0/((Htcf[0]+Htcf[8])/(2.0f*Htcf[0]*Htcf[8])+1.0f/hi86);
	Fhm[8][0]=1.0/((Htcf[0]+Htcf[8])/(2.0f*Htcf[0]*Htcf[8])+1.0f/hi86);

	Fhm[0][9]=1.0/((Htcf[0]+Htcf[9])/(2.0f*Htcf[0]*Htcf[9])+1.0f/hi86);
	Fhm[9][0]=1.0/((Htcf[0]+Htcf[9])/(2.0f*Htcf[0]*Htcf[9])+1.0f/hi86);

	Fhm[0][pipe_hole]=1.0/((Htcf[0]+Htcf[pipe_hole])/(2.0f*Htcf[0]*Htcf[pipe_hole]));
	Fhm[pipe_hole][0]=1.0/((Htcf[0]+Htcf[pipe_hole])/(2.0f*Htcf[0]*Htcf[pipe_hole]));

	Fhm[0][shrinkage_hole]=1.0/((Htcf[0]+Htcf[shrinkage_hole])/(2.0f*Htcf[0]*Htcf[shrinkage_hole]));
	Fhm[shrinkage_hole][0]=1.0/((Htcf[0]+Htcf[shrinkage_hole])/(2.0f*Htcf[0]*Htcf[shrinkage_hole]));
	

//2
	Fhm[2][4]=1.0/((Htcf[2]+Htcf[4])/(2.0f*Htcf[2]*Htcf[4])+1.0f/hi1);
	Fhm[4][2]=1.0/((Htcf[2]+Htcf[4])/(2.0f*Htcf[2]*Htcf[4])+1.0f/hi1);

	Fhm[2][6]=1.0/((Htcf[2]+Htcf[6])/(2.0f*Htcf[2]*Htcf[6])+1.0f/hi86);
	Fhm[6][2]=1.0/((Htcf[2]+Htcf[6])/(2.0f*Htcf[2]*Htcf[6])+1.0f/hi86);

	Fhm[2][7]=1.0/((Htcf[2]+Htcf[7])/(2.0f*Htcf[2]*Htcf[7])+1.0f/hi86);
	Fhm[7][2]=1.0/((Htcf[2]+Htcf[7])/(2.0f*Htcf[2]*Htcf[7])+1.0f/hi86);

	Fhm[2][8]=1.0/((Htcf[2]+Htcf[8])/(2.0f*Htcf[2]*Htcf[8])+1.0f/hi86);
	Fhm[8][2]=1.0/((Htcf[2]+Htcf[8])/(2.0f*Htcf[2]*Htcf[8])+1.0f/hi86);

	Fhm[2][9]=1.0/((Htcf[2]+Htcf[9])/(2.0f*Htcf[2]*Htcf[9]));
	Fhm[9][2]=1.0/((Htcf[2]+Htcf[9])/(2.0f*Htcf[2]*Htcf[9]));

	Fhm[2][pipe_hole]=1.0/((Htcf[2]+Htcf[pipe_hole])/(2.0f*Htcf[2]*Htcf[pipe_hole]));
	Fhm[pipe_hole][2]=1.0/((Htcf[2]+Htcf[pipe_hole])/(2.0f*Htcf[2]*Htcf[pipe_hole]));

	Fhm[2][shrinkage_hole]=1.0/((Htcf[2]+Htcf[shrinkage_hole])/(2.0f*Htcf[2]*Htcf[shrinkage_hole]));
	Fhm[shrinkage_hole][2]=1.0/((Htcf[2]+Htcf[shrinkage_hole])/(2.0f*Htcf[2]*Htcf[shrinkage_hole]));

//4
	Fhm[4][6]=1.0/((Htcf[4]+Htcf[6])/(2.0f*Htcf[4]*Htcf[6]));
	Fhm[6][4]=1.0/((Htcf[4]+Htcf[6])/(2.0f*Htcf[4]*Htcf[6]));

	Fhm[4][7]=1.0/((Htcf[4]+Htcf[7])/(2.0f*Htcf[4]*Htcf[7])+1.0f/hi86);
	Fhm[7][4]=1.0/((Htcf[4]+Htcf[7])/(2.0f*Htcf[4]*Htcf[7])+1.0f/hi86);

	Fhm[4][8]=1.0/((Htcf[4]+Htcf[8])/(2.0f*Htcf[4]*Htcf[8]));
	Fhm[8][4]=1.0/((Htcf[4]+Htcf[8])/(2.0f*Htcf[4]*Htcf[8]));

	Fhm[4][9]=1.0/((Htcf[4]+Htcf[9])/(2.0f*Htcf[4]*Htcf[9]));
	Fhm[9][4]=1.0/((Htcf[4]+Htcf[9])/(2.0f*Htcf[4]*Htcf[9]));

	Fhm[4][pipe_hole]=1.0/((Htcf[4]+Htcf[pipe_hole])/(2.0f*Htcf[4]*Htcf[pipe_hole]));
	Fhm[pipe_hole][4]=1.0/((Htcf[4]+Htcf[pipe_hole])/(2.0f*Htcf[4]*Htcf[pipe_hole]));

	Fhm[4][shrinkage_hole]=1.0/((Htcf[4]+Htcf[shrinkage_hole])/(2.0f*Htcf[4]*Htcf[shrinkage_hole]));
	Fhm[shrinkage_hole][4]=1.0/((Htcf[4]+Htcf[shrinkage_hole])/(2.0f*Htcf[4]*Htcf[shrinkage_hole]));

//6
	Fhm[6][7]=1.0/((Htcf[6]+Htcf[7])/(2.0f*Htcf[6]*Htcf[7]));
	Fhm[7][6]=1.0/((Htcf[6]+Htcf[7])/(2.0f*Htcf[6]*Htcf[7]));

	Fhm[6][8]=1.0/((Htcf[6]+Htcf[8])/(2.0f*Htcf[6]*Htcf[8])+1.0f/hi86);
	Fhm[8][6]=1.0/((Htcf[6]+Htcf[8])/(2.0f*Htcf[6]*Htcf[8])+1.0f/hi86);

	Fhm[6][9]=1.0/((Htcf[6]+Htcf[9])/(2.0f*Htcf[6]*Htcf[9]));
	Fhm[9][6]=1.0/((Htcf[6]+Htcf[9])/(2.0f*Htcf[6]*Htcf[9]));

	Fhm[6][pipe_hole]=1.0/((Htcf[6]+Htcf[pipe_hole])/(2.0f*Htcf[6]*Htcf[pipe_hole]));
	Fhm[pipe_hole][6]=1.0/((Htcf[6]+Htcf[pipe_hole])/(2.0f*Htcf[6]*Htcf[pipe_hole]));

	Fhm[6][shrinkage_hole]=1.0/((Htcf[6]+Htcf[shrinkage_hole])/(2.0f*Htcf[6]*Htcf[shrinkage_hole]));
	Fhm[shrinkage_hole][6]=1.0/((Htcf[6]+Htcf[shrinkage_hole])/(2.0f*Htcf[6]*Htcf[shrinkage_hole]));

//7
	Fhm[7][8]=1.0/((Htcf[8]+Htcf[7])/(2.0f*Htcf[8]*Htcf[7]));
	Fhm[8][7]=1.0/((Htcf[8]+Htcf[7])/(2.0f*Htcf[8]*Htcf[7]));

	Fhm[7][9]=1.0/((Htcf[9]+Htcf[7])/(2.0f*Htcf[9]*Htcf[7]));
	Fhm[9][7]=1.0/((Htcf[9]+Htcf[7])/(2.0f*Htcf[9]*Htcf[7]));

	Fhm[7][pipe_hole]=1.0/((Htcf[7]+Htcf[pipe_hole])/(2.0f*Htcf[7]*Htcf[pipe_hole]));
	Fhm[pipe_hole][7]=1.0/((Htcf[7]+Htcf[pipe_hole])/(2.0f*Htcf[7]*Htcf[pipe_hole]));

	Fhm[7][shrinkage_hole]=1.0/((Htcf[7]+Htcf[shrinkage_hole])/(2.0f*Htcf[7]*Htcf[shrinkage_hole]));
	Fhm[shrinkage_hole][7]=1.0/((Htcf[7]+Htcf[shrinkage_hole])/(2.0f*Htcf[7]*Htcf[shrinkage_hole]));

//8
	Fhm[9][8]=1.0/((Htcf[8]+Htcf[9])/(2.0f*Htcf[8]*Htcf[9])+1.0f/hi89);
	Fhm[8][9]=1.0/((Htcf[8]+Htcf[9])/(2.0f*Htcf[8]*Htcf[9])+1.0f/hi89);

	Fhm[8][pipe_hole]=1.0/((Htcf[8]+Htcf[pipe_hole])/(2.0f*Htcf[8]*Htcf[pipe_hole]));
	Fhm[pipe_hole][8]=1.0/((Htcf[8]+Htcf[pipe_hole])/(2.0f*Htcf[8]*Htcf[pipe_hole]));

	Fhm[8][shrinkage_hole]=1.0/((Htcf[8]+Htcf[shrinkage_hole])/(2.0f*Htcf[8]*Htcf[shrinkage_hole]));
	Fhm[shrinkage_hole][8]=1.0/((Htcf[8]+Htcf[shrinkage_hole])/(2.0f*Htcf[8]*Htcf[shrinkage_hole]));

//9
	Fhm[9][pipe_hole]=1.0/((Htcf[9]+Htcf[pipe_hole])/(2.0f*Htcf[9]*Htcf[pipe_hole]));
	Fhm[pipe_hole][9]=1.0/((Htcf[9]+Htcf[pipe_hole])/(2.0f*Htcf[9]*Htcf[pipe_hole]));

	Fhm[9][shrinkage_hole]=1.0/((Htcf[9]+Htcf[shrinkage_hole])/(2.0f*Htcf[9]*Htcf[shrinkage_hole]));
	Fhm[shrinkage_hole][9]=1.0/((Htcf[9]+Htcf[shrinkage_hole])/(2.0f*Htcf[9]*Htcf[shrinkage_hole]));

//pipe_hole
	Fhm[pipe_hole][shrinkage_hole]=1.0/((Htcf[pipe_hole]+Htcf[shrinkage_hole])/(2.0f*Htcf[pipe_hole]*Htcf[shrinkage_hole]));
	Fhm[shrinkage_hole][pipe_hole]=1.0/((Htcf[pipe_hole]+Htcf[shrinkage_hole])/(2.0f*Htcf[pipe_hole]*Htcf[shrinkage_hole]));

	Fhm[5][0]=0.0;
	Fhm[0][5]=0.0;

	Fhm[5][2]=0.0;
	Fhm[2][5]=0.0;

	Fhm[5][6]=0.0;
	Fhm[6][5]=0.0;

	Fhm[5][9]=0.0;
	Fhm[9][5]=0.0;
}

void CMacrosegregation::AllCalculation(CDC *pDC)
{// no calculation of flow field, using the modified Niyama criterion to determine the porosity
	
	float numtime1=5.0f;//5
	float numtime2=5.0f;//5
	
	unsigned int i=0;
	unsigned int j=0;
	unsigned int k=0;

	BOOL write=TRUE;
	
	ParameterAssign();
	
	dlt_tem=MinPara();

	PlaceInfo();// allocate memory and initiliaze the variables

	pDC->TextOut (580,10,"凝固时间：");
	
	
    /// saving the information for Commerical-Display Software

	unsigned int physicaldata=2;
	unsigned int baoliu=0;

	FILE*pfbi;
	if((pfbi=fopen("d:\\Ldr\\3D_ingot\\all_data.dat","wb"))==NULL)
	{
		AfxMessageBox("can not open pfbi");
		return;
	}
	
	fwrite(&L,sizeof(unsigned int),1,pfbi);
	fwrite(&N,sizeof(unsigned int),1,pfbi);
	fwrite(&M,sizeof(unsigned int),1,pfbi);
	fwrite(&physicaldata,sizeof(unsigned int),1,pfbi);
	fwrite(&Space_step,sizeof(double),1,pfbi);
	
	unsigned int validatt=0;
	for(i=0; i<L; i++)// chan, 网格属性
	{
		for(k=0; k<N; k++)
		{
			for(j=0; j<M; j++)
			{
				unsigned int chan1=(i+DDL)*MT*NT+(j+DDM)*NT+k+DDN;
				if(Att[chan1]==0)
				{
					validatt=validatt+1;
				}
						
			}//j
		}//k
	}//i

	fwrite(&validatt,sizeof(unsigned int),1,pfbi);

	fwrite(&baoliu,sizeof(unsigned int),1,pfbi);//1
	fwrite(&baoliu,sizeof(unsigned int),1,pfbi);//2
	fwrite(&baoliu,sizeof(unsigned int),1,pfbi);//3
	fwrite(&baoliu,sizeof(unsigned int),1,pfbi);//4
	fwrite(&baoliu,sizeof(unsigned int),1,pfbi);//5
	fwrite(&baoliu,sizeof(unsigned int),1,pfbi);//6
	fwrite(&baoliu,sizeof(unsigned int),1,pfbi);//7
	fwrite(&baoliu,sizeof(unsigned int),1,pfbi);//8
	fwrite(&baoliu,sizeof(unsigned int),1,pfbi);//9
	fwrite(&baoliu,sizeof(unsigned int),1,pfbi);//10
	fwrite(&baoliu,sizeof(unsigned int),1,pfbi);//11
	fwrite(&baoliu,sizeof(unsigned int),1,pfbi);//12
	fwrite(&baoliu,sizeof(unsigned int),1,pfbi);//13
	fwrite(&baoliu,sizeof(unsigned int),1,pfbi);//14
	fwrite(&baoliu,sizeof(unsigned int),1,pfbi);//15
	fwrite(&baoliu,sizeof(unsigned int),1,pfbi);//16
	fwrite(&baoliu,sizeof(unsigned int),1,pfbi);//17
	
	

	for(i=0; i<L; i++)// chan, 网格单元标号
	{
		for(k=0; k<N; k++)
		{
			for(j=0; j<M; j++)
			{
				unsigned int chan1=(i+DDL)*limitXY+(j+DDM)*limitX+k+DDN;
				unsigned int chan=i*limitxy+j*limitx+k;

				if(Att[chan1]==0)
				{
					fwrite(&chan,sizeof(unsigned int),1,pfbi);
				}
			}//j

		}//k
	}//i

		

	/// macrosegregation calculation
	while(TIME<=Time_end)
	{
		/// End calculation
			BOOL End_P=FALSE;

			for(i=0;i<LT;i++)
			{
				for(j=0;j<MT;j++)
				{
					for(k=0;k<NT;k++)
					{
						unsigned int chan=i*MT*NT+j*NT+k;
						unsigned int subchan=(i-DDL)*M*N+(j-DDM)*N+k-DDN;
						if(Att[chan]==0)
						{
							if(fsold[subchan]<1.0)
							{
								End_P=TRUE;
								break;
							}
							/*if(Tt[chan]>TS)
							{
								End_P=TRUE;
								break;
							}*/
						}
					}//K
					if(End_P==TRUE)
					{
						break;
					}
				}//J
				if(End_P==TRUE)
				{
					break;
				}
			}//I
			if(End_P==FALSE)
			{
				TIME=Time_end;
			}

		
		// time step calculation
		if(dlt_flow!=0.0)
		{
			if(dlt_flow<(0.5*dlt_tem))
			{
				Time_step=dlt_flow;
			}
			else
			{
				Time_step=(0.5*dlt_tem);
			}
		}
		else
		{
			Time_step=0.5*dlt_tem;//0.1*dlt_tem, 0.7*dlt_tem not OK due to un-convergence
		}	
		
		
		TIME=TIME+Time_step;
		CString str;
		str.Format("%f",TIME);
		pDC->TextOut(650,10,str);

		str.Format("%f",Time_step);
		pDC->TextOut(650,40,str);

		/// for major calculation
		Cal_Temp();
		Cal_FtoT();
		Cal_FtoCon();

		    
		/// adjust liquid concentration
		
			double all_con=0.0;
			double al_num=0.0;
			double l_num=0.0;
			double aver_con=0.0;

			for(i=0;i<L;i++)
			{
				for(j=0;j<M;j++)
				{
					for(k=0;k<N;k++)
					{
						unsigned int chan=i*M*N+j*N+k;
						unsigned int chan1=(i+DDL)*MT*NT+(j+DDM)*NT+(k+DDN);
						if(Att[chan1]==0)
						{
							all_con=all_con+(Cmix[chan]/Dnst[0]);
							al_num=al_num+1.0;
						
							if((CL[chan]!=0.0)&&(fsold[chan]<fs_critical)&&(Tt[chan1]>TS))
							{
								l_num=l_num+1.0;
							}
						}
					}//k
				}//j
			}//i

			if(al_num!=0.0)
			{
				aver_con=(all_con/al_num)-m_fcarbon;
			}
			
			if((aver_con!=0.0)&&(l_num>0.0))
			{
				double ini_con=0.0;
				
				ini_con=(all_con-al_num*m_fcarbon)/l_num;
			
				for(i=0;i<L;i++)
				{
					for(j=0;j<M;j++)
					{
						for(k=0;k<N;k++)
						{
							unsigned int chan=i*M*N+j*N+k;
							unsigned int chan1=(i+DDL)*MT*NT+(j+DDM)*NT+(k+DDN);
							if((Att[chan1]==0)&&(CL[chan]!=0.0)&&(fsold[chan]<fs_critical)&&(Tt[chan1]>TS))
							{
								double ccyy=(Cmix[chan]/Dnst[0]-ini_con-fsold[chan]*CCS[chan])/(1.0-fsold[chan]);
								
								if(aver_con>0.0)
								{		
									if(ccyy>=m_fcarbon)
									{
										Cmix[chan]=Cmix[chan]-ini_con*Dnst[0];
									}
		
								}
								else
								{	
									if(ccyy<CEE)
									{
										Cmix[chan]=Cmix[chan]-ini_con*Dnst[0];
									}
								}//aver_con<0
							}//if
							
						}//k
					}//j
				}//i
			}//aver_con!=0

			
			
			CString text2;
			text2.Format("aver_con=%f ",aver_con);
			pDC->TextOut(10,150,text2);

		
		/// calculate fs by quadratic equation
		for(i=0;i<LT;i++)
		{
			for(j=0;j<MT;j++)
			{
				for(k=0;k<NT;k++)
				{
					unsigned int chan=(i-DDL)*M*N+(j-DDM)*N+k-DDN;
					unsigned int chan1=(i)*MT*NT+(j)*NT+(k);
					
					if(Att[chan1]==0)
					{
						if(fsold[chan]==1.0)
						{
							Tt[chan1]=TH[chan1]/(Dnst[0]*Spht[0]);
						}
						else if(fsold[chan]<1.0)
						{
							double clin=((Cmix[chan]-Cmixold[chan])+Dnst[0]*(1.0-fsold[chan])*CL[chan])/(Dnst[0]*(1.0-fsold[chan]));
							double tmin=TM-m_ixielv*clin;
							
							if((TH[chan1]-Dnst[0]*Spht[0]*tmin)>=(Dnst[0]*L_heat))
							{	
								
								if(fsold[chan]!=0.0)
								{
									fsold[chan]=0.0;
								}
									
								if(clin>(Cmix[chan]/Dnst[0]))
								{// reduce error
									clin=Cmix[chan]/Dnst[0];
								}
								
								/*if(clin>CE_con)
								{
									clin=CE_con;
								}*/
								
								Tt[chan1]=(TH[chan1]-Dnst[0]*L_heat)/(Dnst[0]*Spht[0]);

								
								CL[chan]=Cmix[chan]/Dnst[0];
								CCS[chan]=0.0;
							}
							else if((TH[chan1]-Dnst[0]*Spht[0]*tmin)>(Dnst[0]*L_heat*(1.0-fsold[chan])))
							{// remelting 

								double peta=-1.0;

								double AA=Dnst[0]*Spht[0]*TM-TH[chan1];
								double EE=Dnst[0]*L_heat;
								double BB=Dnst[0]*Spht[0]*m_ixielv;
								double WW=(1.0-fsold[chan])*clin;
								double QQ=peta*m_fk*fsold[chan]*CL[chan];
								double PP=peta*m_fk*fsold[chan];

								double aa=EE-EE*m_fk;
								double bb=AA*m_fk-AA+EE*m_fk-2.0*EE+EE*m_fk*fsold[chan]+EE*PP;
								double cc=AA*(1.0-m_fk*fsold[chan]-PP)+EE*(1.0-m_fk*fsold[chan]-PP)-BB*WW+BB*QQ;
								
							
								double fsr2=0.0;
								double fsr=0.0;
								double gen=0.0;

								gen=bb*bb-4.0*aa*cc;
								if(gen>=0.0)
								{
									
									fsr2=(-bb-sqrt(gen))/(2.0*aa);

									if(fsr2>=0.0 && fsr2<=1.0)
									{
										fsr=fsr2;
									}
									if(fsr2>1.0)
									{
										fsr=1.0;
									}
									if(fsr2<0.0)
									{
										fsr=0.0;
									}
									
									if(fsr<1.0)
									{
											double clold=CL[chan];
											CL[chan]=((1.0-fsold[chan])*clin-peta*m_fk*fsold[chan]*clold)/((fsr-fsold[chan])*m_fk+(1.0-fsr)-peta*m_fk*fsold[chan]);

											if(CL[chan]<m_fcarbon)
											{
												CL[chan]=m_fcarbon;
												fsr=(fsold[chan]*m_fk+peta*m_fk*fsold[chan]-1.0+(1.0-fsold[chan])*clin-peta*m_fk*fsold[chan]*clold/CL[chan])/(m_fk-1.0);
											}

											if(fsr>1.0)
											{
												fsr=1.0;
											}
											if(fsr<0.0)
											{
												fsr=0.0;
											}

											if(fsr>0.99)
											{
												fsr=1.0;
											}
											
											//Tt[chan1]=(TH[chan1]-(1.0-fsr)*Dnst[0]*L_heat)/(Dnst[0]*Spht[0]);
											Tt[chan1]=TM-m_ixielv*CL[chan];
											
											fsold[chan]=fsr;	
									}
									else
									{
											Tt[chan1]=TH[chan1]/(Dnst[0]*Spht[0]);
											fsold[chan]=1.0;
									}
									if(((1.0-fsold[chan])*CL[chan])>(Cmix[chan]/Dnst[0]))
									{// reduce error
											Cmix[chan]=Dnst[0]*((1.0-fsold[chan])*CL[chan]+fsold[chan]*CCS[chan]);
									}
								}//gen
								
							}
							else// for solidification
							{
									double peta=-1.0;

									double AA=Dnst[0]*Spht[0]*TM-TH[chan1];
									double EE=Dnst[0]*L_heat;
									double BB=Dnst[0]*Spht[0]*m_ixielv;
									double WW=(1.0-fsold[chan])*clin;
									double QQ=peta*m_fk*fsold[chan]*CL[chan];
									double PP=peta*m_fk*fsold[chan];

									double aa=EE-EE*m_fk;
									double bb=AA*m_fk-AA+EE*m_fk-2.0*EE+EE*m_fk*fsold[chan]+EE*PP;
									double cc=AA*(1.0-m_fk*fsold[chan]-PP)+EE*(1.0-m_fk*fsold[chan]-PP)-BB*WW+BB*QQ;
									
									double fsr2=0.0;
									double fsr=0.0;
									double gen=0.0;

									gen=bb*bb-4.0*aa*cc;
									
									if(gen>=0.0)
									{
										fsr2=(-bb-sqrt(gen))/(2.0*aa);

										if(fsr2>=0.0 && fsr2<=1.0)
										{
											fsr=fsr2;
										}
										if(fsr2>1.0)
										{
											fsr=1.0;
										}
										if(fsr2<0.0)
										{
											fsr=0.0;
										}
										
									
										if(fsr<1.0)
										{
											double clold=CL[chan];
											CL[chan]=((1.0-fsold[chan])*clin-peta*m_fk*fsold[chan]*clold)/((fsr-fsold[chan])*m_fk+(1.0-fsr)-peta*m_fk*fsold[chan]);
										
											/*if(CL[chan]>CE_con)
											{
												CL[chan]=CE_con;
												fsr=(fsold[chan]*m_fk+peta*m_fk*fsold[chan]-1.0+(1.0-fsold[chan])*clin-peta*m_fk*fsold[chan]*clold/CL[chan])/(m_fk-1.0);
											}*/

											if(CL[chan]<m_fcarbon)
											{
												CL[chan]=m_fcarbon;
												fsr=(fsold[chan]*m_fk+peta*m_fk*fsold[chan]-1.0+(1.0-fsold[chan])*clin-peta*m_fk*fsold[chan]*clold/CL[chan])/(m_fk-1.0);
											}
												

											if(fsr>1.0)
											{
												fsr=1.0;
											}
											if(fsr<0.0)
											{
												fsr=0.0;
											}

											if(fsr>0.99)
											{
												fsr=1.0;
											}
										
											//Tt[chan1]=(TH[chan1]-(1.0-fsr)*Dnst[0]*L_heat)/(Dnst[0]*Spht[0]);
											Tt[chan1]=TM-m_ixielv*CL[chan];
										
											fsold[chan]=fsr;	
										
										}
										else
										{
											Tt[chan1]=TH[chan1]/(Dnst[0]*Spht[0]);
											fsold[chan]=1.0;
										}
										if(((1.0-fsold[chan])*CL[chan])>(Cmix[chan]/Dnst[0]))
										{// reduce error
											Cmix[chan]=Dnst[0]*((1.0-fsold[chan])*CL[chan]+fsold[chan]*CCS[chan]);
										}
									}//gen
								
								}//for solidification
							}// if T>TS
						}
						else
						{
							if((Att[chan1]!=5)&&(Att[chan1]!=-2))//for mould
							{
								Tt[chan1]=TH[chan1]/(Dnst[Att[chan1]]*Spht[Att[chan1]]);
							}
						}//Att!=0
				}//k
			}//j
		}//i
		

		for(i=0;i<LT;i++)
		{
			for(j=0;j<MT;j++)
			{
				for(k=0;k<NT;k++)
				{
					unsigned int chan=(i-DDL)*M*N+(j-DDM)*N+k-DDN;
					unsigned int chan1=(i)*MT*NT+(j)*NT+(k);
					if(Att[chan1]==0)
					{
						if(fsold[chan]!=0.0)
						{
							CCS[chan]=(Cmix[chan]/Dnst[0]-(1.0-fsold[chan])*CL[chan])/fsold[chan];
							
							if(CCS[chan]<0.0)
							{
								CCS[chan]=0.0;
								fsold[chan]=0.0;
								CL[chan]=Cmix[chan]/Dnst[0];
								Tt[chan1]=(TH[chan1]-Dnst[0]*L_heat)/(Dnst[0]*Spht[0]);	
							}
						}	
						else
						{
							CCS[chan]=0.0;
						}
						
						Cmixold[chan]=Cmix[chan];
					}//Att
				}//k
			}//j
		}//i

		Cal_Flow(pDC);	/// calculate momentum equation (TtoF, Flow calculation

		/// for Niyama criterion, to calculate G and Cooling-rate at tem_critical

		for(i=1;i<(L-1);i++)
		{
			for(j=1;j<(M-1);j++)
			{
				for(k=1;k<(N-1);k++)
				{
					unsigned int chan=i*M*N+j*N+k;
					unsigned int chan1=(i+DDL)*MT*NT+(j+DDM)*NT+(k+DDN);

					unsigned int chan_left1=(i+DDL)*MT*NT+(j+DDM-1)*NT+(k+DDN);
					unsigned int chan_right1=(i+DDL)*MT*NT+(j+DDM+1)*NT+(k+DDN);
					unsigned int chan_up1=(i+DDL)*MT*NT+(j+DDM)*NT+(k+DDN+1);
					unsigned int chan_down1=(i+DDL)*MT*NT+(j+DDM)*NT+(k+DDN-1);
					unsigned int chan_front1=(i+DDL+1)*MT*NT+(j+DDM)*NT+(k+DDN);
					unsigned int chan_back1=(i+DDL-1)*MT*NT+(j+DDM)*NT+(k+DDN);


					if((Att[chan1]==0)&&(Att[chan_left1]==0)&&(Att[chan_right1]==0)&&(Att[chan_up1]==0)&&(Att[chan_down1]==0)&&(Att[chan_front1]==0)&&(Att[chan_back1]==0))
					{
						if(start_poro[chan]==0)
						{
							if(Tt[chan1]<=tem_critical)
							{
								double coolingrate=(Ttold[chan1]-Tt[chan1])/Time_step;

								double grad[6];
								grad[0]=(Tt[chan_left1]-Tt[chan1])/Space_step;
								grad[1]=(Tt[chan_right1]-Tt[chan1])/Space_step;
								grad[2]=(Tt[chan_up1]-Tt[chan1])/Space_step;
								grad[3]=(Tt[chan_down1]-Tt[chan1])/Space_step;
								grad[4]=(Tt[chan_front1]-Tt[chan1])/Space_step;
								grad[5]=(Tt[chan_back1]-Tt[chan1])/Space_step;

								double max_grad=grad[0];

								for(int gradi=0;gradi<=5;gradi++)
								{
									if(grad[gradi]>max_grad)
									{
										max_grad=grad[gradi];
									}		
								}

								if(coolingrate>0.0)
								{
									cooling_rate[chan]=coolingrate;
									tem_grad[chan]=max_grad;
									
									start_poro[chan]=1;
								}
							}
						}
					}//Att
				}//k
			}//j
		}//i

		/// calculate heat-transfer-coefficient
		Heat_Transfer_Coefficient();

		// calculate shrinkage from the free-surface of top
		//Shrink (pDC);

		/// exchange tem and subfsold

		for(i=0;i<LT;i++)// for air layer
		{
			for(j=0;j<MT;j++)
			{
				for(k=0;k<NT;k++)
				{
					unsigned int chan=i*MT*NT+j*NT+k;
					unsigned int subchan=(i-DDL)*M*N+(j-DDM)*N+k-DDN;
					if(Att[chan]==0)
					{
						Ttold[chan]=Tt[chan];
						subfsold[subchan]=fsold[subchan];
					}
				
				}
			}
		}
	
		/// save result files

		int timesave1=(int)(TIME/numtime1);// in all_data file, for tianjing university display
		if(timesave1==1)
		{
			fwrite(&TIME,sizeof(double),1,pfbi);
			
			
			for(i=0; i<limitz; i++)// temperature field
			{
				for(k=0; k<limitx; k++)
				{
					for(j=0; j<limity; j++)
					{
						unsigned int chan1=(i+DDL)*limitXY+(j+DDM)*limitX+k+DDN;
						
						if(Att[chan1]==0)
						{
							fwrite(&Tt[chan1],sizeof(double),1,pfbi);
						}
					}//j

				}//k
			}//i


			for(i=0; i<limitz; i++)//  Vu
			{
				for(k=0; k<limitx; k++)
				{
					for(j=0; j<limity; j++)
					{
						unsigned int chan1=(i+DDL)*limitXY+(j+DDM)*limitX+k+DDN;
						unsigned int subchan=i*limitxy+j*limitx+k;//(i)*M*N+(j)*N+k;
						
						if(Att[chan1]==0)
						{
							fwrite(&Vu[subchan],sizeof(double),1,pfbi);
						}
					}//j

				}//k
			}//i

			for(i=0; i<limitz; i++)//  Vv
			{
				for(k=0; k<limitx; k++)
				{
					for(j=0; j<limity; j++)
					{
						unsigned int chan1=(i+DDL)*limitXY+(j+DDM)*limitX+k+DDN;
						unsigned int subchan=i*limitxy+j*limitx+k;//(i)*M*N+(j)*N+k;
						
						if(Att[chan1]==0)
						{
							fwrite(&Vv[subchan],sizeof(double),1,pfbi);
						}
					}//j

				}//k
			}//i

			for(i=0; i<limitz; i++)//  Vu
			{
				for(k=0; k<limitx; k++)
				{
					for(j=0; j<limity; j++)
					{
						unsigned int chan1=(i+DDL)*limitXY+(j+DDM)*limitX+k+DDN;
						unsigned int subchan=i*limitxy+j*limitx+k;//(i)*M*N+(j)*N+k;
						
						if(Att[chan1]==0)
						{
							fwrite(&Vw[subchan],sizeof(double),1,pfbi);
						}
					}//j

				}//k
			}//i
			
			numtime1=numtime1+5000.0f;//50
		}

		/*int timesave2=(int)(TIME/numtime2);
		if(timesave2==1)
		{
			FILE*myfp;
	
			char tubf[10];
			sprintf(tubf,"%d", (int)(numtime2));

			CString str1="d:\\Ldr\\3D_ingot\\fsmat";
			
			str1=str1+tubf+_T(".dat");
			
			myfp=fopen(str1,"w+");

			fprintf(myfp,"%f ",allmysuo);	
			fprintf(myfp,"\n");
	
			// for matlab display

			//for(i=0; i<LT; i++)
			//{
				for(j=0; j<MT; j++)
				{
					for(k=0; k<NT; k++)// for ingot
					{
						unsigned int chan1=ii*MT*NT+j*NT+k;
						unsigned int subchan=(ii-DDL)*M*N+(j-DDM)*N+k-DDN;
						if(Att[chan1]==0 || Att[chan1]>=10)
						{
							fprintf(myfp,"%f ",subfsold[subchan]);	
						
						}
						else
						{
							fprintf(myfp,"%f ",0.0);
						}
					
					}//k
					fprintf(myfp,"\n");
						
				}//j
				//fprintf(myfp,"\n");
			//}
			
			fclose(myfp);
			numtime2=numtime2+5000.0f;//50
		}	

		*/
	}// end calculation

	FILE*pfniya1;
	FILE*pfniya2;
	FILE*pfniya3;
	FILE*pfniya4;
	FILE*pfniya5;
	FILE*pfshrink;
	FILE*pffs;
	FILE*allmy;

	if((pfniya1=fopen("d:\\Ldr\\3D_ingot\\temgrad.dat","w+"))==NULL)
	{
		AfxMessageBox("can not open tem_grad");
		return;
	}
	if((pfniya2=fopen("d:\\Ldr\\3D_ingot\\coolingrate.dat","w+"))==NULL)
	{
		AfxMessageBox("can not open cooling_rate");
		return;
	}
	if((pfniya3=fopen("d:\\Ldr\\3D_ingot\\sdas.dat","w+"))==NULL)
	{
		AfxMessageBox("can not open sdas");
		return;
	}
	if((pfniya4=fopen("d:\\Ldr\\3D_ingot\\fporosity.dat","w+"))==NULL)
	{
		AfxMessageBox("can not open f_porosity");
		return;
	}
	if((pfniya5=fopen("d:\\Ldr\\3D_ingot\\Niyama.dat","w+"))==NULL)
	{
		AfxMessageBox("can not open I_fcritical");
		return;
	}
	if((pfshrink=fopen("d:\\Ldr\\3D_ingot\\shrink_hole.dat","w+"))==NULL)
	{
		AfxMessageBox("can not open shrink_hole");
		return;
	}
	if((pffs=fopen("d:\\Ldr\\3D_ingot\\final_fs.dat","w+"))==NULL)
	{
		AfxMessageBox("can not open final_fs");
		return;
	}
	if((allmy=fopen("d:\\Ldr\\3D_ingot\\allmysuo.dat","w+"))==NULL)
	{
		AfxMessageBox("can not open allmysuo");
		return;
	}
	
	/// output Niyama information
		//for(i=0;i<LT;i++)
		//{
			for(j=0;j<MT;j++)
			{
				for(k=0;k<NT;k++)
				{
					unsigned int chan=(ii-DDL)*M*N+(j-DDM)*N+k-DDN;
					unsigned int chan1=ii*MT*NT+j*NT+k;

					if(Att[chan1]==0 || Att[chan1]>=10)
					{
						fprintf(pffs,"%f ",subfsold[chan]);	
						
					}
					else
					{
						fprintf(pffs,"%f ",0.0);
					}

					if(Att[chan1]==0)
					{
						double sdas=0.0;
						double Niyama=0.0;
						double fporo=0.0;
						double I_fcritical=0.0;

						if(start_poro[chan]==1)
						{
							sdas=Csdas*pow(cooling_rate[chan],(-1.0/3.0));//cm

							Niyama=tem_grad[chan]*sdas*sqrt(delt_P_critical)/sqrt(VsctyL*Dnst[0]*100.0*shrinkage_coeff*(TL-TS)*cooling_rate[chan]);

							double paraA=0.0;
							double paraB=0.0;
							double paraC=0.0;
							double paraD=0.0;

							if(Niyama<=28.2)
							{
								paraA=1.654;
								paraB=3.052;
								paraC=0.0;
								paraD=0.0;
							}
							else
							{
								paraA=0.0;
								paraB=0.0;
								paraC=43.05;
								paraD=1.254;
							}

							I_fcritical=-paraA*log10(Niyama)+paraB+paraC*pow(Niyama,-paraD);

							if(100.0*(DnstS-Dnst[0])/DnstS<I_fcritical)
							{
								fporo=100.0*(DnstS-Dnst[0])/DnstS;
							}
							else
							{
								fporo=I_fcritical;
							}
							
						}
						else
						{
							sdas=-1.0;
							fporo=-1.0;
							Niyama=-1.0;
						}

						fprintf(pfniya1,"%f ",tem_grad[chan]);
						fprintf(pfniya2,"%f ",cooling_rate[chan]);
						fprintf(pfniya3,"%f ",sdas);
						fprintf(pfniya4,"%f ",fporo);
						fprintf(pfniya5,"%f ",Niyama);
					}
					else
					{
						fprintf(pfniya1,"%f ",-2.0);
						fprintf(pfniya2,"%f ",-2.0);
						fprintf(pfniya3,"%f ",-2.0);
						fprintf(pfniya4,"%f ",-2.0);
						fprintf(pfniya5,"%f ",-2.0);
					}//Att

					fprintf(pfshrink,"%d ",Att[chan1]);
					
				}//k
				fprintf(pfniya1,"\n");
				fprintf(pfniya2,"\n");
				fprintf(pfniya3,"\n");
				fprintf(pfniya4,"\n");
				fprintf(pfniya5,"\n");
				fprintf(pfshrink,"\n");
				fprintf(pffs,"\n");
			}//j
		
			//fprintf(pfniya1,"\n");
			//fprintf(pfniya2,"\n");
			//fprintf(pfniya3,"\n");
			//fprintf(pfniya4,"\n");
			//fprintf(pfniya5,"\n");
			//fprintf(pfshrink,"\n");
		//}//i

		fprintf(allmy,"%f ",allmysuo);

		fclose(pfniya1);
		fclose(pfniya2);
		fclose(pfniya3);
		fclose(pfniya4);
		fclose(pfniya5);
		fclose(pfshrink);
		fclose(pffs);
		fclose(allmy);

}

void CMacrosegregation::Cal_Temp()
{
		unsigned int i=0;
		unsigned int j=0;
		unsigned int k=0;

		double change2=Time_step*Space_step;
		double change3=Time_step*Space_step*Space_step;

		for(i=0;i<LT;i++)// for air layer
		{
			for(j=0;j<MT;j++)
			{
				for(k=0;k<NT;k++)
				{
					unsigned int chan=i*MT*NT+j*NT+k;
					if(Att[chan]==-2)
					{
						Tt[chan]=Temp_e;
					}
					
				}
			}
		}

		// heat transfer calculation
		for(i=1;i<(LT-1);i++)
		{
			for(j=1;j<(MT-1);j++)
			{
				for(k=1;k<(NT-1);k++)
				{
					unsigned int chan=i*MT*NT+j*NT+k;
					unsigned int chan1=(i+1)*MT*NT+j*NT+k;//front
					unsigned int chan2=(i-1)*MT*NT+j*NT+k;//back
					unsigned int chan3=i*MT*NT+(j+1)*NT+k;//right
					unsigned int chan4=i*MT*NT+(j-1)*NT+k;//left
					unsigned int chan5=i*MT*NT+j*NT+k+1;//up
					unsigned int chan6=i*MT*NT+j*NT+k-1;//down

					double lbd=0.0f;
					double qs=0;
					double q[6];
					char wc[6];
					double wi[6];
					char cc=Att[chan];
					double ci=Tt[chan];
					
				    if(cc!=-2 && cc!=pipe_hole && cc!=shrinkage_hole)
					{
						for(int x=0;x<6;x++)
						{
							q[x]=0;
							wi[x]=20.0;
							wc[x]=-2;
						}

						wi[0]=Tt[chan1];
						wi[1]=Tt[chan2];
						wi[2]=Tt[chan3];
						wi[3]=Tt[chan4];
						wi[4]=Tt[chan5];
						wi[5]=Tt[chan6];

						wc[0]=Att[chan1];
						wc[1]=Att[chan2];
						wc[2]=Att[chan3];
						wc[3]=Att[chan4];
						wc[4]=Att[chan5];
						wc[5]=Att[chan6];
					
						for(int i1=0;i1<6;i1++)
						{
							if(wc[i1]==(-2) || wc[i1]==pipe_hole || wc[i1]==shrinkage_hole )
							{
								if(cc==0)
								{//casting~air
									if(wc[i1]==(-2))
									{
										double ht1=(double)(pow((wi[i1]+273.0)/1000.0,2)+pow((ci+273.0)/1000.0,2));
										double ht2=(wi[i1]+273.0)/1000.0+(ci+273.0)/1000.0;
										double hr=0.45*0.00567*ht1*ht2;//0.45f*0.00567f*ht1*ht2;
										q[i1]=hr*(wi[i1]-ci)*change3;
									}
									else
									{
										q[i1]=0.0;
									}
								}
								else 
								{// mold~air

									 if(cc==2)//mold
									 {
										if(wc[i1]==(-2))
										{
											double th1=(double)pow((ci+273.0)/1000.0,3);
											double qh=0.9*4.0*0.00567*th1;
											q[i1]=(0.00075+qh)*(wi[i1]-ci)*change3;
										 }
										 else
										 {
											 q[i1]=0.0;
											 
										 }
									 }
									 if((cc==6)||(cc==9)||(cc==8)|| cc==4 || cc==7 )// refractory ||cc==pipe_hole || cc==shrinkage_hole
									 {
										if(wc[i1]==(-2))
										{
									   		double th1=(double)pow((ci+273.0)/1000.0,3);
											double qh=0.5*4.0*0.00567*th1;
											q[i1]=(0.0005+qh)*(wi[i1]-ci)*change3;
										}
										else
										{
											q[i1]=0.0;
										}
									 }
								  
							   }
							}
							else
							{
								if(wc[i1]!=cc)
								{// mold~casting, diffrent moulds
									
									lbd=Fhm[cc][wc[i1]];//Fhm界面热阻
									q[i1]=lbd*(wi[i1]-ci)*change2;//ci
								}
								else
								{//casting~casting  or mold~mold
									
									lbd=Htcf[cc];
									q[i1]=lbd*(wi[i1]-ci)*change2;
								}
							}
							qs=qs+q[i1];
						}//i1

						if(qs==0.000) 
						{
							TH[chan]=TH[chan];
						}
						else
						{
							double volume=Space_step*Space_step*Space_step;
							TH[chan]=qs/volume+TH[chan];
						}
					}//cc!=-2
						
					}//k
				}//j
			}//i
}

void CMacrosegregation::Cal_FtoT()
{
		unsigned int i=0;
		unsigned int j=0;
		unsigned int k=0;
		
		double f1=0.0;
		double f2=0.0;
		double f3=0.0;
		double f4=0.0;
		double f5=0.0;
		double f6=0.0;
		double fst1=0.0;
		
		double three=(double)(Time_step/Space_step);

	
		/// effect of flow on tem
		for(i=0;i<L;i++)
		{
			for(j=0;j<M;j++)
			{
				for(k=0;k<N;k++)
				{
					unsigned int chan1=i*M*N+j*N+k;
					unsigned int chan=(i+DDL)*MT*NT+(j+DDM)*NT+k+DDN;
				
						if((Att[chan]==0)&&(fsold[chan1]<fs_critical)&&(Tt[chan]>TS)) //no effect, (1.0f-fs_end)
						{	
							  fst1=0.0; 

							  f1=0.0; 
							  f2=0.0; 
							  f3=0.0; 
							  f4=0.0;
							  f5=0.0;
							  f6=0.0;


							   if(Vu[chan1]<0.0)
							   {
								 f1=(0.0-Vu[chan1])*three; 
								 fst1=fst1+f1;
							   }

							   if(Vu[chan1-M*N]>0.0)
							   {
								 f2=Vu[chan1-M*N]*three;	
								 fst1=fst1+f2;
							   }
							   if(Vv[chan1]<0.0)
							   {
								 f3=(0.0-Vv[chan1])*three; 
								 fst1=fst1+f3;
							   }
							   if(Vv[chan1-N]>0.0)
							   {
								 f4=Vv[chan1-N]*three;	
								 fst1=fst1+f4;
							   }
							   if(Vw[chan1]<0.0)
							   {
								 f5=(0.0-Vw[chan1])*three; 
								 fst1=fst1+f5;
							   }
							   if(Vw[chan1-1]>0.0)
							   {
								 f6=Vw[chan1-1]*three;	
								 fst1=fst1+f6;
							   }

							   if(fst1>1.0f)
							   {
								   fst1=1.0f;
							   }
						
							    double tem1=Tt[chan];
								double tem2=Tt[chan+MT*NT];
								double tem3=Tt[chan-MT*NT];
								double tem4=Tt[chan+NT];
								double tem5=Tt[chan-NT];
								double tem6=Tt[chan+1];
								double tem7=Tt[chan-1];
							   
							   TH[chan]=TH[chan]+Dnst[0]*Spht[0]*(f1*tem2+f2*tem3+f3*tem4+f4*tem5+f5*tem6+f6*tem7-fst1*tem1)+Dnst[0]*L_heat*(f1+f2+f3+f4+f5+f6-fst1);	
							  
						}//if

				  }//j
			  }//j
		}//i

}

void CMacrosegregation::Cal_FtoCon()
{
		unsigned int i=0;
		unsigned int j=0;
		unsigned int k=0;

		double f1=0.0;
		double f2=0.0;
		double f3=0.0;
		double f4=0.0;
		double f5=0.0;
		double f6=0.0;

		double fst1=0.0;
		double three=Time_step/Space_step;
		
		for(i=0;i<L;i++)// for liquid
		{
			for(j=0;j<M;j++)
			{
				for(k=0;k<N;k++)
				{
					unsigned int chan1=i*M*N+j*N+k;
					unsigned int chan=(i+DDL)*MT*NT+(j+DDM)*NT+k+DDN;

					if((Att[chan]==0)&&(Tt[chan]>TS))
					{
						if(fsold[chan1]<fs_critical)
						{
							  fst1=0.0; 

							  f1=0.0; 
							  f2=0.0; 
							  f3=0.0; 
							  f4=0.0;
							  f5=0.0;
							  f6=0.0;

							  if(Vu[chan1]<0.0f)
							  {
								  f1=(0.0f-Vu[chan1])*three;
								  fst1=fst1+f1;
							  }
							  
							  if(Vu[chan1-M*N]>0.0f)
							  {
								  f2=(Vu[chan1-M*N]*three);
								  fst1=fst1+f2;
							  }
							  
							  if(Vv[chan1]<0.0)
							  {
								  f3=(0.0-Vv[chan1])*three; 
								  fst1=fst1+f3;
							  }
							  if(Vv[chan1-N]>0.0)
							  {
								  f4=Vv[chan1-N]*three;	
								  fst1=fst1+f4;
							  }
							   
							  if(Vw[chan1]<0.0)
							  {
								  f5=(0.0-Vw[chan1])*three; 
								  fst1=fst1+f5;
							  }
							  if(Vw[chan1-1]>0.0)
							  {
								 f6=Vw[chan1-1]*three;	
								 fst1=fst1+f6;
							  }

							   if(fst1>1.0f)
							   {
								   fst1=1.0f;
							   }
							   
							   double con1=CL[chan1];
							   double con2=CL[chan1+M*N];
							   double con3=CL[chan1-M*N];
							   double con4=CL[chan1+N];
							   double con5=CL[chan1-N];
							   double con6=CL[chan1+1];
							   double con7=CL[chan1-1];
							
							   double ccm=Cmix[chan1];
							   Cmix[chan1]=Cmix[chan1]+Dnst[0]*(f1*con2+f2*con3+f3*con4+f4*con5+f5*con6+f6*con7-fst1*con1);
							   
						}
					}//Att
				}//k
			}//j
		}//i

		

}

void CMacrosegregation::WriteSpecial(CString str)
{
	    FILE*pf;
		FILE*pf1;

		char atmp;
		int w=0;
		float ttmp=0.0f;
        unsigned int i,j,k;
		unsigned short int yh[111];


		CString strPath=str;
		CString Prefix;
		Prefix=GetFileNameByPath(strPath);
		Prefix=GetFileTitleByName(Prefix);
		Prefix=GetFilePrefixByTitle(Prefix);
		CString namepdt=Prefix+_T(".PDT");//热物性文件(.pdt)名

		if(w==0)
		{
			if((pf=fopen(str,"rb"))==NULL)
			{
				 AfxMessageBox("can not open sgn file");
				 return;
			}
			fread(yh,sizeof(short int),111,pf);

               LT=yh[0];
               MT=yh[1];
               NT=yh[2];
			  

			   if(Att==NULL)
			   {//铸件属性
				   Att=(char*)malloc(MT*NT*LT*sizeof(char));
			   }
			   if(Att==NULL)
			   {
				   AfxMessageBox("allocate memory for *.sgn file");
				   return;
			   }
			   
			   for(i=0;i<LT;i++)
			   {
				   for(j=0;j<MT;j++)
				   {
					  for(k=0;k<NT;k++)
					  {
	                      Att[i*MT*NT+j*NT+k]=-2;
					  }
					} 
			   }
				
			   for(i=0;i<LT;i++)
			   {
				   for(j=0;j<MT;j++)
				   {
					  for(k=0;k<NT;k++)
					  {
	                      fread(&atmp,sizeof(char),1,pf);
	                      Att[i*MT*NT+j*NT+k]=atmp;
					  }
					} 
			   }
			   w=w+1;
			   fclose(pf);

		}

		if((pf1=fopen(str,"wb"))==NULL)
		{
			AfxMessageBox("can not write complex sgn file");
			return;
		}
		fwrite(yh,sizeof(short int),111,pf1);

		 for(i=0;i<LT;i++)
		 {
			 for(j=0;j<MT;j++)
			 {
				 for(k=0;k<NT;k++)
				 {
					 if(k>=314)
					 {
						 if(Att[i*MT*NT+j*NT+k]==2)
						 {
							Att[i*MT*NT+j*NT+k]=1;
						 }
					 }
					 if(k>=243 && k<314)
					 {
						 if(Att[i*MT*NT+j*NT+k]==2)
						 {
							Att[i*MT*NT+j*NT+k]=3;
						 }
					 }
				 }
			 }
		 }
		 
		 for(i=0;i<LT;i++)
		 {
			for(j=0;j<MT;j++)
			{
				for(k=0;k<NT;k++)
				{
	                fwrite(&Att[i*MT*NT+j*NT+k],sizeof(char),1,pf1);
	                 
				}
			} 
		 }

		 fclose(pf1);
		
		

		   AfxMessageBox("完成write sgn file");
		   return;

}
void CMacrosegregation::OpenSpecial(CString str)
{//open the *.sgn file to get the information of the ingot on 3D
	
		FILE*pf;
		FILE*pf1;

		char atmp;
		int w=0;
		float ttmp=0.0f;
        unsigned int i,j,k;
		unsigned short int yh[111];
		short int yhh[111];

		CString strPath=str;
		CString Prefix;
		Prefix=GetFileNameByPath(strPath);
		Prefix=GetFileTitleByName(Prefix);
		Prefix=GetFilePrefixByTitle(Prefix);
		CString namepdt=Prefix+_T(".PDT");//热物性文件(.pdt)名

		if(w==0)
		{
			if((pf=fopen(str,"rb"))==NULL)
			{
				 AfxMessageBox("can not open sgn file");
				 return;
			}
			fread(yh,sizeof(short int),111,pf);

               LT=yh[0];
               MT=yh[1];
               NT=yh[2];
			  

			   if(Att==NULL)
			   {//铸件属性
				   Att=(char*)malloc(MT*NT*LT*sizeof(char));
			   }
			   if(Att==NULL)
			   {
				   AfxMessageBox("allocate memory for *.sgn file");
				   return;
			   }

			  
			  
			   
			   for(i=0;i<LT;i++)
			   {
				   for(j=0;j<MT;j++)
				   {
					  for(k=0;k<NT;k++)
					  {
	                      Att[i*MT*NT+j*NT+k]=-2;
					  }
					} 
			   }
				
			   for(i=0;i<LT;i++)
			   {
				   for(j=0;j<MT;j++)
				   {
					  for(k=0;k<NT;k++)
					  {
	                      fread(&atmp,sizeof(char),1,pf);
	                      Att[i*MT*NT+j*NT+k]=atmp;
					
						  if(Att[i*MT*NT+j*NT+k]==1)// change air-element from 1 to -2
						  {
							  Att[i*MT*NT+j*NT+k]=-2;
						  }
						  
					  }
					} 
			   }
			   w=w+1;
			   fclose(pf);

			   for(i=1;i<(LT-1);i++)
			   {
				   for(j=1;j<(MT-1);j++)
				   {
					  for(k=1;k<(NT-1);k++)
					  {
						  unsigned int chan=i*MT*NT+j*NT+k;
						  
						  unsigned int chanleft=i*MT*NT+(j-1)*NT+k;
						  unsigned int chanright=i*MT*NT+(j+1)*NT+k;
						  unsigned int chanfront=(i+1)*MT*NT+j*NT+k;
						  unsigned int chanback=(i-1)*MT*NT+j*NT+k;
						  unsigned int chandown=i*MT*NT+j*NT+k-1;

						  if(Att[chan]==0)
						  {
							  if(Att[chanleft]==-2)
							  {
								  Att[chanleft]=2;
							  }
							  if(Att[chanright]==-2)
							  {
								  Att[chanright]=2;
							  }
							  if(Att[chanfront]==-2)
							  {
								  Att[chanfront]=2;
							  }
							  if(Att[chanback]==-2)
							  {
								  Att[chanback]=2;
							  }
							  if(Att[chandown]==-2)
							  {
								  Att[chandown]=2;
							  }
						  }//0
						  if(Att[chan]==6)
						  {
							  if(Att[chanleft]==-2)
							  {
								  Att[chanleft]=2;
							  }
							  if(Att[chanright]==-2)
							  {
								  Att[chanright]=2;
							  }
							  if(Att[chanfront]==-2)
							  {
								  Att[chanfront]=2;
							  }
							  if(Att[chanback]==-2)
							  {
								  Att[chanback]=2;
							  }
							  if(Att[chandown]==-2)
							  {
								  Att[chandown]=2;
							  }
						  }//6

						  /*if(Att[chan]==8)
						  {
							  if(Att[chanleft]==-2)
							  {
								  Att[chanleft]=8;
							  }
							  if(Att[chanright]==-2)
							  {
								  Att[chanright]=8;
							  }
							  if(Att[chanfront]==-2)
							  {
								  Att[chanfront]=8;
							  }
							  if(Att[chanback]==-2)
							  {
								  Att[chanback]=8;
							  }
							  if(Att[chandown]==-2)
							  {
								  Att[chandown]=8;
							  }
						  }//8
						  */

						  if(Att[chan]==9)
						  {
							  if(Att[chanleft]==-2)
							  {
								  Att[chanleft]=9;
							  }
							  if(Att[chanright]==-2)
							  {
								  Att[chanright]=9;
							  }
							  if(Att[chanfront]==-2)
							  {
								  Att[chanfront]=9;
							  }
							  if(Att[chanback]==-2)
							  {
								  Att[chanback]=9;
							  }
							  if(Att[chandown]==-2)
							  {
								  Att[chandown]=9;
							  }
						  }//9
						  

						  if(Att[chan]==4)
						  {
							  if(Att[chanleft]==-2)
							  {
								  Att[chanleft]=4;
							  }
							  if(Att[chanright]==-2)
							  {
								  Att[chanright]=4;
							  }
							  if(Att[chanfront]==-2)
							  {
								  Att[chanfront]=4;
							  }
							  if(Att[chanback]==-2)
							  {
								  Att[chanback]=4;
							  }
							  if(Att[chandown]==-2)
							  {
								  Att[chandown]=4;
							  }
						  }//4

						  if(Att[chan]==7)
						  {
							  if(Att[chanleft]==-2)
							  {
								  Att[chanleft]=7;
							  }
							  if(Att[chanright]==-2)
							  {
								  Att[chanright]=7;
							  }
							  if(Att[chanfront]==-2)
							  {
								  Att[chanfront]=7;
							  }
							  if(Att[chanback]==-2)
							  {
								  Att[chanback]=7;
							  }
							  if(Att[chandown]==-2)
							  {
								  Att[chandown]=7;
							  }
						  }//7
					  }
					} 
			   }

			   for(i=1;i<(LT-1);i++)// the lowest part should be 2,not -2
			   {
				   for(j=1;j<(MT-1);j++)
				   {
					  k=1;
					  if(Att[i*MT*NT+j*NT+k]==0)
					  {
						  Att[i*MT*NT+j*NT+k]=2;
					  }
					  
				   }
			   }


			  
		}
		
		BFluidFile(str);// write the flow field file

		CString fluidname=str+_T(".fld");
		
		if((pf1=fopen(fluidname,"rb"))==NULL)
		{
			AfxMessageBox("can not open the *.fld file");
		}

		 if(pf1!=NULL)
		 {
			 fread(&yhh,sizeof(short int),111,pf1);

			  DL=yhh[4];
			  DM=yhh[5];
			  DN=yhh[6];
			  L=yhh[7];
			  M=yhh[8];
			  N=yhh[9];
		 }

		   fclose(pf1);

		   AfxMessageBox("完成初始化");
		   return;
}
void CMacrosegregation::BFluidFile(CString str)
{
	CString fluidname=str+_T(".fld");

	FILE*fp;//sgn
	FILE*fp1;//fld

	unsigned short int i,j,k,si,i1,j1,k1,i2,j2,k2;
	unsigned int FL=0;
	unsigned int FM=0;
	unsigned int FN=0;
	short int LL=0;
	short int MM=0;
	short int NN=0;
	short int yhh[111];
	
	if((fp=fopen(str,"rb"))==NULL) 
	{
		AfxMessageBox ("Cannot open file *.sgn");
	}
	if(fp!=NULL)
	{
		fread(&yhh,sizeof(short int),111,fp);
		FL=yhh[0]; 
		FM=yhh[1]; 
		FN=yhh[2];
	}
	

	si=0;
	
	for(i=0;i<FL;i++)
	{
	  for(j=0;j<FM;j++)
      {
		  for(k=0;k<FN;k++)
		  {
			  char tp;
			  fread(&tp,sizeof(char),1,fp);
			  if(si==0)
			  if(tp==0)
			  {
				i1=i2=i; 
				j1=j2=j; 
				k1=k2=k;
				si=1;   
			  }
			  if(tp==0)
			  {
				i2=i>i2 ? i:i2;  //把大值赋给i2
				i1=i<i1 ? i:i1;  //把小值赋给i1
				j2=j>j2 ? j:j2;  
				j1=j<j1 ? j:j1;
				k2=k>k2 ? k:k2;  
				k1=k<k1 ? k:k1;  
			
			  }
		  }
	  }
	}
	
	i2++; 
	i1--;
	j2++; 
	j1--;
	k2++;
	k1--; //k2？？？？？？？？
	
	LL=i2-i1+1; 
	MM=j2-j1+1; 
	NN=k2-k1+1;
	
	yhh[0]=FL; //LT
	yhh[1]=FM; //MT
	yhh[2]=FN; //NT
	yhh[3]=0;
	yhh[4]=i1; //DL
	yhh[5]=j1; //DM
	yhh[6]=k1; //DN
	yhh[7]=LL; //L
	yhh[8]=MM; //M
	yhh[9]=NN; //N
	yhh[100]=111;

   if((fp1=fopen(fluidname,"wb"))==NULL)
   {
	   AfxMessageBox ("Cannot open file *.fld");
   }
   if(fp1!=NULL)
   {   
	   fwrite(&yhh,sizeof(short int),111,fp1);
   }
   
   fclose(fp);
   fclose(fp1);
}
void CMacrosegregation::PlaceInfo()
{
	//#define  OUTPUT_ATT

	unsigned int i=0;
	unsigned int j=0;
	unsigned int k=0;

	if(m_place==0)//i
	{
		ii=m_spai;//i--LT,j---MT, k---NT
		limitZ=LT;
		limitY=MT;
		limitX=NT;
		limitXY=MT*NT;

		limitz=L;
		limity=M;
		limitx=N;
		limitxy=M*N;

		DDL=DL;
		DDM=DM;
		DDN=DN;
	}
	if(m_place==1)//j
	{
		ii=m_spaj;
		limitZ=MT;
		limitY=NT;
		limitX=LT;
		limitXY=LT*NT;

		limitz=M;
		limity=N;
		limitx=L;
		limitxy=L*N;

		DDL=DM;
		DDM=DN;
		DDN=DL;
	}
	if(m_place==2)//k
	{
		ii=m_spak;
		limitZ=NT;
		limitY=MT;
		limitX=LT;
		limitXY=LT*MT;	

		limitz=N;
		limity=M;
		limitx=L;
		limitxy=L*M;

		DDL=DN;
		DDM=DM;
		DDN=DL;
	}
	
	//ingot+mould
	if(Ttold==NULL)
	{
		Ttold=(double*)malloc(MT*NT*LT*sizeof(double));
	}

	if(Tt==NULL)
	{
		Tt=(double*)malloc(MT*NT*LT*sizeof(double));
	}
	if(TH==NULL)
	{
		TH=(double*)malloc(MT*NT*LT*sizeof(double));
	}
	
	//only ingot
	if(fsold==NULL)
	{
		fsold=(double*)malloc(L*M*N*sizeof(double));
	}
	if(subfsold==NULL)
	{
		subfsold=(double*)malloc(L*M*N*sizeof(double));
	}

	
	if(CL==NULL)
	{
		CL=(double*)malloc(L*M*N*sizeof(double));
	}

	if(Cmix==NULL)
	{
		Cmix=(double*)malloc(L*M*N*sizeof(double));
	}
	if(Cmixold==NULL)
	{
		Cmixold=(double*)malloc(L*M*N*sizeof(double));
	}
	
	
	if(CCS==NULL)
	{
		CCS=(double*)malloc(L*M*N*sizeof(double));
	}
	if(Vv==NULL)
	{
		Vv=(double*)malloc(L*M*N*sizeof(double));
	}
	if(Vu==NULL)
	{
		Vu=(double*)malloc(L*M*N*sizeof(double));
	}
	if(Vw==NULL)
	{
		Vw=(double*)malloc(L*M*N*sizeof(double));
	}
	if(Pp==NULL)
	{
		Pp=(double*)malloc(L*M*N*sizeof(double));
	}

	if(start_poro==NULL)
	{
		start_poro=(unsigned int*)malloc(L*M*N*sizeof(unsigned int));
	}
	if(tem_grad==NULL)
	{
		tem_grad=(double*)malloc(L*M*N*sizeof(double));
	}
	if(cooling_rate==NULL)
	{
		cooling_rate=(double*)malloc(L*M*N*sizeof(double));
	}

	
	if((Tt==NULL)||(TH==NULL)||(fsold==NULL)||(subfsold==NULL)||(CL==NULL)||(Cmix==NULL)||(Cmixold==NULL)||(CCS==NULL)||(Vv==NULL)||(Vu==NULL)||(Vw==NULL)||(Pp==NULL)||(tem_grad==NULL)||(cooling_rate==NULL)||(start_poro==NULL))
	{
		AfxMessageBox(" can not allocate memory for macrosegregation calculation");
		return;
	}
	

	for(i=0;i<LT;i++)
	{
		for(j=0;j<MT;j++)
		{
			for(k=0;k<NT;k++)
			{
				unsigned int chan=i*MT*NT+j*NT+k;
				if(Att[chan]==0)
				{
					Ttold[chan]=Temp_c;// casting temperature
					Tt[chan]=Temp_c; 
					TH[chan]=Temp_c*Dnst[0]*Spht[0]+Dnst[0]*L_heat;
				
				}
				else
				{
					if(Att[chan]!=-2)
					{
						if(Att[chan]==8 || Att[chan]==2 || Att[chan]==6)
						{
							Tt[chan]=Temp_mold; // mold temperature
							TH[chan]=Temp_mold*Dnst[Att[chan]]*Spht[Att[chan]];
						}
						else
						{
							Tt[chan]=Temp_e; // mold temperature
							TH[chan]=Temp_e*Dnst[Att[chan]]*Spht[Att[chan]];
						}
					}
					else
					{
						Tt[chan]=Temp_e;
						TH[chan]=0.0;
					}
				}
			}//k
		}//j
	}//i
	
	//only ingot: for flow fields

	for(i=0;i<L;i++)
	{
		for(j=0;j<M;j++)
		{
			for(k=0;k<N;k++)
			{
				unsigned int chan=0;
				chan=i*M*N+j*N+k;
				
				Vv[chan]=0.0;
				Vu[chan]=0.0;
				Vw[chan]=0.0;
				Pp[chan]=0.0;

				CCS[chan]=0.0;
				subfsold[chan]=0.0;
				fsold[chan]=0.0;
				CL[chan]=m_fcarbon;
				Cmix[chan]=Dnst[0]*m_fcarbon;
				Cmixold[chan]=Dnst[0]*m_fcarbon;

				start_poro[chan]=0;
				tem_grad[chan]=-1.0;
				cooling_rate[chan]=-1.0;
			
			}//k
		}//j
	}//i



	Min_k=0;
	Max_k=0;
	Min_j=0;
	Max_j=0;


	for(k=0;k<limitX;k++)
	{
		for(j=0;j<limitY;j++)
		{
			unsigned int chan=ii*limitXY+j*limitX+k;
			unsigned int chan1=ii*limitXY+j*limitX+k-1;
			if(Att[chan]==0 && Att[chan1]!=0)
			{
				Min_k=k;
				break;
			}
		}
		if(Min_k!=0)
		{
			break;
		}
	}

	for(j=0;j<limitY;j++)
	{
		for(k=0;k<limitX;k++)
		{
			unsigned int chan=ii*limitXY+j*limitX+k;
			unsigned int chan1=ii*limitXY+j*limitX+k+1;
			if(Att[chan]==0 && Att[chan1]!=0)
			{
				Max_k=k;
				break;
			}
		}
		if(Max_k!=0)
		{
			break;
		}
	}
	
	for(k=(limitX-1);k>0;k--)
	{
		for(j=0;j<limitY;j++)
		{
			unsigned int chan=ii*limitXY+j*limitX+k;
			unsigned int chan1=ii*limitXY+(j-1)*limitX+k;
			if(Att[chan]==0 && Att[chan1]!=0)
			{
				Min_j=j;
				break;
			}
		}
		if(Min_j!=0)
		{
			break;
		}
	}
	for(k=(limitX-1);k>0;k--)
	{
		for(j=0;j<limitY;j++)
		{
			unsigned int chan=ii*limitXY+j*limitX+k;
			unsigned int chan1=ii*limitXY+(j+1)*limitX+k;
			if(Att[chan]==0 && Att[chan1]!=0)
			{
				Max_j=j;
				break;
			}
		}
		if(Max_j!=0)
		{
			break;
		}
	}



//#if OUTPUT_ATT

	/*FILE*fp;
	fp=fopen("d:\\Ldr\\character.dat","w+");
		
	for(j=0;j<MT;j++)
	{
		for(k=0;k<NT;k++)
		{
			unsigned int chan=(ii)*MT*NT+j*NT+k;
			fprintf(fp,"%d ",Att[chan]);
		}
		fprintf(fp," \n"); 
	}
	fclose(fp);
	int quq=90;
	*/
	
	

	/*double nnum=0.0;
	double nnum1=0.0;
	for(i=0;i<LT;i++)
	{
		for(j=0;j<limitY;j++)
		{
			for(k=0;k<limitX;k++)
			{
				unsigned int chan=i*limitXY+j*limitX+k;
				
				if(Att[chan]==0 || Att[chan]==7)
				{
					nnum1=nnum1+1.0;
				}
				if(Att[chan]==7)
				{
					nnum=nnum+1.0;
				}
			}
		}
	}
	int qq=90;
	*/
	//#endif
	
}

double CMacrosegregation::MinPara()
{
	int i=0;
	double min=0.0f;
	double a[18];
	for(i=0;i<18;i++)
	{
		a[i]=1000.0f;
	}

		if(Dnst[0]!=0)
		{
			a[0]=Space_step*Space_step*Dnst[0]*Spht[0]/(Htcf[0]*4.0f);//Fe-C
			a[1]=Space_step*Space_step*Dnst[0]*Spht[0]/(3.0f*Htcf[0]+Fhm[0][2]);
			a[2]=Space_step*Space_step*Dnst[0]*Spht[0]/(2.0f*Htcf[0]+Fhm[0][2]+Fhm[0][6]);
			a[3]=Space_step*Space_step*Dnst[0]*Spht[0]/(2.0f*Htcf[0]+Fhm[0][2]+Fhm[0][9]);
			a[4]=Space_step*Space_step*Dnst[0]*Spht[0]/(2.0f*Htcf[0]+2.0f*Fhm[0][2]);
		}
		if(Dnst[2]!=0)
		{
			a[5]=Space_step*Space_step*Dnst[2]*Spht[2]/(Htcf[2]*4.0f);//mold material
			a[6]=Space_step*Space_step*Dnst[2]*Spht[2]/(3.0f*Htcf[2]+Fhm[2][0]);
			a[7]=Space_step*Space_step*Dnst[2]*Spht[2]/(2.0f*Htcf[2]+Fhm[2][0]+Fhm[2][6]);
			a[8]=Space_step*Space_step*Dnst[2]*Spht[2]/(2.0f*Htcf[2]+Fhm[2][0]+Fhm[2][9]);
			a[9]=Space_step*Space_step*Dnst[2]*Spht[2]/(2.0f*Htcf[2]+2.0f*Fhm[2][0]);
		
		}
		if(Dnst[6]!=0)
		{
			a[10]=Space_step*Space_step*Dnst[6]*Spht[6]/(Htcf[6]*4.0f);//mold material
			a[11]=Space_step*Space_step*Dnst[6]*Spht[6]/(3.0f*Htcf[6]+Fhm[6][0]);
			a[12]=Space_step*Space_step*Dnst[6]*Spht[6]/(2.0f*Htcf[6]+2.0f*Fhm[6][0]);
		}
		if(Dnst[9]!=0)
		{
			a[13]=Space_step*Space_step*Dnst[9]*Spht[9]/(Htcf[9]*4.0f);//mold material
			a[14]=Space_step*Space_step*Dnst[9]*Spht[9]/(3.0f*Htcf[9]+Fhm[9][0]);
			a[15]=Space_step*Space_step*Dnst[9]*Spht[9]/(2.0f*Htcf[9]+2.0f*Fhm[9][0]);
		}

		if(Dnst[8]!=0)
		{
			a[16]=Space_step*Space_step*Dnst[8]*Spht[8]/(Htcf[8]*4.0f);//mold material
		}
		if(Dnst[7]!=0)
		{
			a[17]=Space_step*Space_step*Dnst[7]*Spht[7]/(Htcf[7]*4.0f);//mold material
		}
	
		min=a[0];
		for(i=0;i<18;i++)
		{
			if(a[i]<min)
			{
				min=a[i];
			}
		}
		return min;
}

void CMacrosegregation::Heat_Transfer_Coefficient()
{
		double hi=0.0;
		double Tem_ingot=0.0;
		double Tem_mold=0.0;
		double Tem_peri=0.0;
		double num=0.0;
		double num1=0.0;
		double all_fl=0.0;

		unsigned int i=0;
		unsigned int j=0;
		unsigned int k=0;

		for(i=1;i<LT-1;i++)
		{
			for(j=1;j<MT-1;j++)
			{
				for(k=1;k<NT-1;k++)
				{
					unsigned int chan=i*MT*NT+j*NT+k;

					unsigned int chanup=i*MT*NT+j*NT+k+1;
					unsigned int chandown=i*MT*NT+j*NT+k-1;
					unsigned int chanleft=i*MT*NT+(j-1)*NT+k;
					unsigned int chanright=i*MT*NT+(j+1)*NT+k;
					unsigned int chanfront=(i+1)*MT*NT+j*NT+k;
					unsigned int chanback=(i-1)*MT*NT+j*NT+k;

					unsigned int subchan=(i-DDL)*M*N+(j-DDM)*N+k-DDN;
					if(Att[chan]==0)
					{
						if(Att[chanup]>0 || Att[chandown]>0 || Att[chanleft]>0 ||Att[chanright]>0 || Att[chanfront]>0 ||Att[chanback]>0)
						{
							if(Att[chanup]!=pipe_hole && Att[chanup]!=shrinkage_hole && Att[chandown]!=pipe_hole && Att[chandown]!=shrinkage_hole && Att[chanleft]!=pipe_hole && Att[chanleft]!=shrinkage_hole && Att[chanright]!=pipe_hole && Att[chanright]!=shrinkage_hole && Att[chanfront]!=pipe_hole && Att[chanfront]!=shrinkage_hole && Att[chanback]!=pipe_hole && Att[chanback]!=shrinkage_hole )
							{
								num1=num1+1.0;
								Tem_peri=Tem_peri+Tt[chan]/1000.0;
								all_fl=all_fl+(1.0-fsold[subchan]);
							}
						}

					}//Att
				}//k
			}//j
		}//i

		for(i=1;i<LT-1;i++)
		{
			for(j=1;j<MT-1;j++)
			{
				for(k=1;k<NT-1;k++)
				{
					unsigned int chan=i*MT*NT+j*NT+k;

					unsigned int chanup=i*MT*NT+j*NT+k+1;
					unsigned int chandown=i*MT*NT+j*NT+k-1;
					unsigned int chanleft=i*MT*NT+(j-1)*NT+k;
					unsigned int chanright=i*MT*NT+(j+1)*NT+k;
					unsigned int chanfront=(i+1)*MT*NT+j*NT+k;
					unsigned int chanback=(i-1)*MT*NT+j*NT+k;

					if(Att[chan]==0)
					{

						if(Att[chanup]>0 && Att[chanup]!=pipe_hole && Att[chanup]!=shrinkage_hole)
						{
							num=num+1;
							Tem_ingot=Tem_ingot+(Tt[chan]+273.0f)/1000.0;
							Tem_mold=Tem_mold+(Tt[chanup]+273.0f)/1000.0;
						}
						if(Att[chandown]>0 && Att[chandown]!=pipe_hole && Att[chandown]!=shrinkage_hole)
						{
							num=num+1;
							Tem_ingot=Tem_ingot+(Tt[chan]+273.0f)/1000.0;
							Tem_mold=Tem_mold+(Tt[chandown]+273.0f)/1000.0;
						}
						if(Att[chanleft]>0 && Att[chanleft]!=pipe_hole && Att[chanleft]!=shrinkage_hole)
						{
							num=num+1;
							Tem_ingot=Tem_ingot+(Tt[chan]+273.0f)/1000.0;
							Tem_mold=Tem_mold+(Tt[chanleft]+273.0f)/1000.0;
						}
						if(Att[chanright]>0 && Att[chanright]!=pipe_hole && Att[chanright]!=shrinkage_hole)
						{
							num=num+1;
							Tem_ingot=Tem_ingot+(Tt[chan]+273.0f)/1000.0;
							Tem_mold=Tem_mold+(Tt[chanright]+273.0f)/1000.0;
						}
						if(Att[chanfront]>0 && Att[chanfront]!=pipe_hole && Att[chanfront]!=shrinkage_hole)
						{
							num=num+1;
							Tem_ingot=Tem_ingot+(Tt[chan]+273.0f)/1000.0;
							Tem_mold=Tem_mold+(Tt[chanfront]+273.0f)/1000.0;
						}
						if(Att[chanback]>0 && Att[chanback]!=pipe_hole && Att[chanback]!=shrinkage_hole)
						{
							num=num+1;
							Tem_ingot=Tem_ingot+(Tt[chan]+273.0f)/1000.0;
							Tem_mold=Tem_mold+(Tt[chanback]+273.0f)/1000.0;
						}
					}//att
				}//k
			}//j
		}//i


		hi=0.0f;
		if((1000.0f*Tem_peri/num1)>=TL)
		{
			hi=himax;
		}
		if(((1000.0f*Tem_peri/num1)>TS)&&((1000.0f*Tem_peri/num1)<TL))
		{
			double FL=all_fl/num1;
			hi=(double)((himin+FL*(himax-himin))+(1.0f-FL)*(0.00567f*0.4f*(pow(Tem_ingot/num,4)-pow(Tem_mold/num,4))/(Tem_ingot/num-Tem_mold/num)));
		}
		if((1000.0f*Tem_peri/num1)<=TS)
		{
			hi=(double)(himin+0.00567f*0.4f*(pow(Tem_ingot/num,4)-pow(Tem_mold/num,4))/(Tem_ingot/num-Tem_mold/num));
		}

		Fhm[0][2]=1.0/((Htcf[0]+Htcf[2])/(2.0f*Htcf[0]*Htcf[2])+1.0f/(Space_step*hi));
		Fhm[2][0]=1.0/((Htcf[0]+Htcf[2])/(2.0f*Htcf[0]*Htcf[2])+1.0f/(Space_step*hi));

	
}
void CMacrosegregation::Cal_Flow(CDC *pDC)//1
{
	double ddss=0.05;
	double alfa=0.8;     // 权重因子
	double beta=1.5;//1.5,0.8f;    //松驰因子
	double pj=0.0001; //0.0001      //收敛精度
	double Gravity=-980.0;

	 long int md=0; 
	 unsigned int i=0;	 
     unsigned int j=0;
	 unsigned int k=0;
	
	 long int xi=0;
	 long int ix=0;
	 long int jy=0;
	 long int yj=0;
	 long int zk=0;
	 long int kz=0;
	
	 long int pjn=0;
	 unsigned int lmn=0;

	 double sgnu=0.0;
	 double sgnv=0.0;
	 double sgnw=0.0;

	 double FU=0.0;
	 double FV=0.0;
	 double FW=0.0;

	 double VSTX=0.0;
	 double VSTY=0.0;
	 double VSTZ=0.0;
	 double DLTP=0.0;

	 double U=0.0;
	 double V=0.0;
	 double W=0.0;

	 double dv=0.0;
	 double Dd=0.0;
	 double Con1=0.0;
	 double Con2=0.0;

	 double it1=0.0;
	 double it2=0.0;
	 double st2=0.0;
	
	 double dd1=0.0;
	 double dd2=0.0;
	 double dd3=0.0;
	 
	 double dsu=0.0;
	

	 double*NVv;
	 double*NVu;
	 double*NVw;
	 char*att;
	 double*Tlt;

	 NVv=NULL;
	 NVu=NULL;
	 NVw=NULL;
	 att=NULL;
	 Tlt=NULL;

/////////////////////
	
	  NVv=(double *)malloc(L*M*N*sizeof(double));
	  NVu=(double *)malloc(L*M*N*sizeof(double));
	  NVw=(double *)malloc(L*M*N*sizeof(double));
	  att=(char *)malloc(L*M*N*sizeof(char));
	  Tlt=(double *)malloc(L*M*N*sizeof(double));

	  if(NVv==NULL || NVu==NULL || NVw==NULL  || att==NULL ||Tlt==NULL)
	  {
		  AfxMessageBox("No enough memory in liquid flow field calculation !");
	  }

	  st2=2.0*Space_step;

	  for(i=0;i<L;i++)
	  {
		  for(j=0;j<M;j++)
		  {
			  for(k=0;k<N;k++)
			  {
				  unsigned int chan=i*M*N+j*N+k;
				  unsigned int chan1=(i+DDL)*MT*NT+(j+DDM)*NT+k+DDN;

				  att[chan]=-2;
				  Tlt[chan]=0.0;

				  if(Att[chan1]==0)
				  {
					  att[chan]=Att[chan1];
					  Tlt[chan]=Tt[chan1];
				  }
			  }
		  }
	  }


	  for(i=0;i<(L*M*N);i++)
	  {
		  if((att[i]==0)&&((fsold[i]>=fs_critical)||(Tlt[i]<TS)))// flow element became a non-flow element
		  { 
			  Vu[i]=0.0;
			  Vv[i]=0.0;
			  Vw[i]=0.0;
			  
			  Vu[i-M*N]=0.0;
			  Vv[i-N]=0.0;
			  Vw[i-1]=0.0;
		  }
	  } 
	 
	  for(i=0;i<(L*M*N);i++)
	  {
		  NVv[i]=Vv[i];
		  NVu[i]=Vu[i];
		  NVw[i]=Vw[i];
	  } 

		  for(i=0;i<((L-1)*M*N);i++)//calculating the Liquid flow 
		  {	 
			  if((fsold[i]<fs_critical)&&(fsold[i+M*N]<fs_critical)&&(att[i]==0)&&(att[i+M*N]==0))
			  {
				  if((Tlt[i]>TS)&&(Tlt[i+M*N]>TS))
				  {
					 if(Vu[i]>=0.0) 
					 {
						 sgnu=1.0;
					 }  
					 else
					 { 
						 sgnu=-1.0;
					 }
					 
					 it1=(1.0+alfa*sgnu)*(Dnst[0]*Vu[i]-Dnst[0]*Vu[i-M*N]);
					 it2=(1.0-alfa*sgnu)*(Dnst[0]*Vu[i+M*N]-Dnst[0]*Vu[i]);
					 FU=(it1+it2)*Vu[i]/(st2*Dnst[0]);
					 
					 V=(float)((Vv[i]+Vv[i-N]+Vv[i+M*N]+Vv[i+M*N-N])/4.0);
			 
					 if(V>=0) 
					 {
						 sgnv=1.0;
					 }
					 else
					 {		   	
						 sgnv=-1.0;
					 }
					 it1=(float)(1.0+alfa*sgnv)*(Vu[i]-Vu[i-N]);
					 it2=(float)(1.0-alfa*sgnv)*(Vu[i+N]-Vu[i]);
					 FV=(it1+it2)*V/st2;

					 W=(float)((Vw[i]+Vw[i-1]+Vw[i+M*N]+Vw[i+M*N-1])/4.0);
					 
					 if(W>=0.0) 
					 {
						 sgnw=1.0;
					 }
					 else
					 {		   	
						 sgnw=-1.0;
					 }
					 
					 it1=(1.0+alfa*sgnw)*(Dnst[0]*Vu[i]-Dnst[0]*Vu[i-1]);
					 it2=(1.0-alfa*sgnw)*(Dnst[0]*Vu[i+1]-Dnst[0]*Vu[i]);
					 
					 FW=(it1+it2)*W/(st2*Dnst[0]);
				
					 VSTX=VsctyL*(Vu[i+M*N]+Vu[i-M*N]+Vu[i+N]+Vu[i-N]+Vu[i+1]+Vu[i-1]-6.0*Vu[i])/(Space_step*Space_step);//粘度
					 
					 DLTP=(Pp[i]-Pp[i+M*N])/(Space_step*Dnst[0]);

					 double drag1=ddss*ddss*(1.0-fsold[i])*(1.0-fsold[i])*(1.0-fsold[i]);//0.008*0.008
					 double drag2=180.0*VsctyL*fsold[i]*fsold[i]*Time_step;
					 NVu[i]=(Vu[i]*drag1+Time_step*(DLTP-FU-FV-FW+VSTX)*drag1)/(drag1+drag2);

				  }//Tlt
				  	
			  }
			  
			  if((fsold[i]<fs_critical)&&(fsold[i+N]<fs_critical)&&(att[i]==0)&&(att[i+N]==0)) 
			  {
				  if((Tlt[i]>TS)&&(Tlt[i+N]>TS))
				  {
				  	U=((Vu[i]+Vu[i-M*N]+Vu[i+N]+Vu[i-M*N+N])/4.0);
			
					if(U>=0) 
					{
						sgnu=1.0;
					}
					else
					{
						sgnu=-1.0;
					}
					
					it1=(1.0+alfa*sgnu)*(Vv[i]-Vv[i-M*N]);
					it2=(1.0-alfa*sgnu)*(Vv[i+M*N]-Vv[i]);
			
					FU=(it1+it2)*U/st2;
			
					if(Vv[i]>=0) 
					{
						sgnv=1.0;
					}
					else
					{
						sgnv=-1.0;
					}
			
					it1=(1.0+alfa*sgnv)*(Vv[i]-Vv[i-N]);
					it2=(1.0-alfa*sgnv)*(Vv[i+N]-Vv[i]);
			
					FV=(it1+it2)*Vv[i]/st2;
			
					W=((Vw[i]+Vw[i-1]+Vw[i+N]+Vw[i+N-1])/4.0);
			
					if(W>=0) 
					{
						sgnw=1.0;
					}
					else
					{
						sgnw=-1.0;
					}
				  
					it1=(float)(1.0+alfa*sgnw)*(Vv[i]-Vv[i-1]);
					it2=(float)(1.0-alfa*sgnw)*(Vv[i+1]-Vv[i]);
			
					FW=(it1+it2)*W/st2;
			
					VSTY=VsctyL*(Vv[i+M*N]+Vv[i-M*N]+Vv[i+N]+Vv[i-N]+Vv[i+1]+Vv[i-1]-6.0*Vv[i])/(Space_step*Space_step);
					DLTP=(Pp[i]-Pp[i+N])/(Space_step*Dnst[0]);	
					
					double drag1=ddss*ddss*(1.0-fsold[i])*(1.0-fsold[i])*(1.0-fsold[i]);////0.008*0.008
					double drag2=180.0*VsctyL*fsold[i]*fsold[i]*Time_step;
					NVv[i]=(Vv[i]*drag1+Time_step*(DLTP-FU-FV-FW+VSTY)*drag1)/(drag1+drag2);

				  }//Tlt
			  }

			  if((fsold[i]<fs_critical)&&(fsold[i+1]<fs_critical)&&(att[i]==0)&&(att[i+1]==0)) 
			  {
				 
				  if((Tlt[i]>TS)&&(Tlt[i+1]>TS))
				  {
					  U=(Vu[i]+Vu[i-M*N]+Vu[i+1]+Vu[i-M*N+1])/4.0;
					  
					  if(U>=0.0) 
					  {
						  sgnu=1.0;
					  }
					  else
					  {
						  sgnu=-1.0;
					  }
					
					  it1=(1.0+alfa*sgnu)*(Dnst[0]*Vw[i]-Dnst[0]*Vw[i-M*N]);
					  it2=(1.0-alfa*sgnu)*(Dnst[0]*Vw[i+M*N]-Dnst[0]*Vw[i]);
					  FU=(it1+it2)*U/(st2*Dnst[0]);
					  
					  V=(Vv[i]+Vv[i-N]+Vv[i+1]+Vv[i-N+1])/4.0;
					  
					  if(V>=0) 
					  {
						sgnv=1.0;
					  }
					  else
					  {
						sgnv=-1.0;
					  }
			
			          it1=(1.0+alfa*sgnv)*(Vw[i]-Vw[i-N]);
			          it2=(1.0-alfa*sgnv)*(Vw[i+N]-Vw[i]);
			          FV=(it1+it2)*V/st2;
					
					  if(Vw[i]>=0.0) 
					  {
						 sgnw=1.0;
					  }
					  else
					  {
						  sgnw=-1.0;
					  }
					  
					  it1=(1.0+alfa*sgnw)*(Dnst[0]*Vw[i]-Dnst[0]*Vw[i-1]);
					  it2=(1.0-alfa*sgnw)*(Dnst[0]*Vw[i+1]-Dnst[0]*Vw[i]);
					  FW=(it1+it2)*Vw[i]/(st2*Dnst[0]);
					  
					  double vvssll=VsctyL;
					
					  VSTZ=VsctyL*(Vw[i+M*N]+Vw[i-M*N]+Vw[i+N]+Vw[i-N]+Vw[i+1]+Vw[i-1]-6.0*Vw[i])/(Space_step*Space_step);
					  DLTP=(Pp[i]-Pp[i+1])/(Space_step*Dnst[0]);
					 
					  double tem_con=tem_con=Tem_coeff*(TL-Tlt[i])+Con_coeff*(m_fcarbon-CL[i]);//Tem_coeff*(Tem_ref-Tlt[i])+Con_coeff*(m_fcarbon-CL[i]);
					 
					  double drag1=ddss*ddss*(1.0-fsold[i])*(1.0-fsold[i])*(1.0-fsold[i]);////0.008*0.008
					  double drag2=180.0*VsctyL*fsold[i]*fsold[i]*Time_step; 	
					  NVw[i]=(Vw[i]*drag1+Time_step*(DLTP-FU-FV-FW+VSTZ+Gravity*tem_con)*drag1)/(drag1+drag2);	
					   
				  }//Tlt
				  
			  }
		}//i
	
	lmn=L*M*N;
	Con1=(beta*Space_step/6.0);
	Con2=Space_step/Time_step;
	md=0;
	
	do
	{
		CString text1;
		text1.Format("itera_V_P=%d ",(int)md);	
		pDC->TextOut(10,90,text1);

		md++;
		pjn=0;

		for(i=0;i<lmn;i++) 
		{
			if((fsold[i]<fs_critical)&&(att[i]==0)&&(Tlt[i]>TS))
			{
					
				Dd=(NVu[i]-NVu[i-M*N]+NVv[i]-NVv[i-N]+NVw[i]-NVw[i-1])/Space_step;
			
				if( Dd>pj || Dd<(0.0-pj) )//d.pj为收敛精度
				{
					pjn++;
					
					ix=i-M*N;
			        jy=i-N;
			        kz=i-1;
			        
					xi=i+M*N;
			        yj=i+N;
			        zk=i+1;
					
					dv=(0.0-Con1*Dd);//修正压力
					Pp[i]=Pp[i]+dv*Con2*Dnst[0];  
					
					//liquid
					if((fsold[ix]<fs_critical)&&(att[ix]==0)&&(Tlt[ix]>TS))
					{
						NVu[ix]=NVu[ix]-dv;
					}
					if((fsold[xi]<fs_critical)&&(att[xi]==0)&&(Tlt[xi]>TS))
					{
						NVu[i]=NVu[i]+dv;
					}

					if((fsold[jy]<fs_critical)&&(att[jy]==0)&&(Tlt[jy]>TS))
					{
						NVv[jy]=NVv[jy]-dv;
					}
					if((fsold[yj]<fs_critical)&&(att[yj]==0)&&(Tlt[yj]>TS))
					{
						NVv[i]=NVv[i]+dv;
					}
					if((fsold[kz]<fs_critical)&&(att[kz]==0)&&(Tlt[kz]>TS))
					{
						NVw[kz]=NVw[kz]-dv;
					}
					if((fsold[zk]<fs_critical)&&(att[zk]==0)&&(Tlt[zk]>TS))
					{
						NVw[i]=NVw[i]+dv;
					}
				}
			}
		}
	}
	while(pjn!=0);

	for(i=0;i<(L*M*N);i++)//新时刻速度
	{
		Vv[i]=NVv[i];
		Vu[i]=NVu[i];
		Vw[i]=NVw[i];
	}
	
	/// for time step

	dd1=0.0f;
	for(i=0;i<(L*M*N);i++) 
	{
		if(fabs(Vu[i])>fabs(dd1)) 
		{
			dd1=Vu[i];  
		}
	}
	
	dd2=0.0f;
	for(i=0;i<(L*M*N);i++) 
	{
		if(fabs(Vv[i])>fabs(dd2)) 
		{
			dd2=Vv[i]; 
		}
	}
	
	dd3=0.0f;
	for(i=0;i<(L*M*N);i++)
	{
		if(fabs(Vw[i])>fabs(dd3)) 
		{
			dd3=Vw[i];
		}
	}

	if(fabs(dd2)>pj*Space_step || fabs(dd1)>pj*Space_step || fabs(dd3)>pj*Space_step)
	{
		if(fabs(dd2)>fabs(dd1)) 
		{
			dsu=dd2;
		}
		else
		{
			dsu=dd1;
		}
		
		if(fabs(dd3)>fabs(dsu)) 
		{
			dsu=dd3;
		}

		if(dsu!=0.0f)
		{
			dlt_flow=(float)(0.5*Space_step/(6.0*fabs(dsu)));//时间步长
		}
		else
		{
			dlt_flow=0.0f;
		}
	}


	CString fldtextl;
	fldtextl.Format("Ulmax=%f,Vlmax=%f, Wlmax=%f ",dd1,dd2, dd3);
	pDC->TextOut(10,50,fldtextl);


	if(NVu!=NULL)
	{
		free(NVu);
		NVu=NULL;		
	}
	if(NVv!=NULL)
	{
		free(NVv);
		NVv=NULL;		
	}

	if(NVw!=NULL)
	{
		free(NVw);
		NVw=NULL;		
	}

	if(att!=NULL)
	{
		free(att);
		att=NULL;		
	}
	if(Tlt!=NULL)
	{
		free(Tlt);
		Tlt=NULL;		
	}
}

void CMacrosegregation::Shrink(CDC *pDC)
{
	unsigned int i=0;
	unsigned int j=0;
	unsigned int k=0;

	double volume=Space_step*Space_step*Space_step;

	// calculate the toppest liquid surface for pipe formation
	
	unsigned int PIPE_k_suo1=0;
	unsigned int PIPE_k_suo2=0;
	int flage1=0;

	for(k=(NT-1);k>solid_k;k--)
	{
		for(i=0;i<LT;i++)
		{
			for(j=0;j<MT;j++)
			{
				unsigned int chan=i*MT*NT+j*NT+k;
				unsigned int subchan=(i-DDL)*M*N+(j-DDM)*N+k-DDN;

				if(Att[chan]==0)
				{
					if(fsold[subchan]<PIPEsuo)
					{
						flage1=1;
						PIPE_k_suo1=k;
						break;
					}
				}
			}
			
			if(flage1==1)
			{
				break;
			}
		}
	
		if(flage1==1)
		{
			break;
		}
	}


	flage1=0;

	for(k=(NT-1);k>solid_k;k--)
	{
		for(i=0;i<LT;i++)
		{
			for(j=0;j<MT;j++)
			{
				unsigned int chan=i*MT*NT+j*NT+k;
				unsigned int subchan=(i-DDL)*M*N+(j-DDM)*N+k-DDN;

				if(Att[chan]==pipe_hole)
				{
					if(fsold[subchan]<1.0)
					{
						flage1=1;
						PIPE_k_suo2=k;
						break;
					}
				}
			}
			
			if(flage1==1)
			{
				break;
			}
		}
	
		if(flage1==1)
		{
			break;
		}
	}

	///
	unsigned int Shrink_k_suo1=0;	
	unsigned int Shrink_k_suo2=0;

	if(PIPE_k_suo1==0 && PIPE_k_suo2==0)
	{
		flage1=0;
		for(k=(NT-1);k>solid_k;k--)
		{
			for(i=0;i<LT;i++)
			{
				for(j=0;j<MT;j++)
				{
					unsigned int chan=i*MT*NT+j*NT+k;
					unsigned int subchan=(i-DDL)*M*N+(j-DDM)*N+k-DDN;

					if(Att[chan]==0)
					{
						if(fsold[subchan]<FSsuo && fsold[subchan]>=PIPEsuo)
						{
							flage1=1;
							Shrink_k_suo1=k;
							break;
						}
					}
				}
				
				if(flage1==1)
				{
					break;
				}
			}
		
			if(flage1==1)
			{
				break;
			}
		}



		flage1=0;
		for(k=(NT-1);k>solid_k;k--)
		{
			for(i=0;i<LT;i++)
			{
				for(j=0;j<MT;j++)
				{
					unsigned int chan=i*MT*NT+j*NT+k;
					unsigned int subchan=(i-DDL)*M*N+(j-DDM)*N+k-DDN;

					if(Att[chan]==shrinkage_hole)
					{
						if(fsold[subchan]<1.0)
						{
							flage1=1;
							Shrink_k_suo2=k;
							break;
						}
					}
				}
				
				if(flage1==1)
				{
					break;
				}
			}
		
			if(flage1==1)
			{
				break;
			}
		}
	}
	

	// to judge, whether the isolated-liquid-pool could be formed
	if(solid_k==0)
	{
		for(k=(NT-1);k>0;k--)
		{
			unsigned int mid_i=(unsigned int)(((double)LT)/2.0);
			unsigned int mid_j=(unsigned int)(((double)MT)/2.0);

			unsigned int chan=mid_i*MT*NT+mid_j*NT+k;
			unsigned int subchan=(mid_i-DDL)*M*N+(mid_j-DDM)*N+k-DDN;

			if(Att[chan]==0)
			{
				if(fsold[subchan]>=FSsuo)
				{
					for(unsigned int tk=k-1;tk>0;tk--)
					{
						unsigned int downchan=mid_i*MT*NT+mid_j*NT+tk;
						unsigned int downsubchan=(mid_i-DDL)*M*N+(mid_j-DDM)*N+tk-DDN;

						if(Att[downchan]==0 && fsold[downsubchan]<FSsuo)//PIPEsuo
						{
							solid_k=tk;
						}

						if(solid_k!=0)
						{
							break;
						}
					}
					
				}//fs>FSsuo
			}//Att==0

			if(solid_k!=0)
			{
				break;
			}
		}//k
	}//solid_k==0

	// if the isolated-liquid-pool has already been formed, find the highest liquid-top position
		unsigned int solid_k_suo1=0;
		unsigned int solid_k_suo2=0;

		flage1=0;
		
		for(k=solid_k;k>0;k--)
		{
			for(i=0;i<LT;i++)
			{
				for(j=0;j<MT;j++)
				{
					unsigned int chan=i*MT*NT+j*NT+k;
					unsigned int subchan=(i-DDL)*M*N+(j-DDM)*N+k-DDN;

					if(Att[chan]==0)
					{
						if(fsold[subchan]<FSsuo)
						{
							flage1=1;
							solid_k_suo1=k;
							break;
						}
					}
				}
				
				if(flage1==1)
				{
					break;
				}
			}
		
			if(flage1==1)
			{
				break;
			}
		}//1

		
		flage1=0;
		for(k=solid_k;k>0;k--)
		{
			for(i=0;i<LT;i++)
			{
				for(j=0;j<MT;j++)
				{
					unsigned int chan=i*MT*NT+j*NT+k;
					unsigned int subchan=(i-DDL)*M*N+(j-DDM)*N+k-DDN;

					if(Att[chan]==pipe_hole || Att[chan]==shrinkage_hole)
					{
						if(fsold[subchan]<1.0)
						{
							flage1=1;
							solid_k_suo2=k;
							break;
						}
					}
				}
				
				if(flage1==1)
				{
					break;
				}
			}
		
			if(flage1==1)
			{
				break;
			}
		}//2




	// calculate contraction
	
	if(solid_k==0)
	{
		for(i=0;i<LT;i++)
		{
			for(j=0;j<MT;j++)
			{
				for(k=0;k<NT;k++)
				{
					unsigned int chan=i*MT*NT+j*NT+k;
					unsigned int subchan=(i-DDL)*M*N+(j-DDM)*N+k-DDN;

					if(Att[chan]==0 && Tt[chan]>TS)
					{
						if(Tt[chan]<Ttold[chan] && fsold[subchan]<FSsuo)// liquid contraction
						{
							allsuo=allsuo+Lsuo*(Ttold[chan]-Tt[chan])*volume;
							allmysuo=allmysuo+Lsuo*(Ttold[chan]-Tt[chan])*volume;
						}
						if(fsold[subchan]>subfsold[subchan] && fsold[subchan]<FSsuo)// solid contraction
						{
							allsuo=allsuo+Ssuo*(fsold[subchan]-subfsold[subchan])*volume;
							allmysuo=allmysuo+Ssuo*(fsold[subchan]-subfsold[subchan])*volume;
						}
					}
				}//k
			}//j
		}//i

		
	}
	else
	{
		for(i=0;i<LT;i++)
		{
			for(j=0;j<MT;j++)
			{
				for(k=solid_k+1;k<NT;k++)
				{
					unsigned int chan=i*MT*NT+j*NT+k;
					unsigned int subchan=(i-DDL)*M*N+(j-DDM)*N+k-DDN;

					if(Att[chan]==0 && Tt[chan]>TS)
					{
						if(Tt[chan]<Ttold[chan] && fsold[subchan]<FSsuo)// liquid contraction
						{
							allsuo=allsuo+Lsuo*(Ttold[chan]-Tt[chan])*volume;
							allmysuo=allmysuo+Lsuo*(Ttold[chan]-Tt[chan])*volume;
						}
						if(fsold[subchan]>subfsold[subchan] && fsold[subchan]<FSsuo)// solid contraction
						{
							allsuo=allsuo+Ssuo*(fsold[subchan]-subfsold[subchan])*volume;
							allmysuo=allmysuo+Ssuo*(fsold[subchan]-subfsold[subchan])*volume;
						}
					}
				}//k
			}//j
		}//i

		for(i=0;i<LT;i++)
		{
			for(j=0;j<MT;j++)
			{
				for(k=0;k<=solid_k;k++)
				{
					unsigned int chan=i*MT*NT+j*NT+k;
					unsigned int subchan=(i-DDL)*M*N+(j-DDM)*N+k-DDN;

					if(Att[chan]==0 && Tt[chan]>TS)
					{
						if(Tt[chan]<Ttold[chan] && fsold[subchan]<FSsuo)// liquid contraction
						{
							allsuo_solidk=allsuo_solidk+Lsuo*(Ttold[chan]-Tt[chan])*volume;
							allmysuo=allmysuo+Lsuo*(Ttold[chan]-Tt[chan])*volume;
						}
						if(fsold[subchan]>subfsold[subchan] && fsold[subchan]<FSsuo)// solid contraction
						{
							allsuo_solidk=allsuo_solidk+Ssuo*(fsold[subchan]-subfsold[subchan])*volume;
							allmysuo=allmysuo+Ssuo*(fsold[subchan]-subfsold[subchan])*volume;
						}
					}
				}//k
			}//j
		}//i


	}

	

	// calculate the movement of top-liquid-surface: formation of pipe

		unsigned int PIPE_k_suo=0;//NT-1;
		if(PIPE_k_suo1>=PIPE_k_suo2)
		{
			PIPE_k_suo=PIPE_k_suo1;
		}
		else
		{
			PIPE_k_suo=PIPE_k_suo2;
		}

		unsigned int Shrink_k_suo=0;//NT-1;
		if(Shrink_k_suo1>=Shrink_k_suo2)
		{
			Shrink_k_suo=Shrink_k_suo1;
		}
		else
		{
			Shrink_k_suo=Shrink_k_suo2;
		}


		unsigned int solid_k_suo=0;	
		if(solid_k_suo1>=solid_k_suo2)
		{
			solid_k_suo=solid_k_suo1;
		}
		else
		{
			solid_k_suo=solid_k_suo2;
		}

		
		double allsolute=0.0;
		int flage=0;
	
		/// for pipe formation

		if(PIPE_k_suo!=0)
		{
			k=PIPE_k_suo;

			do
			{
				PIPE_k_suo=k;
				
				double all_pipe_suo=0.0;
				double numberhole=0.0;

				for(i=0;i<LT;i++)
				{
					for(j=0;j<MT;j++)
					{
						unsigned int chan=i*MT*NT+j*NT+k;
						unsigned int subchan=(i-DDL)*M*N+(j-DDM)*N+k-DDN;

						if((Att[chan]==0)&&(Tt[chan]>TS))
						{
							if(fsold[subchan]<PIPEsuo)//FSsuo
							{
								all_pipe_suo=all_pipe_suo+volume*(1.0-fsold[subchan]);
								numberhole=numberhole+1.0;
							}
						}

						
						if(Att[chan]==pipe_hole )
						{
							if(fsold[subchan]<1.0)
							{
								all_pipe_suo=all_pipe_suo+volume*(1.0-fsold[subchan]);
								numberhole=numberhole+1.0;
							}
						}
					}//j
				}//i

			
				if(allsuo>=all_pipe_suo)
				{// set void volume

						for(i=0;i<LT;i++)
						{
							for(j=0;j<MT;j++)
							{
								unsigned int chan=i*MT*NT+j*NT+k;
								unsigned int subchan=(i-DDL)*M*N+(j-DDM)*N+k-DDN;

								if((Att[chan]==0)&&(Tt[chan]>TS))
								{
									if(fsold[subchan]<PIPEsuo)
									{
										Att[chan]=pipe_hole;
										TH[chan]=800.0*Dnst[pipe_hole]*Spht[pipe_hole];//air
												
										allsolute=allsolute+(CL[subchan]-m_fcarbon)*(1.0-fsold[subchan]);
										
										subfsold[subchan]=fsold[subchan];
										fsold[subchan]=1.0;
									}
									
								}

								if(Att[chan]==pipe_hole)
								{
									if(fsold[subchan]<1.0)
									{
										//Att[chan]=pipe_hole;
										//TH[chan]=Tt[chan]*Dnst[pipe_hole]*Spht[pipe_hole];//air
										//TH[chan]=800.0*Dnst[pipe_hole]*Spht[pipe_hole];
												
										allsolute=allsolute+(CL[subchan]-m_fcarbon)*(1.0-fsold[subchan]);
										
										//subfsold[subchan]=fsold[subchan];
										fsold[subchan]=1.0;
									}
								}
							
							}//j
						}//i
						
						allsuo=allsuo-all_pipe_suo;
						all_pipe_suo=0.0;
						
						if(allsuo==0.0)
						{
							flage=1;
						}
						else
						{
							if(k>solid_k+1)
							{
								k=k-1;
							}
							else
							{
								flage=1;
							}
						}

				}//>all_psuo>=all_pipe_suo
				else
				{
					if(numberhole!=0.0)
					{	
						for(i=0;i<LT;i++)
						{
							for(j=0;j<MT;j++)
							{
								unsigned int chan=i*MT*NT+j*NT+k;
								unsigned int subchan=(i-DDL)*M*N+(j-DDM)*N+k-DDN;

								if((Att[chan]==0)&&(Tt[chan]>TS))
								{
									if(fsold[subchan]<PIPEsuo)
									{
										Att[chan]=pipe_hole;
										TH[chan]=800.0*Dnst[pipe_hole]*Spht[pipe_hole];//air
												
										allsolute=allsolute+(CL[subchan]-m_fcarbon)*(1.0-fsold[subchan]);
										
										subfsold[subchan]=fsold[subchan];
										fsold[subchan]=fsold[subchan]+allsuo/(volume*numberhole);
										
										if(fsold[subchan]>1.0)
										{
											fsold[subchan]=1.0;
										}
									}

								
								}// Att==0 && Tt>TS
								else
								{

									if(Att[chan]==pipe_hole)
									{
										if(fsold[subchan]<1.0)
										{
											//Att[chan]=pipe_hole;
											//TH[chan]=800.0*Dnst[pipe_hole]*Spht[pipe_hole];
													
											allsolute=allsolute+(CL[subchan]-m_fcarbon)*(1.0-fsold[subchan]);
											
											//subfsold[subchan]=fsold[subchan];
											fsold[subchan]=fsold[subchan]+allsuo/(volume*numberhole);
											
											if(fsold[subchan]>1.0)
											{
												fsold[subchan]=1.0;
											}
										}
									}
								}//else
							}//j
						}//i

						allsuo=0.0;
						flage=1;
						
					}//numberhole!=0.0
				}//else

				} while(flage==0);
			}//PIPE_k_suo!=0


		/// for macroshrinkage formation
		if(Shrink_k_suo!=0)
		{
			k=Shrink_k_suo;

			do
			{	
				double all_pipe_suo=0.0;
				double numberhole=0.0;

				for(i=0;i<LT;i++)
				{
					for(j=0;j<MT;j++)
					{
						unsigned int chan=i*MT*NT+j*NT+k;
						unsigned int subchan=(i-DDL)*M*N+(j-DDM)*N+k-DDN;

						if((Att[chan]==0)&&(Tt[chan]>TS))
						{
							if(fsold[subchan]<FSsuo && fsold[subchan]>=PIPEsuo)
							{
								all_pipe_suo=all_pipe_suo+volume*(1.0-fsold[subchan]);
								numberhole=numberhole+1.0;
							}
						}

						
						if(Att[chan]==shrinkage_hole)
						{
							if(fsold[subchan]<1.0)
							{
								all_pipe_suo=all_pipe_suo+volume*(1.0-fsold[subchan]);
								numberhole=numberhole+1.0;
							}
						}
					}//j
				}//i

			
				if(allsuo>=all_pipe_suo)
				{// set void volume

						for(i=0;i<LT;i++)
						{
							for(j=0;j<MT;j++)
							{
								unsigned int chan=i*MT*NT+j*NT+k;
								unsigned int subchan=(i-DDL)*M*N+(j-DDM)*N+k-DDN;

								if((Att[chan]==0)&&(Tt[chan]>TS))
								{
									if(fsold[subchan]>=PIPEsuo && fsold[subchan]<FSsuo)
									{
										Att[chan]=shrinkage_hole;
										TH[chan]=800.0*Dnst[shrinkage_hole]*Spht[shrinkage_hole];//air
												
										allsolute=allsolute+(CL[subchan]-m_fcarbon)*(1.0-fsold[subchan]);
										
										subfsold[subchan]=fsold[subchan];
										fsold[subchan]=1.0;
									}
									
								}

								if(Att[chan]==shrinkage_hole)
								{
									if(fsold[subchan]<1.0)
									{
										//Att[chan]=pipe_hole;
										//TH[chan]=Tt[chan]*Dnst[pipe_hole]*Spht[pipe_hole];//air
										//TH[chan]=800.0*Dnst[pipe_hole]*Spht[pipe_hole];
												
										allsolute=allsolute+(CL[subchan]-m_fcarbon)*(1.0-fsold[subchan]);
										
										//subfsold[subchan]=fsold[subchan];
										fsold[subchan]=1.0;
									}
								}
							
							}//j
						}//i
						
						allsuo=allsuo-all_pipe_suo;
						all_pipe_suo=0.0;
						
						if(allsuo==0.0)
						{
							flage=1;
						}
						else
						{
							if(k>solid_k+1)
							{
								k=k-1;
							}
							else
							{
								flage=1;
							}
						}

				}//>all_psuo>=all_pipe_suo
				else
				{
					if(numberhole!=0.0)
					{	
						for(i=0;i<LT;i++)
						{
							for(j=0;j<MT;j++)
							{
								unsigned int chan=i*MT*NT+j*NT+k;
								unsigned int subchan=(i-DDL)*M*N+(j-DDM)*N+k-DDN;

								if((Att[chan]==0)&&(Tt[chan]>TS))
								{
									if(fsold[subchan]>=PIPEsuo && fsold[subchan]<FSsuo )
									{
										Att[chan]=shrinkage_hole;
										TH[chan]=800.0*Dnst[shrinkage_hole]*Spht[shrinkage_hole];//air
												
										allsolute=allsolute+(CL[subchan]-m_fcarbon)*(1.0-fsold[subchan]);
										
										subfsold[subchan]=fsold[subchan];
										fsold[subchan]=fsold[subchan]+allsuo/(volume*numberhole);
										
										if(fsold[subchan]>1.0)
										{
											fsold[subchan]=1.0;
										}
									}

								
								}// Att==0 && Tt>TS
								else
								{
									if(Att[chan]==shrinkage_hole)
									{
										if(fsold[subchan]<1.0)
										{
											//Att[chan]=pipe_hole;
											//TH[chan]=800.0*Dnst[pipe_hole]*Spht[pipe_hole];
													
											allsolute=allsolute+(CL[subchan]-m_fcarbon)*(1.0-fsold[subchan]);
											
											//subfsold[subchan]=fsold[subchan];
											fsold[subchan]=fsold[subchan]+allsuo/(volume*numberhole);
											
											if(fsold[subchan]>1.0)
											{
												fsold[subchan]=1.0;
											}
										}
									}
								}//else
							}//j
						}//i

						allsuo=0.0;
						flage=1;
						
					}//numberhole!=0.0
				}//else

				} while(flage==0);
			}//Shrink_k_suo!=0
			

			flage=0;
			if(solid_k_suo!=0)
			{
				k=solid_k_suo;

				do
				{
					double all_pipe_suo=0.0;
					double numberhole=0.0;

					for(i=0;i<LT;i++)
					{
						for(j=0;j<MT;j++)
						{
							unsigned int chan=i*MT*NT+j*NT+k;
							unsigned int subchan=(i-DDL)*M*N+(j-DDM)*N+k-DDN;

							if((Att[chan]==0)&&(Tt[chan]>TS))
							{
								if(fsold[subchan]<FSsuo)
								{
									all_pipe_suo=all_pipe_suo+volume*(1.0-fsold[subchan]);
									numberhole=numberhole+1.0;
								}
							}

							
							if(Att[chan]==pipe_hole || Att[chan]==shrinkage_hole)
							{
								if(fsold[subchan]<1.0)
								{
									all_pipe_suo=all_pipe_suo+volume*(1.0-fsold[subchan]);
									numberhole=numberhole+1.0;
								}
							}
						}//j
					}//i

				
					if(allsuo_solidk>=all_pipe_suo)
					{// set void volume

							for(i=0;i<LT;i++)
							{
								for(j=0;j<MT;j++)
								{
									unsigned int chan=i*MT*NT+j*NT+k;
									unsigned int subchan=(i-DDL)*M*N+(j-DDM)*N+k-DDN;

									if((Att[chan]==0)&&(Tt[chan]>TS))
									{
										if(fsold[subchan]<PIPEsuo)
										{
											Att[chan]=pipe_hole;
											TH[chan]=800.0*Dnst[pipe_hole]*Spht[pipe_hole];//air
													
											allsolute=allsolute+(CL[subchan]-m_fcarbon)*(1.0-fsold[subchan]);
											
											subfsold[subchan]=fsold[subchan];
											fsold[subchan]=1.0;
										}
										if(fsold[subchan]>=PIPEsuo && fsold[subchan]<FSsuo)
										{
											Att[chan]=shrinkage_hole;
											TH[chan]=800.0*Dnst[shrinkage_hole]*Spht[shrinkage_hole];//air
													
											allsolute=allsolute+(CL[subchan]-m_fcarbon)*(1.0-fsold[subchan]);
											
											subfsold[subchan]=fsold[subchan];
											fsold[subchan]=1.0;
										}
									}

									if(Att[chan]==pipe_hole)
									{
										if(fsold[subchan]<1.0)
										{			
											allsolute=allsolute+(CL[subchan]-m_fcarbon)*(1.0-fsold[subchan]);
											fsold[subchan]=1.0;
										}
									}
									if(Att[chan]==shrinkage_hole)
									{
										if(fsold[subchan]<1.0)
										{
											allsolute=allsolute+(CL[subchan]-m_fcarbon)*(1.0-fsold[subchan]);
											fsold[subchan]=1.0;
										}
									}
								}//j
							}//i
							
							allsuo_solidk=allsuo_solidk-all_pipe_suo;
							all_pipe_suo=0.0;
							
							if(allsuo_solidk==0.0)
							{
								flage=1;
							}
							else
							{
								if(k>1)
								{
									k=k-1;
								}
								else
								{
									flage=1;
								}
							}

					}//>allsuo_solidk>=all_pipe_suo
					else
					{
						if(numberhole!=0.0)
						{	
							for(i=0;i<LT;i++)
							{
								for(j=0;j<MT;j++)
								{
									unsigned int chan=i*MT*NT+j*NT+k;
									unsigned int subchan=(i-DDL)*M*N+(j-DDM)*N+k-DDN;

									if((Att[chan]==0)&&(Tt[chan]>TS))
									{
										if(fsold[subchan]<PIPEsuo)
										{
											Att[chan]=pipe_hole;
											TH[chan]=800.0*Dnst[pipe_hole]*Spht[pipe_hole];//air
													
											allsolute=allsolute+(CL[subchan]-m_fcarbon)*(1.0-fsold[subchan]);
											
											subfsold[subchan]=fsold[subchan];
											fsold[subchan]=fsold[subchan]+allsuo_solidk/(volume*numberhole);
											
											if(fsold[subchan]>1.0)
											{
												fsold[subchan]=1.0;
											}
										}

										if(fsold[subchan]>=PIPEsuo && fsold[subchan]<FSsuo)
										{
											Att[chan]=shrinkage_hole;
											TH[chan]=800.0*Dnst[shrinkage_hole]*Spht[shrinkage_hole];
													
											allsolute=allsolute+(CL[subchan]-m_fcarbon)*(1.0-fsold[subchan]);
											
											subfsold[subchan]=fsold[subchan];
											fsold[subchan]=fsold[subchan]+allsuo_solidk/(volume*numberhole);
											
											if(fsold[subchan]>1.0)
											{
												fsold[subchan]=1.0;
											}
										}
									}// Att==0 && Tt>TS
									else
									{

										if(Att[chan]==pipe_hole)
										{
											if(fsold[subchan]<1.0)
											{
												//Att[chan]=pipe_hole;
												//TH[chan]=800.0*Dnst[pipe_hole]*Spht[pipe_hole];
														
												allsolute=allsolute+(CL[subchan]-m_fcarbon)*(1.0-fsold[subchan]);
												
												//subfsold[subchan]=fsold[subchan];
												fsold[subchan]=fsold[subchan]+allsuo_solidk/(volume*numberhole);
												
												if(fsold[subchan]>1.0)
												{
													fsold[subchan]=1.0;
												}
											}
										}

										if(Att[chan]==shrinkage_hole)
										{
											if(fsold[subchan]<1.0)
											{
												//Att[chan]=shrinkage_hole;
												//TH[chan]=800.0*Dnst[shrinkage_hole]*Spht[shrinkage_hole];
														
												allsolute=allsolute+(CL[subchan]-m_fcarbon)*(1.0-fsold[subchan]);
												
												//subfsold[subchan]=fsold[subchan];
												fsold[subchan]=fsold[subchan]+allsuo_solidk/(volume*numberhole);
												
												if(fsold[subchan]>1.0)
												{
													fsold[subchan]=1.0;
												}
											}
										}
									}//else
								}//j
							}//i

							allsuo_solidk=0.0;
							flage=1;
							
						}//numberhole!=0.0
					}//else

				} while(flage==0);
			}// solid_k!=0

		
	CString fldtextl;  
	fldtextl.Format("Max_k=%d ",PIPE_k_suo);
	pDC->TextOut(1000,50,fldtextl);

	fldtextl.Format("Shrink_k_suo=%d ",Shrink_k_suo);
	pDC->TextOut(1000,100,fldtextl);

	fldtextl.Format("Solid_k=%d ",solid_k);
	pDC->TextOut(1000,150,fldtextl);

	fldtextl.Format("allsuo=%f ",allsuo);
	pDC->TextOut(1000,200,fldtextl);

	fldtextl.Format("allmysuo=%f ",allmysuo);
	pDC->TextOut(1000,250,fldtextl);

}
