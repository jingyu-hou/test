#include "QMaterNewDlg.h"
#include <QMessageBox>

QMaterNewDlg::QMaterNewDlg(QWidget *parent)
: QDialog(parent)
{
    creatGUI();
}

QMaterNewDlg::~QMaterNewDlg()
{
	if (m_TTTDlg!=NULL)
	{
		delete m_TTTDlg;
		m_TTTDlg=NULL;
	}
}
void QMaterNewDlg::creatGUI()
{
    m_OkBtn = new QPushButton(tr("确定"));
    m_CancelBtn = new QPushButton(tr("取消"));

    m_LName = new QLabel(tr("名字:"),this);
    m_NameEdit = new QLineEdit(this);

    QGroupBox *gbGroup = new QGroupBox(tr("材料行为"), this);
    gbGroup->setStyleSheet(QString::fromUtf8("::title{color:blue}"));
	m_TTTDlg=NULL;
	if (!m_TTTDlg)m_TTTDlg= new QHPTTTDlg(this);

    m_BtnDelList = new QPushButton(tr("删除"),gbGroup);
    m_BarMenu  = new QMenuBar(gbGroup);
    m_DensityAct =new QAction(tr("密度"),gbGroup);
    m_ElasticAct = new QAction(tr("弹性"),gbGroup);
    m_PlasticAct = new QAction(tr("塑性"),gbGroup);
	m_RateDependent=new QAction(tr("应变率相关"),gbGroup);
	m_Damage= new QAction(tr("损伤"),gbGroup);
	m_DynamicRecrystallization=new QAction(tr("动态再结晶"),gbGroup);

    m_ExpansionAct = new QAction(tr("热膨胀"),gbGroup);
    m_ConductivityAct = new QAction(tr("导热"),gbGroup);
    m_SpecificHeatAct = new QAction(tr("比热"),gbGroup);
	m_TTTAct = new QAction(tr("TTT"),gbGroup);
    m_MatelPowderAct=new QAction(tr("金属粉末"),gbGroup);

    m_GeneralMenu = m_BarMenu->addMenu(tr("一般"));
    m_GeneralMenu->addAction(m_DensityAct);

    m_MechanicalMenu = m_BarMenu->addMenu(tr("力学"));
    m_subMeEMenu=m_MechanicalMenu->addMenu(tr("弹性"));
    m_subMePMenu=m_MechanicalMenu->addMenu(tr("塑性"));
	m_MatelPowderMenu=m_MechanicalMenu->addMenu(tr("金属"));//金属粉末
	m_MatelPowderMenu->addAction(m_MatelPowderAct);

	//m_subMePMenu->addAction(m_MatelPowderAct);
    m_subMeEMenu->addAction(m_ElasticAct);
    m_subMePMenu->addAction(m_PlasticAct);
	m_subMePMenu->addAction(m_RateDependent);
	m_subMePMenu->addAction(m_Damage);
	m_subMePMenu->addAction(m_DynamicRecrystallization);
	
    m_MechanicalMenu->addAction(m_ExpansionAct);
    m_TermalMenu = m_BarMenu->addMenu(tr("热"));
    m_TermalMenu->addAction(m_ConductivityAct);
    m_TermalMenu->addAction(m_SpecificHeatAct);
	//m_TermalMenu->addAction(m_TTTAct);

    m_ListWiget = new QListWidget(gbGroup);

    QGridLayout *gbGrid=new QGridLayout(gbGroup);
    gbGrid->addWidget(m_BarMenu,0,0,1,1);
    gbGrid->addWidget(m_BtnDelList,0,1,1,1);
    gbGrid->addWidget(m_ListWiget,1,0,1,2);

    //---
    m_gbCurGroup = new QGroupBox(tr("DataShow"), this);
    m_strList<<"密度"<<"弹性"<<"塑性"<<"热膨胀"<<"导热"<<"比热"<<"金属粉末"<<
		"应变率相关"<<"损伤"<<"动态再结晶"<<"TTT";//0~9
    InitTabCurveGUI();

    //---
    QHBoxLayout *HLayOut0=new QHBoxLayout();
    HLayOut0->addWidget(m_LName);
    HLayOut0->addWidget(m_NameEdit);
    HLayOut0->addStretch();
    QHBoxLayout *HLayOut1 = new QHBoxLayout();
    HLayOut1->addWidget(gbGroup);
    QHBoxLayout *HLayOut2 = new QHBoxLayout();
    HLayOut2->addWidget(m_gbCurGroup);
    QHBoxLayout *HLayOutBtn = new QHBoxLayout();
    HLayOutBtn->addStretch();
    HLayOutBtn->addWidget(m_OkBtn);
    HLayOutBtn->addWidget(m_CancelBtn);
    QVBoxLayout* MainLayOut = new QVBoxLayout();
    MainLayOut->addLayout(HLayOut0);
    MainLayOut->addLayout(HLayOut1);
    MainLayOut->addLayout(HLayOut2);
    MainLayOut->addLayout(HLayOutBtn);
    setLayout(MainLayOut);
   

    connect(m_DensityAct,SIGNAL(triggered()),this,SLOT(DensitySlot()));
    connect(m_ElasticAct,SIGNAL(triggered()),this,SLOT(ElasticSlot()));
    connect(m_PlasticAct,SIGNAL(triggered()),this,SLOT(PlasticSlot()));

	connect(m_RateDependent,SIGNAL(triggered()),this,SLOT(RateDependentSlot()));
	connect(m_Damage,SIGNAL(triggered()),this,SLOT(DamageSlot()));
	connect(m_DynamicRecrystallization,SIGNAL(triggered()),this,SLOT(DynamicRecrystallizationSlot()));


    connect(m_ExpansionAct,SIGNAL(triggered()),this,SLOT(ExpansionSlot()));
    connect(m_ConductivityAct,SIGNAL(triggered()),this,SLOT(ConductSlot()));
    connect(m_SpecificHeatAct,SIGNAL(triggered()),this,SLOT(SpecificSlot()));

    connect(m_TTTAct,SIGNAL(triggered()),this,SLOT(TTTSlot()));

    connect(m_MatelPowderAct,SIGNAL(triggered()),this,SLOT(MatelPowderSlot()));

    connect(m_ListWiget,SIGNAL(currentRowChanged(int)),this,SLOT(MaterListSlot(int)));
    connect(m_BtnDelList,SIGNAL(clicked()),this,SLOT(DelMaterialSlot()));
    connect(m_OkBtn,SIGNAL(clicked()),this,SLOT(OkBtnSlot()));
    connect(m_CancelBtn,SIGNAL(clicked()),this,SLOT(CancelBtnSlot()));
    
    m_gbCurGroup->hide();
    //m_ListWiget->setMaximumWidth(300);
    gbGroup->setMaximumWidth(500);
    m_gbCurGroup->setMaximumWidth(500);
    
}
void QMaterNewDlg::ListShowSet(int index)
{
    int i=0;
    m_gbCurGroup->setTitle(m_strList[index]);
    if (m_ListWiget->count()<=0){
        m_ListWiget->addItem(m_strList[index]);
	}else{
		for (i=0;i<m_ListWiget->count();i++){
            if (m_strList[index]==m_ListWiget->item(i)->text()){
                m_ListWiget->setCurrentRow(i);//该行被选中
                break;
            }
		}
        if (i == m_ListWiget->count()){        
            m_ListWiget->addItem(m_strList[index]);	
            m_ListWiget->setCurrentRow(i);//该行被选中
        }
    }
}
//--创建数据及曲线显示窗口
void QMaterNewDlg::InitTabCurveGUI()
{
    QHBoxLayout *gbLayout = new QHBoxLayout(m_gbCurGroup);
    for (int i=0;i<m_strList.size();i++){
         QTableCurveDataPlot *tmpWiget=new QTableCurveDataPlot(m_gbCurGroup,i);
         m_TabCurDataWidget.push_back(tmpWiget);
         gbLayout->addWidget(tmpWiget);
    }
}
/*
删除当前选中的材料
*/
void QMaterNewDlg::DelMaterialSlot()
{
    int indexCNT = m_ListWiget->count();
    int indexRow = m_ListWiget->currentRow();

    if (indexRow == 0 && indexCNT==1){	
        m_gbCurGroup->setTitle("");

    }
    QListWidgetItem *item = m_ListWiget->takeItem(indexRow);
    delete item;
}
/*
    List点击相应
*/
void QMaterNewDlg::MaterListSlot(int Row)
{
    if (Row <0) {//无可显示内容
        for (int i=0;i<m_TabCurDataWidget.size();i++){
            m_TabCurDataWidget.at(i)->setVisible(false);
        } 
        return;
    }

    QString str=m_ListWiget->item(Row)->text();
    m_gbCurGroup->setTitle(str);
    int id =m_strList.indexOf(str);

    if (id!=-1){
        setWidgetVisible(id);
    }
    
}
//--菜单项对应
void QMaterNewDlg::TTTSlot()
{   
	m_TTTDlg->show(); 
	m_TTTDlg->raise();//最上层
	m_TTTDlg->activateWindow();//激活
}
//--菜单项对应
void QMaterNewDlg::DensitySlot()
{
    setWidgetVisible(M_ENUM_DENSITY);
}
//--菜单项对应
void QMaterNewDlg::ElasticSlot()
{
    setWidgetVisible(M_ENUM_ELASTIC);
}
//--菜单项对应
void QMaterNewDlg::PlasticSlot()
{
    setWidgetVisible(M_ENUM_PLASTIC);
}
//--菜单项对应
void QMaterNewDlg::RateDependentSlot()
{
    setWidgetVisible(M_ENUM_Rate_Dependent_Plastic);
}
//--菜单项对应
void QMaterNewDlg::DamageSlot()
{
    setWidgetVisible(M_ENUM_Damage);
}
//--菜单项对应
void QMaterNewDlg::DynamicRecrystallizationSlot()
{
    setWidgetVisible(M_ENUM_Dynamic_Recrystallization);
}
//--菜单项对应
void QMaterNewDlg::ExpansionSlot()
{
    setWidgetVisible(M_ENUM_EXPANS);
}
//--菜单项对应
void QMaterNewDlg::ConductSlot()
{
	setWidgetVisible(M_ENUM_CONDUCT);
}
//--菜单项对应
void QMaterNewDlg::SpecificSlot()
{
    setWidgetVisible(M_ENUM_SPECIFICE);
}
//--菜单项对应
void QMaterNewDlg::MatelPowderSlot()
{
    setWidgetVisible(M_ENUM_METALPOWDER);
}

void QMaterNewDlg::setWidgetVisible(int type)
{
	ListShowSet(type);
	m_gbCurGroup->show();
    for (int i=0;i<m_TabCurDataWidget.size();i++){
        m_TabCurDataWidget.at(i)->setVisible(false);
        if (type==i){
            m_TabCurDataWidget.at(i)->setVisible(true);
        }
    }
}
void QMaterNewDlg::CancelBtnSlot()
{
    this->reject();
}
//--保存退出
void QMaterNewDlg::OkBtnSlot()
{
    if(TabDataToSave()){
        this->accept();
    } 
}
//从树形节点上读取当前的Data显示到表格上;
void QMaterNewDlg::setNewData()
{
    QString str=m_NameEdit->text();
    //QMap<QString,QMap<int,MCompoment>>::Iterator iitt= MFileData.begin();
    //if(MFileData.find(str)==MFileData.end()){return;}//不存在退出
    QMap<int,MCompoment> tmpData;
    tmpData = MFileData[str];//Mdata;
    QMap<int,MCompoment>::Iterator iitt= tmpData.begin();
    QString str1;
    for (iitt;iitt!=tmpData.end();iitt++)
    {
       int index = iitt.key();
       ListShowSet(index);
       m_TabCurDataWidget.at(index)->SetTableCurveData(iitt.value());
    }
    //--写入List
}
//--读取界面上的数据进行保存到内存中;
bool QMaterNewDlg::TabDataToSave()
{

    QString strEditName=m_NameEdit->text();

    if (strEditName ==""){
        QMessageBox::warning(this,tr(""),tr("请输入名字"),QMessageBox::Yes,QMessageBox::Yes);
        return false;
    }
    
    int nCnt=m_ListWiget->count();//style 有几种
    //QMap<int,MCompoment> TmpMData;
    Mdata.clear();//qy20201126
    for (int i=0;i<nCnt;i++)
    {
       MCompoment tmpMCompoment;
       QString strStyle=m_ListWiget->item(i)->text();//QString Style;"Density"
       int Id = m_strList.indexOf(strStyle);
       if (Id<0)return false;
	   if(strStyle=="密度")strStyle="Density";
	   else if(strStyle=="弹性")strStyle="Elastic";
	   else if(strStyle=="塑性")strStyle="Plastic";
	   else if(strStyle=="比热")strStyle="SpecificHeat";
	   else if(strStyle=="导热")strStyle="Conductivity";
	   else if(strStyle=="金属粉末")strStyle="MetalPowder";
	   else if(strStyle=="动态再结晶")strStyle="DynamicRecrystallization";

	  
       QTableCurveDataPlot *tmpData= m_TabCurDataWidget.at(Id);
       if (tmpData->m_TabWidget->count() ==1){
           int nRow=tmpData->m_TableData->rowCount();
           int nCol=tmpData->m_TableData->columnCount();
           vector<MCell> tmpVCell;
           for (int kk=0;kk<nRow;kk++){
               MCell tmpCell;
               for(int jj=0;jj<nCol;jj++){
                   tmpCell.strName=tmpData->m_TableData->horizontalHeaderItem(jj)->text();
                   if (tmpData->m_TableData->item(kk,jj)!=NULL){
                       tmpCell.Data=tmpData->m_TableData->item(kk,jj)->text();
                   }
                   tmpVCell.push_back(tmpCell);
               }
           }
           tmpMCompoment.id=Id;
           tmpMCompoment.FileName=strEditName;
           if (strStyle=="热膨胀")
           {
			   strStyle="Expansion";
               tmpMCompoment.strStyleParam=tr("zero=")+tmpData->m_editZero->text();
		   }else if(strStyle=="应变率相关"){
			   strStyle="RateDependent";
			   tmpMCompoment.strStyleParam=tmpData->N_Rate_Dependent->currentText();
			   if(tmpData->PlasticWorkCheck->checkState())tmpMCompoment.strStyleParam1=tmpData->PlasticWorkValue->text();

		   }else if(strStyle=="损伤"){
               strStyle="Damage";
			   tmpMCompoment.strStyleParam=tmpData->N_Damage->currentText();
           }
		   tmpMCompoment.strStyle = strStyle;
           tmpMCompoment.dataColNum=nCol;
           tmpMCompoment.dataRowNum=nRow;
           tmpMCompoment.PageIndex=Id;
           tmpMCompoment.data=tmpVCell;
           Mdata.insert(Id,tmpMCompoment);
       }else{//有两个Tab金属粉末特性
            vector<MCell> tmpVCell;
           //--参数写入
           int nParaRow=tmpData->m_TableData1->rowCount();
           int nParaCol=tmpData->m_TableData1->columnCount();

           for (int kk=0;kk<nParaRow;kk++){
               MCell tmpCell;
               for(int jj=0;jj<nParaCol;jj++){
                   tmpCell.strName=tmpData->m_TableData1->horizontalHeaderItem(jj)->text();
                   if (tmpData->m_TableData1->item(kk,jj)!=NULL){
                       tmpCell.Data=tmpData->m_TableData1->item(kk,jj)->text();
                   }else{
                       tmpCell.Data="";
                   }
                   tmpVCell.push_back(tmpCell);
               }
           }
           //-数据写入
           int nRow=tmpData->m_TableData->rowCount();
           int nCol=tmpData->m_TableData->columnCount();
           for (int kk=0;kk<nRow;kk++){
               MCell tmpCell;
               for(int jj=0;jj<nCol;jj++){
                   tmpCell.strName=tmpData->m_TableData->horizontalHeaderItem(jj)->text();
                   if (tmpData->m_TableData->item(kk,jj)!=NULL){
                       tmpCell.Data=tmpData->m_TableData->item(kk,jj)->text();
                   }
                   tmpVCell.push_back(tmpCell);
               }
           }
          
           tmpMCompoment.id=Id;
           tmpMCompoment.FileName=strEditName;
           tmpMCompoment.strStyle = strStyle;
           tmpMCompoment.dataColNum=nParaCol;
           tmpMCompoment.dataRowNum=nParaRow;
           tmpMCompoment.PageIndex=Id;
           tmpMCompoment.data=tmpVCell;
           tmpMCompoment.dataColNum2=nCol;
           tmpMCompoment.dataRowNum2=nRow;
           Mdata.insert(Id,tmpMCompoment);
       }//if(tmpData->m_TabWidget->count() ==1)
    }// for (int i=0;i<nCnt;i++)
  
    MFileData.insert(strEditName,Mdata);
  
    
    return true;
}

void QMaterNewDlg::UpDataMaterList(QMap<int, MCompoment> Mdata)
{
    QMap<int, MCompoment>::Iterator it = Mdata.begin();
    QString str;
    int PageIndex,PageListRow=0;
    MCompoment tmpCmp;
    int rowCount;
    m_ListWiget->clear();
    for (it; it != Mdata.end(); ++it)
    {
        rowCount=Mdata.size();
        tmpCmp=it.value();
        str=tmpCmp.strStyle;
        PageIndex = tmpCmp.id;
        ListShowSet(PageIndex);
    }
}
void QMaterNewDlg::LanguageUpData()
{
    m_OkBtn->setText("OK");
    m_CancelBtn->setText("Cancel");
    m_LName->setText("Name:");
}
/*=========================================================
----------------------数据添加与曲线显示-------------------
*///=======================================================
QTableCurveDataPlot::QTableCurveDataPlot(QWidget *parent,int type)
:QWidget(parent)
{
    QWidget *m_Widge = new QWidget(this);
    m_AddBtn = new QPushButton(tr("增加"),m_Widge);
    m_DelBtn = new QPushButton(tr("删除"),m_Widge);

    m_InsertBtn = new QPushButton(tr("插入"),m_Widge);
    m_PlotBtn = new QPushButton(tr("绘图"),m_Widge);

    m_TabWidget = new QTabWidget(m_Widge);
    //QTableWidget *tmpTableData = new QTableWidget(m_Widge);
   

    if (type==M_ENUM_METALPOWDER){
        m_TableData1 = new QTableWidget(m_Widge);
        m_TabWidget->addTab(m_TableData1,"粉末特性");//参数页面设置
        m_TableData= new QTableWidget(m_Widge);
        m_TabWidget->addTab(m_TableData,tr("粉末硬化"));//数据页
        m_TabWidget->setCurrentIndex(0);
    }else{
        m_TableData= new QTableWidget(m_Widge);
        QString str=parent->windowTitle();
        m_TabWidget->addTab(m_TableData,str);//数据页
    }
    m_chartViewers = new QChartViewer(m_Widge);
    mailayout =new QGridLayout(this);
    mailayout->addWidget(m_AddBtn,0,0,1,1);
    mailayout->addWidget(m_DelBtn,0,1,1,1);
    mailayout->addWidget(m_InsertBtn,0,2,1,1);
   
    mailayout->addWidget(m_PlotBtn,0,4,1,1);
    if (type == M_ENUM_EXPANS){//热膨胀
        m_LabRfTemperate=new QLabel(tr("参考温度:"),m_Widge);
        m_editZero=new QLineEdit(tr("20"),m_Widge);
        mailayout->addWidget(m_LabRfTemperate,1,0,1,1);
        mailayout->addWidget(m_editZero,1,1,1,1);
        m_LabRfTemperate->setAlignment(Qt::AlignRight|Qt::AlignCenter);
        m_DelBtn->setMaximumWidth(80);
        m_editZero->setMaximumWidth(80);
	}else if(type == M_ENUM_Rate_Dependent_Plastic){
		m_Rate_Dependent=new QLabel(tr("应变率相关模型:"),m_Widge);
		N_Rate_Dependent=new QComboBox(m_Widge);
		N_Rate_Dependent->addItem("HansenSpittel");
		N_Rate_Dependent->addItem("Arrhenius");
		QLabel *pImageLabel = new QLabel(this);
		QString image=":/images/Hansenspittel.png";
		QPixmap pixmap(image);
		pImageLabel->setPixmap(pixmap);
		pImageLabel->setScaledContents(true);
		pImageLabel->show();

		PlasticWorkCheck = new QCheckBox(tr("塑性功比例"),m_Widge);
		PlasticWorkValue=new QLineEdit(m_Widge);
		PlasticWorkValue->setText("0.9");
		PlasticWorkValue->setEnabled(false);
		connect(PlasticWorkCheck,SIGNAL(clicked()),this,SLOT(PlasticWorkValueSlot()));

		mailayout->addWidget(m_Rate_Dependent,1,0,1,1);
		mailayout->addWidget(N_Rate_Dependent,1,1,1,1);
		mailayout->addWidget(pImageLabel,2,0,2,3);

		mailayout->addWidget(PlasticWorkCheck,4,0,1,1);
		mailayout->addWidget(PlasticWorkValue,4,1,1,1);

		m_Rate_Dependent->setAlignment(Qt::AlignRight|Qt::AlignCenter);
		m_DelBtn->setMaximumWidth(150);	
		N_Rate_Dependent->setMaximumWidth(150);
		connect(N_Rate_Dependent,SIGNAL(activated(int)),this,SLOT(RateDependentSlot01(int)));
	}else if(type == M_ENUM_Damage){
		m_Damage=new QLabel(tr("损伤模型:"),m_Widge);
		N_Damage=new QComboBox(m_Widge);
		N_Damage->addItem("NormalizedCL");
		N_Damage->addItem("CockroftLatham");
		N_Damage->addItem("McClintock");
		N_Damage->addItem("Freudenthal");
		N_Damage->addItem("RiceTracy");
		N_Damage->addItem("Oyane");
		N_Damage->addItem("Oyanenegative");
		N_Damage->addItem("Ayada");
		N_Damage->addItem("Ayadanegative");
		N_Damage->addItem("Osakada");
		N_Damage->addItem("Brozzo");
		N_Damage->addItem("ZhaoKuhn");
		N_Damage->addItem("MPSUTS");
		QLabel *pImageLabel = new QLabel(this);
		QPixmap pixmap(":/images/NormalizedCL.png");
		pImageLabel->setPixmap(pixmap);
		pImageLabel->setScaledContents(true);
		mailayout->addWidget(m_Damage,1,0,1,1);
		mailayout->addWidget(N_Damage,1,1,1,1);
		mailayout->addWidget(pImageLabel,2,0,2,2);
		m_Damage->setAlignment(Qt::AlignRight|Qt::AlignCenter);
		m_DelBtn->setMaximumWidth(170);	
		N_Damage->setMaximumWidth(170);
		connect(N_Damage,SIGNAL(activated(int)),this,SLOT(DamageSlot01(int)));
	}else if(type ==M_ENUM_Dynamic_Recrystallization){
		QLabel *pImageLabel = new QLabel(this);
		QPixmap pixmap(":/images/dynamicrecy.png");
		pImageLabel->setPixmap(pixmap);
		mailayout->addWidget(pImageLabel,2,0,3,3);
    }
    mailayout->addWidget(m_TabWidget,5,0,3,5);
   // mailayout->addWidget(m_chartViewers,0,5,3,5);
    connect(m_AddBtn,SIGNAL(clicked()),this,SLOT(AddLineBtnSlot()));
    connect(m_DelBtn,SIGNAL(clicked()),this,SLOT(DelLineBtnSlot()));
    connect(m_InsertBtn,SIGNAL(clicked()),this,SLOT(InsertLineBtnSlot()));
    connect(m_PlotBtn,SIGNAL(clicked()),this,SLOT(PlotBtnSlot()));
    
//--初始化表头参数等信息
    QStringList strListR,strListC;
    m_TableData->verticalHeaderItem(1);
    m_TableData->setRowCount(1);
   // strListR.append("1");
   // m_TableData->setVerticalHeaderLabels(strListR);
    switch (type){
    case M_ENUM_DENSITY:{	
        strListC.append("密度");
        strListC.append("温度(℃)");
        m_TableData->horizontalHeaderItem(2);
        m_TableData->setColumnCount(2);
        m_TableData->setHorizontalHeaderLabels(strListC);
      }break;
    case M_ENUM_EXPANS:{//热膨胀系数		
        strListC.append("热膨胀");
        strListC.append("温度(℃)");
        m_TableData->horizontalHeaderItem(2);
        m_TableData->setColumnCount(2);
        m_TableData->setHorizontalHeaderLabels(strListC);
      }break;
    case M_ENUM_ELASTIC:{				
        strListC<<"杨氏模量"<<"泊松比";
        strListC.append("温度(℃)");
        m_TableData->horizontalHeaderItem(3);
        m_TableData->setColumnCount(3);
      
        m_TableData->setHorizontalHeaderLabels(strListC);
      }break;
    case M_ENUM_PLASTIC:{
        strListC<<"屈服应力(Pa)"<<"塑性应变";
        strListC.append("温度(℃)");
        m_TableData->horizontalHeaderItem(3);
        m_TableData->setColumnCount(3);
       
        m_TableData->setHorizontalHeaderLabels(strListC);
      }break;
    case M_ENUM_CONDUCT:{
        strListC.append("导热");
        strListC.append("温度(℃)");
        m_TableData->horizontalHeaderItem(2);
        m_TableData->setColumnCount(2);
       
        m_TableData->setHorizontalHeaderLabels(strListC);
      }break;
    case M_ENUM_SPECIFICE:{ 
        strListC.append("比热");
        strListC.append("温度(℃)");
        m_TableData->horizontalHeaderItem(2);
        m_TableData->setColumnCount(2);
      
        m_TableData->setHorizontalHeaderLabels(strListC);
      }break;
    case M_ENUM_METALPOWDER:{
        strListC<<"pb/σy"<<"相对密度";
        m_TableData->horizontalHeaderItem(2);
        m_TableData->setColumnCount(2);
        m_TableData->setHorizontalHeaderLabels(strListC);

        strListR.clear();strListC.clear();
        m_TableData1->verticalHeaderItem(1);
        m_TableData1->setRowCount(5);
        //strListR<<"β角度"<<"R形状控制参数"<<"ρ0初始密度"<<"d0/σy"<<"σ_threshold";
        strListR<<"1"<<"2"<<"3"<<"4"<<"5";
        m_TableData1->setVerticalHeaderLabels(strListR);
        strListC<<"参数"<<"值";
        m_TableData1->horizontalHeaderItem(2);
        m_TableData1->setColumnCount(2);
        m_TableData1->setHorizontalHeaderLabels(strListC);
        QStringList strListPara;
        strListPara<<"β角度"<<"R形状控制参数"<<"d0/σy"<<"σ_threshold"<<"ρ0初始密度";
        for (int kk=0; kk<strListPara.size();kk++){
            m_TableData1->setItem(kk,0,new QTableWidgetItem(strListPara[kk]));
            m_TableData1->item(kk,0)->setFlags(m_TableData1->item(kk,0)->flags()&~Qt::ItemIsEnabled);
            QTableWidgetItem *items = new QTableWidgetItem();
            m_TableData1->setItem(kk,1,items); 
        }
		}break;
	case M_ENUM_Rate_Dependent_Plastic:{

      }break;
	case M_ENUM_Damage:{
         //strListC.clear();
		//strListC.append("参数");
		//strListC.append("值");
		//QStringList strListPara;
		//strListPara<<"损伤阈值"<<"a"<<"b";
		//m_TableData->horizontalHeaderItem(2);
		//m_TableData->setColumnCount(2);
		//m_TableData->setHorizontalHeaderLabels(strListC);
		//m_TableData->setRowCount(3);
		//for(int kk=0; kk<strListPara.size();kk++){
		//	//m_TableData->insertRow(kk);
		//	m_TableData->setItem(kk,0,new QTableWidgetItem(strListPara[kk]));
		//	m_TableData->item(kk,0)->setFlags(m_TableData->item(kk,0)->flags()&~Qt::ItemIsEnabled);
		//	QTableWidgetItem *items = new QTableWidgetItem();
		//	m_TableData->setItem(kk,1,items);
		//}
		}break;
	case M_ENUM_Dynamic_Recrystallization:{
		strListC.clear();
		strListC.append("参数");
		strListC.append("值");
		QStringList strListPara;
		strListPara<<"a1"<<"n1"<<"m1"<<"q1"<<"c1"<<"d0"<<"a5"<<"h5"<<"n5"<<"m5"<<"q5"<<"c5"<<"a10"
			<<"βd"<<"kd"<<"a8"<<"h8"<<"n8"<<"m8"<<"q8"<<"c8";
		m_TableData->horizontalHeaderItem(2);
        m_TableData->setColumnCount(2);
        m_TableData->setHorizontalHeaderLabels(strListC);
		m_TableData->setRowCount(21);
		for(int kk=0; kk<strListPara.size();kk++){
			m_TableData->setItem(kk,0,new QTableWidgetItem(strListPara[kk]));
			m_TableData->item(kk,0)->setFlags(m_TableData->item(kk,0)->flags()&~Qt::ItemIsEnabled);
			QTableWidgetItem *items = new QTableWidgetItem();
			m_TableData->setItem(kk,1,items);
		}
		}break;
    default:break;
    } 
    for (int i=0; i<m_TableData->rowCount(); i++){
		if(type==M_ENUM_Dynamic_Recrystallization)break;
        for (int j=0;j<m_TableData->columnCount();j++) {
            QTableWidgetItem *items = new QTableWidgetItem();
            m_TableData->setItem(i,j,items);   
        }   
    }
    //复制粘贴
     m_popMenu=new QMenu;
     m_TableData->setContextMenuPolicy(Qt::CustomContextMenu);
    
     m_popMenu->addAction(tr("复制"),this,SLOT(TableCopyRBtnSlot()));//(ExpandAllSlot()));
     m_popMenu->addAction(tr("粘贴"),this,SLOT(TablePasteRBtnSlot()));//(CollapseAllSlot()));
     m_popMenu->addAction(tr("清除"),this,SLOT(TableClearRBtnSlot()));
     connect(m_TableData, SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(contextTableMenuSlot(const QPoint &)));
     if (type==M_ENUM_METALPOWDER) {
          m_TableData1->setContextMenuPolicy(Qt::CustomContextMenu);
          connect(m_TableData1, SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(contextTableMenuSlot(const QPoint &)));
     }
     m_PlotBtn->hide();
    //Plot();
}
QTableCurveDataPlot::~QTableCurveDataPlot()
{

}

void QTableCurveDataPlot::PlasticWorkValueSlot()
{
	QCheckBox *btn=(QCheckBox*)sender();
	bool ActivateOrNot=btn->checkState();
	if(ActivateOrNot){
		PlasticWorkValue->setEnabled(true);
	}else{
		PlasticWorkValue->setEnabled(false);
	}
}

void QTableCurveDataPlot::DamageSlot01(int id)
{
	QLabel *pImageLabel = new QLabel(this);
	QString image="";
	if(id==0)image=":/images/NormalizedCL.png";
	else if(id==1)image=":/images/CockroftLatham.png";
	else if(id==2)image=":/images/McClintock.png";
	else if(id==3)image=":/images/Freudenthal.png";
	else if(id==4)image=":/images/RiceTracy.png";
	else if(id==5)image=":/images/Oyane.png";
	else if(id==6)image=":/images/Oyane.png";
	else if(id==7)image=":/images/Ayada.png";
	else if(id==8)image=":/images/Ayada.png";
	else if(id==9)image=":/images/Osakada.png";
	else if(id==10)image=":/images/Brozzo.png";
	else if(id==11)image=":/images/ZhaoKuhn.png";
	else if(id==12)image=":/images/MPSUTS.png";
	QPixmap pixmap(image);
	pImageLabel->setPixmap(pixmap);
	pImageLabel->setScaledContents(true);
	pImageLabel->show();
	mailayout->addWidget(pImageLabel,2,0,2,2);

	QStringList strListR,strListC;
	strListC.append("参数");
	strListC.append("值");
	int nun=m_TableData->rowCount();
	if (nun<0)return;
	if(nun>=0){
		for(int i=nun;i>=0;i--){
			m_TableData->removeRow(i);
		}
	}

	QStringList strListPara;
	strListPara<<"损伤阈值"<<"a"<<"b";
	m_TableData->horizontalHeaderItem(2);
	m_TableData->setColumnCount(2);
	m_TableData->setHorizontalHeaderLabels(strListC);
	for(int kk=0; kk<strListPara.size();kk++){
		m_TableData->insertRow(kk);
		m_TableData->setItem(kk,0,new QTableWidgetItem(strListPara[kk]));
		m_TableData->item(kk,0)->setFlags(m_TableData->item(kk,0)->flags()&~Qt::ItemIsEnabled);
		QTableWidgetItem *items = new QTableWidgetItem();
		m_TableData->setItem(kk,1,items);
		if(id==0||id==1||id==3||id==7||id==8||id==10||id==11){
			break;
		}else if(id==2&&kk==1||id==4&&kk==1||id==5&&kk==1||id==6&&kk==1||id==12&&kk==1){
			break;
		}
	}
}
void QTableCurveDataPlot::RateDependentSlot01(int id)
{
	QLabel *pImageLabel = new QLabel(this);
	QString image="";
	

    QStringList strListR,strListC;
	strListC.append("参数");
	strListC.append("值");

	int nun=m_TableData->rowCount();
	if (nun<0)return;
	if(nun>=0){
		for(int i=nun;i>=0;i--){
			m_TableData->removeRow(i);
		}
	}
	m_TableData->setColumnCount(2);
	m_TableData->horizontalHeaderItem(2);
	m_TableData->setHorizontalHeaderLabels(strListC);
	strListC.clear();strListR.clear();
	strListC<<"A"<<"α"<<"β"<<"n"<<"m";
	strListR<<"A"<<"α"<<"n"<<"ΔH";
	if(id==0){
		image=":/images/Hansenspittel.png";
		for (int kk=0; kk<strListC.size();kk++){
			m_TableData->insertRow(kk);
			m_TableData->setItem(kk,0,new QTableWidgetItem(strListC[kk]));
			m_TableData->item(kk,0)->setFlags(m_TableData->item(kk,0)->flags()&~Qt::ItemIsEnabled);
			QTableWidgetItem *items = new QTableWidgetItem();
			m_TableData->setItem(kk,1,items);
		}
	}else if(id==1){
		image=":/images/Arrhenius.png";
		for (int kk=0; kk<strListR.size();kk++){
			m_TableData->insertRow(kk);
			m_TableData->setItem(kk,0,new QTableWidgetItem(strListR[kk]));
			m_TableData->item(kk,0)->setFlags(m_TableData->item(kk,0)->flags()&~Qt::ItemIsEnabled);
			QTableWidgetItem *items = new QTableWidgetItem();
			m_TableData->setItem(kk,1,items);
		}
	}
	QPixmap pixmap(image);
	pImageLabel->setPixmap(pixmap);
	pImageLabel->setScaledContents(true);
	pImageLabel->show();
	mailayout->addWidget(pImageLabel,2,0,2,3);
}
void QTableCurveDataPlot::LanguageUpData()
{
    m_AddBtn->setText(tr("Add"));
    m_DelBtn->setText(tr("Del"));
    m_InsertBtn->setText(tr("Insert"));
    m_PlotBtn->setText(tr("Plot"));
    m_LabRfTemperate->setText(tr("Reference Temperature"));
}
//--
void QTableCurveDataPlot::PlotBtnSlot()
{
    Plot();
}
//--tab增加行
void QTableCurveDataPlot::AddLineBtnSlot()
{
    int nRow = m_TableData->rowCount();
    if (nRow >= 0){
        m_TableData->insertRow(nRow);
        for(int j=0;j<m_TableData->columnCount();j++){
            m_TableData->setItem(nRow,j,new QTableWidgetItem());
        }
    }
    Plot();
}
//--tab插入行(当前行的上一行增加)
void QTableCurveDataPlot::InsertLineBtnSlot()
{
    int curRow = m_TableData->currentRow();

    m_TableData->insertRow(curRow);
}
//--tab删除行/列
void QTableCurveDataPlot::DelLineBtnSlot()
{
    int curRow = m_TableData->currentRow();
    int curCol = m_TableData->currentColumn();
    if (curCol<0 ||curCol<0)return;
    
    if (curCol==0){//进行的是行选
        m_TableData->removeRow(curRow);
    }else if(curRow==0 &&curCol!=0){//进行的是列选
        m_TableData->removeColumn(curCol);
    }
}
//--将数据写入到界面中
void QTableCurveDataPlot::SetTableCurveData(MCompoment data)
{
    //int nParaCol = data.dataColNum2; //存在第二页面
    //int nParaRow = data.dataRowNum2;
    //if (nParaCol!=0&&nParaRow!=0){//金属粉末 参数读取
    //    m_TableData1->verticalHeaderItem(nParaRow);
    //    m_TableData1->setRowCount(nParaRow);
    //    m_TableData1->setColumnCount(nParaCol);
    //    m_TableData1->horizontalHeaderItem(nParaCol);//
    //    QStringList strListRowHead;
    //    for (int j=0;j<nParaCol;j++){
    //        strListRowHead << data.data.at(j).strName;
    //    }
    //    m_TableData1->setHorizontalHeaderLabels(strListRowHead);
    //    for (int i=0;i<nParaRow;i++){
    //        for(int j=0;j<nParaCol;j++){
    //            m_TableData1->setItem(i,j,new QTableWidgetItem(data.data.at(2*i+j).Data));
    //        }
    //    }
    //}

    //int iBaseRow=nParaCol*nParaRow;
    //int nCol = data.dataColNum;
    //int nRow = data.dataRowNum;
    //m_TableData->verticalHeaderItem(nRow);
    //m_TableData->setRowCount(nRow);
    //m_TableData->setColumnCount(nCol);
    //m_TableData->horizontalHeaderItem(nCol);//
    //QStringList strListRowHead;
    //for (int j=0;j<nCol;j++){
    //    strListRowHead << data.data.at(iBaseRow+j).strName;
    //}
    //m_TableData->setHorizontalHeaderLabels(strListRowHead);
    //for (int i=0;i<nRow;i++){
    //    for(int j=0;j<nCol;j++){
    //        m_TableData->setItem(i,j,new QTableWidgetItem(data.data.at(iBaseRow+i*nCol+j).Data));
    //    }
    //}
   
    int nParaCol = data.dataColNum2; //存在第二页面
    int nParaRow = data.dataRowNum2;
    if (nParaCol!=0&&nParaRow!=0){//金属粉末
        int nCol = data.dataColNum;
        int nRow = data.dataRowNum;
        int iBaseRow=nCol*nRow;

        m_TableData1->verticalHeaderItem(nRow);
        m_TableData1->setRowCount(nRow);
        m_TableData1->setColumnCount(nCol);
        m_TableData1->horizontalHeaderItem(nCol);//
        QStringList strListRowHead;
        for (int j=0;j<nCol;j++){
            strListRowHead << data.data.at(j).strName;
        }
        m_TableData1->setHorizontalHeaderLabels(strListRowHead);
        for (int i=0;i<nRow;i++){
            for(int j=0;j<nCol;j++){
                m_TableData1->setItem(i,j,new QTableWidgetItem(data.data.at(2*i+j).Data));
            }
        }

        m_TableData->verticalHeaderItem(nParaRow);
        m_TableData->setRowCount(nParaRow);
        m_TableData->setColumnCount(nParaCol);
        m_TableData->horizontalHeaderItem(nParaCol);//
        QStringList strListRowHead1;
        for (int j=0;j<nParaCol;j++){
            strListRowHead1 << data.data.at(iBaseRow+j).strName;
        }
        m_TableData->setHorizontalHeaderLabels(strListRowHead1);
        for (int i=0;i<nParaRow;i++){
            for(int j=0;j<nParaCol;j++){
                m_TableData->setItem(i,j,new QTableWidgetItem(data.data.at(iBaseRow+i*nParaCol+j).Data));
            }
        }
    }else{
		if(data.strStyleParam=="NormalizedCL"||data.strStyleParam=="CockroftLatham"||data.strStyleParam=="McClintock"||
			data.strStyleParam=="Freudenthal"||data.strStyleParam=="RiceTracy"||data.strStyleParam=="Oyane"||
			data.strStyleParam=="Oyanenegative"||data.strStyleParam=="Ayada"||data.strStyleParam=="Ayadanegative"||
			data.strStyleParam=="Osakada"||data.strStyleParam=="Brozzo"||data.strStyleParam=="ZhaKuhn"||
			data.strStyleParam=="MPSUTS"){
				int id=N_Damage->findText(data.strStyleParam);
				N_Damage->setCurrentIndex(id);
		}else if(data.strStyleParam=="Arrhenius"||data.strStyleParam=="HansenSpittel"){
			int id=N_Rate_Dependent->findText(data.strStyleParam);
			N_Rate_Dependent->setCurrentIndex(id);
			if(data.strStyleParam1!=""){
				PlasticWorkValue->setText(data.strStyleParam1);
				PlasticWorkCheck->setChecked(true);
				PlasticWorkValue->setEnabled(true);
			}
		}else if(data.strStyle.toLower()=="expansion"&&data.strStyleParam!=""){
			QString strL=data.strStyleParam.toLower();
			m_editZero->setText(strL.remove("zero=", Qt::CaseInsensitive));
		}
        int nCol = data.dataColNum;
        int nRow = data.dataRowNum;
        m_TableData->verticalHeaderItem(nRow);
        m_TableData->setRowCount(nRow);
        m_TableData->setColumnCount(nCol);
        m_TableData->horizontalHeaderItem(nCol);//
        QStringList strListRowHead;
        for (int j=0;j<nCol;j++){
            strListRowHead << data.data.at(j).strName;
        }
        m_TableData->setHorizontalHeaderLabels(strListRowHead);
        for (int i=0;i<nRow;i++){
            for(int j=0;j<nCol;j++){
                m_TableData->setItem(i,j,new QTableWidgetItem(data.data.at(i*nCol+j).Data));
            }
        }
    
    }
}

//-绘图
void QTableCurveDataPlot::Plot()
{
   /* int nRow=m_TableData->rowCount();
    int nCol=m_TableData->columnCount();
    int num=nRow;
    QString *dataTmpX = new QString[num];
    double *dataTmpY = new double[num];   
    for (int i=0; i<nRow; i++){
        dataTmpX[i] = m_TableData->item(i,0)->text();
        dataTmpY[i] = m_TableData->item(i,1)->text().toDouble();
    }
    const char **tmp = new const char*[num];
    QByteArray *b = new QByteArray[num]; 
    const char *imageMap = 0;
    for (int tt=0; tt < num; tt++){   
        b[tt] = dataTmpX[tt].toAscii().data();
        tmp[tt] = b[tt]; 
    }
    StringArray dataX = StringArray(tmp, num);
    vector<DoubleArray> dataY ;
    dataY.push_back(DoubleArray(dataTmpY, num));
    ChartShow2XY(dataX,dataY,imageMap);
    dataTmpX = NULL;dataTmpY = NULL;b=NULL;tmp=NULL;
    delete(dataTmpY);delete(dataTmpX);delete(tmp);delete(b);*/
}
void QTableCurveDataPlot::ChartShow2XY(StringArray dataX, vector<DoubleArray> dataY, const char *imageMap)
{
    // Each demo module can display a number of charts. We display all of them. 
    MaterialChart *d = materialCharts+0;//demoCharts + 2;//demoIndex;
    for (int tt=0;tt<d->noOfCharts;tt++)
    {
        imageMap = 0;
        m_chartViewers->setChart(d->createChart(dataX, dataY, &imageMap));
        m_chartViewers->setImageMap(imageMap);
        delete m_chartViewers->getChart();
    }

    // We hide unused chart viewers.
    //for (int kk = 0; kk < noOfChartViewers; ++kk){
    m_chartViewers->setVisible(d->noOfCharts);
    //}   
    //flowLayoutCharts();
}

void QTableCurveDataPlot::contextTableMenuSlot(const QPoint &p)
{
    m_popMenu->exec(QCursor::pos());
}
void QTableCurveDataPlot::TableCopyRBtnSlot()
{
    if (m_TableData->isVisible()) {
        TableDataCopy();
    }else{
        TableData1Copy();
    }
}
void QTableCurveDataPlot::TablePasteRBtnSlot()
{
    if (m_TableData->isVisible()) {
        TableDataPaste();
    }else{
        TableData1Paste();
    }
}
void QTableCurveDataPlot::TableClearRBtnSlot()//clear all data
{
    //m_TableData->clearContents();
    if (m_TableData->isVisible()) {
        TableDataClear();
    }else{
        TableData1Clear();
    }
}
//--sub copy、clear、paste
void QTableCurveDataPlot::TableDataPaste()
{
    QClipboard *clipBoard= QApplication::clipboard();
    QString strPasterText=clipBoard->text(QClipboard::Clipboard);
    QStringList strRowList=strPasterText.split("\n",QString::SkipEmptyParts);
    int nPasteRow=strRowList.count();//需要粘贴的行数
    int nPasteCol=0;
    for (int i=0;i<nPasteRow;i++)
    {
        QStringList strColumnList=strRowList.at(i).split("\t",QString::SkipEmptyParts);
        nPasteCol=strColumnList.count();
    }


    QList<QTableWidgetSelectionRange> ranges=m_TableData->selectedRanges();
    int nCount=ranges.count();//片区
    if (nCount<=0)return; 
    for (int i=0;i<nCount;i++)
    {
        int topRow=ranges.at(i).topRow();//第几行开始
        int bottomRow=ranges.at(i).bottomRow();//第几行结束
        int leftColumn=ranges.at(i).leftColumn();
        int rightColumn=ranges.at(i).rightColumn();
        int nSizeCol=abs(rightColumn-leftColumn)+1;
        int nSizeRow=abs(bottomRow-topRow)+1;//所选片段的大小

        //if (nSizeCol !=nPasteCol ||nSizeRow!=nPasteRow)
        //{
        //    return ;//
        //}
        if (nSizeRow >= nPasteRow){
            for (int j=topRow,tt=0;j<=bottomRow;j++,tt++){
                if (tt>=nPasteRow)continue;
                QStringList strColumnList=strRowList.at(tt).split("\t",QString::SkipEmptyParts);
                for (int kk=leftColumn,mm=0;kk<=rightColumn;kk++,mm++){
                    if (mm>=nPasteCol)continue;
                    m_TableData->item(j,kk)->setData(Qt::DisplayRole,strColumnList.at(mm).toDouble());    
                }
            }
        }else{//增加table行数，进行数值填充
            int nRow = m_TableData->rowCount();
            int iInSertRow=nPasteRow+topRow-nRow;//需要进行增行数
            for (int i = nRow,tt=0; i<nRow+iInSertRow; i++,tt++){//增加Table行数
                m_TableData->insertRow(i);
                for(int j=0;j<=m_TableData->columnCount();j++){
                    QTableWidgetItem *itemX =new QTableWidgetItem();  
                    QVariant data;
                    itemX->setData(Qt::DisplayRole, data.toDouble());
                    m_TableData->setItem(i,j,itemX);
                }              
            }
            //进行赋值及填充
            for (int j=topRow,tt=0;j<=topRow+nPasteRow;j++,tt++){  
                if (tt>=nPasteRow)continue;
                QStringList strColumnList=strRowList.at(tt).split("\t",QString::SkipEmptyParts);
                for (int kk=leftColumn,mm=0;kk<=rightColumn;kk++,mm++){
                    if (mm>=nPasteCol) continue;
                    m_TableData->item(j,kk)->setData(Qt::DisplayRole,strColumnList.at(mm).toDouble());    
                } 
            }
        }
    }
}
void QTableCurveDataPlot::TableData1Paste()
{   
    QClipboard *clipBoard= QApplication::clipboard();
    QString strPasterText=clipBoard->text(QClipboard::Clipboard);
    QStringList strRowList=strPasterText.split("\n",QString::SkipEmptyParts);
    int nPasteRow=strRowList.count();//需要粘贴的行数
    int nPasteCol=0;
    for (int i=0;i<nPasteRow;i++)
    {
        QStringList strColumnList=strRowList.at(i).split("\t",QString::SkipEmptyParts);
        nPasteCol=strColumnList.count();
    }

    QList<QTableWidgetSelectionRange> ranges=m_TableData1->selectedRanges();
    int nCount=ranges.count();//片区
    if (nCount<=0)return;


    for (int i=0;i<nCount;i++)
    {
        int topRow=ranges.at(i).topRow();//第几行开始
        int bottomRow=ranges.at(i).bottomRow();//第几行结束
        int leftColumn=ranges.at(i).leftColumn();
        int rightColumn=ranges.at(i).rightColumn();
        int nSizeCol=abs(rightColumn-leftColumn)+1;
        int nSizeRow=abs(bottomRow-topRow)+1;

        for (int j=topRow,tt=0;j<=bottomRow;j++,tt++)
        {
            QStringList strColumnList;
            if (tt>=nPasteRow)continue;
            strColumnList=strRowList.at(tt).split("\t",QString::SkipEmptyParts);
            for (int kk=leftColumn,mm=0;kk<=rightColumn;kk++,mm++){
                if (mm>=nPasteCol)continue;
                m_TableData1->item(j,kk)->setText(strColumnList.at(mm));    
            } 
        }

    }
}
void QTableCurveDataPlot::TableDataClear()
{
    QList<QTableWidgetSelectionRange> ranges=m_TableData->selectedRanges();
    int nCount=ranges.count();
    if (nCount<=0)return;


    for (int i=0;i<nCount;i++){
        int topRow=ranges.at(i).topRow();//第几行开始
        int bottomRow=ranges.at(i).bottomRow();//第几行结束
        int leftColumn=ranges.at(i).leftColumn();
        int rightColumn=ranges.at(i).rightColumn();

        for (int j=topRow;j<=bottomRow;j++){
            for (int kk=leftColumn;kk<=rightColumn;kk++){
                m_TableData->item(j,kk)->setText("");
            }    
        }
    }
}
void QTableCurveDataPlot::TableData1Clear()
{
    QList<QTableWidgetSelectionRange> ranges=m_TableData1->selectedRanges();
    int nCount=ranges.count();
    if (nCount<=0)return;


    for (int i=0;i<nCount;i++){
        int topRow=ranges.at(i).topRow();//第几行开始
        int bottomRow=ranges.at(i).bottomRow();//第几行结束
        int leftColumn=ranges.at(i).leftColumn();
        int rightColumn=ranges.at(i).rightColumn();

        for (int j=topRow;j<=bottomRow;j++){
            for (int kk=leftColumn;kk<=rightColumn;kk++){
                m_TableData1->item(j,kk)->setText("");
            }    
        }
    }
}
void QTableCurveDataPlot::TableDataCopy()
{
    QList<QTableWidgetSelectionRange> ranges=m_TableData->selectedRanges();
    int nCount=ranges.count();
    if (nCount<=0)return;

    QString strSelectText;
    for (int i=0;i<nCount;i++)
    {
        int topRow=ranges.at(i).topRow();//第几行开始
        int bottomRow=ranges.at(i).bottomRow();//第几行结束
        int leftColumn=ranges.at(i).leftColumn();
        int rightColumn=ranges.at(i).rightColumn();

        for (int j=topRow;j<=bottomRow;j++)
        {
            for (int kk=leftColumn;kk<=rightColumn;kk++){
                strSelectText+=m_TableData->item(j,kk)->text();
                if (kk<rightColumn){
                    strSelectText+="\t";
                }      
            }
            if (j<bottomRow){ 
                strSelectText+="\n";
            }
        }
    }
    QClipboard *clipboard= QApplication::clipboard();
    clipboard->setText(strSelectText);
}
void QTableCurveDataPlot::TableData1Copy()
{
    QList<QTableWidgetSelectionRange> ranges=m_TableData1->selectedRanges();
    int nCount=ranges.count();
    if (nCount<=0)return;

    QString strSelectText;
    for (int i=0;i<nCount;i++)
    {
        int topRow=ranges.at(i).topRow();//第几行开始
        int bottomRow=ranges.at(i).bottomRow();//第几行结束
        int leftColumn=ranges.at(i).leftColumn();
        int rightColumn=ranges.at(i).rightColumn();

        for (int j=topRow;j<=bottomRow;j++)
        {
            for (int kk=leftColumn;kk<=rightColumn;kk++){
                strSelectText+=m_TableData1->item(j,kk)->text();
                if (kk<rightColumn){
                    strSelectText+="\t";
                }      
            }
            if (j<bottomRow){ 
                strSelectText+="\n";
            }
        }
    }
    QClipboard *clipboard= QApplication::clipboard();
    clipboard->setText(strSelectText);
}

