#ifndef  AD_DEFINED
#define  AD_DEFINED

#include <QString>
#include <QStringList>
#include <QVector>
#include <QMenu>
using namespace std;


#define PLOTOPTNAME "/ConfigFile/CommonPlot.cfg"
#define PLOTOPTNAMESTART "PlotOption"
#define READRESULT "/ConfigFile/CommonVTKtext.cfg"
#define READRESULTSTART "ReadResult"
enum MaterialENUM{
	M_ENUM_DENSITY=0,
	M_ENUM_ELASTIC=1,
	M_ENUM_PLASTIC=2,
	M_ENUM_EXPANS=3,
	M_ENUM_CONDUCT=4,
	M_ENUM_SPECIFICE=5,
    M_ENUM_METALPOWDER=6,//粉末特性
    M_ENUM_Rate_Dependent_Plastic=7,//速率相关塑料
    M_ENUM_Damage=8,//损伤
	M_ENUM_Dynamic_Recrystallization=9
};
enum NElSufENUM{
    COM_ENUM_NSET =0,  //节点
    COM_ENUM_ELSET =1, //单元
    COM_ENUM_SURFSET=2,//表面
};

//Menu相关Step 工具条等
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
    QString m_VisName;//文件名
}ResultOutputS;
//--

//-- material chart
struct _WELCME_CHART_PROP_{
    QString Title;//图标名称(none)
    //char *Label;//标签
    QStringList strListLabel;
    QString xLabel;//x标签(none)
    QString yLabel;//y标签
    QString yLabel2;//y2标签
};

//--材料 
typedef struct _MATERIAL_STRUCT_{
	QString Name;   //名字
	QString	Styles; //类型
	QString	*Data;  //数据
    QStringList MaterList;//comList
    int MaterListIndex;//com中的第几个
    int id;
	_MATERIAL_STRUCT_(){
		//Name =    "Material";
		Name =    "";
		Styles =  "Elastic";
        id = 1;
        MaterListIndex=0;
	}	
}MaterialS,*pMaterialS;

//--截面
typedef struct _SECTION_STRUCT_{
    QString Name;   //名字
    QString	strCategory; //属性
    QString	Styles;  //类型
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

//--颜色设置(vtk)
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


//--输出inp 
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
  QString strCommentary;// 注释MATERIALS
  QString strMaterialName;//名字Material-1
  QStringList strMaterialStyleName;//类型名字Plastic、Elastic...
  QStringList strMaterialStyleParam;//expansion -->zero(可选参数);
  QStringList strMaterialStyleParam2;
  QStringList strSubMaterialName;//Young's Modulus Poisson's Ratio...
  QStringList strSubMCol;//plastic,Elastic列数
  QStringList strSubMCol2;//金属粉末特性，第二页数据列
  //QStringList strDataX;
  //QStringList strDataY1;
  QVector<QStringList*> strData[20];
  QStringList strMetalPowerData;//金属粉末特性数据
  //QVector <HideMCell> strData; //对应类型名字数据（）eg:Plastic下
  int dataColNum;//数据列个数
  int dataRowNum;//数据行个数
  int dataColNum2;//分了Tab的数据，第二页的数据列
  int dataRowNum2;
  int DateVectorNum;//几组数据
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
    QString strCommentaryName;//*section:合金粉末
    QString strSectionStyle;//solid (solid Section)
	QString strPartType;//部件类型(刚体，弹塑性、弹性)
    QString strelset;//单元集合
    QString StrMaterial;//当前Material选择
    QString PartName;
    _OUTPUTSECTION_Inp_S_(){
		PartName="";
        strCommentaryName = "合金粉末";
        strSectionStyle ="Solid Section";
        strelset ="POWER";
        StrMaterial ="METAL_POWDER";
    }
}SectionInpS;
//--Curve Inp 
typedef struct _OUTPUTCurve_Inp_S_{
    QString strCommentaryName;//*曲线定义
    QString strCurveStyle;//*Amplitude；
    QString strName;//曲线的名字
    QString strTime;//?
    QStringList strDataX;
    QStringList strDataY;

    int ThirdStrTime;//0-NO="",1-YES time=“TOTAL TIME”
    _OUTPUTCurve_Inp_S_(){
        strCurveStyle="Amplitude";
        strTime = "TOTAL TIME";
        ThirdStrTime =1;
    }
    //--重载判断相等
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
    QString strCommentaryName;//边界条件
    QString strBoundaryName;//*Boundary;
    QString strPSetName;//Dis,Axia
    QString strURstyle;//自由度（1-x,2-y,3-z,4-ux,5-uy,6-zy）
	QString SportType;//0代表位移；1代表速度;2代表无运动
	QString ParametreType;//常数、随时间变化、温度
	QString Parametre;
	QString strAmplitudeName;
	QString ScalingFactor;//缩放系数
	QString SolutionName;
	QString StepNumber;//0,1,2,3,......
   
	_OUTPUTBC_Inp_S_(){
		strCommentaryName="";
		strBoundaryName="";
		strPSetName="";
		strURstyle="";
		SportType="无";
		ParametreType="常数";
		strAmplitudeName="";
		ScalingFactor="1";
		SolutionName="";
		StepNumber="";
		Parametre="";
	}
}BCInpS;

//--HIPsystem Inp(热等静压制度)
typedef struct  _OUTPUTHIPSystem_Inp_S_{
    QString strCommentaryName;//注释**压力载荷
    QString strStyle;//Boundary/Dsload
    QString strAmplitudeName;//对应曲线的名字
    QString strPsetName;//点集合
    QString strCofficient;//缩放系数
	QString SolutionName;
	QString StepNumber;//0,1,2,3,......
}HIPSystemInpS;

//--HIPSolve Inp(热等静压求解器设置)
typedef struct  _OUTPUTHIPSolve_Inp_S_{
    QString strCommentaryName;
    QString strStepInc;//最大增益步数
    QString strNlgeom; //耦合/不耦合（YES/NO）
    QString strNlgeomSTR; //耦合/不耦合（输出英文）
	QString strProcess;//工艺流程:凝固、热等静压、锻造、热处理
    QString strDelMX;//最大温度增益50
    QString strInitStep;//初始步长
    QString strTotalTime;//模拟总时长
    QString strMinStep;//最小步长
    QString strMaxStep;//最大步长
	QString TimeFrequencyInc;

    _OUTPUTHIPSolve_Inp_S_(){
        strCommentaryName = "求解器设置";
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
    QString strPSetName;//"集合的名字"
    QString strVal;//数值.
	QString IncludeName;//包含的外部加载文件 luo
    _OUTPUTINIT_Inp_S_(){
        strCommentaryName ="初始条件";
        strInitCondition="Initial Conditions";//"Initial Conditions"
        strInitTypeName="TEMPERATURE";//"TEMPERATURE"
        strPSetName="Whole";//"集合的名字"
        strVal="20";//数值.
    }
}InitInpS;
//--结果变量输出 Inp
typedef struct  _OUTPUTVariable_Inp_S_{
    QString strCommentaryName;//Frd结果输出
    QString strNodeFile;//节点变量NODE FILE(dat:NODE PRINT)
    bool bELFileShow;//是否输出EL FILE关键字及内容（0=不输出、1=输出）
    QString strELFile;//单元变量EL FILE(dat:EL PRINT)
    QString strTimeOrFreqName;//TIME POINTS/FREQUENCY
    QString strELInclude;
    QString strNInclude;
    QString strTname;//T1,T10
	QString NodeSet;//点集合名称
	QStringList NodeSet1;//点集合名称
    QString Igforce;//是否输出压力曲线，0代表不输出，1代表输出。
	_OUTPUTVariable_Inp_S_(){
		Igforce="0";
	}
}VariableInpS;

//--Inp 节点定义
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
//--Inp 单元节点定义
typedef struct _INPUT_EL_Inp_S_{
    QString strELTitle; //*Element
    QString strELType;  //CAX4
    QString strELSET;   //可选参数
    QList<QString> strData;    //数据区域
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
//--Inp 节点集合
typedef struct _INPUT_NSet_Inp_S_{
    QString strNSetTitle; //*Nset
    QStringList strNSetName;  //nset=Axia,Dis,whole,temp,n1
    QStringList strNSetStyle;  //Generate
    //QList<QString> strData;//数据区域
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
//--Inp 单元集合
typedef struct _INPUT_ELSet_Inp_S_{
    QString strELSetTitle; //*Elset
    QStringList strElSetName;  //elset=POWER,CANISTER,e1...
    QStringList strElSetStyle;  //Generate
    QStringList strData;//数据区域(只有一行)//2020-6-18不只有一行，多行
	QString PartName;
    void clear(){
		PartName="";
        strELSetTitle="";
        strElSetName.clear();
        strElSetStyle.clear();
        strData.clear();
    }
}ElSetInpS;
//--Inp 表面集合
/*Surface, type=ELEMENT, name=press
press_S4, S4
press_S2, S2
*/
typedef struct _INPUT_SURFACE_Inp_S_{
    QString strSurfaceTitle; //*Surface
    QString strSurfaceType;  //type=ELEMENT,NODE
    QStringList strSurfaceName;  //name = press//(Surf-1,Surf-222)
    QStringList strSurfaceDataNum;//每个press下包含有几行
    QList<QString> strData;  //数据区域press_S4, S4 press_S2, S2
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

//--包含整个节点、单元、表面集合等信息
typedef struct _INPUT_TotalELPSET_Inp_S_{
    NodeInpS     NodeInpData;
    ELInpS       ELInpData;
    NSetInpS     NSetInpData;
    ElSetInpS    ElSetInpData;
    SurfaceInpS  SurfaceInpData;
}NodeELSetInps;

//--gmsh中单元、节点结合、单元集合、表面集合；
typedef struct _GMSH_INP_S_{
    NodeInpS NodeInpData;
    NSetInpS gmshNSetData;
    QList<ELInpS> gmshELData;
    
    ElSetInpS gmshELSetData;
}GmshInpS;

//--集合中的信息type+str
typedef struct _S_NELSurfset_S_{
    QString strName;//点集合、单元集合、表面集合
    int iType;//类型选择  <0:点集合;1:单元集合;2:表面集合>
}NElSurfChsS;

//HP INP（热处理）-----
typedef struct _S_HP_TTT_S_{//--TTT/CCT
    QString strName;//*PHASECURVE,
    QString strPhaseCurveType;//type = TTT/CCT
    QStringList strHeadParam;//1,1,0,1.,1.0(相序，转变分数1，数据点数，转变分数2，初值)
    QStringList strData;//数据点数为0，则没有数据；不为0，则为包含的数据;
}HPphaseCurveInps;

typedef struct _S_HP_PHASEQUILIBRIUM_S_{//相平衡
    QString strName;//*PHASEEQUILIBRIUM
    QString strType;//no
    QStringList strHeadParam;//2,9(相序，总点数)
    QStringList strData;//数据项
}HPphaseBalanceInps,HPincubatePeriodInps,HPPhasePropSpecInps,HPPhasePropCondInps,HPPhaseZBFInps;

typedef struct _S_HP_PHASEELATENTHEAT_S_{//潜热//硬度
    QString strName;//*PHASELATENTHEAT
    QStringList strHeadParam;//2,102.0(相序，数值)
}HPphaseElateHeatInps,HPphaseHardInps;

typedef struct _S_HP_PHASECTROL_S_{//热处理方式
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
//--HPsystem Inp(热处理制度)
//Film/radiate(从文件中读取到界面中)
typedef struct  _OUTPUTHPSystemFR_Inp_S_{
    QString strStep;//step第几步；
    QString strCommentaryName;//注释**
	QString strProcess;
    QString strStyle;//film/radiate
    QString strAmplitudeName;//对应曲线的名字
    QString strFilmRadiateAmpName;//FilmAmp/raditeAmpName
    QString strOP;//op = new
    QString strInclude;//eg.该 radiate包含有3个
                        //*radiate, op=NEW
                        //_Surf-3_S1, R1, 900., 0.4
                        //_Surf-3_S2, R2, 900., 0.4 
                        //_Surf-3_S3, R3, 900., 0.4
    QString strElsetName;//单元集合
    QString strSurfaceNum;//编号
    QString strEnvirTemprat;//环境温度
    QString strCofficient;//系数(膜系数,辐射系数)

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
////--热处理 --从界面中写入到文件中；
//typedef struct  _OUTPUTHPSystem_Inp_S_{
//    QString strStep;//step
//    QString strCommentaryName;//注释**
//    QString strStyle;//film/radiate
//    QString strAmplitudeName;//对应曲线的名字
//    QString strFilmRadiateAmpName;//FilmAmp/raditeAmpName
//    QString strOP;//op = new
//    QString strInclude;//eg.该 radiate包含有3个
//    QString strElsetName;//单元集合
//    QString strRElsetName;//R集合
//    QString strSurfaceNum;//编号
//
//    QString strEnvirTemprat;//环境温度
//    QString strCofficientF;//系数(膜系数,辐射系数)
//    QString strCofficientR;
//}HPSystemInpS;
//--热处理 --求解器设置（物理常量）
typedef struct  _OUTPUTHPPhy_Inp_S_{
    QString strCommentaryName;//注释**
    QString strStyle;//Physical Constants
    QString strAbsZero;//绝对零度
    QString strStefanBoltzman;//斯忒藩-玻尔兹曼常数
	QString strMassScalingFactor;//质量缩放系数
}HPPhyInpS;


//--Contact relationship(接触关系)
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
   QString MasterS,SlaveS;//主面、从面

}CPPInInp;

typedef struct ContactPair{//*Contact Pair
   QString CCPName;
   QString MainSurface;//摸具
   QString SlaveSurface;//锻件
   QString SurfaceType;//SURFACE TO SUREACE
}CPInInp;

typedef struct ContactTie{//*Tie
   QString TieName;
   QString Adjust;
   QString MainSurface;//摸具
   QString SlaveSurface;//锻件
}CPTieInp;

//--接触信息的输出
typedef struct  _OUTPUTContactInformation_Inp_S1_{
    QString MJ,Part;//摸具和锻件
    QString FrictionCoefficient;//摩擦系数
    QString HeatExchangeType;//常数、随时间变化、随温度变化、随压力变化
    QString HeatExchangeParameter;//热交换参数为常数时的值
    QString strCofficient;//幅值曲线缩放系数
	bool Binding;//是否绑定
	QString NormalStiffness;
	QString TangentialStiffness;
	QString FrictionGeneratesHeat;
	QString m_ContactType;
	QString AmplitudeName;//幅值曲线名称

}OCIInpS;

typedef struct  _OUTPUTContactInformation_Inp_S2_{
	QString NormalStiffness;
	QString TangentialStiffness;
	QString FrictionGeneratesHeat;
	QString m_ContactType;
}OCIInpS2;

//锻造制度
typedef struct ForgingSystem{//*Boundary,*Step;
   QList<QString> NodeSetName;
   QString SportType;//0代表位移;1代表速度.
   QString Direction;
   QString MotionParameterType;//常数、随时间变化
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

//热边界名称
typedef struct _S_HotName_S_{
   QVector<QString> m_ThermalBoundaryName2;
}m_TBName;

//重力
typedef struct _Gravity_S_{
   QString Value;//重力值
   QString direation;//重力方向
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
