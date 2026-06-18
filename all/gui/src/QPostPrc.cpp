#include "./PostProcess/XYPlot_Panel.h"
#include "QPostPrc.h"
#include "Information_Widget.h"
#include <QFileInfo>
#include <QTimer>

static void ShowPostDialogInFront(QDialog *dlg, QWidget *owner)
{
    if (!dlg) return;
    QWidget *parent = owner ? owner->window() : 0;
    if (parent && dlg->parentWidget() != parent) {
        dlg->setParent(parent, Qt::Dialog);
    }
    dlg->setWindowModality(Qt::ApplicationModal);
    dlg->show();
    dlg->setFocus(Qt::ActiveWindowFocusReason);
    dlg->raise();
    dlg->activateWindow();
    QTimer::singleShot(0, dlg, SLOT(raise()));
    QTimer::singleShot(50, dlg, SLOT(raise()));
    QTimer::singleShot(100, dlg, SLOT(activateWindow()));
}


QPostPrc::QPostPrc(QWidget *parent)
	: QWidget(parent)
{
    m_PostWidPlotOptDlg = NULL;
	m_PosWigReadResultDlg = NULL;
    m_FileScalarName = "";
    m_ActorListData.clear();
	//QString FilePathName;
	//FilePathName=QCoreApplication::applicationDirPath()+PLOTOPTNAME;
	//LoadConfigFile(FilePathName);
   
}

QPostPrc::~QPostPrc()
{
    if (m_PostWidPlotOptDlg!=NULL)
    {
        delete m_PostWidPlotOptDlg;
        m_PostWidPlotOptDlg = NULL;
    }
	if (m_PosWigReadResultDlg!=NULL)
	{
		delete m_PosWigReadResultDlg;
		m_PosWigReadResultDlg = NULL;
	}
    m_ActorListData.clear();
}

void QPostPrc::setStyle()
{
    m_TreeModel = new QTree_Model(this,M_TREE_POST);
    m_TreeView = new QTreeMangerView(m_TreeModel,this);

    m_TreeView->header()->hide();
    m_TreeView->setAutoScroll(true);
    m_TreeView->setGeometry(0,0,500,500);
    m_TreeView->setModel(m_TreeModel);
    m_TreeView->expandToDepth(3);

    m_PosWigFile = new QPostWigFile(this,&m_WHColor);
    m_PostWigResultOut = new QPostWigResultOutput(this);
    xyplotPanel_ = new XYPlot_Panel(this);
    m_StackedWidget = new QStackedWidget(this);
    m_StackedWidget->addWidget(m_PosWigFile);
    m_StackedWidget->addWidget(m_PostWigResultOut);
    m_StackedWidget->addWidget(xyplotPanel_);

    QVBoxLayout *MainLayout = new QVBoxLayout(this);
    MainLayout->addWidget(m_TreeView);
    MainLayout->addWidget(m_StackedWidget);
    MainLayout->addStretch();
    this->setLayout(MainLayout);
    this->setMinimumWidth(300);

    //m_TreeModel->item(0,0)->child(0,0)->setEnabled(false);
	connect(m_TreeView,SIGNAL(EmitChangeModelIndex(const QModelIndex &)),this,
		SLOT(ChangeModelIndexSlot(const QModelIndex &)));
	//connect(m_PosWigFile,SIGNAL(emitDataOk()),this,SLOT(ShowTheTreeItemSlot()));
    connect(m_PosWigFile,SIGNAL(emitDataMenu(const map<QString, QStringList> *)),this,SLOT(UpDataMenuListSlot(const map<QString, QStringList> *)));		
    connect(m_PosWigFile,SIGNAL(frdDataOk(FrdDataVIS*)),xyplotPanel_,SLOT(InitPlotData(FrdDataVIS*)));
    connect(m_PosWigFile,SIGNAL(DatOk(resultDatS)),xyplotPanel_,SLOT(InitPlotDatData(resultDatS)));

    connect(m_PosWigFile,SIGNAL(frdDataOk(FrdDataVIS*)),m_PostWigResultOut,SLOT(InitSectionDataSlot(FrdDataVIS*)));
    
    connect(m_PostWigResultOut,SIGNAL(emitScalar(ResultVisS)),this,SLOT(UpDataScalar(ResultVisS)));
    connect(m_PosWigFile->m_ListActorWiget,SIGNAL(emitClickAcotrListWig(vector<int>)),this,SLOT(SetActorSlot(vector<int>)));
    //--鍓栭潰
    //connect(m_PostWigResultOut,SIGNAL(emitSectionAdd(int)),this,SLOT(SectionCutAddSlot(int)));
    //--init 
    m_CfgFile=new QCfgFileManage();
    m_CfgFile->LoadConfigFile();
    m_optDlgs=m_CfgFile->m_optDlgs;
    m_WHColor=ChangeColorToDouble(m_optDlgs.ClrEdgesWHPlot);
    m_FSColor=ChangeColorToDouble(m_optDlgs.ClrEdgesFSPlot);
    m_CSColor=ChangeColorToDouble(m_optDlgs.ClrEdgesCSPLot);
    m_HisPointClr=ChangeColorToDouble(m_optDlgs.ClrHisCuvPoint);
    m_PointSizeFactor=m_optDlgs.ClrHisPointfactor.toDouble();
    if (m_PostWidPlotOptDlg == NULL){
        m_PostWidPlotOptDlg = new QPostWidPlotOptDlg(this);	
        m_PostWidPlotOptDlg->SetData(m_optDlgs);
        connect(m_PostWidPlotOptDlg,SIGNAL(emitParam(OptDlgS)),this,SLOT(UpDataVisSlot(OptDlgS)));
    }

    m_readReDlg = m_CfgFile->m_readRltDlgs;
    if (m_PosWigReadResultDlg == NULL){
        m_PosWigReadResultDlg = new QPostWigReadResultDlg(this);
        m_PosWigReadResultDlg->SetData(m_readReDlg);
        connect(m_PosWigReadResultDlg,SIGNAL(emitReadResultParam(ReadResultDlgS)),this,SLOT(UpVTKTextVisSlot(ReadResultDlgS)));
    }
    UpVTKTextVisSlot(m_readReDlg);
    m_PosWigFile->frdVIS_.SetColorSize(m_PointSizeFactor,m_HisPointClr);
    xyplotPanel_->InitPoint(m_PointSizeFactor, m_HisPointClr);//init xyplot point & color
}

//****************************************************
// Method:    鍗曞嚮鏍戣妭鐐圭殑鍝嶅簲;
// Returns:   void;
// Parameter: index     鏍戣妭鐐?
// Author:	  
//****************************************************
void QPostPrc::ChangeModelIndexSlot( const QModelIndex & index )
{  
  //  Render_Widget *renWidget = Render_Widget::GetInstance();
    QString type = index.data(Qt::UserRole+3).toString();
    if (type == "PlotOpt")
    {
        ShowPostDialogInFront(m_PostWidPlotOptDlg, this);
    }
    else if (type == "DatafrdOpt")//
    {
        m_StackedWidget->setCurrentWidget(m_PosWigFile);
    }
	else if (type == "ReadRest")//
	{
		//m_StackedWidget->setCurrentWidget(m_PosWigReadResults);
	 
	   ShowPostDialogInFront(m_PosWigReadResultDlg, this);
	}
	else if (type == "PlotRest")
	{
		m_StackedWidget->setCurrentWidget(m_PostWigResultOut);
		//m_PostWigResultOut->setMenu();
	}
	else if (type == "ListRest")
	{
		//m_StackedWidget->setCurrentWidget(m_PosWigFile);鎵撳紑褰撳墠Frd鏂囦欢
	}
	else if (type == "Anim")
	{
		m_StackedWidget->setCurrentWidget(m_PosWigFile);
	}
	else if (type == "TimHisCur")
	{
		m_StackedWidget->setCurrentWidget(xyplotPanel_);
	}
	else if (type == "RMesh")
	{
		//m_StackedWidget->setCurrentWidget(m_PosWigFile);
        #ifdef _MSC_VER
		QString &str = QCoreApplication::applicationDirPath(); 
		QString &cmd = QString("%1//RMesh/RMesh.exe").arg(str);
		QProcess *RMshProcess= new QProcess;
		RMshProcess->start(cmd);
        #else
		QString cmd = "/mnt/d/WeICME/WeICMECAE/RMesh/RMesh.exe";
		QProcess *RMshProcess= new QProcess(this);
		RMshProcess->setProcessChannelMode(QProcess::ForwardedChannels);
		bool ok = RMshProcess->startDetached(cmd);
		if (!ok) {
			QMessageBox::warning(0, "RMesh", "Failed to launch: " + cmd);
		}
        #endif
	}
	else if (type == "Other")
	{
		//m_StackedWidget->setCurrentWidget(m_PosWigFile);
	}
    currentTreeNodeType_ = type;
}

/*
	鏁版嵁閫夋嫨鏄剧ず
*/
void QPostPrc::UpDataMenuListSlot(const map<QString, QStringList> *varMap)
{
    /*QString scalar("L102-DISP:D1");
    m_PosWigFile->frdVIS_.SetContourVisible(scalar,true);
    m_PosWigFile->frdVIS_.SetContourLevel(20);
    m_PosWigFile->frdVIS_.SetContourType(scalar,2);
    m_PosWigFile->frdVIS_.SetContourVariable(scalar);
    m_PosWigFile->frdVIS_.SetContourOpacity(0.5);
    m_PosWigFile->frdVIS_.Update();*/
	if(varMap->size()<=0)return;
    QString header(varMap->begin()->first);
    QString headerName(header.split("-").at(0));
    QStringList vars1; 
    QString PreHeader;//L101;
    ResultOutputS m_ResultO;
    const QVector<QString> *OnlyHead = m_PosWigFile->frdVIS_.GetScalarVectorInfo();
    map<QString, QStringList>::const_iterator cit = varMap->begin();
    for (int kk=0; kk<OnlyHead->size(); kk++) {
        QString cmpHead=OnlyHead->at(kk);

        for (cit; cit != varMap->end(); ++cit){
            QString curHeader=cit->first.split("-").at(0);
            if (curHeader==cmpHead){
                if ( curHeader!= PreHeader){  
                    m_ResultO.m_Menu.m_MenuName<<curHeader;
                   /* if (cit!=varMap->begin()){
                        m_ResultO.m_Menu.m_subMenuName.push_back(vars1);
                    } */  
                    PreHeader = curHeader;
                    //vars1.clear();//vars2.clear();
                }
                vars1<<cit->first.split("-").at(1);
                m_ResultO.m_Menu.m_sub2MenuName.push_back(cit->second) ;  
            }
        }// for (cit; cit != varMap->end(); ++cit)

        if (cit==varMap->end()){
            m_ResultO.m_Menu.m_subMenuName.push_back(vars1);
            vars1.clear();
        } 
        cit = varMap->begin();
    }
    vector<int> previousActorList = m_ActorListData;
    vector<int> allActorList;
    vector<int>::const_iterator it1=m_PosWigFile->frdVIS_.GetGridIds()->begin();//鑾峰彇鎵€鏈塈dList
    for (it1;it1!=m_PosWigFile->frdVIS_.GetGridIds()->end();it1++){ 
       allActorList.push_back(*it1);
    }
    m_WholeActorData=allActorList;
    m_ActorListData.clear();
    if (previousActorList.empty()) {
        m_ActorListData = allActorList;
    } else {
        vector<int>::const_iterator oldIt=previousActorList.begin();
        for (oldIt;oldIt!=previousActorList.end();oldIt++){
            if (std::find(allActorList.begin(),allActorList.end(),*oldIt)!=allActorList.end()) {
                m_ActorListData.push_back(*oldIt);
            }
        }
    }
    emit emitStepPlayMenu(m_ResultO);//涓荤晫闈㈡挱鏀炬潯    
}
/*----------------------------------------------
	  Depend on Plot vtktext鈥榮 Param to Set the VIS. 
-------------------------------------------------*/
void QPostPrc::UpVTKTextVisSlot(ReadResultDlgS vtkTextProp)
{
    int index=vtkTextProp.RadioTextIndex;
   
    switch(index){
        case 0:
            m_PosWigFile->frdTextVIS_.SetVisible(false);
            break;
        default:m_PosWigFile->frdTextVIS_.SetVisible(true);
            break;
    }
    m_readReDlg=vtkTextProp;
    if (m_FileScalarName!=""){
        m_PosWigFile->frdTextVIS_.ShowText(m_PosWigFile->frdVIS_.GetText(m_FileScalarName),m_readReDlg.RadioTextIndex); 
    }
    m_PosWigFile->frdTextVIS_.Update();
}
/*----------------------------------------------
	  Depend on Plot Option鈥榮 Param to Set the VIS. 
-------------------------------------------------*/

void QPostPrc::UpDataVisSlot(OptDlgS OptDlg)
{
    if (m_FileScalarName !=""){
        m_PosWigFile->frdVIS_.SetContourVisible(1,m_FileScalarName,false);
    }
  
    m_optDlgs = OptDlg;
    //m_ActorListData=m_PosWigFile->frdVIS_.GetGridIds();//鑾峰彇鎵€鏈塈dList
    SetOptionS(m_optDlgs,&m_ActorListData);
    //VTKColorS tmpWHColor,tmpFSColor,tmpCSColor;
}
void QPostPrc::SetOptionS(OptDlgS OptDlg,const vector<int>* idS)
{
    m_WHColor=ChangeColorToDouble(OptDlg.ClrEdgesWHPlot);
    m_FSColor=ChangeColorToDouble(OptDlg.ClrEdgesFSPlot);
    m_CSColor=ChangeColorToDouble(OptDlg.ClrEdgesCSPLot);
    m_PosWigFile->init(&m_WHColor);
    int index=OptDlg.BaseRenderIndex;
    //int nSize=idS->size();
    vector<int>::const_iterator it=idS->begin();

    
        vector<int>::const_iterator it1=m_PosWigFile->frdVIS_.GetGridIds()->begin();
        for (it1;it1!=m_PosWigFile->frdVIS_.GetGridIds()->end();it1++)
        {
            m_PosWigFile->frdVIS_.SetMeshVisible((*it1),false,m_WHColor);
            m_PosWigFile->frdVIS_.SetShadeVisible((*it1),false,m_WHColor);
            m_PosWigFile->frdVIS_.SetOutlineVisible((*it1),false,m_WHColor);
            m_PosWigFile->frdVIS_.SetAllEdgeMeshVisible((*it1),false,m_WHColor);
        }
   
    for (it;it!=idS->end();it++)
    {
        if(index == 0){
            m_PosWigFile->frdVIS_.SetOutlineVisible((*it),false,m_WHColor);
            m_PosWigFile->frdVIS_.SetAllEdgeMeshVisible((*it),false,m_WHColor);
            m_PosWigFile->frdVIS_.SetShadeVisible((*it),false,m_WHColor);
            m_PosWigFile->frdVIS_.SetMeshVisible((*it),true,m_WHColor);

        }else if (index == 2){
            m_PosWigFile->frdVIS_.SetOutlineVisible((*it),false,m_FSColor);
            m_PosWigFile->frdVIS_.SetAllEdgeMeshVisible((*it),false,m_FSColor);
            m_PosWigFile->frdVIS_.SetMeshVisible((*it),false,m_FSColor);
            m_PosWigFile->frdVIS_.SetShadeVisible((*it),true,m_FSColor);
        }else if (index == 3){
            m_PosWigFile->frdVIS_.SetOutlineVisible((*it),false,m_CSColor);
            m_PosWigFile->frdVIS_.SetAllEdgeMeshVisible((*it),false,m_CSColor);
            m_PosWigFile->frdVIS_.SetMeshVisible((*it),true,m_WHColor);
            m_PosWigFile->frdVIS_.SetShadeVisible((*it),true,m_FSColor);
        }else if (index == 4){
            m_PosWigFile->frdVIS_.SetShadeVisible((*it),false,m_CSColor);
            m_PosWigFile->frdVIS_.SetMeshVisible((*it),false,m_CSColor);
            m_PosWigFile->frdVIS_.SetOutlineVisible((*it),false,m_CSColor);
            m_PosWigFile->frdVIS_.SetAllEdgeMeshVisible((*it),true,m_CSColor);
        }else if (index == 5){
            m_PosWigFile->frdVIS_.SetShadeVisible((*it),false,m_CSColor);
            m_PosWigFile->frdVIS_.SetMeshVisible((*it),false,m_CSColor);
            m_PosWigFile->frdVIS_.SetAllEdgeMeshVisible((*it),false,m_CSColor);
            m_PosWigFile->frdVIS_.SetOutlineVisible((*it),true,m_CSColor);
        }
    }
   
    //--璁剧疆棰滆壊
    //--璁剧疆绾垮绛?
    m_PosWigFile->frdVIS_.SetWidth(OptDlg.ClrEdgeAThikIndex *1.25);
    m_PosWigFile->frdVIS_.Update();
    //--璁剧疆鎷惧彇棰滆壊銆佸ぇ灏忥紱
    m_HisPointClr=ChangeColorToDouble(OptDlg.ClrHisCuvPoint);
    m_PosWigFile->frdVIS_.SetColorSize(OptDlg.ClrHisPointfactor.toDouble(),m_HisPointClr);
}
void QPostPrc::SetActorSlot(vector<int> dataId)
{
    m_ActorListData=dataId;
    SetOptionS(m_optDlgs,&m_ActorListData);
	QStringList strList;
	for (int kk=0;kk<m_ActorListData.size();kk++)
	{
		strList<<QString("%1").arg(m_ActorListData.at(kk));
	}
	m_PostWigResultOut->setActorList(strList);

    ResultVisS visParam = m_PostWigResultOut->m_VisParam;
    if (visParam.strName == "" && m_FileScalarName != "") {
        visParam.strName = m_FileScalarName;
    }
    if (visParam.strName != "") {
        UpDataScalar(visParam);
    }

    vector<int>::const_iterator it=m_WholeActorData.begin();
    for (it;it!=m_WholeActorData.end();it++) {
        if (std::find(m_ActorListData.begin(),m_ActorListData.end(),*it)==m_ActorListData.end()) {
            m_PosWigFile->frdVIS_.HideGridActors(*it);
        }
    }
    m_PosWigFile->frdVIS_.Update();
}
/*
    浜戝浘鍙婄瓑鍊肩嚎鍥?
    1.orig:浜戝浘涓€鐩翠娇鐢ㄥ師濮嬩簯鍥?
    2.current:浜戝浘浣跨敤褰撳墠姝ョ殑浜戝浘;
           bgridshow:缃戞牸浣跨敤褰撳墠鐨勭綉鏍?
*/
void QPostPrc::UpDataScalar(ResultVisS ResultVis)
{
    QString scalar=ResultVis.strName;
    if (scalar == ""){
        return;//娌℃湁鍔犺浇鏂囦欢
    }
    QString strOrigCurrentName = scalar.left(scalar.indexOf(":"));
    QString strComponent = scalar.section(":", 1, 1);
    const map<QString, QStringList> *scalarInfo = m_PosWigFile->frdVIS_.GetScalarInfo();
    if (scalarInfo == 0 ||
        scalarInfo->find(strOrigCurrentName) == scalarInfo->end() ||
        scalarInfo->find(strOrigCurrentName)->second.indexOf(strComponent) == -1) {
        Information_Widget::GetInstance()->ShowInformation("Selected FRD result is not available: " + scalar);
        return;
    }
    int iOrigCurrent=ResultVis.m_iOrigCurrentChg;
    if (iOrigCurrent <0) return;
    xyplotPanel_->UpDataComb(iOrigCurrent,scalar);
    m_PostWigResultOut->upDateScalar(scalar);
    m_FileScalarName=scalar;
    QString strNumLabel=strOrigCurrentName.left(strOrigCurrentName.indexOf("-")).remove("L",Qt::CaseInsensitive);
    //
    //鍏抽棴鍘熷缃戞牸鏄剧ず----
     vector<int>::iterator it=m_WholeActorData.begin();
     for (it;it!=m_WholeActorData.end();it++)
     {
         m_PosWigFile->frdVIS_.SetMeshVisible((*it),false,m_WHColor);
         m_PosWigFile->frdVIS_.SetShadeVisible((*it),false,m_WHColor);
         m_PosWigFile->frdVIS_.SetOutlineVisible((*it),false,m_WHColor);
         m_PosWigFile->frdVIS_.SetAllEdgeMeshVisible((*it),false,m_WHColor);
         m_PosWigFile->frdVIS_.SetContourVisible(*it,scalar,false);
     }
    m_PosWigFile->frdVIS_.HideAllContours();
    static QString strOldName = strOrigCurrentName;
    if (ResultVis.bContour){//浜戝浘 //鏄剧ず鍙樺舰鍚庝簯鍥?--
        if (strOldName == strOrigCurrentName){//涓嶉渶瑕佹洿鏂?
            vector<int>::const_iterator it=m_WholeActorData.begin();
             for (it;it!=m_WholeActorData.end();it++){
                 if (std::find(m_ActorListData.begin(),m_ActorListData.end(),*it)!=m_ActorListData.end()){
                     m_PosWigFile->frdVIS_.SetContourVisible(*it,scalar,false);
                     m_PosWigFile->frdVIS_.SetContourVisible(*it,scalar,true,"L"+strOldName.left(strOldName.indexOf("-")).remove("L",Qt::CaseInsensitive)+"-DISP");
                 }else{
                     m_PosWigFile->frdVIS_.SetContourVisible(*it,scalar,false);
                     m_PosWigFile->frdVIS_.SetContourVisible(*it,scalar,false,"L"+strOldName.left(strOldName.indexOf("-")).remove("L",Qt::CaseInsensitive)+"-DISP");
                 }
                
                 m_PosWigFile->frdVIS_.SetContourLevel(*it,ResultVis.m_iClrScalar,"L"+strNumLabel+"-DISP");//棰滆壊绾у埆
                 m_PosWigFile->frdVIS_.SetContourType(*it,scalar,ResultVis.m_iStyle,"L"+strNumLabel+"-DISP");//浜戝浘0 or 2绛夊€肩嚎
                 m_PosWigFile->frdVIS_.SetContourVariable(*it,scalar,"L"+strNumLabel+"-DISP");
                 m_PosWigFile->frdVIS_.SetContourOpacity(*it,ResultVis.m_dTrans,"L"+strNumLabel+"-DISP");//閫忔槑搴?

				if (std::find(m_ActorListData.begin(),m_ActorListData.end(),*it)!=m_ActorListData.end()){               
					 if ((*it)==(*m_ActorListData.begin()))
						 m_PosWigFile->frdVIS_.SetContourScalarbarVisible(*it,true,"L"+strNumLabel+"-DISP");//鍙樉绀轰竴涓狟ar
					 else
						 m_PosWigFile->frdVIS_.SetContourScalarbarVisible(*it,false,"L"+strNumLabel+"-DISP");
				}else{
					m_PosWigFile->frdVIS_.SetContourScalarbarVisible(*it,false,"L"+strNumLabel+"-DISP");
				}

                 if (ResultVis.bContourMinMax){
                     m_PosWigFile->frdVIS_.SetContourRange(*it,ResultVis.contourMin,ResultVis.contourMax,"L"+strNumLabel+"-DISP");//鑼冨洿
                 }
             }
        }else{//鏇存柊
            vector<int>::const_iterator it=m_WholeActorData.begin();
            for (it;it!=m_WholeActorData.end();it++){
                if (std::find(m_ActorListData.begin(),m_ActorListData.end(),*it)!=m_ActorListData.end()){
                    m_PosWigFile->frdVIS_.SetContourVisible(*it,scalar,false);
                    m_PosWigFile->frdVIS_.SetContourVisible(*it,scalar,false,"L"+strOldName.left(strOldName.indexOf("-")).remove("L",Qt::CaseInsensitive)+"-DISP");
                    m_PosWigFile->frdVIS_.SetContourVisible(*it,scalar,true,"L"+strNumLabel+"-DISP");
                }else{
                    m_PosWigFile->frdVIS_.SetContourVisible(*it,scalar,false);
                    m_PosWigFile->frdVIS_.SetContourVisible(*it,scalar,false,"L"+strOldName.left(strOldName.indexOf("-")).remove("L",Qt::CaseInsensitive)+"-DISP");
                    m_PosWigFile->frdVIS_.SetContourVisible(*it,scalar,false,"L"+strNumLabel+"-DISP");
                }
                m_PosWigFile->frdVIS_.SetContourLevel(*it,ResultVis.m_iClrScalar,"L"+strNumLabel+"-DISP");//棰滆壊绾у埆
                m_PosWigFile->frdVIS_.SetContourType(*it,scalar,ResultVis.m_iStyle,"L"+strNumLabel+"-DISP");//浜戝浘0 or 2绛夊€肩嚎
                m_PosWigFile->frdVIS_.SetContourVariable(*it,scalar,"L"+strNumLabel+"-DISP");
                m_PosWigFile->frdVIS_.SetContourOpacity(*it,ResultVis.m_dTrans,"L"+strNumLabel+"-DISP");//閫忔槑搴?
				if (std::find(m_ActorListData.begin(),m_ActorListData.end(),*it)!=m_ActorListData.end()){  
					if ((*it)==(*m_ActorListData.begin()))
						m_PosWigFile->frdVIS_.SetContourScalarbarVisible(*it,true,"L"+strNumLabel+"-DISP");//鍙樉绀轰竴涓狟ar
					else
						m_PosWigFile->frdVIS_.SetContourScalarbarVisible(*it,false,"L"+strNumLabel+"-DISP");
				}else{
					m_PosWigFile->frdVIS_.SetContourScalarbarVisible(*it,false,"L"+strNumLabel+"-DISP");
				}

                if (ResultVis.bContourMinMax){
                    m_PosWigFile->frdVIS_.SetContourRange(*it,ResultVis.contourMin,ResultVis.contourMax,"L"+strNumLabel+"-DISP");//鑼冨洿
                }
            }
            strOldName = strOrigCurrentName;
        }
    }else{//鍏抽棴浜戝浘
        vector<int>::const_iterator it=m_WholeActorData.begin();
        for (it;it!=m_WholeActorData.end();it++){
             m_PosWigFile->frdVIS_.SetContourVisible(*it,scalar,false);
             m_PosWigFile->frdVIS_.SetContourVisible(*it,scalar,false,"L"+strOldName.left(strOldName.indexOf("-")).remove("L",Qt::CaseInsensitive)+"-DISP");//闅愯棌涔嬪墠鐨?
             m_PosWigFile->frdVIS_.SetContourVisible(*it,scalar,false,"L"+strNumLabel+"-DISP");
             m_PosWigFile->frdVIS_.SetContourLevel(*it,ResultVis.m_iClrScalar,"L"+strNumLabel+"-DISP");//棰滆壊绾у埆
             m_PosWigFile->frdVIS_.SetContourType(*it,scalar,ResultVis.m_iStyle,"L"+strNumLabel+"-DISP");//浜戝浘0 or 2绛夊€肩嚎
             m_PosWigFile->frdVIS_.SetContourVariable(*it,scalar,"L"+strNumLabel+"-DISP");
             m_PosWigFile->frdVIS_.SetContourOpacity(*it,ResultVis.m_dTrans,"L"+strNumLabel+"-DISP");//閫忔槑搴?   
             if (ResultVis.bContourMinMax){
                 m_PosWigFile->frdVIS_.SetContourRange(*it,ResultVis.contourMin,ResultVis.contourMax,"L"+strNumLabel+"-DISP");//鑼冨洿
             }
        }
       
        strOldName = strOrigCurrentName; 
    }
	//--QY Clip 20210831
	/*vector<int>::const_iterator its=m_WholeActorData.begin();
	for (its;its!=m_WholeActorData.end();its++){
		for (int kk=0;kk<ResultVis.CutIdList.size();kk++){
		m_PosWigFile->frdVIS_.SetCutContourVariable(*its,ResultVis.CutIdList.at(kk),scalar);
		} 
	}*/
  
    //缃戞牸
    if (ResultVis.bContour) {
        m_PosWigFile->frdVIS_.RaiseVisibleContours("L"+strNumLabel+"-DISP", m_ActorListData);
    }

    bool bGridVisable = ResultVis.bCurGrid;
    QString oldDispName=strOrigCurrentName;
    m_PosWigFile->frdVIS_.SetDisplacementColor(ResultVis.m_gridShowColor);

    if (bGridVisable == true)
    {
        if (iOrigCurrent==0){//鏄剧ず鍘熷鐨勭綉鏍?
            QString dispHeader = "L"+strNumLabel+"-DISP";
            vector<int>::const_iterator it=m_WholeActorData.begin();
            for (it;it!=m_WholeActorData.end();it++){
                if (std::find(m_ActorListData.begin(),m_ActorListData.end(),*it)!=m_ActorListData.end()){
                    m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,"L"+oldDispName.left(oldDispName.indexOf("-")).remove("L",Qt::CaseInsensitive)+"-DISP",false);//闅愯棌涔嬪墠鐨?
                    m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,dispHeader,true);
                }else{
                    m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,"L"+oldDispName.left(oldDispName.indexOf("-")).remove("L",Qt::CaseInsensitive)+"-DISP",false);//闅愯棌涔嬪墠鐨?
                    m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,dispHeader,false);
                }
            }
            oldDispName=dispHeader;
        }else{
            if (strOrigCurrentName==oldDispName){
                vector<int>::const_iterator it=m_WholeActorData.begin();
                for (it;it!=m_WholeActorData.end();it++){
                    if (std::find(m_ActorListData.begin(),m_ActorListData.end(),*it)!=m_ActorListData.end())
                       m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,"L"+strOrigCurrentName.left(strOrigCurrentName.indexOf("-")).remove("L",Qt::CaseInsensitive)+"-DISP",true);
                    else
                       m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,"L"+strOrigCurrentName.left(strOrigCurrentName.indexOf("-")).remove("L",Qt::CaseInsensitive)+"-DISP",false);
                }
            }else{
                if(oldDispName.isEmpty()){
                    vector<int>::const_iterator it=m_WholeActorData.begin();
                    for (it;it!=m_WholeActorData.end();it++){
                        if (std::find(m_ActorListData.begin(),m_ActorListData.end(),*it)!=m_ActorListData.end()){
                            m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,oldDispName,false); //闅愯棌涔嬪墠鐨?
                            m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,"L"+strOrigCurrentName.left(strOrigCurrentName.indexOf("-")).remove("L",Qt::CaseInsensitive)+"-DISP",true);//鏄剧ず鐜板湪鐨?
                        }else{
                            m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,oldDispName,false); //闅愯棌涔嬪墠鐨?
                            m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,"L"+strOrigCurrentName.left(strOrigCurrentName.indexOf("-")).remove("L",Qt::CaseInsensitive)+"-DISP",false);
                        }                          
                    }
                }else{
                    vector<int>::const_iterator it=m_WholeActorData.begin();
                    for (it;it!=m_WholeActorData.end();it++){
                         if (std::find(m_ActorListData.begin(),m_ActorListData.end(),*it)!=m_ActorListData.end()){
                            m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,"L"+oldDispName.left(oldDispName.indexOf("-")).remove("L",Qt::CaseInsensitive)+"-DISP",false);//闅愯棌涔嬪墠鐨?
                            m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,"L"+strOrigCurrentName.left(strOrigCurrentName.indexOf("-")).remove("L",Qt::CaseInsensitive)+"-DISP",true);//鏄剧ず鐜板湪鐨?
                         }else{
                             m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,"L"+oldDispName.left(oldDispName.indexOf("-")).remove("L",Qt::CaseInsensitive)+"-DISP",false);//闅愯棌涔嬪墠鐨?
                             m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,"L"+strOrigCurrentName.left(strOrigCurrentName.indexOf("-")).remove("L",Qt::CaseInsensitive)+"-DISP",false);//
                         }
                     }    
                 }
               oldDispName=strOrigCurrentName;
           }
        }
    }else{
        QString header;header.clear(); 
        vector<int>::const_iterator it=m_WholeActorData.begin();
        for (it;it!=m_WholeActorData.end();it++){
            m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,header,false);
            m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,"L"+oldDispName.left(oldDispName.indexOf("-")).remove("L",Qt::CaseInsensitive)+"-DISP",false);//闅愯棌涔嬪墠鐨?
            m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,"L"+strOrigCurrentName.left(strOrigCurrentName.indexOf("-")).remove("L",Qt::CaseInsensitive)+"-DISP",false);//
         }
        oldDispName=strOrigCurrentName;
    }
    VTKColorS boundaryColor;
    boundaryColor.r = 0.0;
    boundaryColor.g = 0.0;
    boundaryColor.b = 0.0;
    vector<int>::const_iterator boundaryIt=m_WholeActorData.begin();
    for (boundaryIt; boundaryIt!=m_WholeActorData.end(); boundaryIt++){
        bool selected = std::find(m_ActorListData.begin(),m_ActorListData.end(),*boundaryIt)!=m_ActorListData.end();
        bool showBoundary = bGridVisable && selected && (iOrigCurrent == 0);
        m_PosWigFile->frdVIS_.SetAllEdgeMeshVisible(*boundaryIt, false, ResultVis.m_gridShowColor);
        m_PosWigFile->frdVIS_.SetOutlineVisible(*boundaryIt, showBoundary, boundaryColor);
        if (!selected) {
            m_PosWigFile->frdVIS_.HideGridActors(*boundaryIt);
        }
    }
    m_PosWigFile->frdVIS_.Update();
    //--鑾峰彇frdVis涓暟鎹紝杩涜frdText鏄剧ず
    m_PosWigFile->frdTextVIS_.ShowText(m_PosWigFile->frdVIS_.GetText(scalar),m_readReDlg.RadioTextIndex);
    m_PosWigFile->frdTextVIS_.Update();

	//--Writer Player(鍐橝VI)
	if (ResultVis.movieSet==1){
			QString aviName = ResultVis.movieName;
			QFileInfo aviInfo(aviName);
			if (aviInfo.path() == "." || aviInfo.path().isEmpty()) {
				QString frdDir = QFileInfo(m_PosWigFile->frdFilePath()).absolutePath();
				aviName = frdDir + "/" + aviInfo.fileName();
			}
		if(m_PosWigFile->frdVIS_.StartAVI(aviName,ResultVis.movieNameRate))
			m_PosWigFile->frdVIS_.ModifiedAVI();
	}else if (ResultVis.movieSet==2){
		m_PosWigFile->frdVIS_.ModifiedAVI();
	}else if (ResultVis.movieSet==9){
		m_PosWigFile->frdVIS_.EndAVI();
	}
   // m_PostWigResultOut->InitSectionDataSlot(&m_PosWigFile->frdVIS_);
}



///*
//	Tree涓搴旂殑鍒囨崲
//*/
//void QPostPrc::ShowTheTreeItemSlot()
//{
//    //m_TreeModel->item(0,0)->child(0,0)->setEnabled(true);
//}
//---
VTKColorS QPostPrc::ChangeColorToDouble(QString str)
{
    VTKColorS tmpColor;
    QString tmpStr;
    str.split(",");
    tmpStr=str.remove("rgba");
    tmpStr=tmpStr.remove("(");
    tmpStr=tmpStr.remove(")");
    QStringList strList =tmpStr.split(",");
    tmpColor.r = strList.at(0).toInt()/255.0;
    tmpColor.g = strList.at(1).toInt()/255.0;
    tmpColor.b = strList.at(2).toInt()/255.0;
    return tmpColor;
}



//--鍓栭潰:娣诲姞鍓栭潰
void QPostPrc::SectionCutAddSlot(int CutId)
{
   // if (m_FileScalarName.isEmpty())return;

   // //鍏抽棴鍘熷缃戞牸鏄剧ず----
   // m_PosWigFile->frdVIS_.SetMeshVisible(false,m_WHColor);
   // m_PosWigFile->frdVIS_.SetShadeVisible(false,m_WHColor);
   // m_PosWigFile->frdVIS_.SetOutlineVisible(false,m_WHColor);
   // m_PosWigFile->frdVIS_.SetAllEdgeMeshVisible(false,m_WHColor);

   ////鍏抽棴浜戝浘----
   // m_PosWigFile->frdVIS_.SetContourVisible(m_FileScalarName,false,m_FileScalarName.split(":").at(0)); 
   // //m_PosWigFile->frdVIS_.Update();
}

