#include "QHPTTTDlg.h"
#include <QMessageBox>
#include <QMenu>
#include <QScrollArea>
#include <QHeaderView>

QHPTTTDlg* QHPTTTDlg::instance = 0;
QHPTTTDlg* QHPTTTDlg::instance2 = 0;
QHPTTTDlg::QHPTTTDlg(QWidget *parent)
    :QDialog(parent)
{
    CreatGUI();
}

QHPTTTDlg::~QHPTTTDlg()
{
    m_SetBtnChangePoint.clear();
    m_SetBtnPhaseBalance.clear();
    m_SetBtnYunYuQi.clear();
    m_SetBtnDaoRe.clear();
    m_SetBtnBiRe.clear();
    m_changePointDlg.clear();
    m_phaseBalanceDlg.clear();
}
QHPTTTDlg* QHPTTTDlg::GetInstance(QWidget *parent)
{
    if (instance == 0)
    {
        instance = new QHPTTTDlg(parent);
    }

    return instance;
}
QHPTTTDlg* QHPTTTDlg::GetInstance2()
{
    if (instance2 == 0)
    {
        instance2 = new QHPTTTDlg();
    }
    return instance2;
}


void QHPTTTDlg::CreatGUI()
{
    setWindowTitle("TTT");
    //--Tab1
    m_Tab1=new QWidget();//(tr("ddd1"));
    //m_ImageFormula = new QPushButton(QIcon(":/images/NewIcon.png"), tr("��"),m_Tab1);
    m_CheckBoxGestation = new QCheckBox(tr("����������"),m_Tab1);
    m_CheckBoxCrystalSize = new QCheckBox(tr("ĸ�Ӿ����ߴ�"),m_Tab1);
    m_CheckBoxCalculate = new QCheckBox(tr("��������"),m_Tab1);
    
    m_gbBox=new QGroupBox(m_Tab1);
    QHBoxLayout *gbHout1=new QHBoxLayout();
    QHBoxLayout *gbHout2=new QHBoxLayout();
    QHBoxLayout *gbHout3=new QHBoxLayout();
    QVBoxLayout *gbmainlayout=new QVBoxLayout(m_gbBox);
    
    QLabel *lable1=new QLabel(tr("Dy"),m_gbBox);
    m_EditTab1P1 = new QLineEdit(m_gbBox);
    gbHout1->addWidget(lable1);
    gbHout1->addWidget(m_EditTab1P1);
    gbHout1->addStretch();

    QLabel *lable2=new QLabel(tr("DyTTT"),m_gbBox);
    m_EditTab1P2 = new QLineEdit(m_gbBox);
    gbHout2->addWidget(lable2);
    gbHout2->addWidget(m_EditTab1P2);
    gbHout2->addStretch();

    QLabel *lable3=new QLabel(tr("m"),m_gbBox);
    m_EditTab1P3 = new QLineEdit(m_gbBox);
    gbHout3->addWidget(lable3);
    gbHout3->addWidget(m_EditTab1P3);
    gbHout3->addStretch();

    lable1->setMaximumWidth(20); lable2->setMaximumWidth(20); lable3->setMaximumWidth(20);
    m_EditTab1P1->setMaximumWidth(80); m_EditTab1P2->setMaximumWidth(80); m_EditTab1P3->setMaximumWidth(80);


    gbmainlayout->addLayout(gbHout1);
    gbmainlayout->addLayout(gbHout2);
    gbmainlayout->addLayout(gbHout3);

    QGridLayout *Gridlayout1 = new QGridLayout(m_Tab1);
    Gridlayout1->addWidget(m_CheckBoxGestation,0,0,1,1);
    Gridlayout1->addWidget(m_CheckBoxCrystalSize,1,0,1,1);
    Gridlayout1->addWidget(m_CheckBoxCalculate,2,0,1,1);
    Gridlayout1->addWidget(m_gbBox,3,0,1,1);
    //Gridlayout1->addWidget(m_ImageFormula,0,1,3,3);
    m_Tab1->setLayout(Gridlayout1);

    //--Tab2
    m_Tab2=new QWidget();//(tr("ddd2"));
    m_gbBoxTab2 = new QGroupBox(m_Tab2);
    m_gbBoxTab2->setCheckable(true);
    m_gbBoxTab2->setChecked(false);
    m_gbBoxTab2->setTitle(tr("����ǿ��"));
    m_gbBoxTab2->setStyleSheet(QString::fromUtf8("::title{color:blue}"));
    QHBoxLayout *HlayoutTab2_1=new QHBoxLayout();
    QHBoxLayout *HlayoutTab2_2=new QHBoxLayout();
    QHBoxLayout *HlayoutTab2_3=new QHBoxLayout();
    QHBoxLayout *HlayoutTab2_4=new QHBoxLayout();
    QLabel *labTab2P1= new QLabel(tr("d0:"));
    QLabel *labTab2P2= new QLabel(tr("A1:"));
    QLabel *labTab2P3= new QLabel(tr("Q:"));
    labTab2P1->setMidLineWidth(20); labTab2P2->setMidLineWidth(20); labTab2P3->setMidLineWidth(20);

    m_EditTab2P11 = new QLineEdit(m_gbBoxTab2);
    m_EditTab2P12 = new QLineEdit(m_gbBoxTab2);
    m_EditTab2P13 = new QLineEdit(m_gbBoxTab2);
    m_EditTab2P11->setMaximumWidth(80); m_EditTab2P12->setMaximumWidth(80); m_EditTab2P13->setMaximumWidth(80);

    HlayoutTab2_1->addWidget(labTab2P1);
    HlayoutTab2_1->addWidget(m_EditTab2P11);
    HlayoutTab2_1->addWidget(labTab2P2);
    HlayoutTab2_1->addWidget(m_EditTab2P12);
    HlayoutTab2_1->addWidget(labTab2P3);
    HlayoutTab2_1->addWidget(m_EditTab2P13);
    HlayoutTab2_1->addStretch();
    labTab2P1= new QLabel(tr("R:"));labTab2P2= new QLabel(tr("n:"));labTab2P3= new QLabel(tr("YS0:"));
    labTab2P1->setAlignment(Qt::AlignBottom);labTab2P2->setAlignment(Qt::AlignBottom);labTab2P3->setAlignment(Qt::AlignBottom);
    labTab2P1->setMidLineWidth(20); labTab2P2->setMidLineWidth(20); labTab2P3->setMidLineWidth(20);
    m_EditTab2P21 = new QLineEdit(m_gbBoxTab2);
    m_EditTab2P22 = new QLineEdit(m_gbBoxTab2);
    m_EditTab2P23 = new QLineEdit(m_gbBoxTab2);
    m_EditTab2P21->setMaximumWidth(80); m_EditTab2P22->setMaximumWidth(80); m_EditTab2P23->setMaximumWidth(80);
    HlayoutTab2_2->addWidget(labTab2P1);
    HlayoutTab2_2->addWidget(m_EditTab2P21);
    HlayoutTab2_2->addWidget(labTab2P2);
    HlayoutTab2_2->addWidget(m_EditTab2P22);
    HlayoutTab2_2->addWidget(labTab2P3);
    HlayoutTab2_2->addWidget(m_EditTab2P23);
    HlayoutTab2_2->addStretch();
    //HlayoutTab2_2->setMargin(20);
    //HlayoutTab2_2->setSpacing(20);

    labTab2P1= new QLabel(tr("M:"));
    labTab2P2= new QLabel(tr("A2:"));
    labTab2P3= new QLabel(tr("��:"));
    labTab2P1->setMidLineWidth(20); labTab2P2->setMidLineWidth(20); labTab2P3->setMidLineWidth(20);
    m_EditTab2P31 = new QLineEdit(m_gbBoxTab2);
    m_EditTab2P32 = new QLineEdit(m_gbBoxTab2);
    m_EditTab2P33 = new QLineEdit(m_gbBoxTab2);
    m_EditTab2P31->setMaximumWidth(80); m_EditTab2P32->setMaximumWidth(80); m_EditTab2P33->setMaximumWidth(80);

    HlayoutTab2_3->addWidget(labTab2P1);
    HlayoutTab2_3->addWidget(m_EditTab2P31);
    HlayoutTab2_3->addWidget(labTab2P2);
    HlayoutTab2_3->addWidget(m_EditTab2P32);
    HlayoutTab2_3->addWidget(labTab2P3);
    HlayoutTab2_3->addWidget(m_EditTab2P33);
    HlayoutTab2_3->addStretch();
    //HlayoutTab2_3->setMargin(20);
    //HlayoutTab2_3->setSpacing(20);

    labTab2P1= new QLabel(tr("yapb:"));
    labTab2P2= new QLabel(tr("b:"));
    labTab2P3= new QLabel(tr("w:"));
    m_EditTab2P41 = new QLineEdit(m_gbBoxTab2);
    m_EditTab2P42 = new QLineEdit(m_gbBoxTab2);
    m_EditTab2P43 = new QLineEdit(m_gbBoxTab2);
    labTab2P1->setMidLineWidth(20); labTab2P2->setMidLineWidth(20); labTab2P3->setMidLineWidth(20); 
    m_EditTab2P41->setMaximumWidth(80); m_EditTab2P42->setMaximumWidth(80); m_EditTab2P43->setMaximumWidth(80); 

    HlayoutTab2_4->addWidget(labTab2P1);
    HlayoutTab2_4->addWidget(m_EditTab2P41);
    HlayoutTab2_4->addWidget(labTab2P2);
    HlayoutTab2_4->addWidget(m_EditTab2P42);
    HlayoutTab2_4->addWidget(labTab2P3);
    HlayoutTab2_4->addWidget(m_EditTab2P43);
    HlayoutTab2_4->addStretch();
    //HlayoutTab2_4->setMargin(20);
    //HlayoutTab2_4->setSpacing(20);
    
    QVBoxLayout *gbBoxTab2_V= new QVBoxLayout(m_gbBoxTab2);
    gbBoxTab2_V->addLayout(HlayoutTab2_1);
    gbBoxTab2_V->addLayout(HlayoutTab2_2);
    gbBoxTab2_V->addLayout(HlayoutTab2_3);
    gbBoxTab2_V->addLayout(HlayoutTab2_4);
    gbBoxTab2_V->addStretch();

    QImage *png = new QImage;//
    png->load(":/images/HP.png");
    QLabel *imgLabel = new QLabel(tr("img"));
    imgLabel->setPixmap(QPixmap::fromImage(*png));//setPicture(QIcon(":/images/+x.png"));

    QGridLayout *Gridlayout2 = new QGridLayout(m_Tab2);
    Gridlayout2->addWidget(m_gbBoxTab2,0,0,1,1);
    Gridlayout2->addWidget(imgLabel,0,1,4,6);
    m_Tab2->setLayout(Gridlayout2);


    //--Tab3
    m_Tab3=new QWidget();//(tr("ddd3"));
    m_CheckBoxQianRe = new QCheckBox(tr("Ǳ��"));
    m_CheckBoxReWuXing = new QCheckBox(tr("������"));
    m_CheckBoxCalcChg = new QCheckBox(tr("�������"));
    QVBoxLayout *Gridlayout3 = new QVBoxLayout(m_Tab3);
    Gridlayout3->addWidget(m_CheckBoxQianRe);
    Gridlayout3->addWidget(m_CheckBoxReWuXing);
    Gridlayout3->addWidget(m_CheckBoxCalcChg);
    Gridlayout3->addStretch();
    
    m_tabView = new QTabWidget();
    m_tabView->addTab(m_Tab1, tr("��������"));
    m_tabView->addTab(m_Tab2, tr("����ǿ��"));
    m_tabView->addTab(m_Tab3, tr("����"));
   

    QScrollArea *areaInfomation_ = new QScrollArea();
    areaInfomation_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    areaInfomation_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    areaInfomation_->setWidgetResizable(true);
    areaInfomation_->setWidget(m_tabView);//setWidgetResizable(true);

    QStringList strList;
    //strList.append(tr("��"));
    strList<<tr("ת�����")<<tr("��ʼֵ")<<tr("��ƽ��")<<tr("������")<<tr("����")<<tr("����")<<tr("Ǳ��")<<tr("Ӳ��");
    m_TableWidget = new QMyTableWidget(this);//QTableWidget();
    m_TableWidget->setColumnCount(strList.size());
    m_TableWidget->setRowCount(0);
    m_TableWidget->verticalHeaderItem(1);
    //QLabel *lab_w=new QLabel(tr("��"));
    //lab_w->move(1,5);
    //lab_w->installEventFilter(this);
   
    
    m_TableWidget->setHorizontalHeaderLabels(strList);
    for (int j=0;j<strList.size();j++){
       m_TableWidget->horizontalHeader()->setResizeMode(j,QHeaderView::ResizeToContents);
    }
   
    //--combox
    for (int i=0; i<0; i++){
        QPushButton *tmpBtn = new QPushButton(tr("�½�"));
        m_TableWidget->setCellWidget(i,COL_CHANGEPOINT,tmpBtn);
        m_SetBtnChangePoint.insert(i,tmpBtn);//push_back(tmpBtn);//ת�����
        //connect(tmpBtn,SIGNAL(RightClicked(int,int)),this,SLOT(comBoxRChangePointSlot(int,int)));
        //connect(tmpBtn,SIGNAL(clicked(int,int)),this,SLOT(comBoxNewChangePointSlot(int,int)));
        connect(tmpBtn,SIGNAL(clicked()),this,SLOT(SetBtnSLot())); 
       
        tmpBtn = new QPushButton(tr("�½�"));
        m_TableWidget->setCellWidget(i,COL_PHASEBALANCE,tmpBtn);
        m_SetBtnPhaseBalance.insert(i,tmpBtn);//push_back(tmpBtn);//��λƽ�⡢
        //connect(tmpBtn,SIGNAL(RightClicked(int,int)),this,SLOT(comBoxRPhaseSlot(int,int)));
       // connect(tmpBtn,SIGNAL(clicked(int,int)),this,SLOT(comBoxNewPhaseSlot(int,int)));
        //connect(tmpBtn,SIGNAL(clicked()),this,SLOT(ChangePointSetBtnSLot()));
        connect(tmpBtn,SIGNAL(clicked()),this,SLOT(SetBtnSLot()));  

        tmpBtn = new QPushButton(tr("�½�"));
        m_TableWidget->setCellWidget(i,COL_YUNYUQI,tmpBtn);
        m_SetBtnYunYuQi.insert(i,tmpBtn);//push_back(tmpBtn);//������
        connect(tmpBtn,SIGNAL(clicked()),this,SLOT(SetBtnSLot())); 
        tmpBtn->setEnabled(false);

        tmpBtn = new QPushButton(tr("�½�"));
        m_TableWidget->setCellWidget(i,COL_DAORE,tmpBtn);
        m_SetBtnDaoRe.insert(i,tmpBtn);//push_back(tmpBtn);//����
        connect(tmpBtn,SIGNAL(clicked()),this,SLOT(SetBtnSLot())); 
        tmpBtn->setEnabled(false);

        tmpBtn = new QPushButton(tr("�½�"));
        m_TableWidget->setCellWidget(i,COL_BIRE,tmpBtn);
        m_SetBtnBiRe.insert(i,tmpBtn);//push_back(tmpBtn);//����
        connect(tmpBtn,SIGNAL(clicked()),this,SLOT(SetBtnSLot()));        
        tmpBtn->setEnabled(false);


        ////��
        //QTableWidgetItem *tmpItem2=new QTableWidgetItem(QString("%1").arg(i+1));
        //m_TableWidget->setItem(i,0,tmpItem2);
        ////��ʼֵ
        //tmpItem2=new QTableWidgetItem(tr("10%"));
        //m_TableWidget->setItem(i,2,tmpItem2);

        //��
        //QTableWidgetItem *tmpItem2=new QTableWidgetItem(QString("%1").arg(i+1));
        //m_TableWidget->setItem(i,0,tmpItem2);
        //��ʼֵ
        QTableWidgetItem *tmpItem2=new QTableWidgetItem(tr("0.01"));
        m_TableWidget->setItem(i,COL_INIT,tmpItem2);

        tmpItem2=new QTableWidgetItem(tr("0.01"));
        m_TableWidget->setItem(i,COL_QIANRE,tmpItem2);
        m_TableWidget->item(i,COL_QIANRE)->setFlags(m_TableWidget->item(i,COL_QIANRE)->flags()&~Qt::ItemIsEnabled);
        
        tmpItem2=new QTableWidgetItem(tr("0.01"));
        m_TableWidget->setItem(i,COL_HARD,tmpItem2);
        m_TableWidget->item(i,COL_HARD)->setFlags(m_TableWidget->item(i,COL_HARD)->flags()&~Qt::ItemIsEnabled);
    }

    m_TableWidget->setMinimumHeight(200);
 
    m_AddLineBtn = new QPushButton(tr("����"));
    m_DelLineBtn = new QPushButton(tr("ɾ��"));
    m_InsertLineBtn = new QPushButton(tr("����"));
	m_ImportLineBtn = new QPushButton(tr("����"));
	m_ExportLineBtn = new QPushButton(tr("����"));
    m_OkBtn = new QPushButton(tr("ȷ��"));

    QGridLayout *mainLayout =new QGridLayout();
    mainLayout->addWidget(areaInfomation_,0,0,4,2);
    mainLayout->addWidget(m_TableWidget,4,0,4,1);
    mainLayout->addWidget(m_AddLineBtn,4,1,1,1);
    mainLayout->addWidget(m_DelLineBtn,5,1,1,1);
	//mainLayout->addWidget(m_InsertLineBtn,6,1,1,1);
	mainLayout->addWidget(m_ImportLineBtn,6,1,1,1);//luo
    mainLayout->addWidget(m_ExportLineBtn,7,1,1,1);//luo
    mainLayout->addWidget(m_OkBtn,8,1,1,1);//luo


    this->setLayout(mainLayout);
    this->setMinimumWidth(500);

    connect(m_CheckBoxGestation,SIGNAL(clicked()),this,SLOT(GestationSlot()));
    connect(m_CheckBoxCrystalSize,SIGNAL(clicked()),this,SLOT(CrystalSizeSlot()));
    connect(m_CheckBoxQianRe,SIGNAL(clicked()),this,SLOT(QianReSlot()));
    connect(m_CheckBoxReWuXing,SIGNAL(clicked()),this,SLOT(ReWuXingSlot()));
    connect(m_CheckBoxCalculate,SIGNAL(clicked()),this,SLOT(CalYunyuSlot()));

    connect(m_AddLineBtn,SIGNAL(clicked()),this,SLOT(AddLineBtnSlot()));//����
    //connect(m_InsertLineBtn,SIGNAL(clicked()),this,SLOT(InsertLineBtnSlot()));//����
    connect(m_DelLineBtn,SIGNAL(clicked()),this,SLOT(DelLineBtnSlot()));//ɾ��
	connect(m_ImportLineBtn,SIGNAL(clicked()),this,SLOT(ImportLineBtnSlot()));//����
    connect(m_ExportLineBtn,SIGNAL(clicked()),this,SLOT(ExportLineBtnSlot()));//����
    connect(m_OkBtn,SIGNAL(clicked()),this,SLOT(OkBtnSlot()));//ȷ��
    //m_ImageFormula->setFlat(true);
    m_CheckBoxCrystalSize->setEnabled(false);
    m_CheckBoxCalculate->setEnabled(false);
   // m_gbBox->hide();
}

void QHPTTTDlg::TTTAddLine(int nCurRow)
{
    int nRow = m_TableWidget->rowCount();
    if (nRow<=0)return;

    QPushButton *tmpBtn = new QPushButton(tr("�½�"));
    m_TableWidget->setCellWidget(nCurRow,COL_CHANGEPOINT,tmpBtn);
    m_SetBtnChangePoint.insert(nCurRow,tmpBtn);//push_back(tmpBtn);//ת�����
    connect(tmpBtn,SIGNAL(clicked()),this,SLOT(SetBtnSLot()));

    tmpBtn = new QPushButton(tr("�½�"));
    m_TableWidget->setCellWidget(nCurRow,COL_PHASEBALANCE,tmpBtn);
    m_SetBtnPhaseBalance.insert(nCurRow,tmpBtn);//push_back(tmpBtn);//��λƽ�⡢
    connect(tmpBtn,SIGNAL(clicked()),this,SLOT(SetBtnSLot())); 

    tmpBtn = new QPushButton(tr("�½�"));
    m_TableWidget->setCellWidget(nCurRow,COL_YUNYUQI,tmpBtn);
    m_SetBtnYunYuQi.insert(nCurRow,tmpBtn);//push_back(tmpBtn);//������
    connect(tmpBtn,SIGNAL(clicked()),this,SLOT(SetBtnSLot())); 
    tmpBtn->setEnabled(m_CheckBoxGestation->isChecked());

    tmpBtn = new QPushButton(tr("�½�"));
    m_TableWidget->setCellWidget(nCurRow,COL_DAORE,tmpBtn);
    m_SetBtnDaoRe.insert(nCurRow,tmpBtn);//push_back(tmpBtn);//����
    connect(tmpBtn,SIGNAL(clicked()),this,SLOT(SetBtnSLot())); 
    tmpBtn->setEnabled(m_CheckBoxReWuXing->isChecked());

    tmpBtn = new QPushButton(tr("�½�"));
    m_TableWidget->setCellWidget(nCurRow,COL_BIRE,tmpBtn);
    m_SetBtnBiRe.insert(nCurRow,tmpBtn);//push_back(tmpBtn);//����
    connect(tmpBtn,SIGNAL(clicked()),this,SLOT(SetBtnSLot())); 
    tmpBtn->setEnabled(m_CheckBoxReWuXing->isChecked());

    //��ʼֵ
    QTableWidgetItem *tmpItem2=new QTableWidgetItem(tr("0.01"));
    m_TableWidget->setItem(nCurRow,COL_INIT,tmpItem2);

    tmpItem2=new QTableWidgetItem(tr("0.01"));//Ǳ��
    m_TableWidget->setItem(nCurRow,COL_QIANRE,tmpItem2);
    tmpItem2=new QTableWidgetItem(tr("0.01"));//Ӳ��
    m_TableWidget->setItem(nCurRow,COL_HARD,tmpItem2);

    if (m_CheckBoxQianRe->isChecked()){
      m_TableWidget->item(nCurRow,COL_QIANRE)->setFlags(m_TableWidget->item(nCurRow,COL_QIANRE)->flags() | Qt::ItemIsEnabled);
      m_TableWidget->item(nCurRow,COL_HARD)->setFlags(m_TableWidget->item(nCurRow,COL_HARD)->flags() | Qt::ItemIsEnabled);
    }else{      
      m_TableWidget->item(nCurRow,COL_QIANRE)->setFlags(m_TableWidget->item(nCurRow,COL_QIANRE)->flags()&~Qt::ItemIsEnabled);
      m_TableWidget->item(nCurRow,COL_HARD)->setFlags(m_TableWidget->item(nCurRow,COL_HARD)->flags()&~Qt::ItemIsEnabled);
    } 
}
/*
    Table������ʱ���Ը���combox����ITEM��ֵ��
*/
bool QHPTTTDlg::AddExistComboxText(MyCombox *tmpCombox, int curCol)
{
    int nRow=m_TableWidget->rowCount();
    MyCombox *ff = new MyCombox(this,m_SetBtnChangePoint.size());
    for (int i=0;i<nRow;i++){
        ff=(MyCombox*)(m_TableWidget->cellWidget(i,curCol));
        if (ff!=NULL){
            for (int kk=0;kk<ff->count();kk++){
                tmpCombox->addItem(ff->itemText(kk));
            }
            return true;
        }
    }
    tmpCombox->addItem("�½�");//����
    return false;
}
/*
    ��Table��COMBOX����true/false��
*/
void QHPTTTDlg::SetTableComboxEnable(int curCol,bool bEnable)
{
    int nRow=m_TableWidget->rowCount();

    for (int i=0;i<nRow;i++){
        QPushButton *tmpBtn=(QPushButton*)m_TableWidget->cellWidget(i,curCol);
        if (tmpBtn!=NULL){
            tmpBtn->setEnabled(bEnable);
        }
    }
}
//--����
void QHPTTTDlg::AddLineBtnSlot()
{
    int nRow = m_TableWidget->rowCount();
    m_TableWidget->setRowCount(nRow+1);
    TTTAddLine(nRow);
}
//--����
void QHPTTTDlg::InsertLineBtnSlot()
{
    int curRow = m_TableWidget->currentRow();
    m_TableWidget->insertRow(curRow);
    TTTAddLine(curRow);
}
//--ɾ��
void QHPTTTDlg::DelLineBtnSlot()
{
    int curRow = m_TableWidget->currentRow();
    if (curRow<0){
      return;
    }

    m_TableWidget->removeRow(curRow);
   /* m_ComboxChangePoint.remove(curRow);
    m_ComboxPhaseBalance.remove(curRow);
    m_ComboxYunYuQi.remove(curRow);
    m_ComboxDaoRe.remove(curRow);
    m_ComboxBiRe.remove(curRow);
    m_ComboxQianRe.remove(curRow);*/
}
void QHPTTTDlg::ImportLineBtnSlot()
{
	TTTSetDataS DATA;
	QString fileName =QFileDialog::getOpenFileName(this,tr("Read xml"),QString(),"XML files(*.xml)");
	if (fileName == ""){
		return ;
	}
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly|QIODevice::Text)){
		qDebug()<<"���ļ�ʱ����";
		return ;
	}
	QStringList lists = fileName.split(".");
	QStringList listName =lists.first().split("/");
	m_NowFileName = listName.back();
	QString m_data;
	QXmlStreamReader Reader(&file);
	QXmlStreamReader::TokenType type;
	QString strtmp;
	while (!Reader.atEnd())
	{
		type = Reader.readNext();
		strtmp=Reader.name().toString();
		if(type == QXmlStreamReader::EndElement)
		{
			if(Reader.name()=="�������") 
				break;
		}
		if (type == QXmlStreamReader::StartElement)
		{
			if (strtmp=="�������")
			{
				DATA=ParseEntry(Reader);
			}
		}	
	}
	SetInpData(DATA);
	file.close();
	if (Reader.hasError())//��������ʱ
	{
	}
}
//����
void QHPTTTDlg::ExportLineBtnSlot()
{
	QString fileName =QFileDialog::getSaveFileName(this,tr("Save as xml"),QString(),"XML files(*.xml)");
	if (fileName == ""){
		return;
	}
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly|QIODevice::Text)){
		return;
	}
	QXmlStreamWriter writer(&file);
	writer.setAutoFormatting(true);
	writer.writeStartElement("�������");
	int nRow = m_TableWidget->rowCount();
	int nCol = m_TableWidget->columnCount();
	QPushButton *tmpBtn=NULL;
	QStringList strHeadParam;
	QStringList strData; 
	QString     strData1;  
	for (int i=0; i<nRow; i++){
		writer.writeStartElement("��"+QString::number(i+1,10)+"���������");
		for(int KK=0;KK<8;KK++){
			tmpBtn =(QPushButton *)m_TableWidget->cellWidget(i,KK);
			switch(KK){
				case 0:
					{
						if (tmpBtn==NULL) continue;
						int id=m_SetBtnChangePoint.key(tmpBtn);
						if (id==-1)continue;
						if (m_changePointDlg.find(id)==m_changePointDlg.end()){continue;}
						int iTabNum=m_changePointDlg[id]->GetCurveNum();
						QString strTmp,str;
						writer.writeStartElement("TTTCCT����");
						for (int jj=0; jj<iTabNum; jj++)
						{
							int DataNum = m_changePointDlg[id]->GetDataNum(jj);
							QString strTmp = tr("%1").arg(i+1)+","+//����
								tr("%1").arg(jj+1)+","+//
								tr("%1").arg(DataNum)+","+//���ݵ���
								m_changePointDlg[id]->GetCurveThPoint(jj)+","+//����
								m_TableWidget->item(i,COL_INIT)->text();
							writer.writeTextElement("��"+QString::number(jj+1,10)+"������", strTmp);
							if (DataNum){
								strData1=m_changePointDlg[id]->GetPlotData(jj);
								strData=strData1.split(";");
								int IIK=strData.size();
								for(int IJK=0;IJK<IIK;IJK++){;
								writer.writeTextElement("����ֵ", strData.at(IJK));
								}
							}  
						}
						writer.writeEndElement();
					}
					break;
				case 1:
					{
						break;
					}
				case 2:
					{
						if (tmpBtn==NULL) continue;
						int id=m_SetBtnPhaseBalance.key(tmpBtn);
						if (id==-1)continue;
						if (m_phaseBalanceDlg.find(id)==m_phaseBalanceDlg.end()){continue;}
						int DataNum=m_phaseBalanceDlg[id]->GetDataNum();
						QString strTmp;
						strTmp = tr("%1").arg(i+1)+","+//����
							tr("%1").arg(DataNum);//���ݵ���
						writer.writeStartElement("��ƽ��");
						writer.writeTextElement("��"+QString::number(i+1,10)+"����ƽ��", strTmp);
						if (DataNum) {
							strData1=m_phaseBalanceDlg[id]->GetPlotData();
							strData=strData1.split(";");
							int IIK=strData.size();
							for(int IJK=0;IJK<IIK;IJK++){
								writer.writeTextElement("��ƽ��ֵ", strData.at(IJK));
							}
						}
						writer.writeEndElement();
					}
					break;
				case 3:
					{
						if (tmpBtn==NULL) continue;
						int id=m_SetBtnYunYuQi.key(tmpBtn);
						if (id==-1)continue;
						if (m_IncubationPeriodDlg.find(id)==m_IncubationPeriodDlg.end()){continue;}
						int DataNum=m_IncubationPeriodDlg[id]->GetDataNum();
						QString strTmp;
						strTmp = tr("%1").arg(i+1)+","+//����
							tr("%1").arg(DataNum);//���ݵ���
						writer.writeStartElement("������");
						writer.writeTextElement("��"+QString::number(i+1,10)+"����������", strTmp);
						if (DataNum) {
							strData1=m_IncubationPeriodDlg[id]->GetPlotData();
							strData=strData1.split(";");
							int IIK=strData.size();
							for(int IJK=0;IJK<IIK;IJK++){
								writer.writeTextElement("������ֵ", strData.at(IJK));
							}
						}  
						writer.writeEndElement();
					}
					break;
				case 4:
					{
						if (tmpBtn==NULL) continue;
						int id=m_SetBtnDaoRe.key(tmpBtn);
						if (id==-1)continue;
						if (m_PhasePropCondDlg.find(id)==m_PhasePropCondDlg.end()){continue;}
						int DataNum=m_PhasePropCondDlg[id]->GetDataNum();
						QString strTmp;
						strTmp = tr("%1").arg(i+1)+","+//����
							tr("%1").arg(DataNum);//���ݵ���
						writer.writeStartElement("����");
						writer.writeTextElement("��"+QString::number(i+1,10)+"���ർ��ϵ��", strTmp);
						if (DataNum) {
							strData1=m_PhasePropCondDlg[id]->GetPlotData();
							strData=strData1.split(";");
							int IIK=strData.size();
							for(int IJK=0;IJK<IIK;IJK++){
								writer.writeTextElement("����ϵ��ֵ", strData.at(IJK));
							}
						}
						writer.writeEndElement();
					}
					break;
				case 5:
					{
						if (tmpBtn==NULL) continue;
						int id=m_SetBtnBiRe.key(tmpBtn);
						if (id==-1)continue;
						if (m_PhasePropSpecDlg.find(id)==m_PhasePropSpecDlg.end()){continue;}
						int DataNum=m_PhasePropSpecDlg[id]->GetDataNum();
						QString strTmp;
						strTmp = tr("%1").arg(i+1)+","+//����
							tr("%1").arg(DataNum);//���ݵ���
						writer.writeStartElement("����");
						writer.writeTextElement("��"+QString::number(i+1,10)+"�������", strTmp);
						if (DataNum){
							strData1=m_PhasePropSpecDlg[id]->GetPlotData();
							strData=strData1.split(";");
							int IIK=strData.size();
							for(int IJK=0;IJK<IIK;IJK++){
								writer.writeTextElement("����ֵ", strData.at(IJK));
							}
						}  
						writer.writeEndElement();
					}
					break;
				case 6:
					{
						QString strDataVal = m_TableWidget->item(i,COL_QIANRE)->text();     
						QString strTmp;
						strTmp = tr("%1").arg(i+1)+","+//����
							tr("%1").arg(strDataVal);//��ֵ
						writer.writeStartElement("Ǳ��");
						writer.writeTextElement("Ǳ��ֵ", strTmp);    
						writer.writeEndElement();
					}
					break;
				case 7:
					{
						QString strDataVal = m_TableWidget->item(i,COL_HARD)->text();     
						QString strTmp;
						strTmp = tr("%1").arg(i+1)+","+//����
							tr("%1").arg(strDataVal);//��ֵ
                        writer.writeStartElement("Ӳ��");
						writer.writeTextElement("Ӳ��ֵ", strTmp);
						writer.writeEndElement();
					}
					break;
				default:;
			}
		} 
		writer.writeEndElement();
	} 
	writer.writeEndElement();
	writer.writeEndDocument(); 
}
//--ȷ��
void QHPTTTDlg::OkBtnSlot()
{
    this->accept();
}
//--���������ڼ�
void QHPTTTDlg::GestationSlot()
{
    if (m_CheckBoxGestation->isChecked()){//ѡ���������ڼ�
        m_CheckBoxCrystalSize->setEnabled(true);//
        m_CheckBoxCalculate->setEnabled(true);
        SetTableComboxEnable(COL_YUNYUQI,!m_CheckBoxCalculate->isChecked());
    }else{
        m_CheckBoxCrystalSize->setEnabled(false);
        m_gbBox->hide();
        m_CheckBoxCalculate->setEnabled(false);
        SetTableComboxEnable(COL_YUNYUQI,false);
    } 
}
//--�����������ѡ�����ܱ༭����
void QHPTTTDlg::CalYunyuSlot()
{
	SetTableComboxEnable(COL_YUNYUQI,!m_CheckBoxCalculate->isChecked());
}
//--��ĸ�����ߴ�
void QHPTTTDlg::CrystalSizeSlot()
{
    if (m_CheckBoxCrystalSize->isChecked()){
        m_gbBox->show();
    }else{
        m_gbBox->hide();
    }
}
//--Ǳ��checkbox
void QHPTTTDlg::QianReSlot()
{
  //SetTableComboxEnable(COL_QIANRE,m_CheckBoxQianRe->isChecked());
    if (m_CheckBoxQianRe->isChecked()){
        for (int curRow=0;curRow<m_TableWidget->rowCount();curRow++){
            m_TableWidget->item(curRow,COL_QIANRE)->setFlags(m_TableWidget->item(curRow,COL_QIANRE)->flags() | Qt::ItemIsEnabled);
            m_TableWidget->item(curRow,COL_HARD)->setFlags(m_TableWidget->item(curRow,COL_HARD)->flags() | Qt::ItemIsEnabled);
        } 
    }else{
        for (int curRow=0;curRow<m_TableWidget->rowCount();curRow++){
            m_TableWidget->item(curRow,COL_QIANRE)->setFlags(m_TableWidget->item(curRow,COL_QIANRE)->flags()&~Qt::ItemIsEnabled);
            m_TableWidget->item(curRow,COL_HARD)->setFlags(m_TableWidget->item(curRow,COL_HARD)->flags()&~Qt::ItemIsEnabled);
        }
    } 
}
//--������
void QHPTTTDlg::ReWuXingSlot()
{
   SetTableComboxEnable(COL_DAORE,m_CheckBoxReWuXing->isChecked());
   SetTableComboxEnable(COL_BIRE,m_CheckBoxReWuXing->isChecked());   
}
TTTSetDataS QHPTTTDlg::ParseEntry(QXmlStreamReader &reader)
{
	TTTSetDataS PDATA;
	PDATA.iPhaseNum=0;
	QStringList Data11;
	int PhaseN,NCurveNumber;//���š����߱��
	QMap<int,PhaseChangeTabS> PhaseCurve;
	PhaseChangeTabS TmpPhaseCurve;
	PhaseBaseItem PDSB;
	QString strRef;
	QXmlStreamReader::TokenType type;	
	while (!reader.atEnd())
	{
		type = reader.readNext();
		QString strName = reader.name().toString();
		if(type == QXmlStreamReader::EndElement)
		{
			if(strName.mid(2)=="���������") 
				break;
		}
		if (type == QXmlStreamReader::StartElement)
		{
			while(!reader.atEnd()){
				type = reader.readNext();
				QString strRef = reader.name().toString();
				if(type == QXmlStreamReader::EndElement)
				{
					if(strRef=="TTTCCT����"||strRef=="��ƽ��"||strRef=="����"||strRef=="����"||strRef=="Ǳ��"||strRef=="Ӳ��") 
						continue;
				}
				if(type == QXmlStreamReader::EndElement)
				{
					if(strRef.mid(2)=="���������"){ 
						PDATA.PhaseCurve.insert(PhaseN,PhaseCurve);
						if(PhaseN>PDATA.iPhaseNum)PDATA.iPhaseNum=PhaseN;
						break;}
				}
				if (type == QXmlStreamReader::StartElement)
				{
					int NumberC=0;
					while(!reader.atEnd()){
						type = reader.readNext();
						QString strRef = reader.name().toString();
						if(type == QXmlStreamReader::EndElement){
							if(strRef=="TTTCCT����"||strRef=="Ǳ��"||strRef=="Ӳ��")break;
							if(strRef=="��ƽ��"){
								PDATA.PhaseBalance.insert(PhaseN,PDSB);
								break;
							}else if(strRef=="����"){
								PDATA.PhasePropCond.insert(PhaseN,PDSB);
								break;
							}else if(strRef=="����"){
								PDATA.PhasePropSpec.insert(PhaseN,PDSB);
								break;
							}else if(strRef=="������"){
								PDATA.PhaseIP.insert(PhaseN,PDSB);
								break;
							}
						}
						if (type == QXmlStreamReader::StartElement)
						{
							int NameId = chgTheNameToInt(strRef);
							switch(NameId){
								case 1:
									{
										QString Data= reader.readElementText();
										if(Data.remove(QRegExp("\\s"))!=""){
											Data11<<Data.split(",");
											PhaseN=Data11.at(0).toInt();//����
											NCurveNumber=Data11.at(1).toInt();//������
											TmpPhaseCurve.iNum=Data11.at(2).toInt();//���ݵ���
											TmpPhaseCurve.strChangeP=Data11.at(3);//���߷���
											TmpPhaseCurve.strData.clear();
											PDATA.strInitVal.insert(PhaseN,Data11.at(4));//��ʼֵ
										}
									}
									break;
								case 2:
									{
										QString Data= reader.readElementText();
										if(Data.remove(QRegExp("\\s"))!=""){
											NumberC+=1;
											TmpPhaseCurve.strData<<Data;
										}
									}
									break;
								case 3:
									{
										QString Data= reader.readElementText();
										if(Data.remove(QRegExp("\\s"))!=""){
											QStringList Data22;
											Data22<<Data.split(",");
											PDSB.iNum=Data22.at(1).toInt();
											PDSB.strData.clear();
											Data22.clear();
										}
									}
									break;
								case 4:
									{
										QString Data= reader.readElementText();
										if(Data.remove(QRegExp("\\s"))!=""){
											PDSB.strData<<Data;
										}
									}
									break;
								case 9:
									{
										QString Data= reader.readElementText();
										if(Data.remove(QRegExp("\\s"))!=""){
											QStringList Data22;
											Data22<<Data.split(",");
											int PhaseN1=Data22.at(0).toInt();//����
											PDATA.strLatenTheat.insert(PhaseN1,Data22.at(1));
											Data22.clear();
										}
									}
									break;
								case 10:
									{
										QString Data= reader.readElementText();
										if(Data.remove(QRegExp("\\s"))!=""){
											QStringList Data22;
											Data22<<Data.split(",");
											int PhaseN1=Data22.at(0).toInt();//����
											PDATA.strHard.insert(PhaseN1,Data22.at(1));
											Data22.clear();}
									}
									break;
								default:;
							}
							if(strRef.mid(2)=="������"&&Data11.at(2).toInt()==0){
								PhaseCurve.insert(NCurveNumber,TmpPhaseCurve);
								Data11.clear();
							}
							if(NumberC==TmpPhaseCurve.iNum&&NumberC!=0){
								NumberC=0;
								PhaseCurve.insert(NCurveNumber,TmpPhaseCurve);
								Data11.clear();
							}
						}
					}
				}
			}
		}
	}
	return PDATA;
}
int QHPTTTDlg::chgTheNameToInt(QString strRef)
{
	QString strRef1=strRef.at(0);
	if(strRef1=="��")strRef=strRef.mid(2);
	if (strRef=="������"){return 1;
	}else if (strRef=="����ֵ"){return 2;
    }else if (strRef=="����ƽ��"||strRef=="���ർ��ϵ��"||strRef=="�������"||strRef=="����������"){return 3;
	}else if (strRef=="��ƽ��ֵ"||strRef=="����ϵ��ֵ"||strRef=="����ֵ"||strRef=="������ֵ"){return 4;
	}else if (strRef=="Ǳ��ֵ"){return 9;
	}else if (strRef=="Ӳ��ֵ"){return 10;
	}else if (strRef=="TTTCCT����"||strRef=="��ƽ��"||strRef=="����"||strRef=="����"||strRef=="Ӳ��"){return 11;
	}else if (strRef==""){return 12;}
	return -1;
}
////--

//--д�뵽inp��;
HPTTTInps QHPTTTDlg::WriteHPTTTToInp()
{ 
   int nRow = m_TableWidget->rowCount();
   int nCol = m_TableWidget->columnCount();

   HPTTTInps TotalTTTInps;
   HPphaseCurveInps tmpChangePointInps;
   HPphaseBalanceInps tmpPhaseBalanceInps;
   HPincubatePeriodInps tmpIncuPeriodInps;
   HPphaseElateHeatInps tmpPhaseElateHeatInps;//Ǳ��
   HPphaseHardInps tmpPhaseHardInps;//Ӳ��
   HPPhasePropSpecInps tmpPhasePropSpecInps;//����
   HPPhasePropCondInps tmpPhasePropCondInps;//����
   HPphaseCtrolInps tmpPhaseCtrolInps;
   HPPhaseYSInps tmpPhaseYSInps;//����ǿ��
   HPPhaseGSInps tmpPhaseGSInps;//�����ߴ����

   QStringList strHeadParam;
   QStringList strData; 

   QPushButton *tmpBtn=NULL;

   //-----ת�����
   for (int i=0; i<nRow; i++){
        tmpBtn =(QPushButton *)m_TableWidget->cellWidget(i,COL_CHANGEPOINT);
        if (tmpBtn==NULL) continue;;
        int id=m_SetBtnChangePoint.key(tmpBtn);
        if (id==-1)continue;
        if (m_changePointDlg.find(id)==m_changePointDlg.end()){continue;}
        int iTabNum=m_changePointDlg[id]->GetCurveNum();
        QString strTmp;
        for (int jj=0; jj<iTabNum; jj++)
        {
           int DataNum = m_changePointDlg[id]->GetDataNum(jj);
           QString strTmp = tr("%1").arg(i+1)+","+//����
                            tr("%1").arg(jj+1)+","+//
                            tr("%1").arg(DataNum)+","+//���ݵ���
                            m_changePointDlg[id]->GetCurveThPoint(jj)+","+//����
                            m_TableWidget->item(i,COL_INIT)->text();       
             strHeadParam<<strTmp;
             if (DataNum)strData<<m_changePointDlg[id]->GetPlotData(jj);  
        }     
    }  
    tmpChangePointInps.strName="PHASECURVE";
    tmpChangePointInps.strPhaseCurveType="TTT";
    tmpChangePointInps.strHeadParam=strHeadParam;
    tmpChangePointInps.strData=strData;

   //-----��ƽ��
   strHeadParam.clear();strData.clear();
   for (int i=0; i<nRow; i++){ 
       tmpBtn=(QPushButton *)m_TableWidget->cellWidget(i,COL_PHASEBALANCE);
       if (tmpBtn==NULL) continue;;
       int id=m_SetBtnPhaseBalance.key(tmpBtn);
       if (id==-1)continue;;
       if (m_phaseBalanceDlg.find(id)==m_phaseBalanceDlg.end()){continue;}

       int DataNum=m_phaseBalanceDlg[id]->GetDataNum();
       QString strTmp;
       strTmp = tr("%1").arg(i+1)+","+//����
                tr("%1").arg(DataNum);//���ݵ���
       strHeadParam<<strTmp;
       if (DataNum) strData<<m_phaseBalanceDlg[id]->GetPlotData();            
   }  
   tmpPhaseBalanceInps.strName="PHASEEQUILIBRIUM";
   tmpPhaseBalanceInps.strHeadParam=strHeadParam;
   tmpPhaseBalanceInps.strData=strData;

   //--�ȴ�����ʽ����
   QString strCntlAnalyStyle;
   strCntlAnalyStyle ="1";//������������
   if (m_CheckBoxGestation->isChecked())
       strCntlAnalyStyle="2";//����������
   if (m_CheckBoxCrystalSize->isChecked())
       strCntlAnalyStyle="3";//ĸ���ྦྷ���ߴ�

   QString strCntrPeriod;
   strCntrPeriod="0";
   if (strCntlAnalyStyle=="1"&&m_CheckBoxCalculate->isChecked())
       strCntrPeriod="1";

   QString strCntrLatentheat;
   strCntrLatentheat="0";//�Ƿ���Ǳ��
   if (m_CheckBoxQianRe->isChecked())
       strCntrLatentheat="1";

   QString strCntrProp;
   strCntrProp="0";//�Ƿ���������
   if (m_CheckBoxReWuXing->isChecked())
       strCntrProp="1";
   tmpPhaseCtrolInps.strName="PHASECTROL";
   tmpPhaseCtrolInps.strHeadParam = strCntlAnalyStyle+","+strCntrLatentheat+","+
       strCntrProp+","+strCntrPeriod+",0";

   //-----Ǳ��
   strHeadParam.clear();
   if (strCntrLatentheat.toInt()==1)
   {
       for (int i=0; i<nRow; i++){ 
           QString strDataVal = m_TableWidget->item(i,COL_QIANRE)->text();     
           QString strTmp;
           strTmp = tr("%1").arg(i+1)+","+//����
                    tr("%1").arg(strDataVal);//��ֵ
           strHeadParam<<strTmp;      
       } 
        tmpPhaseElateHeatInps.strName="PHASELATENTHEAT";
        tmpPhaseElateHeatInps.strHeadParam=strHeadParam;
   }
   //--Ӳ��
   strHeadParam.clear();
   if (strCntrLatentheat.toInt()==1)
   {
       for (int i=0; i<nRow; i++){ 
           QString strDataVal = m_TableWidget->item(i,COL_HARD)->text();     
           QString strTmp;
           strTmp = tr("%1").arg(i+1)+","+//����
               tr("%1").arg(strDataVal);//��ֵ
           strHeadParam<<strTmp;      
       } 
       tmpPhaseHardInps.strName="PHASEHARDNESS";
       tmpPhaseHardInps.strHeadParam=strHeadParam;
   }


   ////-----������
   strHeadParam.clear();strData.clear();
   if (strCntlAnalyStyle.toInt()==2)
   {
       for (int i=0; i<nRow; i++){ 
           tmpBtn=(QPushButton *)m_TableWidget->cellWidget(i,COL_YUNYUQI);
           if (tmpBtn==NULL) continue;;
           int id=m_SetBtnYunYuQi.key(tmpBtn);
           if (id==-1)continue;;
           if (m_IncubationPeriodDlg.find(id)==m_IncubationPeriodDlg.end()){continue;}

           int DataNum=m_IncubationPeriodDlg[id]->GetDataNum();
           QString strTmp;
           strTmp = tr("%1").arg(i+1)+","+//����
               tr("%1").arg(DataNum);//���ݵ���
           strHeadParam<<strTmp;
           if (DataNum) strData<<m_IncubationPeriodDlg[id]->GetPlotData();            
       }  
       tmpIncuPeriodInps.strName="INCUBATIONPERIOD";
       tmpIncuPeriodInps.strHeadParam=strHeadParam;
       tmpIncuPeriodInps.strData=strData;
   }
   

   // //-----����
    strHeadParam.clear();strData.clear();
    if (strCntrProp.toInt()==1)
    {
    
        for (int i=0; i<nRow; i++){ 
            tmpBtn=(QPushButton *)m_TableWidget->cellWidget(i,COL_BIRE);
            if (tmpBtn==NULL) continue;;
            int id=m_SetBtnBiRe.key(tmpBtn);
            if (id==-1)continue;;
            if (m_PhasePropSpecDlg.find(id)==m_PhasePropSpecDlg.end()){continue;}

            int DataNum=m_PhasePropSpecDlg[id]->GetDataNum();
            QString strTmp;
            strTmp = tr("%1").arg(i+1)+","+//����
                tr("%1").arg(DataNum);//���ݵ���
            strHeadParam<<strTmp;
            if (DataNum) strData<<m_PhasePropSpecDlg[id]->GetPlotData();            
        }  

        tmpPhasePropSpecInps.strName="PHASEPROP";
        tmpPhasePropSpecInps.strType="SPEC";
        tmpPhasePropSpecInps.strHeadParam=strHeadParam;
        tmpPhasePropSpecInps.strData=strData;
       // //-----����
       strHeadParam.clear();strData.clear();
       for (int i=0; i<nRow; i++){ 
           tmpBtn=(QPushButton *)m_TableWidget->cellWidget(i,COL_DAORE);
           if (tmpBtn==NULL) continue;;
           int id=m_SetBtnDaoRe.key(tmpBtn);
           if (id==-1)continue;;
           if (m_PhasePropCondDlg.find(id)==m_PhasePropCondDlg.end()){continue;}

           int DataNum=m_PhasePropCondDlg[id]->GetDataNum();
           QString strTmp;
           strTmp = tr("%1").arg(i+1)+","+//����
               tr("%1").arg(DataNum);//���ݵ���
           strHeadParam<<strTmp;
           if (DataNum) strData<<m_PhasePropCondDlg[id]->GetPlotData();            
       }  

       tmpPhasePropCondInps.strName="PHASEPROP";
       tmpPhasePropCondInps.strType="COND";
       tmpPhasePropCondInps.strHeadParam=strHeadParam;
       tmpPhasePropCondInps.strData=strData;
    }
   //
    
    //--����ǿ�Ȳ���
    if(m_gbBoxTab2->isChecked()==true)
    {
        tmpPhaseYSInps.strName="PHASEYS";
        tmpPhaseYSInps.strHeadParam= m_EditTab2P11->text()+","+m_EditTab2P12->text()+","+m_EditTab2P13->text()+","+
            m_EditTab2P21->text()+","+m_EditTab2P22->text()+","+m_EditTab2P23->text()+","+                         
            m_EditTab2P31->text()+","+m_EditTab2P32->text()+","+m_EditTab2P33->text()+
            m_EditTab2P41->text()+","+m_EditTab2P42->text()+","+m_EditTab2P43->text();                               

    }
 //--�����ߴ����
   if(m_CheckBoxCrystalSize->isChecked())
   {
        tmpPhaseGSInps.strName="PHASEGS";
        tmpPhaseGSInps.strHeadParam=m_EditTab1P1->text()+","+m_EditTab1P2->text()+","+m_EditTab1P3->text();
   }
   TotalTTTInps.PhaseCurveInps=tmpChangePointInps;
   TotalTTTInps.PhaseBalanceInps=tmpPhaseBalanceInps;
   TotalTTTInps.PhaseElateHeatInps=tmpPhaseElateHeatInps;
   TotalTTTInps.PhaseHardInps=tmpPhaseHardInps;
   TotalTTTInps.PhaseCtrolInps=tmpPhaseCtrolInps;
   TotalTTTInps.IncubatePeriodInps=tmpIncuPeriodInps;
   TotalTTTInps.PhasePropCondInps=tmpPhasePropCondInps;
   TotalTTTInps.PhasePropSpecInps=tmpPhasePropSpecInps;
   TotalTTTInps.PhaseYSInps=tmpPhaseYSInps;
   TotalTTTInps.PhaseGSInps=tmpPhaseGSInps;
   return TotalTTTInps;
}


void QHPTTTDlg::SetBtnSLot()
{
    //get position
    QPushButton *btn=(QPushButton*) sender();
    int x=btn->mapToParent(QPoint(0,0)).x();
    int y=btn->mapToParent(QPoint(0,0)).y();
    QModelIndex index=m_TableWidget->indexAt(QPoint(x,y));

    int iRow=index.row();
    int iCol=index.column();


    if (iRow<0 || iCol<0) return;
    QPushButton *tmpBtn = (QPushButton*)m_TableWidget->cellWidget(iRow, iCol);
    switch(iCol){
    case COL_CHANGEPOINT:{
        int id=m_SetBtnChangePoint.key(tmpBtn);
        if (id==-1)return;
        if (m_changePointDlg.find(id)!=m_changePointDlg.end()){
            if (m_changePointDlg[id]->exec()){
                m_changePointDlg.insert(id,m_changePointDlg[id]);
                tmpBtn->setText(tr("�༭"));
            }
        }else{//new dlg
            QChangePointDialog *NewDlg = new QChangePointDialog(this);
            if (NewDlg->exec()){
                m_changePointDlg.insert(id,NewDlg);
                tmpBtn->setText(tr("�༭"));
            }
        }
       }break;
    case COL_PHASEBALANCE:{
        int id=m_SetBtnPhaseBalance.key(tmpBtn);
        if (id==-1)return;
        if (m_phaseBalanceDlg.find(id)!=m_phaseBalanceDlg.end()){
            if (m_phaseBalanceDlg[id]->exec()){
                m_phaseBalanceDlg.insert(id,m_phaseBalanceDlg[id]);
                tmpBtn->setText(tr("�༭"));
            }
        }else{//new dlg
            QPhaseBalanceDialog *NewDlg = new QPhaseBalanceDialog(this);
			QStringList strList;
			strList<<tr("�¶�")<<tr("����");
			NewDlg->m_TCurveDataPlot->ChangeLable(strList);
            if (NewDlg->exec()){
                m_phaseBalanceDlg.insert(id,NewDlg);
                tmpBtn->setText(tr("�༭"));
            }
        }
       }break;
    case COL_YUNYUQI:{
        int id=m_SetBtnYunYuQi.key(tmpBtn);
        if (id==-1)return;
        if (m_IncubationPeriodDlg.find(id)!=m_IncubationPeriodDlg.end()){
            if (m_IncubationPeriodDlg[id]->exec()){
                m_IncubationPeriodDlg.insert(id,m_IncubationPeriodDlg[id]);
                tmpBtn->setText(tr("�༭"));
            }
        }else{//new dlg
            QIncubationPeriod *NewDlg = new QIncubationPeriod(this);
            if (NewDlg->exec()){
                m_IncubationPeriodDlg.insert(id,NewDlg);
                tmpBtn->setText(tr("�༭"));
            }
        }
       }break;
    case COL_DAORE:{//����
        int id=m_SetBtnDaoRe.key(tmpBtn);
        if (id==-1)return;
        if (m_PhasePropCondDlg.find(id)!=m_PhasePropCondDlg.end()){
            if(m_PhasePropCondDlg[id]->exec()){
               m_PhasePropCondDlg.insert(id,m_PhasePropCondDlg[id]);
               tmpBtn->setText(tr("�༭"));
            }
        }else{//new dlg
            QPhasePropDialog *NewDlg = new QPhasePropDialog(this);
            NewDlg->setWindowTitle(tr("����"));
            QStringList strList;
            strList<<tr("�¶�")<<tr("����ֵ");
            NewDlg->m_TCurveDataPlot->ChangeLable(strList);
            if(NewDlg->exec()){
                m_PhasePropCondDlg.insert(id,NewDlg);
                tmpBtn->setText(tr("�༭"));
            }
        }
      }break;
    case COL_BIRE:{//����
        int id=m_SetBtnBiRe.key(tmpBtn);
        if (id==-1)return;
        if (m_PhasePropSpecDlg.find(id)!=m_PhasePropSpecDlg.end()){
            if (m_PhasePropSpecDlg[id]->exec()){
                m_PhasePropSpecDlg.insert(id,m_PhasePropSpecDlg[id]);
                tmpBtn->setText(tr("�༭"));
            }
        }else{//new dlg
            QPhasePropDialog *NewDlg = new QPhasePropDialog(this);
            NewDlg->setWindowTitle(tr("����"));
            QStringList strList;
            strList<<tr("�¶�")<<tr("����ֵ");
            NewDlg->m_TCurveDataPlot->ChangeLable(strList);
            if(NewDlg->exec()){
                m_PhasePropSpecDlg.insert(id,NewDlg);
                tmpBtn->setText(tr("�༭"));
            }
        }
      }break;    
    default:break;
    }
}


//����ȡ��INP������ʾ�ڵ�ǰ��TTT��
void QHPTTTDlg::SetInpData(TTTSetDataS DATA)
{
    if (DATA.iPhaseNum<=0){//���TTT����������
        /*int nRow = m_TableWidget->rowCount();
        int nCol = m_TableWidget->columnCount();
        for (int tt=0;tt<nRow;tt++){
         m_TableWidget->removeRow(0);
        }*/
       
        return;
    }
        
    int iPhaseNum=DATA.iPhaseNum;
//--���������
    int nRow=m_TableWidget->rowCount();
    for (int i=0;i<nRow;i++){
         m_TableWidget->removeRow(i);
    }
    m_SetBtnChangePoint.clear();
//--����������   
    for (int i=0;i<iPhaseNum;i++){
        m_TableWidget->setRowCount(i+1);
        TTTAddLine(i);
    }
    
//--д�������ֲ���(��ʼֵ)
    for (int i=0;i<=iPhaseNum;i++)
    {
       if(DATA.strInitVal.find(i)==DATA.strInitVal.end())continue;
       QString str=DATA.strInitVal[i];
       if (i==0)continue;
       m_TableWidget->item(i-1,COL_INIT)->setText(str);//setItem(i,COL_INIT,tmpItem2);
    }
//--д��Ǳ��(����1,2,3,4,������)
    for (int i=0;i<=iPhaseNum;i++)
    {
        if(DATA.strLatenTheat.find(i)==DATA.strLatenTheat.end())continue;
        QString str=DATA.strLatenTheat[i];
        if (i==0)continue;
        m_TableWidget->item(i-1,COL_QIANRE)->setText(str);//setItem(i,COL_INIT,tmpItem2);
    }
//--д��Ӳ��(����1,2,3,4,������)
    for (int i=0;i<=iPhaseNum;i++)
    {
        if(DATA.strHard.find(i)==DATA.strHard.end())continue;
        QString str=DATA.strHard[i];
        if (i==0)continue;
        m_TableWidget->item(i-1,COL_HARD)->setText(str);//setItem(i,COL_INIT,tmpItem2);
    }

//--д����ƽ��
    for (int i=0;i<=iPhaseNum;i++)
    {
        if (DATA.PhaseBalance.find(i)==DATA.PhaseBalance.end())continue;
        PhaseBaseItem tmpBaseItem;
        tmpBaseItem=DATA.PhaseBalance[i];
        if (i==0)continue;
        QPushButton *tmpBtn = (QPushButton*)m_TableWidget->cellWidget(i-1, COL_PHASEBALANCE);
        int id=m_SetBtnPhaseBalance.key(tmpBtn);
        if (id==-1)return;
        if (m_phaseBalanceDlg.find(id)!=m_phaseBalanceDlg.end()){
            m_phaseBalanceDlg[id]->m_TCurveDataPlot->SetData(tmpBaseItem.iNum,tmpBaseItem.strData);
            m_phaseBalanceDlg.insert(id,m_phaseBalanceDlg[id]);
            tmpBtn->setText(tr("�༭"));
        }else{//new dlg
            QPhaseBalanceDialog *NewDlg = new QPhaseBalanceDialog(this);
            QStringList strList;
            strList<<tr("�¶�")<<tr("����");
            NewDlg->m_TCurveDataPlot->ChangeLable(strList);
            NewDlg->m_TCurveDataPlot->SetData(tmpBaseItem.iNum,tmpBaseItem.strData);
            //�������ݱ������ǰ������
            m_phaseBalanceDlg.insert(id,NewDlg);
            tmpBtn->setText(tr("�༭"));
        }
    }
//--д�����
    for (int i=0;i<=iPhaseNum;i++)
    {
        if (DATA.PhasePropSpec.find(i)==DATA.PhasePropSpec.end())continue;
        PhaseBaseItem tmpBaseItem;
        tmpBaseItem=DATA.PhasePropSpec[i];
        if (i==0)continue;
        QPushButton *tmpBtn = (QPushButton*)m_TableWidget->cellWidget(i-1, COL_BIRE);
        int id=m_SetBtnBiRe.key(tmpBtn);
        if (id==-1)return;
        if (m_PhasePropSpecDlg.find(id)!=m_PhasePropSpecDlg.end()){
            m_PhasePropSpecDlg[id]->m_TCurveDataPlot->SetData(tmpBaseItem.iNum,tmpBaseItem.strData);
            m_PhasePropSpecDlg.insert(id,m_PhasePropSpecDlg[id]);
            tmpBtn->setText(tr("�༭"));
        }else{//new dlg
            QPhasePropDialog *NewDlg = new QPhasePropDialog(this);
            NewDlg->setWindowTitle(tr("����"));
            QStringList strList;
            strList<<tr("�¶�")<<tr("����ֵ");
            NewDlg->m_TCurveDataPlot->ChangeLable(strList);
            NewDlg->m_TCurveDataPlot->SetData(tmpBaseItem.iNum,tmpBaseItem.strData);
            //�������ݱ������ǰ������
            m_PhasePropSpecDlg.insert(id,NewDlg);
            tmpBtn->setText(tr("�༭"));
        }
    }

//--д�뵼��
    for (int i=0;i<=iPhaseNum;i++)
    {
        if (DATA.PhasePropCond.find(i)==DATA.PhasePropCond.end())continue;
        PhaseBaseItem tmpBaseItem;
        tmpBaseItem=DATA.PhasePropCond[i];
        if (i==0)continue;
        QPushButton *tmpBtn = (QPushButton*)m_TableWidget->cellWidget(i-1, COL_DAORE);
        int id=m_SetBtnDaoRe.key(tmpBtn);
        if (id==-1)return;
        if (m_PhasePropCondDlg.find(id)!=m_PhasePropCondDlg.end()){
            m_PhasePropCondDlg[id]->m_TCurveDataPlot->SetData(tmpBaseItem.iNum,tmpBaseItem.strData);
            m_PhasePropCondDlg.insert(id,m_PhasePropCondDlg[id]);
            tmpBtn->setText(tr("�༭"));
        }else{//new dlg
            QPhasePropDialog *NewDlg = new QPhasePropDialog(this);
            NewDlg->setWindowTitle(tr("����"));
            QStringList strList;
            strList<<tr("�¶�")<<tr("����ֵ");
            NewDlg->m_TCurveDataPlot->ChangeLable(strList);
            NewDlg->m_TCurveDataPlot->SetData(tmpBaseItem.iNum,tmpBaseItem.strData);
            //�������ݱ������ǰ������
            m_PhasePropCondDlg.insert(id,NewDlg);
            tmpBtn->setText(tr("�༭"));
        }
    }

//--д��������
    for (int i=0;i<=iPhaseNum;i++)
    {
        if (DATA.PhaseIP.find(i)==DATA.PhaseIP.end())continue;
        PhaseBaseItem tmpBaseItem;
        tmpBaseItem=DATA.PhaseIP[i];
        if (i==0)continue;
        QPushButton *tmpBtn = (QPushButton*)m_TableWidget->cellWidget(i-1, COL_YUNYUQI);
        int id=m_SetBtnYunYuQi.key(tmpBtn);
        if (id==-1)return;
        if (m_IncubationPeriodDlg.find(id)!=m_IncubationPeriodDlg.end()){
            m_IncubationPeriodDlg[id]->m_TCurveDataPlot->SetData(tmpBaseItem.iNum,tmpBaseItem.strData);
            m_IncubationPeriodDlg.insert(id,m_IncubationPeriodDlg[id]);
            tmpBtn->setText(tr("�༭"));
        }else{//new dlg
            QIncubationPeriod *NewDlg = new QIncubationPeriod(this);
            QStringList strList;
            strList<<tr("�¶�")<<tr("ʱ��");
            NewDlg->m_TCurveDataPlot->ChangeLable(strList);
            NewDlg->m_TCurveDataPlot->SetData(tmpBaseItem.iNum,tmpBaseItem.strData);
            //�������ݱ������ǰ������
            m_IncubationPeriodDlg.insert(id,NewDlg);
            tmpBtn->setText(tr("�༭"));
        }
    }
    //--д��ת�����
    for (int i=0;i<=iPhaseNum;i++)
    {
        if (DATA.PhaseCurve.find(i)==DATA.PhaseCurve.end()) continue;
        QMap<int ,PhaseChangeTabS> TmpPhaseChangeTab;
        TmpPhaseChangeTab=DATA.PhaseCurve[i];
        //QMap<int, PhaseChangeTabS>::Iterator it = TmpPhaseChangeTab.begin();
        
        if (i==0)continue;
        QPushButton *tmpBtn = (QPushButton*)m_TableWidget->cellWidget(i-1, COL_CHANGEPOINT);
        int id=m_SetBtnChangePoint.key(tmpBtn);
        if (id==-1)return;
        if (m_changePointDlg.find(id)!=m_changePointDlg.end()){
            m_changePointDlg[id]->setData(TmpPhaseChangeTab);
            m_changePointDlg.insert(id,m_changePointDlg[id]);
            tmpBtn->setText(tr("�༭"));
        }else{//new dlg
            QChangePointDialog *NewDlg = new QChangePointDialog(this);
            NewDlg->setData(TmpPhaseChangeTab);
            m_changePointDlg.insert(id,NewDlg);
            tmpBtn->setText(tr("�༭"));
        }
    }
    //--д������ǿ��
    QStringList strYSList=DATA.strYS.split(",");
    if (strYSList.size()==12)
    {
        m_EditTab2P11->setText(strYSList.at(0).simplified());
        m_EditTab2P12->setText(strYSList.at(1).simplified());
        m_EditTab2P13->setText(strYSList.at(2).simplified());

        m_EditTab2P21->setText(strYSList.at(3).simplified());
        m_EditTab2P22->setText(strYSList.at(4).simplified());
        m_EditTab2P23->setText(strYSList.at(5).simplified());

        m_EditTab2P31->setText(strYSList.at(6).simplified());
        m_EditTab2P32->setText(strYSList.at(7).simplified());
        m_EditTab2P33->setText(strYSList.at(8).simplified());

        m_EditTab2P41->setText(strYSList.at(9).simplified());
        m_EditTab2P42->setText(strYSList.at(10).simplified());
        m_EditTab2P43->setText(strYSList.at(11).simplified());
    }
    //--д�뾧���ߴ�
     QStringList strCrystalList=DATA.strGS.split(",");
     if (strCrystalList.size()==3)
     {
         m_EditTab1P1->setText(strCrystalList.at(0).simplified());
         m_EditTab1P2->setText(strCrystalList.at(1).simplified());
         m_EditTab1P3->setText(strCrystalList.at(2).simplified());
     }

    //--д���ȴ�����ʽ
      QStringList strPhaseControl=DATA.strPhaseCtrol.split(",");
      int strCntrAnalyStyle,
              strCntrLatentheat,
              strCntrProp,
              strCntrEquilibrium,
              strCntrTTT;//StrTTT,Ĭ������Ϊ1
      if (strPhaseControl.size()==5)
      {
          strCntrAnalyStyle=strPhaseControl.at(0).toInt();
          strCntrLatentheat=strPhaseControl.at(1).toInt();
          strCntrProp=strPhaseControl.at(2).toInt();
          strCntrEquilibrium=strPhaseControl.at(3).toInt();
          strCntrTTT=strPhaseControl.at(4).toInt();
      }

      if (strCntrAnalyStyle==1){//������������
          m_CheckBoxGestation->setChecked(false);
      }else if(strCntrAnalyStyle==2){//����������
          m_CheckBoxGestation->setChecked(true);
      }else if(strCntrAnalyStyle==3){//ĸ���ྦྷ���ߴ�
          m_CheckBoxCrystalSize->setChecked(true);
      }
        
      if (strCntrLatentheat==1)
      {
          m_CheckBoxQianRe->setChecked(true);
          QianReSlot();
      }
      if (strCntrProp==1)
      {
          m_CheckBoxReWuXing->setChecked(true);
          ReWuXingSlot();
      }
}

//--��TTT���Խ��б���
void QHPTTTDlg::ImportInpTTT(ReadInpResultS TTTMaterial)
{
    int nsize = TTTMaterial.TmpHPphaseCurveInps.size();
    int iPhaseNum=0;//������
    TTTSetDataS SetDataS;
    //--������
    QMap<int,QMap<int,PhaseChangeTabS> > AllPhaseCurve;
    QMap<int,PhaseChangeTabS> PhaseCurve,PhaseCurveOld;
    PhaseChangeTabS TmpPhaseCurve;
    //--��ʼֵ
    QMap<int ,QString> strInitVal;
    QString strInitValOld;

    int iBaseNum=0;
    int iPhaseOld=1;//ÿ�ξ�Ϊ��1��ʼ
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
            int iTabIndex=strListHeadParam.at(1).toInt();//���ߺ�
            int iDataNum=strListHeadParam.at(2).toInt();//���ݵ���
            QString TmpstrInitVal=strListHeadParam.at(4);//���ݳ�ֵ

            if (iPhaseNum!=iPhaseOld&&jjphaseSizeCnt){
                //strListHeadParam=TTTMaterial.TmpHPphaseCurveInps.at(i).strHeadParam.at(jjphaseSizeCnt-1).split(",",QString::SkipEmptyParts);
                strInitVal.insert(iPhaseOld,strInitValOld);
                AllPhaseCurve.insert(iPhaseOld,PhaseCurveOld);
                PhaseCurve.clear();
                iPhaseOld=iPhaseNum;
            }
            //--
            TmpPhaseCurve.iNum=iDataNum;
            TmpPhaseCurve.strChangeP=strListHeadParam.at(3);//ת�����
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
    //--Ǳ��
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
    //--Ӳ��
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
    //--����ǿ��
    QString strMapYS;
    nsize=TTTMaterial.TmpHPPhaseYSInps.size();
    for (int i=0;i<nsize;i++)
    {
        strMapYS=TTTMaterial.TmpHPPhaseYSInps.at(i).strHeadParam;
    }
    //--�����ߴ�
    QString strMapGS;
    nsize=TTTMaterial.TmpHPPhaseGSInps.size();
    for (int i=0;i<nsize;i++)
    {
        strMapGS=TTTMaterial.TmpHPPhaseGSInps.at(i).strHeadParam;
    }

    //--�ȴ�����ʽ
    QString strMapPhaseCtrol;
    nsize=TTTMaterial.TmpHPphaseCtrolInps.size();
    for (int i=0;i<nsize;i++)
    {
        strMapPhaseCtrol=TTTMaterial.TmpHPphaseCtrolInps.at(i).strHeadParam;
    }

     QMap<int,PhaseBaseItem> mapPhaseBSS;//--��ƽ��
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
     //--������(δ���)
     QMap<int,PhaseBaseItem> mapPhaseIP;//������
     mapPhaseIP=ProcessMapData(TTTMaterial.TmpHPincubatePeriodInps);
     QMap<int,PhaseBaseItem> mapPhasePropSpec;//����
     mapPhasePropSpec=ProcessMapData(TTTMaterial.TmpHPPhasePropSpecInps);
     QMap<int,PhaseBaseItem> mapPhasePropCond;//����
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

    this->SetInpData(SetDataS);
}

QMap<int,PhaseBaseItem> QHPTTTDlg::ProcessMapData(QList<HPphaseBalanceInps> BalanceList)
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