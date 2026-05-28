#include "QHPSystemDlg.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QStandardItemModel>

static int execDialogInFront(QDialog *dlg)
{
    if (!dlg) return QDialog::Rejected;
    dlg->setWindowFlags(dlg->windowFlags() | Qt::Window | Qt::WindowStaysOnTopHint);
    dlg->setWindowModality(Qt::ApplicationModal);
    dlg->show();
    dlg->raise();
    dlg->activateWindow();
    return dlg->exec();
}

QHPSystemDlg::QHPSystemDlg(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Heat Treatment System");
    QWidget *Hwid1 = new QWidget(this);
    QHBoxLayout *Hlayout = new QHBoxLayout();
    QHBoxLayout *Hlayout2 = new QHBoxLayout();
    m_DelBtn = new QPushButton(tr("删除"));
	m_AddBtn = new QPushButton(tr("增加"));
    m_InsertBtn = new QPushButton(tr("插入"));
    m_NextBtn = new QPushButton(tr("下一步"));
    m_PreBtn = new QPushButton(tr("上一步"));
	Hlayout->addWidget(m_AddBtn);
    Hlayout->addWidget(m_InsertBtn);
	Hlayout->addWidget(m_DelBtn);
    
    Hlayout2->addWidget(m_PreBtn);
    Hlayout2->addWidget(m_NextBtn);
    QStringList strList;
    strList<<tr("表面集合")<<tr("环境温度")<<tr("对流换热")<<tr("辐射系数")<<tr("热处理制度");
    QHBoxLayout *HLayout0 = new QHBoxLayout();
    m_ListWidget = new QListWidget();
    m_TableWiget = new QTableWidget();
    m_TableWiget->setColumnCount(strList.size());
    m_TableWiget->setRowCount(1);
    m_TableWiget->verticalHeaderItem(1);
    
    m_TableWiget->setHorizontalHeaderLabels(strList);
    //--自动进行缩放
   /* for (int j=0;j<strList.size();j++){
        m_TableWiget->horizontalHeader()->setResizeMode(j,QHeaderView::ResizeToContents);
    }*/

    //
    for (int i=0;i<1;i++)
    {
        MyCombox *TmpCombox=new MyCombox(this,i);
        TmpCombox->addItem("Surf Set1");
        TmpCombox->addItem("Surf Set2");
        TmpCombox->setFrame(false);
        m_TableWiget->setCellWidget(i,COL_SYS_PSET,TmpCombox);
        m_comboxPSetSurf1.insert(i,TmpCombox);//push_back(comboxPSet0);

        TmpCombox=new MyCombox(this,i);
        TmpCombox->addItem("constant");
        TmpCombox->addItem("F(t)");
        TmpCombox->setFrame(false);
        m_TableWiget->setCellWidget(i,COL_SYS_TEMPER,TmpCombox);
        m_ComboxTemper.insert(i,TmpCombox);
        connect(m_ComboxTemper[i],SIGNAL(clicked(int,int)),this,SLOT(comboxSlot()));

        TmpCombox=new MyCombox(this,i);
        TmpCombox->addItem("no");
        TmpCombox->addItem("constant");
        TmpCombox->addItem("F(t)");
        TmpCombox->setFrame(false);
        m_TableWiget->setCellWidget(i,COL_SYS_FILM,TmpCombox);
        m_ComboxFilm.insert(i,TmpCombox);
        connect(m_ComboxFilm[i],SIGNAL(clicked(int,int)),this,SLOT(comboxSlot()));
        //connect(m_ComboxFilm[i],SIGNAL(currentIndexChanged(int)),this,SLOT(comboxChangeSlot()));
        TmpCombox=new MyCombox(this,i);
        TmpCombox->addItem("no");
        TmpCombox->addItem("constant");
        TmpCombox->addItem("F(t)");
        TmpCombox->setFrame(false);
        m_TableWiget->setCellWidget(i,COL_SYS_RADIATE,TmpCombox);
        m_ComboxRadiate.insert(i,TmpCombox);
        connect(m_ComboxRadiate[i],SIGNAL(clicked(int,int)),this,SLOT(comboxSlot()));
        //connect(m_ComboxRadiate[i],SIGNAL(currentIndexChanged(int)),this,SLOT(comboxChangeSlot()));
        TmpCombox=new MyCombox(this,i);
        TmpCombox->addItem(tr("step1"));
        TmpCombox->addItem(tr("新建"));
        TmpCombox->addItem(tr("删除"));
        TmpCombox->setFrame(false);
        m_TableWiget->setCellWidget(i,COL_SYS_SYSTEM,TmpCombox);
        m_ComboxSystem.insert(i,TmpCombox); 
        connect(m_ComboxSystem[i],SIGNAL(clicked(int,int)),this,SLOT(comboxSlot()));
    }
  
	m_TableWiget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    HLayout0->addWidget(m_TableWiget);
    HLayout0->addWidget(m_ListWidget);
    QVBoxLayout *MainLayout = new QVBoxLayout(this);
    MainLayout->addLayout(HLayout0);
    MainLayout->addLayout(Hlayout);
    MainLayout->addLayout(Hlayout2);
    setLayout(MainLayout);
    m_TableWiget->setMinimumWidth(600);
    //connect(m_OKBtn,SIGNAL(clicked()),this,SLOT(ApplyHPSlot()));
    //connect(m_ApplyBtn,SIGNAL(clicked()),this,SLOT(ApplyHPSlot()));
	connect(m_AddBtn,SIGNAL(clicked()),this,SLOT(AddBtnHPSlot()));
	connect(m_DelBtn,SIGNAL(clicked()),this,SLOT(DelBtnHPSlot()));
    connect(m_NextBtn,SIGNAL(clicked()),this,SLOT(NextBtnHPSlot()));
    connect(m_PreBtn,SIGNAL(clicked()),this,SLOT(PreBtnHPSlot()));
    connect(m_InsertBtn,SIGNAL(clicked()),this,SLOT(InsertBtnHPSlot()));
    //connect(m_comboxCurveNew[1],SIGNAL(clicked(int,int)),this,SLOT(comBoxNewChgSlot(int,int)));
   //--delete menu
    m_popMenu = new QMenu;
    m_ListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    m_popMenu->addAction(tr("删除"),this,SLOT(DelRBtnSlot()));
    connect(m_ListWidget, SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(contextListWidgetMenuSlot(const QPoint &)));
    m_ListWidget->hide();  
}

QHPSystemDlg::~QHPSystemDlg()
{
    m_comboxPSetSurf1.clear();
}
void QHPSystemDlg::contextListWidgetMenuSlot(const QPoint &p)
{
    m_popMenu->exec(QCursor::pos());
}
void QHPSystemDlg::WriteHPsysToInp()
{
    HPSystemRInpS nodeRList;
    HPSystemFInpS nodeFList;
    int TabRowCnt = m_TableWiget->rowCount();
    int TabColCnt = m_TableWiget->columnCount();

     QList<HPSystemFInpS>  m_TmpNodeHPSystemFList;//
     QList<HPSystemRInpS>  m_TmpNodeHPSystemRList;//
    
    if (TabRowCnt>0){
        m_TmpNodeHPSystemFList.clear();
        m_TmpNodeHPSystemRList.clear();
        m_nodeHPSystemFList.clear();
        m_nodeHPSystemRList.clear();
        m_nodeCurveList.clear();
    }
    for (int i=0; i<TabRowCnt; i++){
         MyCombox *tmpBtn = (MyCombox*)(m_TableWiget->cellWidget(i, COL_SYS_SYSTEM));//制度
         QString strCurSysStep=tmpBtn->currentText();//--制度
         
         QStringList strInclude,strSurfaceNum;
         MyCombox *ff = (MyCombox*)(m_TableWiget->cellWidget(i, COL_SYS_PSET));//单元
         QString str=ff->currentText();
         int id=m_surfFRS.strName.indexOf(str);
         if (id !=-1){
             int ibaseNum=m_surfFRS.strNum.at(id).toInt();
             for (int kk=id;kk<id+ibaseNum;kk++){
                 strInclude<<m_surfFRS.strInclude.at(kk); 
                 QString str0=m_surfFRS.strIncludeSurfNum.at(kk);
                 strSurfaceNum<<str0.remove("S",Qt::CaseInsensitive);
             }    
         } 
             nodeFList.strStep = strCurSysStep;
             nodeRList.strStep = strCurSysStep;
             QString strTmpEnvirTemprat,strTmpAmplitudeName;
             tmpBtn=(MyCombox*)(m_TableWiget->cellWidget(i, COL_SYS_TEMPER));//环境
             id =m_ComboxTemper.key(tmpBtn);
             if (id!=-1){
                 str= m_ComboxTemper[id]->currentText();
                 if (str.contains("constant")){//数据为常量
                     //if(m_TemperateDlg.find(id)!=m_TemperateDlg.end()){
                     if(m_TemperateDlg.find(str)!=m_TemperateDlg.end()){
                         m_TemperateDlg[str]->SetShowStyle(0);
                         strTmpEnvirTemprat = m_TemperateDlg[str]->GetData(0);
                         strTmpAmplitudeName="";
                     }else{//未进行constant数值设定                                                

                     }
                 }else{ //if (str=="F(t)"){//函数
                     if(m_TemperateDlg.find(str)!=m_TemperateDlg.end()){
                         m_TemperateDlg[str]->SetShowStyle(1);
                         InpCurveInpS nodeCurveList;
                         nodeCurveList=m_TemperateDlg[str]->GetData(1,nodeCurveList);
                         insertToCurveList(nodeCurveList);
                         //emitHPCurve(nodeCurveList);//更新整个Curve(全局),同步热等静压数据
                         strTmpEnvirTemprat="1";
                         strTmpAmplitudeName=nodeCurveList.strName;
                     }else{;}//new dlg未进行constant数值设定   
                 }
             }
        
             tmpBtn=(MyCombox*)(m_TableWiget->cellWidget(i, COL_SYS_FILM));//对流
             id =m_ComboxFilm.key(tmpBtn);
             if (id!=-1){
                 str= m_ComboxFilm[id]->currentText();
                 if (str=="no"){//无film
                 }else if (str.contains("constant")){//数据为常量
                     if(m_FilmDlg.find(str)!=m_FilmDlg.end()){
                         m_FilmDlg[str]->SetShowStyle(0);
                         nodeFList.strCofficient = m_FilmDlg[str]->GetData(0);
                         nodeFList.strStyle="film";
                         nodeFList.strEnvirTemprat=strTmpEnvirTemprat;
                         nodeFList.strAmplitudeName=strTmpAmplitudeName;
                         nodeFList.strFilmRadiateAmpName="";
                     }else{;}//未进行constant数值设定                                                
                     nodeFList.strElsetName=strInclude.join(",");
                     nodeFList.strSurfaceNum=strSurfaceNum.join(",");
                     m_TmpNodeHPSystemFList.append(nodeFList);
                 }else{ //if (str=="F(t)"){//函数   
                     if(m_FilmDlg.find(str)!=m_FilmDlg.end()){
                         m_FilmDlg[str]->SetShowStyle(1);
                         InpCurveInpS nodeCurveList;
                         nodeCurveList=m_FilmDlg[str]->GetData(1,nodeCurveList);
                         insertToCurveList(nodeCurveList);  
                         //emitHPCurve(nodeCurveList);//更新整个Curve(全局),同步热等静压数据
                         nodeFList.strFilmRadiateAmpName=nodeCurveList.strName;
                         nodeFList.strElsetName=strInclude.join(",");
                         nodeFList.strSurfaceNum=strSurfaceNum.join(",");
                         nodeFList.strCofficient="1";
                         nodeFList.strStyle="film";
                         nodeFList.strAmplitudeName=strTmpAmplitudeName;
                         nodeFList.strEnvirTemprat=strTmpEnvirTemprat;
                     }else{;}//new dlg未进行constant数值设定  
                      m_TmpNodeHPSystemFList.append(nodeFList);
                 }  
             }

             tmpBtn=(MyCombox*)(m_TableWiget->cellWidget(i, COL_SYS_RADIATE));//辐射
             id =m_ComboxRadiate.key(tmpBtn);
             if (id!=-1){
                 str= m_ComboxRadiate[id]->currentText();
                 if (str=="no"){//无film
                 }else if (str.contains("constant")){//数据为常量 
                     if(m_RadiateDlg.find(str)!=m_RadiateDlg.end()){
                         m_RadiateDlg[str]->SetShowStyle(0);
                         nodeRList.strCofficient = m_RadiateDlg[str]->GetData(0);
                         nodeRList.strStyle="radiate";
                         nodeRList.strEnvirTemprat=strTmpEnvirTemprat;
                         nodeRList.strAmplitudeName=strTmpAmplitudeName;
                         nodeRList.strFilmRadiateAmpName="";
                     }else{;}//未进行constant数值设定                                                
                     nodeRList.strElsetName=strInclude.join(",");
                     nodeRList.strSurfaceNum=strSurfaceNum.join(",");
                     m_TmpNodeHPSystemRList.append(nodeRList);
                 }else{ //if (str=="F(t)"){//函数
                     //if (strCurSysStep.toLower() != "step1") {//其他新加制度 //--第一个制度
                     //    HPSystemRInpS tmpRdata;
                     //    tmpRdata.strStep=strCurSysStep;
                     //    tmpRdata.strStyle="radiate";
                     //    tmpRdata.strOP="新建";//"op=new;
                     //    m_nodeHPSystemRList.append(tmpRdata); 
                     //}  
                     if(m_RadiateDlg.find(str)!=m_RadiateDlg.end()){
                         m_RadiateDlg[str]->SetShowStyle(1);
                         InpCurveInpS nodeCurveList;
                         nodeCurveList=m_RadiateDlg[str]->GetData(1,nodeCurveList);
                         insertToCurveList(nodeCurveList);
                         //emitHPCurve(nodeCurveList);//更新整个Curve(全局),同步热等静压数据
                         nodeRList.strFilmRadiateAmpName=nodeCurveList.strName;
                         nodeRList.strElsetName=strInclude.join(",");
                         nodeRList.strSurfaceNum=strSurfaceNum.join(",");
                         nodeRList.strCofficient="1";
                         nodeRList.strStyle="radiate";
                         nodeRList.strEnvirTemprat=strTmpEnvirTemprat;
                         nodeRList.strAmplitudeName=strTmpAmplitudeName;
                     }else{;}//new dlg未进行ft数值设定     
                     m_TmpNodeHPSystemRList.append(nodeRList);
                 }   
             }      
         }
         //--处理m_nodeHPsystemRList;
         int nRSize=m_TmpNodeHPSystemRList.size();
         int nFSize=m_TmpNodeHPSystemFList.size();
         int baseMM=0;
         for (int kk=0;kk<nFSize;kk++){
             if (m_TmpNodeHPSystemFList.at(kk).strStep.toLower() != "step1") {//其他新加制度
                 if (m_TmpNodeHPSystemFList.at(kk-1).strStep.toLower() != m_TmpNodeHPSystemFList.at(kk).strStep.toLower()){
                    for (int mm=baseMM;mm<kk;mm++){
                        HPSystemFInpS tmpFdata;
                        tmpFdata.strStep=m_TmpNodeHPSystemFList.at(kk).strStep.toLower();
                        tmpFdata.strStyle="film";
                        tmpFdata.strOP="新建";//"op=new;
                        m_nodeHPSystemFList.append(tmpFdata);
                    } 
                    baseMM=kk;
                 }
                 HPSystemFInpS tmp=m_TmpNodeHPSystemFList.at(kk);
                 tmp.strOP="新建";
                 m_nodeHPSystemFList.append(tmp);
             }else{m_nodeHPSystemFList.append(m_TmpNodeHPSystemFList.at(kk));}
         }

         baseMM=0;
         for (int kk=0;kk<nRSize;kk++){
             if (m_TmpNodeHPSystemRList.at(kk).strStep.toLower() != "step1"&& kk>0) {//其他新加制度   
                 if (m_TmpNodeHPSystemRList.at(kk-1).strStep.toLower() != m_TmpNodeHPSystemRList.at(kk).strStep.toLower()){
                     for (int mm=baseMM;mm<kk;mm++){
                         HPSystemRInpS tmpRdata;
                         tmpRdata.strStep=m_TmpNodeHPSystemRList.at(kk).strStep.toLower();
                         tmpRdata.strStyle="radiate";
                         tmpRdata.strOP="新建";//"op=new;
                         m_nodeHPSystemRList.append(tmpRdata);
                     }
                     baseMM=kk;
                  }
                 HPSystemRInpS tmp=m_TmpNodeHPSystemRList.at(kk);
                 tmp.strOP="新建";
                 m_nodeHPSystemRList.append(tmp);
             }else{ m_nodeHPSystemRList.append(m_TmpNodeHPSystemRList.at(kk));}
          }
}
//将读取的INP数据显示在当前的表格中
void QHPSystemDlg::SetInpData(ReadInpResultS ReadInpData)
{
    //--init
    m_strListSurf.clear();//重新加载inp文件则进行清除;
	m_PSetList = ReadInpData.TmpSurfaceInps.strSurfaceName;
    int nRowSize = m_TableWiget->rowCount();
    for (int i=0;i<nRowSize;i++){ //清除全部内容，进行重新加载
        m_TableWiget->removeRow(0);
    }
    nRowSize=0;
    
    int nHpSysStepSize=ReadInpData.TmpSolveInps.size();;//求解制度个数
	for(int i=0;i<nHpSysStepSize;i++){
		QString str=ReadInpData.TmpSolveInps.at(i).strProcess;
		if(str=="7")nRowSize++;
	}
    m_strListHpSysStep.clear();//求解器名字；
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//if(nRowSize<=0)return;//当按照制度读取时需要取消注释
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	nRowSize=0;

    int nHpSysRowSize=0;//table 行数  
    for (int iKKRow=0;iKKRow<nHpSysStepSize;iKKRow++){
        m_strListHpSysStep<<QString("Step%1").arg(iKKRow+1);
    }
    if (ReadInpData.TmpHPSystemRInps.size()>0 || ReadInpData.TmpHPSystemFInps.size()>0){
         for (int tt=0;tt<ReadInpData.TmpHPSystemFInps.size();tt++){
            if (ReadInpData.TmpHPSystemFInps.at(tt).strStyle=="film" && ReadInpData.TmpHPSystemFInps.at(tt).strInclude=="1"){
                nHpSysRowSize++;
            }
        }
        for (int tt=0;tt<ReadInpData.TmpHPSystemRInps.size();tt++){
            if (ReadInpData.TmpHPSystemRInps.at(tt).strStyle=="radiate" && ReadInpData.TmpHPSystemRInps.at(tt).strInclude=="1"){
                nHpSysRowSize++;  
            }
        }
    }else{
        nHpSysRowSize=ReadInpData.TmpSolveInps.size();
    }

    //曲线名称
    int iCurveSize=ReadInpData.TmpCurveInps.size();
    m_strListCurveName.clear();
    for (int i=0;i<iCurveSize;i++){
        m_strListCurveName<<ReadInpData.TmpCurveInps.at(i).strName;
    }

    SurfFRS FRS;
    
    int iBaseNum=0;
    for (int tt=0;tt<ReadInpData.TmpSurfaceInps.strSurfaceName.size();tt++){
        int idataNum=ReadInpData.TmpSurfaceInps.strSurfaceDataNum.at(tt).toInt();
        for (int j=iBaseNum;j<iBaseNum+idataNum;j++){
            ReadInpData.TmpSurfaceInps.strData.at(j);
            FRS.strName<<ReadInpData.TmpSurfaceInps.strSurfaceName.at(tt);
            FRS.strNum<<ReadInpData.TmpSurfaceInps.strSurfaceDataNum.at(tt);
            FRS.strInclude<<ReadInpData.TmpSurfaceInps.strData.at(j).split(",",QString::SkipEmptyParts).at(0);
            FRS.strIncludeSurfNum<<ReadInpData.TmpSurfaceInps.strData.at(j).split(",",QString::SkipEmptyParts).at(1);
        }
        iBaseNum+=idataNum;
    }
    m_surfFRS=FRS;

    for(int tt=0;tt<ReadInpData.TmpSurfaceInps.strData.size();tt++){
        m_strListSurf<<ReadInpData.TmpSurfaceInps.strData.at(tt).split(",",QString::SkipEmptyParts).at(0);
    }

     if (nRowSize<nHpSysRowSize)
     {
       for (int i=nRowSize;i<nHpSysRowSize;i++){
            m_TableWiget->insertRow(i);
            MyCombox *comboxPSet0=new MyCombox(this,i);
            m_TableWiget->setCellWidget(i,0,comboxPSet0);
            m_comboxPSetSurf1.insert(i,comboxPSet0);
            connect(comboxPSet0,SIGNAL(clicked(int,int)),this,SLOT(comboxSlot()));

            MyCombox *TmpCombox=new MyCombox(this,0);
            TmpCombox->addItem("constant");
            TmpCombox->addItem("F(t)");
            TmpCombox->setFrame(false);
            TmpCombox->setCurrentIndex(0);
            m_TableWiget->setCellWidget(i,COL_SYS_TEMPER,TmpCombox);
            m_ComboxTemper.insert(i,TmpCombox);
            connect(m_ComboxTemper[i],SIGNAL(clicked(int,int)),this,SLOT(comboxSlot()));

            TmpCombox=new MyCombox(this,0);
            TmpCombox->addItem("no");
            TmpCombox->addItem("constant");
            TmpCombox->addItem("F(t)");
            TmpCombox->setFrame(false);
            TmpCombox->setCurrentIndex(0);
            m_TableWiget->setCellWidget(i,COL_SYS_FILM,TmpCombox);
            m_ComboxFilm.insert(i,TmpCombox);
            connect(m_ComboxFilm[i],SIGNAL(clicked(int,int)),this,SLOT(comboxSlot()));
           // connect(m_ComboxFilm[i],SIGNAL(currentIndexChanged(int)),this,SLOT(comboxChangeSlot())); 

            TmpCombox=new MyCombox(this,0);
            TmpCombox->addItem("no");
            TmpCombox->addItem("constant");
            TmpCombox->addItem("F(t)");
            TmpCombox->setFrame(false);
            TmpCombox->setCurrentIndex(0);
            m_TableWiget->setCellWidget(i,COL_SYS_RADIATE,TmpCombox);
            m_ComboxRadiate.insert(i,TmpCombox);
            connect(m_ComboxRadiate[i],SIGNAL(clicked(int,int)),this,SLOT(comboxSlot()));
            //connect(m_ComboxRadiate[i],SIGNAL(currentIndexChanged(int)),this,SLOT(comboxChangeSlot()));

            TmpCombox=new MyCombox(this,0);
            TmpCombox->addItems(m_strListHpSysStep);
            TmpCombox->addItem(tr("新建"));
            TmpCombox->addItem(tr("删除"));
            TmpCombox->setFrame(false);
            TmpCombox->setCurrentIndex(0);
            m_TableWiget->setCellWidget(i,COL_SYS_SYSTEM,TmpCombox);
            m_ComboxSystem.insert(i,TmpCombox); 
            connect(m_ComboxSystem[i],SIGNAL(clicked(int,int)),this,SLOT(comboxSlot()));
         }
      }else if (nRowSize>nHpSysRowSize){ //clear and remove;
        for (int i=nHpSysRowSize;i<nRowSize;i++){
            m_TableWiget->removeRow(nHpSysRowSize);
        }
      }
     for (int mm=0;mm<nHpSysRowSize;mm++){//clear & init
         m_ComboxFilm[mm]->setCurrentIndex(0);
         m_ComboxRadiate[mm]->setCurrentIndex(0);
     }

        nRowSize = m_TableWiget->rowCount();
      
        int i=0,j=0;
        for (i=0;i<nRowSize;i++){
            MyCombox *tmpCombox =(MyCombox*)m_TableWiget->cellWidget(i, 0);
            tmpCombox->clear();
            for (int j=0;j<ReadInpData.TmpSurfaceInps.strSurfaceName.size();j++){
                if (ReadInpData.TmpSurfaceInps.strSurfaceName.at(j)!=""){
                    tmpCombox->addItem(ReadInpData.TmpSurfaceInps.strSurfaceName.at(j));
                    tmpCombox->setItemData(j,"2",(Qt::UserRole+3));
                }
            } 
            tmpCombox->addItem("新建"); 
            m_comboxPSetSurf1.insert(i,tmpCombox);
            connect(tmpCombox,SIGNAL(clicked(int,int)),this,SLOT(comboxSlot()));

            MyCombox *TmpCombox = (MyCombox*)m_TableWiget->cellWidget(i, COL_SYS_SYSTEM);
            TmpCombox->clear();
            TmpCombox->addItems(m_strListHpSysStep);
            TmpCombox->addItem(tr("新建"));
            TmpCombox->addItem(tr("删除"));
            TmpCombox->setFrame(false);
            m_ComboxSystem.insert(i,TmpCombox); 
            connect(m_ComboxSystem[i],SIGNAL(clicked(int,int)),this,SLOT(comboxSlot()));

            //--环境
            TmpCombox = (MyCombox*)m_TableWiget->cellWidget(i, COL_SYS_TEMPER);
            TmpCombox->addItems(m_strListCurveName);
            for (int ss=2;ss<2+iCurveSize;ss++){
                static_cast<QStandardItemModel*>(TmpCombox->view()->model())->item(ss,0)->setTextAlignment(Qt::AlignRight);
            }
            m_ComboxTemper.insert(i,TmpCombox);
            //--对流
            TmpCombox = (MyCombox*)m_TableWiget->cellWidget(i, COL_SYS_FILM);
            TmpCombox->addItems(m_strListCurveName);
            for (int ss=3;ss<3+iCurveSize;ss++){
                static_cast<QStandardItemModel*>(TmpCombox->view()->model())->item(ss,0)->setTextAlignment(Qt::AlignRight);
            }
            m_ComboxFilm.insert(i,TmpCombox);
            //--辐射
            TmpCombox = (MyCombox*)m_TableWiget->cellWidget(i, COL_SYS_RADIATE);
            TmpCombox->addItems(m_strListCurveName);
            for (int ss=3;ss<3+iCurveSize;ss++){
                static_cast<QStandardItemModel*>(TmpCombox->view()->model())->item(ss,0)->setTextAlignment(Qt::AlignRight);
            }
            m_ComboxRadiate.insert(i,TmpCombox);
       
        } 

        int iKKRow=0;
        for (int tt=0;tt<ReadInpData.TmpHPSystemFInps.size();tt++)//FIME查找所属那个面//--查找并显示
        {
            if (ReadInpData.TmpHPSystemFInps.at(tt).strStyle=="film" && ReadInpData.TmpHPSystemFInps.at(tt).strInclude=="1"){
                //--pset
                QString str=ReadInpData.TmpHPSystemFInps.at(tt).strElsetName;
                int id= FRS.strInclude.indexOf(str);
                QString strTmpName=FRS.strName.at(id);
                id=m_comboxPSetSurf1[iKKRow]->findText(strTmpName);
                if (id!=-1){
                    m_comboxPSetSurf1[iKKRow]->setCurrentIndex(id);
                }

                //--system -step 
                str=ReadInpData.TmpHPSystemFInps.at(tt).strStep;
                m_ComboxSystem[iKKRow]->setCurrentIndex(str.toInt()-1);
                m_ComboxSystem.insert(iKKRow,m_ComboxSystem[iKKRow]);

                //--Environment Temper
                str=ReadInpData.TmpHPSystemFInps.at(tt).strAmplitudeName;//has name,f(t)->otherwise constant  
                if (str==""){//constant(strEnvirTemprat)
                    str="constant";
                    QTemperateDialog *NewDlg = new QTemperateDialog(this);
                    NewDlg->SetShowStyle(0);
                    NewDlg->setData(0,ReadInpData.TmpHPSystemFInps.at(tt).strEnvirTemprat);
                    str+="--"+ReadInpData.TmpHPSystemFInps.at(tt).strEnvirTemprat;
                    m_TemperateDlg.insert(str,NewDlg);
                    m_ComboxTemper[iKKRow]->setItemText(0,str);
                    m_ComboxTemper[iKKRow]->setCurrentIndex(0);
                    //double d= ReadInpData.TmpHPSystemFInps.at(tt).strEnvirTemprat.toDouble();
                    //m_ComboxTemper[iKKRowLine]->setItemData(iKKRowLine, d);
                }else{    
                   int iTemperId= m_ComboxTemper[iKKRow]->findText(str);
                   m_ComboxTemper[iKKRow]->setCurrentIndex(iTemperId);
               }
                //--film
                str=ReadInpData.TmpHPSystemFInps.at(tt).strFilmRadiateAmpName;//has name,f(t)->otherwise constant  
                if (str==""){//constant(strfilmeAmp)
                    str="constant";
                    QFilmDialog *NewDlg = new QFilmDialog(this);
                    NewDlg->SetShowStyle(0);
                    NewDlg->setData(0,ReadInpData.TmpHPSystemFInps.at(tt).strCofficient);
                    str+="--"+ReadInpData.TmpHPSystemFInps.at(tt).strCofficient;
                    m_FilmDlg.insert(str,NewDlg);
                    m_ComboxFilm[iKKRow]->setItemText(1,str);
                    m_ComboxFilm[iKKRow]->setCurrentIndex(1);
                }else{
                    /*for (int ff=0;ff<iCurveSize;ff++){ 
                        QFilmDialog *NewDlg = new QFilmDialog(this);
                        NewDlg->SetShowStyle(1);
                        NewDlg->setData(1,ReadInpData.TmpCurveInps.at(ff));
                        m_FilmDlg.insert(ReadInpData.TmpCurveInps.at(ff).strName,NewDlg);
                    }*/
                    int iTemperId= m_ComboxFilm[iKKRow]->findText(str);
                    m_ComboxFilm[iKKRow]->setCurrentIndex(iTemperId);
                }             
                iKKRow++;
            }
        }
         for (int tt=0;tt<ReadInpData.TmpHPSystemRInps.size();tt++){//Radiate查找所属那个面//--查找并显示
             if (ReadInpData.TmpHPSystemRInps.at(tt).strStyle=="radiate" && ReadInpData.TmpHPSystemRInps.at(tt).strInclude=="1"){
                QString str=ReadInpData.TmpHPSystemRInps.at(tt).strElsetName;
                int id= FRS.strInclude.indexOf(str);
                QString strTmpName=FRS.strName.at(id);
                id=m_comboxPSetSurf1[iKKRow]->findText(strTmpName);
                if (id!=-1){
                    m_comboxPSetSurf1[iKKRow]->setCurrentIndex(id);
                }
                //--system -step  
                str=ReadInpData.TmpHPSystemRInps.at(tt).strStep;
                m_ComboxSystem[iKKRow]->setCurrentIndex(str.toInt()-1);
                m_ComboxSystem.insert(iKKRow,m_ComboxSystem[iKKRow]);
              
                //--Environment Temper
                str=ReadInpData.TmpHPSystemRInps.at(tt).strAmplitudeName;//has name,f(t)->otherwise constant  
                if (str==""){//constant(strEnvirTemprat)
                    str="constant";
                    QTemperateDialog *NewDlg = new QTemperateDialog(this);
                    NewDlg->SetShowStyle(0);
                    NewDlg->setData(0,ReadInpData.TmpHPSystemRInps.at(tt).strEnvirTemprat);
                    str+="--"+ReadInpData.TmpHPSystemRInps.at(tt).strEnvirTemprat;   
                    m_TemperateDlg.insert(str,NewDlg);
                    m_ComboxTemper[iKKRow]->setItemText(0,str);
                    m_ComboxTemper[iKKRow]->setCurrentIndex(0);
                }else{ 
                   /* for (int ff=0;ff<iCurveSize;ff++){ 
                        QTemperateDialog *NewDlg = new QTemperateDialog(this);
                        NewDlg->SetShowStyle(1);   
                        NewDlg->setData(1,ReadInpData.TmpCurveInps.at(ff));
                        m_TemperateDlg.insert(str,NewDlg);
                    }*/
                    int iTemperId= m_ComboxTemper[iKKRow]->findText(str);
                    m_ComboxTemper[iKKRow]->setCurrentIndex(iTemperId);
                }
               //--radiate
                str=ReadInpData.TmpHPSystemRInps.at(tt).strFilmRadiateAmpName;//has name,f(t)->otherwise constant  
                if (str==""){//constant(strfilmeAmp)
                    str="constant";
                    QRadiateDialog *NewDlg = new QRadiateDialog(this);
                    NewDlg->SetShowStyle(0);
                    NewDlg->setData(0,ReadInpData.TmpHPSystemRInps.at(tt).strCofficient);
                    str+="--"+ReadInpData.TmpHPSystemRInps.at(tt).strCofficient;
                    m_RadiateDlg.insert(str,NewDlg);
                    m_ComboxRadiate[iKKRow]->setItemText(1,str);
                    m_ComboxRadiate[iKKRow]->setCurrentIndex(1);
                }else{
                    
                    int iTemperId= m_ComboxRadiate[iKKRow]->findText(str);
                    m_ComboxRadiate[iKKRow]->setCurrentIndex(iTemperId);
                } 

               iKKRow++;
            }
        }

         //--dlg
         for (int ff=0;ff<iCurveSize;ff++){
             QTemperateDialog *NewDlg = new QTemperateDialog(this);
             NewDlg->SetShowStyle(1);
             NewDlg->setData(1,ReadInpData.TmpCurveInps.at(ff));
             m_TemperateDlg.insert(ReadInpData.TmpCurveInps.at(ff).strName,NewDlg); 

         }
         for (int ff=0;ff<iCurveSize;ff++){ 
             QRadiateDialog *NewDlg = new QRadiateDialog(this);
             NewDlg->SetShowStyle(1); 
             NewDlg->setData(1,ReadInpData.TmpCurveInps.at(ff));
             m_RadiateDlg.insert(ReadInpData.TmpCurveInps.at(ff).strName,NewDlg);
         }  
         for (int ff=0;ff<iCurveSize;ff++){ 
             QFilmDialog *NewDlg = new QFilmDialog(this);
             NewDlg->SetShowStyle(1);
             NewDlg->setData(1,ReadInpData.TmpCurveInps.at(ff));
             m_FilmDlg.insert(ReadInpData.TmpCurveInps.at(ff).strName,NewDlg);
         }
    WriteHPsysToInp();
}
void QHPSystemDlg::AddHPSysLine(int nCurRow)
{
    int nRow=m_TableWiget->rowCount();
    if (nRow<0)return;
    if (nCurRow==0){
        MyCombox *comboxPSet0=new MyCombox(this, nCurRow);
        if(m_PSetList.count()){
            for (int kk=0;kk<m_PSetList.count();kk++){
                comboxPSet0->addItem(m_PSetList.at(kk));
                comboxPSet0->setItemData(kk,"2",(Qt::UserRole+3));
            } 
        }else{
            MyCombox *ff = new MyCombox(this,m_comboxPSetSurf1.count());
            for (int i=0;i<nRow;i++){
                ff=(MyCombox*)(m_TableWiget->cellWidget(i,0));
                if (ff!=NULL){
                    for (int kk=0;kk<ff->count();kk++){
                        comboxPSet0->addItem(ff->itemText(kk));
                        comboxPSet0->setItemData(kk,"2",(Qt::UserRole+3));
                    }
					 break;
				}
            }
        }
        m_comboxPSetSurf1.insert(nCurRow,comboxPSet0);
        connect(comboxPSet0,SIGNAL(clicked(int,int)),this,SLOT(comboxSlot()));
        m_TableWiget->setCellWidget(nCurRow,0,comboxPSet0);

        MyCombox *TmpCombox=new MyCombox(this,0);
        TmpCombox->addItem("constant");
        TmpCombox->addItem("F(t)");
        if (m_strListCurveName.size()>0){
            TmpCombox->addItems(m_strListCurveName);
        }
        TmpCombox->setFrame(false);
        m_TableWiget->setCellWidget(nCurRow,COL_SYS_TEMPER,TmpCombox);
        m_ComboxTemper.insert(nCurRow,TmpCombox);
        connect(m_ComboxTemper[nCurRow],SIGNAL(clicked(int,int)),this,SLOT(comboxSlot()));

        TmpCombox=new MyCombox(this,0);
        TmpCombox->addItem("no");
        TmpCombox->addItem("constant");
        TmpCombox->addItem("F(t)");
        if (m_strListCurveName.size()>0){
            TmpCombox->addItems(m_strListCurveName);
        }
        TmpCombox->setFrame(false);
        m_TableWiget->setCellWidget(nCurRow,COL_SYS_FILM,TmpCombox);
        m_ComboxFilm.insert(nCurRow,TmpCombox);
        connect(m_ComboxFilm[nCurRow],SIGNAL(clicked(int,int)),this,SLOT(comboxSlot()));
        //connect(m_ComboxFilm[nRow],SIGNAL(currentIndexChanged(int)),this,SLOT(comboxChangeSlot()));

        TmpCombox=new MyCombox(this,0);
        TmpCombox->addItem("no");
        TmpCombox->addItem("constant");
        TmpCombox->addItem("F(t)");
        if (m_strListCurveName.size()>0){
            TmpCombox->addItems(m_strListCurveName);
        }
        TmpCombox->setFrame(false);
        m_TableWiget->setCellWidget(nCurRow,COL_SYS_RADIATE,TmpCombox);
        m_ComboxRadiate.insert(nCurRow,TmpCombox);
        connect(m_ComboxRadiate[nCurRow],SIGNAL(clicked(int,int)),this,SLOT(comboxSlot()));
        //connect(m_ComboxRadiate[nRow],SIGNAL(currentIndexChanged(int)),this,SLOT(comboxChangeSlot()));

        TmpCombox=new MyCombox(this,0);
        TmpCombox->addItem(tr("step1"));
        TmpCombox->addItem(tr("新建"));
        TmpCombox->addItem(tr("删除"));
        TmpCombox->setFrame(false);
        m_TableWiget->setCellWidget(nCurRow,COL_SYS_SYSTEM,TmpCombox);
        m_ComboxSystem.insert(nCurRow,TmpCombox); 
        connect(m_ComboxSystem[nCurRow],SIGNAL(clicked(int,int)),this,SLOT(comboxSlot()));
        return;
    }

    MyCombox *comboxPSet0=new MyCombox(this, nCurRow);
    if(m_PSetList.count()){
        for (int kk=0;kk<m_PSetList.count();kk++){
            comboxPSet0->addItem(m_PSetList.at(kk));
            comboxPSet0->setItemData(kk,"2",(Qt::UserRole+3));
        } 
    }else{
        MyCombox *ff = new MyCombox(this,m_comboxPSetSurf1.count());
        for (int i=0;i<nRow;i++){
            ff=(MyCombox*)(m_TableWiget->cellWidget(i,0));
            if (ff!=NULL){
                for (int kk=0;kk<ff->count();kk++){
                    comboxPSet0->addItem(ff->itemText(kk));
                    comboxPSet0->setItemData(kk,"2",(Qt::UserRole+3));
                }
                break;
            }
        }
    }
    m_comboxPSetSurf1.insert(nCurRow,comboxPSet0);
    connect(comboxPSet0,SIGNAL(clicked(int,int)),this,SLOT(comboxSlot()));
    m_TableWiget->setCellWidget(nCurRow,0,comboxPSet0);
    //--环境
    MyCombox *TmpCombox = new MyCombox(this,0);
    QStringList strList;
    for (int fff=0;fff<((MyCombox*)m_TableWiget->cellWidget(0, COL_SYS_TEMPER))->count();fff++){
        strList<<((MyCombox*)m_TableWiget->cellWidget(0, COL_SYS_TEMPER))->itemText(fff);
    }
    TmpCombox->addItems(strList);

    for (int tt=2;tt<TmpCombox->count();tt++){
        static_cast<QStandardItemModel*>(TmpCombox->view()->model())->item(tt,0)->setTextAlignment(Qt::AlignRight);
    }
    m_TableWiget->setCellWidget(nCurRow,COL_SYS_TEMPER,TmpCombox);
    m_ComboxTemper.insert(nCurRow,TmpCombox);
    connect(m_ComboxTemper[nCurRow],SIGNAL(clicked(int,int)),this,SLOT(comboxSlot()));
    //--对流
    TmpCombox=new MyCombox(this,0);//(MyCombox*)m_TableWiget->cellWidget(0, COL_SYS_FILM); 
    strList.clear();
    for (int fff=0;fff<((MyCombox*)m_TableWiget->cellWidget(0, COL_SYS_FILM))->count();fff++){
        strList<<((MyCombox*)m_TableWiget->cellWidget(0, COL_SYS_FILM))->itemText(fff);
    }
    TmpCombox->addItems(strList);
    for (int tt=3;tt<TmpCombox->count();tt++){
        static_cast<QStandardItemModel*>(TmpCombox->view()->model())->item(tt,0)->setTextAlignment(Qt::AlignRight);
    }
    m_TableWiget->setCellWidget(nCurRow,COL_SYS_FILM,TmpCombox);
    m_ComboxFilm.insert(nCurRow,TmpCombox);
    connect(m_ComboxFilm[nCurRow],SIGNAL(clicked(int,int)),this,SLOT(comboxSlot()));
    //connect(m_ComboxFilm[nCurRow],SIGNAL(currentIndexChanged(int)),this,SLOT(comboxChangeSlot()));
    //--辐射
    TmpCombox=new MyCombox(this,0);//(MyCombox*)m_TableWiget->cellWidget(0, COL_SYS_RADIATE);
    TmpCombox->addItems(strList);
    for (int tt=3;tt<TmpCombox->count();tt++){
        static_cast<QStandardItemModel*>(TmpCombox->view()->model())->item(tt,0)->setTextAlignment(Qt::AlignRight);
    }
    m_TableWiget->setCellWidget(nCurRow,COL_SYS_RADIATE,TmpCombox);
    m_ComboxRadiate.insert(nCurRow,TmpCombox);
    connect(m_ComboxRadiate[nCurRow],SIGNAL(clicked(int,int)),this,SLOT(comboxSlot())); 
    //connect(m_ComboxRadiate[nCurRow],SIGNAL(currentIndexChanged(int)),this,SLOT(comboxChangeSlot()));
   
    TmpCombox=new MyCombox(this,0);
    MyCombox *ff =(MyCombox*)(m_TableWiget->cellWidget(0,COL_SYS_SYSTEM));
    if (ff!=NULL){
        for (int kk=0;kk<ff->count();kk++){
            TmpCombox->addItem(ff->itemText(kk));
            TmpCombox->setFrame(false);
        }
        if (ff->count()==0){
            TmpCombox->addItem("新建");
            TmpCombox->addItem("删除");
            TmpCombox->setFrame(false);
        }
    }
    m_TableWiget->setCellWidget(nCurRow,COL_SYS_SYSTEM,TmpCombox);
    m_ComboxSystem.insert(nCurRow,TmpCombox); 
    connect(m_ComboxSystem[nCurRow],SIGNAL(clicked(int,int)),this,SLOT(comboxSlot()));
}
//增加
void QHPSystemDlg::AddBtnHPSlot()
{
	int nRow =m_TableWiget->rowCount();
	m_TableWiget->insertRow(nRow);
    AddHPSysLine(nRow);
}
//插入
void QHPSystemDlg::InsertBtnHPSlot()
{
    int curRow = m_TableWiget->currentRow();
    m_TableWiget->insertRow(curRow);
    AddHPSysLine(curRow);
}
//删除
void QHPSystemDlg::DelBtnHPSlot()
{
    int curRow =m_TableWiget->currentRow();
    if (curRow!=-1){
        m_TableWiget->removeRow(curRow);
    }
}
/*
功能:下一步
描述:保存热处理制度及曲线数据
*/
void QHPSystemDlg::NextBtnHPSlot()
{
    int iStepSize=m_strListHpSysStep.size();
    int nRow =m_TableWiget->rowCount();
    if (nRow>0)
    {
        MyCombox *tmpBtn = (MyCombox*)m_TableWiget->cellWidget(0, COL_SYS_SYSTEM);
        int id =m_ComboxSystem.key(tmpBtn);
        if (id!=-1){
             iStepSize=(tmpBtn->count());
             if ((tmpBtn->findText("删除",Qt::MatchCaseSensitive))>0){
                iStepSize--;
            }
            if ((tmpBtn->findText("新建",Qt::MatchCaseSensitive)>0)){
                iStepSize--;
            }    
        }     
    }
    emitHPsystemNext(iStepSize);
    WriteHPsysToInp();
    this->accept();
}
void QHPSystemDlg::PreBtnHPSlot()
{
    WriteHPsysToInp();
    this->accept();
}
//--设置radiate 与 film 互斥属性
void QHPSystemDlg::comboxChangeSlot()
{
    //MyCombox *btn=(MyCombox*) sender();
    //int x=btn->mapToParent(QPoint(0,0)).x();
    //int y=btn->mapToParent(QPoint(0,0)).y();
    //QModelIndex index= m_TableWiget->indexAt(QPoint(x,y));

    //int iRow=index.row();
    //int iCol=index.column();

    //if (iRow<0 || iCol<0) return;
    //MyCombox *tmpBtn = (MyCombox*)m_TableWiget->cellWidget(iRow, iCol);
    //switch(iCol){
    //     case COL_SYS_FILM:{
    //         int id =m_ComboxFilm.key(tmpBtn);
    //         if (id==-1)return;
    //         int ind=m_ComboxFilm[id]->currentIndex();
    //         QString str= m_ComboxFilm[id]->currentText();
    //         MyCombox* RadiateBtn =(MyCombox*)m_TableWiget->cellWidget(iRow,COL_SYS_RADIATE);
    //         RadiateBtn->setEnabled(false);
    //         if (str=="no") {//将Radiate进行显示可使用
    //             RadiateBtn->setEnabled(true);
    //         }
    //        }break;
    //     case COL_SYS_RADIATE:{
    //         int id =m_ComboxRadiate.key(tmpBtn);
    //         if (id==-1)return;
    //         int ind=m_ComboxRadiate[id]->currentIndex();
    //         QString str= m_ComboxRadiate[id]->currentText();
    //         MyCombox* FilmBtn =(MyCombox*)m_TableWiget->cellWidget(iRow,COL_SYS_FILM);
    //         FilmBtn->setEnabled(false);
    //         if (str=="no"){//函数
    //             FilmBtn->setEnabled(true);
    //         }
    //      }break;
    //     default:break;
    //}
  
}

void QHPSystemDlg::comboxSlot()
{  
    m_ListWidget->hide();
    MyCombox *btn=(MyCombox*) sender();
    int x=btn->mapToParent(QPoint(0,0)).x();
    int y=btn->mapToParent(QPoint(0,0)).y();
    QModelIndex index= m_TableWiget->indexAt(QPoint(x,y));

    int iRow=index.row();
    int iCol=index.column();

    if (iRow<0 || iCol<0) return;
    MyCombox *tmpBtn = (MyCombox*)m_TableWiget->cellWidget(iRow, iCol);
    switch(iCol){
        case COL_SYS_PSET:{
            int id=m_comboxPSetSurf1.key(tmpBtn);
            if (id==-1)return;
            int ind= m_comboxPSetSurf1[id]->currentIndex();
            NElSurfChsS tmpChs;
            tmpChs.strName =m_comboxPSetSurf1[id]->currentText();
            tmpChs.iType = m_comboxPSetSurf1[id]->itemData(ind,Qt::UserRole+3).toInt();
            emit emitPsetHighLight(tmpChs);
          }break;
        case COL_SYS_TEMPER:{
            int id =m_ComboxTemper.key(tmpBtn);
            if (id==-1)return;
            int ind=m_ComboxTemper[id]->currentIndex();
            QString str= m_ComboxTemper[id]->currentText();
            if (str.contains("constant")){// == "constant"){//数据为常量

                if(m_TemperateDlg.find(str)!=m_TemperateDlg.end()){
                    m_TemperateDlg[str]->SetShowStyle(0);
                    QString testStr1=m_TemperateDlg[str]->m_LineValue->text();
                    if (execDialogInFront(m_TemperateDlg[str])){
                        QString newStr="constant--"+m_TemperateDlg[str]->m_LineValue->text();
                       if (testStr1 != newStr){
                           QTemperateDialog *newDlg = new QTemperateDialog(this);
                           newDlg->m_LineValue->setText(m_TemperateDlg[str]->m_LineValue->text());
                           m_TemperateDlg.insert(newStr,newDlg);
                       }
					   m_TemperateDlg.insert(str,m_TemperateDlg["constant--"+testStr1]);
					   m_TemperateDlg[str]->m_LineValue->setText(testStr1);
                       m_ComboxTemper[id]->setItemText(0,newStr);
                    }
                }else{//new dlg
                    QTemperateDialog *NewDlg = new QTemperateDialog(this);
                    NewDlg->SetShowStyle(0);
					if (execDialogInFront(NewDlg)){
                       m_TemperateDlg.insert("constant--"+NewDlg->m_LineValue->text(),NewDlg);
					   m_ComboxTemper[id]->setItemText(0,"constant--"+NewDlg->m_LineValue->text());
					}
                }
            }else if (str=="F(t)"){//函数--NEW F(t)
                QTemperateDialog *NewDlg = new QTemperateDialog(this);
                NewDlg->SetShowStyle(1);
                if (execDialogInFront(NewDlg)){
                    str=NewDlg->m_LineName->text();
                    m_TemperateDlg.insert(str,NewDlg);
                    InpCurveInpS nodeCurveList;
                    nodeCurveList=m_TemperateDlg[str]->GetData(1,nodeCurveList);
                   
                    QFilmDialog *NewFDlg = new QFilmDialog(this);
                    NewFDlg->SetShowStyle(1);
                    NewFDlg->setData(1, nodeCurveList);
                    m_FilmDlg.insert(str,NewFDlg);
                    QRadiateDialog *NewRDlg = new QRadiateDialog(this);
                    NewRDlg->SetShowStyle(1);
                    NewRDlg->setData(1,nodeCurveList);
                    m_RadiateDlg.insert(str,NewRDlg);
                    comboxAppendText(tmpBtn,nodeCurveList.strName); 
                    QStringList combList;combList.clear();
                    for (int kk=0;kk<tmpBtn->count();kk++){//保存
                        if(tmpBtn->itemText(kk).toUpper()=="NO"||tmpBtn->itemText(kk).toUpper()=="CONSTANT"||tmpBtn->itemText(kk).toUpper()=="F(T)")continue;
                        combList<<tmpBtn->itemText(kk);
                    }
                    emitHPCurve(combList,nodeCurveList);
                }  
            }else{//其他combox成员
                if(m_TemperateDlg.find(str)!=m_TemperateDlg.end()){
                    m_TemperateDlg[str]->SetShowStyle(1);
                    if (execDialogInFront(m_TemperateDlg[str])){
                        QString strLineName=m_TemperateDlg[str]->m_LineName->text();
                        m_TemperateDlg.insert(strLineName, m_TemperateDlg[str]); 
                        InpCurveInpS nodeCurveList;
                        nodeCurveList=m_TemperateDlg[strLineName]->GetData(1,nodeCurveList);
                        //insertToCurveList(nodeCurveList);
                        comboxChangeText(str,strLineName);
                        QStringList combList;combList.clear();
                        for (int kk=0;kk<tmpBtn->count();kk++){//保存
                            if(tmpBtn->itemText(kk).toUpper()=="NO"||tmpBtn->itemText(kk).toUpper()=="CONSTANT"||tmpBtn->itemText(kk).toUpper()=="F(T)")continue;
                            combList<<tmpBtn->itemText(kk);
                        }   
                        emitHPCurve(combList,nodeCurveList);  
                        m_FilmDlg[str]->setData(1,nodeCurveList);
                        m_FilmDlg.insert(strLineName,m_FilmDlg[str]);
                        m_RadiateDlg[str]->setData(1,nodeCurveList);
                        m_RadiateDlg.insert(strLineName,m_RadiateDlg[str]); 
                    }
                }
            }
          }break;
        case COL_SYS_FILM:{
            int id =m_ComboxFilm.key(tmpBtn);
            if (id==-1)return;
            int ind=m_ComboxFilm[id]->currentIndex();
            QString str= m_ComboxFilm[id]->currentText();
            //MyCombox* RadiateBtn =(MyCombox*)m_TableWiget->cellWidget(iRow,COL_SYS_RADIATE);
            //RadiateBtn->setEnabled(false);
            if (str.contains("constant")){//数据为常量
                if(m_FilmDlg.find(str)!=m_FilmDlg.end()){
                    m_FilmDlg[str]->SetShowStyle(0);
                    QString testStr1=m_FilmDlg[str]->m_LineValue->text();
                    if (execDialogInFront(m_FilmDlg[str])){
                        QString newStr="constant--"+m_FilmDlg[str]->m_LineValue->text();
                        if (testStr1 != newStr){
                            QFilmDialog *newDlg=new QFilmDialog(this);
                            newDlg->m_LineValue->setText(m_FilmDlg[str]->m_LineValue->text());
                            m_FilmDlg.insert(newStr,newDlg);
                        }
                        //m_FilmDlg.insert(str,m_FilmDlg[str]);
                        m_FilmDlg.insert(str,m_FilmDlg["constant--"+testStr1]);
                        m_FilmDlg[str]->m_LineValue->setText(testStr1);
                        m_ComboxFilm[id]->setItemText(1,newStr);
                    }
                }else{//new dlg
                    QFilmDialog *NewDlg = new QFilmDialog(this);
                    NewDlg->SetShowStyle(0);
					if (execDialogInFront(NewDlg)){
						m_FilmDlg.insert("constant--"+NewDlg->m_LineValue->text(),NewDlg);
						m_ComboxFilm[id]->setItemText(1,"constant--"+NewDlg->m_LineValue->text());
					}      
                }
            }else if (str=="F(t)"){//函数--NEW F(t)
                QFilmDialog *NewDlg = new QFilmDialog(this);
                NewDlg->SetShowStyle(1);
                if (execDialogInFront(NewDlg)){
                    str=NewDlg->m_LineName->text();
                    m_FilmDlg.insert(str,NewDlg);
                    InpCurveInpS nodeCurveList;
                    nodeCurveList=m_FilmDlg[str]->GetData(1,nodeCurveList);

                    QTemperateDialog *NewTDlg = new QTemperateDialog(this);
                    NewTDlg->SetShowStyle(1);
                    NewTDlg->setData(1, nodeCurveList);
                    m_TemperateDlg.insert(str,NewTDlg);
                    QRadiateDialog *NewRDlg = new QRadiateDialog(this);
                    NewRDlg->SetShowStyle(1);
                    NewRDlg->setData(1,nodeCurveList);
                    m_RadiateDlg.insert(str,NewRDlg);
                    comboxAppendText(tmpBtn,nodeCurveList.strName);
                    QStringList combList;combList.clear();
                    for (int kk=0;kk<tmpBtn->count();kk++){//保存
                        if(tmpBtn->itemText(kk).toUpper()=="NO"||tmpBtn->itemText(kk).toUpper()=="CONSTANT"||tmpBtn->itemText(kk).toUpper()=="F(T)")continue;
                        combList<<tmpBtn->itemText(kk);
                    }
                     
                    emitHPCurve(combList,nodeCurveList);
                }
            }else if (str=="no") {//将Radiate进行显示可使用
               //RadiateBtn->setEnabled(true);
            }else{
                if(m_FilmDlg.find(str)!=m_FilmDlg.end()){
                    m_FilmDlg[str]->SetShowStyle(1);
                    if (execDialogInFront(m_FilmDlg[str])){
                        QString strLineName=m_FilmDlg[str]->m_LineName->text();
                        m_FilmDlg.insert(strLineName,m_FilmDlg[str]);
                        InpCurveInpS nodeCurveList;
                        nodeCurveList=m_FilmDlg[strLineName]->GetData(1,nodeCurveList);
                        //insertToCurveList(nodeCurveList);
                        comboxChangeText(str,strLineName);
                        QStringList combList;combList.clear();
                        for (int kk=0;kk<tmpBtn->count();kk++){//保存
                            if(tmpBtn->itemText(kk).toUpper()=="NO"||tmpBtn->itemText(kk).toUpper()=="CONSTANT"||tmpBtn->itemText(kk).toUpper()=="F(T)")continue;
                            combList<<tmpBtn->itemText(kk);
                        }
                        
                        emitHPCurve(combList,nodeCurveList);
                        m_TemperateDlg[str]->setData(1,nodeCurveList);
                        m_TemperateDlg.insert(strLineName,m_TemperateDlg[str]);
                        m_RadiateDlg[str]->setData(1,nodeCurveList);
                        m_RadiateDlg.insert(strLineName,m_RadiateDlg[str]);   
                    }
                }
            }
          }break;
        case COL_SYS_RADIATE:{
            int id =m_ComboxRadiate.key(tmpBtn);
            if (id==-1)return;
            int ind=m_ComboxRadiate[id]->currentIndex();
            QString str= m_ComboxRadiate[id]->currentText();
            //MyCombox* FilmBtn =(MyCombox*)m_TableWiget->cellWidget(iRow,COL_SYS_FILM);
            //FilmBtn->setEnabled(false);
            //if (str == "constant"){//数据为常量
            //    if(m_RadiateDlg.find(str)!=m_RadiateDlg.end()){
            //        m_RadiateDlg[str]->SetShowStyle(0);
            //        if (m_RadiateDlg[str]->exec())
            //            m_RadiateDlg.insert(str,m_RadiateDlg[str]);
            //    }else{//new dlg
            //        QRadiateDialog *NewDlg = new QRadiateDialog(this);
            //        NewDlg->SetShowStyle(0);
            //        if (NewDlg->exec())
            //            m_RadiateDlg.insert(str,NewDlg);
            //    }
            if (str.contains("constant")){//数据为常量
                if(m_RadiateDlg.find(str)!=m_RadiateDlg.end()){
                    m_RadiateDlg[str]->SetShowStyle(0);
                    QString testStr1=m_RadiateDlg[str]->m_LineValue->text();
                    if (execDialogInFront(m_RadiateDlg[str])){
                        QString newStr="constant--"+m_RadiateDlg[str]->m_LineValue->text();
                        if (testStr1 != newStr){
                            QRadiateDialog *newDlg=new QRadiateDialog(this);
                            newDlg->m_LineValue->setText(m_RadiateDlg[str]->m_LineValue->text());
                            m_RadiateDlg.insert(newStr,newDlg);
                        }
                        //m_FilmDlg.insert(str,m_FilmDlg[str]);
                        m_RadiateDlg.insert(str,m_RadiateDlg["constant--"+testStr1]);
                        m_RadiateDlg[str]->m_LineValue->setText(testStr1);
                        m_ComboxRadiate[id]->setItemText(1,newStr);
                    }
                }else{//new dlg
                    QRadiateDialog *NewDlg = new QRadiateDialog(this);
                    NewDlg->SetShowStyle(0);
					if (execDialogInFront(NewDlg)){
						m_RadiateDlg.insert("constant--"+NewDlg->m_LineValue->text(),NewDlg);
						m_ComboxRadiate[id]->setItemText(1,"constant--"+NewDlg->m_LineValue->text());
					}
               }
            }else if (str=="F(t)"){//函数--NEW F(t)
               QRadiateDialog *NewDlg = new QRadiateDialog(this);
                NewDlg->SetShowStyle(1);
                if (execDialogInFront(NewDlg)){
                    str=NewDlg->m_LineName->text();
                    m_RadiateDlg.insert(str,NewDlg);
                    InpCurveInpS nodeCurveList;
                    nodeCurveList=m_RadiateDlg[str]->GetData(1,nodeCurveList);

                    QTemperateDialog *NewTDlg = new QTemperateDialog(this);
                    NewTDlg->SetShowStyle(1);
                    NewTDlg->setData(1, nodeCurveList);
                    m_TemperateDlg.insert(str,NewTDlg);
                    QFilmDialog *NewFDlg = new QFilmDialog(this);
                    NewFDlg->SetShowStyle(1);
                    NewFDlg->setData(1,nodeCurveList);
                    m_FilmDlg.insert(str,NewFDlg);
                    comboxAppendText(tmpBtn,nodeCurveList.strName);
                    QStringList combList;combList.clear();
                    for (int kk=0;kk<tmpBtn->count();kk++){//保存
                        if(tmpBtn->itemText(kk).toUpper()=="NO"||tmpBtn->itemText(kk).toUpper()=="CONSTANT"||tmpBtn->itemText(kk).toUpper()=="F(T)")continue;
                        combList<<tmpBtn->itemText(kk);
                    } 
                    
                    emitHPCurve(combList,nodeCurveList);
                }    
            }else if (str=="no"){//函数
                //FilmBtn->setEnabled(true);
            }else{
                if(m_RadiateDlg.find(str)!=m_RadiateDlg.end()){
                    m_RadiateDlg[str]->SetShowStyle(1);
                    if (execDialogInFront(m_RadiateDlg[str])){
                        QString strLineName=m_RadiateDlg[str]->m_LineName->text();
                        m_RadiateDlg.insert(strLineName,m_RadiateDlg[str]);
                        InpCurveInpS nodeCurveList;
                        nodeCurveList=m_RadiateDlg[strLineName]->GetData(1,nodeCurveList);
                        comboxChangeText(str,strLineName);
                        QStringList combList;combList.clear();
                        for (int kk=0;kk<tmpBtn->count();kk++){//保存
                            if(tmpBtn->itemText(kk).toUpper()=="NO"||tmpBtn->itemText(kk).toUpper()=="CONSTANT"||tmpBtn->itemText(kk).toUpper()=="F(T)")continue;
                            combList<<tmpBtn->itemText(kk);
                        }
                        emitHPCurve(combList,nodeCurveList);
                        m_TemperateDlg[str]->setData(1,nodeCurveList);
                        m_TemperateDlg.insert(strLineName,m_TemperateDlg[str]);
                        m_FilmDlg[str]->setData(1,nodeCurveList);
                        m_FilmDlg.insert(strLineName,m_FilmDlg[str]);   
                    }
                }
            }
          }break;
        case COL_SYS_SYSTEM:{
            int id =m_ComboxSystem.key(tmpBtn);
            if (id==-1)return;
            int ind=m_ComboxSystem[id]->currentIndex();
            QString str= m_ComboxSystem[id]->currentText();
            if (str =="新建"){//new
                QString strSysName="step"+QString("%1").arg(m_ComboxSystem[id]->count()-1);
                m_ComboxSystem[id]->insertItem(ind,strSysName);
                m_ComboxSystem[id]->setCurrentIndex(ind);
                m_ComboxSystem.insert(id,m_ComboxSystem[id]);
                //--更新整个SYSTEM combox；
                int nRow=m_TableWiget->rowCount();
                for (int tt=0; tt<nRow; tt++){
                    if (tt==iRow)continue;
                     MyCombox *ff = (MyCombox*)m_TableWiget->cellWidget(tt, COL_SYS_SYSTEM);
                     int idComb=m_ComboxSystem.key(ff);
                     QString curStr=ff->currentText();
                     int curId=ff->currentIndex();
                     ff->insertItem(ind,strSysName);
                     ff->setCurrentIndex(curId);
                     m_ComboxSystem.insert(idComb,ff);
                }
            }else if(str =="删除"){//delete
                int nsize=m_ComboxSystem[id]->count();
                if (nsize>0)m_ListWidget->clear();
                QStringList strListCur;
                for (int tt=0;tt<nsize;tt++){
                    m_ComboxSystem[id]->setCurrentIndex(tt);
                    QString strT=m_ComboxSystem[id]->currentText();
                   
                    if(strT=="新建" || strT=="删除")continue;
                    strListCur<<strT;
                }
                if (!strListCur.empty()){
                    m_ListWidget->show();
                    m_ListWidget->addItems(strListCur); 
                }
            }
          }break;
        default:break;
    }   
}
//--给combox后追加
void QHPSystemDlg::comboxAppendText(MyCombox* nowBtn, QString strName)
{
    int nSize = m_TableWiget->rowCount();
    int nColSize = m_TableWiget->columnCount();
    for (int i=0; i<nSize; i++){
        for (int j=COL_SYS_TEMPER; j<=COL_SYS_RADIATE; j++){
            MyCombox *tmpBtn = (MyCombox*)m_TableWiget->cellWidget(i, j);
            tmpBtn->insertItem(tmpBtn->count(),strName);
            if (tmpBtn == nowBtn){
                tmpBtn->setCurrentIndex(tmpBtn->count()-1);
            }else{;}   
            static_cast<QStandardItemModel*>(tmpBtn->view()->model())->item(tmpBtn->count()-1,0)->setTextAlignment(Qt::AlignRight);
        }
    }  
}
//--给combox中Text更改
void QHPSystemDlg::comboxChangeText(QString strOldName, QString strNewName)
{
    int nSize = m_TableWiget->rowCount();
    int nColSize = m_TableWiget->columnCount();
    for (int i=0; i<nSize; i++){
        for (int j=COL_SYS_TEMPER; j<=COL_SYS_RADIATE; j++){
            MyCombox *tmpBtn = (MyCombox*)m_TableWiget->cellWidget(i, j);
            int id=tmpBtn->findText(strOldName);
            tmpBtn->setItemText(id,strNewName);
        }
    }  
}
void QHPSystemDlg::DelRBtnSlot()
{
    QString str;
     str = m_ListWidget->currentItem()->text();
     QMessageBox::StandardButton bt= QMessageBox::question(this, tr("删除"), tr("确定删除？"), QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);

     if(bt == QMessageBox::No) return;
     delete m_ListWidget->currentItem();

     QStringList strListCur;
     for (int tt=0;tt<m_ListWidget->count();tt++){
         strListCur<<m_ListWidget->item(tt)->text() ;
     }
     strListCur<<tr("新建")<<tr("删除");
     
     //--更新整个SYSTEM combox；
     int nRow=m_TableWiget->rowCount();
     for (int tt=0; tt<nRow; tt++){
         MyCombox *ff = (MyCombox*)m_TableWiget->cellWidget(tt, COL_SYS_SYSTEM);
         int idComb=m_ComboxSystem.key(ff);
         QString curStr=ff->currentText();
         ff->clear();
         ff->addItems(strListCur);
         int iExist=strListCur.indexOf(curStr);
         if (iExist!=-1){
             ff->setCurrentIndex(iExist);
         }
         m_ComboxSystem.insert(idComb,ff);
     }
}

void QHPSystemDlg::insertToCurveList(InpCurveInpS CurveData)
{
   int nSize = m_nodeCurveList.count();
   if (nSize<0)return;
   if (nSize==0){
        m_nodeCurveList.append(CurveData);
   }else{
       for (int i=0;i<nSize;i++){
           if(m_nodeCurveList.at(i).strName==CurveData.strName){
               m_nodeCurveList.insert(i,CurveData);
               break;
           } 
       }
        m_nodeCurveList.append(CurveData);
   }
}
//--来自HIP中数据同步更新HP
//*1.QTemperateDialog,QFilmDialog,QRadiateDialog-- Map 中增加(str,data);
//*2.combox中同步data;
void QHPSystemDlg::updateHPCurveDataSlot(QStringList combList,InpCurveInpS CurveData)
{
    int nRow=m_TableWiget->rowCount();

    for (int kk=0;kk<nRow;kk++){
        //--环境
        MyCombox *tmpBtn = (MyCombox*)m_TableWiget->cellWidget(kk, COL_SYS_TEMPER);
        int id =m_ComboxTemper.key(tmpBtn);
        if (id==-1)return;     
        int ind=m_ComboxTemper[id]->currentIndex();
        QString str = m_ComboxTemper[id]->currentText();//当前显示的str；
        m_ComboxTemper[id]->clear();//清除所有list，
        m_ComboxTemper[id]->addItem("constant");
        m_ComboxTemper[id]->addItem("F(t)");
        m_ComboxTemper[id]->addItems(combList);
        for (int tt=2;tt<m_ComboxTemper[id]->count();tt++){
            static_cast<QStandardItemModel*>(tmpBtn->view()->model())->item(tt,0)->setTextAlignment(Qt::AlignRight);
        }
        int iCurId=m_ComboxTemper[id]->findText(str);
        if (iCurId==-1){
            m_ComboxTemper[id]->setCurrentIndex(ind);
        }else{
            m_ComboxTemper[id]->setCurrentIndex(iCurId);
        }
        str=CurveData.strName;
        
        //if (str==CurveData.strName)//"F(t)"){//函数
            if(m_TemperateDlg.find(str)!=m_TemperateDlg.end()){
                m_TemperateDlg[str]->SetShowStyle(1);
                m_TemperateDlg[str]->setData(1,CurveData);
                m_TemperateDlg.insert(str,m_TemperateDlg[str]); 
            }else{//new dlg  
               QTemperateDialog *TmpDlg=new QTemperateDialog(this);
               TmpDlg->SetShowStyle(1);
               TmpDlg->setData(1, CurveData);
               m_TemperateDlg.insert(str, TmpDlg); 
            } 
        //}    
        //--对流
        tmpBtn = (MyCombox*)m_TableWiget->cellWidget(kk, COL_SYS_FILM);
        id =m_ComboxFilm.key(tmpBtn);
        if (id==-1)return;
        ind=m_ComboxFilm[id]->currentIndex();
        str= m_ComboxFilm[id]->currentText();

        m_ComboxFilm[id]->clear();//清除所有list，
        m_ComboxFilm[id]->addItem("no");
        m_ComboxFilm[id]->addItem("constant");
        m_ComboxFilm[id]->addItem("F(t)");
        m_ComboxFilm[id]->addItems(combList);
        for (int tt=3;tt<m_ComboxFilm[id]->count();tt++){
            static_cast<QStandardItemModel*>(tmpBtn->view()->model())->item(tt,0)->setTextAlignment(Qt::AlignRight);
        }
        iCurId=m_ComboxFilm[id]->findText(str);
        if (iCurId==-1){
            m_ComboxFilm[id]->setCurrentIndex(ind);
        }else{
            m_ComboxFilm[id]->setCurrentIndex(iCurId);
        }
       

        str=CurveData.strName;

        //if (str==CurveData.strName)//"F(t)"){//函数
            if(m_FilmDlg.find(str)!=m_FilmDlg.end()){
                m_FilmDlg[str]->SetShowStyle(1);
                m_FilmDlg[str]->setData(1,CurveData);
                m_FilmDlg.insert(str,m_FilmDlg[str]); 
            }else{//new dlg 
                QFilmDialog *TmpDlg=new QFilmDialog(this);
                TmpDlg->SetShowStyle(1);
                TmpDlg->setData(1, CurveData);
                m_FilmDlg.insert(str, TmpDlg); 
            }  
        //}    
        //--辐射
        tmpBtn = (MyCombox*)m_TableWiget->cellWidget(kk, COL_SYS_RADIATE);
        id =m_ComboxRadiate.key(tmpBtn);
        if (id==-1)return;
        ind=m_ComboxRadiate[id]->currentIndex();
        str= m_ComboxRadiate[id]->currentText();

        m_ComboxRadiate[id]->clear();//清除所有list，
        m_ComboxRadiate[id]->addItem("no");
        m_ComboxRadiate[id]->addItem("constant");
        m_ComboxRadiate[id]->addItem("F(t)");
        m_ComboxRadiate[id]->addItems(combList);
        for (int tt=3;tt<m_ComboxRadiate[id]->count();tt++){
            static_cast<QStandardItemModel*>(tmpBtn->view()->model())->item(tt,0)->setTextAlignment(Qt::AlignRight);
        }
        iCurId=m_ComboxRadiate[id]->findText(str);
        if (iCurId==-1){
            m_ComboxRadiate[id]->setCurrentIndex(ind);
        }else{
            m_ComboxRadiate[id]->setCurrentIndex(iCurId);
        }
      

        str=CurveData.strName;

        //if (str=="F(t)"){//函数
        str=CurveData.strName;
            if(m_RadiateDlg.find(str)!=m_RadiateDlg.end()){
                m_RadiateDlg[str]->SetShowStyle(1);
                m_RadiateDlg[str]->setData(id,CurveData);
                m_RadiateDlg.insert(str,m_RadiateDlg[str]); 
            }else{//new dlg 
                QRadiateDialog *TmpDlg=new QRadiateDialog(this);
                TmpDlg->SetShowStyle(1);
                TmpDlg->setData(1,CurveData);
                m_RadiateDlg.insert(str, TmpDlg);
            }  
        //}    
    }
}
//--热处理制度数据温度、对流、辐射、制度

//--------------------------------------
//----------------  温度 ---------------
//--------------------------------------
QTemperateDialog::QTemperateDialog(QWidget *parent)
{
    setWindowTitle("Environment Temperature");
    QHBoxLayout *Hlayout1 = new QHBoxLayout();
    m_lab = new QLabel(tr("常量:"));
    m_LineValue = new QLineEdit(); 
    Hlayout1->addWidget(m_lab);
    Hlayout1->addWidget(m_LineValue);
    Hlayout1->addStretch();

    QGridLayout *Hlayout2 = new QGridLayout();
    m_NameLab=new QLabel(tr("名称:"));
    m_LineName=new QLineEdit();
 
    m_TCurveDataPlot=new QTCurveDataPlot(this);
    Hlayout2->addWidget(m_NameLab,0,0,1,1);
    Hlayout2->addWidget(m_LineName,0,1,1,1);
    Hlayout2->addWidget(m_TCurveDataPlot,1,0,1,2);
    

    QHBoxLayout *HlayoutLast = new QHBoxLayout();
    m_OKBtn = new QPushButton(tr("确定"));
    m_CancelBtn = new QPushButton(tr("取消"));
    HlayoutLast->addStretch();
    HlayoutLast->addWidget(m_OKBtn);
    HlayoutLast->addWidget(m_CancelBtn);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(Hlayout1);
    mainLayout->addLayout(Hlayout2);
    mainLayout->addLayout(HlayoutLast);
    this->setLayout(mainLayout);

    connect(m_OKBtn,SIGNAL(clicked()),this,SLOT(OkBtnSlot()));
    connect(m_CancelBtn,SIGNAL(clicked()),this,SLOT(reject()));
}

QTemperateDialog::~QTemperateDialog()
{

}

void QTemperateDialog::OkBtnSlot()
{
    if (m_LineName->text()==""&& m_LineName->isVisible()){
        QMessageBox::warning(this,tr(""),tr("Please input a name"),QMessageBox::Ok,QMessageBox::Ok);       
    }else{
        this->accept();
    }
}
/*
    常量- F(t)时间函数
*/
void QTemperateDialog::SetShowStyle(int iStyle)
{
    if (iStyle==0){//常量
        m_NameLab->hide();
        m_LineName->hide();
        m_TCurveDataPlot->hide();
        m_lab->show();
        m_LineValue->show();
    }else{//F(t)函数
        QStringList strList;
        strList<<tr("时间")<<tr("环境");
        m_TCurveDataPlot->ChangeLable(strList);
        m_TCurveDataPlot->show();
        m_NameLab->show();
        m_LineName->show();
        m_lab->hide();
        m_LineValue->hide();
    }
}
//--常量设置
void QTemperateDialog::setData(int style,QString strDataVal)
{
    if (style!=0)return;
    m_LineValue->setText(strDataVal);
    
}
//--曲线设置
void QTemperateDialog::setData(int style,InpCurveInpS data)
{
    if (style!=1) return;
    m_LineName->setText(data.strName);
    int nRowSize = data.strDataX.size();
    QStringList strList;
    for (int kk=0;kk<data.strDataX.size();kk++){  
        strList<<data.strDataX.at(kk)+","+data.strDataY.at(kk);
    }
   
    m_TCurveDataPlot->SetData(nRowSize,strList);
    strList.clear();
    strList<<tr("时间")<<tr("温度");
    m_TCurveDataPlot->ChangeLable(strList);
}
QString QTemperateDialog::GetData(int style)
{
    if (style==0){
       return m_LineValue->text();
    }
    return "";
}
InpCurveInpS QTemperateDialog::GetData(int style,InpCurveInpS data)
{
   if (style == 1){
        int nRow=m_TCurveDataPlot->m_TabCurveWiget->rowCount();
        int nCol=m_TCurveDataPlot->m_TabCurveWiget->columnCount();
        QStringList strList;
        data.strCurveStyle="Amplitude";
        //data.ThirdStrTime=0;//不包含时间写入
        data.strName=m_LineName->text();
        for (int i=0;i<nRow;i++)
        {
            QString strTmpX,strTmpY;
            strTmpX = m_TCurveDataPlot->m_TabCurveWiget->item(i,0)->text();
            strTmpY = m_TCurveDataPlot->m_TabCurveWiget->item(i,1)->text();
            if (strTmpX==""&&strTmpY=="")continue;//均为"",则无数据 
            data.strDataX<<strTmpX; 
            data.strDataY<<strTmpY; 
        }  
    }
    return data;
}

//--------------------------------------
//----------------  对流 ---------------
//--------------------------------------
QFilmDialog::QFilmDialog(QWidget *parent)
{
    setWindowTitle("Convection");
    QHBoxLayout *Hlayout1 = new QHBoxLayout();
    m_lab = new QLabel(tr("常量:"));
    m_LineValue = new QLineEdit();
    Hlayout1->addWidget(m_lab);
    Hlayout1->addWidget(m_LineValue);
    Hlayout1->addStretch();

    QGridLayout *Hlayout2 = new QGridLayout();
    m_NameLab=new QLabel(tr("名称:"));
    m_LineName=new QLineEdit();
    m_TCurveDataPlot=new QTCurveDataPlot(this);
    Hlayout2->addWidget(m_NameLab,0,0,1,1);
    Hlayout2->addWidget(m_LineName,0,1,1,1);
    Hlayout2->addWidget(m_TCurveDataPlot,1,0,1,2);
    

    QHBoxLayout *HlayoutLast = new QHBoxLayout();
    m_OKBtn = new QPushButton(tr("确定"));
    m_CancelBtn = new QPushButton(tr("取消"));
    HlayoutLast->addStretch();
    HlayoutLast->addWidget(m_OKBtn);
    HlayoutLast->addWidget(m_CancelBtn);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(Hlayout1);
    mainLayout->addLayout(Hlayout2);
    mainLayout->addLayout(HlayoutLast);
    this->setLayout(mainLayout);

    connect(m_OKBtn,SIGNAL(clicked()),this,SLOT(OkBtnSlot()));
    connect(m_CancelBtn,SIGNAL(clicked()),this,SLOT(reject()));
}

QFilmDialog::~QFilmDialog()
{

}

void QFilmDialog::OkBtnSlot()
{
    if (m_LineName->text()==""&& m_LineName->isVisible()){
        QMessageBox::warning(this,tr(""),tr("Please input a name"),QMessageBox::Ok,QMessageBox::Ok);       
    }else{
        this->accept();
    }
}
/*
    常量- F(t)时间函数
*/
void QFilmDialog::SetShowStyle(int iStyle)
{
    if (iStyle==0){//常量
        m_NameLab->hide();
        m_LineName->hide();
        m_TCurveDataPlot->hide();
        m_lab->show();
        m_LineValue->show();
    }else{//F(t)函数
        QStringList strList;
        strList<<tr("时间")<<tr("对流");
        m_TCurveDataPlot->ChangeLable(strList);
        m_TCurveDataPlot->show();
        m_NameLab->show();
        m_LineName->show();
        m_lab->hide();
        m_LineValue->hide();
    }
}
//--常量设置
void QFilmDialog::setData(int style,QString strDataVal)
{
    if (style!=0)return;
    m_LineValue->setText(strDataVal);

}
//--曲线设置
void QFilmDialog::setData(int style,InpCurveInpS data)
{
    if (style!=1) return;
    m_LineName->setText(data.strName);
    int nRowSize = data.strDataX.size();
    QStringList strList;
    for (int kk=0;kk<data.strDataX.size();kk++){  
        strList<<data.strDataX.at(kk)+","+data.strDataY.at(kk);
    }

    m_TCurveDataPlot->SetData(nRowSize,strList);
    strList.clear();
    strList<<tr("时间")<<tr("温度");
    m_TCurveDataPlot->ChangeLable(strList);
}

QString QFilmDialog::GetData(int style)
{
    if (style==0){
        return m_LineValue->text();
    }
    return "";
}
InpCurveInpS QFilmDialog::GetData(int style,InpCurveInpS data)
{
    if (style == 1){
        int nRow=m_TCurveDataPlot->m_TabCurveWiget->rowCount();
        int nCol=m_TCurveDataPlot->m_TabCurveWiget->columnCount();
        QStringList strList;
        data.strCurveStyle="Amplitude";
        //data.ThirdStrTime=0;//不包含时间写入
        data.strName=m_LineName->text();
        for (int i=0;i<nRow;i++)
        {
            QString strTmpX,strTmpY;
            strTmpX = m_TCurveDataPlot->m_TabCurveWiget->item(i,0)->text();
            strTmpY = m_TCurveDataPlot->m_TabCurveWiget->item(i,1)->text();
            if (strTmpX==""&&strTmpY=="")continue;//均为"",则无数据 
            data.strDataX<<strTmpX; 
            data.strDataY<<strTmpY; 
        }

    }
    return data;
}
//--------------------------------------
//----------------  辐射 ---------------
//--------------------------------------
QRadiateDialog::QRadiateDialog(QWidget *parent)
{
    setWindowTitle("Radiation");
    QHBoxLayout *Hlayout1 = new QHBoxLayout();
    m_lab = new QLabel(tr("常量:"));
    m_LineValue = new QLineEdit();
    Hlayout1->addWidget(m_lab);
    Hlayout1->addWidget(m_LineValue);
    Hlayout1->addStretch();

    QGridLayout *Hlayout2 = new QGridLayout();
    m_NameLab=new QLabel(tr("名称:"));
    m_LineName=new QLineEdit();
    m_TCurveDataPlot=new QTCurveDataPlot(this);
    Hlayout2->addWidget(m_NameLab,0,0,1,1);
    Hlayout2->addWidget(m_LineName,0,1,1,1);
    Hlayout2->addWidget(m_TCurveDataPlot,1,0,1,2);
    

    QHBoxLayout *HlayoutLast = new QHBoxLayout();
    m_OKBtn = new QPushButton(tr("确定"));
    m_CancelBtn = new QPushButton(tr("取消"));
    HlayoutLast->addStretch();
    HlayoutLast->addWidget(m_OKBtn);
    HlayoutLast->addWidget(m_CancelBtn);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(Hlayout1);
    mainLayout->addLayout(Hlayout2);
    mainLayout->addLayout(HlayoutLast);
    this->setLayout(mainLayout);

    connect(m_OKBtn,SIGNAL(clicked()),this,SLOT(OkBtnSlot()));
    connect(m_CancelBtn,SIGNAL(clicked()),this,SLOT(reject()));
}

QRadiateDialog::~QRadiateDialog()
{

}

void QRadiateDialog::OkBtnSlot()
{
    if (m_LineName->text()==""&& m_LineName->isVisible()){
        QMessageBox::warning(this,tr(""),tr("Please input a name"),QMessageBox::Ok,QMessageBox::Ok);       
    }else{
        this->accept();
    }
}
/*
    常量- F(t)时间函数
*/
void QRadiateDialog::SetShowStyle(int iStyle)
{
    if (iStyle==0){//常量
        m_NameLab->hide();
        m_LineName->hide();
        m_TCurveDataPlot->hide();
        m_lab->show();
        m_LineValue->show();
    }else{//F(t)函数
        QStringList strList;
        strList<<tr("时间")<<tr("辐射");
        m_TCurveDataPlot->ChangeLable(strList);
        m_TCurveDataPlot->show();
        m_NameLab->show();
        m_LineName->show();
        m_lab->hide();
        m_LineValue->hide();
    }
}
//--常量设置
void QRadiateDialog::setData(int style,QString strDataVal)
{
    if (style!=0)return;
    m_LineValue->setText(strDataVal);

}
//--曲线设置
void QRadiateDialog::setData(int style,InpCurveInpS data)
{
    if (style!=1) return;
    m_LineName->setText(data.strName);
    int nRowSize = data.strDataX.size();
    QStringList strList;
    for (int kk=0;kk<data.strDataX.size();kk++){  
        strList<<data.strDataX.at(kk)+","+data.strDataY.at(kk);
    }

    m_TCurveDataPlot->SetData(nRowSize,strList);
    strList.clear();
    strList<<tr("时间")<<tr("温度");
    m_TCurveDataPlot->ChangeLable(strList);
}
QString QRadiateDialog::GetData(int style)
{
    if (style==0){
        return m_LineValue->text();
    }
    return "";
}
InpCurveInpS QRadiateDialog::GetData(int style,InpCurveInpS data)
{
    if (style == 1){
        int nRow=m_TCurveDataPlot->m_TabCurveWiget->rowCount();
        int nCol=m_TCurveDataPlot->m_TabCurveWiget->columnCount();
        QStringList strList;
        data.strCurveStyle="Amplitude";
        //data.ThirdStrTime=0;//不包含时间写入
        data.strName=m_LineName->text();
        for (int i=0;i<nRow;i++)
        {
            QString strTmpX,strTmpY;
            strTmpX = m_TCurveDataPlot->m_TabCurveWiget->item(i,0)->text();
            strTmpY = m_TCurveDataPlot->m_TabCurveWiget->item(i,1)->text();
            if (strTmpX==""&&strTmpY=="")continue;//均为"",则无数据 
            data.strDataX<<strTmpX; 
            data.strDataY<<strTmpY; 
        }

    }
    return data;
}
