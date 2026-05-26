#include "./PostProcess/XYPlot_Panel.h"
#include "QPostPrc.h"


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
    //--剖面
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
// Method:    单击树节点的响应;
// Returns:   void;
// Parameter: index     树节点
// Author:	  
//****************************************************
void QPostPrc::ChangeModelIndexSlot( const QModelIndex & index )
{  
  //  Render_Widget *renWidget = Render_Widget::GetInstance();
    QString type = index.data(Qt::UserRole+3).toString();
    if (type == "PlotOpt")
    {
        m_PostWidPlotOptDlg->show(); 
        m_PostWidPlotOptDlg->raise();//最上层
        m_PostWidPlotOptDlg->activateWindow();//激活
    }
    else if (type == "DatafrdOpt")//
    {
        m_StackedWidget->setCurrentWidget(m_PosWigFile);
    }
	else if (type == "ReadRest")//
	{
		//m_StackedWidget->setCurrentWidget(m_PosWigReadResults);
	 
	   m_PosWigReadResultDlg->show(); 
	   m_PosWigReadResultDlg->raise();//最上层
	   m_PosWigReadResultDlg->activateWindow();//激活
	}
	else if (type == "PlotRest")
	{
		m_StackedWidget->setCurrentWidget(m_PostWigResultOut);
		//m_PostWigResultOut->setMenu();
	}
	else if (type == "ListRest")
	{
		//m_StackedWidget->setCurrentWidget(m_PosWigFile);打开当前Frd文件
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
		QString str = QCoreApplication::applicationDirPath(); 
		QString cmd = str+"/RMesh";
		QProcess *RMshProcess= new QProcess(this);
		RMshProcess->start(cmd);
        #endif
	}
	else if (type == "Other")
	{
		//m_StackedWidget->setCurrentWidget(m_PosWigFile);
	}
    currentTreeNodeType_ = type;
}

/*
	数据选择显示
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
    m_ActorListData.clear();
    vector<int>::const_iterator it1=m_PosWigFile->frdVIS_.GetGridIds()->begin();//获取所有IdList
    for (it1;it1!=m_PosWigFile->frdVIS_.GetGridIds()->end();it1++){ 
       m_ActorListData.push_back(*it1);
      
    }
    m_WholeActorData=m_ActorListData;
    emit emitStepPlayMenu(m_ResultO);//主界面播放条
    
}
/*----------------------------------------------
	  Depend on Plot vtktext‘s Param to Set the VIS. 
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
	  Depend on Plot Option‘s Param to Set the VIS. 
-------------------------------------------------*/

void QPostPrc::UpDataVisSlot(OptDlgS OptDlg)
{
    if (m_FileScalarName !=""){
        m_PosWigFile->frdVIS_.SetContourVisible(1,m_FileScalarName,false);
    }
  
    m_optDlgs = OptDlg;
    //m_ActorListData=m_PosWigFile->frdVIS_.GetGridIds();//获取所有IdList
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
   
    //--设置颜色
    //--设置线宽等
    m_PosWigFile->frdVIS_.SetWidth(OptDlg.ClrEdgeAThikIndex *1.25);
    m_PosWigFile->frdVIS_.Update();
    //--设置拾取颜色、大小；
    m_HisPointClr=ChangeColorToDouble(OptDlg.ClrHisCuvPoint);
    m_PosWigFile->frdVIS_.SetColorSize(OptDlg.ClrHisPointfactor.toDouble(),m_HisPointClr);
}
void QPostPrc::SetActorSlot(vector<int> dataId)
{
    m_ActorListData=dataId;
    if (m_ActorListData.empty() && !m_WholeActorData.empty()) {
        m_ActorListData = m_WholeActorData;
    }
    SetOptionS(m_optDlgs,&m_ActorListData);
	QStringList strList;
	for (int kk=0;kk<m_ActorListData.size();kk++)
	{
		strList<<QString("%1").arg(m_ActorListData.at(kk));
	}
	m_PostWigResultOut->setActorList(strList);
}
/*
    云图及等值线图
    1.orig:云图一直使用原始云图;
    2.current:云图使用当前步的云图;
           bgridshow:网格使用当前的网格;
*/
void QPostPrc::UpDataScalar(ResultVisS ResultVis)
{
    QString scalar=ResultVis.strName;
    if (scalar == ""){
        return;//没有加载文件
    }
    if (m_ActorListData.empty() && !m_WholeActorData.empty()) {
        m_ActorListData = m_WholeActorData;
    }
    
    int iOrigCurrent=ResultVis.m_iOrigCurrentChg;
    if (iOrigCurrent <0) return;
    xyplotPanel_->UpDataComb(iOrigCurrent,scalar);
    m_PostWigResultOut->upDateScalar(scalar);
    m_FileScalarName=scalar;
    QString strOrigCurrentName = scalar.left(scalar.indexOf(":"));
    QString strNumLabel=strOrigCurrentName.left(strOrigCurrentName.indexOf("-")).remove("L",Qt::CaseInsensitive);
    //
    //关闭原始网格显示----
     vector<int>::iterator it=m_WholeActorData.begin();
     for (it;it!=m_WholeActorData.end();it++)
     {
         m_PosWigFile->frdVIS_.SetMeshVisible((*it),false,m_WHColor);
         m_PosWigFile->frdVIS_.SetShadeVisible((*it),false,m_WHColor);
         m_PosWigFile->frdVIS_.SetOutlineVisible((*it),false,m_WHColor);
         m_PosWigFile->frdVIS_.SetAllEdgeMeshVisible((*it),false,m_WHColor);
         m_PosWigFile->frdVIS_.SetContourVisible(*it,scalar,false);
     }
    static QString strOldName = strOrigCurrentName;
    if (ResultVis.bContour){//云图 //显示变形后云图---
        if (strOldName == strOrigCurrentName){//不需要更新
            vector<int>::const_iterator it=m_WholeActorData.begin();
             for (it;it!=m_WholeActorData.end();it++){
                 if (std::find(m_ActorListData.begin(),m_ActorListData.end(),*it)!=m_ActorListData.end()){
                     m_PosWigFile->frdVIS_.SetContourVisible(*it,scalar,false);
                     m_PosWigFile->frdVIS_.SetContourVisible(*it,scalar,true,"L"+strOldName.left(strOldName.indexOf("-")).remove("L",Qt::CaseInsensitive)+"-DISP");
                 }else{
                     m_PosWigFile->frdVIS_.SetContourVisible(*it,scalar,false);
                     m_PosWigFile->frdVIS_.SetContourVisible(*it,scalar,false,"L"+strOldName.left(strOldName.indexOf("-")).remove("L",Qt::CaseInsensitive)+"-DISP");
                 }
                
                 m_PosWigFile->frdVIS_.SetContourLevel(*it,ResultVis.m_iClrScalar,"L"+strNumLabel+"-DISP");//颜色级别
                 m_PosWigFile->frdVIS_.SetContourType(*it,scalar,ResultVis.m_iStyle,"L"+strNumLabel+"-DISP");//云图0 or 2等值线
                 m_PosWigFile->frdVIS_.SetContourVariable(*it,scalar,"L"+strNumLabel+"-DISP");
                 m_PosWigFile->frdVIS_.SetContourOpacity(*it,ResultVis.m_dTrans,"L"+strNumLabel+"-DISP");//透明度

				if (std::find(m_ActorListData.begin(),m_ActorListData.end(),*it)!=m_ActorListData.end()){               
					 if ((*it)==(*m_ActorListData.begin()))
						 m_PosWigFile->frdVIS_.SetContourScalarbarVisible(*it,true,"L"+strNumLabel+"-DISP");//只显示一个Bar
					 else
						 m_PosWigFile->frdVIS_.SetContourScalarbarVisible(*it,false,"L"+strNumLabel+"-DISP");
				}else{
					m_PosWigFile->frdVIS_.SetContourScalarbarVisible(*it,false,"L"+strNumLabel+"-DISP");
				}

                 if (ResultVis.bContourMinMax){
                     m_PosWigFile->frdVIS_.SetContourRange(*it,ResultVis.contourMin,ResultVis.contourMax,"L"+strNumLabel+"-DISP");//范围
                 }
             }
        }else{//更新
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
                m_PosWigFile->frdVIS_.SetContourLevel(*it,ResultVis.m_iClrScalar,"L"+strNumLabel+"-DISP");//颜色级别
                m_PosWigFile->frdVIS_.SetContourType(*it,scalar,ResultVis.m_iStyle,"L"+strNumLabel+"-DISP");//云图0 or 2等值线
                m_PosWigFile->frdVIS_.SetContourVariable(*it,scalar,"L"+strNumLabel+"-DISP");
                m_PosWigFile->frdVIS_.SetContourOpacity(*it,ResultVis.m_dTrans,"L"+strNumLabel+"-DISP");//透明度
				if (std::find(m_ActorListData.begin(),m_ActorListData.end(),*it)!=m_ActorListData.end()){  
					if ((*it)==(*m_ActorListData.begin()))
						m_PosWigFile->frdVIS_.SetContourScalarbarVisible(*it,true,"L"+strNumLabel+"-DISP");//只显示一个Bar
					else
						m_PosWigFile->frdVIS_.SetContourScalarbarVisible(*it,false,"L"+strNumLabel+"-DISP");
				}else{
					m_PosWigFile->frdVIS_.SetContourScalarbarVisible(*it,false,"L"+strNumLabel+"-DISP");
				}

                if (ResultVis.bContourMinMax){
                    m_PosWigFile->frdVIS_.SetContourRange(*it,ResultVis.contourMin,ResultVis.contourMax,"L"+strNumLabel+"-DISP");//范围
                }
            }
            strOldName = strOrigCurrentName;
        }
    }else{//关闭云图
        vector<int>::const_iterator it=m_WholeActorData.begin();
        for (it;it!=m_WholeActorData.end();it++){
             m_PosWigFile->frdVIS_.SetContourVisible(*it,scalar,false);
             m_PosWigFile->frdVIS_.SetContourVisible(*it,scalar,false,"L"+strOldName.left(strOldName.indexOf("-")).remove("L",Qt::CaseInsensitive)+"-DISP");//隐藏之前的
             m_PosWigFile->frdVIS_.SetContourVisible(*it,scalar,false,"L"+strNumLabel+"-DISP");
             m_PosWigFile->frdVIS_.SetContourLevel(*it,ResultVis.m_iClrScalar,"L"+strNumLabel+"-DISP");//颜色级别
             m_PosWigFile->frdVIS_.SetContourType(*it,scalar,ResultVis.m_iStyle,"L"+strNumLabel+"-DISP");//云图0 or 2等值线
             m_PosWigFile->frdVIS_.SetContourVariable(*it,scalar,"L"+strNumLabel+"-DISP");
             m_PosWigFile->frdVIS_.SetContourOpacity(*it,ResultVis.m_dTrans,"L"+strNumLabel+"-DISP");//透明度    
             if (ResultVis.bContourMinMax){
                 m_PosWigFile->frdVIS_.SetContourRange(*it,ResultVis.contourMin,ResultVis.contourMax,"L"+strNumLabel+"-DISP");//范围
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
  
    //网格
    bool bGridVisable = ResultVis.bCurGrid;
    static QString oldDispName=strOrigCurrentName;
    m_PosWigFile->frdVIS_.SetDisplacementColor(ResultVis.m_gridShowColor);

    if (bGridVisable == true)
    {
        if (iOrigCurrent==0){//显示原始的网格
            vector<int>::const_iterator it=m_WholeActorData.begin();
            QString header;header.clear(); 
            for (it;it!=m_WholeActorData.end();it++){
                if (std::find(m_ActorListData.begin(),m_ActorListData.end(),*it)!=m_ActorListData.end()){
                    m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,"L"+oldDispName.left(oldDispName.indexOf("-")).remove("L",Qt::CaseInsensitive)+"-DISP",false);//隐藏之前的
                    m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,header,true); 
                }else{
                    m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,"L"+oldDispName.left(oldDispName.indexOf("-")).remove("L",Qt::CaseInsensitive)+"-DISP",false);//隐藏之前的
                    m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,header,false); 
                }
            }
            oldDispName=header;
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
                            m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,oldDispName,false); //隐藏之前的
                            m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,"L"+strOrigCurrentName.left(strOrigCurrentName.indexOf("-")).remove("L",Qt::CaseInsensitive)+"-DISP",true);//显示现在的
                        }else{
                            m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,oldDispName,false); //隐藏之前的
                            m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,"L"+strOrigCurrentName.left(strOrigCurrentName.indexOf("-")).remove("L",Qt::CaseInsensitive)+"-DISP",false);
                        }                          
                    }
                }else{
                    vector<int>::const_iterator it=m_WholeActorData.begin();
                    for (it;it!=m_WholeActorData.end();it++){
                         if (std::find(m_ActorListData.begin(),m_ActorListData.end(),*it)!=m_ActorListData.end()){
                            m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,"L"+oldDispName.left(oldDispName.indexOf("-")).remove("L",Qt::CaseInsensitive)+"-DISP",false);//隐藏之前的
                            m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,"L"+strOrigCurrentName.left(strOrigCurrentName.indexOf("-")).remove("L",Qt::CaseInsensitive)+"-DISP",true);//显示现在的
                         }else{
                             m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,"L"+oldDispName.left(oldDispName.indexOf("-")).remove("L",Qt::CaseInsensitive)+"-DISP",false);//隐藏之前的
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
            m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,"L"+oldDispName.left(oldDispName.indexOf("-")).remove("L",Qt::CaseInsensitive)+"-DISP",false);//隐藏之前的
            m_PosWigFile->frdVIS_.SetDisplacementVisible(*it,"L"+strOrigCurrentName.left(strOrigCurrentName.indexOf("-")).remove("L",Qt::CaseInsensitive)+"-DISP",false);//
         }
        oldDispName=strOrigCurrentName;
    }
    m_PosWigFile->frdVIS_.Update();
    //--获取frdVis中数据，进行frdText显示
    m_PosWigFile->frdTextVIS_.ShowText(m_PosWigFile->frdVIS_.GetText(scalar),m_readReDlg.RadioTextIndex);
    m_PosWigFile->frdTextVIS_.Update();

	//--Writer Player(写AVI)
	if (ResultVis.movieSet==1){
		if(m_PosWigFile->frdVIS_.StartAVI(ResultVis.movieName,ResultVis.movieNameRate));
			m_PosWigFile->frdVIS_.ModifiedAVI();
	}else if (ResultVis.movieSet==2){
		m_PosWigFile->frdVIS_.ModifiedAVI();
	}else if (ResultVis.movieSet==9){
		m_PosWigFile->frdVIS_.EndAVI();
	}
   // m_PostWigResultOut->InitSectionDataSlot(&m_PosWigFile->frdVIS_);
}



///*
//	Tree中对应的切换
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



//--剖面:添加剖面
void QPostPrc::SectionCutAddSlot(int CutId)
{
   // if (m_FileScalarName.isEmpty())return;

   // //关闭原始网格显示----
   // m_PosWigFile->frdVIS_.SetMeshVisible(false,m_WHColor);
   // m_PosWigFile->frdVIS_.SetShadeVisible(false,m_WHColor);
   // m_PosWigFile->frdVIS_.SetOutlineVisible(false,m_WHColor);
   // m_PosWigFile->frdVIS_.SetAllEdgeMeshVisible(false,m_WHColor);

   ////关闭云图----
   // m_PosWigFile->frdVIS_.SetContourVisible(m_FileScalarName,false,m_FileScalarName.split(":").at(0)); 
   // //m_PosWigFile->frdVIS_.Update();
}
