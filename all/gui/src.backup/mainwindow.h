#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#ifdef _MSC_VER
#include <windows.h>
#endif
#include <iostream>
#include <QMainWindow>
#include <QProcess>
#include <QTime>//luo
#include <qmath.h>//luo
#include <QToolBar>
#include <QDockWidget>
#include <QScrollArea>
#include <QToolBox>
#include <QStatusBar>
#include <QApplication>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QCloseEvent>
#include <QSignalMapper>
#include "WindowView.h"
#include "PreProcess_Panel.h"
#include "QPostPrc.h"
#include "clinguist.h"
#include "Information_Widget.h"
#include "qappstartdlg.h"
#include "qmdichild.h"
#include "Assembling.h"//luo
#include "DistMeasurement.h"//luo
#include "PElSfSetDlg.h"

#include "QHPPartDlg.h"
#include "QHPBCDlg.h"
#include "QHPInitDlg.h"
#include "QHPSystemDlg.h"
#include "QHPSolveSetDlg.h"
#include "QHPSubmissionDlg.h"
//#include "QPartDlg.h"
//#include "QBCDlg.h"
//#include "QInitDlg.h"
#include "QHIPsystemDlg.h"
//#include "QResolveDlg.h"
#include "QVariableOutputDlg.h"
#include "QHIPSubmissionDlg.h"
#include "QForgingContactDlg.h"
#include "QForgingSystemDlg.h"
#include "QForgingSubmissionDlg.h"
#include "QThermalBoundaryDlg.h"//luo
#include "QInpActorWidge.h"
#include "Gravity.h"
#include "QStatistics.h"
//--Ribbon--
#include "SARibbonMainWindow.h"
#include <QAbstractButton>
#include "SARibbonBar.h"
#include "SARibbonCategory.h"
#include "SARibbonPannel.h"
#include "SARibbonToolButton.h"
#include "SARibbonMenu.h"
#include "SARibbonComboBox.h"
#include "SARibbonLineEdit.h"
#include "SARibbonGallery.h"
#include "SARibbonCheckBox.h"
#include "SARibbonQuickAccessBar.h"
#include "SARibbonButtonGroupWidget.h"
//#include "Information_return.h"
#include "appkey.h"//************************
//class MainWindow : public QMainWindow


enum{
ENUM_PRO_Casting=0,//铸造
ENUM_PRO_HIP=1,//热等静压
ENUM_PRO_Forging=2,//锻造
ENUM_PRO_HP=3,//热处理
};
class MainWindow : public SARibbonMainWindow
{
	Q_OBJECT

public:
     MainWindow(QWidget *parent=NULL);
    //MainWindow(QWidget *parent=NULL, Qt::WFlags flag = 0);
	~MainWindow();
     virtual void closeEvent(QCloseEvent *ev);
	 virtual void mouseReleaseEvent(QMouseEvent * ev);
public:
	
//protected:
    //--Ribbon
    //--Ribbon main
    SARibbonBar* m_ribbon;
    SARibbonCategory *m_CategoryApp;//启动
    SARibbonCategory *m_CategoryViewer;//视区
    SARibbonCategory *m_CategoryDataBase;//数据库
    SARibbonCategory *m_CategoryLookOver;//查看
    SARibbonCategory *m_CategoryTool;//工具
    SARibbonCategory *m_CategoryOPtim;//优化
    SARibbonCategory *m_CategoryWindow;//窗口
    SARibbonCategory *m_CategoryHelp;//帮助

    QAbstractButton  *m_FilBtn;
    QPushButton      *m_PushFileBtn;
    
    QMdichild *m_MdiChild;
    QMdiArea *m_MdiArea;
    //--文件菜单中子菜单
    QMenu *m_ChangeLanguageTitle;//
    //--Action
	QAction* AddXAct_;
	QAction* AddYAct_;
	QAction* AddZAct_;
	QAction* SubXAct_;
	QAction* SubYAct_;
	QAction* SubZAct_;
	QAction* ZomeAddAct_;
	QAction* ZomeSubAct_;
    QAction* zoomFitAct_;
    QAction* bgAct_;
    QAction* axisAct_;
	//1.-文件
	QAction* newAct_;
	QAction* openAct_;
	QAction* saveAct_;
	QAction* WorkingPathAct_;
    QAction* mergeAct_;
	QAction* saveAsAct_;
	QAction* importAct_;
	QAction* exportAct_;
	QAction* printAct_;
	QAction* closeAct_;
    QAction* LanguageCAct_;
    QAction* LanguageUAct_;

	//-网格划分
	QAction* GridDivAct_;
	QAction* GridDivActClose_;
    QAction* GridDivActReset_;
	QAction* GridDivActOpenGMesh_;
    //工具界面
    //--模型缩放、距离测量、
    QAction* modelAssembleAct_;//模型装配
	//QAction* modelScalerAct_;//模型缩放
    QAction* disMeasureAct_;//距离测量
    QAction* createSetAct_;//创建集合
    QAction* courseCurveAct_;//历程曲线
      //QAction* TimHisCurveAct_;//时间历史曲线
      //QAction* DistanceCurveAct_;//位置历程曲线
    QAction* postPropAct_;//后处理属性
    QAction* constantAct_;//常数
	QAction* PUAct_;//
	QAction* partElSetHide_;//部件显示隐藏
	QAction* gravity_;//重力
	QAction* quantitystatistics_;//统计

	//--接触与约束
	//QAction* TStyleAct_;
	//QAction* MainSectPrtyAct_;
	//QAction* TPropertyAct_;
	//QAction* BondLimitAct_;
	//QAction* MPointLimitAct_;
	////--边界条件
	//QAction* PosEdgeAct_;
	//QAction* PowEdgeAct_;
	//QAction* TmptEdgeAct_;
	//QAction* EdgeIOAct_;
	////--求解设置
	//QAction* SolverChsAct_;
	//QAction* GemoNolinearAct_;
	//QAction* SolverCtlParaAct_;
	//QAction* ResetAct_;
	//--输出
	//QAction* FrdFileAct_;
	//QAction* DatFileAct_;
	//QAction* MEdgeDataSaveAct_;
	//--help
	QAction* helpAct_;
    QAction* helpLookForAct_;
    QAction* helpAboutAct_;
	//2.Gmsh
	QAction* showOnPntAct_;
	QAction* showOffPntAct_;

    //--HidorShowRibbon
    QAction* HideShowAct_;
    //--窗口
    QAction* CascadeAct_;//级联
    QAction* TileAct_;//平铺方式
    QAction* CloseSubWindowAct_;//关闭子窗口
    QAction* CloseAllSubWindowAct_;//关闭所有子窗口
    QAction* ChooseSubWindowAct_;//选择子窗口

    //--数据库
    QAction* MaterialLibAct_;
    QAction* ProcessLibAct_;

    //--热等静压
    QAction* HIPpartAct_;//部件
    QAction* HIPBCAct_;//边界
    QAction* HIPInitAct_;//初始化
    QAction* HIPSystemAct_;//热等静压制度
    QAction* HIPSolveAct_;//求解设置
    QAction* HIPCheckAct_;//检查
    QAction* HIPSubmissonAct_;//提交计算
    QAction* HIPVariableOutPutAct_;//变量输出
    QAction* HIPRunAct_;//运行

	//锻造
    QAction* ForgingpartAct_;//部件
    QAction* ForgingBCAct_;//边界
    QAction* ForgingContact_;//接触
    QAction* ForgingInitAct_;//初始化
	QAction* ForgingTBAct_;//热边界
    QAction* ForgingSystemAct_;//锻造制度
    QAction* ForgingSloveAct_;//求解设置
    QAction* ForgingCheckAct_;//检查
    QAction* ForgingSubmissonAct_;//提交计算

	//凝固
    QAction* CastingGravityAct_;//重力
	QAction*  CastingpartAct_;//部件
    QAction* CastingBCAct_;//边界
    QAction* CastingContact_;//接触
    QAction* CastingInitAct_;//初始化
	QAction* CastingTBAct_;//热边界
	QAction* CastingMotionAct_;//运动关系
    QAction* CastingSystemAct_;//凝固制度
    QAction* CastingSloveAct_;//求解设置
    QAction* CastingCheckAct_;//检查
    QAction* CastingSubmissonAct_;//提交计算


    //--热挤压

    //--热处理
    QAction* HPpartAct_;//部件
    QAction* HPBCAct_;//热边界
    QAction* HPInitAct_;//初始化
    QAction* HPSystemAct_;//热处理制度
    QAction* HPSloveAct_;//求解设置
    QAction* HPCheckAct_;//检查
    QAction* HPSubmissonAct_;//提交计算
   

	//
	QToolBar* fileToolBar_;
	QToolBar* typeToolBar_;
	QToolBar* viewToolBar_;
	QToolBar* workPathToolBar_;
    QToolBar* postStepBar_;
    QToolBar* commonBar_;

    QStepPlayWidget *m_StepPlayWidget;
	QDockWidget* dockGeometry_;
	QScrollArea* areaGeometry_;
	//PageGeometry* pageGeometry_;
	//WindowView* viewWindow_;

	QDockWidget* dockInfomation_;
	QScrollArea* areaInfomation_;
	Information_Widget* pageInfomation_;
	//--
	//QTreeMangerDock*  ProjectTree_;
	//QTreeMangerView*   ProjectTreeView_;
	QStandardItemModel* ProjrctTreeModel_;
	//--工具栏
	QLabel*			labelWorkPath_;
	QLineEdit*		editWorkPath_;
	//QToolButton*	buttonWorkPath_;
	QPushButton *buttonWorkPath_;
	Assembling *m_AssemblingAct_;//luo
	DistMeasurement *m_DistMeasurementAct_;
	Gravity *m_GravityAct_;
    //--Language
    CLinguist *m_Lin;
    QWindowView*	viewWindow_;
	//-1.前处理
	/*QFrowardPrc*	DockFrowardPrc_;
	QScrollArea*	areaFrowardPrc_;
	QLabel*			statusMsgLabel_;
	*/

    QDockWidget*	DockPreHIPPrc_;
	QScrollArea*	areaProHIPPrc_;
    QPreHIPPrc*	PreHIPPro_;


	//-2.后处理
	QDockWidget*	DockPostPrc_;
	QScrollArea*	areaPostPrc_;
	QPostPrc*		PostPro_;

	//--gmsh
	int		GmshRcH;
	int		GmshRcW;
	QString workPath1;
	QDockWidget *GmshWiget ;
	QProcess    *GmshProcess;
    QProcess    *m_SloverProcess;
//--DLG
    PElSfSetDlg* m_PElSfSetDlg;
    //QPartDlg* m_PartDlg;
    //QBCDlg* m_BCDlg;
    //QInitDlg* m_InitDlg;
    QHIPsystemDlg* m_HIPSystemDlg;
    //QResolveDlg* m_ResolveDlg;	
    QVariableOutputDlg* m_VariableOutputDlg;
	QHIPSubmissionDlg* m_HipSubmissionDlg;

	QInpActorWidge* m_WidgetInpElsetDlg;

	//QSectionProp*	SectionProDlg;
    QAppStartDlg* m_AppStartDlg;//
  //  QHIPPartDlg*    m_HIPpartDlg;
//--HP DLG
     QHPPartDlg *m_HpPartDlg;
     QHPBCDlg   *m_HpBCDlg;
     QHPInitDlg *m_HpInitDlg;
     QHPSystemDlg *m_HpSystemDlg;
     QHPSolveSetDlg *m_HpSolveSetDlg;
     QHPSubmissionDlg *m_HpSubmissionDlg;
	 QStatistics *m_QStatisticsDlg;

//--Forging DLG 
      QForgingContactDlg *m_ForgingContactDlg;
      QForgingSystemDlg  *m_ForgingSystemDlg;
      QForgingSubmissionDlg *m_ForgingSubmissionDlg;
	  ThermalBoundary *m_ThermalBoundaryDlg;//热交换
//Casting DLG
	  Gravity *m_CastingGravityDlg;

      int iSetProStyle;//工艺过程设置类别（HIP,HP...）

//--启动、网格、热等静压、热挤压、锻造、热处理、结果、运行
    QAction* AppStartAct_;
    QAction* AppMeshAct_;
    QAction* AppHIPAct_;//热等静压 Hot Isostatic Pressing(1)
    QAction* AppHotExtrusionAct_;//热挤压 Hot extrusion
    QAction* AppForgingAct_;//锻造
    QAction* AppHeatProcessAct_;//热处理 Heat Treatment(1)
    QAction* AppCastingAct_;//铸造
    QAction* AppResultAct_;//结果
    QAction* AppRunAct_;//运行
    
    SARibbonToolButton *m_HIPBtn,*m_HEBtn,*m_HPBtn;
    SARibbonToolButton *m_HIPPartBtn,*m_HIPBcBtn,*m_HIPInitBtn,*m_HIPSystemBtn,*m_HIPSolveSetBtn;
    SARibbonToolButton *m_HPPartBtn,*m_HPBcBtn,*m_HPInitBtn,*m_HPSystemBtn,*m_HPSolveSetBtn;
	SARibbonToolButton *m_ForingPartBtn,*m_ForingBcBtn,*m_ForingContactBtn,*m_ForingInitBtn;
	SARibbonToolButton *m_ForingTBBtn,*m_ForingSystemBtn,*m_ForingSolveSetBtn,*m_GravityBtn;

	SARibbonToolButton *m_CastingPartBtn,*m_CastingBcBtn,*m_CastingContactBtn,*m_CastingInitBtn;
	SARibbonToolButton *m_CastingTBBtn,*m_CastingMotionBtn,*m_CastingSystemBtn,*m_CastingSolveSetBtn;


    SARibbonContextCategory *m_contextCategoryHIP,*m_contextCategoryHE,*m_contextCategoryHP,*m_contextCategoryForging;
	SARibbonContextCategory *m_contextCategoryCasting;

    SARibbonCategory  *m_contextCategory_HIP_C,*m_contextCategory_HE_C,*m_contextCategory_HP_C,*m_contextCategory_Forging_C;
    SARibbonCategory  *m_contextCategory_Casting_C;
//---
    ResultOutputS m_ResultO;
private slots:
    void NewSlot();
	void OpenSlot();
	void SaveSlot();
	void ClearSlot();
	void GridDivActPrc();
	void GridDivActPrcClose();
    void GridDivActPrcReset();
	void ChangeWorkPath();
    void HIPSolveActOpenSlot();//HIP 运行按钮open
	void InpCheckSlot();
	void GridDivActPrcOpen();
	void HIPSolveActKillSlot();
private:
	void InitDlg();
	void createActions();
	void createDockWindows();
	//void createContextMenu();
	void createToolBars();
	void createStatusBar();
	void ToolApp();//luo
    void InitProcess();
    void InitTree();

    //--ribbon
    void creatRibbon();
    void createCategoryApp(SARibbonCategory* page);
    void createCategoryView(SARibbonCategory* page);
    void createCategoryDateBS(SARibbonCategory* page);
    void createCategoryLookOver(SARibbonCategory* page);
    void createCategoryTool(SARibbonCategory* page);
    void createCategoryOptim(SARibbonCategory* page);
    void createCategoryWindow(SARibbonCategory* page);
    void createCategoryHelp(SARibbonCategory* page);
    void creatContextHIPWindow(SARibbonCategory* page);
    void creatContextHEWindow(SARibbonCategory* page);
    void creatContextHPWindow(SARibbonCategory* page);
	void creatContextForgingWindow(SARibbonCategory* page);
	void creatContextCastingWindow(SARibbonCategory* page);
    void creatFileMenu();

    //--
	void writeSetting ();
	bool okToContinue();

	void CreatePreProcessPanel();

	QString strippedName(const QString &fullFileName);

	void treeViewClick( const QModelIndex& index );
    void UpDataRibbonLanguage(int iLanguage);
    //public Q_SLOTS:
	//  void SetWindowTitle();
public slots:
	 void ViewDirectionChangedSlot(QString direction);
     void ViewSizeChangedSlot(int factor);
     void ViewChangedSlot(int factor);
     void LanguageZHSlot();//中
     void LanguageENSlot();//英
     void retranslateUi(int iLanguage);
     void AddInNewMenuHIPSlot(bool);
     void AddInNewMenuHESlot(bool);
     void AddInNewMenuHPSlot(bool on);
	 void AddInNewMenuForgingSlot(bool);
	 void AddInNewMenuCastingSlot(bool);

     void AppStartDlgSlot();
     void HideShowSlot(bool);
     void TileSlot();
     void CascadeSlot();
     void CloseAllSubWindowSlot();
     void CloseSubWindowSlot();
     void ChooseSubWindowSlot();
    //--
     void ImportPartSlot();
     void ExportPartSlot();
     void HIPpartActSlot();
     void HIPBCActSlot();
     void HIPInitDlgActSlot();
     void HIPSystemDlgActSlot();
     void HIPResolveDlgActSlot();
     void HIPVariableOutPutDlgSlot();
	 void HIPSubmissonActSlot();
	 void MainSaveHIPInpSlot();
     void NowActivateWindowSlot(QMdiSubWindow* SubWind);
     //--hp
     void HPpartActSlot();
     void HPBCActSlot();
     void HPInitActSlot();
     void HPSystemActSlot();
     void HPSloveSetActSlot();
     void HPSubmissonActSlot();
     void MainSaveHPInpSlot();
     void ShowStdOutput();
	     void GmshProcessErrorSlot();
	     void SolverProcessErrorSlot();
     void updataStepPlayCombox(ResultOutputS ResultO);
     void updataStepPlayParam(ResultVisS visParam);
     void PlayStepPlay(StepPlayVisS stepParam);
     void HPSystemNextBtnSlot(int istep);
     void HP_HIPSystemActSlot();
     void HP_SolveSetPreChsSlot();
     void HP_HIPPartActSlot();
     void HPSolveNextActSlot();

	 //Forging
	 void ForgingpartActSlot();
     void ForgingBCActSlot();
	 void ForgingContactActSlot();
     void ForgingInitActSlot();
	 void HPInitActSlot0();
	 void HPInitActSlot02();
	 void ForgingHBSlot();
	 void HPBCActSlot02();
     void ForgingSystemActSlot();
	 void ForgingSystemActSlot0();
     void ForgingSystemActSlot2(m_TBName);
	 void ForgingSystemActSlot3(QMap<int,QString>);
	 void MainSaveForgingInpSlot();
     void ForgingSloveSetActSlot();
     void ForgingSubmissonActSlot();
	 void ForgingSystemNextBtnSlot0(int istep);
	 void ForgingSystemNextBtnSlot(int istep);

	 //Casting
	 void CastingGravitySlot();
	 void CastingpartActSlot();
	 void CastingBCActSlot();
	 void CastingContactActSlot();
	 void CastingInitActSlot();
	 void CastingHBSlot();
	 void CastingMotionBoundarySlot();
	 void CastingSystemActSlot();
	 void CastingSystemNextBtnSlot(int);
	 void CastingSloveSetActSlot();
	 void CastingSubmissonActSlot();

     //--工具-模型装配
     void AssembleDlgSlot();
     void courseCurveDlgSlot();
     void MergeSlot();
     void postPropDlgSlot();
     void CreateSetDlgSlot();
	 void partElSetHideDlgSlot();
	 void DistanceMeasurementSlot();
	 void AssembleDlgViewSlot(AssemblingS_ZP);
	 void GravitySlot();
	 void QStatisticsSlot();
	 void changeS(bool f);
public:
	CRWManage CRWObject;
	void ViewAss( ReadInpResultS m_Data);
	
};

#endif // MAINWINDOW_H
