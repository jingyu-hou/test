#include "mainwindow.h"
#include <QTextCodec>
#include <QFile>
#include <QTimer>
#include <QDesktopWidget>
#include "QMyVTK.h"
#include "SARibbonApplicationButton.h"

static void ImportTrace(const QString &msg)
{
    QFile f("/mnt/d/ZZKK/import_trace.log");
    if (f.open(QIODevice::Append | QIODevice::Text)) {
        f.write(msg.toUtf8());
        f.write("\n");
        f.close();
    }
}

static QString SafeGetOpenFileName(QWidget *parent, const QString &caption, const QString &dir, const QString &filter)
{
    static QFileDialog *dlg = NULL;
    if (!dlg) {
        dlg = new QFileDialog(parent);
        dlg->setOption(QFileDialog::DontUseNativeDialog, true);
        dlg->setFileMode(QFileDialog::ExistingFile);
        dlg->setAcceptMode(QFileDialog::AcceptOpen);
    }
    dlg->setWindowTitle(caption);
    dlg->setDirectory(dir);
    dlg->setNameFilter(filter);
    dlg->selectFile(QString());
    if (dlg->exec() != QDialog::Accepted) {
        return QString();
    }
    QStringList files = dlg->selectedFiles();
    if (files.isEmpty()) {
        return QString();
    }
    return files.first();
}

//MainWindow::MainWindow(QWidget *parent,Qt::WFlags flags)
//   : QMainWindow(parent,flags)
MainWindow::MainWindow(QWidget *par):SARibbonMainWindow(par)
{
	/*QFileDialog::Option options;
	options=QFileDialog::DontUseNativeDialog;
	QString workPath=options;
	QDir::setCurrent(workPath);*/

    m_contextCategoryCasting = 0;
    m_contextCategory_Casting_C = 0;
    m_contextCategoryHE = 0;
    m_contextCategory_HE_C = 0;

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
    if (!qgetenv("WEICME_AUTOINP").isEmpty()) {
        QTimer::singleShot(1200, this, SLOT(OpenSlot()));
    }

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
void MainWindow::ForceShowMainWindow()
{
    QRect screen = QApplication::desktop()->availableGeometry();
    QRect geom;
    if (screen.width() < 400 || screen.height() < 300) {
        geom = QRect(80, 80, 1400, 850);
    } else {
        int w = qMin(screen.width() - 120, 1500);
        int h = qMin(screen.height() - 120, 900);
        geom = QRect(screen.left() + 60, screen.top() + 60, w, h);
    }

    setWindowState(windowState() & ~Qt::WindowMinimized);
    setGeometry(geom);
    showNormal();
    raise();
    activateWindow();
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
	if (!m_WidgetInpElsetDlg){m_WidgetInpElsetDlg = new QInpActorWidge(this);}//工具-部件隐藏/显示
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
    //--热等静压（1）
    if (!m_HIPSystemDlg){m_HIPSystemDlg = new QHIPsystemDlg(this);}
    if (!m_VariableOutputDlg){m_VariableOutputDlg = new QVariableOutputDlg(this);}
	if (!m_HipSubmissionDlg){m_HipSubmissionDlg=new QHIPSubmissionDlg(this);}
    //--热等静压中高亮显示（1.1）
    connect(m_HIPSystemDlg,SIGNAL(emitPsetHighLight(NElSurfChsS)),viewWindow_,SLOT(HightLightPSet(NElSurfChsS)));
	connect(m_HipSubmissionDlg,SIGNAL(emitHipInpSave()),this,SLOT(MainSaveHIPInpSlot()));
	connect(m_HipSubmissionDlg,SIGNAL(emitHipInpRun()),this,SLOT(HIPSolveActOpenSlot()));
	
	
    //showMaximized();
    //--热处理（2）
    if (!m_HpPartDlg){m_HpPartDlg=new QHPPartDlg(this);}
    if (!m_HpBCDlg){m_HpBCDlg=new QHPBCDlg(this);}
    if (!m_HpInitDlg){m_HpInitDlg=new QHPInitDlg(this);}
    if (!m_HpSystemDlg){m_HpSystemDlg=new QHPSystemDlg(this);}
    if (!m_HpSolveSetDlg){m_HpSolveSetDlg=new QHPSolveSetDlg(this);}
    if (!m_HpSubmissionDlg){m_HpSubmissionDlg=new QHPSubmissionDlg(this);}
    //--热处理中高亮显示（2.1）
    connect(m_HpPartDlg,SIGNAL(emitElsetHighLight(NElSurfChsS)),viewWindow_,SLOT(HightLightPSet(NElSurfChsS)));
    connect(m_HpBCDlg,SIGNAL(emitPsetHighLight(NElSurfChsS)),viewWindow_,SLOT(HightLightPSet(NElSurfChsS)));
    connect(m_HpSystemDlg,SIGNAL(emitPsetHighLight(NElSurfChsS)),viewWindow_,SLOT(HightLightPSet(NElSurfChsS)));
	connect(m_HpInitDlg,SIGNAL(emitPsetHighLight(NElSurfChsS)),viewWindow_,SLOT(HightLightPSet(NElSurfChsS)));
    connect(m_HpSubmissionDlg,SIGNAL(emitHpInpSave()),this,SLOT(MainSaveHPInpSlot()));
    connect(m_HpSubmissionDlg,SIGNAL(emitHpInpRun()),this,SLOT(HIPSolveActOpenSlot()));
	connect(m_HpSubmissionDlg,SIGNAL(emitCheck()),this,SLOT(InpCheckSlot()));//inp文件检查功能
	connect(m_HpSubmissionDlg,SIGNAL(emitHipInpKill()),this,SLOT(HIPSolveActKillSlot()));
	//锻造
	if (!m_ForgingContactDlg){m_ForgingContactDlg=new QForgingContactDlg(this);}
	if (!m_ForgingSystemDlg){m_ForgingSystemDlg=new QForgingSystemDlg(this);}
	if (!m_ForgingSubmissionDlg){m_ForgingSubmissionDlg=new QForgingSubmissionDlg(this);}
	if (!m_ThermalBoundaryDlg){m_ThermalBoundaryDlg=new ThermalBoundary(this);}
    connect(m_ForgingSubmissionDlg,SIGNAL(emitForgingInpSave()),this,SLOT(MainSaveForgingInpSlot()));
    connect(m_ForgingSubmissionDlg,SIGNAL(emitForgingInpRun()),this,SLOT(HIPSolveActOpenSlot()));
	connect(m_ForgingContactDlg,SIGNAL(emitPsetHighLight(NElSurfChsS)),viewWindow_,SLOT(HightLightPSet(NElSurfChsS)));//接触界面面集合高亮显示
	connect(m_ForgingSystemDlg,SIGNAL(emitPsetHighLight(NElSurfChsS)),viewWindow_,SLOT(HightLightPSet(NElSurfChsS)));
	connect(m_ThermalBoundaryDlg,SIGNAL(emitPsetHBHighLight(NElSurfChsS)),viewWindow_,SLOT(HightLightPSet(NElSurfChsS)));
	connect(m_ThermalBoundaryDlg,SIGNAL(emitHotName(m_TBName)),this,SLOT(ForgingSystemActSlot2(m_TBName)));
	connect(m_ThermalBoundaryDlg,SIGNAL(emitHotName2(QMap<int,QString>)),this,SLOT(ForgingSystemActSlot3(QMap<int,QString>)));
	//铸造
	if (!m_CastingGravityDlg){m_CastingGravityDlg=new Gravity(this);}
	 // Gravity is a dialog; the start action is wired through its own buttons.
   
    //--全局cuvre（）
    connect(m_HIPSystemDlg,SIGNAL(emitHIPCurveData(QStringList,InpCurveInpS)),m_HpSystemDlg,SLOT(updateHPCurveDataSlot(QStringList,InpCurveInpS)));
    connect(m_HpSystemDlg,SIGNAL(emitHPCurve(QStringList,InpCurveInpS)),m_HIPSystemDlg,SLOT(updateHIPCurveDataSlot(QStringList,InpCurveInpS)));
    //上一步/下一步
    //1.求解设置
    connect(m_HpSolveSetDlg->m_PreBtn,SIGNAL(clicked()),this,SLOT(HP_SolveSetPreChsSlot()));
    connect(m_HpSolveSetDlg->m_NextBtn,SIGNAL(clicked()),this,SLOT(HPSolveNextActSlot()));
    //2.1热处理制度
    connect(m_HpSystemDlg,SIGNAL(emitHPsystemNext(int)),this,SLOT(HPSystemNextBtnSlot(int)));
    connect(m_HpSystemDlg->m_PreBtn,SIGNAL(clicked()),this,SLOT(HPInitActSlot()));
    //2.2热等静压制度
    connect(m_HIPSystemDlg->m_PreHipBtn,SIGNAL(clicked()),this,SLOT(HPInitActSlot()));
    connect(m_HIPSystemDlg->m_NextHipSysBtn,SIGNAL(clicked()),this,SLOT(HIPResolveDlgActSlot()));//HPSloveSetActSlot()));
	//2.3锻造制度
	connect(m_ForgingSystemDlg,SIGNAL(emitForgingsystemNext(int)),this,SLOT(ForgingSystemNextBtnSlot0(int)));
    connect(m_ForgingSystemDlg->m_PreSystemBtn,SIGNAL(clicked()),this,SLOT(ForgingHBSlot()));
	// Next step is emitted by QForgingSystemDlg::emitForgingsystemNext(int).
	//connect(m_ForgingSystemDlg->m_PreSystemBtn,SIGNAL(clicked()),this,SLOT(ForgingHBSlot()));
    //connect(m_ForgingSystemDlg->m_NextSystemBtn,SIGNAL(clicked()),this,SLOT(ForgingSloveSetActSlot()));
	//connect(m_ForgingSystemDlg->m_NextSystemBtn,SIGNAL(clicked(int)),this,SLOT(ForgingSystemNextBtnSlot(int)));

	//重力
    connect(m_CastingGravityDlg->NextBtn,SIGNAL(clicked()),this,SLOT(HP_HIPPartActSlot()));
    //3.部件
    connect(m_HpPartDlg->m_NextSectionBtn,SIGNAL(clicked()),this,SLOT(HPBCActSlot()));
	connect(m_HpPartDlg,SIGNAL(emitMerge()),this,SLOT(MergeSlot()));
    //4.边界
    connect(m_HpBCDlg->m_PreBCBtn,SIGNAL(clicked()),this,SLOT(HP_HIPPartActSlot()));
    connect(m_HpBCDlg->m_NextBCBtn,SIGNAL(clicked()),this,SLOT(HPInitActSlot()));
 
	//5.接触
    connect(m_ForgingContactDlg->m_PreContactBtn,SIGNAL(clicked()),this,SLOT(HPBCActSlot()));
    connect(m_ForgingContactDlg->m_NextContactBtn,SIGNAL(clicked()),this,SLOT(HPInitActSlot0()));

	//connect(m_CastingContactBtn->m_PreContactBtn,SIGNAL(clicked()),this,SLOT(HPBCActSlot()));
    //connect(m_CastingContactBtn->m_NextContactBtn,SIGNAL(clicked()),this,SLOT(HPInitActSlot02()));

    //6.初始化
    connect(m_HpInitDlg->m_PreInitBtn,SIGNAL(clicked()),this,SLOT(HPBCActSlot02()));
    connect(m_HpInitDlg->m_NextInitBtn,SIGNAL(clicked()),this,SLOT(HP_HIPSystemActSlot()));
		//7.热边界
	connect(m_ThermalBoundaryDlg->m_HBPreBtn,SIGNAL(clicked()),this,SLOT(HPInitActSlot02()));
    connect(m_ThermalBoundaryDlg->m_HBNextBtn,SIGNAL(clicked()),this,SLOT(ForgingSystemActSlot0()));

    //-点集合、单元集合、表面集合
	connect(m_PElSfSetDlg,SIGNAL(emitCurrentPSet(QString)),m_HpBCDlg,SLOT(AppendPSetComboxSlot(QString)));
	connect(m_PElSfSetDlg,SIGNAL(emitCurrentPSet(QString)),m_HpInitDlg,SLOT(AppendPSetComboxIniSlot(QString)));
	connect(m_PElSfSetDlg,SIGNAL(emitCurrentPSet(QString)),m_HpSolveSetDlg,SLOT(AppendPSetComboxSolvesetSlot(QString)));

    connect(m_PElSfSetDlg,SIGNAL(emitCurrentElSet(QString)),m_HpPartDlg,SLOT(AppendElSetComboxSlot(QString)));
    connect(m_PElSfSetDlg,SIGNAL(emitCurrentSetsData(QString,int,DecodeGenerateS)),m_HpPartDlg,SLOT(AppendPElData(QString,int,DecodeGenerateS)));
    connect(m_PElSfSetDlg,SIGNAL(emitCurrentSurfSet(QString)),m_HIPSystemDlg,SLOT(AppendSurfSetComboxSlot(QString)));

	connect(m_PElSfSetDlg,SIGNAL(emitCurrentSurfSet(QString)),m_ForgingContactDlg,SLOT(AppendSurfSetComboxContactSlot(QString)));
	connect(m_PElSfSetDlg,SIGNAL(emitCurrentSurfSet(QString)),m_ThermalBoundaryDlg,SLOT(AppendSurfSetComboxTBSlot(QString)));
    //--变量初始化：
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
				float ZB1=Node2.at(1).toFloat();//保存平移量
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
					QString STR201=QString("%1").arg(ZB11);//浮点数转换为字符串
					QString STR202=QString("%1").arg(ZB22);//浮点数转换为字符串
					Node3=Node3+","+STR201+","+STR202;
					if(NM2>3){
						ZB33=data.MRoveA[6]*ZB3;
						QString STR203=QString("%1").arg(ZB33);//浮点数转换为字符串
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
				QString STR201=QString("%1").arg(ZB11);//浮点数转换为字符串
				QString STR202=QString("%1").arg(ZB22);//浮点数转换为字符串
				if(NM2<=3){
					Node3=Node3+","+STR201+","+STR202;
				}else{
					QString STR203=QString("%1").arg(ZB33);//浮点数转换为字符串
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
							float ZB1=Node2.at(1).toFloat();//保存平移量
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
								QString STR201=QString("%1").arg(ZB11);//浮点数转换为字符串
								QString STR202=QString("%1").arg(ZB22);//浮点数转换为字符串
								Node3=Node3+","+STR201+","+STR202;
								if(NM2>3){
									ZB33=data.MRoveA[6]*ZB3;
									QString STR203=QString("%1").arg(ZB33);//浮点数转换为字符串
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
							QString STR201=QString("%1").arg(ZB11);//浮点数转换为字符串
							QString STR202=QString("%1").arg(ZB22);//浮点数转换为字符串
							if(NM2<=3){
								Node3=Node3+","+STR201+","+STR202;
							}else{
								QString STR203=QString("%1").arg(ZB33);//浮点数转换为字符串
								Node3=Node3+","+STR201+","+STR202+","+STR203;
							}
							m_Data.TmpNodeInpS.strData.replace(IJ,Node3);
						}
					}
				}
			}
		}
	//float time=t120.elapsed()/1000.0;

	ImportTrace("OpenSlot: before TreeModel");
        PreHIPPro_->m_TreeModel->setInpData(m_Data);
        ImportTrace("OpenSlot: after TreeModel");
	if (viewWindow_){
		viewWindow_->TabView(1);//setCurrentIndex(1);
		QMdiSubWindow *CurWindow=m_MdiArea->currentSubWindow();
		for (int i=0;i<m_MdiArea->subWindowList().size();i++){
			QMdiSubWindow *TmpWindow=m_MdiArea->subWindowList().at(i);
			if (TmpWindow == TmpWindow){
				ImportTrace("OpenSlot: before ShowCurPreData");
					viewWindow_->ShowCurPreData(m_Data); 
                    ImportTrace("OpenSlot: after ShowCurPreData"); 
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
描述：创建Ribbon格式
*/
void MainWindow::creatRibbon()
{ 
    //setWindowTitle(tr("WelCME"));
    // Keep the native window frame so the WSLg/Windows title bar remains draggable.
	m_ribbon= ribbonBar();
	m_FilBtn=m_ribbon->applitionButton();
	m_FilBtn->setText(tr(" 文件"));
    m_ribbon->setWindowIcon(QPixmap(":/images/welcme.png"));//(":/images/welcme.png"));
     //   
    m_CategoryApp = m_ribbon->addCategoryPage(tr("应用"));
    createCategoryApp(m_CategoryApp);
    m_CategoryViewer = m_ribbon->addCategoryPage(tr("视区"));
    createCategoryView(m_CategoryViewer);
	//以下几行隐藏数据库 luo
    /*m_CategoryDataBase = m_ribbon->addCategoryPage(tr("数据库"));
    createCategoryDateBS(m_CategoryDataBase);*/
	//以上几行隐藏数据库 luo

     //以下几行隐藏查看 luo
    /*m_CategoryLookOver = m_ribbon->addCategoryPage(tr("查看"));
    createCategoryLookOver(m_CategoryLookOver);*/
	//以上几行隐藏查看 luo


    m_CategoryTool = m_ribbon->addCategoryPage(tr("工具"));
    createCategoryTool(m_CategoryTool);
   //以下几行隐藏优化 luo
   /* m_CategoryOPtim = m_ribbon->addCategoryPage(tr("优化"));
    createCategoryOptim(m_CategoryOPtim);*/
	//以上几行隐藏优化 luo

	//以下几行隐藏窗口 luo
    /*m_CategoryWindow = m_ribbon->addCategoryPage(tr("窗口"));
    createCategoryWindow(m_CategoryWindow);*/
	//以上几行隐藏窗口 luo

    m_CategoryHelp = m_ribbon->addCategoryPage(tr("帮助"));
    createCategoryHelp(m_CategoryHelp);

    m_contextCategoryHIP = m_ribbon->addContextCategory(tr("多孔介质"), Qt::red, 1);
    m_contextCategory_HIP_C = m_contextCategoryHIP->addCategoryPage(tr("设置"));
    
    creatContextHIPWindow(m_contextCategory_HIP_C);

    QList<bool> sysFlags = AppKey::Instance()->PSystem();

    // Commercial workflow only exposes porous media, heat treatment and forging.
    m_contextCategoryHE = 0;
    m_contextCategory_HE_C = 0;

    m_contextCategoryHP = m_ribbon->addContextCategory(tr("热处理"), Qt::blue, 3);
    m_contextCategory_HP_C = m_contextCategoryHP->addCategoryPage(tr("设置"));
    creatContextHPWindow(m_contextCategory_HP_C);

	m_contextCategoryForging= m_ribbon->addContextCategory(tr("锻造"), Qt::yellow, 4);
    m_contextCategory_Forging_C = m_contextCategoryForging->addCategoryPage(tr("设置"));
    creatContextForgingWindow(m_contextCategory_Forging_C);

    m_contextCategoryCasting = 0;
    m_contextCategory_Casting_C = 0;

	

    creatFileMenu();
    //-quickAcessBar
    m_ribbon->quickAccessBar()->addButton(HideShowAct_);
    //m_ribbon->quickAccessBar()->addButton(HideShowAct_);
}
//创建下拉菜单
void MainWindow::creatFileMenu()
{ 
    m_PushFileBtn = new QPushButton(m_ribbon);
    m_PushFileBtn->hide();
    m_PushFileBtn =(QPushButton *)m_FilBtn;//强制转换为QPushButton 可进行Menu添加
    m_PushFileBtn->setStyleSheet("QPushButton::menu-indicator{image:none;}");//不显示三角形下拉图片
    //创建一个菜单
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
	//隐藏语言切换 luo
    /*m_ChangeLanguageTitle=fileMenu_->addMenu(tr("语言切换"));
    m_ChangeLanguageTitle->addAction(LanguageCAct_);
    m_ChangeLanguageTitle->addAction(LanguageUAct_);*/
	//隐藏语言切换 luo
    fileMenu_->addAction(closeAct_);

    m_PushFileBtn->setMenu(fileMenu_);  
}

/*
描述：应用窗口创建
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
	//以下几行隐藏启动
    /*btn=pannel->addLargeAction(AppStartAct_);
    btn->setFixedSize(78,58);*/
	//以上几行隐藏启动
 
    btn=pannel->addLargeAction(AppMeshAct_);
    btn->setFixedSize(78,58);
    btn->setCheckable(true);
    btn->setPopupMode(QToolButton::InstantPopup);
    AppMeshAct_->setMenu(gmshMenu);

	
	
	// Hide casting in the commercial workflow.
	
	if(ShowHid.at(0)){
		m_HIPBtn=pannel->addLargeAction(AppHIPAct_);
		m_HIPBtn->setFixedSize(78,58);
		m_HIPBtn->setPopupMode(QToolButton::InstantPopup);
	}
    //m_HIPBtn->setCheckable(true);

    //pannel->addWidget(m_HIPBtn);
	//以下几行隐藏热挤压
	
	// Hide hot extrusion in the commercial workflow.
	//以上几行隐藏热挤压
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

	//以下几行隐藏结果 luo
    /*btn=pannel->addLargeAction(AppResultAct_);
    btn->setFixedSize(78,58);  */
	//以上几行隐藏结果 luo
    btn=pannel->addLargeAction(AppRunAct_);
    btn->setFixedSize(78,58);

    pannel->setExpanding(0);
}
/*
描述：视区窗口创建
*/
void MainWindow::createCategoryView(SARibbonCategory* page)
{
    SARibbonToolButton * btn;
    SARibbonPannel* pannel = page->addPannel(tr("视图区域"));//(tr("Panel 1"));
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
描述：数据库窗口创建
*/
void MainWindow::createCategoryDateBS(SARibbonCategory* page)
{
    SARibbonToolButton * btn;
    SARibbonPannel* pannel = page->addPannel(tr("数据库区域"));//(tr("Panel 1"));
    btn=pannel->addLargeAction(MaterialLibAct_);
    btn->setFixedSize(78,58);
    btn=pannel->addLargeAction(ProcessLibAct_);
    btn->setFixedSize(78,58);
    pannel->setExpanding(0);
}
/*
描述：查看窗口创建
*/
void MainWindow::createCategoryLookOver(SARibbonCategory* page)
{

}
/*
描述：工具窗口创建
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

    //以下几行隐藏历程曲线 luo
   /* btn=pannel->addLargeAction(courseCurveAct_);
    btn->setFixedSize(78,58);
    btn->setCheckable(true);*/
	//以上几行隐藏历程曲线 luo

    //btn->setPopupMode(QToolButton::InstantPopup);
    //courseCurveAct_->setMenu(CurveMenu);

    btn=pannel->addLargeAction(postPropAct_);
    btn->setFixedSize(78,58);

    SARibbonMenu* ConstantMenu = new SARibbonMenu(this);
    //ConstantMenu->addAction(TimHisCurveAct_);//(QIcon(":/images/folder.png"),tr("menu"));
   // ConstantMenu->addAction(DistanceCurveAct_);//QStringLiteral("1"));

    //以下几行隐藏常数 luo 
    /*btn=pannel->addLargeAction(constantAct_);
    btn->setFixedSize(78,58);
    btn->setCheckable(true);*/
	//以上几行隐藏常数 luo
	
    btn->setPopupMode(QToolButton::InstantPopup);
    constantAct_->setMenu(ConstantMenu);
    /*btn=pannel->addLargeAction(AppResultAct_);
    btn->setFixedSize(78,58);  
    btn=pannel->addLargeAction(AppRunAct_);
    btn->setFixedSize(78,58);*/

	//--部件显示隐藏
	btn=pannel->addLargeAction(partElSetHide_);
	btn->setFixedSize(78,58);

	btn=pannel->addLargeAction(gravity_);
	btn->setFixedSize(78,58);

	btn=pannel->addLargeAction(quantitystatistics_);
	btn->setFixedSize(78,58);
	
    pannel->setExpanding(0);
}
/*
描述：优化窗口创建
*/
void MainWindow::createCategoryOptim(SARibbonCategory* page)
{

}

/*
描述：窗口创建
*/
void MainWindow::createCategoryWindow(SARibbonCategory* page)
{
    SARibbonToolButton * btn;
    SARibbonPannel* pannel = page->addPannel(tr("窗口"));//(tr("Panel 1"));
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
描述：帮助窗口创建
*/
void MainWindow::createCategoryHelp(SARibbonCategory* page)
{
    SARibbonToolButton * btn;
    SARibbonPannel* pannel = page->addPannel(tr("帮助"));//(tr("Panel 1"));
    btn=pannel->addLargeAction(helpAct_);
    btn->setFixedSize(78,58);
	//以下几行隐藏查找与关于 luo
    /*btn=pannel->addLargeAction(helpLookForAct_);
    btn->setFixedSize(78,58);
    btn=pannel->addLargeAction(helpAboutAct_);
    btn->setFixedSize(78,58);*/
	//以上几行隐藏查找与关于 luo
}
//热等静压
void MainWindow::creatContextHIPWindow(SARibbonCategory* page)
{
    SARibbonToolButton * btn;
    SARibbonPannel* pannel = page->addPannel(tr("多孔介质"));//(tr("Panel 1"));
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
//热挤压
void MainWindow::creatContextHEWindow(SARibbonCategory* page)
{
    SARibbonToolButton * btn;
    SARibbonPannel* pannel = page->addPannel(tr("多孔介质"));//(tr("Panel 1"));
    btn=pannel->addLargeAction(HIPpartAct_);
    btn->setFixedSize(78,58);
    btn=pannel->addLargeAction(HIPBCAct_);
    btn->setFixedSize(78,58);
}

//锻造
void MainWindow::creatContextForgingWindow(SARibbonCategory* page)
{
	SARibbonToolButton * btn;
    SARibbonPannel* pannel = page->addPannel(tr("锻造"));//(tr("Panel 1"));
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

//铸造流程
void MainWindow::creatContextCastingWindow(SARibbonCategory* page)
{

	SARibbonToolButton * btn;
	SARibbonPannel* pannel = page->addPannel(tr("铸造"));//(tr("Panel 1"));
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

//热处理
void MainWindow::creatContextHPWindow(SARibbonCategory* page)
{
    SARibbonToolButton * btn;
    SARibbonPannel* pannel = page->addPannel(tr("热处理"));//(tr("Panel 1"));
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
	//以下几行打开软件上一次关闭的路径，并设置为当前路径
	QString str = QCoreApplication::applicationDirPath();
	str+="\\path\\WorkPatch.txt";
	QFile f(str);
	if(f.open(QIODevice::ReadOnly)){
		QByteArray t=f.readAll();
		QDir::setCurrent(QString(t));
	}
	

	//以下几行打开软件上一次关闭的路径，并设置为当前路径

	newAct_ = new QAction(QIcon(":/images/NewIcon.png"), tr("&New"), this);
	newAct_->setShortcuts(QKeySequence::New);
	newAct_->setStatusTip(tr("Create a new project"));
    //connect(newAct_,SIGNAL(triggered()),this,SLOT(NewSlot()));
    
	//文件
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
    LanguageUAct_=new QAction(tr("英文"),this);
    LanguageUAct_->setCheckable(true);
    connect(LanguageCAct_,SIGNAL(triggered()),this,SLOT(LanguageZHSlot()));//
    connect(LanguageUAct_,SIGNAL(triggered()),this,SLOT(LanguageENSlot()));//
    m_Lin =CLinguist::GetLinguistInstance();
    connect(m_Lin,SIGNAL(LanguageChanged(int)),this,SLOT(retranslateUi(int)));
	//-网格划分
	GridDivAct_= new QAction(QIcon(":/images/showon.png"), tr("GMesh"), this);
	GridDivActClose_= new QAction(QIcon(":/images/showoff.png"), tr("Close GMesh"), this);
    GridDivActReset_= new QAction(tr("重启GMesh"), this);
	GridDivActOpenGMesh_=new QAction(tr("打开GMesh"), this);

	//--接触与约束
	//TStyleAct_=new QAction(QIcon("/save.png"), tr("Contact Style"), this);
	//MainSectPrtyAct_=new QAction(QIcon("/save.png"), tr("Master-slave Plane"), this);
	//TPropertyAct_=new QAction(QIcon("/save.png"), tr("Contact Property"), this);
	//BondLimitAct_=new QAction(QIcon("/save.png"), tr("Binding Constraint"), this);
	//MPointLimitAct_=new QAction(QIcon("/save.png"), tr("Multipoint Constraint"), this);
	//--边界条件
	//PosEdgeAct_=new QAction(QIcon("/save.png"), tr("Position"), this);
	//PowEdgeAct_=new QAction(QIcon("/save.png"), tr("Force boundary"), this);
	//TmptEdgeAct_=new QAction(QIcon("/save.png"), tr("Temperature"), this);
	//EdgeIOAct_=new QAction(QIcon("/save.png"), tr("Boundary I/O"), this);
	//--求解设置
	//SolverChsAct_=new QAction(QIcon("/save.png"), tr("Solver Choose"), this);
	//GemoNolinearAct_=new QAction(QIcon("/save.png"), tr("Geometric Nonlinearity"), this);
	//SolverCtlParaAct_=new QAction(QIcon("/save.png"), tr("Solver Param"), this);
	//ResetAct_=new QAction(QIcon("/save.png"), tr("Reset"), this);
	//--输出
	//FrdFileAct_=new QAction(QIcon("/save.png"), tr("frd File Setting"), this);
	//DatFileAct_=new QAction(QIcon("/save.png"), tr("dat File Setting"), this);
	//MEdgeDataSaveAct_=new QAction(QIcon("/save.png"), tr("Material Boundary DataSave"), this);
    
	//--HELP
    helpAct_ = new QAction(QIcon(":/images/save.png"),tr("帮助"), this);
	helpAct_->setStatusTip(tr("help"));
	//以下几行隐藏查找与关于 luo
    /*helpLookForAct_ = new QAction(QIcon(":/images/save.png"),tr("Look For"), this);
    helpLookForAct_->setStatusTip(tr("Look For"));
    helpAboutAct_ = new QAction(QIcon(":/images/save.png"),tr("About"), this);
    helpAboutAct_->setStatusTip(tr("About"));*/
	//以上几行隐藏查找与关于 luo

	//--创建+/-xyz,+/-/auto zoom
    AddXAct_ = new QAction(QIcon(":/images/+x.png"),tr("+X"), this);
    AddXAct_->setStatusTip(tr("+X"));
    AddYAct_ = new QAction(QIcon(":/images/+y.png"),tr("+Y"), this);
    AddZAct_ = new QAction(QIcon(":/images/+z.png"),tr("+Z"), this);
    SubXAct_ = new QAction(QIcon(":/images/-x.PNG"),tr("-X"), this);
    SubYAct_ = new QAction(QIcon(":/images/-y.PNG"),tr("-Y"), this);
    SubZAct_ = new QAction(QIcon(":/images/-z.PNG"),tr("-Z"), this);
    ZomeAddAct_ = new QAction(QIcon(":/images/zoom+.png"),tr("放大"), this); 
    ZomeSubAct_ = new QAction(QIcon(":/images/zoom-.png"),tr("缩小"), this); 
    zoomFitAct_ = new QAction(QIcon(":/images/ResetZoom.png"),tr("重置"), this);

    //backgroud、axis
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

    //--级联、层叠、关闭所有、选择
    CascadeAct_ = new QAction(QIcon(":/images/save.png"), tr("级联"), this);
    TileAct_    = new QAction(QIcon(":/images/save.png"), tr("平铺"), this);
    CloseSubWindowAct_ =  new QAction(QIcon(":/images/save.png"), tr("关闭当前"), this);
    CloseAllSubWindowAct_  = new QAction(QIcon(":/images/save.png"), tr("关闭所有"), this);
    ChooseSubWindowAct_    = new QAction(QIcon(":/images/save.png"), tr("选择"), this);
   
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
    AppStartAct_ = new QAction(QIcon(":/images/MainStart.png"), tr("启动"), this);
    AppStartAct_->setCheckable(true);
    AppMeshAct_ = new QAction(QIcon(":/images/mesh.png"), tr("网格"), this);
    AppHIPAct_ = new QAction(QIcon(":/images/HIP.png"), tr("多孔介质"), this);

    AppHotExtrusionAct_ = new QAction(QIcon(":/images/HE.png"), tr("热挤压"), this);
    AppForgingAct_ = new QAction(QIcon(":/images/Forging.png"), tr("锻造"), this);
    AppHeatProcessAct_ = new QAction(QIcon(":/images/HP.png"), tr("热处理"), this);
	AppCastingAct_= new QAction(QIcon(":/images/casting.png"), tr("铸造"), this);//铸造
    AppResultAct_ = new QAction(QIcon(":/images/MainResult.png"), tr("结果"), this);
    AppRunAct_ = new QAction(QIcon(":/images/MainRun.png"), tr("运行"), this);
	

    connect(AppStartAct_, SIGNAL(triggered()), this, SLOT(AppStartDlgSlot()));
    connect(AppHIPAct_, SIGNAL(triggered(bool)), this, SLOT(AddInNewMenuHIPSlot(bool)));
    connect(AppHotExtrusionAct_, SIGNAL(triggered(bool)), this, SLOT(AddInNewMenuHESlot(bool)));
    connect(AppHeatProcessAct_,SIGNAL(triggered(bool)), this, SLOT(AddInNewMenuHPSlot(bool)));
    connect(AppRunAct_,SIGNAL(triggered()),this,SLOT(ForgingSubmissonActSlot()));//求解器运行(目前默认的为HIP求解器)
	connect(AppForgingAct_,SIGNAL(triggered(bool)), this, SLOT(AddInNewMenuForgingSlot(bool)));
	connect(AppCastingAct_,SIGNAL(triggered(bool)), this, SLOT(AddInNewMenuCastingSlot(bool)));//
    //--数据库
    MaterialLibAct_ = new QAction(QIcon(":/images/save.png"), tr("材料数据库"), this);
    ProcessLibAct_ = new QAction(QIcon(":/images/save.png"), tr("过程数据库"), this);

    //--菜单工具
    modelAssembleAct_= new QAction(QIcon(":/images/assembling.png"), tr("装配/缩放"), this);
    //modelScalerAct_= new QAction(QIcon(":/images/save.png"), tr("模型缩放"), this);
    disMeasureAct_= new QAction(QIcon(":/images/distance.png"), tr("距离测量"), this);
    createSetAct_= new QAction(QIcon(":/images/creatset.png"), tr("创建集合"), this);
    courseCurveAct_ = new QAction(QIcon(":/images/save.png"), tr("历程曲线"), this);
    constantAct_= new QAction(QIcon(":/images/save.png"), tr("常数"), this);
    //TimHisCurveAct_=new QAction(QIcon(":/images/save.png"), tr("Time"), this);
    //DistanceCurveAct_=new QAction(QIcon(":/images/save.png"), tr("Distance"), this);
    PUAct_=new QAction(QIcon(":/images/save.png"), tr("PU"), this);
    postPropAct_=new QAction(QIcon(":/images/attribute.png"), tr("后处理属性"), this);
	partElSetHide_=new QAction(QIcon(":/images/showhide.png"),tr("部件显示"),this);//部件显示隐藏
	gravity_=new QAction(QIcon(":/images/gravity.png"),tr("重力"),this);
	quantitystatistics_=new QAction(QIcon(":/images/Quantitystatistics.png"),tr("数量统计"),this);

    connect(modelAssembleAct_,SIGNAL(triggered()),this,SLOT(AssembleDlgSlot()));//添加装配
	connect(disMeasureAct_,SIGNAL(triggered()),this,SLOT(DistanceMeasurementSlot()));//距离测量
    connect(courseCurveAct_,SIGNAL(triggered()),this,SLOT(courseCurveDlgSlot()));//历程曲线
    connect(postPropAct_,SIGNAL(triggered()),this,SLOT(postPropDlgSlot()));//后处理属性
    connect(createSetAct_,SIGNAL(triggered()),this,SLOT(CreateSetDlgSlot()));//创建集合
	connect(partElSetHide_,SIGNAL(triggered()),this,SLOT(partElSetHideDlgSlot()));//部件显示隐藏；
	connect(gravity_,SIGNAL(triggered()),this,SLOT(GravitySlot()));//重力；
	connect(quantitystatistics_,SIGNAL(triggered()),this,SLOT(QStatisticsSlot()));//重力；

    //--热等静压
    HIPpartAct_ = new QAction(QIcon(":/images/part.png"), tr("部件"), this);
    HIPBCAct_ = new QAction(QIcon(":/images/BC.png"), tr("边界"), this);
    HIPInitAct_= new QAction(QIcon(":/images/init.png"), tr("初始化"), this);
    HIPSystemAct_ = new QAction(QIcon(":/images/systerm.png"), tr("制度"), this);
    HIPSolveAct_ = new QAction(QIcon(":/images/solverset.png"), tr("求解设置"), this);
    HIPCheckAct_ = new QAction(QIcon(":/images/save.png"), tr("检查"), this);
    HIPSubmissonAct_ = new QAction(QIcon(":/images/updatacalc.png"), tr("提交计算"), this);
    HIPVariableOutPutAct_ = new QAction(QIcon(":/images/check.png"), tr("变量输出"), this);
    HIPRunAct_= new QAction(QIcon(":/images/updatacalc.png"), tr("运行"), this);


    connect(HIPpartAct_,SIGNAL(triggered()),this,SLOT(HIPpartActSlot()));
    connect(HIPBCAct_,SIGNAL(triggered()),this,SLOT(HIPBCActSlot()));
    connect(HIPInitAct_,SIGNAL(triggered()),this,SLOT(HIPInitDlgActSlot()));
    connect(HIPSystemAct_,SIGNAL(triggered()),this,SLOT(HIPSystemDlgActSlot()));//热等静压制度
    connect(HIPSolveAct_,SIGNAL(triggered()),this,SLOT(HIPResolveDlgActSlot()));//求解器
    connect(HIPVariableOutPutAct_,SIGNAL(triggered()),this,SLOT(HIPVariableOutPutDlgSlot()));//变量输出
    //connect(HIPRunAct_,SIGNAL(triggered()),this,SLOT(HIPSolveActOpenSlot()));//求解器运行
	connect(HIPSubmissonAct_,SIGNAL(triggered()),this,SLOT(HIPSubmissonActSlot()));

    //--热处理
    HPpartAct_ = new QAction(QIcon(":/images/part.png"), tr("部件"), this);
    HPBCAct_ = new QAction(QIcon(":/images/BC.png"), tr("边界"), this);
    HPInitAct_= new QAction(QIcon(":/images/init.png"), tr("初始化"), this);
    HPSystemAct_ = new QAction(QIcon(":/images/systerm.png"), tr("热处理制度"), this);
    HPSloveAct_ = new QAction(QIcon(":/images/solverset.png"), tr("求解设置"), this);
    //HPCheckAct_ = new QAction(QIcon(":/images/check.png"), tr("检查"), this);
    HPSubmissonAct_ = new QAction(QIcon(":/images/updatacalc.png"), tr("提交计算"), this);

    connect(HPpartAct_,SIGNAL(triggered()),this,SLOT(HPpartActSlot()));
    connect(HPBCAct_,SIGNAL(triggered()),this,SLOT(HPBCActSlot()));
    connect(HPInitAct_,SIGNAL(triggered()),this,SLOT(HPInitActSlot()));
    connect(HPSystemAct_,SIGNAL(triggered()),this,SLOT(HPSystemActSlot()));
    connect(HPSloveAct_,SIGNAL(triggered()),this,SLOT(HPSloveSetActSlot()));
    connect(HPSubmissonAct_,SIGNAL(triggered()),this,SLOT(HPSubmissonActSlot()));
    //connect(HPSubmissonAct_,SIGNAL(triggered()),this,SLOT(HIPSolveActOpenSlot()));//求解器运行

	//锻造
	ForgingpartAct_=new QAction(QIcon(":/images/part.png"), tr("部件"), this);
    ForgingBCAct_=new QAction(QIcon(":/images/BC.png"), tr("边界"), this);
    ForgingContact_=new QAction(QIcon(":/images/contact.png"), tr("接触"), this);
    ForgingInitAct_=new QAction(QIcon(":/images/init.png"), tr("初始化"), this);
	ForgingTBAct_=new QAction(QIcon(":/images/huo.png"), tr("热边界"), this);
    ForgingSystemAct_=new QAction(QIcon(":/images/systerm.png"), tr("锻造制度"), this);
    ForgingSloveAct_=new QAction(QIcon(":/images/solverset.png"), tr("求解设置"), this);
    ForgingSubmissonAct_=new QAction(QIcon(":/images/updatacalc.png"), tr("提交计算"), this);

	connect(ForgingpartAct_,SIGNAL(triggered()),this,SLOT(ForgingpartActSlot()));
	connect(ForgingBCAct_,SIGNAL(triggered()),this,SLOT(ForgingBCActSlot()));
	connect(ForgingContact_,SIGNAL(triggered()),this,SLOT(ForgingContactActSlot()));
	connect(ForgingInitAct_,SIGNAL(triggered()),this,SLOT(ForgingInitActSlot()));
	connect(ForgingTBAct_,SIGNAL(triggered()),this,SLOT(ForgingHBSlot()));
	connect(ForgingSystemAct_,SIGNAL(triggered()),this,SLOT(ForgingSystemActSlot()));
	connect(ForgingSloveAct_,SIGNAL(triggered()),this,SLOT(ForgingSloveSetActSlot()));
    connect(ForgingSubmissonAct_,SIGNAL(triggered()),this,SLOT(ForgingSubmissonActSlot()));

	//铸造
	CastingGravityAct_=new QAction(QIcon(":/images/gravity.png"), tr("重力"), this);
	CastingpartAct_=new QAction(QIcon(":/images/part.png"), tr("部件"), this);
    CastingBCAct_=new QAction(QIcon(":/images/BC.png"), tr("边界"), this);
    CastingContact_=new QAction(QIcon(":/images/contact.png"), tr("接触"), this);
    CastingInitAct_=new QAction(QIcon(":/images/init.png"), tr("初始化"), this);
	CastingTBAct_=new QAction(QIcon(":/images/huo.png"), tr("热边界"), this);

	CastingMotionAct_=new QAction(QIcon(":/images/motionboundary.png"), tr("运动关系"), this);

    CastingSystemAct_=new QAction(QIcon(":/images/systerm.png"), tr("凝固制度"), this);
    CastingSloveAct_=new QAction(QIcon(":/images/solverset.png"), tr("求解设置"), this);
    CastingSubmissonAct_=new QAction(QIcon(":/images/updatacalc.png"), tr("提交计算"), this);

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
//    dockGeometry_ =new QDockWidget(tr("控制管理区域"));
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
//    //splitDockWidget(ProjectTree_,dockGeometry_,Qt::Horizontal);//切分窗口

    DockPreHIPPrc_ = new QDockWidget(tr("Pre"),this);
    areaProHIPPrc_ = new QScrollArea(DockPreHIPPrc_);
    ////-1.1祛除标题栏
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
	
	//-2.后处理
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
	tabifyDockWidget(DockPreHIPPrc_,DockPostPrc_);//放置順序

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
	//--信息
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
//创建工具栏
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
    //fileToolBar_->setOrientation(Qt::Horizontal);//设置位置为垂直

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
        openAct_->setText(tr("打开"));
        newAct_->setText(tr("新建"));
        saveAct_->setText(tr("保存"));
		WorkingPathAct_->setText(tr("工作路径"));
        mergeAct_->setText(tr("合并"));
        saveAsAct_->setText(tr("另存为"));
        importAct_->setText(tr("导入"));
        exportAct_->setText(tr("导出"));
        m_ChangeLanguageTitle->setTitle(tr("语言切换"));
        LanguageCAct_->setText(tr("切换中文"));
        LanguageUAct_->setText(tr("切换英文"));
        closeAct_->setText(tr("关闭"));
        printAct_->setText(tr("打印"));
        m_FilBtn->setText(tr(" 文件"));
        //--
        DockPostPrc_->setWindowTitle(QString::fromUtf8("后处理"));
        DockPreHIPPrc_->setWindowTitle(QString::fromUtf8("前处理"));
    }else{
        openAct_->setText(tr("Open"));
        newAct_->setText(tr("新建"));
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
        //m_contextCategory_HIP_C->setContextTitle("设置1");
        //m_contextCategoryHP->setContextTitle("HP");
        //m_contextCategory_HP_C->setContextTitle("设置2");
        //m_contextCategoryHE->setContextTitle("HE");
        //m_contextCategory_HE_C->setContextTitle("设置3");
        DockPostPrc_->setWindowTitle("Post");
        DockPreHIPPrc_->setWindowTitle("Pre");
    }

    UpDataRibbonLanguage(iLanguage);//启动Rabbion语言切换;
    //UpDataWidgetLanguage(iLanguage);//
    //if(!PostPro_){//后处理语言切换
    PostPro_->m_TreeModel->UpDataWidgetLanguage(iLanguage);//启动DockTree语言切换;
    if (PostPro_->m_PosWigReadResultDlg){
        PostPro_->m_PosWigReadResultDlg->TabPage1->LanguageUpData();
    }
    if (PostPro_->m_PosWigFile){
        PostPro_->m_PosWigFile->LanguageUpData();
    }
    
    //}
}
/*------------------------------------------
----------------Ribbon语言切换---------------
--------------------------------------------*/
void MainWindow::UpDataRibbonLanguage(int iLanguage)
{
    if (iLanguage == zh_cn){
        m_CategoryApp->changeWindowTitle(tr("应用"));//onWindowTitleChanged(tr("Application"));
        m_CategoryViewer->changeWindowTitle(tr("视区"));
        //m_CategoryDataBase->changeWindowTitle(tr("数据库"));//隐藏数据库 luo
        //m_CategoryLookOver->changeWindowTitle(tr("查看"));//隐藏查看 luo
        m_CategoryTool->changeWindowTitle(tr("工具"));
        //m_CategoryOPtim->changeWindowTitle(tr("优化"));//隐藏优化 luo
        //m_CategoryWindow->changeWindowTitle(tr("窗口"));//隐藏窗口 luo
        m_CategoryHelp->changeWindowTitle(tr("帮助"));
		if (m_contextCategory_Casting_C) m_contextCategory_Casting_C->changeWindowTitle(tr("设置0"));
        m_contextCategory_HIP_C->changeWindowTitle(tr("设置1"));
	        if (m_contextCategory_HE_C) m_contextCategory_HE_C->changeWindowTitle(tr("设置2"));
        m_contextCategory_HP_C->changeWindowTitle(tr("设置3"));


		if (m_contextCategoryCasting) {
		m_contextCategoryCasting->setContextTitle("铸造");
	        m_contextCategory_Casting_C->changeWindowTitle("设置0");
		}
	        m_contextCategoryHIP->setContextTitle("多孔介质");
	        m_contextCategory_HIP_C->changeWindowTitle("设置1");
	        m_contextCategoryHP->setContextTitle("热处理");
	        m_contextCategory_HP_C->changeWindowTitle("设置3");
	        if (m_contextCategoryHE) {
	        m_contextCategoryHE->setContextTitle("热挤压");
	        m_contextCategory_HE_C->changeWindowTitle("设置3");
	        }
		m_contextCategoryForging->setContextTitle("锻造");
	        m_contextCategory_Forging_C->changeWindowTitle("设置2");

        AppStartAct_->setText(tr("启动"));
        AppMeshAct_->setText(tr("网格"));
		AppCastingAct_->setText(tr("铸造"));
        AppHIPAct_->setText(tr("多孔介质"));
        AppHotExtrusionAct_->setText(tr("热挤压"));
        AppForgingAct_->setText(tr("锻造"));
        AppHeatProcessAct_->setText(tr("热处理"));
        AppResultAct_->setText(tr("结果"));
        AppRunAct_->setText(tr("运行"));

        //--
        HIPpartAct_->setText(tr("部件"));
    }else{
        m_CategoryApp->changeWindowTitle(tr("Application"));
        m_CategoryViewer->changeWindowTitle(tr("Viewer"));
        //m_CategoryDataBase->changeWindowTitle(tr("Database")); //隐藏数据库 luo
        m_CategoryLookOver->changeWindowTitle(tr("LookOver")); 
        m_CategoryTool->changeWindowTitle(tr("Tools"));
        //m_CategoryOPtim->changeWindowTitle(tr("Optimization"));//隐藏优化 luo
        //m_CategoryWindow->changeWindowTitle(tr("Windows"));
        m_CategoryHelp->changeWindowTitle(tr("Help"));


		if (m_contextCategoryCasting) {
		m_contextCategoryCasting->setContextTitle("Casting");
	        m_contextCategory_Casting_C->changeWindowTitle("Set0");
		}
	        m_contextCategoryHIP->setContextTitle("HIP");
	        m_contextCategory_HIP_C->changeWindowTitle("Set1");
	        m_contextCategoryHP->setContextTitle("HP");
	        m_contextCategory_HP_C->changeWindowTitle("Set3");
	        if (m_contextCategoryHE) {
	        m_contextCategoryHE->setContextTitle("HE");
	        m_contextCategory_HE_C->changeWindowTitle("Set3");
	        }
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
        //AppHotExtrusionAct_->setText(tr("HotExtrusion")); //热挤压被隐藏 luo
        AppForgingAct_->setText(tr("Forging"));
        AppHeatProcessAct_->setText(tr("HeatProcess"));
		

        AppResultAct_->setText(tr("Result"));
        AppRunAct_->setText(tr("Run"));
        //--
        HIPpartAct_->setText(tr("Parts"));
    }
    m_ribbon->hideContextCategory(m_contextCategoryHIP);
    if (m_contextCategoryHE) m_ribbon->hideContextCategory(m_contextCategoryHE);
    m_ribbon->hideContextCategory(m_contextCategoryHP);
}

void MainWindow::AppStartDlgSlot()
{
    m_AppStartDlg=new QAppStartDlg(this);
    m_AppStartDlg->show();
    m_AppStartDlg->raise();//最上层
    m_AppStartDlg->activateWindow();//激活
   
}
void MainWindow::AddInNewMenuHIPSlot(bool on)
{
    //--树状图显示
        iSetProStyle = ENUM_PRO_HIP;
        tabifyDockWidget(DockPostPrc_, DockPreHIPPrc_);//放置順序
   // if(on){
        if (m_contextCategoryCasting) m_ribbon->hideContextCategory(m_contextCategoryCasting);
		if (m_contextCategoryHE) m_ribbon->hideContextCategory(m_contextCategoryHE);
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
	    if (m_contextCategoryCasting) m_ribbon->hideContextCategory(m_contextCategoryCasting);
        m_ribbon->hideContextCategory(m_contextCategoryHIP);
        m_ribbon->hideContextCategory(m_contextCategoryHP);
		m_ribbon->hideContextCategory(m_contextCategoryForging);
        if (m_contextCategoryHE) m_ribbon->showContextCategory(m_contextCategoryHE);
		
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
		if (m_contextCategoryCasting) m_ribbon->hideContextCategory(m_contextCategoryCasting);
        m_ribbon->hideContextCategory(m_contextCategoryHIP);
        m_ribbon->hideContextCategory(m_contextCategoryHP);
        if (m_contextCategoryHE) m_ribbon->hideContextCategory(m_contextCategoryHE);
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
		if (m_contextCategoryCasting) m_ribbon->hideContextCategory(m_contextCategoryCasting);
        m_ribbon->hideContextCategory(m_contextCategoryHIP);
        if (m_contextCategoryHE) m_ribbon->hideContextCategory(m_contextCategoryHE);
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
	 if (m_contextCategoryHE) m_ribbon->hideContextCategory(m_contextCategoryHE);
	 m_ribbon->hideContextCategory(m_contextCategoryForging);
	 m_ribbon->hideContextCategory(m_contextCategoryHP);
	 if (m_contextCategoryCasting) m_ribbon->showContextCategory(m_contextCategoryCasting);
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
//--进行模型合并
void MainWindow::MergeSlot()
{
	statusBar()->showMessage(tr("合并..."));
	QString workPath=QDir::currentPath();
	//QString workPath=workPath2;
    //workPath = workPath.left(workPath.lastIndexOf("/"));
	QString filename = SafeGetOpenFileName(this, "Read file...", workPath, "Inp Files (*.inp )");
    if (filename.size()==0){
       return;
    }
	QFile file(filename);
    if(!file.exists()){
        return ;
    }
    if (!file.open(QIODevice::ReadOnly)){
        return ;
    }

	int NodeNumber=CRWObject.m_ReadInpResult.TmpNodeInpS.strData.size();
	int ElementNumber=CRWObject.m_ReadInpResult.TmpElInpS.strData.size();

	CRWManage CRWObject02;
	int ret=CRWObject02.ReadSectionInpFile02(&file,NodeNumber,ElementNumber,filename);
    file.close();
    if (!ret)return;
	//节点信息
	CRWObject.m_ReadInpResult.TmpNodeInpS.strData.append(CRWObject02.m_ReadInpResult.TmpNodeInpS.strData);
	//单元信息
	CRWObject.m_ReadInpResult.TmpElInpS.strData.append(CRWObject02.m_ReadInpResult.TmpElInpS.strData);
	CRWObject.m_ReadInpResult.TmpElInpS.ElementType.append(CRWObject02.m_ReadInpResult.TmpElInpS.ElementType);
    CRWObject.m_ReadInpResult.TmpElInpS.NumberE.append(CRWObject02.m_ReadInpResult.TmpElInpS.NumberE);
	//节点集合
	CRWObject.m_ReadInpResult.TmpNsetInps.strNSetName.append(CRWObject02.m_ReadInpResult.TmpNsetInps.strNSetName);
	CRWObject.m_ReadInpResult.TmpNsetInps.strNSetStyle.append(CRWObject02.m_ReadInpResult.TmpNsetInps.strNSetStyle);
	CRWObject.m_ReadInpResult.TmpNsetInps.strData.append(CRWObject02.m_ReadInpResult.TmpNsetInps.strData);
	//单元集合
	CRWObject.m_ReadInpResult.TmpElSetInps.strData.append(CRWObject02.m_ReadInpResult.TmpElSetInps.strData);
	CRWObject.m_ReadInpResult.TmpElSetInps.strElSetStyle.append(CRWObject02.m_ReadInpResult.TmpElSetInps.strElSetStyle);
	CRWObject.m_ReadInpResult.TmpElSetInps.strElSetName.append(CRWObject02.m_ReadInpResult.TmpElSetInps.strElSetName);
	//表面集合
	CRWObject.m_ReadInpResult.TmpSurfaceInps.strSurfaceName.append(CRWObject02.m_ReadInpResult.TmpSurfaceInps.strSurfaceName);
	CRWObject.m_ReadInpResult.TmpSurfaceInps.strSurfaceDataNum.append(CRWObject02.m_ReadInpResult.TmpSurfaceInps.strSurfaceDataNum);
	CRWObject.m_ReadInpResult.TmpSurfaceInps.strData.append(CRWObject02.m_ReadInpResult.TmpSurfaceInps.strData);
	//释放读入的inp文件内容
	CRWObject02.m_ReadInpResult.TmpNodeInpS.clear();
	CRWObject02.m_ReadInpResult.TmpElInpS.clear();
	CRWObject02.m_ReadInpResult.TmpNsetInps.clear();
	CRWObject02.m_ReadInpResult.TmpElSetInps.clear();
	CRWObject02.m_ReadInpResult.TmpSurfaceInps.clear();
    //写入数据到各个界面中
    ReadInpResultS m_Data = CRWObject.m_ReadInpResult;
	ImportTrace("OpenSlot: before Statistics");
		m_QStatisticsDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after Statistics");
    //--写入到HIP界面数据
    ImportTrace("OpenSlot: before HIPSystem");
        m_HIPSystemDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after HIPSystem");
    ImportTrace("OpenSlot: before VariableOutput");
        m_VariableOutputDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after VariableOutput");
    //--写入到HP界面数据
    ImportTrace("OpenSlot: before HpPart");
        m_HpPartDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after HpPart");
    ImportTrace("OpenSlot: before HpBC");
        m_HpBCDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after HpBC");
    ImportTrace("OpenSlot: before ForgingContact");
		m_ForgingContactDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after ForgingContact");
	ImportTrace("OpenSlot: before ThermalBoundary");
		m_ThermalBoundaryDlg->SetInpDataHB(m_Data);
        ImportTrace("OpenSlot: after ThermalBoundary");
	ImportTrace("OpenSlot: before ForgingSystem");
		m_ForgingSystemDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after ForgingSystem");
    ImportTrace("OpenSlot: before HpInit");
        m_HpInitDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after HpInit");
    ImportTrace("OpenSlot: before HpSystem");
        m_HpSystemDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after HpSystem");
    ImportTrace("OpenSlot: before HpSolveSet");
        m_HpSolveSetDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after HpSolveSet");

	//--write to QInpActorWidge
	ImportTrace("OpenSlot: before WidgetInpElset");
		m_WidgetInpElsetDlg->SetInpData(m_Data.TmpElSetInps);
        ImportTrace("OpenSlot: after WidgetInpElset");//
   	//写入到装配界面
	ImportTrace("OpenSlot: before Assembling");
		m_AssemblingAct_->SetInpData(m_Data);
        ImportTrace("OpenSlot: after Assembling");
    //--写入到Tree
    ImportTrace("OpenSlot: before TreeModel");
        PreHIPPro_->m_TreeModel->setInpData(m_Data);
        ImportTrace("OpenSlot: after TreeModel");
    //--写入到view显示区域
    viewWindow_->TabView(1);//->tabView_setCurrentIndex(1);
    QMdiSubWindow *CurWindow=m_MdiArea->currentSubWindow();
    for (int i=0;i<m_MdiArea->subWindowList().size();i++){
        QMdiSubWindow *TmpWindow=m_MdiArea->subWindowList().at(i);
        if (TmpWindow == TmpWindow){
            ImportTrace("OpenSlot: before ShowCurPreData");
					viewWindow_->ShowCurPreData(m_Data); 
                    ImportTrace("OpenSlot: after ShowCurPreData"); 
            break;
		}
	}
   Information_Widget::GetInstance()->ShowInformation(QString(filename+" Open Succeed!"));
}
void MainWindow::OpenSlot()
{
    // QLabel *qlabel = new QLabel("Hello Qt");
    //qlabel->show();
    statusBar()->showMessage(tr("打开..."));
	QString workPath=QDir::currentPath();
	//QString workPath=workPath2;
    //workPath = workPath.left(workPath.lastIndexOf("/"));
	QString filename;
    QByteArray autoInp = qgetenv("WEICME_AUTOINP");
    if (!autoInp.isEmpty()) {
        filename = QString::fromLocal8Bit(autoInp.constData());
        ImportTrace(QString("OpenSlot: auto file ") + filename);
    } else {
        filename = SafeGetOpenFileName(this, "Read file...", workPath, "Inp Files (*.inp );;Frd Files (*.Frd *.frd);;Dat Files (*.Dat *.dat)");
    }
    if (filename.size()==0){
       return;
    }
   
    QFile file(filename);
    if(!file.exists()){
        return ;
    }
    if (!file.open(QIODevice::ReadOnly)){
        return ;
    }
    QStringList lists = filename.split(".");
    QString fileFormat = lists.back().toLower();//frd文件
    if (fileFormat=="inp") {
       // CRWManage CRWObject;
        ImportTrace("OpenSlot: before ReadSectionInpFile");
        int ret=CRWObject.ReadSectionInpFile(&file,filename);
        ImportTrace("OpenSlot: after ReadSectionInpFile");
        file.close();
        if (!ret)return;
		ReadInpResultS m_Data = CRWObject.m_ReadInpResult;
        //写入数据到各个界面中
        //--写入到HIP界面数据
        //m_PartDlg->SetInpData(m_Data);
        //m_BCDlg->SetInpData(m_Data);
        //m_InitDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: before HIPSystem");
        m_HIPSystemDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after HIPSystem");
        //m_ResolveDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: before VariableOutput");
        m_VariableOutputDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after VariableOutput");
		ImportTrace("OpenSlot: before Statistics");
		m_QStatisticsDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after Statistics");
        //--写入到HP界面数据
        ImportTrace("OpenSlot: before HpPart");
        m_HpPartDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after HpPart");
        ImportTrace("OpenSlot: before HpBC");
        m_HpBCDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after HpBC");
		ImportTrace("OpenSlot: before ForgingContact");
		m_ForgingContactDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after ForgingContact");
		ImportTrace("OpenSlot: before ThermalBoundary");
		m_ThermalBoundaryDlg->SetInpDataHB(m_Data);
        ImportTrace("OpenSlot: after ThermalBoundary");
		ImportTrace("OpenSlot: before ForgingSystem");
		m_ForgingSystemDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after ForgingSystem");
        ImportTrace("OpenSlot: before HpInit");
        m_HpInitDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after HpInit");
		ImportTrace("OpenSlot: before Gravity");
		m_GravityAct_->SetInpData(m_Data);
        ImportTrace("OpenSlot: after Gravity");
        ImportTrace("OpenSlot: before HpSystem");
        m_HpSystemDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after HpSystem");
        ImportTrace("OpenSlot: before HpSolveSet");
        m_HpSolveSetDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after HpSolveSet");
		//写入到装配界面
		ImportTrace("OpenSlot: before Assembling");
		m_AssemblingAct_->SetInpData(m_Data);
        ImportTrace("OpenSlot: after Assembling");
		//工具->部件显示隐藏界面
		ImportTrace("OpenSlot: before WidgetInpElset");
		m_WidgetInpElsetDlg->SetInpData(m_Data.TmpElSetInps);
        ImportTrace("OpenSlot: after WidgetInpElset");
        //--写入到Tree
        ImportTrace("OpenSlot: before TreeModel");
        PreHIPPro_->m_TreeModel->setInpData(m_Data);
        ImportTrace("OpenSlot: after TreeModel");
		 //--写入到view显示区域
		
		if (viewWindow_){
			viewWindow_->TabView(1);//tabView_->setCurrentIndex(1);
			QMdiSubWindow *CurWindow=m_MdiArea->currentSubWindow();
			for (int i=0;i<m_MdiArea->subWindowList().size();i++){
				QMdiSubWindow *TmpWindow=m_MdiArea->subWindowList().at(i);
				if (TmpWindow == TmpWindow){
					ImportTrace("OpenSlot: before ShowCurPreData");
					viewWindow_->ShowCurPreData(m_Data); 
                    ImportTrace("OpenSlot: after ShowCurPreData"); 
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
    }
    statusBar()->showMessage(tr("Ready")); 
    //occGeomImExport::FileFormat format;  
}
void MainWindow::ViewAss( ReadInpResultS m_Data)
{
	ImportTrace("OpenSlot: before TreeModel");
        PreHIPPro_->m_TreeModel->setInpData(m_Data);
        ImportTrace("OpenSlot: after TreeModel");
    if (viewWindow_){
        viewWindow_->TabView(1);//tabView_->setCurrentIndex(1);
        QMdiSubWindow *CurWindow=m_MdiArea->currentSubWindow();
        for (int i=0;i<m_MdiArea->subWindowList().size();i++){
             QMdiSubWindow *TmpWindow=m_MdiArea->subWindowList().at(i);
              if (TmpWindow == TmpWindow){
                  ImportTrace("OpenSlot: before ShowCurPreData");
					viewWindow_->ShowCurPreData(m_Data); 
                    ImportTrace("OpenSlot: after ShowCurPreData"); 
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
	if(CheStr=="*Node"&&Nsize<=0)"错误:缺少节点信息";
	Nsize=m_HpPartDlg->m_NodeElSetData.ELInpData.strELType.size();
	Nsize1=m_HpPartDlg->m_NodeElSetData.ELInpData.strData.size();
	Nsize2=m_HpPartDlg->m_NodeElSetData.ELInpData.NumberE.size();
	Nsize3=m_HpPartDlg->m_NodeElSetData.ELInpData.ElementType.size();
	for(int i=0;i<Nsize2;i++){
		Nsize4+=m_HpPartDlg->m_NodeElSetData.ELInpData.NumberE.at(i);
	}
	if(Nsize>0&&Nsize1<=0||Nsize2!=Nsize3||Nsize1!=Nsize4)"错误:缺少单元信息";
	Nsize=m_HpBCDlg->m_nodeBCList.size();
	for(int i=0;i<Nsize;i++){
         CheStr=m_HpBCDlg->m_nodeBCList.at(i).strBoundaryName;
         Nsize1=m_HpBCDlg->m_nodeBCList.at(i).strPSetName.size();
		 Nsize2=m_HpBCDlg->m_nodeBCList.at(i).strURstyle.size();
		 if(CheStr=="Boundary"&&Nsize1<=0||CheStr=="Boundary"&&Nsize2<=0)"警告:缺少部分边界条件，有可能会造成计算无法进行";
	}
	Nsize=m_HpSolveSetDlg->m_nodeVarList.size();
	for(int i=0;i<Nsize;i++){
		CheStr=m_HpSolveSetDlg->m_nodeVarList.at(i).strELInclude;
		CheStr1=m_HpSolveSetDlg->m_nodeVarList.at(i).strNInclude;
		CheStr2=m_HpSolveSetDlg->m_nodeVarList.at(i).strTimeOrFreqName;
		CheStr3=m_HpSolveSetDlg->m_nodeVarList.at(i).strTname;
		if(CheStr!=""&&CheStr1!="")"警告:缺少结果的输出";
		if(CheStr2=="TIME POINTS"&&CheStr3=="")"警告:结果变量按照时间间隔输出时，缺少必要的时间间隔信息";
	}
	Nsize=m_HpSystemDlg->m_nodeHPSystemRList.size();
	Nsize1=m_HpSystemDlg->m_nodeHPSystemFList.size();
	Nsize2=m_HpSolveSetDlg->m_nodeHPPhyList.size();
	CheStr=m_HpSolveSetDlg->m_nodeHPPhyList.at(0).strStyle;
	CheStr1=m_HpSolveSetDlg->m_nodeHPPhyList.at(0).strAbsZero;
	CheStr2=m_HpSolveSetDlg->m_nodeHPPhyList.at(0).strStefanBoltzman;
	if(Nsize>0||Nsize1>0&&Nsize2<=0||CheStr=="")"错误:计算模型中包含对流/辐射边界信息，但物理常数尚未指定";
	if(Nsize>0||Nsize1>0&&CheStr1=="")"错误:计算模型中包含对流/辐射边界信息，但尚未指定绝对零度";
	if(Nsize>0||Nsize1>0&&CheStr2=="")"错误:计算模型中包含对流/辐射边界信息，但尚未指定波尔兹曼常数";
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
    //QMessageBox::critical(this,QString("错误"),QString("保存..."));
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
    CRWObject.WriteUniformLoadInpFile(&file,m_GravityAct_->m_OutputGravity_InfS,m_HpPartDlg->m_nodeSList);////均布荷载
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

//*-导入部件文件
void MainWindow::ImportPartSlot()
{
    statusBar()->showMessage(tr("导入部件..."));
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
    //QStringList lists = filename.split(".");
    //QString fileFormat = lists.back();
    //导入部件
    QString strName,strPartName,strfileFormat;
    QStringList listName = filename.split("/");
    strName = listName.back();
    strfileFormat  = strName.right((strName.lastIndexOf(".")));
    strPartName = strName.left((strName.lastIndexOf(".")));
    if(PreHIPPro_)
    {
        PreHIPPro_->InsertPart(strPartName);
    }

}
//*-导出部件文件
void MainWindow::ExportPartSlot()
{

}
void MainWindow::ClearSlot()
{}

void MainWindow::closeEvent(QCloseEvent *ev)
{
    QMessageBox::StandardButton bt = QMessageBox::question(this, tr("关闭"), tr("你想退出吗？"), QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);
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
//--更改工作路径
void MainWindow::ChangeWorkPath()
{  
	QString workPath=QFileDialog::getExistingDirectory(this,tr("工作路径"),"");
	if (workPath.size()==0)
	{
		return;
	}
	
	if (QDir(workPath).exists())
	{
		QDir::setCurrent(workPath);
		Information_Widget::GetInstance()->ShowInformation("已设置工作路径为：");
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
	//QByteArray ba= exeName.toLatin1();
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
    //QString &cmd = QString("%1//gmsh-4.3.0-Windows64//gmsh.exe").arg(str);
    QString cmd = str+"/gmsh";
    GmshProcess= new QProcess(this);
    GmshProcess->start(cmd,QStringList()<<"t1.geo");
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
    QByteArray ba= exeName.toLatin1();
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
  // str+="../gmsh/gmsh.sh";
   workpath+="/gmsh/gmsh.sh";
  // qDebug() <<workpath;
#endif
    QProcess *RMshProcess= new QProcess(this);
    RMshProcess->start(workpath);
   // RMshProcess->start(str);
}

void MainWindow::ViewDirectionChangedSlot(QString direction)
{
    // int indexPage=m_MdiArea->subWindowList().count()-1;
     QMdiSubWindow *tmp =m_MdiArea->currentSubWindow();
    if (std::find(m_MdiArea->subWindowList().begin(), m_MdiArea->subWindowList().end(), tmp) ==
        m_MdiArea->subWindowList().end()){  
         int indexPage=m_MdiArea->subWindowList().count()-1;
    }else{//找到
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
        }else{//找到
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
        }else{//找到
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
        }else{//找到
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
    }else{//找到
        int indexPage=m_MdiArea->subWindowList().count();
        int tt = viewWindow_->tabView_->currentIndex();
        QMyVTK::GetInstance(tt)->ViewChange(factor);
    }
}
void MainWindow::HideShowSlot(bool on)
{
    m_ribbon->setHideMode(on);
}
//级联
void MainWindow::CascadeSlot()
{
    m_MdiArea->cascadeSubWindows();//级联方式
}
//平铺
void MainWindow::TileSlot()
{
    m_MdiArea->tileSubWindows();//平铺方式
}
//关闭所有子窗口
void MainWindow::CloseAllSubWindowSlot()
{
    m_MdiArea->closeAllSubWindows();//
}
//关闭当前
void MainWindow::CloseSubWindowSlot()
{
    int indexPage=m_MdiArea->subWindowList().count();
    if (indexPage>1)
    {
        m_MdiArea->closeActiveSubWindow();//
    }
}
//选择
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

void MainWindow::HideProcessDialogs()
{
    if (m_HpPartDlg) m_HpPartDlg->hide();
    if (m_HpBCDlg) m_HpBCDlg->hide();
    if (m_ForgingContactDlg) m_ForgingContactDlg->hide();
    if (m_HpInitDlg) m_HpInitDlg->hide();
    if (m_ThermalBoundaryDlg) m_ThermalBoundaryDlg->hide();
    if (m_ForgingSystemDlg) m_ForgingSystemDlg->hide();
    if (m_HpSystemDlg) m_HpSystemDlg->hide();
    if (m_HIPSystemDlg) m_HIPSystemDlg->hide();
    if (m_HpSolveSetDlg) m_HpSolveSetDlg->hide();
    if (m_HpSubmissionDlg) m_HpSubmissionDlg->hide();
    if (m_ForgingSubmissionDlg) m_ForgingSubmissionDlg->hide();
    if (m_HipSubmissionDlg) m_HipSubmissionDlg->hide();
}

void MainWindow::ShowProcessDialog(QDialog *dlg)
{
    if (!dlg) return;
    HideProcessDialogs();
    dlg->show();
    dlg->raise();
    dlg->activateWindow();
}

//创建集合
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
    ShowProcessDialog(m_HpPartDlg);
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
	ShowProcessDialog(m_HpBCDlg);
}

void MainWindow::HPBCActSlot02()
{
	if (viewWindow_){//pre 
		viewWindow_->TabView(1);
	}
	if (iSetProStyle != ENUM_PRO_Forging){
		m_HpBCDlg->ShowBC(0);
        m_HIPBcBtn->setEnabled(true);
		ShowProcessDialog(m_HpBCDlg);
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
	    ShowProcessDialog(m_HpInitDlg);
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
	ShowProcessDialog(m_HpInitDlg);
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
//--HPDLG  求解设置1上一步
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
//求解制度
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
//热处理制度
void MainWindow::HPSystemActSlot()
{
    ShowProcessDialog(m_HpSystemDlg);
    
    if (iSetProStyle==ENUM_PRO_HIP){
        m_HIPSystemBtn->setEnabled(true);
    }else if (iSetProStyle==ENUM_PRO_HP){
        m_HPSystemBtn->setEnabled(true);
    }else if (iSetProStyle==ENUM_PRO_Forging){
        m_ForingSystemBtn->setEnabled(true);
    }      
}
//--HPDLG求解器设置1（）
void MainWindow::HPSloveSetActSlot()
{
    m_HpSolveSetDlg->SetOutPutTab(0);
    ShowProcessDialog(m_HpSolveSetDlg);
    m_HPSolveSetBtn->setEnabled(true);
}
//--HPDLG求解器设置2（下一步）
void MainWindow::HPSystemNextBtnSlot(int istep)
{
    m_HpSolveSetDlg->SetSloveStep(istep);
    m_HpSolveSetDlg->SetOutPutTab(0);
    ShowProcessDialog(m_HpSolveSetDlg);
    m_HPSolveSetBtn->setEnabled(true);
}
//--HPDLG
void MainWindow::HPSubmissonActSlot()
{ 
    ShowProcessDialog(m_HpSubmissionDlg);
}
void MainWindow::HPSolveNextActSlot()
{
    if (m_HpSolveSetDlg->m_tabView->currentIndex()==0){
        m_HpSolveSetDlg->SetOutPutTab(1);
        ShowProcessDialog(m_HpSolveSetDlg);
        //m_HPSolveSetBtn->setEnabled(true);
        return;
    }
    ShowProcessDialog(m_HpSubmissionDlg);
}
//--HipDLG
//部件对话框
//
void MainWindow::HIPpartActSlot()
{
    //m_PartDlg->show(); 
    //m_PartDlg->raise();//最上层
    //m_PartDlg->activateWindow();//激活
    m_HpPartDlg->ShowPartDlgStyle(ENUM_PRO_HIP);
    ShowProcessDialog(m_HpPartDlg);
}
//--HipDlg
//边界对话框
void MainWindow::HIPBCActSlot()
{
    m_HpBCDlg->ShowBC(ENUM_PRO_HIP);
	ShowProcessDialog(m_HpBCDlg); 
    m_HIPBcBtn->setEnabled(true);
}
//--HipDlg
//边界对话框
void MainWindow::HIPInitDlgActSlot()
{
    //m_InitDlg->show(); 
    //m_InitDlg->raise();//最上层
    //m_InitDlg->activateWindow();//激活
    ShowProcessDialog(m_HpInitDlg); 
    m_HIPInitBtn->setEnabled(true);
}
//--HIPDLG
//热等静压制度
void MainWindow::HIPSystemDlgActSlot()
{
    ShowProcessDialog(m_HIPSystemDlg); 
    m_HIPSystemDlg->raise();//最上层
    m_HIPSystemDlg->activateWindow();//激活
    m_HIPSystemBtn->setEnabled(true);
}
//--HIPDLG
//求解器设置对话框
void MainWindow::HIPResolveDlgActSlot()
{
    m_HpSolveSetDlg->SetOutPutTab(0);
    ShowProcessDialog(m_HpSolveSetDlg); 
    m_HIPSolveSetBtn->setEnabled(true);
}
//--HIPDLG
//变量输出对话框
void MainWindow::HIPVariableOutPutDlgSlot()
{   
    m_VariableOutputDlg->show(); 
    m_VariableOutputDlg->raise();//最上层
    m_VariableOutputDlg->activateWindow();//激活
}
//--HIPDLG
//-提交运算
void MainWindow::HIPSubmissonActSlot()
{
	//m_HipSubmissionDlg->show();
	//m_HipSubmissionDlg->raise();////最上层
	//m_HipSubmissionDlg->activateWindow();//激活

	ShowProcessDialog(m_HpSubmissionDlg);
}

//--工具1.模型装配
void MainWindow::AssembleDlgSlot()
{
	m_AssemblingAct_->show();
	m_AssemblingAct_->raise();
	m_AssemblingAct_->activateWindow();
}
//--工具2.历程曲线
void MainWindow::courseCurveDlgSlot()
{
    PostPro_->xyplotPanel_->show();
    PostPro_->xyplotPanel_->raise();//最上层
    PostPro_->xyplotPanel_->activateWindow();//激活
} 
//--工具3.后处理属性
void MainWindow::postPropDlgSlot()
{
    PostPro_->m_PostWidPlotOptDlg->show(); 
    PostPro_->m_PostWidPlotOptDlg->raise();//最上层
    PostPro_->m_PostWidPlotOptDlg->activateWindow();//激活
}
//--工具4.部件显示隐藏
void MainWindow::partElSetHideDlgSlot()
{
	m_WidgetInpElsetDlg->show();
	m_WidgetInpElsetDlg->raise();
	m_WidgetInpElsetDlg->activateWindow();
}

//--工具5.距离测量
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
//求解器调用：打开求解器运算
//cmd中：两个路径1.求解器路径;
//2.Inp路径；

void MainWindow::HIPSolveActOpenSlot()
{
    QString workPath = QDir::currentPath();
    QString filename = SafeGetOpenFileName(this, "Read Inp file...", workPath, "Inp Files (*.inp);;");
    if (filename.size() == 0) {
        return;
    }

    QFile parseFile(filename);
    if (parseFile.exists() && parseFile.open(QIODevice::ReadOnly)) {
        int parseRet = CRWObject.ReadSectionInpFile(&parseFile, filename);
        parseFile.close();
        if (parseRet) {
            ReadInpResultS m_Data = CRWObject.m_ReadInpResult;
            ImportTrace("OpenSlot: before HIPSystem");
        m_HIPSystemDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after HIPSystem");
            ImportTrace("OpenSlot: before VariableOutput");
        m_VariableOutputDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after VariableOutput");
            ImportTrace("OpenSlot: before Statistics");
		m_QStatisticsDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after Statistics");
            ImportTrace("OpenSlot: before HpPart");
        m_HpPartDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after HpPart");
            ImportTrace("OpenSlot: before HpBC");
        m_HpBCDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after HpBC");
            ImportTrace("OpenSlot: before ForgingContact");
		m_ForgingContactDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after ForgingContact");
            ImportTrace("OpenSlot: before ThermalBoundary");
		m_ThermalBoundaryDlg->SetInpDataHB(m_Data);
        ImportTrace("OpenSlot: after ThermalBoundary");
            ImportTrace("OpenSlot: before ForgingSystem");
		m_ForgingSystemDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after ForgingSystem");
            ImportTrace("OpenSlot: before HpInit");
        m_HpInitDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after HpInit");
            ImportTrace("OpenSlot: before Gravity");
		m_GravityAct_->SetInpData(m_Data);
        ImportTrace("OpenSlot: after Gravity");
            ImportTrace("OpenSlot: before HpSystem");
        m_HpSystemDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after HpSystem");
            ImportTrace("OpenSlot: before HpSolveSet");
        m_HpSolveSetDlg->SetInpData(m_Data);
        ImportTrace("OpenSlot: after HpSolveSet");
            ImportTrace("OpenSlot: before Assembling");
		m_AssemblingAct_->SetInpData(m_Data);
        ImportTrace("OpenSlot: after Assembling");
            ImportTrace("OpenSlot: before WidgetInpElset");
		m_WidgetInpElsetDlg->SetInpData(m_Data.TmpElSetInps);
        ImportTrace("OpenSlot: after WidgetInpElset");
            ImportTrace("OpenSlot: before TreeModel");
        PreHIPPro_->m_TreeModel->setInpData(m_Data);
        ImportTrace("OpenSlot: after TreeModel");
            if (viewWindow_) {
                viewWindow_->TabView(1);
                ImportTrace("OpenSlot: before ShowCurPreData");
					viewWindow_->ShowCurPreData(m_Data); 
                    ImportTrace("OpenSlot: after ShowCurPreData");
            }
            Information_Widget::GetInstance()->ShowInformation(QString("Loaded INP data: ") + filename);
        }
    }

    QFileInfo inputInfo(filename);
    QString appDir = QCoreApplication::applicationDirPath();
    QDir appRoot(appDir);
    QString allRoot = appRoot.absolutePath();
    if (allRoot.endsWith("/gui", Qt::CaseInsensitive)) {
        allRoot.chop(4);
    }

    QStringList solverCandidates;
#if _MSC_VER
    solverCandidates << QDir(allRoot).absoluteFilePath("solver/AESim-FM.exe")
                     << QDir(allRoot).absoluteFilePath("Solver/WeICME.exe");
#else
    solverCandidates << QDir(allRoot).absoluteFilePath("solver/AESim-FM")
                     << QDir(allRoot).absoluteFilePath("Solver/WeICME");
#endif

    QString solverPath;
    for (int i = 0; i < solverCandidates.size(); ++i) {
        QFileInfo candidate(solverCandidates.at(i));
        if (candidate.exists() && candidate.isFile() && candidate.isExecutable()) {
            solverPath = candidate.absoluteFilePath();
            break;
        }
    }

    if (solverPath.isEmpty()) {
        Information_Widget::GetInstance()->ShowInformation("Solver binary not found. Checked: " + solverCandidates.join("; "));
        return;
    }

    QFileInfo solverInfo(solverPath);
    m_SloverProcess = new QProcess(this);
    m_SloverProcess->setWorkingDirectory(inputInfo.absolutePath());
    QProcessEnvironment env02 = QProcessEnvironment::systemEnvironment();
    QString CPUNumber = m_HpSubmissionDlg->m_EditCalcNum->text();
    env02.insert("OMP_NUM_THREADS", CPUNumber);
    m_SloverProcess->setProcessEnvironment(env02);
    this->connect(m_SloverProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(ShowStdOutput()));
    this->connect(m_SloverProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(ShowStdOutput()));

    QString solverArg = inputInfo.completeBaseName();
    Information_Widget::GetInstance()->ShowInformation("Start solver: " + solverPath);
    Information_Widget::GetInstance()->ShowInformation("Working directory: " + inputInfo.absolutePath());
    Information_Widget::GetInstance()->ShowInformation("Input job: " + solverArg);
    m_SloverProcess->start(solverPath, QStringList() << "-i" << solverArg);
    if (!m_SloverProcess->waitForStarted(3000)) {
        Information_Widget::GetInstance()->ShowInformation("Failed to start solver: " + solverPath);
    }
}
void MainWindow::HIPSolveActKillSlot()
{
	if(m_SloverProcess!=NULL){
		m_SloverProcess->kill();
       Information_Widget::GetInstance()->ShowInformation("终止计算");
	}
}
//锻造
void MainWindow::ForgingpartActSlot()
{
	m_HpPartDlg->ShowPartDlgStyle(ENUM_PRO_Forging);
    ShowProcessDialog(m_HpPartDlg);
}

void MainWindow::ForgingBCActSlot()
{
	m_HpBCDlg->ShowBC(ENUM_PRO_Forging);
	ShowProcessDialog(m_HpBCDlg);
	m_ForingBcBtn->setEnabled(true);
}
//接触对话框
void MainWindow::ForgingContactActSlot()
{
	m_ForgingContactDlg->ShowContact(ENUM_PRO_Forging);
    ShowProcessDialog(m_ForgingContactDlg); 
	m_ForingContactBtn->setEnabled(true);
}
//初始化
void MainWindow::ForgingInitActSlot()
{
	ShowProcessDialog(m_HpInitDlg); 
	m_ForingInitBtn->setEnabled(true);
}
void MainWindow::ForgingHBSlot()
{
	ShowProcessDialog(m_ThermalBoundaryDlg);
	m_ForingTBBtn->setEnabled(true);
}
//锻造制度
void MainWindow::ForgingSystemActSlot()
{
	m_ForgingSystemDlg->ShowFS3(ENUM_PRO_Forging); 
    ShowProcessDialog(m_ForgingSystemDlg); 
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
//锻造制度的下一步
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
    ShowProcessDialog(m_HpSolveSetDlg);
    m_HPSolveSetBtn->setEnabled(true);
}

//求解器设置对话框
void MainWindow::ForgingSloveSetActSlot()
{
	//m_HpSolveSetDlg->SetOutPutTab(0);
    ShowProcessDialog(m_HpSolveSetDlg); 
	m_ForingSolveSetBtn->setEnabled(true);
}

//-提交运算
void MainWindow::ForgingSubmissonActSlot()
{
	//m_ForgingSubmissionDlg->show();
	//m_ForgingSubmissionDlg->raise();////最上层
	//m_ForgingSubmissionDlg->activateWindow();//激活
	ShowProcessDialog(m_HpSubmissionDlg);
}
//凝固流程
void MainWindow::CastingGravitySlot()
{
	m_CastingGravityDlg->ShowHideSlot(1);
    m_CastingGravityDlg->show(); 
    m_CastingGravityDlg->raise();//最上层
    m_CastingGravityDlg->activateWindow();//激活
	m_CastingGravityDlg->setEnabled(true);
}
void MainWindow::CastingpartActSlot()
{
	m_HpPartDlg->ShowPartDlgStyle(0);
    m_HpPartDlg->show();
    m_HpPartDlg->raise();////最上层
    m_HpPartDlg->activateWindow();//激活
	m_CastingPartBtn->setEnabled(true);
}

void MainWindow::CastingBCActSlot()
{
	m_HpBCDlg->ShowBC(2);
	m_HpBCDlg->show();
    m_HpBCDlg->raise();////最上层
    m_HpBCDlg->activateWindow();//激活
	m_CastingBcBtn->setEnabled(true);
}
void MainWindow::CastingContactActSlot()
{
	m_ForgingContactDlg->ShowContact(ENUM_PRO_Casting);
	m_ForgingContactDlg->show(); 
	m_ForgingContactDlg->raise();//最上层
	m_ForgingContactDlg->activateWindow();//激活
	m_CastingContactBtn->setEnabled(true);
}
void MainWindow::CastingInitActSlot()
{
	m_HpInitDlg->show();
    m_HpInitDlg->raise();//最上层
    m_HpInitDlg->activateWindow();//激活
	m_CastingInitBtn->setEnabled(true);
}
void MainWindow::CastingHBSlot()
{
	m_ThermalBoundaryDlg->show();
	m_ThermalBoundaryDlg->raise();//最上层
    m_ThermalBoundaryDlg->activateWindow();//激活
	m_CastingTBBtn->setEnabled(true);
}
void MainWindow::CastingMotionBoundarySlot()
{
	m_ForgingSystemDlg->ShowFS3(ENUM_PRO_Casting);
	m_ForgingSystemDlg->show(); 
    m_ForgingSystemDlg->raise();//最上层
    m_ForgingSystemDlg->activateWindow();//激活
	m_CastingMotionBtn->setEnabled(true);
}
void MainWindow::CastingSystemActSlot()
{
	m_ForgingSystemDlg->ShowFS3(ENUM_PRO_Casting);
	m_ForgingSystemDlg->show(); 
    m_ForgingSystemDlg->raise();//最上层
    m_ForgingSystemDlg->activateWindow();//激活
	m_CastingMotionBtn->setEnabled(true);
	//m_CastingSystemBtn->setEnabled(true);
    
}
void MainWindow::CastingSystemNextBtnSlot(int istep)
{
	m_HpSolveSetDlg->SetSloveStep(istep);
    m_HpSolveSetDlg->SetOutPutTab(0);
    m_HpSolveSetDlg->show();
    m_HpSolveSetDlg->raise();////最上层
    m_HpSolveSetDlg->activateWindow();//激活
    m_HPSolveSetBtn->setEnabled(true);
}
void MainWindow::CastingSloveSetActSlot()
{
	m_HpSolveSetDlg->show(); 
    m_HpSolveSetDlg->raise();//最上层
    m_HpSolveSetDlg->activateWindow();//激活
	m_CastingSolveSetBtn->setEnabled(true);
}
void MainWindow::CastingSubmissonActSlot()
{
	m_HpSubmissionDlg->show();
    m_HpSubmissionDlg->raise();////最上层
    m_HpSubmissionDlg->activateWindow();//激活
}

//--临时添加
void MainWindow::ShowStdOutput()
{
    QByteArray data = m_SloverProcess->readAllStandardOutput();
    QString str(data);
    Information_Widget::GetInstance()->ShowInformation(str);
}
//--StepPlay播放条1
void MainWindow::updataStepPlayCombox(ResultOutputS ResultO)
{
    if (viewWindow_){
        viewWindow_->TabView(0);//tabView_->setCurrentIndex(0);
    }
   m_StepPlayWidget->updataCombox(ResultO);
}
//--StepPlay播放条2(widgeresultout->emit to updata)
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
//--StepPlay播放条3
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




void MainWindow::GmshProcessErrorSlot()
{
    Information_Widget::GetInstance()->ShowInformation("Gmsh process error occurred");
}

void MainWindow::SolverProcessErrorSlot()
{
    Information_Widget::GetInstance()->ShowInformation("solver process error occurred");
}
