#include "QHPSolveSetDlg.h"
#include <QHeaderView>

QHPSolveSetDlg::QHPSolveSetDlg(QWidget *parent)
    : QDialog(parent)
{
      CreatGUI();
}

QHPSolveSetDlg::~QHPSolveSetDlg()
{

}

void QHPSolveSetDlg::CreatGUI()
{
    setWindowTitle(tr("���������"));
    m_Tab1=new QWidget();//(tr("ddd1"));
    //m_checkBoxHostStress = new QCheckBox(tr("��Ӧ��"),m_Tab1);
    //m_checkBoxNodeTemper = new QCheckBox(tr("�ڵ��¶�"),m_Tab1);
    //m_checkBoxCrystalSize = new QCheckBox(tr("�����ߴ�"),m_Tab1);
    //m_checkBoxEquiStress = new QCheckBox(tr("��ЧӦ��"),m_Tab1);
    //m_checkBoxTemperGrads = new QCheckBox(tr("�¶��ݶ�"),m_Tab1);
    //m_checkBoxBendIntensity = new QCheckBox(tr("����ǿ��"),m_Tab1);
    //m_checkBoxPlasticStrain = new QCheckBox(tr("����Ӧ��"),m_Tab1);
    //m_checkBoxHeatFlux = new QCheckBox(tr("�����ܶ�"),m_Tab1);
    //m_checkBoxVOF = new QCheckBox(tr("�������"),m_Tab1);
    //m_checkBoxEquiStrain = new QCheckBox(tr("��ЧӦ��"),m_Tab1);
    //QGridLayout *gridLayout = new QGridLayout();
    //gridLayout->addWidget(m_checkBoxHostStress,0,0,1,1);
    //gridLayout->addWidget(m_checkBoxNodeTemper,0,1,1,1);
    //gridLayout->addWidget(m_checkBoxCrystalSize,0,2,1,1);

    //gridLayout->addWidget(m_checkBoxEquiStress,1,0,1,1);
    //gridLayout->addWidget(m_checkBoxTemperGrads,1,1,1,1);
    //gridLayout->addWidget(m_checkBoxBendIntensity,1,2,1,1);

    //gridLayout->addWidget(m_checkBoxPlasticStrain,2,0,1,1);
    //gridLayout->addWidget(m_checkBoxHeatFlux,2,1,1,1);
    //gridLayout->addWidget(m_checkBoxVOF,2,2,1,1);

    //gridLayout->addWidget(m_checkBoxEquiStrain,3,0,1,1);
	IJKL=0;

    QGroupBox *gbGroup = new QGroupBox(tr("Choose"),m_Tab1);
    gbGroup->setStyleSheet(QString::fromUtf8("::title{color:blue}"));
    
    QHBoxLayout *Hlayout = new QHBoxLayout();
    QHBoxLayout *Hlayout3 = new QHBoxLayout();
	QVBoxLayout *Vlayout = new QVBoxLayout();
	QVBoxLayout *Vlayout1 = new QVBoxLayout();

    QGridLayout *gGridlayout1 = new QGridLayout(gbGroup);
    //QHBoxLayout *gMainLayout = new QHBoxLayout(gbGroup);
    
	m_CheckBox[0]= new QCheckBox(tr("Ӧ��"),gbGroup);
	m_CheckBox[1]= new QCheckBox(tr("λ��"),gbGroup);
	m_CheckBox[2]= new QCheckBox(tr("�ٽᾧ"),gbGroup);
	m_CheckBox[3]= new QCheckBox(tr("Ӧ��"),gbGroup);
	m_CheckBox[4]= new QCheckBox(tr("���"),gbGroup);
	m_CheckBox[5]= new QCheckBox(tr("��Ч����Ӧ��"),gbGroup);
	m_CheckBox[6]= new QCheckBox(tr("�¶�"),gbGroup);
	m_CheckBox[7]= new QCheckBox(tr("����Ӧ��"),gbGroup);
	m_CheckBox[8]= new QCheckBox(tr("����"),gbGroup);


    m_AllCheckBox = new QCheckBox(tr("ȫѡ"),gbGroup);
    //Ӧ����Ӧ�䡢λ�ơ��¶�ΪĬ�����
	m_CheckBox[0]->setChecked(true);
	m_CheckBox[1]->setChecked(true);
	m_CheckBox[3]->setChecked(true);
	m_CheckBox[6]->setChecked(true);
   
    for (int kk=0;kk<9 ;kk++){
        gGridlayout1->addWidget(m_CheckBox[kk]);
    }
    m_OutputDatCheckBox = new QCheckBox(tr("���Dat�ļ�"));
	m_ExternalForcesCheckBox=new QCheckBox(tr("�������"));
	m_DatCombox= new QComboBox();
	m_NodeSetName=new QLineEdit(tr(""));
	m_AddBtn=new QPushButton(tr("����"));
	m_DeletBtn=new QPushButton(tr("ɾ��"));

	QButtonGroup *groupSurfBtn=new QButtonGroup();
	groupSurfBtn->addButton(m_AddBtn,0);
	groupSurfBtn->addButton(m_DeletBtn,1);


    m_Combox= new QComboBox(gbGroup);
    m_Combox->addItem("��ʱ�������");
    m_Combox->addItem("������Ƶ�����"); 
    gGridlayout1->addWidget(m_AllCheckBox,0,1,1,1);
    //m_OKBtn = new QPushButton(tr("ȷ��"));
    //m_DefaultBtn = new QPushButton(tr("Ĭ��"));
    //m_CancelBtn = new QPushButton(tr("ɾ��"));
    QHBoxLayout *Hlayout2 = new QHBoxLayout();
    m_TimeEdit = new QLineEdit(tr("10"));
    m_TimeFreqStr<<tr("���ʱ����(S):")<<tr("���Ƶ�ʼ��(HZ):");
    m_timLabel = new QLabel(m_TimeFreqStr.at(0));
    //m_fLabel = new QLabel(tr("���Ƶ�ʼ��(HZ):"));
    /*m_ComboxTime = new QComboBox();
    m_ComboxTime->addItem("10s");
    m_ComboxTime->addItem("20s");
    m_ComboxTime->addItem("30s");
    m_ComboxTime->addItem("40s");*/
    Hlayout2->addWidget(m_timLabel);
    Hlayout2->addWidget(m_TimeEdit);
	//Hlayout2->addWidget(m_TimeEdit);
    
    Vlayout->addWidget(gbGroup);
    //Vlayout->addWidget(m_OutputDatCheckBox);
	Vlayout1->addWidget(m_Combox);

     //Hlayout3->addWidget(m_OutputDatCheckBox);
	 //Hlayout3->addStretch();
	 Hlayout3->addWidget(m_ExternalForcesCheckBox);
	 Hlayout3->addWidget(m_DatCombox);
	 Hlayout3->addWidget(m_NodeSetName);
	 Hlayout3->addWidget(m_AddBtn);
	 Hlayout3->addWidget(m_DeletBtn);
	 Hlayout3->addStretch();
   // Hlayout->addWidget(m_OKBtn);
   // Hlayout->addWidget(m_DefaultBtn);
    //Hlayout->addWidget(m_CancelBtn); 
    //Hlayout->addWidget(m_SectionBtn);
    //Hlayout->addWidget(m_AlloyBtn);
    QVBoxLayout *MainLayout = new QVBoxLayout();
    MainLayout->addLayout(Vlayout);
	MainLayout->addLayout(Hlayout3);
	MainLayout->addLayout(Vlayout1);
    //MainLayout->addLayout(Hlayout2);
    MainLayout->addLayout(Hlayout);
    m_Tab1->setLayout(MainLayout);
    //--Tab2
    m_Tab2=new QWidget();//(tr("ddd2"))
    QStringList strList;
    strList<<tr("�ƶ�")<<tr("ģ����ʱ��")<<tr("��ʼ����")<<tr("��С����")<<tr("��󲽳�")<<tr("�����������")<<tr("����¶�����")<<tr("����㷨")<<tr("���������");
    m_TableWig = new QTableWidget(m_Tab2);
    m_TableWig->setColumnCount(strList.size());
    m_TableWig->setRowCount(1);
    m_TableWig->verticalHeaderItem(1);
    
    m_TableWig->setHorizontalHeaderLabels(strList);
    for (int j=0;j<strList.size();j++){
        m_TableWig->horizontalHeader()->setResizeMode(j,QHeaderView::ResizeToContents);
    }
	m_TableWig->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	for(int i=0;i<5;i++){
		m_TableWig->horizontalHeader()->setResizeMode(i, QHeaderView::Interactive);
	}

    QTableWidgetItem *tmpItem1=new QTableWidgetItem(tr("step1"));
    m_TableWig->setItem(0,0,tmpItem1);
    tmpItem1=new QTableWidgetItem(tr("10"));
    m_TableWig->setItem(0,1,tmpItem1);
    tmpItem1=new QTableWidgetItem(tr("1"));
    m_TableWig->setItem(0,2,tmpItem1);
    tmpItem1=new QTableWidgetItem(tr("1e-5"));
    m_TableWig->setItem(0,3,tmpItem1);
    tmpItem1=new QTableWidgetItem(tr("20"));
    m_TableWig->setItem(0,4,tmpItem1);
    tmpItem1=new QTableWidgetItem(tr("1000000"));
    m_TableWig->setItem(0,5,tmpItem1);
    tmpItem1=new QTableWidgetItem(tr("50"));
    m_TableWig->setItem(0,6,tmpItem1);
	tmpItem1=new QTableWidgetItem("1");
    m_TableWig->setItem(0,8,tmpItem1);

    m_comboxCoupled = new QComboBox();
    m_comboxCoupled->addItem("��ȫ���");
    m_comboxCoupled->addItem("����ȫ���");
	m_comboxCoupled->addItem("��ʾ���");
    m_comboxCoupled->setFrame(false);
    m_TableWig->setCellWidget(0,7,m_comboxCoupled);
	connect(m_comboxCoupled,SIGNAL(activated(int)),this,SLOT(ShowAndHideSlot(int)));

    QLabel *lab1=new QLabel(tr("�������:"),m_Tab2);
    QLabel *lab2=new QLabel(tr("˹߯��-������������:"),m_Tab2);
	QLabel *lab3=new QLabel(tr("��������ϵ��:"),m_Tab2);
    m_EditAbsZero=new QLineEdit(tr("-273.15"),m_Tab2);
    m_EditStefanBoltzman=new QLineEdit(tr("5.6704e-08"),m_Tab2);
	MassScalingFactor=new QLineEdit(tr("1.0"),m_Tab2);
	MassScalingFactor->setEnabled(false);

    //m_Tab2AddBtn=new QPushButton(tr("����"),m_Tab2);
    //m_Tab2DelBtn=new QPushButton(tr("ɾ��"),m_Tab2);
    //m_Tab2InsertBtn=new QPushButton(tr("����"),m_Tab2);

    QGridLayout *gridLayoutT2 = new QGridLayout();
    gridLayoutT2->addWidget(m_TableWig,0,0,1,4);
    gridLayoutT2->addWidget(lab1,1,0,1,1);
    gridLayoutT2->addWidget(lab2,1,2,1,1);
	gridLayoutT2->addWidget(lab3,2,0,1,1);

    gridLayoutT2->addWidget(m_EditAbsZero,1,1,1,1);
    gridLayoutT2->addWidget(m_EditStefanBoltzman,1,3,1,1);
	gridLayoutT2->addWidget(MassScalingFactor,2,1,1,1);

    m_Tab2->setLayout(gridLayoutT2);

    m_tabView = new QTabWidget();
    m_tabView->addTab(m_Tab1, tr("�������"));
    m_tabView->addTab(m_Tab2, tr("�������"));

    m_NextBtn = new QPushButton(tr("��һ��"));
    m_PreBtn = new QPushButton(tr("��һ��"));
    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->addWidget(m_tabView,0,0,2,1);
    mainLayout->addWidget(m_PreBtn,0,1,1,1);
    mainLayout->addWidget(m_NextBtn,1,1,1,1);
    this->setLayout(mainLayout);
	this->setMinimumWidth(1150);
    connect(m_NextBtn,SIGNAL(clicked()),this,SLOT(ApplayBtnSlot()));
    connect(m_AllCheckBox,SIGNAL(clicked()),this,SLOT(AllChooseSlot()));
    connect(m_Combox,SIGNAL(currentIndexChanged(int)),this,SLOT(ChangeTimeFreqSlot(int)));
    connect(m_PreBtn,SIGNAL(clicked()),this,SLOT(PreBtnSlot()));
	connect(groupSurfBtn,SIGNAL(buttonClicked(int)),this,SLOT(AddDeletSolt(int)));
	connect(m_comboxCoupled,SIGNAL(activated(int)),this,SLOT(MCFSlot(int)));
}
void QHPSolveSetDlg::AddDeletSolt(int i)
{
	QString NodeSetName0;
	QString NodeSetName1="";
	NodeSetName0=m_DatCombox->currentText();
	NodeSetName1=m_NodeSetName->text();
	if(i==0){
		if(NodeSetName1.split(",",QString::SkipEmptyParts).contains(NodeSetName0,Qt::CaseInsensitive))return;

		if(NodeSetName1==""){
			NodeSetName1=NodeSetName0;
		}else{
			NodeSetName1+=","+NodeSetName0;
		}
		m_NodeSetName->clear();
		m_NodeSetName->setText(NodeSetName1);
	}else if(i==1){
		bool f=NodeSetName0.split(",",QString::SkipEmptyParts).contains(NodeSetName0,Qt::CaseInsensitive);

		if(NodeSetName1==""||!f)return;
		if(NodeSetName1.split(",",QString::SkipEmptyParts).size()!=1)NodeSetName0=","+NodeSetName0;
		NodeSetName1.remove(NodeSetName0,Qt::CaseInsensitive);
		m_NodeSetName->clear();
		m_NodeSetName->setText(NodeSetName1);
	}
}

void QHPSolveSetDlg::WriteSloveToInp()
{
//--
    m_nodeHPSolveList.clear();m_nodeHPPhyList.clear();//���¼���ʱ������������ж�����Ҫ���棬���ǲ����
    m_nodeVarList.clear();
    HPSolveInpS nodeHPList;
    int TabRowCnt=m_TableWig->rowCount();
    int TabColCnt=m_TableWig->columnCount();
    if (TabRowCnt>0){
        m_nodeHPSolveList.clear();
        nodeHPList.strStepInc=m_TableWig->item(0,5)->text();
    }
    
    //--coupled - uncoupled
    for (int i = 0; i<TabRowCnt; i++)
    {
        QComboBox *ff = (QComboBox *)m_TableWig->cellWidget(i,TabColCnt-2);
        if ( ff->currentIndex()==0){
            nodeHPList.strNlgeom = "YES";
            nodeHPList.strNlgeomSTR = "coupled Temperature-displacement";
        }else if ( ff->currentIndex()==1){
            nodeHPList.strNlgeom = "YES";
            nodeHPList.strNlgeomSTR = "uncoupled Temperature-displacement";
        }else if ( ff->currentIndex()==2){
            nodeHPList.strNlgeom = "YES";
            nodeHPList.strNlgeomSTR = "Dynamic,Direct,Explicit";
        }
        nodeHPList.strDelMX = m_TableWig->item(i,6)->text();
        nodeHPList.strInitStep = m_TableWig->item(i,2)->text();
        nodeHPList.strTotalTime =  m_TableWig->item(i,1)->text();
        nodeHPList.strMinStep = m_TableWig->item(i,3)->text();
        nodeHPList.strMaxStep = m_TableWig->item(i,4)->text();
		nodeHPList.TimeFrequencyInc=m_TableWig->item(i,8)->text();
        m_nodeHPSolveList.append(nodeHPList);

		VariableInpS nodeTmp;

		nodeTmp.strTname = m_TimeEdit->text();
		nodeTmp.bELFileShow=true;
		if(!m_OutputDatCheckBox->isChecked()){
			nodeTmp.strNodeFile = "NODE FILE";
			nodeTmp.strELFile = "EL FILE";
		}else{
			nodeTmp.strNodeFile = "NODE PRINT";
			nodeTmp.strELFile = "EL PRINT";
		}
		if(m_ExternalForcesCheckBox->isChecked()){
			QStringList SetName=m_NodeSetName->text().split(",",QString::SkipEmptyParts);
            nodeTmp.NodeSet1<<SetName;
			nodeTmp.NodeSet=m_DatCombox->currentText();
			nodeTmp.Igforce = "1";
		}	

		if (m_timLabel->text()=="���ʱ����(S):"){
			nodeTmp.strTimeOrFreqName = "TIME POINTS";
		}else if(m_timLabel->text()=="���Ƶ�ʼ��(HZ):"){
			nodeTmp.strTimeOrFreqName = "FREQUENCY";
		}
		if (m_CheckBox[0]->isChecked()){
			nodeTmp.strELInclude="S";
		}
		if (m_CheckBox[2]->isChecked()){
			nodeTmp.strELInclude=nodeTmp.strELInclude+",DRX";
		}
		if (m_CheckBox[3]->isChecked()){
			nodeTmp.strELInclude=nodeTmp.strELInclude+",E";
		}
		if (m_CheckBox[4]->isChecked()){
			nodeTmp.strELInclude=nodeTmp.strELInclude+",PHAS";
		}
		if (m_CheckBox[5]->isChecked()){
			nodeTmp.strELInclude=nodeTmp.strELInclude+",PEEQ";
		}
		if (m_CheckBox[7]->isChecked()){
			nodeTmp.strELInclude=nodeTmp.strELInclude+",RPD";
		}
		if (m_CheckBox[8]->isChecked()){
			nodeTmp.strELInclude=nodeTmp.strELInclude+",DMG";
		}

		if (m_CheckBox[1]->isChecked()){
			nodeTmp.strNInclude="U";
		}
		if (m_CheckBox[6]->isChecked()){
			nodeTmp.strNInclude=nodeTmp.strNInclude+",NT";
		}

		//--SDV :*EL FILE
		/*if (m_CheckBox[2]->isChecked()||m_CheckBox[4]->isChecked()||m_CheckBox[5]->isChecked()){
			nodeTmp.strELInclude=nodeTmp.strELInclude+",SDV";
		}*/
		if ((m_CheckBox[1]->isChecked()||m_CheckBox[6]->isChecked())&&
			!(m_CheckBox[0]->isChecked())&& !(m_CheckBox[2]->isChecked())&&
			!(m_CheckBox[3]->isChecked())&& !(m_CheckBox[4]->isChecked())&&
			!(m_CheckBox[5]->isChecked())&&!(m_CheckBox[7]->isChecked())&&
			!(m_CheckBox[8]->isChecked())){//2021.2.25��ֻ���¶�ORλ������һ������ʱ��EL�ؼ��ֲ���ʾ
				nodeTmp.bELFileShow =false;
		}
		m_nodeVarList.append(nodeTmp);
	} 
    HPPhyInpS NodePhyList;
    NodePhyList.strCommentaryName="��������";
    NodePhyList.strStyle="Physical Constants";
    NodePhyList.strAbsZero=m_EditAbsZero->text();
    NodePhyList.strStefanBoltzman=m_EditStefanBoltzman->text();
	if(MassScalingFactor->text()!="1"&&IJKL==1)NodePhyList.strMassScalingFactor=MassScalingFactor->text();
    m_nodeHPPhyList.append(NodePhyList);
//***********************************************************
}

void QHPSolveSetDlg::ApplayBtnSlot()
{
    WriteSloveToInp();
    this->accept();
}
void QHPSolveSetDlg::PreBtnSlot()
{
    this->accept();
}

void QHPSolveSetDlg::AllChooseSlot()
{
    for (int kk=0; kk<7 ;kk++){
        m_CheckBox[kk]->setChecked(m_AllCheckBox->isChecked());
    }
}

void QHPSolveSetDlg::ChangeTimeFreqSlot(int Index)
{
    m_timLabel->setText(m_TimeFreqStr.at(Index));
}
void QHPSolveSetDlg::SetInpData(ReadInpResultS ReadInpData)
{
    int NVar=ReadInpData.TmpVariableInps.size();
	m_PSetList=ReadInpData.TmpNsetInps.strNSetName;
	m_DatCombox->addItems(m_PSetList);
    if(ReadInpData.TmpVariableInps.size()<=0)return;	

    QString str=ReadInpData.TmpVariableInps.at(0).strTname;
    m_TimeEdit->setText(str);
    str =ReadInpData.TmpVariableInps.at(0).strTimeOrFreqName;

    if (str=="frequency"){
        m_timLabel->setText(m_TimeFreqStr.at(1));
        m_Combox->setCurrentIndex(1);
    }else{
        m_timLabel->setText(m_TimeFreqStr.at(0));
        m_Combox->setCurrentIndex(0);
    }
    QString setName0="";
	int KLIK=0;
	for(int j=0;j<NVar;j++){
		if( ReadInpData.TmpVariableInps.at(j).Igforce=="1"){
			m_ExternalForcesCheckBox->setChecked(true);
			if(KLIK==0){
				setName0=ReadInpData.TmpVariableInps.at(j).NodeSet;
			}else if(KLIK!=0&&ReadInpData.TmpVariableInps.at(j).NodeSet!=""){
				setName0+=","+ReadInpData.TmpVariableInps.at(j).NodeSet;
			}
			//int id=m_DatCombox->findText(ReadInpData.TmpVariableInps.at(j).NodeSet);
			//m_DatCombox->setCurrentIndex(id);
			KLIK++;
		}
	}
	if(setName0!="")m_NodeSetName->setText(setName0);

    if(ReadInpData.TmpVariableInps.at(0).strELFile=="*elfile"||
        ReadInpData.TmpVariableInps.at(0).strNodeFile=="*nodefile"){ 
            m_OutputDatCheckBox->setChecked(false);
    }else{
        m_OutputDatCheckBox->setChecked(true);
    }
    QStringList strList;
	for(int JI=0;JI<NVar;JI++){
		QString Str=ReadInpData.TmpVariableInps.at(JI).strELInclude;
		if(Str!="")strList<<ReadInpData.TmpVariableInps.at(JI).strELInclude.split(",");//EL File
	}
    for (int i=0;i<strList.size();i++){
        str=strList.at(i);
        strList.replace(i,str.simplified());
    }

    if(strList.contains("s",Qt::CaseInsensitive)){
        m_CheckBox[0]->setChecked(true);
    }
	if(strList.contains("drx",Qt::CaseInsensitive)){
		m_CheckBox[2]->setChecked(true);
	}
    if(strList.contains("e",Qt::CaseInsensitive)){
        m_CheckBox[3]->setChecked(true);
    }
	if(strList.contains("phas",Qt::CaseInsensitive)){
		m_CheckBox[4]->setChecked(true);
	}
	if(strList.contains("peeq",Qt::CaseInsensitive)){
        m_CheckBox[5]->setChecked(true);
    }
	if(strList.contains("rdp",Qt::CaseInsensitive)){
		m_CheckBox[7]->setChecked(true);
	}
	if(strList.contains("dmg",Qt::CaseInsensitive)){
		m_CheckBox[8]->setChecked(true);
	}

    strList=ReadInpData.TmpVariableInps.at(0).strNInclude.split(",");//node File
    for (int i=0;i<strList.size();i++){
        str=strList.at(i);
        strList.replace(i,str.simplified());
    }
    if (strList.contains("u",Qt::CaseInsensitive)){
        m_CheckBox[1]->setChecked(true);
    }
    if (strList.contains("nt",Qt::CaseInsensitive)){
        m_CheckBox[6]->setChecked(true);
    }
    /*if(strList.contains("SDV",Qt::CaseInsensitive)){
        m_CheckBox[2]->setChecked(true);
        m_CheckBox[4]->setChecked(true);
        m_CheckBox[5]->setChecked(true);
    }*/

    //-- 
    int nRowSize = m_TableWig->rowCount();
    int nSolveRowSize = ReadInpData.TmpSolveInps.size();
    if (nSolveRowSize<=0){return;}

    //notֻ��һ��
    for (int i=nRowSize;i<nSolveRowSize;i++)
    {
        m_TableWig->insertRow(i);
    }
    
    for (int kk=0;kk<nSolveRowSize;kk++)
    {  
        QString strTmp=QString("step%1").arg(kk+1);
        QTableWidgetItem * tmpItem=new QTableWidgetItem(strTmp);
        m_TableWig->setItem(kk,0,tmpItem);
        strTmp=ReadInpData.TmpSolveInps.at(kk).strTotalTime;
        tmpItem=new QTableWidgetItem(strTmp);
        m_TableWig->setItem(kk,1,tmpItem);
        strTmp=ReadInpData.TmpSolveInps.at(kk).strInitStep;
        tmpItem=new QTableWidgetItem(strTmp);
        m_TableWig->setItem(kk,2,tmpItem);
        strTmp=ReadInpData.TmpSolveInps.at(kk).strMinStep;
        tmpItem=new QTableWidgetItem(strTmp);
        m_TableWig->setItem(kk,3,tmpItem);
        strTmp=ReadInpData.TmpSolveInps.at(kk).strMaxStep;
        tmpItem=new QTableWidgetItem(strTmp);
        m_TableWig->setItem(kk,4,tmpItem);

        strTmp=ReadInpData.TmpSolveInps.at(kk).strStepInc;//������沽��
        tmpItem=new QTableWidgetItem(strTmp);
        m_TableWig->setItem(kk,5,tmpItem);
        strTmp=ReadInpData.TmpSolveInps.at(kk).strDelMX;//����¶�����
        tmpItem=new QTableWidgetItem(strTmp);
        m_TableWig->setItem(kk,6,tmpItem);

		strTmp=ReadInpData.TmpSolveInps.at(kk).TimeFrequencyInc;
		tmpItem=new QTableWidgetItem(strTmp);
        m_TableWig->setItem(kk,8,tmpItem);
        strTmp=ReadInpData.TmpSolveInps.at(kk).strNlgeomSTR;	
       
       QComboBox* TmpcomboxCoupled = new QComboBox();
        TmpcomboxCoupled->addItem("��ȫ���");
        TmpcomboxCoupled->addItem("����ȫ���");
		TmpcomboxCoupled->addItem("��ʾ���");
        TmpcomboxCoupled->setFrame(false);
        m_TableWig->setCellWidget(kk,7,TmpcomboxCoupled);
        if (strTmp.toLower()=="*coupled temperature-displacement"){
            TmpcomboxCoupled->setCurrentIndex(0);
        }else if(strTmp.toLower()=="*uncoupled temperature-displacement"){
            TmpcomboxCoupled->setCurrentIndex(1);
		}else if(strTmp.toLower()=="*dynamic"){
			TmpcomboxCoupled->setCurrentIndex(2);
			m_TableWig->setColumnHidden(3,true);
			m_TableWig->setColumnHidden(4,true);
			m_TableWig->setColumnHidden(5,true);
			m_TableWig->setColumnHidden(6,true);
        }
		connect(TmpcomboxCoupled,SIGNAL(activated(int)),this,SLOT(ShowAndHideSlot(int)));
		connect(TmpcomboxCoupled,SIGNAL(activated(int)),this,SLOT(MCFSlot(int)));
    }
    
    WriteSloveToInp();  
}
void QHPSolveSetDlg::SetOutPutTab(int id)
{
    m_tabView->setCurrentIndex(id);//������ƽ���
}

//----------------------------------
//--1.���ݵ�ǰ��step���������ö�Ӧ
//--2.��������ơ�����
//----------------------------------
void QHPSolveSetDlg::SetSloveStep(int istep)
{
   int nRow = m_TableWig->rowCount();
    
   if (nRow ==istep){
   }else if(nRow>istep){//ȥ�������step
        for (int i=istep;i<nRow;i++){
             m_TableWig->removeRow(istep);
        }
   }else if(nRow<istep){//����istep�������
       m_TableWig->setRowCount(istep);
        for (int i=nRow;i<istep;i++){
            int curRow=i;
            QString strTmp=QString("step%1").arg(i+1);
            QTableWidgetItem * tmpItem1=new QTableWidgetItem(strTmp);
            m_TableWig->setItem(curRow,0,tmpItem1);
            tmpItem1=new QTableWidgetItem(tr("10"));
            m_TableWig->setItem(curRow,1,tmpItem1);
            tmpItem1=new QTableWidgetItem(tr("1"));
            m_TableWig->setItem(curRow,2,tmpItem1);
            tmpItem1=new QTableWidgetItem(tr("1e-5"));
            m_TableWig->setItem(curRow,3,tmpItem1);
            tmpItem1=new QTableWidgetItem(tr("20"));
            m_TableWig->setItem(curRow,4,tmpItem1);
            tmpItem1=new QTableWidgetItem(tr("1000000"));
            m_TableWig->setItem(curRow,5,tmpItem1);
            tmpItem1=new QTableWidgetItem(tr("50"));
            m_TableWig->setItem(curRow,6,tmpItem1);
			tmpItem1=new QTableWidgetItem("1");
            m_TableWig->setItem(curRow,8,tmpItem1);


            m_comboxCoupled = new QComboBox();
            m_comboxCoupled->addItem("��ȫ���");
            m_comboxCoupled->addItem("����ȫ���");
			 m_comboxCoupled->addItem("��ʾ���");
            m_comboxCoupled->setFrame(false);
            m_TableWig->setCellWidget(curRow,7,m_comboxCoupled);
			connect(m_comboxCoupled,SIGNAL(activated(int)),this,SLOT(ShowAndHideSlot(int)));
			connect(m_comboxCoupled,SIGNAL(activated(int)),this,SLOT(MCFSlot(int)));
        }
   }
}
void QHPSolveSetDlg::ShowAndHideSlot(int id)
{
	if(id==2){
		m_TableWig->setColumnHidden(3,true);
		m_TableWig->setColumnHidden(4,true);
		m_TableWig->setColumnHidden(5,true);
		m_TableWig->setColumnHidden(6,true);
	}else{
		m_TableWig->setColumnHidden(3,false);
		m_TableWig->setColumnHidden(4,false);
		m_TableWig->setColumnHidden(5,false);
		m_TableWig->setColumnHidden(6,false);
	}
}

void QHPSolveSetDlg::AppendPSetComboxSolvesetSlot(QString str)
{
    if(m_PSetList.indexOf(str) == -1){
        m_PSetList.append(str);
		QStringList str1;
		str1<<str;
		m_DatCombox->addItems(str1);
    }
    
}

void QHPSolveSetDlg::MCFSlot(int i)
{
	if(i==0||i==1){
		MassScalingFactor->setEnabled(false);
		IJKL=0;
		return;
	}
	IJKL=1;
	MassScalingFactor->setEnabled(true);
}