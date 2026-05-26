#include "QMaterPropDlg.h"

QMaterPropDlg* QMaterPropDlg::instance = 0;

QMaterPropDlg::QMaterPropDlg(QWidget *parent)
	: QDialog(parent)
{
	CreatGUI();
    m_IndexMaterName = 0;
}

QMaterPropDlg::~QMaterPropDlg()
{

}
QMaterPropDlg* QMaterPropDlg::GetInstance()
{
    if (instance == 0){
        instance = new QMaterPropDlg;
    }

    return instance;
}
//--材料界面
void QMaterPropDlg::CreatGUI()
{
	this->setWindowTitle(QString::fromUtf8("编辑材料"));
    Qt::WindowFlags ture= Qt::Dialog;
    ture |= Qt::WindowMaximizeButtonHint;
    ture |= Qt::WindowMinimizeButtonHint;
	ture |= Qt::WindowCloseButtonHint;

    this->setWindowFlags(ture);

	QLabel *LName = new QLabel(tr("名字:"),this);
	m_NameEdit = new QLineEdit(this);

	//setLayout(HLayout1);

	m_BtnApplay = new QPushButton(tr("确定"));
	m_BtnCancel = new QPushButton(tr("取消"));
	m_BtnExport = new QPushButton(tr("导出"));
	m_BtnImport = new QPushButton(tr("导入"));
    m_BtnExport->setFixedWidth(80);
    m_BtnImport->setFixedWidth(80);
	m_BtnApplay->setFixedWidth(80);
	m_BtnCancel->setFixedWidth(80);
	
	QGroupBox *gbGroup = new QGroupBox(tr("材料行为"), this);
	gbGroup->setStyleSheet(QString::fromUtf8("::title{color:blue}"));
	QVBoxLayout *gbVLayout = new QVBoxLayout(gbGroup);
	QGridLayout *gbLayout = new QGridLayout(gbGroup);
	QHBoxLayout *gbHLayout2 = new QHBoxLayout(gbGroup);

	//m_BarMenu  = new QMenuBar(gbGroup);
	//m_DensityAct =new QAction(tr("Density"),gbGroup);
	//m_ElasticAct = new QAction(tr("Elastic"),gbGroup);
	//m_PlasticAct = new QAction(tr("Plastic"),gbGroup);
	//m_ExpansionAct = new QAction(tr("Expansion"),gbGroup);
	//m_ConductivityAct = new QAction(tr("Conductivity"),gbGroup);
	//m_SpecificHeatAct = new QAction(tr("SpecificHeat"),gbGroup);
    //m_MatelPowderAct=new QAction(tr("MatelPowder"),gbGroup);

	//m_GeneralMenu = m_BarMenu->addMenu(tr("General"));
	//m_GeneralMenu->addAction(m_DensityAct);

	//m_MechanicalMenu = m_BarMenu->addMenu(tr("Mechanical"));
	//m_subMeEMenu=m_MechanicalMenu->addMenu(tr("Elasticy"));
	//m_subMePMenu=m_MechanicalMenu->addMenu(tr("Plasticy"));
	//m_subMeEMenu->addAction(m_ElasticAct);
	//m_subMePMenu->addAction(m_PlasticAct);
	//m_MechanicalMenu->addAction(m_ExpansionAct);
	//m_TermalMenu = m_BarMenu->addMenu(tr("Termal"));
	//m_TermalMenu->addAction(m_ConductivityAct);
	//m_TermalMenu->addAction(m_SpecificHeatAct);


    //m_MatelPowderMenu=m_BarMenu->addMenu(tr("Matel"));//金属粉末
    //m_MatelPowderMenu->addAction(m_MatelPowderAct);

	//m_BtnDelList = new QPushButton(QIcon(":/images/save.png"),tr("Del"),gbGroup);//(gbGroup);
	//m_BtnDelList->setMaximumWidth(20);

    m_NewMaterialAct_ = new QAction(tr("新建"),this);
    m_BarMenu = new QMenuBar();
    m_NewMaterialMenu = m_BarMenu->addMenu(tr("文件"));//新建
    m_NewMaterialMenu->addAction(m_NewMaterialAct_);


    tabTreeView_ = new QTabWidget(gbGroup);
    m_TreeModel = new QTree_Model(tabTreeView_,M_TREE_MATER);
    m_TreeView = new QTreeMangerView(m_TreeModel,tabTreeView_,M_TREE_MATER);
    //m_TreeView1 = new QTreeMangerView(m_TreeModel,tabTreeView_,M_TREE_MATER);
    tabTreeView_->addTab(m_TreeView,QIcon(":/images/NewIcon.png"),tr("材料管理1"));
   // tabTreeView_->addTab(m_TreeView1,QIcon(":/images/NewIcon.png"),tr("材料管理2"));
    tabTreeView_->setCurrentIndex(0);
   

    tabTreeView_->setMaximumWidth(300);
    m_TreeView->header()->hide();
    m_TreeView->setAutoScroll(true);
    m_TreeView->setModel(m_TreeModel);
    m_TreeView->expandToDepth(3);

    //m_TreeView1->header()->hide();
    //m_TreeView1->setAutoScroll(true);
    //m_TreeView1->setModel(m_TreeModel);
    //m_TreeView1->expandToDepth(3);

    gbLayout->addWidget(tabTreeView_,1,1,2,1);
	//gbLayout->addWidget(m_BarMenu,1,2,1,2);
	//gbLayout->addWidget(m_BtnDelList,1,4,1,1);

    //---右侧list显示
	m_ListWiget = new QListWidget(gbGroup); 
    m_ListWiget2 = new QTableWidget(gbGroup);
    m_ListWiget2->setColumnCount(5);
    m_ListWiget2->setRowCount(1);
    m_ListWiget2->verticalHeaderItem(1);
    QStringList strList;
    strList.append("力学性能参数");
    strList<<"数据类型"<<"数据"<<"数据单位"<<"温度单位";
    m_ListWiget2->setHorizontalHeaderLabels(strList);
    //m_ListWiget2->setEnabled(false);

	QTableWidgetItem *tmpItem=NULL;
	for (int i=0;i<5;i++){
		tmpItem=new QTableWidgetItem(tr("%1").arg(i));
		m_ListWiget2->setItem(0,i,tmpItem);
	}
	
    connect(m_ListWiget2,SIGNAL(cellClicked(int,int)),this,SLOT(DataTestSlot(int,int)));
    //--
    m_WidTTT = new QHPTTTDlg(gbGroup);
    m_WidCCT = new QHPCCTDlg(gbGroup);
    //--
    tabMaterView_ = new QTabWidget(gbGroup);
    tabMaterView_->addTab(m_ListWiget,QIcon(":/images/NewIcon.png"),tr("热物性1"));//("力学"));
    tabMaterView_->addTab(m_ListWiget2,QIcon(":/images/NewIcon.png"),tr("热物性2"));
    tabMaterView_->addTab(m_WidTTT,QIcon(":/images/NewIcon.png"),tr("TTT"));
    //tabMaterView_->addTab(m_WidCCT,QIcon(":/images/NewIcon.png"),tr("CCT"));
	
  	tabMaterView_->setTabEnabled(3,false);//CCT暂不适用;
    tabMaterView_->setCurrentIndex(0);
    gbLayout->addWidget(tabMaterView_,2,2,1,4);
	gbVLayout->addLayout(gbLayout);
	tabMaterView_->setMinimumHeight(400);
    tabMaterView_->setMinimumWidth(400);

	
	//--Hlayout3:
	m_gbGroup2 = new QGroupBox();//(tr("Material Some"), this);
	m_gbGroup2->setStyleSheet(QString::fromUtf8("::title{color:blue}"));
	//m_gbGroup2->setGeometry(120,300,1000,1000);
	QVBoxLayout *gb2Layout = new QVBoxLayout(m_gbGroup2);
	m_MaterDataWig = new QMaterDataWig(m_gbGroup2);
	m_MaterDataWig->setMaximumHeight(300);
   

	gb2Layout->addWidget(m_MaterDataWig);

	QHBoxLayout *HLayout0 = new QHBoxLayout();
    HLayout0->addWidget(m_BarMenu);
	QHBoxLayout *HLayout1 = new QHBoxLayout();
	HLayout1->addWidget(LName);
	HLayout1->addWidget(m_NameEdit);
	QHBoxLayout *HLayout2 = new QHBoxLayout();
	HLayout2->addWidget(gbGroup);
	QHBoxLayout *HLayout3 = new QHBoxLayout();
	HLayout3->addWidget(m_gbGroup2);
    HLayout3->setSizeConstraint(QLayout::SetFixedSize);
	QHBoxLayout *BtnLayout = new QHBoxLayout();
    BtnLayout->addStretch();
	BtnLayout->addWidget(m_BtnExport);
	BtnLayout->addWidget(m_BtnImport);
	BtnLayout->addWidget(m_BtnApplay);
	BtnLayout->addWidget(m_BtnCancel);
	//setLayout(BtnLayout2); 
	
	QVBoxLayout* MainLayOut= new QVBoxLayout();
    MainLayOut->addLayout(HLayout0);
	MainLayOut->addLayout(HLayout1);
	MainLayOut->addLayout(HLayout2);
	MainLayOut->addLayout(HLayout3);
	MainLayOut->addLayout(BtnLayout);
	setLayout(MainLayOut);

	m_comList.append(tr("密度"));//0
	m_comList.append(tr("弹性"));//1
	m_comList.append(tr("塑性"));//2
	m_comList.append(tr("热膨胀"));//3
	m_comList.append(tr("导热"));//4
	m_comList.append(tr("比热"));//5
    m_comList.append(tr("金属粉末"));//6
	m_comList.append(tr("应变率相关"));//7
	m_comList.append(tr("损伤"));//8
    m_comList.append(tr("动态再结晶"));//9
	m_comList.append(tr("TTT"));//9

	//m_comList.append(tr("Density"));//0
	//m_comList.append(tr("Elastic"));//1
	//m_comList.append(tr("Plastic"));//2
	//m_comList.append(tr("Expansion"));//3
	//m_comList.append(tr("Conductivity"));//4
	//m_comList.append(tr("SpecificHeat"));//5
 //   m_comList.append(tr("MetalPowder"));//6
	//m_comList.append(tr("RateDependent"));//7
	//m_comList.append(tr("Damage"));//8
 //   m_comList.append(tr("DynamicRecrystallization"));//9
	//m_comList.append(tr("TTT"));//9
	
    m_gbGroup2->hide();
 

    connect(m_TreeView,SIGNAL(EmitChangeModeIndexMater(const QModelIndex &)),this,
        SLOT(ChangeModelIndexSlot(const QModelIndex &)));
	connect(m_BtnApplay,SIGNAL(clicked()),this,SLOT(BtnApplaySlot()));
	connect(m_BtnCancel,SIGNAL(clicked()),this,SLOT(BtnCancelSlot()));
	connect(m_BtnExport,SIGNAL(clicked()),this,SLOT(BtnExportDataSlot()));
	connect(m_BtnImport,SIGNAL(clicked()),this,SLOT(BtnImportDataSlot()));
	//connect(m_BtnDelList,SIGNAL(clicked()),this,SLOT(BtnDeletSlot()));
	

    m_TreeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_TreeView, SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(contextMaterMenuSlot(const QPoint &)));
    connect(m_ListWiget,SIGNAL(currentRowChanged(int)),this,SLOT(MaterListSlot(int)));
    connect(m_NewMaterialAct_,SIGNAL(triggered()),this,SLOT(NewMatialDlgSlot()));
    connect(m_MaterDataWig,SIGNAL(emitMaterList(QMap<int, MCompoment>)),this,SLOT(UpDataMaterListSLot(QMap<int, MCompoment>)));
}
//SetData 向界面写入数据
void QMaterPropDlg::SetData(SectionS para)
{
	m_Name = para.MaterPara.Name;
	m_NameEdit->setText(m_Name);
	m_IndexMaterName = para.MaterPara.id;
}
//PutData 从界面读出数据
void QMaterPropDlg::PutData(MaterialS &para)
{
	para.Name = m_Name;
    para.id = m_IndexMaterName;
}
//--SLOT
void QMaterPropDlg::BtnApplaySlot()
{
    SectionS tmpMater;
    tmpMater.MaterPara.id = m_IndexMaterName+1;
    m_Name=m_NameEdit->text();
    tmpMater.MaterPara.Name = m_Name;

    emit emitMaterParam(tmpMater);
    this->accept();
}
void QMaterPropDlg::BtnCancelSlot()
{
	//m_MaterDataWig->ExportData();
    this->close();
}
//导入Inp文件中材料数据部分
void QMaterPropDlg::ImportInpMaterial(QList<MaterialInpS> MaterialInps)
{
    QString strMName; 
    QString strColList;
    for (int i=0;i<MaterialInps.size();i++)
    {
        strMName=MaterialInps.at(i).strMaterialName;
        m_TreeModel->AddMaterialUserName(strMName);
        //strColList = MaterialInps.at(i).strSubMCol;
        QMap <int,MCompoment> Mdata;
        MCell tmpCellData;
        QVector<QStringList*> tmpStrData;
        for (int j=0;j<MaterialInps.at(i).strMaterialStyleName.size();j++)
        { 
            MCompoment tmpData;
            tmpData.FileName = strMName;
            tmpData.strStyle=MaterialInps.at(i).strMaterialStyleName.at(j).toLower();
            QStringList tList=tmpData.strStyle.split(",",QString::SkipEmptyParts);
            tmpData.strStyle=tList.at(0);
            if (tmpData.strStyle!="density" &&tmpData.strStyle!="elastic"&&tmpData.strStyle!="plastic"&&
                tmpData.strStyle!="expansion" &&tmpData.strStyle!="conductivity"&&tmpData.strStyle!="specificheat"&&
                tmpData.strStyle!="metalpowder"&&tmpData.strStyle!="powderhardening"&&tmpData.strStyle!="rate-dependentplastic"&&
				tmpData.strStyle!="damage"&&tmpData.strStyle!="dynamicrecrystallization")
            {
                continue;
            }
            tmpData.strStyleParam="";
			tmpData.strStyleParam1="";
            if (tList.size()>=2){
                tmpData.strStyleParam=tList.at(1);
            }
            tmpData.dataColNum = MaterialInps.at(i).strSubMCol.at(j).toInt();//MaterialInps.at(i).dataColNum;//分成几列
			if(MaterialInps.at(i).strMaterialStyleParam2.size()>0&&tmpData.strStyle.toLower()=="rate-dependentplastic"){
				tmpData.strStyleParam1=MaterialInps.at(i).strMaterialStyleParam2.at(0);
			}
            
            tmpStrData= MaterialInps.at(i).strData[0];
            int nSize1= tmpStrData.at(j)->size();//tmpStrData[j][0]->size();
            int nSize2=tmpData.dataColNum;
            tmpData.dataRowNum = (int)(nSize1/nSize2);

            if (tmpData.strStyle == "density"){
                tmpData.strStyle="Density";
                tmpData.PageIndex=M_ENUM_DENSITY;
                tmpData.id=M_ENUM_DENSITY;
                for (int kk=0; kk<nSize1; kk=kk+tmpData.dataColNum){
                   if (tmpData.dataColNum==1){   
                       tmpCellData.strName="密度";
                       QStringList *tmp= tmpStrData.at(j);
                       tmpCellData.Data=tmp->at(kk); 
                       tmpData.data.push_back(tmpCellData);
                   }else{//==2
                        tmpCellData.strName="密度";
                        QStringList *tmp= tmpStrData.at(j);
                        tmpCellData.Data=tmp->at(kk); 
                        tmpData.data.push_back(tmpCellData);
                        tmpCellData.strName="温度(℃)";
                        tmpCellData.Data=tmp->at(kk+1); 
                        tmpData.data.push_back(tmpCellData);
                    }
                    Mdata.insert(M_ENUM_DENSITY,tmpData);
                }
            }else if (tmpData.strStyle == "elastic"){
                tmpData.strStyle="Elastic";
                tmpData.PageIndex=M_ENUM_ELASTIC;
                tmpData.id=M_ENUM_ELASTIC;
                for (int kk=0; kk<nSize1; kk=kk+tmpData.dataColNum){
                    if (tmpData.dataColNum==1){  
                        tmpCellData.strName="杨氏模量";
                        QStringList *tmp= tmpStrData.at(j);
                        tmpCellData.Data=tmp->at(kk); 
                        tmpData.data.push_back(tmpCellData);
                    }else if(tmpData.dataColNum==2){//==2
                        tmpCellData.strName="杨氏模量";
                        QStringList *tmp= tmpStrData.at(j);
                        tmpCellData.Data=tmp->at(kk); 
                        tmpData.data.push_back(tmpCellData);

                        tmpCellData.strName="泊松比";
                        tmpCellData.Data=tmp->at(kk+1); 
                        tmpData.data.push_back(tmpCellData);
                    }else if(tmpData.dataColNum==3){//==2
                        tmpCellData.strName="杨氏模量";
                        QStringList *tmp= tmpStrData.at(j);
                        tmpCellData.Data=tmp->at(kk); 
                        tmpData.data.push_back(tmpCellData);

                        tmpCellData.strName="泊松比";
                        tmpCellData.Data=tmp->at(kk+1); 
                        tmpData.data.push_back(tmpCellData);

                        tmpCellData.strName="温度(℃)";
                        tmpCellData.Data=tmp->at(kk+2); 
                        tmpData.data.push_back(tmpCellData);
                    }
                    Mdata.insert(M_ENUM_ELASTIC,tmpData);
                }
            }else if (tmpData.strStyle=="plastic"){
                tmpData.strStyle="Plastic";
                tmpData.PageIndex=M_ENUM_PLASTIC;
                tmpData.id=M_ENUM_PLASTIC;
                for (int kk=0; kk<nSize1; kk=kk+tmpData.dataColNum){
                    if (tmpData.dataColNum==1){  
                        tmpCellData.strName="屈服应力(Pa)";
                        QStringList *tmp= tmpStrData.at(j);
                        tmpCellData.Data=tmp->at(kk); 
                        tmpData.data.push_back(tmpCellData);
                    }else if(tmpData.dataColNum==2){//==2
                        tmpCellData.strName="屈服应力(Pa)";
                        QStringList *tmp= tmpStrData.at(j);
                        tmpCellData.Data=tmp->at(kk); 
                        tmpData.data.push_back(tmpCellData);

                        tmpCellData.strName="塑性应变";
                        tmpCellData.Data=tmp->at(kk+1); 
                        tmpData.data.push_back(tmpCellData);
                    }else if(tmpData.dataColNum==3){//==3
                        tmpCellData.strName="屈服应力(Pa)";
                        QStringList *tmp= tmpStrData.at(j);
                        tmpCellData.Data=tmp->at(kk); 
                        tmpData.data.push_back(tmpCellData);

                        tmpCellData.strName="塑性应变";
                        tmpCellData.Data=tmp->at(kk+1); 
                        tmpData.data.push_back(tmpCellData);
                        tmpCellData.strName="温度(℃)";
                        tmpCellData.Data=tmp->at(kk+2); 
                        tmpData.data.push_back(tmpCellData);
                    } 
                    Mdata.insert(M_ENUM_PLASTIC,tmpData);
                }
            }else if (tmpData.strStyle=="expansion"){
                tmpData.strStyle="Expansion";
                tmpData.PageIndex=M_ENUM_EXPANS;
                tmpData.id=M_ENUM_EXPANS;
                for (int kk=0; kk<nSize1; kk=kk+tmpData.dataColNum){
                    if (tmpData.dataColNum==1){  
                        tmpCellData.strName="热膨胀";
                        QStringList *tmp= tmpStrData.at(j);
                        tmpCellData.Data=tmp->at(kk); 
                        tmpData.data.push_back(tmpCellData);
                    }else if(tmpData.dataColNum==2){//==2
                        tmpCellData.strName="热膨胀";
                        QStringList *tmp= tmpStrData.at(j);
                        tmpCellData.Data=tmp->at(kk); 
                        tmpData.data.push_back(tmpCellData);

                        tmpCellData.strName="温度(℃)";
                        tmpCellData.Data=tmp->at(kk+1); 
                        tmpData.data.push_back(tmpCellData);
                    }
                    Mdata.insert(M_ENUM_EXPANS,tmpData);
                }
            }else if (tmpData.strStyle=="conductivity"){
                tmpData.strStyle="Conductivity";
                tmpData.PageIndex=M_ENUM_CONDUCT;
                tmpData.id=M_ENUM_CONDUCT;
                for (int kk=0; kk<nSize1; kk=kk+tmpData.dataColNum){
                    if (tmpData.dataColNum==1){  
                        tmpCellData.strName="导热";
                        QStringList *tmp= tmpStrData.at(j);
                        tmpCellData.Data=tmp->at(kk); 
                        tmpData.data.push_back(tmpCellData);
                    }else if(tmpData.dataColNum==2){//==2
                        tmpCellData.strName="导热";
                        QStringList *tmp= tmpStrData.at(j);
                        tmpCellData.Data=tmp->at(kk); 
                        tmpData.data.push_back(tmpCellData);

                        tmpCellData.strName="温度(℃)";
                        tmpCellData.Data=tmp->at(kk+1); 
                        tmpData.data.push_back(tmpCellData);
                    }
                    Mdata.insert(M_ENUM_CONDUCT,tmpData);
                }
            }else if (tmpData.strStyle=="specificheat"){
                tmpData.strStyle="SpecificHeat";
                tmpData.PageIndex=M_ENUM_SPECIFICE;
                tmpData.id=M_ENUM_SPECIFICE;
                for (int kk=0; kk<nSize1; kk=kk+tmpData.dataColNum){
                    if (tmpData.dataColNum==1){  
                        tmpCellData.strName="比热";
                        QStringList *tmp= tmpStrData.at(j);
                        tmpCellData.Data=tmp->at(kk); 
                        tmpData.data.push_back(tmpCellData);
                    }else if(tmpData.dataColNum==2){//==2
                        tmpCellData.strName="比热";
                        QStringList *tmp= tmpStrData.at(j);
                        tmpCellData.Data=tmp->at(kk); 
                        tmpData.data.push_back(tmpCellData);

                        tmpCellData.strName="温度(℃)";
                        tmpCellData.Data=tmp->at(kk+1); 
                        tmpData.data.push_back(tmpCellData);
                    }
                    Mdata.insert(M_ENUM_SPECIFICE,tmpData);
                }
            }else if (tmpData.strStyle=="metalpowder"){
                tmpData.strStyle="MetalPowder";
                tmpData.PageIndex=M_ENUM_METALPOWDER;
                tmpData.id=M_ENUM_METALPOWDER;
                tmpData.dataColNum=2;//读取的数据是1*5，但在图标中需要显示为5*2
                tmpData.dataRowNum=5;
                QStringList *tmp= tmpStrData.at(j);
                tmpCellData.strName="参数";
                tmpCellData.Data="β角度";
                tmpData.data.push_back(tmpCellData);
                tmpCellData.strName="值";
                tmpCellData.Data=tmp->at(0); 
                tmpData.data.push_back(tmpCellData);

                tmpCellData.strName="参数";
                tmpCellData.Data="R形状控制参数";
                tmpData.data.push_back(tmpCellData);
                tmpCellData.strName="值";
                tmpCellData.Data=tmp->at(1); 
                tmpData.data.push_back(tmpCellData);

                tmpCellData.strName="参数";
                tmpCellData.Data="d0/σy";
                tmpData.data.push_back(tmpCellData);
                tmpCellData.strName="值";
                tmpCellData.Data=tmp->at(2); 
                tmpData.data.push_back(tmpCellData);

                tmpCellData.strName="参数";
                tmpCellData.Data="σ_threshold";
                tmpData.data.push_back(tmpCellData);
                tmpCellData.strName="值";
                tmpCellData.Data=tmp->at(3); 
                tmpData.data.push_back(tmpCellData);

                tmpCellData.strName="参数";
                tmpCellData.Data="ρ0初始密度";
                tmpData.data.push_back(tmpCellData);
                tmpCellData.strName="值";
                tmpCellData.Data=tmp->at(4); 
                tmpData.data.push_back(tmpCellData);
                Mdata.insert(M_ENUM_METALPOWDER,tmpData);
            }else if (tmpData.strStyle=="powderhardening"){
                tmpData.PageIndex=M_ENUM_METALPOWDER;
                tmpData.id=M_ENUM_METALPOWDER;
                tmpData=Mdata.value(M_ENUM_METALPOWDER);
                tmpData.dataColNum2=nSize2;
                tmpData.dataRowNum2=(int)(nSize1/nSize2);
                for (int kk=0; kk<nSize1; kk=kk+tmpData.dataColNum){
                   if(tmpData.dataColNum==2){//==2
                        tmpCellData.strName="pb/σy";
                        QStringList *tmp= tmpStrData.at(j);
                        tmpCellData.Data=tmp->at(kk); 
                        tmpData.data.push_back(tmpCellData);

                        tmpCellData.strName="相对密度";
                        tmpCellData.Data=tmp->at(kk+1); 
                        tmpData.data.push_back(tmpCellData);
                    }
                    Mdata.insert(M_ENUM_METALPOWDER,tmpData);
                }
               }else if(tmpData.strStyle=="rate-dependentplastic"){
				tmpData.strStyle="RateDependent";
                tmpData.PageIndex=M_ENUM_Rate_Dependent_Plastic;
                tmpData.id=M_ENUM_Rate_Dependent_Plastic;
                tmpData.dataColNum=2;

				if( tmpStrData.at(j)->at(0)=="1"){
					tmpData.strStyleParam="HansenSpittel";
					tmpData.dataRowNum=5;
					QStringList *tmp= tmpStrData.at(j);

					tmpCellData.strName="参数";
					tmpCellData.Data="A";
					tmpData.data.push_back(tmpCellData);
					tmpCellData.strName="值";
					tmpCellData.Data=tmp->at(3);
					tmpData.data.push_back(tmpCellData);

					tmpCellData.strName="参数";
					tmpCellData.Data="α";
					tmpData.data.push_back(tmpCellData);
					tmpCellData.strName="值";
					tmpCellData.Data=tmp->at(4); 
					tmpData.data.push_back(tmpCellData);

					tmpCellData.strName="参数";
					tmpCellData.Data="β";
					tmpData.data.push_back(tmpCellData);
					tmpCellData.strName="值";
					tmpCellData.Data=tmp->at(5);
					tmpData.data.push_back(tmpCellData);

					tmpCellData.strName="参数";
					tmpCellData.Data="n";
					tmpData.data.push_back(tmpCellData);
					tmpCellData.strName="值";
					tmpCellData.Data=tmp->at(6); 
					tmpData.data.push_back(tmpCellData);

					tmpCellData.strName="参数";
					tmpCellData.Data="m";
					tmpData.data.push_back(tmpCellData);
					tmpCellData.strName="值";
					tmpCellData.Data=tmp->at(7); 
					tmpData.data.push_back(tmpCellData);
				}else{
					tmpData.strStyleParam="Arrhenius";
					tmpData.dataRowNum=4;
					QStringList *tmp= tmpStrData.at(j);

					tmpCellData.strName="参数";
					tmpCellData.Data="A";
					tmpData.data.push_back(tmpCellData);
					tmpCellData.strName="值";
					tmpCellData.Data=tmp->at(3);
					tmpData.data.push_back(tmpCellData);

					tmpCellData.strName="参数";
					tmpCellData.Data="α";
					tmpData.data.push_back(tmpCellData);
					tmpCellData.strName="值";
					tmpCellData.Data=tmp->at(4); 
					tmpData.data.push_back(tmpCellData);

					tmpCellData.strName="参数";
					tmpCellData.Data="n";
					tmpData.data.push_back(tmpCellData);
					tmpCellData.strName="值";
					tmpCellData.Data=tmp->at(5);
					tmpData.data.push_back(tmpCellData);

					tmpCellData.strName="参数";
					tmpCellData.Data="ΔH";
					tmpData.data.push_back(tmpCellData);
					tmpCellData.strName="值";
					tmpCellData.Data=tmp->at(5); 
					tmpData.data.push_back(tmpCellData);
				}
				Mdata.insert(M_ENUM_Rate_Dependent_Plastic,tmpData);
			}else if(tmpData.strStyle=="damage"){
				tmpData.dataColNum=2;
				tmpData.dataRowNum=1;
				if(tmpStrData.at(j)->at(0)=="1"){tmpData.strStyleParam="NormalizedCL";}
				else if(tmpStrData.at(j)->at(0)=="2"){tmpData.strStyleParam="CockroftLatham";}
				else if(tmpStrData.at(j)->at(0)=="3"){tmpData.strStyleParam="McClintock";tmpData.dataRowNum=2;}
				else if(tmpStrData.at(j)->at(0)=="4"){tmpData.strStyleParam="Freudenthal";}
				else if(tmpStrData.at(j)->at(0)=="5"){tmpData.strStyleParam="RiceTracy";tmpData.dataRowNum=2;}
				else if(tmpStrData.at(j)->at(0)=="6"){tmpData.strStyleParam="Oyane";tmpData.dataRowNum=2;}
				else if(tmpStrData.at(j)->at(0)=="7"){tmpData.strStyleParam="Oyanenegative";tmpData.dataRowNum=2;}
				else if(tmpStrData.at(j)->at(0)=="8"){tmpData.strStyleParam="Ayada";}
				else if(tmpStrData.at(j)->at(0)=="9"){tmpData.strStyleParam="Ayadanegative";}
				else if(tmpStrData.at(j)->at(0)=="10"){tmpData.strStyleParam="Osakada";tmpData.dataRowNum=3;}
				else if(tmpStrData.at(j)->at(0)=="11"){tmpData.strStyleParam="Brozzo";}
				else if(tmpStrData.at(j)->at(0)=="12"){tmpData.strStyleParam="ZhaoKuhn";}
				else if(tmpStrData.at(j)->at(0)=="13"){tmpData.strStyleParam="MPSUTS";tmpData.dataRowNum=2;}

				/*if(tmpStrData.at(j)->at(0)=="1")tmpData.strStyleParam="Normalized C&L";
				else if(tmpStrData.at(j)->at(0)=="2")tmpData.strStyleParam="Cockroft&Latham";
				else if(tmpStrData.at(j)->at(0)=="3")tmpData.strStyleParam="McClintock";
				else if(tmpStrData.at(j)->at(0)=="4")tmpData.strStyleParam="Freudenthal";
				else if(tmpStrData.at(j)->at(0)=="5")tmpData.strStyleParam="Rice&Tracy";
				else if(tmpStrData.at(j)->at(0)=="6")tmpData.strStyleParam="Oyane";
				else if(tmpStrData.at(j)->at(0)=="7")tmpData.strStyleParam="Oyane(negative)";
				else if(tmpStrData.at(j)->at(0)=="8")tmpData.strStyleParam="Ayada";
				else if(tmpStrData.at(j)->at(0)=="9")tmpData.strStyleParam="Ayada(negative)";
				else if(tmpStrData.at(j)->at(0)=="10")tmpData.strStyleParam="Osakada";
				else if(tmpStrData.at(j)->at(0)=="11")tmpData.strStyleParam="Brozzo";
				else if(tmpStrData.at(j)->at(0)=="12")tmpData.strStyleParam="Zhao&Kuhn";
				else if(tmpStrData.at(j)->at(0)=="13")tmpData.strStyleParam="MPS/UTS";*/
				tmpData.strStyle="Damage";
                tmpData.PageIndex=M_ENUM_Damage;
                tmpData.id=M_ENUM_Damage;
				QStringList *tmp= tmpStrData.at(j);

				tmpCellData.strName="参数";
				tmpCellData.Data="损伤阈值";
                tmpData.data.push_back(tmpCellData);
				tmpCellData.strName="值";
				tmpCellData.Data=tmp->at(1);
				tmpData.data.push_back(tmpCellData);

				if(tmpData.dataRowNum>1){
					tmpCellData.strName="参数";
					tmpCellData.Data="a";
					tmpData.data.push_back(tmpCellData);
					tmpCellData.strName="值";
					tmpCellData.Data=tmp->at(2);
					tmpData.data.push_back(tmpCellData);
				}

				if(tmpData.dataRowNum>2){
					tmpCellData.strName="参数";
					tmpCellData.Data="b";
					tmpData.data.push_back(tmpCellData);
					tmpCellData.strName="值";
					tmpCellData.Data=tmp->at(3);
					tmpData.data.push_back(tmpCellData);
				}
				Mdata.insert(M_ENUM_Damage,tmpData);
			}else if(tmpData.strStyle=="dynamicrecrystallization"){
				tmpData.strStyle="DynamicRecrystallization";
                tmpData.PageIndex=M_ENUM_Dynamic_Recrystallization;
                tmpData.id=M_ENUM_Dynamic_Recrystallization;
                tmpData.dataColNum=2;
				tmpData.dataRowNum=21;
				QStringList *tmp= tmpStrData.at(j);

				tmpCellData.strName="参数";
				tmpCellData.Data="a1";
				tmpData.data.push_back(tmpCellData);
				tmpCellData.strName="值";
				tmpCellData.Data=tmp->at(0);
				tmpData.data.push_back(tmpCellData);

				tmpCellData.strName="参数";
				tmpCellData.Data="n1";
				tmpData.data.push_back(tmpCellData);
				tmpCellData.strName="值";
				tmpCellData.Data=tmp->at(1);
				tmpData.data.push_back(tmpCellData);

				tmpCellData.strName="参数";
				tmpCellData.Data="m1";
				tmpData.data.push_back(tmpCellData);
				tmpCellData.strName="值";
				tmpCellData.Data=tmp->at(2);
				tmpData.data.push_back(tmpCellData);

				tmpCellData.strName="参数";
				tmpCellData.Data="q1";
				tmpData.data.push_back(tmpCellData);
				tmpCellData.strName="值";
				tmpCellData.Data=tmp->at(3);
				tmpData.data.push_back(tmpCellData);

				tmpCellData.strName="参数";
				tmpCellData.Data="c1";
				tmpData.data.push_back(tmpCellData);
				tmpCellData.strName="值";
				tmpCellData.Data=tmp->at(4);
				tmpData.data.push_back(tmpCellData);

				tmpCellData.strName="参数";
				tmpCellData.Data="d0";
				tmpData.data.push_back(tmpCellData);
				tmpCellData.strName="值";
				tmpCellData.Data=tmp->at(7);
				tmpData.data.push_back(tmpCellData);

				tmpCellData.strName="参数";
				tmpCellData.Data="a5";
				tmpData.data.push_back(tmpCellData);
				tmpCellData.strName="值";
				tmpCellData.Data=tmp->at(8);
				tmpData.data.push_back(tmpCellData);

				tmpCellData.strName="参数";
				tmpCellData.Data="h5";
				tmpData.data.push_back(tmpCellData);
				tmpCellData.strName="值";
				tmpCellData.Data=tmp->at(9);
				tmpData.data.push_back(tmpCellData);

				tmpCellData.strName="参数";
				tmpCellData.Data="n5";
				tmpData.data.push_back(tmpCellData);
				tmpCellData.strName="值";
				tmpCellData.Data=tmp->at(10);
				tmpData.data.push_back(tmpCellData);

				tmpCellData.strName="参数";
				tmpCellData.Data="m5";
				tmpData.data.push_back(tmpCellData);
				tmpCellData.strName="值";
				tmpCellData.Data=tmp->at(11);
				tmpData.data.push_back(tmpCellData);

				tmpCellData.strName="参数";
				tmpCellData.Data="q5";
				tmpData.data.push_back(tmpCellData);
				tmpCellData.strName="值";
				tmpCellData.Data=tmp->at(12);
				tmpData.data.push_back(tmpCellData);

				tmpCellData.strName="参数";
				tmpCellData.Data="c5";
				tmpData.data.push_back(tmpCellData);
				tmpCellData.strName="值";
				tmpCellData.Data=tmp->at(13);
				tmpData.data.push_back(tmpCellData);

				tmpCellData.strName="参数";
				tmpCellData.Data="a10";
				tmpData.data.push_back(tmpCellData);
				tmpCellData.strName="值";
				tmpCellData.Data=tmp->at(14);
				tmpData.data.push_back(tmpCellData);

				tmpCellData.strName="参数";
				tmpCellData.Data="βd";
				tmpData.data.push_back(tmpCellData);
				tmpCellData.strName="值";
				tmpCellData.Data=tmp->at(15);
				tmpData.data.push_back(tmpCellData);

				tmpCellData.strName="参数";
				tmpCellData.Data="kd";
				tmpData.data.push_back(tmpCellData);
				tmpCellData.strName="值";
				tmpCellData.Data=tmp->at(16);
				tmpData.data.push_back(tmpCellData);

				tmpCellData.strName="参数";
				tmpCellData.Data="a8";
				tmpData.data.push_back(tmpCellData);
				tmpCellData.strName="值";
				tmpCellData.Data=tmp->at(17);
				tmpData.data.push_back(tmpCellData);

				tmpCellData.strName="参数";
				tmpCellData.Data="h8";
				tmpData.data.push_back(tmpCellData);
				tmpCellData.strName="值";
				tmpCellData.Data=tmp->at(18);
				tmpData.data.push_back(tmpCellData);

				tmpCellData.strName="参数";
				tmpCellData.Data="n8";
				tmpData.data.push_back(tmpCellData);
				tmpCellData.strName="值";
				tmpCellData.Data=tmp->at(19);
				tmpData.data.push_back(tmpCellData);

				tmpCellData.strName="参数";
				tmpCellData.Data="m8";
				tmpData.data.push_back(tmpCellData);
				tmpCellData.strName="值";
				tmpCellData.Data=tmp->at(20);
				tmpData.data.push_back(tmpCellData);

				tmpCellData.strName="参数";
				tmpCellData.Data="q8";
				tmpData.data.push_back(tmpCellData);
				tmpCellData.strName="值";
				tmpCellData.Data=tmp->at(21);
				tmpData.data.push_back(tmpCellData);

				tmpCellData.strName="参数";
				tmpCellData.Data="c8";
				tmpData.data.push_back(tmpCellData);
				tmpCellData.strName="值";
				tmpCellData.Data=tmp->at(22);
				tmpData.data.push_back(tmpCellData);
				Mdata.insert(M_ENUM_Dynamic_Recrystallization,tmpData);
            }
        }//for (int j=0;j<MaterialInps.size();j++)
		QMaterNewDlg *MaterNewDlg= new QMaterNewDlg();
		//MaterNewDlg->Mdata=Mdata;//qy20201126
        MaterNewDlg->MFileData[strMName]=Mdata;
		MaterNewDlg->m_NameEdit->setText(strMName);
		MaterNewDlg->setNewData();
		m_NewMap.insert(strMName,MaterNewDlg);
        m_MaterDataWig->WriteImportDataToMFile(strMName,Mdata);
        m_gbGroup2->show();
    } //for (int i=0;i<MaterialInps.size();i++) 
}
//--将TTT属性进行保存
void QMaterPropDlg::ImportInpTTT(ReadInpResultS TTTMaterial)
{
    int nsize = TTTMaterial.TmpHPphaseCurveInps.size();
    int iPhaseNum=0;//总相数
    TTTSetDataS SetDataS;
    //--相变积分
    QMap<int,QMap<int,PhaseChangeTabS> > AllPhaseCurve;
    QMap<int,PhaseChangeTabS> PhaseCurve,PhaseCurveOld;
    PhaseChangeTabS TmpPhaseCurve;
    //--初始值
    QMap<int ,QString> strInitVal;
    QString strInitValOld;

    int iBaseNum=0;
    int iPhaseOld=1;//每次均为从1开始
    for (int i=0; i<nsize; i++){//*PHASECURVE
        if (TTTMaterial.TmpHPphaseCurveInps.at(i).strName.toLower()!="phasecurve"||
            TTTMaterial.TmpHPphaseCurveInps.at(i).strPhaseCurveType.toLower()!="ttt")
            continue;
        
        int nPhaseSize=TTTMaterial.TmpHPphaseCurveInps.at(i).strHeadParam.size();
        QStringList strListData;
        strListData=TTTMaterial.TmpHPphaseCurveInps.at(i).strData;
        for (int jjphaseSizeCnt=0; jjphaseSizeCnt<nPhaseSize; jjphaseSizeCnt++){
            QStringList strListHeadParam=TTTMaterial.TmpHPphaseCurveInps.at(i).strHeadParam.at(jjphaseSizeCnt).split(",",QString::SkipEmptyParts);
            iPhaseNum=strListHeadParam.at(0).toInt();
            int iTabIndex=strListHeadParam.at(1).toInt();//曲线号
            int iDataNum=strListHeadParam.at(2).toInt();//数据点数
            QString TmpstrInitVal=strListHeadParam.at(4);//数据初值

            if (iPhaseNum!=iPhaseOld&&jjphaseSizeCnt){
                //strListHeadParam=TTTMaterial.TmpHPphaseCurveInps.at(i).strHeadParam.at(jjphaseSizeCnt-1).split(",",QString::SkipEmptyParts);
                strInitVal.insert(iPhaseOld,strInitValOld);
                AllPhaseCurve.insert(iPhaseOld,PhaseCurveOld);
                PhaseCurve.clear();
                iPhaseOld=iPhaseNum;
            }
            //--
            TmpPhaseCurve.iNum=iDataNum;
            TmpPhaseCurve.strChangeP=strListHeadParam.at(3);//转变分数
            TmpPhaseCurve.strData.clear();
            if(iDataNum){
                for (int tt=iBaseNum;tt<iBaseNum+iDataNum;tt++){     
                    TmpPhaseCurve.strData<<strListData.at(tt);
                }
                iBaseNum+=iDataNum;
            }
            PhaseCurve.insert(iTabIndex,TmpPhaseCurve);//
            PhaseCurveOld = PhaseCurve;
            strInitValOld = TmpstrInitVal;

            if (jjphaseSizeCnt==nPhaseSize-1){
                strInitVal.insert(iPhaseNum,strInitValOld);
                AllPhaseCurve.insert(iPhaseNum,PhaseCurve);
            }
        } 
    }
    //--潜热
    QMap<int,QString> strMapLatenTheat;
    nsize = TTTMaterial.TmpHPphaseElateHeatInps.size();
    QStringList strEleatheat;
    for (int i=0;i<nsize;i++){
         strEleatheat=TTTMaterial.TmpHPphaseElateHeatInps.at(i).strHeadParam;
         for (int j=0;j<strEleatheat.size();j++){
             QStringList strList=strEleatheat.at(j).split(",",QString::SkipEmptyParts);
             int id=strList.at(0).toInt();
             QString strVal=strList.at(1);
             strMapLatenTheat.insert(id,strVal);
         }
    }
    //--硬度
    QMap<int,QString> strMapHard;
    nsize = TTTMaterial.TmpHPphaseHardInps.size();
    QStringList strHard;
    for (int i=0;i<nsize;i++){
        strHard=TTTMaterial.TmpHPphaseHardInps.at(i).strHeadParam;
        for (int j=0;j<strHard.size();j++){
            QStringList strList=strHard.at(j).split(",",QString::SkipEmptyParts);
            int id=strList.at(0).toInt();
            QString strVal=strList.at(1);
            strMapHard.insert(id,strVal);
        }
    }
    //--屈服强度
    QString strMapYS;
    nsize=TTTMaterial.TmpHPPhaseYSInps.size();
    for (int i=0;i<nsize;i++)
    {
        strMapYS=TTTMaterial.TmpHPPhaseYSInps.at(i).strHeadParam;
    }
    //--晶粒尺寸
    QString strMapGS;
    nsize=TTTMaterial.TmpHPPhaseGSInps.size();
    for (int i=0;i<nsize;i++)
    {
        strMapGS=TTTMaterial.TmpHPPhaseGSInps.at(i).strHeadParam;
    }

    //--热处理方式
    QString strMapPhaseCtrol;
    nsize=TTTMaterial.TmpHPphaseCtrolInps.size();
    for (int i=0;i<nsize;i++)
    {
        strMapPhaseCtrol=TTTMaterial.TmpHPphaseCtrolInps.at(i).strHeadParam;
    }

     QMap<int,PhaseBaseItem> mapPhaseBSS;//--相平衡
     mapPhaseBSS=ProcessMapData(TTTMaterial.TmpHPphaseBalanceInps);
     //PhaseBaseItem PhaseBaseItemD;
     //nsize=TTTMaterial.TmpHPphaseBalanceInps.size();
     //QStringList strListBSHeadParam;QStringList strListBSData;
     //iBaseNum=0;
     //for (int i=0;i<nsize;i++){
     //    strListBSHeadParam=TTTMaterial.TmpHPphaseBalanceInps.at(i).strHeadParam;//2,9;3,12;4,12;
     //    strListBSData=TTTMaterial.TmpHPphaseBalanceInps.at(i).strData;
     //    for (int j=0;j<strListBSHeadParam.size();j++){
     //        QStringList strListHead=strListBSHeadParam.at(j).split(",",QString::SkipEmptyParts);
     //        int strId=strListHead.at(0).toInt();
     //        int strNum=strListHead.at(1).toInt();
     //        PhaseBaseItemD.iNum=strNum;  
     //        QStringList strListBSD;
     //        for (int kk=iBaseNum;kk<iBaseNum+strNum;kk++){
     //           strListBSD<<strListBSData.at(kk);    
     //        }
     //        iBaseNum+=strNum;
     //        PhaseBaseItemD.strData=strListBSD;
     //        mapPhaseBSS.insert(strId,PhaseBaseItemD);
     //    }
     //}
     //--孕育期(未完成)
     QMap<int,PhaseBaseItem> mapPhaseIP;//孕育期
     mapPhaseIP=ProcessMapData(TTTMaterial.TmpHPincubatePeriodInps);
     QMap<int,PhaseBaseItem> mapPhasePropSpec;//比热
     mapPhasePropSpec=ProcessMapData(TTTMaterial.TmpHPPhasePropSpecInps);
     QMap<int,PhaseBaseItem> mapPhasePropCond;//导热
     mapPhasePropCond=ProcessMapData(TTTMaterial.TmpHPPhasePropCondInps);
    

    SetDataS.iPhaseNum=iPhaseNum;
    SetDataS.PhaseCurve=AllPhaseCurve;
    SetDataS.strInitVal=strInitVal;
    SetDataS.strLatenTheat=strMapLatenTheat;
    SetDataS.strHard=strMapHard;
    SetDataS.PhaseBalance=mapPhaseBSS;
    SetDataS.PhaseIP=mapPhaseIP;
    SetDataS.PhasePropCond=mapPhasePropCond;
    SetDataS.PhasePropSpec=mapPhasePropSpec;
    SetDataS.strYS=strMapYS;
    SetDataS.strGS=strMapGS;
    SetDataS.strPhaseCtrol=strMapPhaseCtrol;

    m_WidTTT->SetInpData(SetDataS);
}
QMap<int,PhaseBaseItem> QMaterPropDlg::ProcessMapData(QList<HPphaseBalanceInps> BalanceList)
{
    int nsize=BalanceList.size();
    QMap<int,PhaseBaseItem> mapPhaseBSS;
    PhaseBaseItem PhaseBaseItemD;
    QStringList strListBSHeadParam;QStringList strListBSData;
    int iBaseNum=0;
    for (int i=0;i<nsize;i++){
        strListBSHeadParam=BalanceList.at(i).strHeadParam;//2,9;3,12;4,12;
        strListBSData=BalanceList.at(i).strData;
        for (int j=0;j<strListBSHeadParam.size();j++){
            QStringList strListHead=strListBSHeadParam.at(j).split(",",QString::SkipEmptyParts);
            int strId=strListHead.at(0).toInt();
            int strNum=strListHead.at(1).toInt();
            PhaseBaseItemD.iNum=strNum;  
            QStringList strListBSD;
            for (int kk=iBaseNum;kk<iBaseNum+strNum;kk++){
                strListBSD<<strListBSData.at(kk);    
            }
            iBaseNum+=strNum;
            PhaseBaseItemD.strData=strListBSD;
            mapPhaseBSS.insert(strId,PhaseBaseItemD);
        }
    }
    return mapPhaseBSS;
}
//导入xml文件
void QMaterPropDlg::BtnImportDataSlot()
{
	if (m_MaterDataWig->ImportData())
	{
		 QString strMName;
		 strMName=m_MaterDataWig->m_NowFileName;
         m_gbGroup2->show();
         m_NameEdit->setText(strMName);
         m_MaterDataWig->SetTabWidgetData(m_MaterDataWig->Mdata.begin()->PageIndex);
         m_TreeModel->AddMaterialUserName(m_MaterDataWig->m_NowFileName);
		 QMaterNewDlg *MaterNewDlg= new QMaterNewDlg();
		 MaterNewDlg->Mdata=m_MaterDataWig->Mdata;
		 MaterNewDlg->m_NameEdit->setText(strMName);
		 m_NewMap.insert(strMName,MaterNewDlg);
	}
}
//导出xml文件
void QMaterPropDlg::BtnExportDataSlot()
{
	m_MaterDataWig->ExportData();
}
//--List 
void QMaterPropDlg::MaterListSlot(int Row)
{
	if (Row <0){
		return;
	}
	m_Name=m_ListWiget->item(Row)->text();
	m_gbGroup2->setTitle(m_NowShowFileName+" - "+m_Name);
	for (int i=0;i<m_comList.size();i++)
	{
		if (m_comList[i] == m_Name)
		{
			//m_MaterDataWig->ChgWidgetSlot(i,m_comList[i]);
            m_MaterDataWig->ChgWidgetSlot(i,m_comList[i],m_NowShowFileName);
			break;
		}
	}
}
//--ListXIAN
void QMaterPropDlg::UpDataMaterListSLot(QMap<int, MCompoment> Mdata)
{
    QMap<int, MCompoment>::Iterator it = Mdata.begin();
    QString str;
    int PageIndex,PageListRow=0;
    MCompoment tmpCmp;
    int rowCount;
    m_ListWiget->clear();
    m_ListWiget2->clearContents();
    
    for (it; it != Mdata.end(); ++it)
    {
        rowCount=Mdata.size();
        tmpCmp=it.value();
        str=tmpCmp.strStyle;
        PageIndex = tmpCmp.id;
        m_NowShowFileName=tmpCmp.FileName;
        ListShowSet(PageIndex);
        //--
        m_ListWiget2->setRowCount(rowCount);
        QTableWidgetItem *tmpItem1=new QTableWidgetItem(m_comList[PageIndex]);
        m_ListWiget2->setItem(PageListRow,0,tmpItem1);
        if (PageIndex == M_ENUM_METALPOWDER||PageIndex ==M_ENUM_Rate_Dependent_Plastic||
			PageIndex ==M_ENUM_Damage||PageIndex ==M_ENUM_Dynamic_Recrystallization){
            QTableWidgetItem *tmpItem2=new QTableWidgetItem(tr("F"));
            m_ListWiget2->setItem(PageListRow,1,tmpItem2);
        }else{
            QTableWidgetItem *tmpItem2=new QTableWidgetItem(tr("F(T)"));
            m_ListWiget2->setItem(PageListRow,1,tmpItem2);
        }
        QTableWidgetItem *tmpItem=new QTableWidgetItem();
        m_ListWiget2->setItem(PageListRow,2,tmpItem);
        tmpItem->setIcon(QIcon(":/images/save.png"));
        tmpItem->setFlags(tmpItem->flags() &=(~Qt::ItemIsEditable));
        tmpItem->setTextAlignment(Qt::AlignVCenter);
        PageListRow++;
       /* QComboBox *combox=new QComboBox();
        combox->addItem("数据单位1");
        combox->addItem("数据单位2");
        combox->setFrame(false);
        m_ListWiget2->setCellWidget(PageIndex,3,combox);

        QComboBox *combox1=new QComboBox();
        combox1->addItem("单位1");
        combox1->addItem("单位2");
        combox1->setFrame(true);
        m_ListWiget2->setCellWidget(PageIndex,4,combox1);*/
        //--
    }
}
////--菜单项对应
//void QMaterPropDlg::DensitySlot()
//{
//	ListShowSet(M_ENUM_DENSITY);
//	//m_MaterDataWig->ChgWidgetSlot(M_ENUM_DENSITY,m_comList[M_ENUM_DENSITY]);//Dialog显示
//    m_gbGroup2->show();
//    m_MaterDataWig->SetTabWidgetData(M_ENUM_DENSITY);
////--List显示
//}
////--菜单项对应
//void QMaterPropDlg::ElasticSlot()
//{
//	ListShowSet(M_ENUM_ELASTIC);
//    m_gbGroup2->show();
//    m_MaterDataWig->SetTabWidgetData(M_ENUM_ELASTIC);
//	//--List显示
//}
////--菜单项对应
//void QMaterPropDlg::PlasticSlot()
//{
//	ListShowSet(M_ENUM_PLASTIC);
//    m_gbGroup2->show();
//    m_MaterDataWig->SetTabWidgetData(M_ENUM_PLASTIC);
//	//--List显示
//}
////--菜单项对应
//void QMaterPropDlg::ExpansionSlot()
//{
//	ListShowSet(M_ENUM_EXPANS);
//    m_gbGroup2->show();
//    m_MaterDataWig->SetTabWidgetData(M_ENUM_EXPANS);
//	//--List显示
//}
////--菜单项对应
//void QMaterPropDlg::ConductSlot()
//{
//	ListShowSet(M_ENUM_CONDUCT);
//    m_gbGroup2->show();
//	m_MaterDataWig->SetTabWidgetData(M_ENUM_CONDUCT);
//    //--List显示
//}
////--菜单项对应
//void QMaterPropDlg::SpecificSlot()
//{
//	ListShowSet(M_ENUM_SPECIFICE);
//	//m_MaterDataWig->ChgWidgetSlot(M_ENUM_SPECIFICE,m_comList[M_ENUM_SPECIFICE]);//Dialog显示
//	//--List显示
//    m_gbGroup2->show();
//    m_MaterDataWig->SetTabWidgetData(M_ENUM_SPECIFICE);
//}
////--菜单项对应
//void QMaterPropDlg::MatelPowderSlot()
//{
//    ListShowSet(M_ENUM_METALPOWDER);
//   
//    //--List显示
//    m_gbGroup2->show();
//    m_MaterDataWig->SetTabWidgetData(M_ENUM_METALPOWDER);
//}
//---将数据列表显示在ListWiget中；
void QMaterPropDlg::ListShowSet(int index)
{
	int i=0;
	m_gbGroup2->setTitle(m_NowShowFileName+" - "+m_comList[index]);
	if (m_ListWiget->count()<=0)
	{
		m_ListWiget->addItem(m_comList[index]);
		MaterListSlot(0);

	}else{
	    for (i=0;i<m_ListWiget->count();i++)
	    {
		    if (m_comList[index]==m_ListWiget->item(i)->text()){
			    m_ListWiget->setCurrentRow(i);//该行被选中
			    break;
		    }
	    }
	    if (i == m_ListWiget->count())
	    {
		    m_ListWiget->addItem(m_comList[index]);	
		    m_ListWiget->setCurrentRow(i);//该行被选中
	    }
	}
}
/*
刪除選中的List表格
*/
void QMaterPropDlg::BtnDeletSlot()
{
	int indexCNT = m_ListWiget->count();
	int indexRow = m_ListWiget->currentRow();

	if (indexRow == 0 && indexCNT==1)
	{	
		m_MaterDataWig->HideALL();
		m_gbGroup2->setTitle("");
	}
	QListWidgetItem *item = m_ListWiget->takeItem(indexRow);
	delete item;

    if (m_IndexMaterName>1)
    {
        m_IndexMaterName--;
    }
    SectionS tmpMater;
    tmpMater.MaterPara.id = m_IndexMaterName;
    m_Name=m_NameEdit->text();
    tmpMater.MaterPara.Name = m_Name;

    emit emitMaterParam(tmpMater);
	
}
/*
新建新的材料对话框
*/
void QMaterPropDlg::NewMatialDlgSlot()
{
    QMaterNewDlg *MaterNewDlg= new QMaterNewDlg();
    int ret=MaterNewDlg->exec();
    if (ret == QDialog::Accepted){
        QString str=MaterNewDlg->m_NameEdit->text();
        //m_MaterNewDlg.push_back(MaterNewDlg);
        m_NewMap.insert(str,MaterNewDlg);
    
        m_TreeModel->AddMaterialUserName(str);    //--Tree中加入名字
        m_MaterDataWig->WriteImportDataToMFile(str,MaterNewDlg->Mdata);//--写入到wig中
        m_gbGroup2->show();//显示当前
    }else{

    }
    
}

void QMaterPropDlg::contextMaterMenuSlot(const QPoint &p)
{
    QModelIndex index = m_TreeView->currentIndex();
    QString fileName;

    QMenu *RightMenu = new QMenu;
    bool f = m_TreeView->model()->hasChildren(index);
    if (!f)
    {
        fileName=m_TreeView->model()->data(index).toString();
        RightMenu->addAction(tr("编辑"),this,SLOT(EditMaterialSlot()));
        m_NowDelFileName= fileName;
    }
	RightMenu->addAction(tr("展开"),m_TreeView,SLOT(expandAll()));//(ExpandAllSlot()));
	RightMenu->addAction(tr("折叠"),m_TreeView,SLOT(collapseAll()));//(CollapseAllSlot()));
	if (!f)
    {
		RightMenu->addAction(tr("删除"),this,SLOT(DelFileNameSlot()));
    }
    RightMenu->exec(QCursor::pos());
}
void QMaterPropDlg::DelFileNameSlot()
{
    if (m_NowDelFileName!="")
    {
        if(m_TreeModel->DelMaterialUserName(m_NowDelFileName)==-1){//最后一个文档
           m_gbGroup2->hide();
           m_ListWiget->clear();
           m_ListWiget2->clearContents();
        };

        m_TreeView->expandAll();
    }  
}
void QMaterPropDlg::EditMaterialSlot()
{
    int tmpK;
    if (m_NowDelFileName!=""){
        if(m_NewMap.find(m_NowDelFileName)==m_NewMap.end()){//没有找到
           tmpK=0;
        }else{//已经存在列表中
           m_NewMap[m_NowDelFileName]->m_NameEdit->setText(m_NowDelFileName);
           m_NewMap[m_NowDelFileName]->MFileData[m_NowDelFileName]=m_NewMap[m_NowDelFileName]->Mdata;
           m_NewMap[m_NowDelFileName]->setNewData();
           int ret=m_NewMap[m_NowDelFileName]->exec();

           if (ret == QDialog::Accepted){
			  
               QString str=m_NewMap[m_NowDelFileName]->m_NameEdit->text();
			    m_NewMap.insert(str,m_NewMap[m_NowDelFileName]);
               m_MaterDataWig->WriteImportDataToMFile(str,m_NewMap[str]->Mdata);//--写入到wig中
               QModelIndex iTmp;
               m_TreeModel->ChangeMaterialUserName(m_NowDelFileName,str,iTmp);
               ChangeModelIndexSlot(iTmp);
               //m_gbGroup2->show();//显示当前
			   m_TreeView->expandAll();
           }
        }
    }
}
//****************************************************
// Method:    单击树节点的响应;
// Returns:   void;
// Parameter: index     树节点
// Author:
//****************************************************
void QMaterPropDlg::ChangeModelIndexSlot(const QModelIndex & index)
{
     QString type = index.data(Qt::UserRole+3).toString();
     QMap<int, MCompoment> tmpMdata;
     
    /* for (iitt; iitt!=m_MaterDataWig->MFileData.end();iitt++){
         if (type == iitt.key()){
             tmpMdata=iitt.value();
             QMap<int, MCompoment>::Iterator it = tmpMdata.begin();
             m_NowShowFileName = type;
             m_NameEdit->setText(m_NowShowFileName);
             for (it; it != tmpMdata.end(); it++){
                 UpDataMaterListSLot(tmpMdata);
             }
             break;
         }      
     }*/
     if (m_MaterDataWig->MFileData.find(type)==m_MaterDataWig->MFileData.end())
     {//不存在
         if(m_NewMap.find(type)==m_NewMap.end()){//不存在
             return;
         }else{
           QMap<QString,QMap<int,MCompoment> >::Iterator iitt= m_NewMap[type]->MFileData.begin();
            
           for (iitt; iitt!=m_NewMap[type]->MFileData.end();iitt++){
               if (type == iitt.key()){
                   tmpMdata=iitt.value();
                   QMap<int, MCompoment>::Iterator it = tmpMdata.begin();
                   QString str;int PageIndex;
                   m_NowShowFileName = type;
                   m_NameEdit->setText(m_NowShowFileName);
                   for (it; it != tmpMdata.end(); it++){UpDataMaterListSLot(tmpMdata);}
                   break;
               }      
           }
         }
     }else{//已经存在的m_MaterDataWig
         QMap<QString,QMap<int,MCompoment> >::Iterator iitt= m_MaterDataWig->MFileData.begin();
		 for (int kk=0;kk<10;kk++)//清除
		 {
			 m_MaterDataWig->m_DataList[kk].clear();
		 }
		 
         for (iitt; iitt!=m_MaterDataWig->MFileData.end();iitt++){
             if (type == iitt.key()){
                 tmpMdata=iitt.value();
                 QMap<int, MCompoment>::Iterator it = tmpMdata.begin();
                 QString str;int PageIndex;
                 m_NowShowFileName = type;
                 m_NameEdit->setText(m_NowShowFileName);
                 if (tmpMdata.isEmpty()){//clear now 
                    UpDataMaterListSLot(tmpMdata);
                    m_gbGroup2->hide();
                 }
                 for (it; it != tmpMdata.end(); it++){
                     m_gbGroup2->show();
                     UpDataMaterListSLot(tmpMdata);
                 }
                 break;
             }   
         }
     }
}
//--单击tableview显示列表
void QMaterPropDlg::DataTestSlot(int Row,int Col)
{
    QString m_Name;
    if (Row <0){
        return;
    }
    if (Col==2)
    {
        m_Name=m_ListWiget2->item(Row,0)->text();//m_ListWiget->item(Row)->text();
        m_gbGroup2->setTitle(m_NowShowFileName+" - "+m_Name);
        //for (int i=0; i<TYPENUM ;i++)
        //{
        //    if (m_comList[i] == m_Name)
        //    {
        //        //m_MaterDataWig->ChgWidgetSlot(i,m_comList[i]);
        //        m_MaterDataWig->ChgWidgetSlot(i,m_Name,m_NowShowFileName);
        //        break;
        //    }
        //}
		int id=m_comList.indexOf(m_Name);
		if(id!=-1)
			 m_MaterDataWig->ChgWidgetSlot(id,m_Name,m_NowShowFileName);

    }
}
