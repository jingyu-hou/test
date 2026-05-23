#include "QHPPartDlg.h"

QHPPartDlg::QHPPartDlg(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("AESim-FM"));
	ENumberS=0;
    m_SectionManageDlg = NULL;
    m_MaterManagerDlg  = NULL;
 

    QHBoxLayout *Hlayout1= new QHBoxLayout();
    QHBoxLayout *Hlayout2= new QHBoxLayout();
	QHBoxLayout *Vlayout = new QHBoxLayout();
	QVBoxLayout *hlay = new QVBoxLayout();
    
    m_NewSectionBtn = new QPushButton(tr("新增"));
    m_DelSectionBtn = new QPushButton(tr("删除"));
	m_ImprotPart=new QPushButton(tr("导入部件"));
    m_ApplaySectionBtn = new QPushButton(tr("应用"));
    m_NextSectionBtn = new QPushButton(tr("下一步"));
    Hlayout1->addStretch();
    Hlayout1->addWidget(m_NewSectionBtn);
    Hlayout1->addWidget(m_DelSectionBtn);
	Hlayout1->addWidget(m_ImprotPart);
    Hlayout1->addStretch();
    Hlayout2->addWidget(m_ApplaySectionBtn);
    Hlayout2->addWidget(m_NextSectionBtn);
	QGroupBox *GBox=new QGroupBox(tr("分析模型选项"),this);
	m_Tab1=new QWidget();
	D3Model=new QRadioButton(tr("3D"));
	AxialSymmetry=new QRadioButton(tr("轴对称"));
	PlaneStress=new QRadioButton(tr("平面应力"));
	PlaneStrain=new QRadioButton(tr("平面应变"));
	//Default0=new QRadioButton(tr("与Inp文件单元类型一致"));
    //Default0->setChecked(true);
	AxialSymmetry->setChecked(true);
	hlay->addWidget(D3Model);
	hlay->addWidget(AxialSymmetry);
	hlay->addWidget(PlaneStress);
	hlay->addWidget(PlaneStrain);
	//hlay->addWidget(Default0);
	GBox->setLayout(hlay);//将hlay放入GBox容器中
	connect(D3Model,SIGNAL(clicked(bool)),this,SLOT(AMOptions(bool)));
	connect(AxialSymmetry,SIGNAL(clicked(bool)),this,SLOT(AMOptions(bool)));
	connect(PlaneStress,SIGNAL(clicked(bool)),this,SLOT(AMOptions(bool)));
	connect(PlaneStrain,SIGNAL(clicked(bool)),this,SLOT(AMOptions(bool)));
	connect(m_ImprotPart,SIGNAL(clicked()),this,SLOT(MergeSlot()));
	//connect(Default0,SIGNAL(clicked(bool)),this,SLOT(AMOptions(bool)));

    //--部件列表
    int iniRowCnt=0;//
    QStringList strList;
    //strList<<"部件"<<"材料"<<"单元集合"<<"TTT/CCT";
	strList<<"部件"<<"部件类型"<<"材料"<<"单元集合"<<"TTT/CCT";
    
	m_TableWiget = new QTableWidget();
	m_TableWiget->setColumnCount(strList.size());
	m_TableWiget->setRowCount(iniRowCnt);
	m_TableWiget->verticalHeaderItem(1);
	m_TableWiget->setHorizontalHeaderLabels(strList);

    //晶粒度
    QTableWidgetItem *tmpItem2=new QTableWidgetItem(tr("Part1"));
    m_TableWiget->setItem(0,0,tmpItem2);

    //温度场
    tmpItem2=new QTableWidgetItem(tr("Part2"));
    m_TableWiget->setItem(1,0,tmpItem2);

	//增加
	tmpItem2=new QTableWidgetItem(tr("Part3"));
    m_TableWiget->setItem(2,0,tmpItem2);


    Vlayout->addWidget(m_TableWiget);
	Vlayout->addWidget(GBox);

    QVBoxLayout *MainLayout = new QVBoxLayout(this);
    MainLayout->addLayout(Vlayout);
    MainLayout->addLayout(Hlayout1);
    MainLayout->addLayout(Hlayout2);

    setLayout(MainLayout);
    //this->setMinimumWidth(300);
    for (int tt=0;tt<iniRowCnt;tt++)
    {
		/*QPushButton* m_ImprotPart = new QPushButton(tr("导入"));
		m_TableWiget->setCellWidget(tt,1,m_ImprotPart);
		connect(m_ImprotPart,SIGNAL(clicked()),this,SLOT(MergeSlot()));*/

		MyCombox *tmpCombox01=new MyCombox(this,tt);
        tmpCombox01->addItem(tr("弹塑性"));
        tmpCombox01->addItem(tr("弹性"));
		tmpCombox01->addItem(tr("刚体"));
        tmpCombox01->setFrame(false);
        m_TableWiget->setCellWidget(tt,1,tmpCombox01);
		m_comboxPartType.insert(tt,tmpCombox01);

        MyCombox *tmpCombox0=new MyCombox(this,tt);
        tmpCombox0->addItem(tr("新建"));
        tmpCombox0->setFrame(false);
        m_TableWiget->setCellWidget(tt,2,tmpCombox0);
        connect(tmpCombox0,SIGNAL(clicked(int,int)),this,SLOT(comBoxMChgSlot()));
        connect(tmpCombox0,SIGNAL(doubleclicked(int,int)),this,SLOT(comBoxDoubleSlot()));
        m_comboxMaterial.insert(tt,tmpCombox0);//push_back(tmpCombox0);

        MyCombox *tmpCombox1=new MyCombox(this,tt);
        //m_comboxElset[tt]->addItem("Element Set1");
        //m_comboxElset[tt]->addItem("Element Set2");
        //tmpCombox1->addItem(tr("新建"));
        tmpCombox1->setFrame(false);
        m_TableWiget->setCellWidget(tt,3,tmpCombox1);
        m_comboxElset.insert(tt,tmpCombox1); 

        MyCombox *tmpCombox=new MyCombox(this,tt);
        tmpCombox->addItem(tr("TTT"));
        tmpCombox->addItem(tr("CCT"));
        tmpCombox->setFrame(false);
        m_TableWiget->setCellWidget(tt,4,tmpCombox);
    }
	m_TableWiget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	m_TableWiget->horizontalHeader()->setResizeMode(0, QHeaderView::Interactive);
	m_TableWiget->horizontalHeader()->setResizeMode(1, QHeaderView::Interactive);
	m_TableWiget->horizontalHeader()->setResizeMode(2, QHeaderView::Interactive);
     //---Btn
    connect(m_NewSectionBtn,SIGNAL(clicked()),this,SLOT(SHPPropSlot()));//(NewSectionSlot()));
    connect(m_ApplaySectionBtn,SIGNAL(clicked()),this,SLOT(ApplayPropSlot()));   
    connect(m_DelSectionBtn,SIGNAL(clicked()),this,SLOT(DeletePropSlot()));
    connect(m_NextSectionBtn,SIGNAL(clicked()),this, SLOT(NextBtnSlot()));
    m_MaterManagerDlg = QMaterManagerDlg::GetInstance();
    m_SectionManageDlg = QSectionManagerDlg::GetInstance();
 
    connect(m_MaterManagerDlg,SIGNAL(emitUpMaterListData(SectionS)),m_SectionManageDlg,SLOT(UpDataMater(SectionS)));
   
    this->setMinimumWidth(600);
}

QHPPartDlg::~QHPPartDlg()
{
    if (m_SectionManageDlg!=NULL)
    {
        delete m_SectionManageDlg;
        m_SectionManageDlg = NULL;
    }
    if (m_MaterManagerDlg !=NULL)
    {
        delete m_MaterManagerDlg;
        m_MaterManagerDlg = NULL;
    }
    m_comboxElset.clear();
    m_comboxMaterial.clear();
	m_comboxPartType.clear();
}

//将读取的INP数据显示在当前的表格中
//--清除当前材料数据库中的数据，加载INP文件中的材料数据；
void QHPPartDlg::SetInpData(ReadInpResultS ReadInpData)
{
    int nRow = m_TableWiget->rowCount();
    int nSectionRow = ReadInpData.TmpSectionInps.size();

    if (nRow < nSectionRow) {
        m_TableWiget->setRowCount(nSectionRow);
        for (int i=nRow; i<nSectionRow; i++){
            QTableWidgetItem *tmpItem2=new QTableWidgetItem(QString("NewSection-%1").arg(i));
            m_TableWiget->setItem(i,0,tmpItem2);
			/*QPushButton* m_ImprotPart = new QPushButton(tr("导入"));
		    m_TableWiget->setCellWidget(i,1,m_ImprotPart);*/
		    MyCombox *tmpCombox01=new MyCombox(this,i);
            tmpCombox01->addItem(tr("弹塑性"));
            tmpCombox01->addItem(tr("弹性"));
		    tmpCombox01->addItem(tr("刚体"));
            tmpCombox01->setFrame(false);
            m_TableWiget->setCellWidget(i,1,tmpCombox01);
		    m_comboxPartType.insert(i,tmpCombox01);

            MyCombox *tmpComboxMateial=new MyCombox(this,i);//QComboBox();
            MyCombox *tmpCombox=new MyCombox(this,i);
			MyCombox *tmpComboxTTT=new MyCombox(this,i);
            m_TableWiget->setCellWidget(i,2,tmpComboxMateial);
            m_TableWiget->setCellWidget(i,3,tmpCombox);
            m_comboxMaterial.insert(nRow,tmpComboxMateial);
            m_comboxElset.insert(nRow,tmpCombox);

			tmpComboxTTT->addItem(tr("TTT"));
			tmpComboxTTT->addItem(tr("CCT"));
			tmpComboxTTT->setFrame(false);
			m_TableWiget->setCellWidget(i,4,tmpComboxTTT);
			
        } 
    }
    int nMaterialSize = ReadInpData.TmpMaterialInps.size();
    int nElementSize = ReadInpData.TmpElSetInps.strElSetName.size();
    nRow = m_TableWiget->rowCount();
    //--材料ComboBox添加
    QStringList materList;
    for (int kk=0; kk<nMaterialSize; kk++){
        materList<<ReadInpData.TmpMaterialInps.at(kk).strMaterialName;
    }

    for (int i=0;i<nRow;i++){
        MyCombox *tmpComboxM = (MyCombox*)m_TableWiget->cellWidget(i,ENUM_PART_Material);
        tmpComboxM->clear();
        tmpComboxM->addItems(materList);
        tmpComboxM->addItem("新建");
        m_comboxMaterial.insert(i,tmpComboxM);    
		connect(m_comboxMaterial[i],SIGNAL(clicked(int,int)),this,SLOT(comBoxMChgSlot()));
        connect(m_comboxMaterial[i],SIGNAL(doubleclicked(int,int)),this,SLOT(comBoxDoubleSlot()));
    }
    //--根据当前Section 行所选择的Material，进行设置当前combox1的index；
    for (int i=0;i<ReadInpData.TmpSectionInps.size();i++){
        QString str=ReadInpData.TmpSectionInps.at(i).StrMaterial;
        int id=m_comboxMaterial[i]->findText(str);
        m_comboxMaterial[i]->setCurrentIndex(id);
    }
    //--材料数据内容添加
    m_MaterManagerDlg->GetInstance()->m_MaterProDlg->GetInstance()->ImportInpMaterial(ReadInpData.TmpMaterialInps);
    m_MaterManagerDlg->GetInstance()->m_MaterProDlg->GetInstance()->ImportInpTTT(ReadInpData);
	//m_MaterManagerDlg->GetInstance()->m_TTTDlg->GetInstance2()->ImportInpTTT(ReadInpData);

    //--单元集合
    elementList=ReadInpData.TmpElSetInps.strElSetName;
	//以下几行将abaqus和gmesh生成的多余单元集合删除
	int IJKI=0;
	/*for(int i=0;i<elementList.size();i++){
		if(elementList.at(i).mid(0,1)=="_"){
			elementList.removeAt(i);
			i=0;
			break;
		}
	}*/
	
	int JK1=elementList.size();
	int JK=JK1;
	while(IJKI<JK){//abaqus
		QString elsetName=elementList.at(IJKI);
		if(elementList.at(IJKI).mid(0,1)=="_"){
			elementList.removeAt(IJKI);
			IJKI=0;
		}else{
             IJKI++;
		}
		JK=elementList.size();
	}
    
	if(IJKI==JK1)IJKI=0;
	else IJKI=elementList.size();
	while(IJKI<elementList.size()){//gmesh
		QString elsetName=elementList.at(IJKI);
		int IK=IJKI;
		for(int j=0;j<ReadInpData.TmpSurfaceInps.strData.size();j++){
			QString strName=ReadInpData.TmpSurfaceInps.strData.at(j).split(",",QString::SkipEmptyParts).at(0);
			if(strName==elsetName){
				elementList.removeAt(IJKI);
				IJKI++;
				break;	
			}
		}
		if(IJKI==0||IK==IJKI)IJKI++;
		else IJKI=0;
	}
	//以上几行将abaqus和gmesh生成的多余单元集合删除

    for (int i=0;i<nRow;i++){
        MyCombox *tmpComboxElset = (MyCombox*)m_TableWiget->cellWidget(i,ENUM_PART_ESET);
        tmpComboxElset->clear();
        tmpComboxElset->addItems(elementList);
        //tmpComboxElset->addItem("新建"); 
        m_comboxElset.insert(i,tmpComboxElset);
		connect(m_comboxElset[i],SIGNAL(clicked(int,int)),this,SLOT(ComboElSlot()));
    }
    for (int i=0;i<ReadInpData.TmpSectionInps.size();i++){//
        QString str=ReadInpData.TmpSectionInps.at(i).strelset;
        int id = m_comboxElset[i]->findText(str);
        m_comboxElset[i]->setCurrentIndex(id);
    }
    QString EType1=ReadInpData.TmpElInpS.strELType.mid(0,2);
	QString EType2=ReadInpData.TmpElInpS.strELType.mid(0,3);
	QString EType3,EType4;
	if(ReadInpData.TmpElInpS.ElementType.size()>0){
		EType3=ReadInpData.TmpElInpS.ElementType.at(0).mid(0,2);
		EType4=ReadInpData.TmpElInpS.ElementType.at(0).mid(0,3);
	}
	if(EType1=="c3"||EType3=="c3")D3Model->setChecked(true);
	else if(EType1=="ca"||EType3=="ca")AxialSymmetry->setChecked(true);
	else if(EType2=="cps"||EType4=="cps")PlaneStress->setChecked(true);
	else if(EType2=="cpe"||EType4=="cpe")PlaneStrain->setChecked(true);
    writeUnitPSetToInp(ReadInpData);//将Inp中读取到的节点、单元、集合等写入到当前的INP中
    WritePartDataToInp();
}

void QHPPartDlg::ShowPartDlgStyle(int style)
{
    QStringList strList;
	strList.clear();
	strList<<"部件"<<"部件类型"<<"材料"<<"单元集合"<<"TTT/CCT";
	//strList<<"部件"<<"导入部件"<<"部件类型"<<"材料"<<"单元集合"<<"TTT/CCT";
	m_TableWiget->setHorizontalHeaderLabels(strList); 
	int tt=m_TableWiget->columnCount();
	int nRow = m_TableWiget->rowCount();
	if (style == 0){//铸造 ;
		m_TableWiget->setColumnHidden(1,false);
		m_TableWiget->setColumnHidden(2,false);
		m_TableWiget->setColumnHidden(3,false);
		m_TableWiget->setColumnHidden(4,true);
		if (nRow == 1){
			m_TableWiget->item(0,0)->setText(tr("Part1"));
			m_TableWiget->setRowHidden(0,false);
		}else if (nRow ==2){
			m_TableWiget->item(0,0)->setText(tr("Part1"));
			m_TableWiget->item(1,0)->setText(tr("Part2"));
			m_TableWiget->setRowHidden(0,false);
			m_TableWiget->setRowHidden(1,false);
		}
	}else if (style == 1){//热等静压 ;
		m_TableWiget->setColumnHidden(1,true);
		m_TableWiget->setColumnHidden(2,false);
		m_TableWiget->setColumnHidden(3,false);
		m_TableWiget->setColumnHidden(4,true);
		if (nRow == 1){
			m_TableWiget->item(0,0)->setText(tr("包套"));
			m_TableWiget->setRowHidden(0,false);
		}else if (nRow ==2){
			m_TableWiget->item(0,0)->setText(tr("包套"));
			m_TableWiget->item(1,0)->setText(tr("金属粉末"));
			m_TableWiget->setRowHidden(0,false);
			m_TableWiget->setRowHidden(1,false);
		} else if (nRow ==3){
			m_TableWiget->item(0,0)->setText(tr("包套"));
			m_TableWiget->item(1,0)->setText(tr("金属粉末"));
			m_TableWiget->setRowHidden(0,false);
			m_TableWiget->setRowHidden(1,false);
			m_TableWiget->setRowHidden(2,true);
			m_TableWiget->removeRow(2);
		} 
	}else if(style == 2){//锻造
		m_TableWiget->setColumnHidden(1,false);
		m_TableWiget->setColumnHidden(2,false);
		m_TableWiget->setColumnHidden(3,false);
		m_TableWiget->setColumnHidden(4,true);
		if (nRow == 1){
			m_TableWiget->item(0,0)->setText(tr("模具1"));
			m_TableWiget->setRowHidden(0,false);
		}else if (nRow ==2){
			m_TableWiget->item(0,0)->setText(tr("模具1"));
			m_TableWiget->item(1,0)->setText(tr("锻造件"));
			m_TableWiget->setRowHidden(0,false);
			m_TableWiget->setRowHidden(1,false);
		}else if (nRow ==3){
			m_TableWiget->item(0,0)->setText(tr("模具1"));
			m_TableWiget->item(1,0)->setText(tr("锻造件"));
			m_TableWiget->item(2,0)->setText(tr("模具2"));
			m_TableWiget->setRowHidden(0,false);
			m_TableWiget->setRowHidden(1,false);
			m_TableWiget->setRowHidden(2,false);
		}
	}else if(style == 3){//热处理
		m_TableWiget->setColumnHidden(1,true);
		m_TableWiget->setColumnHidden(2,false);
		m_TableWiget->setColumnHidden(3,false);
		m_TableWiget->setColumnHidden(4,true);
		if (nRow == 1){
			m_TableWiget->item(0,0)->setText(tr("Part1"));
			m_TableWiget->setRowHidden(0,false);
		}else if (nRow ==2){
			m_TableWiget->item(0,0)->setText(tr("Part1"));
			m_TableWiget->setRowHidden(0,false);
			m_TableWiget->setRowHidden(1,true);
			m_TableWiget->removeRow(1);
		}else if (nRow ==3){
			m_TableWiget->item(0,0)->setText(tr("Part1"));
			m_TableWiget->setRowHidden(0,false);
			m_TableWiget->setRowHidden(1,true);
			m_TableWiget->setRowHidden(2,true);
			m_TableWiget->removeRow(2);
			m_TableWiget->removeRow(1);
		}
	}
}
//--新增按钮
void QHPPartDlg::SHPPropSlot()
{ 
    int j=0;SectionS  TmpSectionPara;
	QStringList MaterList;
    if( m_SectionManageDlg->BtnCreateSlot())//section创建成功则进行Table的显示
    {
        TmpSectionPara=m_SectionManageDlg->m_para;
        int RowCnt = m_TableWiget->rowCount();
        m_TableWiget->setRowCount(RowCnt+1);
        m_TableWiget->setItem(RowCnt,j,new QTableWidgetItem());
        m_TableWiget->item(RowCnt,j)->setText(TmpSectionPara.Name);

        MyCombox *tmpMCombox = new MyCombox(this,RowCnt);//QComboBox();
		if(RowCnt==0){tmpMCombox->addItems(TmpSectionPara.MaterPara.MaterList);}
		else{
			MyCombox *Combox=m_comboxMaterial.value(RowCnt-1);
			int wiic=Combox->count();
			QStringList vsv;
			vsv.clear();
			for(int i=0;i<wiic-1;i++){
				vsv<<Combox->itemText(i);
			}
			tmpMCombox->addItems(vsv);
		} 
        if (-1==tmpMCombox->findText("新建")){
           tmpMCombox->addItem("新建");
        }
        m_TableWiget->setCellWidget(RowCnt,ENUM_PART_Material,tmpMCombox);
        tmpMCombox->setCurrentIndex(TmpSectionPara.MaterPara.MaterListIndex);
        m_comboxMaterial.insert(RowCnt,tmpMCombox);
        connect(tmpMCombox,SIGNAL(clicked(int,int)),this,SLOT(comBoxMChgSlot()));
        connect(tmpMCombox,SIGNAL(doubleclicked(int,int)),this,SLOT(comBoxDoubleSlot()));
		/*QPushButton* m_ImprotPart = new QPushButton(tr("导入"));
		m_TableWiget->setCellWidget(RowCnt,1,m_ImprotPart);
		connect(m_ImprotPart,SIGNAL(clicked()),this,SLOT(MergeSlot()));*/
        MyCombox *tmpTTTCombox=new MyCombox(this,RowCnt);
        tmpTTTCombox->addItem("TTT");
        tmpTTTCombox->addItem("CCT");
        tmpTTTCombox->setFrame(false);
        m_TableWiget->setCellWidget(RowCnt,ENUM_PART_TTTCCT,tmpTTTCombox);

        MyCombox *tmpCombox = new MyCombox(this,RowCnt);
        if (RowCnt ==0){
            //tmpCombox->addItem("Element Set1");
            //tmpCombox->addItem(tr("新建"));
			tmpCombox->addItems(elementList);
            tmpCombox->setFrame(false);
            m_comboxElset.insert(RowCnt,tmpCombox);
            connect(m_comboxElset[RowCnt],SIGNAL(clicked(int,int)),this,SLOT(ComboElSlot()));
        }else{
            QStringList comboxList;
            MyCombox *tmpCB=(MyCombox*)m_TableWiget->cellWidget(0,ENUM_PART_ESET);
            for (int i=0;i<tmpCB->count();i++){
                comboxList<<tmpCB->itemText(i);
            }
            tmpCombox->addItems(comboxList);
            tmpCombox->setFrame(false);
            connect(tmpCombox,SIGNAL(clicked(int,int)),this,SLOT(ComboElSlot()));
        }
        m_TableWiget->setCellWidget(RowCnt,ENUM_PART_ESET,tmpCombox);
        m_comboxElset.insert(RowCnt,tmpCombox);

		MyCombox *PartTypeCombox = new MyCombox(this,RowCnt);
        PartTypeCombox->addItem(tr("弹塑性"));
        PartTypeCombox->addItem(tr("弹性"));
	    PartTypeCombox->addItem(tr("刚体"));
        PartTypeCombox->setFrame(false);
        m_TableWiget->setCellWidget(RowCnt,ENUM_PART_Type,PartTypeCombox);
        m_comboxPartType.insert(RowCnt,tmpCombox);
        ////将TableWiget中的所有列表均进行更新
        int cntList=TmpSectionPara.MaterPara.MaterList.size();
        int NewIndex;
		for(int kk=0;kk<RowCnt;kk++){
            NewIndex=tmpMCombox->findText("新建");
            for (int tt=0;tt<cntList;tt++){
                if(-1==tmpMCombox->findText(m_MaterManagerDlg->GetInstance()->m_para.MaterList.at(tt))){
					QString wed;
					wed=m_MaterManagerDlg->GetInstance()->m_para.MaterList.at(tt);
					if(wed!="")tmpMCombox->insertItem(NewIndex,wed); 
                    NewIndex++;
                }
            }
        }
    }
}
//--Delete
void QHPPartDlg::DeletePropSlot()
{
    int curRow = m_TableWiget->currentRow();

    if (curRow!=-1){
        m_TableWiget->removeRow(curRow);
        //m_comboxElset.remove(curRow);
        //m_comboxMaterial.remove(curRow);
    }

}
//--材料comboBox
void QHPPartDlg::comBoxMChgSlot()
{
    
    MyCombox *btn=(MyCombox*) sender();
    int x=btn->mapToParent(QPoint(0,0)).x();
    int y=btn->mapToParent(QPoint(0,0)).y();
    QModelIndex index= m_TableWiget->indexAt(QPoint(x,y));

    int iRow=index.row();
    int iCol=index.column();
    if (iCol != ENUM_PART_Material) return;

    MyCombox *tmpBtn = (MyCombox*)m_TableWiget->cellWidget(iRow, iCol);
    int id=m_comboxMaterial.key(tmpBtn);
    if (id==-1)return;


    QString strIdName = m_comboxMaterial[id]->currentText();
    if (strIdName.toLower() == "新建")
    {
        if(m_MaterManagerDlg->BtnCreateSlot()){
            QStringList treeNameList = m_MaterManagerDlg->GetInstance()->m_MaterProDlg->GetInstance()->m_TreeModel->m_TreeMaterList.name;
            if (treeNameList.count()==0){
                treeNameList<<m_MaterManagerDlg->GetInstance()->m_MaterProDlg->GetInstance()->m_Name;
            }
            //MyCombox *tmpComboxBtn = new MyCombox(this,0);
            tmpBtn->clear();
            tmpBtn->addItems(treeNameList);
            tmpBtn->addItem("新建");
            m_comboxMaterial.insert(id,tmpBtn);
            QString tmpStr=m_MaterManagerDlg->GetInstance()->m_MaterProDlg->GetInstance()->m_NowShowFileName;
            if (tmpStr==""){
                tmpStr=m_MaterManagerDlg->GetInstance()->m_MaterProDlg->GetInstance()->m_Name;
            }
            int curretIndex = treeNameList.indexOf(tmpStr);
            if(curretIndex!=-1){//显示在当前tabwiget中
                m_comboxMaterial[id]->setCurrentIndex(curretIndex) ;
                m_comboxMaterial.insert(id,tmpBtn);
            } 

            for (int kk=0;kk<m_TableWiget->rowCount();kk++){
                if (kk !=id)
                {
                    MyCombox *tmpOtherBtn=(MyCombox*)m_TableWiget->cellWidget(kk, iCol);
                    QString CurrentStr=tmpOtherBtn->currentText();//m_comboxMaterial[id]->currentText();
                    tmpOtherBtn->clear();
                    tmpOtherBtn->addItems(treeNameList);
                    tmpOtherBtn->addItem("新建");
                    int CurrentIndex=tmpOtherBtn->findText(CurrentStr);
                    if(-1 != CurrentIndex){
                        tmpOtherBtn->setCurrentIndex(CurrentIndex) ;
                        //m_TableWiget->setCellWidget(iRow,ENUM_PART_Material,m_comboxMaterial[id]);
                        //((MyCombox*)m_TableWiget->cellWidget(iRow, ENUM_PART_Material))->setCurrentIndex(curretIndex);
                    } 
                    m_comboxMaterial.insert(kk,tmpOtherBtn);
                }
            }
        }//else{
        //    QStringList treeNameList = m_MaterManagerDlg->GetInstance()->m_MaterProDlg->GetInstance()->m_TreeModel->m_TreeMaterList.name;
        //    for (int kk=0;kk<m_TableWiget->rowCount();kk++){
        //        QString CurrentStr=m_comboxMaterial[id]->currentText();
        //        m_comboxMaterial[id]->clear();
        //        m_comboxMaterial[id]->addItems(treeNameList);
        //        m_comboxMaterial[id]->addItem("新建");
        //        int CurrentIndex=m_comboxMaterial[id]->findText(CurrentStr);
        //        if(-1 != CurrentIndex){
        //            m_comboxMaterial[id]->setCurrentIndex(CurrentIndex) ;
        //            //m_TableWiget->setCellWidget(iRow,ENUM_PART_Material,m_comboxMaterial[id]);
        //            //((MyCombox*)m_TableWiget->cellWidget(iRow, ENUM_PART_Material))->setCurrentIndex(CurrentIndex);
        //        }
        //    }
        //}
    }//if (strIdName == "新建")
}

void QHPPartDlg::comBoxDoubleSlot()
{
  /*  QMessageBox::warning(this, tr("Dock Widgets"),
        tr("Cannot write file %1:\n%2."));
     m_combox1[comWigId]->hidePopup();*/

    MyCombox *btn=(MyCombox*) sender();
    int x=btn->mapToParent(QPoint(0,0)).x();
    int y=btn->mapToParent(QPoint(0,0)).y();
    QModelIndex index= m_TableWiget->indexAt(QPoint(x,y));

    int iRow=index.row();
    int iCol=index.column();
    if (iCol != ENUM_PART_Material) return;
    MyCombox *tmpBtn = (MyCombox*)m_TableWiget->cellWidget(iRow, iCol);
    int id=m_comboxMaterial.key(tmpBtn);
    if (id==-1)return;


    int currentId = m_comboxMaterial[iRow]->currentIndex();
    QString currentStr = m_comboxMaterial[iRow]->currentText();
    m_MaterManagerDlg->GetInstance()->m_MaterProDlg->GetInstance()->m_NameEdit->setText(currentStr);
   // m_MaterManagerDlg->GetInstance()->EditMaterial(currentId);
	if (m_MaterManagerDlg->GetInstance()->EditMaterial(currentId)){ //okBtnClick
		m_comboxMaterial[iRow]->setItemText(iRow,m_MaterManagerDlg->GetInstance()->m_MaterProDlg->GetInstance()->m_NameEdit->text());
        m_comboxMaterial.insert(iRow,m_comboxMaterial[iRow]);
	}
}

//-emit hightlight
void QHPPartDlg::ComboElSlot()
{

    MyCombox *btn=(MyCombox*) sender();
    int x=btn->mapToParent(QPoint(0,0)).x();
    int y=btn->mapToParent(QPoint(0,0)).y();
    QModelIndex index= m_TableWiget->indexAt(QPoint(x,y));

    int iRow=index.row();
    int iCol=index.column();
    if (iCol != ENUM_PART_ESET) return;

    MyCombox *tmpBtn = (MyCombox*)m_TableWiget->cellWidget(iRow, iCol);
    int id=m_comboxElset.key(tmpBtn);
    if (id==-1)return;
    int ind= m_comboxElset[id]->currentIndex();
    QString str =m_comboxElset[id]->currentText();
    int iTmp = m_comboxElset[id]->itemData(ind,Qt::UserRole+3).toInt();
    //--设置属性
    NElSurfChsS tmpChs;
    tmpChs.strName =str;
    tmpChs.iType=COM_ENUM_ELSET;
    emit emitElsetHighLight(tmpChs);
}
//--将part部分写入到inp中
void QHPPartDlg::WritePartDataToInp()
{
    SectionInpS nodeSList;
    MaterialInpS nodeMList;
    int TabRowCnt=m_TableWiget->rowCount();
    int TabColCnt=m_TableWiget->columnCount();
    if (TabRowCnt>0)m_nodeSList.clear();
	else return;
    for(int i=0; i<TabRowCnt; i++){
        nodeSList.strCommentaryName="";//m_TableWiget->item(i,0)->text();
        nodeSList.strSectionStyle="Solid Section";
		nodeSList.strPartType=((MyCombox*)m_TableWiget->cellWidget(i,ENUM_PART_Type))->currentText();
        nodeSList.StrMaterial=((MyCombox*)m_TableWiget->cellWidget(i,ENUM_PART_Material))->currentText();//m_comboxMaterial[i]->currentText();
        nodeSList.strelset=((MyCombox*)m_TableWiget->cellWidget(i,ENUM_PART_ESET))->currentText();
        if(nodeSList.StrMaterial!="新建")m_nodeSList.append(nodeSList);
    }
    QMap<QString,QMap<int ,MCompoment> > MFileData;
    MFileData = m_MaterManagerDlg->GetInstance()->m_MaterProDlg->GetInstance()->m_MaterDataWig->MFileData;

    //MFileData = m_MaterManagerDlg->m_MaterProListDlg.at(0)->m_MaterDataWig->MFileData;
    QMap<QString,QMap<int ,MCompoment> >::Iterator iitt=MFileData.begin();
    QMap<int ,MCompoment> MData;
    m_nodeMList.clear();
    nodeMList.strCommentary="MATERIALS";
    for(iitt; iitt != MFileData.end(); iitt++)
    {
        for (int tt=0;tt<20;tt++){nodeMList.strData[tt].clear();}
        MData = iitt.value();
        if (MData.count()==0)
            continue;
        QMap<int, MCompoment>::Iterator it = MData.begin();

        QString str = it->strStyle;
        QStringList strListR,strListX,strListXPower,strStyleParamList,strStyleParamList2;//strListD[10];
        nodeMList.strMaterialName = it->FileName;
        int dataNum=0;
        QStringList *strListD=NULL;
        for (it; it != MData.end(); it++){
            strListR<<it->strStyle;
            strStyleParamList<<it->strStyleParam;
			QString Str=it->strStyleParam1;
			if(Str!="")strStyleParamList2<<Str;//
            if (it->PageIndex!=M_ENUM_METALPOWDER)
            {
                int dataNCol = it->dataColNum;
                int dataNRow = it->dataRowNum;

                for (int kk=0; kk<dataNCol; kk++){
                    strListX<<it->data.at(kk).strName;
                }
                strListD = new QStringList[dataNRow];
                for (int kk=0;kk<dataNRow;kk++){
                    for (int jj=0;jj<dataNCol;jj++){
                        strListD[kk]<<it->data.at(kk*dataNCol+jj).Data;
                    }  
                    nodeMList.strData[dataNum].push_back(strListD);
                }
            }else{//金属粉末特性
                for (int kk=0;kk<it->dataRowNum;kk++){
                    strListXPower<<it->data.at(2*kk+1).Data; 
                }
                int dataNCol = it->dataColNum2;
                int dataNRow = it->dataRowNum2;
                int baseN=(it->dataColNum)*(it->dataRowNum);
                strListD = new QStringList[dataNRow];
                for (int kk=0;kk<(dataNRow);kk++){
                    for (int jj=0;jj<dataNCol;jj++){
                        strListD[kk]<<it->data.at(baseN+kk*dataNCol+jj).Data;
                    }  
                    nodeMList.strData[dataNum].push_back(strListD);
                }
            }

            dataNum++;
            strListD=NULL;
            delete (strListD);
            nodeMList.iPageIndex=it->PageIndex;
        }
        nodeMList.strMetalPowerData=strListXPower;
        nodeMList.DateVectorNum = dataNum;
        nodeMList.strMaterialStyleName =strListR;
        nodeMList.strMaterialStyleParam=strStyleParamList;
		nodeMList.strMaterialStyleParam2=strStyleParamList2;
        m_nodeMList.append(nodeMList);
    }
    //-将TTT/CCT中的内容，写入到材料库中
    //m_HPTTTDlg->GetInstance(this)->WriteHPTTTToInp();
    bool bTTT=false; bool bCCT=false;
    for (int i=0;i<TabRowCnt;i++){//
        MyCombox *tmpCombox=(MyCombox *)m_TableWiget->cellWidget(i,ENUM_PART_TTTCCT);
        if (tmpCombox!=NULL){
            QString str =tmpCombox->currentText();
            if (str == "TTT" && bTTT == false){
                bTTT=true;
                m_HpTTTDataInps=m_MaterManagerDlg->GetInstance()->m_MaterProDlg->GetInstance()->m_WidTTT->WriteHPTTTToInp();
				//m_HpTTTDataInps=m_MaterManagerDlg->GetInstance()->m_MaterNewDlg->m_WindTTT->WriteHPTTTToInp();
				//m_HpTTTDataInps=m_MaterManagerDlg->m_MaterNewDlg->m_WindTTT->WriteHPTTTToInp();
            }else if (str == "CCT" && bCCT == false){
                bCCT=true;
                m_HpTTTDataInps=m_MaterManagerDlg->GetInstance()->m_MaterProDlg->GetInstance()->m_WidCCT->WriteHPCCToInp();
            }
        }
    }
    if (!m_nodeMList.size()){
        //nodeMList.strMetalPowerData=strListXPower;
        //nodeMList.DateVectorNum = dataNum;
        nodeMList.strMaterialName =((MyCombox*)m_TableWiget->cellWidget(0,ENUM_PART_Material))->currentText();//m_comboxMaterial[0]->currentText();
        if(nodeMList.strMaterialName!="新建")m_nodeMList.append(nodeMList);
    }
}

void QHPPartDlg::writeUnitPSetToInp(ReadInpResultS ReadInpData)
{
    m_NodeElSetData.NodeInpData = ReadInpData.TmpNodeInpS;
    m_NodeElSetData.ELInpData = ReadInpData.TmpElInpS;
    m_NodeElSetData.NSetInpData=ReadInpData.TmpNsetInps;
    m_NodeElSetData.ElSetInpData=ReadInpData.TmpElSetInps;
    m_NodeElSetData.SurfaceInpData=ReadInpData.TmpSurfaceInps;
}

void QHPPartDlg::ApplayPropSlot()
{
    //--
    QString info =(tr("部件数据已添加到Inp缓存中"));
    Information_Widget::GetInstance()->ShowInformation(QString(info));
    WritePartDataToInp();
}
void QHPPartDlg::NextBtnSlot()
{
    ApplayPropSlot();
    this->accept();
}
void QHPPartDlg::MergeSlot()
{
	emit emitMerge();
}
//--
void QHPPartDlg::AppendElSetComboxSlot(QString str)
{
    int nRow =m_TableWiget->rowCount();
    
    for (int i=0;i<nRow;i++){
        MyCombox *TmpcomboxPset = (MyCombox*)m_TableWiget->cellWidget(i, ENUM_PART_ESET);//new MyCombox(this, nRow);
        if(TmpcomboxPset->findText(str)==-1)
        {
             TmpcomboxPset->addItem(str);  
        }         
    }
}
//--Add data to 
void QHPPartDlg::AppendPElData(QString str, int iStyle, DecodeGenerateS data)
{
    //--单元
    switch (iStyle){
    case COM_ENUM_NSET:{
        int id=m_NodeElSetData.NSetInpData.strNSetName.indexOf(str);
        if (id == -1){
            m_NodeElSetData.NSetInpData.strNSetName << str;
            //m_NodeElSetData.ElSetInpData.strElSetStyle=data.iDataGenerate;
            QString strT=data.strDataList.join(",");
            QStringList strTL;strTL<<strT;
            m_NodeElSetData.NSetInpData.strData<<strTL;
            strT.clear();
            m_NodeElSetData.NSetInpData.strNSetStyle<<strT;     
        }else{
            QString strT=data.strDataList.join(",");
            QStringList strTL;strTL<<strT;
            m_NodeElSetData.NSetInpData.strData[id] = strTL;
            m_NodeElSetData.NSetInpData.strNSetStyle[id]=""; 
        }                      
       }break;
    case COM_ENUM_ELSET:{
        int id=m_NodeElSetData.ElSetInpData.strElSetName.indexOf(str);
        if (id == -1){
            m_NodeElSetData.ElSetInpData.strElSetName << str;
            //m_NodeElSetData.ElSetInpData.strElSetStyle=data.iDataGenerate;
             QString strT=data.strDataList.join(",");
            m_NodeElSetData.ElSetInpData.strData << strT;
            m_NodeElSetData.ElSetInpData.strElSetStyle<<"";     
        }else{
            QString strT=data.strDataList.join(",");
            m_NodeElSetData.ElSetInpData.strData.replace(id, strT);
            m_NodeElSetData.ElSetInpData.strElSetStyle.replace(id,""); 
        }                      
       }break;
    case COM_ENUM_SURFSET:{
        int id=m_NodeElSetData.SurfaceInpData.strSurfaceName.indexOf(str);
        if (id == -1){
            m_NodeElSetData.SurfaceInpData.strSurfaceName<<str;
            int iSize=data.strDataList.size();
            m_NodeElSetData.SurfaceInpData.strSurfaceDataNum<<QString("%1").arg(iSize);
            for (int kk=0;kk<iSize;kk++){
                m_NodeElSetData.SurfaceInpData.strData<<data.strDataList.at(kk);
            }   
        }else{
            int startId=0,endId=0;
            for(int kk=0; kk<id; kk++){
                startId+=m_NodeElSetData.SurfaceInpData.strSurfaceDataNum.at(kk).toInt();
            }
            endId=m_NodeElSetData.SurfaceInpData.strSurfaceDataNum.at(id).toInt();
            for (int kk=startId;kk<endId;kk++)
            {
                 m_NodeElSetData.SurfaceInpData.strData.removeAt(kk);
            }
            int iSize=data.strDataList.size();
            m_NodeElSetData.SurfaceInpData.strSurfaceDataNum[id]=QString("%1").arg(iSize);
            for (int kk=startId;kk<iSize;kk++){
                m_NodeElSetData.SurfaceInpData.strData.insert(kk,data.strDataList.at(kk));
            }
         }
      }break;
    default:break;
    }
	//--节点 
}
void QHPPartDlg::AMOptions(bool BL)
{
	QRadioButton *RBtn=qobject_cast<QRadioButton*>(sender());//获取发送信号的单选框
	QString Dimension=m_NodeElSetData.ELInpData.strELType.mid(0,2);
	int DM,Nsize;
	if(Dimension=="C3")DM=3;
	else DM=2;
	if(ENumberS==0&&BL==true){
        EleTy<<m_NodeElSetData.ELInpData.ElementType;
		ENumberS++;
	}

	if(RBtn==D3Model&&BL==true&&DM==3);
	else if(RBtn==AxialSymmetry&&BL==true&&DM==2){
		Nsize=m_NodeElSetData.ELInpData.ElementType.size();
		for(int i=0;i<Nsize;i++){
			QString BC=m_NodeElSetData.ELInpData.ElementType.at(i);
			BC=BC.replace(0,3,"CAX");
			m_NodeElSetData.ELInpData.ElementType.replace(i,BC);
		}
	}else if(RBtn==PlaneStress&&BL==true&&DM==2){
		Nsize=m_NodeElSetData.ELInpData.ElementType.size();
		for(int i=0;i<Nsize;i++){
			QString BC=m_NodeElSetData.ELInpData.ElementType.at(i);
			BC=BC.replace(0,3,"CPS");
			m_NodeElSetData.ELInpData.ElementType.replace(i,BC);
		}
	}else if(RBtn==PlaneStrain&&BL==true&&DM==2){
		Nsize=m_NodeElSetData.ELInpData.ElementType.size();
		for(int i=0;i<Nsize;i++){
			QString BC=m_NodeElSetData.ELInpData.ElementType.at(i);
			BC=BC.replace(0,3,"CPE");
			m_NodeElSetData.ELInpData.ElementType.replace(i,BC);
		}
	}
}
