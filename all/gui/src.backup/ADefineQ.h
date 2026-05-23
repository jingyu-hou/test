#ifndef  AD_DEFINED
#define  AD_DEFINED

#include <QString>
#include <QStringList>
#include <QVector>
#include <QtGui/QMenu>
using namespace std;


#define PLOTOPTNAME "/CommonPlot.cfg"
#define PLOTOPTNAMESTART "PlotOption"
#define READRESULT "/CommonVTKtext.cfg"
#define READRESULTSTART "ReadResult"
enum MaterialENUM{
	M_ENUM_DENSITY=0,
	M_ENUM_ELASTIC=1,
	M_ENUM_PLASTIC=2,
	M_ENUM_EXPANS=3,
	M_ENUM_CONDUCT=4,
	M_ENUM_SPECIFICE=5,
    M_ENUM_METALPOWDER=6,//��ĩ����
    M_ENUM_Rate_Dependent_Plastic=7,//�����������
    M_ENUM_Damage=8,//����
	M_ENUM_Dynamic_Recrystallization=9
};
enum NElSufENUM{
    COM_ENUM_NSET =0,  //�ڵ�
    COM_ENUM_ELSET =1, //��Ԫ
    COM_ENUM_SURFSET=2,//����
};

//Menu���Step ��������
typedef struct{
    //QMenu *m_Menu;
    //QAction* m_Action;
    //QMenu *m_SubMenu[2000];
    QStringList m_MenuName;//L101
    vector<QStringList> m_subMenuName;//DISP
    vector<QStringList> m_sub2MenuName;//D1
}MenuSubMenu;
typedef struct{
    MenuSubMenu m_Menu;
    QString m_VisName;//�ļ���
}ResultOutputS;
//--

//-- material chart
struct _WELCME_CHART_PROP_{
    QString Title;//ͼ������(none)
    //char *Label;//��ǩ
    QStringList strListLabel;
    QString xLabel;//x��ǩ(none)
    QString yLabel;//y��ǩ
    QString yLabel2;//y2��ǩ
};

//--���� 
typedef struct _MATERIAL_STRUCT_{
	QString Name;   //����
	QString	Styles; //����
	QString	*Data;  //����
    QStringList MaterList;//comList
    int MaterListIndex;//com�еĵڼ���
    int id;
	_MATERIAL_STRUCT_(){
		//Name =    "Material";
		Name =    "";
		Styles =  "Elastic";
        id = 1;
        MaterListIndex=0;
	}	
}MaterialS,*pMaterialS;

//--����
typedef struct _SECTION_STRUCT_{
    QString Name;   //����
    QString	strCategory; //����
    QString	Styles;  //����
    int id;
    MaterialS   MaterPara;
    _SECTION_STRUCT_(){
        Name = "Section";
        strCategory = "Sollid";
        Styles = "Homogeneous";
        id = 1;
        //MaterList.clear();
    }

}SectionS,*pSectionS;


typedef struct _TOTAL_STRUCT_{
 SectionS    SectionPara;
 MaterialS   MaterPara;
}TotalS;
typedef struct NPOINT_STRUCT_
{
	double x;
	double y;
	double z;
}PointCoordinate;

//--��ɫ����(vtk)
typedef struct _COLOR_S_{
    double r;
    double g;
    double b;
    _COLOR_S_(){
        r=0.0;
        g=0.0;
        b=1.0;

		/*r=0.0;
        g=1.0;
        b=0.0;*/
    }
}VTKColorS;


//--���inp 
typedef struct _S_MHCELL_S_ 
{
    QString strName;
    QStringList Data;
    _S_MHCELL_S_()
    {
        strName="";
     Data.clear();
    }
}HideMCell,*pHideMCell;

//--Material Inp
//description:
typedef struct  _OUTPUTMATER_Inp_S_{
  QString strCommentary;// ע��MATERIALS
  QString strMaterialName;//����Material-1
  QStringList strMaterialStyleName;//��������Plastic��Elastic...
  QStringList strMaterialStyleParam;//expansion -->zero(��ѡ����);
  QStringList strMaterialStyleParam2;
  QStringList strSubMaterialName;//Young's Modulus Poisson's Ratio...
  QStringList strSubMCol;//plastic,Elastic����
  QStringList strSubMCol2;//������ĩ���ԣ��ڶ�ҳ������
  //QStringList strDataX;
  //QStringList strDataY1;
  QVector<QStringList*> strData[20];
  QStringList strMetalPowerData;//������ĩ��������
  //QVector <HideMCell> strData; //��Ӧ�����������ݣ���eg:Plastic��
  int dataColNum;//�����и���
  int dataRowNum;//�����и���
  int dataColNum2;//����Tab�����ݣ��ڶ�ҳ��������
  int dataRowNum2;
  int DateVectorNum;//��������
  int iPageIndex;
  _OUTPUTMATER_Inp_S_(){
      dataColNum=0;
      dataRowNum=0;
      dataColNum2=0;
      dataRowNum2=0; 
      DateVectorNum=0;
      iPageIndex=0;
      strMaterialStyleParam.clear();
      strMaterialStyleName.clear();
  }
}MaterialInpS;

//--Section Inp
typedef struct  _OUTPUTSECTION_Inp_S_{
    QString strCommentaryName;//*section:�Ͻ��ĩ
    QString strSectionStyle;//solid (solid Section)
	QString strPartType;//��������(���壬�����ԡ�����)
    QString strelset;//��Ԫ����
    QString StrMaterial;//��ǰMaterialѡ��
    QString PartName;
    _OUTPUTSECTION_Inp_S_(){
		PartName="";
        strCommentaryName = "�Ͻ��ĩ";
        strSectionStyle ="Solid Section";
        strelset ="POWER";
        StrMaterial ="METAL_POWDER";
    }
}SectionInpS;
//--Curve Inp 
typedef struct _OUTPUTCurve_Inp_S_{
    QString strCommentaryName;//*���߶���
    QString strCurveStyle;//*Amplitude��
    QString strName;//���ߵ�����
    QString strTime;//?
    QStringList strDataX;
    QStringList strDataY;

    int ThirdStrTime;//0-NO="",1-YES time=��TOTAL TIME��
    _OUTPUTCurve_Inp_S_(){
        strCurveStyle="Amplitude";
        strTime = "TOTAL TIME";
        ThirdStrTime =1;
    }
    //--�����ж����
    bool operator == (const _OUTPUTCurve_Inp_S_& t)
    {
        QString strTmpX=t.strDataX.join(",");
        QString strTmpY=t.strDataY.join(",");
        QString strRDataX=strDataX.join(",");
        QString strRDataY=strDataY.join(",");
        int x=QString::compare(strTmpX,strRDataX,Qt::CaseInsensitive);
        int y=QString::compare(strTmpY,strRDataY,Qt::CaseInsensitive);
        return (strName == t.strName);//&&(strDataX==t.strDataX);
    }
   /* _OUTPUTCurve_Inp_S_ operator replace(const _OUTPUTCurve_Inp_S_& t)
    {   
        return t;
    }*/
}InpCurveInpS;
//--BC Inp
typedef struct  _OUTPUTBC_Inp_S_{
    QString strCommentaryName;//�߽�����
    QString strBoundaryName;//*Boundary;
    QString strPSetName;//Dis,Axia
    QString strURstyle;//���ɶȣ�1-x,2-y,3-z,4-ux,5-uy,6-zy��
	QString SportType;//0����λ�ƣ�1�����ٶ�;2�������˶�
	QString ParametreType;//��������ʱ��仯���¶�
	QString Parametre;
	QString strAmplitudeName;
	QString ScalingFactor;//����ϵ��
	QString SolutionName;
	QString StepNumber;//0,1,2,3,......
   
	_OUTPUTBC_Inp_S_(){
		strCommentaryName="";
		strBoundaryName="";
		strPSetName="";
		strURstyle="";
		SportType="��";
		ParametreType="����";
		strAmplitudeName="";
		ScalingFactor="1";
		SolutionName="";
		StepNumber="";
		Parametre="";
	}
}BCInpS;

//--HIPsystem Inp(�ȵȾ�ѹ�ƶ�)
typedef struct  _OUTPUTHIPSystem_Inp_S_{
    QString strCommentaryName;//ע��**ѹ���غ�
    QString strStyle;//Boundary/Dsload
    QString strAmplitudeName;//��Ӧ���ߵ�����
    QString strPsetName;//�㼯��
    QString strCofficient;//����ϵ��
	QString SolutionName;
	QString StepNumber;//0,1,2,3,......
}HIPSystemInpS;

//--HIPSolve Inp(�ȵȾ�ѹ���������)
typedef struct  _OUTPUTHIPSolve_Inp_S_{
    QString strCommentaryName;
    QString strStepInc;//������沽��
    QString strNlgeom; //���/����ϣ�YES/NO��
    QString strNlgeomSTR; //���/����ϣ����Ӣ�ģ�
	QString strProcess;//��������:���̡��ȵȾ�ѹ�����졢�ȴ���
    QString strDelMX;//����¶�����50
    QString strInitStep;//��ʼ����
    QString strTotalTime;//ģ����ʱ��
    QString strMinStep;//��С����
    QString strMaxStep;//��󲽳�
	QString TimeFrequencyInc;

    _OUTPUTHIPSolve_Inp_S_(){
        strCommentaryName = "���������";
        strStepInc ="100000";
        strNlgeom ="YES";//NO
		strProcess="";
        strNlgeomSTR="coupled Temperature-displacement";//(uncoupled...)
        strDelMX="50";
        strInitStep="10";
        strTotalTime="1000";
        strMaxStep="20";
		TimeFrequencyInc="10";
    }
}HIPSolveInpS;


//Init 
typedef struct  _OUTPUTINIT_Inp_S_{
    QString strCommentaryName;
    QString strInitCondition;//"Initial Conditions"
    QString strInitTypeName;//"TEMPERATURE"
    QString strPSetName;//"���ϵ�����"
    QString strVal;//��ֵ.
	QString IncludeName;//�������ⲿ�����ļ� luo
    _OUTPUTINIT_Inp_S_(){
        strCommentaryName ="��ʼ����";
        strInitCondition="Initial Conditions";//"Initial Conditions"
        strInitTypeName="TEMPERATURE";//"TEMPERATURE"
        strPSetName="Whole";//"���ϵ�����"
        strVal="20";//��ֵ.
    }
}InitInpS;
//--���������� Inp
typedef struct  _OUTPUTVariable_Inp_S_{
    QString strCommentaryName;//Frd������
    QString strNodeFile;//�ڵ����NODE FILE(dat:NODE PRINT)
    bool bELFileShow;//�Ƿ����EL FILE�ؼ��ּ����ݣ�0=�������1=�����
    QString strELFile;//��Ԫ����EL FILE(dat:EL PRINT)
    QString strTimeOrFreqName;//TIME POINTS/FREQUENCY
    QString strELInclude;
    QString strNInclude;
    QString strTname;//T1,T10
	QString NodeSet;//�㼯������
	QStringList NodeSet1;//�㼯������
    QString Igforce;//�Ƿ����ѹ�����ߣ�0�����������1���������
	_OUTPUTVariable_Inp_S_(){
		Igforce="0";
	}
}VariableInpS;

//--Inp �ڵ㶨��
typedef struct _INPUT_NODE_Inp_S_{
    QString strNodeTitle;//*Node
    QList<QString> strData;
	QString PartName;
    _INPUT_NODE_Inp_S_(){
        strNodeTitle="*Node";
		PartName="";
    }
    void clear(){
        strNodeTitle="";
        strData.clear();
    }

}NodeInpS;
//--Inp ��Ԫ�ڵ㶨��
typedef struct _INPUT_EL_Inp_S_{
    QString strELTitle; //*Element
    QString strELType;  //CAX4
    QString strELSET;   //��ѡ����
    QList<QString> strData;    //��������
	QList<int>NumberE;//luo
	QList<QString>ElementType;//luo
	QString PartName;
    void clear(){
		PartName="";
        strELTitle="";
        strELType="";
        strELSET="";
        strData.clear();
		NumberE.clear();//luo
		ElementType.clear();//luo
    }
}ELInpS;
//--Inp �ڵ㼯��
typedef struct _INPUT_NSet_Inp_S_{
    QString strNSetTitle; //*Nset
    QStringList strNSetName;  //nset=Axia,Dis,whole,temp,n1
    QStringList strNSetStyle;  //Generate
    //QList<QString> strData;//��������
    QList<QStringList> strData;
	QString PartName;
    void clear(){
		PartName="";
        strNSetTitle="";
        strNSetName.clear();
        strNSetStyle.clear();
        strData.clear();
    }
}NSetInpS;
//--Inp ��Ԫ����
typedef struct _INPUT_ELSet_Inp_S_{
    QString strELSetTitle; //*Elset
    QStringList strElSetName;  //elset=POWER,CANISTER,e1...
    QStringList strElSetStyle;  //Generate
    QStringList strData;//��������(ֻ��һ��)//2020-6-18��ֻ��һ�У�����
	QString PartName;
    void clear(){
		PartName="";
        strELSetTitle="";
        strElSetName.clear();
        strElSetStyle.clear();
        strData.clear();
    }
}ElSetInpS;
//--Inp ���漯��
/*Surface, type=ELEMENT, name=press
press_S4, S4
press_S2, S2
*/
typedef struct _INPUT_SURFACE_Inp_S_{
    QString strSurfaceTitle; //*Surface
    QString strSurfaceType;  //type=ELEMENT,NODE
    QStringList strSurfaceName;  //name = press//(Surf-1,Surf-222)
    QStringList strSurfaceDataNum;//ÿ��press�°����м���
    QList<QString> strData;  //��������press_S4, S4 press_S2, S2
	QString PartName;
    void clear(){
		PartName="";
        strSurfaceTitle="";
        strSurfaceType="";
        strSurfaceName.clear();
        strSurfaceDataNum.clear();
        strData.clear();
    }
}SurfaceInpS;

//--���������ڵ㡢��Ԫ�����漯�ϵ���Ϣ
typedef struct _INPUT_TotalELPSET_Inp_S_{
    NodeInpS     NodeInpData;
    ELInpS       ELInpData;
    NSetInpS     NSetInpData;
    ElSetInpS    ElSetInpData;
    SurfaceInpS  SurfaceInpData;
}NodeELSetInps;

//--gmsh�е�Ԫ���ڵ��ϡ���Ԫ���ϡ����漯�ϣ�
typedef struct _GMSH_INP_S_{
    NodeInpS NodeInpData;
    NSetInpS gmshNSetData;
    QList<ELInpS> gmshELData;
    
    ElSetInpS gmshELSetData;
}GmshInpS;

//--�����е���Ϣtype+str
typedef struct _S_NELSurfset_S_{
    QString strName;//�㼯�ϡ���Ԫ���ϡ����漯��
    int iType;//����ѡ��  <0:�㼯��;1:��Ԫ����;2:���漯��>
}NElSurfChsS;

//HP INP���ȴ�����-----
typedef struct _S_HP_TTT_S_{//--TTT/CCT
    QString strName;//*PHASECURVE,
    QString strPhaseCurveType;//type = TTT/CCT
    QStringList strHeadParam;//1,1,0,1.,1.0(����ת�����1�����ݵ�����ת�����2����ֵ)
    QStringList strData;//���ݵ���Ϊ0����û�����ݣ���Ϊ0����Ϊ����������;
}HPphaseCurveInps;

typedef struct _S_HP_PHASEQUILIBRIUM_S_{//��ƽ��
    QString strName;//*PHASEEQUILIBRIUM
    QString strType;//no
    QStringList strHeadParam;//2,9(�����ܵ���)
    QStringList strData;//������
}HPphaseBalanceInps,HPincubatePeriodInps,HPPhasePropSpecInps,HPPhasePropCondInps,HPPhaseZBFInps;

typedef struct _S_HP_PHASEELATENTHEAT_S_{//Ǳ��//Ӳ��
    QString strName;//*PHASELATENTHEAT
    QStringList strHeadParam;//2,102.0(������ֵ)
}HPphaseElateHeatInps,HPphaseHardInps;

typedef struct _S_HP_PHASECTROL_S_{//�ȴ�����ʽ
    QString strName;//*PHASECTROL//*PHASEGS //*PHASEYS
    QString strHeadParam;//
}HPphaseCtrolInps,HPPhaseGSInps,HPPhaseYSInps;


typedef struct _S_HP_TTT_TOTAL_S_{//--TTT/CCT
    HPphaseCurveInps    PhaseCurveInps;
    HPphaseBalanceInps   PhaseBalanceInps;
    HPphaseElateHeatInps PhaseElateHeatInps;
    HPphaseHardInps      PhaseHardInps;
    HPphaseCtrolInps     PhaseCtrolInps;
    HPincubatePeriodInps IncubatePeriodInps;
    HPPhasePropSpecInps  PhasePropSpecInps;
    HPPhasePropCondInps  PhasePropCondInps;
    HPPhaseYSInps        PhaseYSInps;
    HPPhaseGSInps        PhaseGSInps;
    HPPhaseZBFInps       PhaseZBFInps;
}HPTTTInps;
//--HPsystem Inp(�ȴ����ƶ�)
//Film/radiate(���ļ��ж�ȡ��������)
typedef struct  _OUTPUTHPSystemFR_Inp_S_{
    QString strStep;//step�ڼ�����
    QString strCommentaryName;//ע��**
	QString strProcess;
    QString strStyle;//film/radiate
    QString strAmplitudeName;//��Ӧ���ߵ�����
    QString strFilmRadiateAmpName;//FilmAmp/raditeAmpName
    QString strOP;//op = new
    QString strInclude;//eg.�� radiate������3��
                        //*radiate, op=NEW
                        //_Surf-3_S1, R1, 900., 0.4
                        //_Surf-3_S2, R2, 900., 0.4 
                        //_Surf-3_S3, R3, 900., 0.4
    QString strElsetName;//��Ԫ����
    QString strSurfaceNum;//���
    QString strEnvirTemprat;//�����¶�
    QString strCofficient;//ϵ��(Ĥϵ��,����ϵ��)

    void clear(){
        strStep="";
		strProcess="";
        strCommentaryName="";
        strStyle="";
        strAmplitudeName="";
        strInclude="";
        strElsetName="";
        strSurfaceNum="";
        strEnvirTemprat="";
        strCofficient="";
        strOP="";
    }
}HPSystemFInpS,HPSystemRInpS;
////--�ȴ��� --�ӽ�����д�뵽�ļ��У�
//typedef struct  _OUTPUTHPSystem_Inp_S_{
//    QString strStep;//step
//    QString strCommentaryName;//ע��**
//    QString strStyle;//film/radiate
//    QString strAmplitudeName;//��Ӧ���ߵ�����
//    QString strFilmRadiateAmpName;//FilmAmp/raditeAmpName
//    QString strOP;//op = new
//    QString strInclude;//eg.�� radiate������3��
//    QString strElsetName;//��Ԫ����
//    QString strRElsetName;//R����
//    QString strSurfaceNum;//���
//
//    QString strEnvirTemprat;//�����¶�
//    QString strCofficientF;//ϵ��(Ĥϵ��,����ϵ��)
//    QString strCofficientR;
//}HPSystemInpS;
//--�ȴ��� --��������ã�����������
typedef struct  _OUTPUTHPPhy_Inp_S_{
    QString strCommentaryName;//ע��**
    QString strStyle;//Physical Constants
    QString strAbsZero;//�������
    QString strStefanBoltzman;//˹߯��-������������
	QString strMassScalingFactor;//��������ϵ��
}HPPhyInpS;


//--Contact relationship(�Ӵ���ϵ)
typedef struct ContactPairProperties{//*Surface Interaction;
	//*Friction;*SURFACE BEHAVIOR;*Gap Condutance;*Gap Heat Generation
   QString CPPName;//CPP:ContactPairProperties
   QString SlipTolerance;//silp tolerance=0.005
   QString FrictionCoefficient;
   QString TangentialStiffness;
   QString NormalType;//LINEAR
   QString NormalStiffness;
   QString HEParam;//HEP:Heat Exchange Parameters
   QString FGenerHeat;//Friction Generates Heat
   QString M_ContactType;
   QString HeatExchangeType;
   QString strCurveName;
   QStringList strDataX;
   QStringList strDataY;
   QString MasterS,SlaveS;//���桢����

}CPPInInp;

typedef struct ContactPair{//*Contact Pair
   QString CCPName;
   QString MainSurface;//����
   QString SlaveSurface;//�ͼ�
   QString SurfaceType;//SURFACE TO SUREACE
}CPInInp;

typedef struct ContactTie{//*Tie
   QString TieName;
   QString Adjust;
   QString MainSurface;//����
   QString SlaveSurface;//�ͼ�
}CPTieInp;

//--�Ӵ���Ϣ�����
typedef struct  _OUTPUTContactInformation_Inp_S1_{
    QString MJ,Part;//���ߺͶͼ�
    QString FrictionCoefficient;//Ħ��ϵ��
    QString HeatExchangeType;//��������ʱ��仯�����¶ȱ仯����ѹ���仯
    QString HeatExchangeParameter;//�Ƚ�������Ϊ����ʱ��ֵ
    QString strCofficient;//��ֵ��������ϵ��
	bool Binding;//�Ƿ��
	QString NormalStiffness;
	QString TangentialStiffness;
	QString FrictionGeneratesHeat;
	QString m_ContactType;
	QString AmplitudeName;//��ֵ��������

}OCIInpS;

typedef struct  _OUTPUTContactInformation_Inp_S2_{
	QString NormalStiffness;
	QString TangentialStiffness;
	QString FrictionGeneratesHeat;
	QString m_ContactType;
}OCIInpS2;

//�����ƶ�
typedef struct ForgingSystem{//*Boundary,*Step;
   QList<QString> NodeSetName;
   QString SportType;//0����λ��;1�����ٶ�.
   QString Direction;
   QString MotionParameterType;//��������ʱ��仯
   QString MotionParameter;
   QString CurveName;
   QString ScalingFactor;
   QString ForgingSystemName;
   QString TBName;
   QString StepNumber;
   QString SolutionName;
}FSystem;
typedef struct Assembling_S_{
    float MRoveA[7];
    QString strLingA;
}AssemblingS_ZP;

//�ȱ߽�����
typedef struct _S_HotName_S_{
   QVector<QString> m_ThermalBoundaryName2;
}m_TBName;

//����
typedef struct _Gravity_S_{
   QString Value;//����ֵ
   QString direation;//��������
    void clear(){
        Value="";
        direation="";
    }
}Gravity_Inp;

typedef HIPSolveInpS HPSolveInpS;
//typedef struct _OUTPUTInp_S_{
//    
//
//
//};

//---------frd VIS vtkTEXT
typedef struct _TEXT_STEP_INC_TIME_S{
    QString dataTime;
    QString dataInc;
    QString dataStep;
}TextStepIncTimeS;
#endif
