#include "mainwindow.h"
#include <QTextCodec>
#include <QFileInfo>
#include "QMyVTK.h"
#include "SARibbonApplicationButton.h"
#include "FileValidation.h"
//MainWindow::MainWindow(QWidget *parent,Qt::WFlags flags)
//   : QMainWindow(parent,flags)
MainWindow::MainWindow(QWidget *par):SARibbonMainWindow(par)
{
	/*QFileDialog::Option options;
	options=QFileDialog::DontUseNativeDialog;
	QString workPath=options;
	QDir::setCurrent(workPath);*/

	InitDlg();
    createActions();
    creatRibbon();
    createDockWindows();
    createStatusBar();
   //createContextMenu();
    createToolBars();
	ToolApp();
    InitProcess();
    InitTree();
    //information_return();
	AppKey::Instance()->start();
   
    m_Lin->m_CurrentLanguage=en_us;
    LanguageZHSlot();

}

MainWindow::~MainWindow()
{
	if (GmshProcess!=NULL)
	{
		GmshProcess->close();
		delete GmshProcess;
		GmshProcess=NULL;
	}
    if (m_SloverProcess!=NULL)
    {
        m_SloverProcess->close();
        delete m_SloverProcess;
        m_SloverProcess = NULL;
    }

    if (m_HIPSystemDlg!=NULL)
    {
        delete m_HIPSystemDlg;
        m_HIPSystemDlg = NULL;
    }
    
    if (m_VariableOutputDlg!=NULL)
    {
        delete m_VariableOutputDlg;
        m_VariableOutputDlg = NULL;
    } 
	if (m_HipSubmissionDlg!=NULL)
	{
		delete m_HipSubmissionDlg;
		m_HipSubmissionDlg = NULL;
	}

    if (m_HpPartDlg!=NULL)
    {
        delete m_HpPartDlg;
        m_HpPartDlg = NULL;
    }
    if (m_HpBCDlg!=NULL)
    {
        delete m_HpBCDlg;
        m_HpBCDlg = NULL;
    }
    if (m_HpInitDlg!=NULL)
    {
        delete m_HpInitDlg;
        m_HpInitDlg = NULL;
    }
    if (m_HpSystemDlg!=NULL)
    {
        delete m_HpSystemDlg;
        m_HpSystemDlg = NULL;
    }
    if (m_HpSolveSetDlg!=NULL)
    {
        delete m_HpSolveSetDlg;
        m_HpSolveSetDlg = NULL;
    }
    if (m_HpSubmissionDlg!=NULL)
    {
        delete m_HpSubmissionDlg;
        m_HpSubmissionDlg = NULL;
    }
	
    //--dlg
    if (m_PElSfSetDlg != NULL)
    {
        delete m_PElSfSetDlg;
        m_PElSfSetDlg=NULL;
    }
    	if (m_ForgingContactDlg!=NULL)
    {
        delete m_ForgingContactDlg;
        m_ForgingContactDlg = NULL;
    }
	if (m_ForgingSystemDlg!=NULL)
    {
        delete m_ForgingSystemDlg;
        m_ForgingSystemDlg = NULL;
    }
	if (m_ForgingSubmissionDlg!=NULL)
    {
        delete m_ForgingSubmissionDlg;
        m_ForgingSubmissionDlg = NULL;
    }
	if (m_WidgetInpElsetDlg!=NULL)
	{
		delete m_WidgetInpElsetDlg;
		m_WidgetInpElsetDlg = NULL;
	}
	//luo
	if (m_AssemblingAct_!=NULL)
    {
        delete m_AssemblingAct_;
        m_AssemblingAct_ = NULL;
    }
	if (m_DistMeasurementAct_!=NULL)
    {
        delete m_DistMeasurementAct_;
        m_DistMeasurementAct_ = NULL;
    }
	if (m_ThermalBoundaryDlg!=NULL)
	{
	delete m_ThermalBoundaryDlg;
	m_ThermalBoundaryDlg = NULL;
	}
	if (m_GravityAct_!=NULL)
	{
	delete m_GravityAct_;
	m_GravityAct_ = NULL;
	}

	if (m_CastingGravityDlg!=NULL)
	{
		delete m_CastingGravityDlg;
		m_CastingGravityDlg = NULL;
	}
	if (m_QStatisticsDlg!=NULL)
	{
		delete m_QStatisticsDlg;
		m_QStatisticsDlg = NULL;
	}
	
}
void MainWindow::InitDlg()
{
    m_HpPartDlg=NULL;
    m_HpSubmissionDlg = NULL;
    m_HpBCDlg=NULL;
    m_HpInitDlg = NULL;
    m_HpSystemDlg =NULL;
    m_HpSolveSetDlg=NULL;
    m_HIPSystemDlg=NULL;
    m_VariableOutputDlg= NULL;
	m_HipSubmissionDlg=NULL;
	GmshProcess = NULL;
    m_SloverProcess = NULL;

	m_WidgetInpElsetDlg = NULL;
    m_PElSfSetDlg=NULL;
	m_ForgingContactDlg=NULL;
	m_ForgingSystemDlg=NULL;
	m_ForgingSubmissionDlg = NULL;
	m_AssemblingAct_=NULL;
	m_DistMeasurementAct_=NULL;
	m_ThermalBoundaryDlg=NULL;
	m_GravityAct_=NULL;
	m_CastingGravityDlg=NULL;
	m_QStatisticsDlg=NULL;
}

void MainWindow::InitProcess()
{
    if (!m_PElSfSetDlg){m_PElSfSetDlg = new PElSfSetDlg(this);}
	if (!m_AssemblingAct_){m_AssemblingAct_=new Assembling(this);}
	if (!m_DistMeasurementAct_){m_DistMeasurementAct_=new DistMeasurement(this);}
	if (!m_WidgetInpElsetDlg){m_WidgetInpElsetDlg = new QInpActorWidge(this);}//����-��������/��ʾ
	if (!m_GravityAct_){m_GravityAct_=new Gravity(this);}
	if (!m_QStatisticsDlg){m_QStatisticsDlg=new QStatistics(this);}
	
    connect(viewWindow_,SIGNAL(emitInpDataOk(InpDataVIS*)),m_PElSfSetDlg,SLOT(InitInpDataSlot(InpDataVIS*)));
    connect(m_PElSfSetDlg,SIGNAL(emitPsetHighLight(NElSurfChsS)),viewWindow_,SLOT(HightLightPSet(NElSurfChsS)));
    connect(viewWindow_,SIGNAL(emitInpDataOk(InpDataVIS*)),m_AssemblingAct_,SLOT(InitInpDataSlot1(InpDataVIS*)));
	connect(viewWindow_,SIGNAL(emitInpDataOk(InpDataVIS*)),m_DistMeasurementAct_,SLOT(InitInpDataSlot2(InpDataVIS*)));
	connect(m_AssemblingAct_,SIGNAL(emitPsetHighLight1(NElSurfChsS)),viewWindow_,SLOT(HightLightPSet(NElSurfChsS)));
	connect(m_WidgetInpElsetDlg,SIGNAL(emitCheckActor(QStringList)),viewWindow_,SLOT(ActorElSetCheckShow(QStringList)));
	//connect(m_WidgetInpElsetDlg,SIGNAL(emitCheckActor(QStringList)),m_PElSfSetDlg,SLOT(UpDataInpElSetDataSlot(QStringList)));//ElementSet Part Show/Hide
    //--view - post/pre window
    //--�ȵȾ�ѹ��1��
    if (!m_HIPSystemDlg){m_HIPSystemDlg = new QHIPsystemDlg(this);}
    if (!m_VariableOutputDlg){m_VariableOutputDlg = new QVariableOutputDlg(this);}
	if (!m_HipSubmissionDlg){m_HipSubmissionDlg=new QHIPSubmissionDlg(this);}
    //--�ȵȾ�ѹ�и�����ʾ��1.1��
    connect(m_HIPSystemDlg,SIGNAL(emitPsetHighLight(NElSurfChsS)),viewWindow_,SLOT(HightLightPSet(NElSurfChsS)));
	connect(m_HipSubmissionDlg,SIGNAL(emitHipInpSave()),this,SLOT(MainSaveHIPInpSlot()));
	connect(m_HipSubmissionDlg,SIGNAL(emitHipInpRun()),this,SLOT(HIPSolveActOpenSlot()));
	
	
    //showMaximized();
    //--�ȴ�����2��
    if (!m_HpPartDlg){m_HpPartDlg=new QHPPartDlg(this);}
    if (!m_HpBCDlg){m_HpBCDlg=new QHPBCDlg(this);}
    if (!m_HpInitDlg){m_HpInitDlg=new QHPInitDlg(this);}
    if (!m_HpSystemDlg){m_HpSystemDlg=new QHPSystemDlg(this);}
    if (!m_HpSolveSetDlg){m_HpSolveSetDlg=new QHPSolveSetDlg(this);}
    if (!m_HpSubmissionDlg){m_HpSubmissionDlg=new QHPSubmissionDlg(this);}
    //--�ȴ����и�����ʾ��2.1��
    connect(m_HpPartDlg,SIGNAL(emitElsetHighLight(NElSurfChsS)),viewWindow_,SLOT(HightLightPSet(NElSurfChsS)));
    connect(m_HpBCDlg,SIGNAL(emitPsetHighLight(NElSurfChsS)),viewWindow_,SLOT(HightLightPSet(NElSurfChsS)));
    connect(m_HpSystemDlg,SIGNAL(emitPsetHighLight(NElSurfChsS)),viewWindow_,SLOT(HightLightPSet(NElSurfChsS)));
	connect(m_HpInitDlg,SIGNAL(emitPsetHighLight(NElSurfChsS)),viewWindow_,SLOT(HightLightPSet(NElSurfChsS)));
    connect(m_HpSubmissionDlg,SIGNAL(emitHpInpSave()),this,SLOT(MainSaveHPInpSlot()));
    connect(m_HpSubmissionDlg,SIGNAL(emitHpInpRun()),this,SLOT(HIPSolveActOpenSlot()));
	connect(m_HpSubmissionDlg,SIGNAL(emitCheck()),this,SLOT(InpCheckSlot()));//inp�ļ���鹦��
	connect(m_HpSubmissionDlg,SIGNAL(emitHipInpKill()),this,SLOT(HIPSolveActKillSlot()));
	//����
	if (!m_ForgingContactDlg){m_ForgingContactDlg=new QForgingContactDlg(this);}
	if (!m_ForgingSystemDlg){m_ForgingSystemDlg=new QForgingSystemDlg(this);}
	if (!m_ForgingSubmissionDlg){m_ForgingSubmissionDlg=new QForgingSubmissionDlg(this);}
	if (!m_ThermalBoundaryDlg){m_ThermalBoundaryDlg=new ThermalBoundary(this);}
    connect(m_ForgingSubmissionDlg,SIGNAL(emitForgingInpSave()),this,SLOT(MainSaveForgingInpSlot()));
    connect(m_ForgingSubmissionDlg,SIGNAL(emitForgingInpRun()),this,SLOT(HIPSolveActOpenSlot()));
	connect(m_ForgingContactDlg,SIGNAL(emitPsetHighLight(NElSurfChsS)),viewWindow_,SLOT(HightLightPSet(NElSurfChsS)));//�Ӵ������漯�ϸ�����ʾ
	connect(m_ForgingSystemDlg,SIGNAL(emitPsetHighLight(NElSurfChsS)),viewWindow_,SLOT(HightLightPSet(NElSurfChsS)));
	connect(m_ThermalBoundaryDlg,SIGNAL(emitPsetHBHighLight(NElSurfChsS)),viewWindow_,SLOT(HightLightPSet(NElSurfChsS)));
	connect(m_ThermalBoundaryDlg,SIGNAL(emitHotName(m_TBName)),this,SLOT(ForgingSystemActSlot2(m_TBName)));
	connect(m_ThermalBoundaryDlg,SIGNAL(emitHotName2(QMap<int,QString>)),this,SLOT(ForgingSystemActSlot3(QMap<int,QString>)));
	//����
	if (!m_CastingGravityDlg){m_CastingGravityDlg=new Gravity(this);}
	 connect(m_CastingGravityDlg,SIGNAL(clicked()),viewWindow_,SLOT(CastingGravitySlot()));
   
    //--ȫ��cuvre����
    connect(m_HIPSystemDlg,SIGNAL(emitHIPCurveData(QStringList,InpCurveInpS)),m_HpSystemDlg,SLOT(updateHPCurveDataSlot(QStringList,InpCurveInpS)));
    connect(m_HpSystemDlg,SIGNAL(emitHPCurve(QStringList,InpCurveInpS)),m_HIPSystemDlg,SLOT(updateHIPCurveDataSlot(QStringList,InpCurveInpS)));
    //��һ��/��һ��
    //1.�������
    connect(m_HpSolveSetDlg->m_PreBtn,SIGNAL(clicked()),this,SLOT(HP_SolveSetPreChsSlot()));
    connect(m_HpSolveSetDlg->m_NextBtn,SIGNAL(clicked()),this,SLOT(HPSolveNextActSlot()));
    //2.1�ȴ����ƶ�
    connect(m_HpSystemDlg,SIGNAL(emitHPsystemNext(int)),this,SLOT(HPSystemNextBtnSlot(int)));
    connect(m_HpSystemDlg->m_PreBtn,SIGNAL(clicked()),this,SLOT(HPInitActSlot()));
    //2.2�ȵȾ�ѹ�ƶ�
    connect(m_HIPSystemDlg->m_PreHipBtn,SIGNAL(clicked()),this,SLOT(HPInitActSlot()));
    connect(m_HIPSystemDlg->m_NextHipSysBtn,SIGNAL(clicked()),this,SLOT(HIPResolveDlgActSlot()));//HPSloveSetActSlot()));
	//2.3�����ƶ�
	connect(m_ForgingSystemDlg,SIGNAL(emitForgingsystemNext(int)),this,SLOT(ForgingSystemNextBtnSlot0(int)));
    connect(m_ForgingSystemDlg->m_PreSystemBtn,SIGNAL(clicked()),this,SLOT(ForgingHBSlot()));
	connect(m_ForgingSystemDlg->m_NextSystemBtn,SIGNAL(clicked(int)),this,SLOT(ForgingSystemNextBtnSlot(int)));
	//connect(m_ForgingSystemDlg->m_PreSystemBtn,SIGNAL(clicked()),this,SLOT(ForgingHBSlot()));
    //connect(m_ForgingSystemDlg->m_NextSystemBtn,SIGNAL(clicked()),this,SLOT(ForgingSloveSetActSlot()));
	//connect(m_ForgingSystemDlg->m_NextSystemBtn,SIGNAL(clicked(int)),this,SLOT(ForgingSystemNextBtnSlot(int)));

	//����
    connect(m_CastingGravityDlg->NextBtn,SIGNAL(clicked()),this,SLOT(HP_HIPPartActSlot()));
    //3.����
    connect(m_HpPartDlg->m_NextSectionBtn,SIGNAL(clicked()),this,SLOT(HPBCActSlot()));
	connect(m_HpPartDlg,SIGNAL(emitMerge()),this,SLOT(MergeSlot()));
    //4.�߽�
    connect(m_HpBCDlg->m_PreBCBtn,SIGNAL(clicked()),this,SLOT(HP_HIPPartActSlot()));
    connect(m_HpBCDlg->m_NextBCBtn,SIGNAL(clicked()),this,SLOT(HPInitActSlot()));
 
	//5.�Ӵ�
    connect(m_ForgingContactDlg->m_PreContactBtn,SIGNAL(clicked()),this,SLOT(HPBCActSlot()));
    connect(m_ForgingContactDlg->m_NextContactBtn,SIGNAL(clicked()),this,SLOT(HPInitActSlot0()));

	//connect(m_CastingContactBtn->m_PreContactBtn,SIGNAL(clicked()),this,SLOT(HPBCActSlot()));
    //connect(m_CastingContactBtn->m_NextContactBtn,SIGNAL(clicked()),this,SLOT(HPInitActSlot02()));

    //6.��ʼ��
    connect(m_HpInitDlg->m_PreInitBtn,SIGNAL(clicked()),this,SLOT(HPBCActSlot02()));
    connect(m_HpInitDlg->m_NextInitBtn,SIGNAL(clicked()),this,SLOT(HP_HIPSystemActSlot()));
		//7.�ȱ߽�
	connect(m_ThermalBoundaryDlg->m_HBPreBtn,SIGNAL(clicked()),this,SLOT(HPInitActSlot02()));
    connect(m_ThermalBoundaryDlg->m_HBNextBtn,SIGNAL(clicked()),this,SLOT(ForgingSystemActSlot0()));

    //-�㼯�ϡ���Ԫ���ϡ����漯��
	connect(m_PElSfSetDlg,SIGNAL(emitCurrentPSet(QString)),m_HpBCDlg,SLOT(AppendPSetComboxSlot(QString)));
	connect(m_PElSfSetDlg,SIGNAL(emitCurrentPSet(QString)),m_HpInitDlg,SLOT(AppendPSetComboxIniSlot(QString)));
	connect(m_PElSfSetDlg,SIGNAL(emitCurrentPSet(QString)),m_HpSolveSetDlg,SLOT(AppendPSetComboxSolvesetSlot(QString)));

    connect(m_PElSfSetDlg,SIGNAL(emitCurrentElSet(QString)),m_HpPartDlg,SLOT(AppendElSetComboxSlot(QString)));
    connect(m_PElSfSetDlg,SIGNAL(emitCurrentSetsData(QString,int,DecodeGenerateS)),m_HpPartDlg,SLOT(AppendPElData(QString,int,DecodeGenerateS)));
    connect(m_PElSfSetDlg,SIGNAL(emitCurrentSurfSet(QString)),m_HIPSystemDlg,SLOT(AppendSurfSetComboxSlot(QString)));

	connect(m_PElSfSetDlg,SIGNAL(emitCurrentSurfSet(QString)),m_ForgingContactDlg,SLOT(AppendSurfSetComboxContactSlot(QString)));
	connect(m_PElSfSetDlg,SIGNAL(emitCurrentSurfSet(QString)),m_ThermalBoundaryDlg,SLOT(AppendSurfSetComboxTBSlot(QString)));
    //--������ʼ����
    iSetProStyle=ENUM_PRO_HIP;
    
}
void MainWindow::ToolApp()
{
	if (!m_AssemblingAct_){m_AssemblingAct_=new Assembling(this);}
	connect(m_AssemblingAct_,SIGNAL(emitPsetHighLight(NElSurfChsS)),viewWindow_,SLOT(HightLightPSet(NElSurfChsS)));
	connect(m_AssemblingAct_,SIGNAL(emitAssemblingS(AssemblingS_ZP)),this,SLOT(AssembleDlgViewSlot(AssemblingS_ZP)));
}
void MainWindow::AssembleDlgViewSlot(AssemblingS_ZP data)
{
	float sin1,sin2,sin3,cos1,cos2,cos3;
	float RMX,RMY,RMZ;
	sin1=qSin(data.MRoveA[3]/180.0*3.141592654);
	sin2=qSin(data.MRoveA[4]/180.0*3.141592654);
    sin3=qSin(data.MRoveA[5]/180.0*3.141592654);
	cos1=qCos(data.MRoveA[3]/180.0*3.141592654);
	cos2=qCos(data.MRoveA[4]/180.0*3.141592654);
	cos3=qCos(data.MRoveA[5]/180.0*3.141592654);
	RMX=data.MRoveA[0];
	RMY=data.MRoveA[1];
	RMZ=data.MRoveA[2];

	ReadInpResultS m_Data = CRWObject.m_ReadInpResult;
	int index,StartNode,EndNode,IncNode;
	int Nuc=m_Data.TmpNsetInps.strNSetName.size();
	for(int i=0;i<Nuc;i++){
       QString STR1=m_Data.TmpNsetInps.strNSetName.at(i);
	   if(STR1==data.strLingA){
		 index=i;
		 break;
	   }
	}
	QString STR2=m_Data.TmpNsetInps.strNSetStyle.at(index);
	if(STR2=="generate"){
        QStringList StartNode1=m_Data.TmpNsetInps.strData.at(index);
		QString StartNode22=StartNode1.first();
		StartNode1=StartNode22.split(",");
		StartNode=StartNode1.at(0).toInt();
		EndNode=StartNode1.at(1).toInt();
		IncNode=StartNode1.at(2).toInt();
		int NumC=m_Data.TmpNodeInpS.strData.size();
		int IJK=0;
		for(int IJ=StartNode-1;IJ<EndNode;IJ++){
            QString Node1=m_Data.TmpNodeInpS.strData.at(IJ);
			QString Node3="";
			QStringList Node2=Node1.split(",");
			int Nodemunber=Node2.at(0).toInt();
            Node3+=QString::number(Nodemunber,10);
			if(Nodemunber==StartNode+IJK*IncNode){
				int NM2=Node2.size();
				float ZB1=Node2.at(1).toFloat();//����ƽ����
				float ZB2=Node2.at(2).toFloat();
				float ZB3=0.0;
				float ZB11=0.0,ZB22=0.0,ZB33=0.0;
				float ZB111=0.0,ZB222=0.0,ZB333=0.0;
				if(NM2>3){
					ZB3=Node2.at(3).toFloat();
				}
				if(data.MRoveA[6]!=0.0){
					ZB11=data.MRoveA[6]*ZB1;
					ZB22=data.MRoveA[6]*ZB2;
					QString STR201=QString("%1").arg(ZB11);//������ת��Ϊ�ַ���
					QString STR202=QString("%1").arg(ZB22);//������ת��Ϊ�ַ���
					Node3=Node3+","+STR201+","+STR202;
					if(NM2>3){
						ZB33=data.MRoveA[6]*ZB3;
						QString STR203=QString("%1").arg(ZB33);//������ת��Ϊ�ַ���
						Node3=Node3+","+STR203;
					}
					m_Data.TmpNodeInpS.strData.replace(IJ,Node3);
					IJK++;
					continue;
				}
				if(data.MRoveA[3]!=0.0&&NM2>3){
					ZB222=ZB2*cos1-ZB3*sin1;
					ZB333=ZB2*sin1+ZB3*cos1;
					ZB2=ZB222;
					ZB3=ZB333;
				}else if(NM2<=3&&data.MRoveA[3]!=0){
					ZB111=cos1*ZB1-sin1*ZB2;
					ZB222=sin1*ZB1+cos1*ZB2;
					ZB1=ZB111;
					ZB2=ZB222;
				}
				if(data.MRoveA[4]!=0.0&&NM2>3){
					ZB111=ZB3*sin2+ZB1*cos2;
					ZB333=ZB3*cos2-ZB1*sin2;
					ZB1=ZB111;
					ZB3=ZB333;
				}else if(NM2<=3&&data.MRoveA[4]!=0){
					ZB111=cos2*ZB1-sin2*ZB2;
					ZB222=sin2*ZB1+cos2*ZB2;
					ZB1=ZB111;
					ZB2=ZB222;
				}
				if(data.MRoveA[5]!=0.0&&NM2>3){
					ZB111=ZB1*cos3-ZB2*sin3;
					ZB222=ZB1*sin3+ZB2*cos3;
					ZB1=ZB111;
					ZB2=ZB222;
				}
				ZB11=ZB1+RMX;
				ZB22=ZB2+RMY;
				if(NM2>3)ZB33=RMZ+ZB3;
				QString STR201=QString("%1").arg(ZB11);//������ת��Ϊ�ַ���
				QString STR202=QString("%1").arg(ZB22);//������ת��Ϊ�ַ���
				if(NM2<=3){
					Node3=Node3+","+STR201+","+STR202;
				}else{
					QString STR203=QString("%1").arg(ZB33);//������ת��Ϊ�ַ���
					Node3=Node3+","+STR201+","+STR202+","+STR203;
				}
				m_Data.TmpNodeInpS.strData.replace(IJ,Node3);
			}
			IJK++; 	 
		}
	}else if(STR2==""){
		QStringList StartNode1=m_Data.TmpNsetInps.strData.at(index);
		int NumC=m_Data.TmpNodeInpS.strData.size();
		for(int i=0;i<StartNode1.size();i++){
			QString StartNode2=StartNode1.at(i);
			QStringList StartNode3=StartNode2.split(",");
			int NUM=StartNode3.size();
			for(int j=0;j<NUM;j++){
				if(StartNode3.at(j)!=""){
					//for(int IJ=0;IJ<NumC;IJ++){
                        int IJ=StartNode3.at(j).toInt()-1;
						QString Node1=m_Data.TmpNodeInpS.strData.at(IJ);
						QString Node3="";
						QStringList Node2=Node1.split(",");
						int Nodemunber=Node2.at(0).toInt();
						Node3+=QString::number(Nodemunber,10);
						if(Nodemunber==StartNode3.at(j).toInt()){//*021
							int NM2=Node2.size();
							float ZB1=Node2.at(1).toFloat();//����ƽ����
							float ZB2=Node2.at(2).toFloat();
							float ZB3=0.0;
							float ZB11=0.0,ZB22=0.0,ZB33=0.0;
							float ZB111=0.0,ZB222=0.0,ZB333=0.0;
							if(NM2>3){
								ZB3=Node2.at(3).toFloat();
							}
							if(data.MRoveA[6]!=0.0){
								ZB11=data.MRoveA[6]*ZB1;
								ZB22=data.MRoveA[6]*ZB2;
								QString STR201=QString("%1").arg(ZB11);//������ת��Ϊ�ַ���
								QString STR202=QString("%1").arg(ZB22);//������ת��Ϊ�ַ���
								Node3=Node3+","+STR201+","+STR202;
								if(NM2>3){
									ZB33=data.MRoveA[6]*ZB3;
									QString STR203=QString("%1").arg(ZB33);//������ת��Ϊ�ַ���
									Node3=Node3+","+STR203;
								}
								m_Data.TmpNodeInpS.strData.replace(IJ,Node3);
								continue;
							}
							if(data.MRoveA[3]!=0.0&&NM2>3){
								ZB222=ZB2*cos1-ZB3*sin1;
								ZB333=ZB2*sin1+ZB3*cos1;
								ZB2=ZB222;
								ZB3=ZB333;
							}else if(NM2<=3&&data.MRoveA[3]!=0){
								ZB111=cos1*ZB1-sin1*ZB2;
								ZB222=sin1*ZB1+cos1*ZB2;
								ZB1=ZB111;
								ZB2=ZB222;
							}
							if(data.MRoveA[4]!=0.0&&NM2>3){
								ZB111=ZB3*sin2+ZB1*cos2;
								ZB333=ZB3*cos2-ZB1*sin2;
								ZB1=ZB111;
								ZB3=ZB333;
							}else if(NM2<=3&&data.MRoveA[4]!=0){
								ZB111=cos2*ZB1-sin2*ZB2;
								ZB222=sin2*ZB1+cos2*ZB2;
								ZB1=ZB111;
								ZB2=ZB222;
							}
							if(data.MRoveA[5]!=0.0&&NM2>3){
								ZB111=ZB1*cos3-ZB2*sin3;
								ZB222=ZB1*sin3+ZB2*cos3;
								ZB1=ZB111;
								ZB2=ZB222;
						 }
							ZB11=ZB1+RMX;
							ZB22=ZB2+RMY;
							if(NM2>3)ZB33=RMZ+ZB3;
							QString STR201=QString("%1").arg(ZB11);//������ת��Ϊ�ַ���
							QString STR202=QString("%1").arg(ZB22);//������ת��Ϊ�ַ���
							if(NM2<=3){
								Node3=Node3+","+STR201+","+STR202;
							}else{
								QString STR203=QString("%1").arg(ZB33);//������ת��Ϊ�ַ���
								Node3=Node3+","+STR201+","+STR202+","+STR203;
							}
							m_Data.TmpNodeInpS.strData.replace(IJ,Node3);
						}
					}
				}
			}
		}
	//float time=t120.elapsed()/1000.0;

	PreHIPPro_->m_TreeModel->setInpData(m_Data);
	if (viewWindow_){
		viewWindow_->TabView(1);//setCurrentIndex(1);
		QMdiSubWindow *CurWindow=m_MdiArea->currentSubWindow();
		for (int i=0;i<m_MdiArea->subWindowList().size();i++){
			QMdiSubWindow *TmpWindow=m_MdiArea->subWindowList().at(i);
			if (TmpWindow == TmpWindow){
				viewWindow_->ShowCurPreData(m_Data); 
				break;
			}
		}
	}
	CRWObject.m_ReadInpResult=m_Data;
	m_HpPartDlg->writeUnitPSetToInp(m_Data);
}
void MainWindow::InitTree()
{
    connect(PreHIPPro_->m_TreeView,SIGNAL(EmitNewCurve(int,int)),m_HIPSystemDlg,SLOT(NewCurve(int,int)));
    connect(PreHIPPro_->m_TreeView,SIGNAL(EmitCurveEdit(QString)),m_HIPSystemDlg,SLOT(updataFromTreeSlot(QString)));
    connect(PreHIPPro_->m_TreeView,SIGNAL(EmitCurveDel(QString)),m_HIPSystemDlg,SLOT(DelCurve(QString)));
    connect(m_HIPSystemDlg,SIGNAL(emitUpDataCurveTreeName(QString,QStringList)),PreHIPPro_->m_TreeView,SLOT(upDataTreeNameSlot(QString, QStringList)));
}
/*
����������Ribbon��ʽ
*/
void MainWindow::creatRibbon()
{ 
    //setWindowTitle(tr("WelCME"));
    // WSLg: avoid override-redirect COPY MODE window.
    // this->setWindowFlags(Qt::FramelessWindowHint);
	m_ribbon= ribbonBar();
	m_FilBtn=m_ribbon->applitionButton();
	m_FilBtn->setText(tr(" �ļ�"));
    m_ribbon->setWindowIcon(QPixmap(":/images/welcme.png"));//(":/images/welcme.png"));
     //   
    m_CategoryApp = m_ribbon->addCategoryPage(tr("Ӧ��"));
    createCategoryApp(m_CategoryApp);
    m_CategoryViewer = m_ribbon->addCategoryPage(tr("����"));
    createCategoryView(m_CategoryViewer);
	//���¼����������ݿ� luo
    /*m_CategoryDataBase = m_ribbon->addCategoryPage(tr("���ݿ�"));
    createCategoryDateBS(m_CategoryDataBase);*/
	//���ϼ����������ݿ� luo

     //���¼������ز鿴 luo
    /*m_CategoryLookOver = m_ribbon->addCategoryPage(tr("�鿴"));
    createCategoryLookOver(m_CategoryLookOver);*/
	//���ϼ������ز鿴 luo


    m_CategoryTool = m_ribbon->addCategoryPage(tr("����"));
    createCategoryTool(m_CategoryTool);
   //���¼��������Ż� luo
   /* m_CategoryOPtim = m_ribbon->addCategoryPage(tr("�Ż�"));
    createCategoryOptim(m_CategoryOPtim);*/
	//���ϼ��������Ż� luo

	//���¼������ش��� luo
    /*m_CategoryWindow = m_ribbon->addCategoryPage(tr("����"));
    createCategoryWindow(m_CategoryWindow);*/
	//���ϼ������ش��� luo

    m_CategoryHelp = m_ribbon->addCategoryPage(tr("����"));
    createCategoryHelp(m_CategoryHelp);

    m_contextCategoryHIP = m_ribbon->addContextCategory(tr("��׽���"), Qt::red, 1);
    m_contextCategory_HIP_C = m_contextCategoryHIP->addCategoryPage(tr("����"));
    
    creatContextHIPWindow(m_contextCategory_HIP_C);

    m_contextCategoryHE = m_ribbon->addContextCategory(tr("�ȼ�ѹ"), Qt::yellow, 2);
    m_contextCategory_HE_C = m_contextCategoryHE->addCategoryPage(tr("����"));
    creatContextHEWindow(m_contextCategory_HE_C);

    m_contextCategoryHP = m_ribbon->addContextCategory(tr("�ȴ���"), Qt::blue, 3);
    m_contextCategory_HP_C = m_contextCategoryHP->addCategoryPage(tr("����"));
    creatContextHPWindow(m_contextCategory_HP_C);

	m_contextCategoryForging= m_ribbon->addContextCategory(tr("����"), Qt::yellow, 4);
    m_contextCategory_Forging_C = m_contextCategoryForging->addCategoryPage(tr("����"));
    creatContextForgingWindow(m_contextCategory_Forging_C);

	m_contextCategoryCasting= m_ribbon->addContextCategory(tr("����"), Qt::yellow, 0);
    m_contextCategory_Casting_C = m_contextCategoryCasting->addCategoryPage(tr("����"));
    creatContextCastingWindow(m_contextCategory_Casting_C);

	

    creatFileMenu();
    //-quickAcessBar
    m_ribbon->quickAccessBar()->addButton(HideShowAct_);
    //m_ribbon->quickAccessBar()->addButton(HideShowAct_);
}
//���������˵�
void MainWindow::creatFileMenu()
{ 
    m_PushFileBtn = new QPushButton(m_ribbon);
    m_PushFileBtn->hide();
    m_PushFileBtn =(QPushButton *)m_FilBtn;//ǿ��ת��ΪQPushButton �ɽ���Menu����
    m_PushFileBtn->setStyleSheet("QPushButton::menu-indicator{image:none;}");//����ʾ����������ͼƬ
    //����һ���˵�
    SARibbonMenu *fileMenu_ = new SARibbonMenu(m_ribbon);
    //fileMenu_= new QMenu(m_ribbon);
    //fileMenu_->addAction(newAct_);
    fileMenu_->addAction(openAct_);
    fileMenu_->addAction(saveAct_);
	fileMenu_->addAction(WorkingPathAct_);
    fileMenu_->addAction(mergeAct_);
    //fileMenu_->addAction(saveAsAct_);
    fileMenu_->addSeparator();
    //fileMenu_->addAction(importAct_);
    //fileMenu_->addAction(exportAct_);
    //fileMenu_->addAction(printAct_);
    //fileMenu_->addAction(LanguageC_);
    m_ChangeLanguageTitle=new QMenu();
	//���������л� luo
    /*m_ChangeLanguageTitle=fileMenu_->addMenu(tr("�����л�"));
    m_ChangeLanguageTitle->addAction(LanguageCAct_);
    m_ChangeLanguageTitle->addAction(LanguageUAct_);*/
	//���������л� luo
    fileMenu_->addAction(closeAct_);

    m_PushFileBtn->setMenu(fileMenu_);  
}

/*
������Ӧ�ô��ڴ���
*/
void MainWindow::createCategoryApp(SARibbonCategory* page)
{
    SARibbonToolButton * btn;
    SARibbonMenu* gmshMenu = new SARibbonMenu(this);
	AppKey *aadf=new AppKey(this);
	QList<bool> ShowHid;
    ShowHid<<aadf->PSystem();
	
    gmshMenu->addAction(GridDivAct_);//(QIcon(":/images/folder.png"),tr("menu"));
    gmshMenu->addAction(GridDivActClose_);//QStringLiteral("1"));
    gmshMenu->addAction(GridDivActReset_);//QStringLiteral("11"));
	gmshMenu->addAction(GridDivActOpenGMesh_);
   
    SARibbonPannel* pannel = page->addPannel(tr("Panel"));
	//���¼�����������
    /*btn=pannel->addLargeAction(AppStartAct_);
    btn->setFixedSize(78,58);*/
	//���ϼ�����������
 
    btn=pannel->addLargeAction(AppMeshAct_);
    btn->setFixedSize(78,58);
    btn->setCheckable(true);
    btn->setPopupMode(QToolButton::InstantPopup);
    AppMeshAct_->setMenu(gmshMenu);

	
	
	if(ShowHid.at(4)){
		btn=pannel->addLargeAction(AppCastingAct_);
		btn->setFixedSize(78,58);
	}
	
	if(ShowHid.at(0)){
		m_HIPBtn=pannel->addLargeAction(AppHIPAct_);
		m_HIPBtn->setFixedSize(78,58);
		m_HIPBtn->setPopupMode(QToolButton::InstantPopup);
	}
    //m_HIPBtn->setCheckable(true);

    //pannel->addWidget(m_HIPBtn);
	//���¼��������ȼ�ѹ
	
	if(ShowHid.at(1)){
		m_HEBtn=pannel->addLargeAction(AppHotExtrusionAct_);
		m_HEBtn->setFixedSize(78,58);
		m_HEBtn->setPopupMode(QToolButton::InstantPopup);
	}
	//���ϼ��������ȼ�ѹ
   // m_HEBtn->setCheckable(true);
   
    //pannel->addWidget(m_HEBtn);
	
	if(ShowHid.at(2)){
		btn=pannel->addLargeAction(AppForgingAct_);
		btn->setFixedSize(78,58);
		
	}
	
	if(ShowHid.at(3)){
		btn=pannel->addLargeAction(AppHeatProcessAct_);
		btn->setFixedSize(78,58);
		
	}

	//���¼������ؽ�� luo
    /*btn=pannel->addLargeAction(AppResultAct_);
    btn->setFixedSize(78,58);  */
	//���ϼ������ؽ�� luo
    btn=pannel->addLargeAction(AppRunAct_);
    btn->setFixedSize(78,58);

    pannel->setExpanding(0);
}
/*
�������������ڴ���
*/
void MainWindow::createCategoryView(SARibbonCategory* page)
{
    SARibbonToolButton * btn;
    SARibbonPannel* pannel = page->addPannel(tr("��ͼ����"));//(tr("Panel 1"));
    btn=pannel->addLargeAction(AddXAct_);
    btn->setFixedSize(78,58);
    btn=pannel->addLargeAction(AddYAct_);
    btn->setFixedSize(78,58);
    btn=pannel->addLargeAction(AddZAct_);
    btn->setFixedSize(78,58);
    btn=pannel->addLargeAction(SubXAct_);
    btn->setFixedSize(78,58);
    btn=pannel->addLargeAction(SubYAct_);
    btn->setFixedSize(78,58);
    btn=pannel->addLargeAction(SubZAct_);
    btn->setFixedSize(78,58);
    pannel->addSeparator();
    btn=pannel->addLargeAction(ZomeAddAct_);
    btn->setFixedSize(78,58);
    btn=pannel->addLargeAction(ZomeSubAct_);
    btn->setFixedSize(78,58);
    btn=pannel->addLargeAction(zoomFitAct_);
    btn->setFixedSize(78,58);
    pannel->setExpanding(0);
}
/*
���������ݿⴰ�ڴ���
*/
void MainWindow::createCategoryDateBS(SARibbonCategory* page)
{
    SARibbonToolButton * btn;
    SARibbonPannel* pannel = page->addPannel(tr("���ݿ�����"));//(tr("Panel 1"));
    btn=pannel->addLargeAction(MaterialLibAct_);
    btn->setFixedSize(78,58);
    btn=pannel->addLargeAction(ProcessLibAct_);
    btn->setFixedSize(78,58);
    pannel->setExpanding(0);
}
/*
�������鿴���ڴ���
*/
void MainWindow::createCategoryLookOver(SARibbonCategory* page)
{

}
/*
���������ߴ��ڴ���
*/
void MainWindow::createCategoryTool(SARibbonCategory* page)
{
    SARibbonToolButton * btn;
    SARibbonMenu* CurveMenu = new SARibbonMenu(this);
    //CurveMenu->addAction(TimHisCurveAct_);//(QIcon(":/images/folder.png"),tr("menu"));
    //CurveMenu->addAction(DistanceCurveAct_);//QStringLiteral("1"));


    SARibbonPannel* pannel = page->addPannel(tr("Tool Panel"));
    btn=pannel->addLargeAction(modelAssembleAct_);
    btn->setFixedSize(78,58);

    //btn=pannel->addLargeAction(modelScalerAct_);
    //btn->setFixedSize(78,58);

    //AppHIPAct_->setCheckable(true);
    m_HIPBtn=pannel->addLargeAction(disMeasureAct_);
    m_HIPBtn->setFixedSize(78,58);
    m_HIPBtn->setPopupMode(QToolButton::InstantPopup);

    m_HEBtn=pannel->addLargeAction(createSetAct_);
    m_HEBtn->setFixedSize(78,58);
    m_HEBtn->setPopupMode(QToolButton::InstantPopup);

    //���¼��������������� luo
   /* btn=pannel->addLargeAction(courseCurveAct_);
    btn->setFixedSize(78,58);
    btn->setCheckable(true);*/
	//���ϼ��������������� luo

    //btn->setPopupMode(QToolButton::InstantPopup);
    //courseCurveAct_->setMenu(CurveMenu);

    btn=pannel->addLargeAction(postPropAct_);
    btn->setFixedSize(78,58);

    SARibbonMenu* ConstantMenu = new SARibbonMenu(this);
    //ConstantMenu->addAction(TimHisCurveAct_);//(QIcon(":/images/folder.png"),tr("menu"));
   // ConstantMenu->addAction(DistanceCurveAct_);//QStringLiteral("1"));

    //���¼������س��� luo 
    /*btn=pannel->addLargeAction(constantAct_);
    btn->setFixedSize(78,58);
    btn->setCheckable(true);*/
	//���ϼ������س��� luo
	
    btn->setPopupMode(QToolButton::InstantPopup);
    constantAct_->setMenu(ConstantMenu);
    /*btn=pannel->addLargeAction(AppResultAct_);
    btn->setFixedSize(78,58);  
    btn=pannel->addLargeAction(AppRunAct_);
    btn->setFixedSize(78,58);*/

	//--������ʾ����
	btn=pannel->addLargeAction(partElSetHide_);
	btn->setFixedSize(78,58);

	btn=pannel->addLargeAction(gravity_);
	btn->setFixedSize(78,58);

	btn=pannel->addLargeAction(quantitystatistics_);
	btn->setFixedSize(78,58);
	
    pannel->setExpanding(0);
}
/*
�������Ż����ڴ���
*/
void MainWindow::createCategoryOptim(SARibbonCategory* page)
{

}

/*
���������ڴ���
*/
void MainWindow::createCategoryWindow(SARibbonCategory* page)
{
    SARibbonToolButton * btn;
    SARibbonPannel* pannel = page->addPannel(tr("����"));//(tr("Panel 1"));
    btn=pannel->addLargeAction(CascadeAct_);
    btn->setFixedSize(78,58);
    btn=pannel->addLargeAction(TileAct_);
    btn->setFixedSize(78,58);
    btn=pannel->addLargeAction(CloseAllSubWindowAct_);
    btn->setFixedSize(78,58);
    btn=pannel->addLargeAction(CloseSubWindowAct_);
    btn->setFixedSize(78,58);
    btn=pannel->addLargeAction(ChooseSubWindowAct_);
    btn->setFixedSize(78,58);
}
/*
�������������ڴ���
*/
void MainWindow::createCategoryHelp(SARibbonCategory* page)
{
    SARibbonToolButton * btn;
    SARibbonPannel* pannel = page->addPannel(tr("����"));//(tr("Panel 1"));
    btn=pannel->addLargeAction(helpAct_);
    btn->setFixedSize(78,58);
	//���¼������ز�������� luo
    /*btn=pannel->addLargeAction(helpLookForAct_);
    btn->setFixedSize(78,58);
    btn=pannel->addLargeAction(helpAboutAct_);
    btn->setFixedSize(78,58);*/
	//���ϼ������ز�������� luo
}
//�ȵȾ�ѹ
void MainWindow::creatContextHIPWindow(SARibbonCategory* page)
{
    SARibbonToolButton * btn;
    SARibbonPannel* pannel = page->addPannel(tr("��׽���"));//(tr("Panel 1"));
    m_HIPPartBtn=pannel->addLargeAction(HIPpartAct_);
    m_HIPPartBtn->setFixedSize(78,58);
    m_HIPPartBtn->setEnabled(true);
    m_HIPBcBtn=pannel->addLargeAction(HIPBCAct_);
    m_HIPBcBtn->setFixedSize(78,58);
    m_HIPBcBtn->setEnabled(false);
    m_HIPInitBtn=pannel->addLargeAction(HIPInitAct_);
    m_HIPInitBtn->setFixedSize(78,58);
    m_HIPInitBtn->setEnabled(false);
    m_HIPSystemBtn=pannel->addLargeAction(HIPSystemAct_);
    m_HIPSystemBtn->setFixedSize(78,58);
    m_HIPSystemBtn->setEnabled(false);
    m_HIPSolveSetBtn=pannel->addLargeAction(HIPSolveAct_);
    m_HIPSolveSetBtn->setFixedSize(78,58);
    m_HIPSolveSetBtn->setEnabled(false);
	btn=pannel->addLargeAction(HIPSubmissonAct_);
	btn->setFixedSize(78,58);
    pannel->setExpanding(0);
}
//�ȼ�ѹ
void MainWindow::creatContextHEWindow(SARibbonCategory* page)
{
    SARibbonToolButton * btn;
    SARibbonPannel* pannel = page->addPannel(tr("��׽���"));//(tr("Panel 1"));
    btn=pannel->addLargeAction(HIPpartAct_);
    btn->setFixedSize(78,58);
    btn=pannel->addLargeAction(HIPBCAct_);
    btn->setFixedSize(78,58);
}

//����
void MainWindow::creatContextForgingWindow(SARibbonCategory* page)
{
	SARibbonToolButton * btn;
    SARibbonPannel* pannel = page->addPannel(tr("����"));//(tr("Panel 1"));
    m_ForingPartBtn=pannel->addLargeAction(ForgingpartAct_);
    m_ForingPartBtn->setFixedSize(78,58);
	m_ForingPartBtn->setEnabled(true);
    m_ForingBcBtn=pannel->addLargeAction(ForgingBCAct_);
    m_ForingBcBtn->setFixedSize(78,58);
	m_ForingBcBtn->setEnabled(false);
    m_ForingContactBtn=pannel->addLargeAction(ForgingContact_);
    m_ForingContactBtn->setFixedSize(78,58);
	m_ForingContactBtn->setEnabled(false);
	m_ForingInitBtn=pannel->addLargeAction(ForgingInitAct_);
    m_ForingInitBtn->setFixedSize(78,58);
	m_ForingInitBtn->setEnabled(false);
	m_ForingTBBtn=pannel->addLargeAction(ForgingTBAct_);
    m_ForingTBBtn->setFixedSize(78,58);
	m_ForingTBBtn->setEnabled(false);
    m_ForingSystemBtn=pannel->addLargeAction(ForgingSystemAct_);
    m_ForingSystemBtn->setFixedSize(78,58);
	m_ForingSystemBtn->setEnabled(false);
    m_ForingSolveSetBtn=pannel->addLargeAction(ForgingSloveAct_);
    m_ForingSolveSetBtn->setFixedSize(78,58);
    btn=pannel->addLargeAction(ForgingSubmissonAct_);
    btn->setFixedSize(78,58);

    pannel->setExpanding(0);

}

//��������
void MainWindow::creatContextCastingWindow(SARibbonCategory* page)
{

	SARibbonToolButton * btn;
	SARibbonPannel* pannel = page->addPannel(tr("����"));//(tr("Panel 1"));
    m_GravityBtn=pannel->addLargeAction(CastingGravityAct_);
    m_GravityBtn->setFixedSize(78,58);
	m_GravityBtn->setEnabled(true);

    m_CastingPartBtn=pannel->addLargeAction(CastingpartAct_);
    m_CastingPartBtn->setFixedSize(78,58);
	m_CastingPartBtn->setEnabled(false);
    m_CastingBcBtn=pannel->addLargeAction(CastingBCAct_);
    m_CastingBcBtn->setFixedSize(78,58);
	m_CastingBcBtn->setEnabled(false);
    m_CastingContactBtn=pannel->addLargeAction(CastingContact_);
    m_CastingContactBtn->setFixedSize(78,58);
	m_CastingContactBtn->setEnabled(false);
	m_CastingInitBtn=pannel->addLargeAction(CastingInitAct_);
    m_CastingInitBtn->setFixedSize(78,58);
	m_CastingInitBtn->setEnabled(false);
	m_CastingTBBtn=pannel->addLargeAction(CastingTBAct_);
    m_CastingTBBtn->setFixedSize(78,58);
	m_CastingTBBtn->setEnabled(false);
	m_CastingMotionBtn=pannel->addLargeAction(CastingMotionAct_);
    m_CastingMotionBtn->setFixedSize(78,58);
	m_CastingMotionBtn->setEnabled(false);
    /*m_CastingSystemBtn=pannel->addLargeAction(CastingSystemAct_);
    m_CastingSystemBtn->setFixedSize(78,58);
	m_CastingSystemBtn->setEnabled(false);*/
    m_CastingSolveSetBtn=pannel->addLargeAction(CastingSloveAct_);
    m_CastingSolveSetBtn->setFixedSize(78,58);
    btn=pannel->addLargeAction(CastingSubmissonAct_);
    btn->setFixedSize(78,58);
    pannel->setExpanding(0);
}

//�ȴ���
void MainWindow::creatContextHPWindow(SARibbonCategory* page)
{
    SARibbonToolButton * btn;
    SARibbonPannel* pannel = page->addPannel(tr("�ȴ���"));//(tr("Panel 1"));
    m_HPPartBtn=pannel->addLargeAction(HPpartAct_);
    m_HPPartBtn->setFixedSize(78,58);
    m_HPPartBtn->setEnabled(true);
    m_HPBcBtn=pannel->addLargeAction(HPBCAct_);
    m_HPBcBtn->setFixedSize(78,58);
    m_HPBcBtn->setEnabled(false);
    m_HPInitBtn=pannel->addLargeAction(HPInitAct_);
    m_HPInitBtn->setFixedSize(78,58);
    m_HPInitBtn->setEnabled(false);
    m_HPSystemBtn=pannel->addLargeAction(HPSystemAct_);
    m_HPSystemBtn->setFixedSize(78,58);
    m_HPSystemBtn->setEnabled(false);
    m_HPSolveSetBtn=pannel->addLargeAction(HPSloveAct_);
    m_HPSolveSetBtn->setFixedSize(78,58);
    m_HPSolveSetBtn->setEnabled(false);
    //btn=pannel->addLargeAction(HPCheckAct_);
   // btn->setFixedSize(78,58);
    btn=pannel->addLargeAction(HPSubmissonAct_);
    btn->setFixedSize(78,58);
    pannel->setExpanding(0);
}
//--
void MainWindow::createActions()
{
	//���¼��д�������һ�ιرյ�·����������Ϊ��ǰ·��
	QString str = QCoreApplication::applicationDirPath();
	str+="\\path\\WorkPatch.txt";
	QFile f(str);
	if(f.open(QIODevice::ReadOnly)){
		QByteArray t=f.readAll();
		QDir::setCurrent(QString(t));
	}
	

	//���¼��д�������һ�ιرյ�·����������Ϊ��ǰ·��

	newAct_ = new QAction(QIcon(":/images/NewIcon.png"), tr("&New"), this);
	newAct_->setShortcuts(QKeySequence::New);
	newAct_->setStatusTip(tr("Create a new project"));
    //connect(newAct_,SIGNAL(triggered()),this,SLOT(NewSlot()));
    
	//�ļ�
	openAct_ = new QAction(QIcon(":/images/open.png"), tr("Open"), this);
	openAct_->setShortcuts(QKeySequence::Open);
	connect(openAct_,SIGNAL(triggered()),this,SLOT(OpenSlot()));

    mergeAct_ = new QAction(QIcon(":/images/merge.png"), tr("Merge"), this);
    connect(mergeAct_,SIGNAL(triggered()),this,SLOT(MergeSlot()));

    saveAct_ = new QAction(QIcon(":/images/save.png"), tr("Save"), this);
	saveAct_->setShortcuts(QKeySequence::Save);
	saveAct_->setStatusTip(tr("Save"));
	connect(saveAct_,SIGNAL(triggered()),this,SLOT(SaveSlot()));

	WorkingPathAct_ = new QAction(QIcon(":/images/workingpath.png"), tr("WorkingPath"), this);
	/*WorkingPathAct_->setShortcuts(QKeySequence::WorkingPath);
	WorkingPathAct_->setStatusTip(tr("WorkingPath"));*/
	connect(WorkingPathAct_,SIGNAL(triggered()),this,SLOT(ChangeWorkPath()));


	saveAsAct_ = new QAction(tr("SaveAs"), this);
	importAct_=new QAction(QIcon(":/images/save.png"), tr("Import"), this);
	exportAct_=new QAction(QIcon(":/images/save.png"), tr("Export"), this);
	printAct_=new QAction(QIcon(":/images/save.png"), tr("Print"), this);
    closeAct_ = new QAction(tr("Close"), this);
    closeAct_->setShortcut(tr("Ctrl+W"));
    closeAct_->setStatusTip(tr("Close the App"));
    connect(importAct_,SIGNAL(triggered()),this,SLOT(ImportPartSlot()));
    connect(exportAct_,SIGNAL(triggered()),this,SLOT(ExportPartSlot()));
    connect(closeAct_,SIGNAL(triggered()),this,SLOT(close()));

    LanguageCAct_=new QAction(tr("Chinese"),this);
    LanguageCAct_->setCheckable(true);
    LanguageUAct_=new QAction(tr("Ӣ��"),this);
    LanguageUAct_->setCheckable(true);
    connect(LanguageCAct_,SIGNAL(triggered()),this,SLOT(LanguageZHSlot()));//
    connect(LanguageUAct_,SIGNAL(triggered()),this,SLOT(LanguageENSlot()));//
    m_Lin =CLinguist::GetLinguistInstance();
    connect(m_Lin,SIGNAL(LanguageChanged(int)),this,SLOT(retranslateUi(int)));
	//-���񻮷�
	GridDivAct_= new QAction(QIcon(":/images/showon.png"), tr("GMesh"), this);
	GridDivActClose_= new QAction(QIcon(":/images/showoff.png"), tr("Close GMesh"), this);
    GridDivActReset_= new QAction(tr("����GMesh"), this);
	GridDivActOpenGMesh_=new QAction(tr("��GMesh"), this);

	//--�Ӵ���Լ��
	//TStyleAct_=new QAction(QIcon("/save.png"), tr("Contact Style"), this);
	//MainSectPrtyAct_=new QAction(QIcon("/save.png"), tr("Master-slave Plane"), this);
	//TPropertyAct_=new QAction(QIcon("/save.png"), tr("Contact Property"), this);
	//BondLimitAct_=new QAction(QIcon("/save.png"), tr("Binding Constraint"), this);
	//MPointLimitAct_=new QAction(QIcon("/save.png"), tr("Multipoint Constraint"), this);
	//--�߽�����
	//PosEdgeAct_=new QAction(QIcon("/save.png"), tr("Position"), this);
	//PowEdgeAct_=new QAction(QIcon("/save.png"), tr("Force boundary"), this);
	//TmptEdgeAct_=new QAction(QIcon("/save.png"), tr("Temperature"), this);
	//EdgeIOAct_=new QAction(QIcon("/save.png"), tr("Boundary I/O"), this);
	//--�������
	//SolverChsAct_=new QAction(QIcon("/save.png"), tr("Solver Choose"), this);
	//GemoNolinearAct_=new QAction(QIcon("/save.png"), tr("Geometric Nonlinearity"), this);
	//SolverCtlParaAct_=new QAction(QIcon("/save.png"), tr("Solver Param"), this);
	//ResetAct_=new QAction(QIcon("/save.png"), tr("Reset"), this);
	//--���
	//FrdFileAct_=new QAction(QIcon("/save.png"), tr("frd File Setting"), this);
	//DatFileAct_=new QAction(QIcon("/save.png"), tr("dat File Setting"), this);
	//MEdgeDataSaveAct_=new QAction(QIcon("/save.png"), tr("Material Boundary DataSave"), this);
    
	//--HELP
    helpAct_ = new QAction(QIcon(":/images/save.png"),tr("����"), this);
	helpAct_->setStatusTip(tr("help"));
	//���¼������ز�������� luo
    /*helpLookForAct_ = new QAction(QIcon(":/images/save.png"),tr("Look For"), this);
    helpLookForAct_->setStatusTip(tr("Look For"));
    helpAboutAct_ = new QAction(QIcon(":/images/save.png"),tr("About"), this);
    helpAboutAct_->setStatusTip(tr("About"));*/
	//���ϼ������ز�������� luo

	//--����+/-xyz,+/-/auto zoom
    AddXAct_ = new QAction(QIcon(":/images/+x.png"),tr("+X"), this);
    AddXAct_->setStatusTip(tr("+X"));
    AddYAct_ = new QAction(QIcon(":/images/+y.png"),tr("+Y"), this);
    AddZAct_ = new QAction(QIcon(":/images/+z.png"),tr("+Z"), this);
    SubXAct_ = new QAction(QIcon(":/images/-x.PNG"),tr("-X"), this);
    SubYAct_ = new QAction(QIcon(":/images/-y.PNG"),tr("-Y"), this);
    SubZAct_ = new QAction(QIcon(":/images/-z.PNG"),tr("-Z"), this);
    ZomeAddAct_ = new QAction(QIcon(":/images/zoom+.png"),tr("�Ŵ�"), this); 
    ZomeSubAct_ = new QAction(QIcon(":/images/zoom-.png"),tr("��С"), this); 
    zoomFitAct_ = new QAction(QIcon(":/images/ResetZoom.png"),tr("����"), this);

    //backgroud��axis
    bgAct_ = new QAction(QIcon(":/images/bg.png"),tr("BackGroundColor"), this);
    axisAct_ = new QAction(QIcon(":/images/axis.png"),tr("axis"), this);

   
    //tools connect
    QSignalMapper *signalMapper = new QSignalMapper(this);
    connect(AddXAct_, SIGNAL(triggered()), signalMapper, SLOT(map()));
    signalMapper->setMapping(AddXAct_, "+X");
    connect(AddYAct_, SIGNAL(triggered()), signalMapper, SLOT(map()));
    signalMapper->setMapping(AddYAct_, "+Y");
    connect(AddZAct_, SIGNAL(triggered()), signalMapper, SLOT(map()));
    signalMapper->setMapping(AddZAct_, "+Z");
    connect(SubXAct_, SIGNAL(triggered()), signalMapper, SLOT(map()));
    signalMapper->setMapping(SubXAct_, "-X");
    connect(SubYAct_, SIGNAL(triggered()), signalMapper, SLOT(map()));
    signalMapper->setMapping(SubYAct_, "-Y");
    connect(SubZAct_, SIGNAL(triggered()), signalMapper, SLOT(map()));
    signalMapper->setMapping(SubZAct_, "-Z");
    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(ViewDirectionChangedSlot(QString)));

    QSignalMapper *signalMapper2 = new QSignalMapper(this);     
    connect(ZomeAddAct_, SIGNAL(triggered()), signalMapper2, SLOT(map()));
    signalMapper2->setMapping(ZomeAddAct_, 1);
    connect(ZomeSubAct_, SIGNAL(triggered()), signalMapper2, SLOT(map()));
    signalMapper2->setMapping(ZomeSubAct_, -1);
    connect(zoomFitAct_, SIGNAL(triggered()), signalMapper2, SLOT(map()));
    signalMapper2->setMapping(zoomFitAct_, 0);
    connect(signalMapper2, SIGNAL(mapped(int)), this, SLOT(ViewSizeChangedSlot(int)));

    QSignalMapper *signalMapper3 = new QSignalMapper(this);
    connect(bgAct_,SIGNAL(triggered()),signalMapper3, SLOT(map()));
    signalMapper3->setMapping(bgAct_, 1);
    connect(axisAct_,SIGNAL(triggered()),signalMapper3, SLOT(map()));
    signalMapper3->setMapping(axisAct_, 2);
    connect(signalMapper3, SIGNAL(mapped(int)), this, SLOT(ViewChangedSlot(int)));
    
	////--connect
	connect(GridDivAct_,SIGNAL(triggered()),this,SLOT(GridDivActPrc()));
	connect(GridDivActClose_,SIGNAL(triggered()),this,SLOT(GridDivActPrcClose()));
    connect(GridDivActReset_,SIGNAL(triggered()),this,SLOT(GridDivActPrcReset()));
	connect(GridDivActOpenGMesh_,SIGNAL(triggered()),this,SLOT(GridDivActPrcOpen()));

    //--������������ر����С�ѡ��
    CascadeAct_ = new QAction(QIcon(":/images/save.png"), tr("����"), this);
    TileAct_    = new QAction(QIcon(":/images/save.png"), tr("ƽ��"), this);
    CloseSubWindowAct_ =  new QAction(QIcon(":/images/save.png"), tr("�رյ�ǰ"), this);
    CloseAllSubWindowAct_  = new QAction(QIcon(":/images/save.png"), tr("�ر�����"), this);
    ChooseSubWindowAct_    = new QAction(QIcon(":/images/save.png"), tr("ѡ��"), this);
   
    //---
    //connect(CascadeAct_,SIGNAL(triggered()),this,SLOT(CascadeSlot()));	
    //connect(TileAct_,SIGNAL(triggered()),this,SLOT(TileSlot()));
    //connect(CloseAllSubWindowAct_,SIGNAL(triggered()),this,SLOT(CloseAllSubWindowSlot()));	
    //connect(CloseSubWindowAct_,SIGNAL(triggered()),this,SLOT(CloseSubWindowSlot()));	
    //connect(ChooseSubWindowAct_,SIGNAL(triggered()),this,SLOT(ChooseSubWindowSlot()));
    //
    //
    HideShowAct_ = new QAction(QIcon(":/images/ArrowDown.png"), tr("hideshowRibbon"), this);
    HideShowAct_->setCheckable(true);
    connect(HideShowAct_,SIGNAL(triggered(bool)),this,SLOT(HideShowSlot(bool)));

    //--Ribbon APP
    AppStartAct_ = new QAction(QIcon(":/images/MainStart.png"), tr("����"), this);
    AppStartAct_->setCheckable(true);
    AppMeshAct_ = new QAction(QIcon(":/images/mesh.png"), tr("����"), this);
    AppHIPAct_ = new QAction(QIcon(":/images/HIP.png"), tr("��׽���"), this);

    AppHotExtrusionAct_ = new QAction(QIcon(":/images/HE.png"), tr("�ȼ�ѹ"), this);
    AppForgingAct_ = new QAction(QIcon(":/images/Forging.png"), tr("����"), this);
    AppHeatProcessAct_ = new QAction(QIcon(":/images/HP.png"), tr("�ȴ���"), this);
	AppCastingAct_= new QAction(QIcon(":/images/casting.png"), tr("����"), this);//����
    AppResultAct_ = new QAction(QIcon(":/images/MainResult.png"), tr("���"), this);
    AppRunAct_ = new QAction(QIcon(":/images/MainRun.png"), tr("����"), this);
	

    connect(AppStartAct_, SIGNAL(triggered()), this, SLOT(AppStartDlgSlot()));
    connect(AppHIPAct_, SIGNAL(triggered(bool)), this, SLOT(AddInNewMenuHIPSlot(bool)));
    connect(AppHotExtrusionAct_, SIGNAL(triggered(bool)), this, SLOT(AddInNewMenuHESlot(bool)));
    connect(AppHeatProcessAct_,SIGNAL(triggered(bool)), this, SLOT(AddInNewMenuHPSlot(bool)));
    connect(AppRunAct_,SIGNAL(triggered()),this,SLOT(ForgingSubmissonActSlot()));//���������(ĿǰĬ�ϵ�ΪHIP�����)
	connect(AppForgingAct_,SIGNAL(triggered(bool)), this, SLOT(AddInNewMenuForgingSlot(bool)));
	connect(AppCastingAct_,SIGNAL(triggered(bool)), this, SLOT(AddInNewMenuCastingSlot(bool)));//
    //--���ݿ�
    MaterialLibAct_ = new QAction(QIcon(":/images/save.png"), tr("�������ݿ�"), this);
    ProcessLibAct_ = new QAction(QIcon(":/images/save.png"), tr("�������ݿ�"), this);

    //--�˵�����
    modelAssembleAct_= new QAction(QIcon(":/images/assembling.png"), tr("װ��/����"), this);
    //modelScalerAct_= new QAction(QIcon(":/images/save.png"), tr("ģ������"), this);
    disMeasureAct_= new QAction(QIcon(":/images/distance.png"), tr("�������"), this);
    createSetAct_= new QAction(QIcon(":/images/creatset.png"), tr("��������"), this);
    courseCurveAct_ = new QAction(QIcon(":/images/save.png"), tr("��������"), this);
    constantAct_= new QAction(QIcon(":/images/save.png"), tr("����"), this);
    //TimHisCurveAct_=new QAction(QIcon(":/images/save.png"), tr("Time"), this);
    //DistanceCurveAct_=new QAction(QIcon(":/images/save.png"), tr("Distance"), this);
    PUAct_=new QAction(QIcon(":/images/save.png"), tr("PU"), this);
    postPropAct_=new QAction(QIcon(":/images/attribute.png"), tr("��������"), this);
	partElSetHide_=new QAction(QIcon(":/images/showhide.png"),tr("������ʾ"),this);//������ʾ����
	gravity_=new QAction(QIcon(":/images/gravity.png"),tr("����"),this);
	quantitystatistics_=new QAction(QIcon(":/images/Quantitystatistics.png"),tr("����ͳ��"),this);

    connect(modelAssembleAct_,SIGNAL(triggered()),this,SLOT(AssembleDlgSlot()));//����װ��
	connect(disMeasureAct_,SIGNAL(triggered()),this,SLOT(DistanceMeasurementSlot()));//�������
    connect(courseCurveAct_,SIGNAL(triggered()),this,SLOT(courseCurveDlgSlot()));//��������
    connect(postPropAct_,SIGNAL(triggered()),this,SLOT(postPropDlgSlot()));//��������
    connect(createSetAct_,SIGNAL(triggered()),this,SLOT(CreateSetDlgSlot()));//��������
	connect(partElSetHide_,SIGNAL(triggered()),this,SLOT(partElSetHideDlgSlot()));//������ʾ���أ�
	connect(gravity_,SIGNAL(triggered()),this,SLOT(GravitySlot()));//������
	connect(quantitystatistics_,SIGNAL(triggered()),this,SLOT(QStatisticsSlot()));//������

    //--�ȵȾ�ѹ
    HIPpartAct_ = new QAction(QIcon(":/images/part.png"), tr("����"), this);
    HIPBCAct_ = new QAction(QIcon(":/images/BC.png"), tr("�߽�"), this);
    HIPInitAct_= new QAction(QIcon(":/images/init.png"), tr("��ʼ��"), this);
    HIPSystemAct_ = new QAction(QIcon(":/images/systerm.png"), tr("�ƶ�"), this);
    HIPSolveAct_ = new QAction(QIcon(":/images/solverset.png"), tr("�������"), this);
    HIPCheckAct_ = new QAction(QIcon(":/images/save.png"), tr("���"), this);
    HIPSubmissonAct_ = new QAction(QIcon(":/images/updatacalc.png"), tr("�ύ����"), this);
    HIPVariableOutPutAct_ = new QAction(QIcon(":/images/check.png"), tr("�������"), this);
    HIPRunAct_= new QAction(QIcon(":/images/updatacalc.png"), tr("����"), this);


    connect(HIPpartAct_,SIGNAL(triggered()),this,SLOT(HIPpartActSlot()));
    connect(HIPBCAct_,SIGNAL(triggered()),this,SLOT(HIPBCActSlot()));
    connect(HIPInitAct_,SIGNAL(triggered()),this,SLOT(HIPInitDlgActSlot()));
    connect(HIPSystemAct_,SIGNAL(triggered()),this,SLOT(HIPSystemDlgActSlot()));//�ȵȾ�ѹ�ƶ�
    connect(HIPSolveAct_,SIGNAL(triggered()),this,SLOT(HIPResolveDlgActSlot()));//�����
    connect(HIPVariableOutPutAct_,SIGNAL(triggered()),this,SLOT(HIPVariableOutPutDlgSlot()));//�������
    //connect(HIPRunAct_,SIGNAL(triggered()),this,SLOT(HIPSolveActOpenSlot()));//���������
	connect(HIPSubmissonAct_,SIGNAL(triggered()),this,SLOT(HIPSubmissonActSlot()));

    //--�ȴ���
    HPpartAct_ = new QAction(QIcon(":/images/part.png"), tr("����"), this);
    HPBCAct_ = new QAction(QIcon(":/images/BC.png"), tr("�߽�"), this);
    HPInitAct_= new QAction(QIcon(":/images/init.png"), tr("��ʼ��"), this);
    HPSystemAct_ = new QAction(QIcon(":/images/systerm.png"), tr("�ȴ����ƶ�"), this);
    HPSloveAct_ = new QAction(QIcon(":/images/solverset.png"), tr("�������"), this);
    //HPCheckAct_ = new QAction(QIcon(":/images/check.png"), tr("���"), this);
    HPSubmissonAct_ = new QAction(QIcon(":/images/updatacalc.png"), tr("�ύ����"), this);

    connect(HPpartAct_,SIGNAL(triggered()),this,SLOT(HPpartActSlot()));
    connect(HPBCAct_,SIGNAL(triggered()),this,SLOT(HPBCActSlot()));
    connect(HPInitAct_,SIGNAL(triggered()),this,SLOT(HPInitActSlot()));
    connect(HPSystemAct_,SIGNAL(triggered()),this,SLOT(HPSystemActSlot()));
    connect(HPSloveAct_,SIGNAL(triggered()),this,SLOT(HPSloveSetActSlot()));
    connect(HPSubmissonAct_,SIGNAL(triggered()),this,SLOT(HPSubmissonActSlot()));
    //connect(HPSubmissonAct_,SIGNAL(triggered()),this,SLOT(HIPSolveActOpenSlot()));//���������

	//����
	ForgingpartAct_=new QAction(QIcon(":/images/part.png"), tr("����"), this);
    ForgingBCAct_=new QAction(QIcon(":/images/BC.png"), tr("�߽�"), this);
    ForgingContact_=new QAction(QIcon(":/images/contact.png"), tr("�Ӵ�"), this);
    ForgingInitAct_=new QAction(QIcon(":/images/init.png"), tr("��ʼ��"), this);
	ForgingTBAct_=new QAction(QIcon(":/images/huo.png"), tr("�ȱ߽�"), this);
    ForgingSystemAct_=new QAction(QIcon(":/images/systerm.png"), tr("�����ƶ�"), this);
    ForgingSloveAct_=new QAction(QIcon(":/images/solverset.png"), tr("�������"), this);
    ForgingSubmissonAct_=new QAction(QIcon(":/images/updatacalc.png"), tr("�ύ����"), this);

	connect(ForgingpartAct_,SIGNAL(triggered()),this,SLOT(ForgingpartActSlot()));
	connect(ForgingBCAct_,SIGNAL(triggered()),this,SLOT(ForgingBCActSlot()));
	connect(ForgingContact_,SIGNAL(triggered()),this,SLOT(ForgingContactActSlot()));
	connect(ForgingInitAct_,SIGNAL(triggered()),this,SLOT(ForgingInitActSlot()));
	connect(ForgingTBAct_,SIGNAL(triggered()),this,SLOT(ForgingHBSlot()));
	connect(ForgingSystemAct_,SIGNAL(triggered()),this,SLOT(ForgingSystemActSlot()));
	connect(ForgingSloveAct_,SIGNAL(triggered()),this,SLOT(ForgingSloveSetActSlot()));
    connect(ForgingSubmissonAct_,SIGNAL(triggered()),this,SLOT(ForgingSubmissonActSlot()));

	//����
	CastingGravityAct_=new QAction(QIcon(":/images/gravity.png"), tr("����"), this);
	CastingpartAct_=new QAction(QIcon(":/images/part.png"), tr("����"), this);
    CastingBCAct_=new QAction(QIcon(":/images/BC.png"), tr("�߽�"), this);
    CastingContact_=new QAction(QIcon(":/images/contact.png"), tr("�Ӵ�"), this);
    CastingInitAct_=new QAction(QIcon(":/images/init.png"), tr("��ʼ��"), this);
	CastingTBAct_=new QAction(QIcon(":/images/huo.png"), tr("�ȱ߽�"), this);

	CastingMotionAct_=new QAction(QIcon(":/images/motionboundary.png"), tr("�˶���ϵ"), this);

    CastingSystemAct_=new QAction(QIcon(":/images/systerm.png"), tr("�����ƶ�"), this);
    CastingSloveAct_=new QAction(QIcon(":/images/solverset.png"), tr("�������"), this);
    CastingSubmissonAct_=new QAction(QIcon(":/images/updatacalc.png"), tr("�ύ����"), this);

	connect(CastingGravityAct_,SIGNAL(triggered()),this,SLOT(CastingGravitySlot()));
	connect(CastingpartAct_,SIGNAL(triggered()),this,SLOT(CastingpartActSlot()));
	connect(CastingBCAct_,SIGNAL(triggered()),this,SLOT(CastingBCActSlot()));
	connect(CastingContact_,SIGNAL(triggered()),this,SLOT(CastingContactActSlot()));
	connect(CastingInitAct_,SIGNAL(triggered()),this,SLOT(CastingInitActSlot()));
	connect(CastingTBAct_,SIGNAL(triggered()),this,SLOT(CastingHBSlot()));

	connect(CastingMotionAct_,SIGNAL(triggered()),this,SLOT(CastingMotionBoundarySlot()));

	connect(CastingSystemAct_,SIGNAL(triggered()),this,SLOT(CastingSystemActSlot()));
	connect(CastingSloveAct_,SIGNAL(triggered()),this,SLOT(CastingSloveSetActSlot()));
    connect(CastingSubmissonAct_,SIGNAL(triggered()),this,SLOT(CastingSubmissonActSlot()));
}

void MainWindow::createDockWindows()
{
//    
//    //--Some
//    dockGeometry_ =new QDockWidget(tr("���ƹ�������"));
//    dockGeometry_->setMinimumWidth(100);
//    dockGeometry_->setMaximumWidth(500);
//    areaGeometry_ = new QScrollArea(dockGeometry_);
//    dockGeometry_->setWidget(areaGeometry_);
//    areaGeometry_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
//    areaGeometry_->setWidgetResizable(true);
//    //pageGeometry_ = new PageGeometry(areaGeometry_);
//    //addDockWidget(Qt::LeftDockWidgetArea,dockGeometry_);
//    addDockWidget(Qt::LeftDockWidgetArea,dockGeometry_);
//    //this->setCorner(Qt::BottomLeftCorner,Qt::LeftDockWidgetArea);
//    //splitDockWidget(ProjectTree_,dockGeometry_,Qt::Horizontal);//�зִ���

    DockPreHIPPrc_ = new QDockWidget(tr("Pre"),this);
    areaProHIPPrc_ = new QScrollArea(DockPreHIPPrc_);
    ////-1.1���������
    //QWidget *oldtitlebar=DockFrowardPrc_->titleBarWidget();
    //DockFrowardPrc_->setTitleBarWidget(new QWidget());
    //if (oldtitlebar){
    //	delete oldtitlebar;
    //}
    areaProHIPPrc_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    DockPreHIPPrc_->setWidget(areaProHIPPrc_);
    areaProHIPPrc_->setWidgetResizable(true);
    DockPreHIPPrc_->setMinimumWidth(200);
    DockPreHIPPrc_->setMaximumWidth(1000);
    DockPreHIPPrc_->resize(100,300);
    PreHIPPro_ = new QPreHIPPrc(this);
    DockPreHIPPrc_->setWidget(PreHIPPro_);
	PreHIPPro_->setAutoFillBackground(true);
    PreHIPPro_->setStyle();
    addDockWidget(Qt::LeftDockWidgetArea,DockPreHIPPrc_);
	
	//-2.����
	DockPostPrc_ = new QDockWidget(tr("Post"),this);
	areaPostPrc_ = new QScrollArea(DockPostPrc_);
	areaPostPrc_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	DockPostPrc_->setWidget(areaPostPrc_);
	areaPostPrc_->setWidgetResizable(true);
	DockPostPrc_->setMinimumWidth(200);
	DockPostPrc_->setMaximumWidth(1000);
	DockPostPrc_->setMinimumHeight(500);
    DockPostPrc_->resize(100,300);
	PostPro_ = new QPostPrc(this);
	areaPostPrc_->setWidget(PostPro_);
	PostPro_->setAutoFillBackground(true);
	PostPro_->setStyle();
	addDockWidget(Qt::LeftDockWidgetArea, DockPostPrc_);
	
	//--
	setTabPosition(Qt::LeftDockWidgetArea, QTabWidget::North);
	setCorner(Qt::BottomLeftCorner,Qt::LeftDockWidgetArea);
	tabifyDockWidget(DockPreHIPPrc_,DockPostPrc_);//�������

	connect(DockPreHIPPrc_,SIGNAL(topLevelChanged(bool)),this,SLOT(changeS(bool)));

    /* 
    //--view Center
    viewWindow_ = new QWindowView();
    this->setCentralWidget(viewWindow_);
    viewWindow_->setWindowTitle("3D View");
   */
    m_MdiArea = new QMdiArea;
    m_MdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_MdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(m_MdiArea);
    m_MdiArea->setViewMode(QMdiArea::TabbedView);
    m_MdiArea->setTabsClosable(false);
    m_MdiArea->setTabsMovable(true);
    m_MdiArea->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
    //m_MdiArea->setDocumentMode(true);
    //m_MdiArea->setContextMenuPolicy(Qt::NoContextMenu);
    //m_MdiArea->setFocusPolicy(Qt::NoFocus);
    //m_MdiArea->setContextMenuPolicy(Qt::CustomContextMenu);
    //m_MdiArea->setContentsMargins(0,0,0,0);
   // m_MdiArea->setViewMode(QMdiArea::TabbedView);
   // m_MdiArea->setTabShape(QTabWidget::Triangular);

    //connect(m_MdiArea,SIGNAL(subWindowActivated(QMdiSubWindow*)),this,SLOT(NowActivateWindowSlot(QMdiSubWindow*)));
  
    NewSlot();
	//--��Ϣ
	dockInfomation_ = new QDockWidget(tr("Information"),this);
	dockInfomation_->setAllowedAreas(Qt::AllDockWidgetAreas);
	areaInfomation_ = new QScrollArea(dockInfomation_);
	dockInfomation_->setWidget(areaInfomation_);
	areaInfomation_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	areaInfomation_->setWidgetResizable(true);
	pageInfomation_ = Information_Widget::GetInstance();
	areaInfomation_->setWidget(pageInfomation_);
	pageInfomation_->setAutoFillBackground(true);
	dockInfomation_->setMinimumHeight(50);
	dockInfomation_->setMaximumHeight(500);
	addDockWidget(Qt::BottomDockWidgetArea,dockInfomation_);
	dockInfomation_->setFeatures(QDockWidget::NoDockWidgetFeatures);
	dockInfomation_->setFeatures(QDockWidget::DockWidgetClosable);

	setCorner(Qt::BottomRightCorner,Qt::RightDockWidgetArea);
	setTabPosition(Qt::RightDockWidgetArea, QTabWidget::North);

	GmshWiget = new QDockWidget(this);
	GmshWiget->setAllowedAreas(Qt::AllDockWidgetAreas);
	areaInfomation_ = new QScrollArea(GmshWiget);
	GmshWiget->setWidget(areaInfomation_);
	areaInfomation_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	areaInfomation_->setWidgetResizable(true);
	addDockWidget(Qt::BottomDockWidgetArea,GmshWiget);
	GmshWiget->setFeatures(QDockWidget::NoDockWidgetFeatures);
	GmshWiget->setFeatures(QDockWidget::DockWidgetClosable);

	setCorner(Qt::BottomRightCorner,Qt::RightDockWidgetArea);
	setTabPosition(Qt::RightDockWidgetArea, QTabWidget::North);
	GmshWiget->hide();
}

void MainWindow::changeS(bool f)
{
	bool ft;
	if (f==true)
	{
		ft=true;
	}else
		ft= false;
}
//����������
void MainWindow::createToolBars()
{
    fileToolBar_=addToolBar(tr("FileBar"));
    fileToolBar_->addAction(openAct_);
    fileToolBar_->addAction(saveAct_);
    fileToolBar_->addSeparator();
    fileToolBar_->addAction(mergeAct_);
    fileToolBar_->addSeparator();
    fileToolBar_->setAllowedAreas(Qt::AllToolBarAreas);
	//fileToolBar_->setOrientation(Qt::Vertical);
   
	fileToolBar_->addAction(AddXAct_);
	fileToolBar_->addAction(AddYAct_);
	fileToolBar_->addAction(AddZAct_);
	fileToolBar_->addAction(SubXAct_);
	fileToolBar_->addAction(SubYAct_);
	fileToolBar_->addAction(SubZAct_);
    fileToolBar_->addSeparator();
	fileToolBar_->addAction(ZomeAddAct_);
	fileToolBar_->addAction(ZomeSubAct_);
    fileToolBar_->addAction(zoomFitAct_);
    //fileToolBar_->setOrientation(Qt::Horizontal);//����λ��Ϊ��ֱ

    typeToolBar_=addToolBar(tr("GmshBar"));
	//typeToolBar_->addAction(GridDivAct_);
	//typeToolBar_->addAction(GridDivActClose_);
    //typeToolBar_->addAction(GridDivActReset_);
	typeToolBar_->addAction(GridDivActOpenGMesh_);

    commonBar_=addToolBar(tr("CommonBar"));
    commonBar_->addAction(axisAct_);
    commonBar_->addAction(bgAct_);

    postStepBar_ =addToolBar(tr("StepBar"));
    m_StepPlayWidget=new QStepPlayWidget(this);
    postStepBar_->addWidget(m_StepPlayWidget);
    postStepBar_->addSeparator();

    connect(PostPro_,SIGNAL(emitStepPlayMenu(ResultOutputS)),this,SLOT(updataStepPlayCombox(ResultOutputS)));
    connect(PostPro_->m_PostWigResultOut,SIGNAL(emitScalar(ResultVisS)),this,SLOT(updataStepPlayParam(ResultVisS)));
    connect(m_StepPlayWidget,SIGNAL(emitPlayStepParam(StepPlayVisS)),this,SLOT(PlayStepPlay(StepPlayVisS)));   
}
void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
    /*statusBar()->setStyleSheet("QStatusBar { border: 2px solid white;background: rgb(229,225,221)}");
    QString color = menuBar()->styleSheet();
    statusMsgLabel_ = new QLabel(statusBar());
    statusBar()->addPermanentWidget(statusMsgLabel_);*/
}
void MainWindow::CreatePreProcessPanel()
{
    QToolBox *toolBox = new QToolBox(areaProHIPPrc_);//(areaGeometry_);
    toolBox->setMinimumWidth(100);
    //QPalette palette(toolBox->palette());
    //palette.setColor(QPalette::Background,Qt::blue);
    //toolBox->setPalette(palette);
    //areaGeometry_->setWidget(toolBox);
	areaProHIPPrc_->setWidget(toolBox);

     QWidget *w = new QWidget(toolBox);
     w->setAutoFillBackground(true);
     QTabWidget *tab = new QTabWidget(w);

     QWidget *w1=new QWidget(toolBox);
     QGroupBox *gbPoints = new QGroupBox(tr("First Area"),w1);
     gbPoints->setStyleSheet(QString::fromUtf8("::title{color:blue}"));
     QGroupBox *gbMesh = new QGroupBox(tr("Second Area"), w1);
     gbMesh->setStyleSheet(QString::fromUtf8("::title{color:blue}"));

     QVBoxLayout *layout1 = new QVBoxLayout(w1);
     layout1->addWidget(gbPoints);
     layout1->addWidget(gbMesh);
     layout1->addStretch();
     w1->setLayout(layout1);
     QWidget *w2 = new QWidget(toolBox);
     QGroupBox *gbGroup = new QGroupBox(tr("Description"), w2);
     gbGroup->setStyleSheet(QString::fromUtf8("::title{color:blue}"));

     QVBoxLayout *layout2 = new QVBoxLayout(w2);
     layout2->addWidget(gbGroup);
     layout2->addStretch();
     w2->setLayout(layout2);

     tab->addTab(w1,"Tab1");
     tab->addTab(w2,"Tab2");
     tab->setTabPosition(QTabWidget::West);

     QPushButton *buttonShowMesh = new QPushButton("Btn1",w);
     connect(buttonShowMesh,SIGNAL(clicked()),this,SLOT(ShowMesh()));
     buttonShowMesh->setFixedWidth(80);

     QPushButton *buttonWriteWGS = new QPushButton("Btn2",w);
     connect(buttonWriteWGS,SIGNAL(clicked()),this,SLOT(WriteWGSFile()));
     buttonWriteWGS->setFixedWidth(80);

     QHBoxLayout *hlayout = new QHBoxLayout();
     hlayout->addWidget(buttonShowMesh);
     hlayout->addWidget(buttonWriteWGS);

     QVBoxLayout *layout = new QVBoxLayout(w);
     layout->addWidget(tab);
     layout->addLayout(hlayout);
     layout->addStretch();
     layout->setContentsMargins(0,0,0,0);
     w->setLayout(layout);
     QTabWidget *tabWidget = new QTabWidget(this);

     QVBoxLayout *lout = new QVBoxLayout(gbPoints);
     lout->setContentsMargins(2, 2, 2, 2);
     lout->addWidget(tabWidget);
     gbPoints->setLayout(lout);
     toolBox->addItem(w,QIcon(":/images/mesh.png"),"Forward");
}
void MainWindow::LanguageZHSlot()
{
    LanguageCAct_->setChecked(true);
    LanguageUAct_->setChecked(false);
    if (m_Lin->m_CurrentLanguage==zh_cn)
        return;
    m_Lin->ChangeLanguage(zh_cn);
}
void MainWindow::LanguageENSlot()
{
    LanguageCAct_->setChecked(false);
    LanguageUAct_->setChecked(true);
    if (m_Lin->m_CurrentLanguage==en_us)
        return;    
    m_Lin->ChangeLanguage(en_us);
}
void MainWindow::retranslateUi(int iLanguage)
{
    if (iLanguage == zh_cn){
        openAct_->setText(tr("��"));
        newAct_->setText(tr("�½�"));
        saveAct_->setText(tr("����"));
		WorkingPathAct_->setText(tr("����·��"));
        mergeAct_->setText(tr("�ϲ�"));
        saveAsAct_->setText(tr("����Ϊ"));
        importAct_->setText(tr("����"));
        exportAct_->setText(tr("����"));
        m_ChangeLanguageTitle->setTitle(tr("�����л�"));
        LanguageCAct_->setText(tr("�л�����"));
        LanguageUAct_->setText(tr("�л�Ӣ��"));
        closeAct_->setText(tr("�ر�"));
        printAct_->setText(tr("��ӡ"));
        m_FilBtn->setText(tr(" �ļ�"));
        //--
        DockPostPrc_->setWindowTitle("����");
        DockPreHIPPrc_->setWindowTitle("ǰ����");
    }else{
        openAct_->setText(tr("Open"));
        newAct_->setText(tr("�½�"));
        saveAct_->setText(tr("Save"));
		WorkingPathAct_->setText(tr("WorkingPath"));
        saveAct_->setText(tr("Merge"));
        saveAsAct_->setText(tr("Save As"));
        importAct_->setText(tr("Import"));
        exportAct_->setText(tr("Export"));
        m_ChangeLanguageTitle->setTitle(tr("Change Language"));
        LanguageCAct_->setText(tr("Chinese"));
        LanguageUAct_->setText(tr("English"));
        closeAct_->setText(tr("Close"));
        printAct_->setText(tr("Print"));
        m_FilBtn->setText(tr("File"));
        //m_contextCategoryHIP->setContextTitle("HIP");
        //m_contextCategory_HIP_C->setContextTitle("����1");
        //m_contextCategoryHP->setContextTitle("HP");
        //m_contextCategory_HP_C->setContextTitle("����2");
        //m_contextCategoryHE->setContextTitle("HE");
        //m_contextCategory_HE_C->setContextTitle("����3");
        DockPostPrc_->setWindowTitle("Post");
        DockPreHIPPrc_->setWindowTitle("Pre");
    }

    UpDataRibbonLanguage(iLanguage);//����Rabbion�����л�;
    //UpDataWidgetLanguage(iLanguage);//
    //if(!PostPro_){//���������л�
    PostPro_->m_TreeModel->UpDataWidgetLanguage(iLanguage);//����DockTree�����л�;
    if (PostPro_->m_PosWigReadResultDlg){
        PostPro_->m_PosWigReadResultDlg->TabPage1->LanguageUpData();
    }
    if (PostPro_->m_PosWigFile){
        PostPro_->m_PosWigFile->LanguageUpData();
    }
    
    //}
}
/*------------------------------------------
----------------Ribbon�����л�---------------
--------------------------------------------*/
void MainWindow::UpDataRibbonLanguage(int iLanguage)
{
    if (iLanguage == zh_cn){
        m_CategoryApp->changeWindowTitle(tr("Ӧ��"));//onWindowTitleChanged(tr("Application"));
        m_CategoryViewer->changeWindowTitle(tr("����"));
        //m_CategoryDataBase->changeWindowTitle(tr("���ݿ�"));//�������ݿ� luo
        //m_CategoryLookOver->changeWindowTitle(tr("�鿴"));//���ز鿴 luo
        m_CategoryTool->changeWindowTitle(tr("����"));
        //m_CategoryOPtim->changeWindowTitle(tr("�Ż�"));//�����Ż� luo
        //m_CategoryWindow->changeWindowTitle(tr("����"));//���ش��� luo
        m_CategoryHelp->changeWindowTitle(tr("����"));
		m_contextCategory_Casting_C->changeWindowTitle(tr("����0"));
        m_contextCategory_HIP_C->changeWindowTitle(tr("����1"));
        m_contextCategory_HE_C->changeWindowTitle(tr("����2"));
        m_contextCategory_HP_C->changeWindowTitle(tr("����3"));

		m_contextCategoryCasting->setContextTitle("����");
        m_contextCategory_Casting_C->changeWindowTitle("����0");
        m_contextCategoryHIP->setContextTitle("��׽���");
        m_contextCategory_HIP_C->changeWindowTitle("����1");
        m_contextCategoryHP->setContextTitle("�ȴ���");
        m_contextCategory_HP_C->changeWindowTitle("����3");
        m_contextCategoryHE->setContextTitle("�ȼ�ѹ");
        m_contextCategory_HE_C->changeWindowTitle("����3");
		m_contextCategoryForging->setContextTitle("����");
        m_contextCategory_Forging_C->changeWindowTitle("����2");
		

        AppStartAct_->setText(tr("����"));
        AppMeshAct_->setText(tr("����"));
		AppCastingAct_->setText(tr("����"));
        AppHIPAct_->setText(tr("��׽���"));
        AppHotExtrusionAct_->setText(tr("�ȼ�ѹ"));
        AppForgingAct_->setText(tr("����"));
        AppHeatProcessAct_->setText(tr("�ȴ���"));
        AppResultAct_->setText(tr("���"));
        AppRunAct_->setText(tr("����"));

        //--
        HIPpartAct_->setText(tr("����"));
    }else{
        m_CategoryApp->changeWindowTitle(tr("Application"));
        m_CategoryViewer->changeWindowTitle(tr("Viewer"));
        //m_CategoryDataBase->changeWindowTitle(tr("Database")); //�������ݿ� luo
        m_CategoryLookOver->changeWindowTitle(tr("LookOver")); 
        m_CategoryTool->changeWindowTitle(tr("Tools"));
        //m_CategoryOPtim->changeWindowTitle(tr("Optimization"));//�����Ż� luo
        //m_CategoryWindow->changeWindowTitle(tr("Windows"));
        m_CategoryHelp->changeWindowTitle(tr("Help"));


		m_contextCategoryCasting->setContextTitle("Casting");
        m_contextCategory_Casting_C->changeWindowTitle("Set0");
        m_contextCategoryHIP->setContextTitle("HIP");
        m_contextCategory_HIP_C->changeWindowTitle("Set1");
        m_contextCategoryHP->setContextTitle("HP");
        m_contextCategory_HP_C->changeWindowTitle("Set3");
        m_contextCategoryHE->setContextTitle("HE");
        m_contextCategory_HE_C->changeWindowTitle("Set3");
		m_contextCategoryForging->setContextTitle("Forging");
        m_contextCategory_Forging_C->changeWindowTitle("Set2");
   /*   m_contextCategory_HIP_C->changeWindowTitle(tr("Set1"));
        m_contextCategory_HE_C->changeWindowTitle(tr("Set2"));
        m_contextCategory_HP_C->changeWindowTitle(tr("Set3"));
      */
        AppStartAct_->setText(tr("Start"));
        AppMeshAct_->setText(tr("Mesh"));
		AppCastingAct_->setText(tr("Casting"));
        AppHIPAct_->setText(tr("HIP"));
        //AppHotExtrusionAct_->setText(tr("HotExtrusion")); //�ȼ�ѹ������ luo
        AppForgingAct_->setText(tr("Forging"));
        AppHeatProcessAct_->setText(tr("HeatProcess"));
		

        AppResultAct_->setText(tr("Result"));
        AppRunAct_->setText(tr("Run"));
        //--
        HIPpartAct_->setText(tr("Parts"));
    }
    m_ribbon->hideContextCategory(m_contextCategoryHIP);
    m_ribbon->hideContextCategory(m_contextCategoryHE);
    m_ribbon->hideContextCategory(m_contextCategoryHP);
}

void MainWindow::AppStartDlgSlot()
{
    m_AppStartDlg=new QAppStartDlg(this);
    m_AppStartDlg->show();
    m_AppStartDlg->raise();//���ϲ�
    m_AppStartDlg->activateWindow();//����
   
}
void MainWindow::AddInNewMenuHIPSlot(bool on)
{
    //--��״ͼ��ʾ
        iSetProStyle = ENUM_PRO_HIP;
        tabifyDockWidget(DockPostPrc_, DockPreHIPPrc_);//�������
   // if(on){
        m_ribbon->hideContextCategory(m_contextCategoryCasting);
		m_ribbon->hideContextCategory(m_contextCategoryHE);
        m_ribbon->hideContextCategory(m_contextCategoryHP);
		m_ribbon->hideContextCategory(m_contextCategoryForging);
        m_ribbon->showContextCategory(m_contextCategoryHIP);
		
        //m_contextCategory_HIP_C->show();
      
   /* }else{
        m_ribbon->hideContextCategory(this->m_contextCategoryHIP);
         m_ribbon->hideContextCategory(this->m_contextCategoryHE);
         m_contextCategory_HE_C->hide();
        m_contextCategory_HIP_C->hide();

    }*/

}
void MainWindow::AddInNewMenuHESlot(bool on)
{

   // if(on){
	    m_ribbon->hideContextCategory(m_contextCategoryCasting);
        m_ribbon->hideContextCategory(m_contextCategoryHIP);
        m_ribbon->hideContextCategory(m_contextCategoryHP);
		m_ribbon->hideContextCategory(m_contextCategoryForging);
        m_ribbon->showContextCategory(m_contextCategoryHE);
		
    /*}else{
        m_ribbon->hideContextCategory(this->m_contextCategoryHE);
        m_ribbon->hideContextCategory(this->m_contextCategoryHIP);
        m_contextCategory_HE_C->hide();
        m_contextCategory_HIP_C->hide();
    }*/

}

void MainWindow::AddInNewMenuForgingSlot(bool on)
{
        iSetProStyle = ENUM_PRO_Forging;
   // if(on){
		m_ribbon->hideContextCategory(m_contextCategoryCasting);
        m_ribbon->hideContextCategory(m_contextCategoryHIP);
        m_ribbon->hideContextCategory(m_contextCategoryHP);
        m_ribbon->hideContextCategory(m_contextCategoryHE);
		m_ribbon->showContextCategory(m_contextCategoryForging);
		
		
    /*}else{
        m_ribbon->hideContextCategory(this->m_contextCategoryHE);
        m_ribbon->hideContextCategory(this->m_contextCategoryHIP);
        m_contextCategory_HE_C->hide();
        m_contextCategory_HIP_C->hide();
    }*/

}

void MainWindow::AddInNewMenuHPSlot(bool on)
{
        iSetProStyle = ENUM_PRO_HP;
   // if(on){
		m_ribbon->hideContextCategory(m_contextCategoryCasting);
        m_ribbon->hideContextCategory(m_contextCategoryHIP);
        m_ribbon->hideContextCategory(m_contextCategoryHE);
		m_ribbon->hideContextCategory(m_contextCategoryForging);
        m_ribbon->showContextCategory(m_contextCategoryHP);
		
    /*}else{
        m_ribbon->hideContextCategory(this->m_contextCategoryHE);
        m_ribbon->hideContextCategory(this->m_contextCategoryHIP);
        m_contextCategory_HE_C->hide();
        m_contextCategory_HIP_C->hide();
    }*/

}

void MainWindow::AddInNewMenuCastingSlot(bool on)
{
	 iSetProStyle = ENUM_PRO_Casting;
	 m_ribbon->hideContextCategory(m_contextCategoryHIP);
	 m_ribbon->hideContextCategory(m_contextCategoryHE);
	 m_ribbon->hideContextCategory(m_contextCategoryForging);
	 m_ribbon->hideContextCategory(m_contextCategoryHP);
	 m_ribbon->showContextCategory(m_contextCategoryCasting); 
}

void MainWindow::NewSlot()
{
   // static int indexPage=1;
    int indexPage=m_MdiArea->subWindowList().count();
    int setIdTab=indexPage;//2*indexPage;
    PostPro_->m_PosWigFile->PostFrdNew(setIdTab);
    viewWindow_ = new QWindowView(m_MdiArea, setIdTab);//indexPage);
    QString str = tr("3d view %1").arg(indexPage++);
    viewWindow_->setWindowTitle(str);    
    m_MdiArea->addSubWindow(viewWindow_);
    viewWindow_->showMaximized();
    
 
    //m_MdiArea->currentSubWindow()->setContextMenuPolicy(Qt::NoContextMenu);
    //m_MdiArea->setContextMenuPolicy(Qt::NoContextMenu);
    //m_MdiArea->currentSubWindow()->setAttribute(Qt::WA_DeleteOnClose);
    //m_MdiArea->setAttribute(Qt::WA_DeleteOnClose);
    
}
//--����ģ�ͺϲ�
void MainWindow::MergeSlot()
{
	statusBar()->showMessage(tr("�ϲ�..."));
	QString workPath=QDir::currentPath();
	//QString workPath=workPath2;
    //workPath = workPath.left(workPath.lastIndexOf("/"));
	QString filename = QFileDialog::getOpenFileName(this, "Read file...",workPath, "Inp Files (*.inp )");
    if (filename.size()==0){
       return;
    }
	if (!filename.toLower().endsWith(".inp")) {
		Information_Widget::GetInstance()->ShowInformation(QString("Only .inp files are supported for merging: %1").arg(filename));
		return;
	}
	int NodeNumber=CRWObject.m_ReadInpResult.TmpNodeInpS.strData.size();
	int ElementNumber=CRWObject.m_ReadInpResult.TmpElInpS.strData.size();
	if (NodeNumber == 0 && ElementNumber == 0) {
		Information_Widget::GetInstance()->ShowInformation(QString("No INP data loaded. Open an INP file first before merging."));
		return;
	}
	QFile file(filename);
    if(!file.exists()){
		Information_Widget::GetInstance()->ShowInformation(QString("File does not exist: %1").arg(filename));
        return ;
    }
    if (!file.open(QIODevice::ReadOnly)){
		Information_Widget::GetInstance()->ShowInformation(QString("Cannot open file: %1").arg(filename));
        return ;
    }

	CRWManage CRWObject02;
	int ret=CRWObject02.ReadSectionInpFile02(&file,NodeNumber,ElementNumber,filename);
    file.close();
    if (!ret){
		Information_Widget::GetInstance()->ShowInformation(QString("Failed to read INP file for merging: %1").arg(filename));
		return;
	}
	//�ڵ���Ϣ
	CRWObject.m_ReadInpResult.TmpNodeInpS.strData.append(CRWObject02.m_ReadInpResult.TmpNodeInpS.strData);
	//��Ԫ��Ϣ
	CRWObject.m_ReadInpResult.TmpElInpS.strData.append(CRWObject02.m_ReadInpResult.TmpElInpS.strData);
	CRWObject.m_ReadInpResult.TmpElInpS.ElementType.append(CRWObject02.m_ReadInpResult.TmpElInpS.ElementType);
    CRWObject.m_ReadInpResult.TmpElInpS.NumberE.append(CRWObject02.m_ReadInpResult.TmpElInpS.NumberE);
	//�ڵ㼯��
	CRWObject.m_ReadInpResult.TmpNsetInps.strNSetName.append(CRWObject02.m_ReadInpResult.TmpNsetInps.strNSetName);
	CRWObject.m_ReadInpResult.TmpNsetInps.strNSetStyle.append(CRWObject02.m_ReadInpResult.TmpNsetInps.strNSetStyle);
	CRWObject.m_ReadInpResult.TmpNsetInps.strData.append(CRWObject02.m_ReadInpResult.TmpNsetInps.strData);
	//��Ԫ����
	CRWObject.m_ReadInpResult.TmpElSetInps.strData.append(CRWObject02.m_ReadInpResult.TmpElSetInps.strData);
	CRWObject.m_ReadInpResult.TmpElSetInps.strElSetStyle.append(CRWObject02.m_ReadInpResult.TmpElSetInps.strElSetStyle);
	CRWObject.m_ReadInpResult.TmpElSetInps.strElSetName.append(CRWObject02.m_ReadInpResult.TmpElSetInps.strElSetName);
	//���漯��
	CRWObject.m_ReadInpResult.TmpSurfaceInps.strSurfaceName.append(CRWObject02.m_ReadInpResult.TmpSurfaceInps.strSurfaceName);
	CRWObject.m_ReadInpResult.TmpSurfaceInps.strSurfaceDataNum.append(CRWObject02.m_ReadInpResult.TmpSurfaceInps.strSurfaceDataNum);
	CRWObject.m_ReadInpResult.TmpSurfaceInps.strData.append(CRWObject02.m_ReadInpResult.TmpSurfaceInps.strData);
	//�ͷŶ����inp�ļ�����
	CRWObject02.m_ReadInpResult.TmpNodeInpS.clear();
	CRWObject02.m_ReadInpResult.TmpElInpS.clear();
	CRWObject02.m_ReadInpResult.TmpNsetInps.clear();
	CRWObject02.m_ReadInpResult.TmpElSetInps.clear();
	CRWObject02.m_ReadInpResult.TmpSurfaceInps.clear();
    //д�����ݵ�����������
    ReadInpResultS m_Data = CRWObject.m_ReadInpResult;
	m_QStatisticsDlg->SetInpData(m_Data);
    //--д�뵽HIP��������
    m_HIPSystemDlg->SetInpData(m_Data);
    m_VariableOutputDlg->SetInpData(m_Data);
    //--д�뵽HP��������
    m_HpPartDlg->SetInpData(m_Data);
    m_HpBCDlg->SetInpData(m_Data);
    m_ForgingContactDlg->SetInpData(m_Data);
	m_ThermalBoundaryDlg->SetInpDataHB(m_Data);
	m_ForgingSystemDlg->SetInpData(m_Data);
    m_HpInitDlg->SetInpData(m_Data);
    m_HpSystemDlg->SetInpData(m_Data);
    m_HpSolveSetDlg->SetInpData(m_Data);

	//--write to QInpActorWidge
	m_WidgetInpElsetDlg->SetInpData(m_Data.TmpElSetInps);//
   	//д�뵽װ�����
	m_AssemblingAct_->SetInpData(m_Data);
    //--д�뵽Tree
    PreHIPPro_->m_TreeModel->setInpData(m_Data);
    //--д�뵽view��ʾ����
    viewWindow_->TabView(1);//->tabView_setCurrentIndex(1);
    QMdiSubWindow *CurWindow=m_MdiArea->currentSubWindow();
    for (int i=0;i<m_MdiArea->subWindowList().size();i++){
        QMdiSubWindow *TmpWindow=m_MdiArea->subWindowList().at(i);
        if (TmpWindow == TmpWindow){
            viewWindow_->ShowCurPreData(m_Data); 
            break;
		}
	}
   Information_Widget::GetInstance()->ShowInformation(QString(filename+" Open Succeed!"));
}
void MainWindow::OpenSlot()
{
    // QLabel *qlabel = new QLabel("Hello Qt");
    //qlabel->show();
    statusBar()->showMessage(tr("��..."));
	QString workPath=QDir::currentPath();
	//QString workPath=workPath2;
    //workPath = workPath.left(workPath.lastIndexOf("/"));
	QString filename = QFileDialog::getOpenFileName(this, "Read file...", workPath, "Inp Files (*.inp );;Frd Files (*.Frd *.frd);;Dat Files (*.Dat *.dat)");   
    if (filename.size()==0){
       return;
    }
   
    QFile file(filename);
    if(!file.exists()){
        return ;
    }
    if (!file.open(QIODevice::ReadOnly)){
        Information_Widget::GetInstance()->ShowInformation(QString("Cannot open file: %1").arg(filename));
        return ;
    }
    QFileInfo fileInfo(filename);
    QString fileFormat = fileInfo.suffix().toLower();//frd�ļ�
    if (fileFormat=="inp") {
       // CRWManage CRWObject;
        int ret=CRWObject.ReadSectionInpFile(&file,filename);
        file.close();
        if (!ret){
            Information_Widget::GetInstance()->ShowInformation(QString("Failed to read INP file: %1").arg(filename));
            return;
        }
		ReadInpResultS m_Data = CRWObject.m_ReadInpResult;
        //д�����ݵ�����������
        //--д�뵽HIP��������
        //m_PartDlg->SetInpData(m_Data);
        //m_BCDlg->SetInpData(m_Data);
        //m_InitDlg->SetInpData(m_Data);
        m_HIPSystemDlg->SetInpData(m_Data);
        //m_ResolveDlg->SetInpData(m_Data);
        m_VariableOutputDlg->SetInpData(m_Data);
		m_QStatisticsDlg->SetInpData(m_Data);
        //--д�뵽HP��������
        m_HpPartDlg->SetInpData(m_Data);
        m_HpBCDlg->SetInpData(m_Data);
		m_ForgingContactDlg->SetInpData(m_Data);
		m_ThermalBoundaryDlg->SetInpDataHB(m_Data);
		m_ForgingSystemDlg->SetInpData(m_Data);
        m_HpInitDlg->SetInpData(m_Data);
		m_GravityAct_->SetInpData(m_Data);
        m_HpSystemDlg->SetInpData(m_Data);
        m_HpSolveSetDlg->SetInpData(m_Data);
		//д�뵽װ�����
		m_AssemblingAct_->SetInpData(m_Data);
		//����->������ʾ���ؽ���
		m_WidgetInpElsetDlg->SetInpData(m_Data.TmpElSetInps);
        //--д�뵽Tree
        PreHIPPro_->m_TreeModel->setInpData(m_Data);
		 //--д�뵽view��ʾ����
		
		if (viewWindow_){
			viewWindow_->TabView(1);//tabView_->setCurrentIndex(1);
			QMdiSubWindow *CurWindow=m_MdiArea->currentSubWindow();
			for (int i=0;i<m_MdiArea->subWindowList().size();i++){
				QMdiSubWindow *TmpWindow=m_MdiArea->subWindowList().at(i);
				if (TmpWindow == TmpWindow){
					viewWindow_->ShowCurPreData(m_Data); 
					break;
				}
			}
		}
       Information_Widget::GetInstance()->ShowInformation(QString(filename+" Open Succeed!"));
    }else if(fileFormat=="frd"){// 
        if (viewWindow_){
            viewWindow_->TabView(1);//tabView_->setCurrentIndex(0);
        }
        if(PostPro_->m_PosWigFile->readOpenFrd(filename)){
            Information_Widget::GetInstance()->ShowInformation(QString(filename+" Open Succeed!"));
        }else{
            Information_Widget::GetInstance()->ShowInformation(QString(filename+" Open Fail!"));
        }
       
        //QFrdDataPro	m_FrdDataPro;
        //resultFrdS m_resultFrd;
        //bool ret=m_FrdDataPro.ReadFileData(frdVIS_, fileName, m_resultFrd);
    }else if(fileFormat=="dat"){
        Information_Widget::GetInstance()->ShowInformation(QString("DAT file reading is not supported in this view. Use Post-Process > Open Frd/Dat instead: %1").arg(filename));
    } else if (!fileFormat.isEmpty()) {
        Information_Widget::GetInstance()->ShowInformation(QString("Unsupported file format: .%1. Please use inp, frd, or dat files.").arg(fileFormat));
    }
    statusBar()->showMessage(tr("Ready"));
    //occGeomImExport::FileFormat format;  
}
void MainWindow::ViewAss( ReadInpResultS m_Data)
{
	PreHIPPro_->m_TreeModel->setInpData(m_Data);
    if (viewWindow_){
        viewWindow_->TabView(1);//tabView_->setCurrentIndex(1);
        QMdiSubWindow *CurWindow=m_MdiArea->currentSubWindow();
        for (int i=0;i<m_MdiArea->subWindowList().size();i++){
             QMdiSubWindow *TmpWindow=m_MdiArea->subWindowList().at(i);
              if (TmpWindow == TmpWindow){
                  viewWindow_->ShowCurPreData(m_Data); 
                  break;
			  }
		}
	}
}
//--hp saveInp
void MainWindow:: MainSaveHPInpSlot()
{
    SaveSlot();
}
//--HIP SAVE
void MainWindow::MainSaveHIPInpSlot()
{
    SaveSlot();
}
//--Forging SAVE
void MainWindow::MainSaveForgingInpSlot()
{
    SaveSlot();
}
void MainWindow::InpCheckSlot()
{
	QString CheStr,CheStr1,CheStr2,CheStr3;
	int   Nsize,Nsize1,Nsize2,Nsize3;
	int   Nsize4=0;

	CheStr=m_HpPartDlg->m_NodeElSetData.NodeInpData.strNodeTitle;
	Nsize=m_HpPartDlg->m_NodeElSetData.NodeInpData.strData.size();
	if(CheStr=="*Node"&&Nsize<=0)"����:ȱ�ٽڵ���Ϣ";
	Nsize=m_HpPartDlg->m_NodeElSetData.ELInpData.strELType.size();
	Nsize1=m_HpPartDlg->m_NodeElSetData.ELInpData.strData.size();
	Nsize2=m_HpPartDlg->m_NodeElSetData.ELInpData.NumberE.size();
	Nsize3=m_HpPartDlg->m_NodeElSetData.ELInpData.ElementType.size();
	for(int i=0;i<Nsize2;i++){
		Nsize4+=m_HpPartDlg->m_NodeElSetData.ELInpData.NumberE.at(i);
	}
	if(Nsize>0&&Nsize1<=0||Nsize2!=Nsize3||Nsize1!=Nsize4)"����:ȱ�ٵ�Ԫ��Ϣ";
	Nsize=m_HpBCDlg->m_nodeBCList.size();
	for(int i=0;i<Nsize;i++){
         CheStr=m_HpBCDlg->m_nodeBCList.at(i).strBoundaryName;
         Nsize1=m_HpBCDlg->m_nodeBCList.at(i).strPSetName.size();
		 Nsize2=m_HpBCDlg->m_nodeBCList.at(i).strURstyle.size();
		 if(CheStr=="Boundary"&&Nsize1<=0||CheStr=="Boundary"&&Nsize2<=0)"����:ȱ�ٲ��ֱ߽��������п��ܻ���ɼ����޷�����";
	}
	Nsize=m_HpSolveSetDlg->m_nodeVarList.size();
	for(int i=0;i<Nsize;i++){
		CheStr=m_HpSolveSetDlg->m_nodeVarList.at(i).strELInclude;
		CheStr1=m_HpSolveSetDlg->m_nodeVarList.at(i).strNInclude;
		CheStr2=m_HpSolveSetDlg->m_nodeVarList.at(i).strTimeOrFreqName;
		CheStr3=m_HpSolveSetDlg->m_nodeVarList.at(i).strTname;
		if(CheStr!=""&&CheStr1!="")"����:ȱ�ٽ�������";
		if(CheStr2=="TIME POINTS"&&CheStr3=="")"����:�����������ʱ�������ʱ��ȱ�ٱ�Ҫ��ʱ������Ϣ";
	}
	Nsize=m_HpSystemDlg->m_nodeHPSystemRList.size();
	Nsize1=m_HpSystemDlg->m_nodeHPSystemFList.size();
	Nsize2=m_HpSolveSetDlg->m_nodeHPPhyList.size();
	CheStr=m_HpSolveSetDlg->m_nodeHPPhyList.at(0).strStyle;
	CheStr1=m_HpSolveSetDlg->m_nodeHPPhyList.at(0).strAbsZero;
	CheStr2=m_HpSolveSetDlg->m_nodeHPPhyList.at(0).strStefanBoltzman;
	if(Nsize>0||Nsize1>0&&Nsize2<=0||CheStr=="")"����:����ģ���а�������/����߽���Ϣ��������������δָ��";
	if(Nsize>0||Nsize1>0&&CheStr1=="")"����:����ģ���а�������/����߽���Ϣ������δָ���������";
	if(Nsize>0||Nsize1>0&&CheStr2=="")"����:����ģ���а�������/����߽���Ϣ������δָ��������������";
	for(int i=0;i<Nsize;i++){
		CheStr=m_HpSystemDlg->m_nodeHPSystemRList.at(i).strElsetName;
		CheStr1=m_HpSystemDlg->m_nodeHPSystemRList.at(i).strFilmRadiateAmpName;
		CheStr2=m_HpSystemDlg->m_nodeHPSystemRList.at(i).strAmplitudeName;
		if(CheStr1!=""){
		}
		if(CheStr2!=""){
		}
	}
	for(int i=0;i<Nsize1;i++){
		CheStr=m_HpSystemDlg->m_nodeHPSystemFList.at(i).strElsetName;
		CheStr1=m_HpSystemDlg->m_nodeHPSystemFList.at(i).strFilmRadiateAmpName;
		CheStr2=m_HpSystemDlg->m_nodeHPSystemFList.at(i).strAmplitudeName;
	}
	m_HpPartDlg->m_nodeSList;
	m_HpPartDlg->m_nodeMList;
	m_HpPartDlg->m_nodeSList;
	m_HpPartDlg->m_HpTTTDataInps;
	m_HIPSystemDlg->m_nodeCurveList;
	m_ForgingSystemDlg->m_CurveList;
	m_ForgingContactDlg->m_OutPutContInfS;
	m_ThermalBoundaryDlg->m_CurveList;
	m_HpInitDlg->m_nodeInitList;
	m_HpSolveSetDlg->m_nodeVarList;
	m_HpSolveSetDlg->m_nodeHPSolveList;
	m_HIPSystemDlg->m_nodeHIPSystemList;
	m_ForgingSystemDlg->m_OutPutFSystemInfS;	
    m_HpBCDlg->m_nodeBCList;
	m_ThermalBoundaryDlg->m_OutPutTBFList;
	m_ThermalBoundaryDlg->m_OutPutTBRList;
}
void MainWindow::SaveSlot()
{
    //QMessageBox::critical(this,QString("����"),QString("����..."));
	QString workPath=QDir::currentPath();
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Choose a file name"),workPath,tr("inp Files (*.inp)"));
    if (fileName.isEmpty())return;
	if(!fileName.contains(".inp"))fileName+=".inp";
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Dock Widgets"),
            tr("Cannot write file %1:\n%2.")
            .arg(fileName)
            .arg(file.errorString()));
        return;
    }
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::restoreOverrideCursor();

    statusBar()->showMessage(tr("Saved '%1'").arg(fileName), 2000);


    CRWManage CRWObject;
    //-s1
	//CRWObject.WritePartFile(&file,m_HpPartDlg->m_NodeElSetData,m_HpPartDlg->m_nodeSList);//ok
	CRWObject.WriteUnitElInpFile(&file,m_HpPartDlg->m_NodeElSetData,m_HpPartDlg->m_nodeSList);//ok
    //-s2
    CRWObject.WriteHIPCurveInpFile(&file,m_HIPSystemDlg->m_nodeCurveList);//ok
	CRWObject.WriteHIPCurveInpFile(&file,m_HpSystemDlg->m_nodeCurveList);//ok
	CRWObject.WriteHIPCurveInpFile(&file,m_ForgingSystemDlg->m_CurveList);//ok
	CRWObject.WriteHIPCurveInpFile(&file,m_ThermalBoundaryDlg->m_CurveList);//ok
    //-s3
    CRWObject.WriteSectionInpFile(&file,m_HpPartDlg->m_nodeSList);
    CRWObject.WriteMaterialInpFile(&file,m_HpPartDlg->m_nodeMList,m_HpPartDlg->m_nodeSList,m_HpSolveSetDlg->m_nodeHPPhyList);//ok
    //-s4
    //CRWObject.WriteMaterialInpFile(&file,m_PartDlg->m_nodeMList);
    //CRWObject.WriteMaterialInpFile(&file,m_HpPartDlg->m_nodeMList);
    CRWObject.WriteHPTTTCCT(&file,m_HpPartDlg->m_HpTTTDataInps);//ok
    //-s5
	CRWObject.WriteContactProperties(&file,m_ForgingContactDlg->m_OutPutContInfS,m_ForgingContactDlg->m_nodeCurveList);//ok
    //-s6
    //CRWObject.WriteBCInpFile(&file,m_BCDlg->m_nodeBCList);
    CRWObject.WriteBCInpFile(&file,m_HpBCDlg->m_nodeBCList);//ok
    //-s7
    //CRWObject.WriteInitInpFile(&file,m_InitDlg->m_nodeInitList);
	CRWObject.WriteInitInpFile(&file,m_HpInitDlg->m_nodeInitList);//ok
	//s8
    CRWObject.WriteUniformLoadInpFile(&file,m_GravityAct_->m_OutputGravity_InfS,m_HpPartDlg->m_nodeSList);////��������
    //--s9
 /*CRWObject.WriteResultOIntervalInpFile(&file,m_VariableOutputDlg->m_nodeVarList,m_ResolveDlg->m_nodeHIPSolveList);
    CRWObject.WriteHIPSolveInpFile(&file,m_ResolveDlg->m_nodeHIPSolveList);
    CRWObject.WriteHIPSystemInpFile(&file,m_HIPSystemDlg->m_nodeHIPSystemList); 
    CRWObject.WriteVariableInpFile(&file,m_VariableOutputDlg->m_nodeVarList);*/
    CRWObject.WriteHPSolveVarInpFile(&file,m_HpSolveSetDlg->m_nodeVarList,
		m_HpSolveSetDlg->m_nodeHPSolveList,m_HpSolveSetDlg->m_nodeHPPhyList,
		m_HpSystemDlg->m_nodeHPSystemRList,m_HpSystemDlg->m_nodeHPSystemFList,
		m_HIPSystemDlg->m_nodeHIPSystemList,m_ForgingSystemDlg->m_OutPutFSystemInfS,
		m_HpBCDlg->m_nodeBCList,m_ThermalBoundaryDlg->m_OutPutTBFList,m_ThermalBoundaryDlg->m_OutPutTBRList);
    //-
    file.close();
    Information_Widget::GetInstance()->ShowInformation(QString(fileName+" Save Succeed!"));
}

//*-���벿���ļ�
void MainWindow::ImportPartSlot()
{
    statusBar()->showMessage(tr("���벿��..."));
    QString filename = QFileDialog::getOpenFileName(this, "Import geometry from file", "", "IGES Files (*.iges *.igs);;STEP Files (*.step *.stp);;BREP Files(*.brep);;STL Files (*.stl);;WGS Files (*.wgs)");
    if (filename.size()==0)
    {
        return;
    }
    QFile file(filename);
    if(!file.exists())
    {
        return ;
    }
    QString strName,strPartName,strfileFormat;
    QFileInfo fi(filename);
    strName = fi.fileName();
    strfileFormat = fi.suffix();
    strPartName = fi.completeBaseName();
    if (strPartName.isEmpty()) {
        strPartName = strName;
    }
    if(PreHIPPro_)
    {
        PreHIPPro_->InsertPart(strPartName);
    }

}
//*-���������ļ�
void MainWindow::ExportPartSlot()
{

}
void MainWindow::ClearSlot()
{}

void MainWindow::closeEvent(QCloseEvent *ev)
{
    QMessageBox::StandardButton bt = QMessageBox::question(this, tr("�ر�"), tr("�����˳���"), QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);
    if (bt == QMessageBox::Yes) 
    {
        if (GmshProcess!=NULL)
        {
            GmshProcess->close();
            delete GmshProcess;
            GmshProcess=NULL;
        }
        if (m_SloverProcess != NULL)
        {
            m_SloverProcess->close();
            delete m_SloverProcess;
            m_SloverProcess=NULL;
        }
        
  
        if (m_HIPSystemDlg!=NULL)
        {
            delete m_HIPSystemDlg;
            m_HIPSystemDlg = NULL;
        }
      
        if (m_VariableOutputDlg!=NULL)
        {
            delete m_VariableOutputDlg;
            m_VariableOutputDlg = NULL;
        }  
        
        ev->accept();
    }else{
        ev->ignore();
    }
}
void MainWindow::mouseReleaseEvent( QMouseEvent * ev )
{
	if(ev->button()==Qt::LeftButton)
	{
;
	}
}
//--���Ĺ���·��
void MainWindow::ChangeWorkPath()
{  
	QString workPath=QFileDialog::getExistingDirectory(this,tr("����·��"),"");
	if (workPath.size()==0)
	{
		return;
	}
	
	if (QDir(workPath).exists())
	{
		QDir::setCurrent(workPath);
		Information_Widget::GetInstance()->ShowInformation("�����ù���·��Ϊ��");
		Information_Widget::GetInstance()->ShowInformation(workPath);

		QString str = QCoreApplication::applicationDirPath();
		QDir dir(str);
		bool res=dir.mkdir("path");
		str+="\\path\\WorkPatch.txt";

		QFile f(str);
		if(!f.open(QIODevice::WriteOnly))return ;
		QTextStream txtOutput(&f);
		txtOutput<<workPath;
		f.close();
	}
	
}
//--
void MainWindow::treeViewClick( const QModelIndex& index )
{
    QString strParent = index.parent().data().toString();
    QString strNode = index.data().toString();
}
//--Gmsh
void MainWindow::GridDivActPrc()
{
	dockInfomation_->hide();
	DockPostPrc_->hide();
	//DockFrowardPrc_->hide();
    DockPreHIPPrc_->hide();
   if (m_MdiArea->subWindowList().count()>0)
    {
        m_MdiArea->hide();
        viewWindow_->hide();
    }

	GmshWiget->show();
	
    if (GmshProcess != NULL)
    {
        return;
    }
    GmshRcW=GmshWiget->width();
    GmshRcH=GmshWiget->height();
#ifdef _MSC_VER
	RECT rc;
	QString &str = QCoreApplication::applicationDirPath(); 
	//QString &cmd = QString("%1//gmsh-4.3.0-Windows64//gmsh.exe").arg(str);
    QString &cmd = QString("%1//gmsh.exe").arg(str);
    GmshProcess= new QProcess;
    GmshProcess->start(cmd,QStringList()<<"C:\\Users\\adi\\AppData\\Roaming/untitled.geo");
 //   Sleep(300);
	//QString exeName="FLTK";
	//QByteArray ba= exeName.toAscii();
	//const char *c_str=ba.data();
	//HWND childHwnd=FindWindowA(c_str,NULL);
	//HWND ParentHwnd = (HWND)GmshWiget->winId();
	//LONG TEST=GetWindowLong(childHwnd,GWL_STYLE);//GWL_STYLE);//GCW_ATOM//&~WS_EX_ACCEPTFILES&~WS_EX_MDICHILD;&~WS_CAPTION;
	//SetWindowLong(childHwnd,(GWL_STYLE),(TEST&~WS_CAPTION&~WS_THICKFRAME));//~WS_CAPTION));
	//GetClientRect(ParentHwnd,&rc);
	//SetParent(childHwnd, ParentHwnd);
	//SetWindowPos(childHwnd,HWND_TOP,0,0,GmshRcW,GmshRcH,SWP_FRAMECHANGED|SWP_SHOWWINDOW);
	//ShowWindow(childHwnd,SW_SHOW);
#else
    QString str = QCoreApplication::applicationDirPath();
    QString cmd = str + "/gmsh";
    if (!QFileInfo(cmd).exists()) {
        Information_Widget::GetInstance()->ShowInformation("gmsh binary not found: " + cmd);
        return;
    }
    GmshProcess = new QProcess(this);
    GmshProcess->start(cmd, QStringList() << "t1.geo");
    if (!GmshProcess->waitForStarted()) {
        Information_Widget::GetInstance()->ShowInformation("Failed to start gmsh");
        delete GmshProcess;
        GmshProcess = NULL;
        return;
    }
    connect(GmshProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(GmshProcessErrorSlot()));
#endif
   
}
void MainWindow::GridDivActPrcClose()
{
	GmshWiget->hide();
	dockInfomation_->show();
	DockPostPrc_->show();
    DockPreHIPPrc_->show();
	//DockFrowardPrc_->show();
	//viewWindow_->show();
    if (m_MdiArea->subWindowList().count()>0)
    {
        m_MdiArea->show();
        viewWindow_->show();
    }
    
}
void MainWindow::GridDivActPrcReset()
{
    dockInfomation_->hide();
    DockPostPrc_->hide();
    //DockFrowardPrc_->hide();
    //
    DockPreHIPPrc_->hide();
    if (m_MdiArea->subWindowList().count()>0)
    {
        m_MdiArea->hide();
        viewWindow_->hide();
    }
    GmshWiget->show();

    if (GmshProcess != NULL)
    {
        GmshProcess->close();
        delete GmshProcess;
        GmshProcess=NULL;
    }
    GmshRcW=GmshWiget->width();
    GmshRcH=GmshWiget->height();
#ifdef _MSC_VER
    RECT rc;
    QString str = QCoreApplication::applicationDirPath(); 
    //QString &cmd = QString("%1//gmsh-4.3.0-Windows64//gmsh.exe").arg(str);
    QString cmd = str+"/gmsh.exe";//QString("%1//gmsh.exe").arg(str);
    GmshProcess= new QProcess;
    GmshProcess->start(cmd,QStringList()<<"C:\\Users\\adi\\AppData\\Roaming/untitled.geo");
    Sleep(300);
    QString exeName="FLTK";
    QByteArray ba= exeName.toAscii();
    const char *c_str=ba.data();
    HWND childHwnd=FindWindowA(c_str,NULL);
    HWND ParentHwnd = (HWND)GmshWiget->winId();
    LONG TEST=GetWindowLong(childHwnd,GWL_STYLE);//GWL_STYLE);//GCW_ATOM//&~WS_EX_ACCEPTFILES&~WS_EX_MDICHILD;&~WS_CAPTION;
    SetWindowLong(childHwnd,(GWL_STYLE),(TEST&~WS_CAPTION&~WS_THICKFRAME));//~WS_CAPTION));
    GetClientRect(ParentHwnd,&rc);
    SetParent(childHwnd, ParentHwnd);
    SetWindowPos(childHwnd,HWND_TOP,0,0,GmshRcW,GmshRcH,SWP_FRAMECHANGED|SWP_SHOWWINDOW);
    ShowWindow(childHwnd,SW_SHOW);
#else
    QString str = QCoreApplication::applicationDirPath(); 
    //QString &cmd = QString("%1//gmsh-4.3.0-Windows64//gmsh.exe").arg(str);
    QString cmd = str+"/gmsh";//QString("%1//gmsh.exe").arg(str);
    GmshProcess= new QProcess(this);
    GmshProcess->start(cmd,QStringList()<<"t1.geo");
#endif 
}

void MainWindow::GridDivActPrcOpen()
{
    QString workpath=QDir::currentPath();
    QString str= QCoreApplication::applicationDirPath();
    workpath=str.left(str.lastIndexOf("/"));
#ifdef _MSC_VER 
   // str+="../gmsh-4.8.4-Windows64/gmsh.exe";
    workpath+="/gmsh-4.8.4-Windows64/gmsh.exe";
#else
    workpath+="/gmsh/gmsh.sh";
    if (!QFileInfo(workpath).exists()) {
        Information_Widget::GetInstance()->ShowInformation("gmsh.sh not found: " + workpath);
        return;
    }
    QProcess *RMshProcess = new QProcess(this);
    RMshProcess->start(workpath);
    if (!RMshProcess->waitForStarted()) {
        Information_Widget::GetInstance()->ShowInformation("Failed to start gmsh.sh");
        delete RMshProcess;
    }
#endif
}

void MainWindow::ViewDirectionChangedSlot(QString direction)
{
    // int indexPage=m_MdiArea->subWindowList().count()-1;
     QMdiSubWindow *tmp =m_MdiArea->currentSubWindow();
    if (std::find(m_MdiArea->subWindowList().begin(), m_MdiArea->subWindowList().end(), tmp) ==
        m_MdiArea->subWindowList().end()){  
         int indexPage=m_MdiArea->subWindowList().count()-1;
    }else{//�ҵ�
         int indexPage=m_MdiArea->subWindowList().count();
         int tt = viewWindow_->tabView_->currentIndex();
         QMyVTK::GetInstance(tt)->ChangeViewDirection(direction);
    }
}
void MainWindow::ViewSizeChangedSlot(int factor)
{
    //int indexPage=m_MdiArea->subWindowList().count();
    QMdiSubWindow *tmp =m_MdiArea->currentSubWindow();
    if (factor > 0)  //zoom in
    {
        if (std::find(m_MdiArea->subWindowList().begin(), m_MdiArea->subWindowList().end(), tmp) ==
            m_MdiArea->subWindowList().end()){  
                int indexPage=m_MdiArea->subWindowList().count()-1;
        }else{//�ҵ�
            int indexPage=m_MdiArea->subWindowList().count();
            int tt = viewWindow_->tabView_->currentIndex();
            QMyVTK::GetInstance(tt)->Zoom(1.1);
        }
        //QMyVTK::GetInstance(indexPage)->Zoom(1.1);
    }
    else if (factor < 0)  //zoom out
    {
        if (std::find(m_MdiArea->subWindowList().begin(), m_MdiArea->subWindowList().end(), tmp) ==
            m_MdiArea->subWindowList().end()){  
                int indexPage=m_MdiArea->subWindowList().count()-1;
        }else{//�ҵ�
            int indexPage=m_MdiArea->subWindowList().count();
            int tt = viewWindow_->tabView_->currentIndex();
            QMyVTK::GetInstance(tt)->Zoom(0.9);
        }
       // QMyVTK::GetInstance(indexPage)->Zoom(0.9);
    }
    else  //reset
    {
        if (std::find(m_MdiArea->subWindowList().begin(), m_MdiArea->subWindowList().end(), tmp) ==
            m_MdiArea->subWindowList().end()){  
                int indexPage=m_MdiArea->subWindowList().count()-1;
        }else{//�ҵ�
            int indexPage=m_MdiArea->subWindowList().count();
            int tt = viewWindow_->tabView_->currentIndex();
            QMyVTK::GetInstance(tt)->Reset();
        }
       // QMyVTK::GetInstance(indexPage)->Reset();
    }
}
void MainWindow:: ViewChangedSlot(int factor)
{
    QMdiSubWindow *tmp =m_MdiArea->currentSubWindow();
    if (std::find(m_MdiArea->subWindowList().begin(), m_MdiArea->subWindowList().end(), tmp) ==
        m_MdiArea->subWindowList().end()){  
            int indexPage=m_MdiArea->subWindowList().count()-1;
    }else{//�ҵ�
        int indexPage=m_MdiArea->subWindowList().count();
        int tt = viewWindow_->tabView_->currentIndex();
        QMyVTK::GetInstance(tt)->ViewChange(factor);
    }
}
void MainWindow::HideShowSlot(bool on)
{
    m_ribbon->setHideMode(on);
}
//����
void MainWindow::CascadeSlot()
{
    m_MdiArea->cascadeSubWindows();//������ʽ
}
//ƽ��
void MainWindow::TileSlot()
{
    m_MdiArea->tileSubWindows();//ƽ�̷�ʽ
}
//�ر������Ӵ���
void MainWindow::CloseAllSubWindowSlot()
{
    m_MdiArea->closeAllSubWindows();//
}
//�رյ�ǰ
void MainWindow::CloseSubWindowSlot()
{
    int indexPage=m_MdiArea->subWindowList().count();
    if (indexPage>1)
    {
        m_MdiArea->closeActiveSubWindow();//
    }
}
//ѡ��
void MainWindow::ChooseSubWindowSlot()
{
    m_MdiArea->closeAllSubWindows();//  
}
void MainWindow::NowActivateWindowSlot(QMdiSubWindow* SubWind)
{
    QMdiSubWindow* TestWind = SubWind;
    for (int i=0; i<m_MdiArea->subWindowList().size();i++){
        if (SubWind = m_MdiArea->subWindowList().at(i)){
            viewWindow_->m_CurrentIndex=i;
            break;
        }
    }
}
//��������
void MainWindow::CreateSetDlgSlot()
{
	if (viewWindow_){//pre 
		viewWindow_->TabView(1);
	}
    m_PElSfSetDlg->show();
    m_PElSfSetDlg->raise();
    m_PElSfSetDlg->activateWindow();
}
//--HPDLG
void MainWindow::HP_HIPPartActSlot()
{
  if (viewWindow_){//pre 
		viewWindow_->TabView(1);
	}
   if(iSetProStyle==ENUM_PRO_Casting){
		CastingpartActSlot();
	}else if (iSetProStyle==ENUM_PRO_HIP){
		HIPpartActSlot();
	}else if (iSetProStyle==ENUM_PRO_Forging){
		ForgingpartActSlot();    
	}else if (iSetProStyle==ENUM_PRO_HP){
		HPpartActSlot();
	} 
}
void MainWindow::HPpartActSlot()
{
	if (viewWindow_){//pre 
		viewWindow_->TabView(1);
	}
    m_HpPartDlg->ShowPartDlgStyle(ENUM_PRO_HP);
    m_HpPartDlg->show();
    m_HpPartDlg->raise();////���ϲ�
    m_HpPartDlg->activateWindow();//����
}
//--HPDLG
void MainWindow::HPBCActSlot()
{
	if (viewWindow_){//pre 
		viewWindow_->TabView(1);
	}
	if (iSetProStyle == ENUM_PRO_Casting){
		m_HpBCDlg->ShowBC(ENUM_PRO_Casting);
        m_CastingBcBtn->setEnabled(true);
	}else if (iSetProStyle == ENUM_PRO_HIP){
		m_HpBCDlg->ShowBC(ENUM_PRO_HIP);
        m_HIPBcBtn->setEnabled(true);
     }else if (iSetProStyle==ENUM_PRO_HP){
	    m_HpBCDlg->ShowBC(ENUM_PRO_HP);
        m_HPBcBtn->setEnabled(true);
    }else if (iSetProStyle==ENUM_PRO_Forging){
		m_HpBCDlg->ShowBC(ENUM_PRO_Forging);
        m_ForingBcBtn->setEnabled(true);
    }   
	m_HpBCDlg->show();
    m_HpBCDlg->raise();////���ϲ�
    m_HpBCDlg->activateWindow();//����
}

void MainWindow::HPBCActSlot02()
{
	if (viewWindow_){//pre 
		viewWindow_->TabView(1);
	}
	if (iSetProStyle != ENUM_PRO_Forging){
		m_HpBCDlg->ShowBC(0);
        m_HIPBcBtn->setEnabled(true);
		m_HpBCDlg->show();
        m_HpBCDlg->raise();////���ϲ�
        m_HpBCDlg->activateWindow();//����
     }else if (iSetProStyle==ENUM_PRO_Forging){
		ForgingContactActSlot();
    }   	
}
//--HPDLG
void MainWindow::HPInitActSlot()
{
	if (viewWindow_){//pre 
		viewWindow_->TabView(1);
	}
	if(iSetProStyle!=ENUM_PRO_Forging&&iSetProStyle!=ENUM_PRO_Casting){
	    m_HpInitDlg->show();
        m_HpInitDlg->raise();////���ϲ�
        m_HpInitDlg->activateWindow();//����
        if (iSetProStyle == ENUM_PRO_HIP){
            m_HIPInitBtn->setEnabled(true);
        }else if (iSetProStyle==ENUM_PRO_HP){
            m_HPInitBtn->setEnabled(true);
		} 
	}else if(iSetProStyle==ENUM_PRO_Forging){
		ForgingContactActSlot();
	}else if(iSetProStyle==ENUM_PRO_Casting){
		CastingContactActSlot();
	}
}

void MainWindow::HPInitActSlot0()
{
	if (viewWindow_){//pre 
		viewWindow_->TabView(1);
	}
	if(iSetProStyle==ENUM_PRO_Forging){
		HPInitActSlot02();
	}else if(iSetProStyle==ENUM_PRO_Casting){
		CastingInitActSlot();
	}
	
}
void MainWindow::HPInitActSlot02()
{
	if (viewWindow_){//pre 
		viewWindow_->TabView(1);
	}
	m_HpInitDlg->show();
    m_HpInitDlg->raise();////���ϲ�
    m_HpInitDlg->activateWindow();//����
    m_ForingInitBtn->setEnabled(true);
}
void MainWindow::ForgingSystemActSlot0()
{
	if(iSetProStyle==ENUM_PRO_Casting){
		//CastingSystemActSlot();
		CastingMotionBoundarySlot();
	}else if(iSetProStyle==ENUM_PRO_Forging){
		ForgingSystemActSlot();
	}
}
//--HPDLG  �������1��һ��
void MainWindow::HP_SolveSetPreChsSlot()
{
    if (m_HpSolveSetDlg->m_tabView->currentIndex()==1){
        //m_HpSolveSetDlg->SetOutPutTab(0);
        HPSloveSetActSlot();
        return;
    }

	if(iSetProStyle==ENUM_PRO_Casting){
		CastingSystemActSlot();
	}else if (iSetProStyle==ENUM_PRO_HIP){
		HIPSystemDlgActSlot();
	}else if (iSetProStyle==ENUM_PRO_HP){
		HPSystemActSlot();
	}else if (iSetProStyle==ENUM_PRO_Forging){
		ForgingSystemActSlot();
	}     
}
//����ƶ�
void MainWindow::HP_HIPSystemActSlot()
{
	if (iSetProStyle==ENUM_PRO_Casting){
		CastingHBSlot();
	}else if (iSetProStyle==ENUM_PRO_HIP){
		HIPSystemDlgActSlot();
	}else if (iSetProStyle==ENUM_PRO_HP){
		HPSystemActSlot();
	}else if (iSetProStyle==ENUM_PRO_Forging){
		ForgingHBSlot();
	}  
}
//�ȴ����ƶ�
void MainWindow::HPSystemActSlot()
{
    m_HpSystemDlg->show();
    m_HpSystemDlg->raise();////���ϲ�
    m_HpSystemDlg->activateWindow();//����
    
    if (iSetProStyle==ENUM_PRO_HIP){
        m_HIPSystemBtn->setEnabled(true);
    }else if (iSetProStyle==ENUM_PRO_HP){
        m_HPSystemBtn->setEnabled(true);
    }else if (iSetProStyle==ENUM_PRO_Forging){
        m_ForingSystemBtn->setEnabled(true);
    }      
}
//--HPDLG���������1����
void MainWindow::HPSloveSetActSlot()
{
    m_HpSolveSetDlg->SetOutPutTab(0);
    m_HpSolveSetDlg->show();
    m_HpSolveSetDlg->raise();////���ϲ�
    m_HpSolveSetDlg->activateWindow();//����
    m_HPSolveSetBtn->setEnabled(true);
}
//--HPDLG���������2����һ����
void MainWindow::HPSystemNextBtnSlot(int istep)
{
    m_HpSolveSetDlg->SetSloveStep(istep);
    m_HpSolveSetDlg->SetOutPutTab(0);
    m_HpSolveSetDlg->show();
    m_HpSolveSetDlg->raise();////���ϲ�
    m_HpSolveSetDlg->activateWindow();//����
    m_HPSolveSetBtn->setEnabled(true);
}
//--HPDLG
void MainWindow::HPSubmissonActSlot()
{ 
    m_HpSubmissionDlg->show();
    m_HpSubmissionDlg->raise();////���ϲ�
    m_HpSubmissionDlg->activateWindow();//����
}
void MainWindow::HPSolveNextActSlot()
{
    if (m_HpSolveSetDlg->m_tabView->currentIndex()==0){
        m_HpSolveSetDlg->SetOutPutTab(1);
        m_HpSolveSetDlg->show();
        m_HpSolveSetDlg->raise();////���ϲ�
        m_HpSolveSetDlg->activateWindow();//����
        //m_HPSolveSetBtn->setEnabled(true);
        return;
    }
    m_HpSubmissionDlg->show();
    m_HpSubmissionDlg->raise();////���ϲ�
    m_HpSubmissionDlg->activateWindow();//����
}
//--HipDLG
//�����Ի���
//
void MainWindow::HIPpartActSlot()
{
    //m_PartDlg->show(); 
    //m_PartDlg->raise();//���ϲ�
    //m_PartDlg->activateWindow();//����
    m_HpPartDlg->ShowPartDlgStyle(ENUM_PRO_HIP);
    m_HpPartDlg->show();
    m_HpPartDlg->raise();
    m_HpPartDlg->activateWindow();//����
}
//--HipDlg
//�߽�Ի���
void MainWindow::HIPBCActSlot()
{
    m_HpBCDlg->ShowBC(ENUM_PRO_HIP);
	m_HpBCDlg->show(); 
    m_HpBCDlg->raise();//���ϲ�
    m_HpBCDlg->activateWindow();//����
    m_HIPBcBtn->setEnabled(true);
}
//--HipDlg
//�߽�Ի���
void MainWindow::HIPInitDlgActSlot()
{
    //m_InitDlg->show(); 
    //m_InitDlg->raise();//���ϲ�
    //m_InitDlg->activateWindow();//����
    m_HpInitDlg->show(); 
    m_HpInitDlg->raise();//���ϲ�
    m_HpInitDlg->activateWindow();//����
    m_HIPInitBtn->setEnabled(true);
}
//--HIPDLG
//�ȵȾ�ѹ�ƶ�
void MainWindow::HIPSystemDlgActSlot()
{
    m_HIPSystemDlg->show(); 
    m_HIPSystemDlg->raise();//���ϲ�
    m_HIPSystemDlg->activateWindow();//����
    m_HIPSystemBtn->setEnabled(true);
}
//--HIPDLG
//��������öԻ���
void MainWindow::HIPResolveDlgActSlot()
{
    m_HpSolveSetDlg->SetOutPutTab(0);
    m_HpSolveSetDlg->show(); 
    m_HpSolveSetDlg->raise();//���ϲ�
    m_HpSolveSetDlg->activateWindow();//����
    m_HIPSolveSetBtn->setEnabled(true);
}
//--HIPDLG
//��������Ի���
void MainWindow::HIPVariableOutPutDlgSlot()
{   
    m_VariableOutputDlg->show(); 
    m_VariableOutputDlg->raise();//���ϲ�
    m_VariableOutputDlg->activateWindow();//����
}
//--HIPDLG
//-�ύ����
void MainWindow::HIPSubmissonActSlot()
{
	//m_HipSubmissionDlg->show();
	//m_HipSubmissionDlg->raise();////���ϲ�
	//m_HipSubmissionDlg->activateWindow();//����

	m_HpSubmissionDlg->show();
    m_HpSubmissionDlg->raise();////���ϲ�
    m_HpSubmissionDlg->activateWindow();//����
}

//--����1.ģ��װ��
void MainWindow::AssembleDlgSlot()
{
	m_AssemblingAct_->show();
	m_AssemblingAct_->raise();
	m_AssemblingAct_->activateWindow();
}
//--����2.��������
void MainWindow::courseCurveDlgSlot()
{
    PostPro_->xyplotPanel_->show();
    PostPro_->xyplotPanel_->raise();//���ϲ�
    PostPro_->xyplotPanel_->activateWindow();//����
} 
//--����3.��������
void MainWindow::postPropDlgSlot()
{
    PostPro_->m_PostWidPlotOptDlg->show(); 
    PostPro_->m_PostWidPlotOptDlg->raise();//���ϲ�
    PostPro_->m_PostWidPlotOptDlg->activateWindow();//����
}
//--����4.������ʾ����
void MainWindow::partElSetHideDlgSlot()
{
	m_WidgetInpElsetDlg->show();
	m_WidgetInpElsetDlg->raise();
	m_WidgetInpElsetDlg->activateWindow();
}

//--����5.�������
void MainWindow::DistanceMeasurementSlot()
{
	m_DistMeasurementAct_->show();
	m_DistMeasurementAct_->raise();
	m_DistMeasurementAct_->activateWindow();
}

void MainWindow::GravitySlot()
{
	m_GravityAct_->ShowHideSlot(0);
	m_GravityAct_->show();
	m_GravityAct_->raise();
	m_GravityAct_->activateWindow();
}
void MainWindow::QStatisticsSlot()
{
	m_QStatisticsDlg->show();
	m_QStatisticsDlg->move(600,300);
	m_QStatisticsDlg->raise();
	m_QStatisticsDlg->activateWindow();
}


//--HIPDLG
//��������ã������������
//cmd�У�����·��1.�����·��;
//2.Inp·����

void MainWindow::HIPSolveActOpenSlot()
{    
    QString workPath=QDir::currentPath();
	
    
    QString filename = QFileDialog::getOpenFileName(this, "Read Inp file...", workPath, "Inp Files (*.inp);;");   
    if (filename.size()==0){
        return;
    }
    QString str = QCoreApplication::applicationDirPath();
    workPath = str.left(str.lastIndexOf("/"));
#if _MSC_VER
    workPath += "/Solver/WeICME.exe";
#else
    workPath += "/Solver/WeICME";
#endif
    if (!QFileInfo(workPath).exists()) {
        Information_Widget::GetInstance()->ShowInformation("Solver binary not found: " + workPath);
        return;
    }
    if (!QFileInfo(filename).exists()) {
        Information_Widget::GetInstance()->ShowInformation("Input file not found: " + filename);
        return;
    }
    if (m_SloverProcess) {
        m_SloverProcess->kill();
        m_SloverProcess->deleteLater();
        m_SloverProcess = NULL;
    }
    QString solverArg = QFileInfo(filename).completeBaseName();
    m_SloverProcess = new QProcess(this);
    QProcessEnvironment env02 = QProcessEnvironment::systemEnvironment();
    QString CPUNumber = m_HpSubmissionDlg->m_EditCalcNum->text();
    env02.insert("OMP_NUM_THREADS", CPUNumber);
    m_SloverProcess->setProcessEnvironment(env02);
    m_SloverProcess->start(workPath, QStringList() << solverArg);
    if (!m_SloverProcess->waitForStarted()) {
        Information_Widget::GetInstance()->ShowInformation("Failed to start solver");
        delete m_SloverProcess;
        m_SloverProcess = NULL;
        return;
    }
    this->connect(m_SloverProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(ShowStdOutput()));
    this->connect(m_SloverProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(SolverProcessErrorSlot()));
	//QString strTemp=QString::fromLocal8Bit(m_SloverProcess->readAllStandardOutput());
	//Information_Widget::GetInstance()->ShowInformation(QString(strTemp));
}
void MainWindow::HIPSolveActKillSlot()
{
	if(m_SloverProcess!=NULL){
		m_SloverProcess->kill();
       Information_Widget::GetInstance()->ShowInformation("��ֹ����");
	}
}
void MainWindow::GmshProcessErrorSlot()
{
    Information_Widget::GetInstance()->ShowInformation("gmsh process error occurred");
}

void MainWindow::SolverProcessErrorSlot()
{
    Information_Widget::GetInstance()->ShowInformation("solver process error occurred");
}
//����
void MainWindow::ForgingpartActSlot()
{
	m_HpPartDlg->ShowPartDlgStyle(ENUM_PRO_Forging);
    m_HpPartDlg->show();
    m_HpPartDlg->raise();////���ϲ�
    m_HpPartDlg->activateWindow();//����
}

void MainWindow::ForgingBCActSlot()
{
	m_HpBCDlg->ShowBC(ENUM_PRO_Forging);
	m_HpBCDlg->show();
    m_HpBCDlg->raise();////���ϲ�
    m_HpBCDlg->activateWindow();//����
	m_ForingBcBtn->setEnabled(true);
}
//�Ӵ��Ի���
void MainWindow::ForgingContactActSlot()
{
	m_ForgingContactDlg->ShowContact(ENUM_PRO_Forging);
    m_ForgingContactDlg->show(); 
    m_ForgingContactDlg->raise();//���ϲ�
    m_ForgingContactDlg->activateWindow();//����
	m_ForingContactBtn->setEnabled(true);
}
//��ʼ��
void MainWindow::ForgingInitActSlot()
{
	m_HpInitDlg->show(); 
    m_HpInitDlg->raise();//���ϲ�
    m_HpInitDlg->activateWindow();//����
	m_ForingInitBtn->setEnabled(true);
}
void MainWindow::ForgingHBSlot()
{
	m_ThermalBoundaryDlg->show();
	m_ThermalBoundaryDlg->raise();//���ϲ�
    m_ThermalBoundaryDlg->activateWindow();//����
	m_ForingTBBtn->setEnabled(true);
}
//�����ƶ�
void MainWindow::ForgingSystemActSlot()
{
	m_ForgingSystemDlg->ShowFS3(ENUM_PRO_Forging); 
    m_ForgingSystemDlg->show(); 
    m_ForgingSystemDlg->raise();//���ϲ�
    m_ForgingSystemDlg->activateWindow();//����
	m_ForingSystemBtn->setEnabled(true);
}

void MainWindow::ForgingSystemActSlot2(m_TBName m_ThB)
{
	QStringList Str;
	for(int i=0;i<m_ThB.m_ThermalBoundaryName2.size();i++){
		Str<<m_ThB.m_ThermalBoundaryName2.at(i);
	}
	m_ForgingSystemDlg->ShowFS(Str);
}
void MainWindow::ForgingSystemActSlot3(QMap<int,QString> m_ThB)
{
	m_ForgingSystemDlg->ShowFS2(m_ThB);
}
//�����ƶȵ���һ��
void MainWindow::ForgingSystemNextBtnSlot0(int istep)
{

	if (iSetProStyle==ENUM_PRO_Casting){
		CastingSystemNextBtnSlot(istep);
	}else if (iSetProStyle==ENUM_PRO_Forging){
		ForgingSystemNextBtnSlot(istep);
	}  
}
void MainWindow::ForgingSystemNextBtnSlot(int istep)
{
	m_HpSolveSetDlg->SetSloveStep(istep);
    m_HpSolveSetDlg->SetOutPutTab(0);
    m_HpSolveSetDlg->show();
    m_HpSolveSetDlg->raise();////���ϲ�
    m_HpSolveSetDlg->activateWindow();//����
    m_HPSolveSetBtn->setEnabled(true);
}

//��������öԻ���
void MainWindow::ForgingSloveSetActSlot()
{
	//m_HpSolveSetDlg->SetOutPutTab(0);
    m_HpSolveSetDlg->show(); 
    m_HpSolveSetDlg->raise();//���ϲ�
    m_HpSolveSetDlg->activateWindow();//����
	m_ForingSolveSetBtn->setEnabled(true);
}

//-�ύ����
void MainWindow::ForgingSubmissonActSlot()
{
	//m_ForgingSubmissionDlg->show();
	//m_ForgingSubmissionDlg->raise();////���ϲ�
	//m_ForgingSubmissionDlg->activateWindow();//����
	m_HpSubmissionDlg->show();
    m_HpSubmissionDlg->raise();////���ϲ�
    m_HpSubmissionDlg->activateWindow();//����
}
//��������
void MainWindow::CastingGravitySlot()
{
	m_CastingGravityDlg->ShowHideSlot(1);
    m_CastingGravityDlg->show(); 
    m_CastingGravityDlg->raise();//���ϲ�
    m_CastingGravityDlg->activateWindow();//����
	m_CastingGravityDlg->setEnabled(true);
}
void MainWindow::CastingpartActSlot()
{
	m_HpPartDlg->ShowPartDlgStyle(0);
    m_HpPartDlg->show();
    m_HpPartDlg->raise();////���ϲ�
    m_HpPartDlg->activateWindow();//����
	m_CastingPartBtn->setEnabled(true);
}

void MainWindow::CastingBCActSlot()
{
	m_HpBCDlg->ShowBC(2);
	m_HpBCDlg->show();
    m_HpBCDlg->raise();////���ϲ�
    m_HpBCDlg->activateWindow();//����
	m_CastingBcBtn->setEnabled(true);
}
void MainWindow::CastingContactActSlot()
{
	m_ForgingContactDlg->ShowContact(ENUM_PRO_Casting);
	m_ForgingContactDlg->show(); 
	m_ForgingContactDlg->raise();//���ϲ�
	m_ForgingContactDlg->activateWindow();//����
	m_CastingContactBtn->setEnabled(true);
}
void MainWindow::CastingInitActSlot()
{
	m_HpInitDlg->show();
    m_HpInitDlg->raise();//���ϲ�
    m_HpInitDlg->activateWindow();//����
	m_CastingInitBtn->setEnabled(true);
}
void MainWindow::CastingHBSlot()
{
	m_ThermalBoundaryDlg->show();
	m_ThermalBoundaryDlg->raise();//���ϲ�
    m_ThermalBoundaryDlg->activateWindow();//����
	m_CastingTBBtn->setEnabled(true);
}
void MainWindow::CastingMotionBoundarySlot()
{
	m_ForgingSystemDlg->ShowFS3(ENUM_PRO_Casting);
	m_ForgingSystemDlg->show(); 
    m_ForgingSystemDlg->raise();//���ϲ�
    m_ForgingSystemDlg->activateWindow();//����
	m_CastingMotionBtn->setEnabled(true);
}
void MainWindow::CastingSystemActSlot()
{
	m_ForgingSystemDlg->ShowFS3(ENUM_PRO_Casting);
	m_ForgingSystemDlg->show(); 
    m_ForgingSystemDlg->raise();//���ϲ�
    m_ForgingSystemDlg->activateWindow();//����
	m_CastingMotionBtn->setEnabled(true);
	//m_CastingSystemBtn->setEnabled(true);
    
}
void MainWindow::CastingSystemNextBtnSlot(int istep)
{
	m_HpSolveSetDlg->SetSloveStep(istep);
    m_HpSolveSetDlg->SetOutPutTab(0);
    m_HpSolveSetDlg->show();
    m_HpSolveSetDlg->raise();////���ϲ�
    m_HpSolveSetDlg->activateWindow();//����
    m_HPSolveSetBtn->setEnabled(true);
}
void MainWindow::CastingSloveSetActSlot()
{
	m_HpSolveSetDlg->show(); 
    m_HpSolveSetDlg->raise();//���ϲ�
    m_HpSolveSetDlg->activateWindow();//����
	m_CastingSolveSetBtn->setEnabled(true);
}
void MainWindow::CastingSubmissonActSlot()
{
	m_HpSubmissionDlg->show();
    m_HpSubmissionDlg->raise();////���ϲ�
    m_HpSubmissionDlg->activateWindow();//����
}

//--��ʱ����
void MainWindow::ShowStdOutput()
{
    QByteArray data = m_SloverProcess->readAllStandardOutput();
    QString str(data);
    Information_Widget::GetInstance()->ShowInformation(str);
}
//--StepPlay������1
void MainWindow::updataStepPlayCombox(ResultOutputS ResultO)
{
    if (viewWindow_){
        viewWindow_->TabView(0);//tabView_->setCurrentIndex(0);
    }
   m_StepPlayWidget->updataCombox(ResultO);
}
//--StepPlay������2(widgeresultout->emit to updata)
void MainWindow::updataStepPlayParam(ResultVisS visParam)
{
    if (viewWindow_){
        viewWindow_->TabView(0);//tabView_->setCurrentIndex(0);
        DockPostPrc_->raise();  
    }
    StepPlayVisS stepParam;
    stepParam.strName = visParam.strName;
    stepParam.m_iStyle = visParam.m_iStyle;
    stepParam.bContour = visParam.bContour;
    stepParam.m_iClrScalar = visParam.m_iClrScalar;
    stepParam.m_dTrans = visParam.m_dTrans;
    stepParam.m_iOrigCurrentChg = visParam.m_iOrigCurrentChg;
    stepParam.bCurGrid= visParam.bCurGrid;
    stepParam.bCutZoneVisible= visParam.bCutZoneVisible;
    stepParam.CutIdList =visParam.CutIdList;
    stepParam.bCutVTKWidgetVisible= visParam.bCutVTKWidgetVisible;
    stepParam.contourMin= visParam.contourMin;
    stepParam.contourMax= visParam.contourMax;
    stepParam.bContourMinMax= visParam.bContourMinMax;
    stepParam.m_gridShowColor=visParam.m_gridShowColor;

	stepParam.movieName=visParam.movieName;
	stepParam.movieNameRate=visParam.movieNameRate;
	stepParam.movieSet=visParam.movieSet;
    m_StepPlayWidget->updataParam(stepParam);
}
//--StepPlay������3
void MainWindow::PlayStepPlay(StepPlayVisS stepParam)
{
    if (viewWindow_){
        viewWindow_->TabView(0);//tabView_->setCurrentIndex(0);
        DockPostPrc_->raise();  
    }
    ResultVisS visParam;
    visParam.strName = stepParam.strName;
    visParam.m_iStyle = stepParam.m_iStyle;
    visParam.bContour = stepParam.bContour;
    visParam.m_iClrScalar = stepParam.m_iClrScalar;
    visParam.m_dTrans = stepParam.m_dTrans;
    visParam.m_iOrigCurrentChg = stepParam.m_iOrigCurrentChg;
    visParam.bCurGrid = stepParam.bCurGrid;
    visParam.bCutZoneVisible = stepParam.bCutZoneVisible;
    visParam.bCutVTKWidgetVisible = stepParam.bCutVTKWidgetVisible;
    visParam.CutIdList = stepParam.CutIdList;
    visParam.contourMin = stepParam.contourMin;
    visParam.contourMax = stepParam.contourMax;
    visParam.bContourMinMax = stepParam.bContourMinMax;
    visParam.m_gridShowColor = stepParam.m_gridShowColor;
	visParam.movieSet = stepParam.movieSet;
	visParam.movieName = stepParam.movieName;
	visParam.movieNameRate = stepParam.movieNameRate;
    PostPro_->UpDataScalar(visParam);
    PostPro_->m_PostWigResultOut->m_VisParam = visParam;//set post wigresultout param;
}


