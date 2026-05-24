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
    //m_ImageFormula = new QPushButton(QIcon(":/images/NewIcon.png"), tr("新"),m_Tab1);
    m_CheckBoxGestation = new QCheckBox(tr("考虑孕育期"),m_Tab1);
    m_CheckBoxCrystalSize = new QCheckBox(tr("母子晶粒尺寸"),m_Tab1);
    m_CheckBoxCalculate = new QCheckBox(tr("计算孕育"),m_Tab1);
    
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
    m_gbBoxTab2->setTitle(tr("屈服强度"));
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
    labTab2P3= new QLabel(tr("τ:"));
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
    m_CheckBoxQianRe = new QCheckBox(tr("潜热"));
    m_CheckBoxReWuXing = new QCheckBox(tr("热物性"));
    m_CheckBoxCalcChg = new QCheckBox(tr("计算变形"));
    QVBoxLayout *Gridlayout3 = new QVBoxLayout(m_Tab3);
    Gridlayout3->addWidget(m_CheckBoxQianRe);
    Gridlayout3->addWidget(m_CheckBoxReWuXing);
    Gridlayout3->addWidget(m_CheckBoxCalcChg);
    Gridlayout3->addStretch();
    
    m_tabView = new QTabWidget();
    m_tabView->addTab(m_Tab1, tr("分析类型"));
    m_tabView->addTab(m_Tab2, tr("屈服强度"));
    m_tabView->addTab(m_Tab3, tr("其他"));
   

    QScrollArea *areaInfomation_ = new QScrollArea();
    areaInfomation_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    areaInfomation_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    areaInfomation_->setWidgetResizable(true);
    areaInfomation_->setWidget(m_tabView);//setWidgetResizable(true);

    QStringList strList;
    //strList.append(tr("相"));
    strList<<tr("转变分数")<<tr("初始值")<<tr("相平衡")<<tr("孕育期")<<tr("导热")<<tr("比热")<<tr("潜热")<<tr("硬度");
    m_TableWidget = new QMyTableWidget(this);//QTableWidget();
    m_TableWidget->setColumnCount(strList.size());
    m_TableWidget->setRowCount(0);
    m_TableWidget->verticalHeaderItem(1);
    //QLabel *lab_w=new QLabel(tr("相"));
    //lab_w->move(1,5);
    //lab_w->installEventFilter(this);
   
    
    m_TableWidget->setHorizontalHeaderLabels(strList);
    for (int j=0;j<strList.size();j++){
       m_TableWidget->horizontalHeader()->setResizeMode(j,QHeaderView::ResizeToContents);
    }
   
    //--combox
    for (int i=0; i<0; i++){
        QPushButton *tmpBtn = new QPushButton(tr("新建"));
        m_TableWidget->setCellWidget(i,COL_CHANGEPOINT,tmpBtn);
        m_SetBtnChangePoint.insert(i,tmpBtn);//push_back(tmpBtn);//转变分数
        //connect(tmpBtn,SIGNAL(RightClicked(int,int)),this,SLOT(comBoxRChangePointSlot(int,int)));
        //connect(tmpBtn,SIGNAL(clicked(int,int)),this,SLOT(comBoxNewChangePointSlot(int,int)));
        connect(tmpBtn,SIGNAL(clicked()),this,SLOT(SetBtnSLot())); 
       
        tmpBtn = new QPushButton(tr("新建"));
        m_TableWidget->setCellWidget(i,COL_PHASEBALANCE,tmpBtn);
        m_SetBtnPhaseBalance.insert(i,tmpBtn);//push_back(tmpBtn);//相位平衡、
        //connect(tmpBtn,SIGNAL(RightClicked(int,int)),this,SLOT(comBoxRPhaseSlot(int,int)));
       // connect(tmpBtn,SIGNAL(clicked(int,int)),this,SLOT(comBoxNewPhaseSlot(int,int)));
        //connect(tmpBtn,SIGNAL(clicked()),this,SLOT(ChangePointSetBtnSLot()));
        connect(tmpBtn,SIGNAL(clicked()),this,SLOT(SetBtnSLot()));  

        tmpBtn = new QPushButton(tr("新建"));
        m_TableWidget->setCellWidget(i,COL_YUNYUQI,tmpBtn);
        m_SetBtnYunYuQi.insert(i,tmpBtn);//push_back(tmpBtn);//孕育期
        connect(tmpBtn,SIGNAL(clicked()),this,SLOT(SetBtnSLot())); 
        tmpBtn->setEnabled(false);

        tmpBtn = new QPushButton(tr("新建"));
        m_TableWidget->setCellWidget(i,COL_DAORE,tmpBtn);
        m_SetBtnDaoRe.insert(i,tmpBtn);//push_back(tmpBtn);//导热
        connect(tmpBtn,SIGNAL(clicked()),this,SLOT(SetBtnSLot())); 
        tmpBtn->setEnabled(false);

        tmpBtn = new QPushButton(tr("新建"));
        m_TableWidget->setCellWidget(i,COL_BIRE,tmpBtn);
        m_SetBtnBiRe.insert(i,tmpBtn);//push_back(tmpBtn);//比热
        connect(tmpBtn,SIGNAL(clicked()),this,SLOT(SetBtnSLot()));        
        tmpBtn->setEnabled(false);


        ////相
        //QTableWidgetItem *tmpItem2=new QTableWidgetItem(QString("%1").arg(i+1));
        //m_TableWidget->setItem(i,0,tmpItem2);
        ////初始值
        //tmpItem2=new QTableWidgetItem(tr("10%"));
        //m_TableWidget->setItem(i,2,tmpItem2);

        //相
        //QTableWidgetItem *tmpItem2=new QTableWidgetItem(QString("%1").arg(i+1));
        //m_TableWidget->setItem(i,0,tmpItem2);
        //初始值
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
 
    m_AddLineBtn = new QPushButton(tr("增加"));
    m_DelLineBtn = new QPushButton(tr("删除"));
    m_InsertLineBtn = new QPushButton(tr("插入"));
	m_ImportLineBtn = new QPushButton(tr("导入"));
	m_ExportLineBtn = new QPushButton(tr("导出"));
    m_OkBtn = new QPushButton(tr("确定"));

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

    connect(m_AddLineBtn,SIGNAL(clicked()),this,SLOT(AddLineBtnSlot()));//增加
    //connect(m_InsertLineBtn,SIGNAL(clicked()),this,SLOT(InsertLineBtnSlot()));//插入
    connect(m_DelLineBtn,SIGNAL(clicked()),this,SLOT(DelLineBtnSlot()));//删除
	connect(m_ImportLineBtn,SIGNAL(clicked()),this,SLOT(ImportLineBtnSlot()));//导入
    connect(m_ExportLineBtn,SIGNAL(clicked()),this,SLOT(ExportLineBtnSlot()));//导出
    connect(m_OkBtn,SIGNAL(clicked()),this,SLOT(OkBtnSlot()));//确定
    //m_ImageFormula->setFlat(true);
    m_CheckBoxCrystalSize->setEnabled(false);
    m_CheckBoxCalculate->setEnabled(false);
   // m_gbBox->hide();
}

void QHPTTTDlg::TTTAddLine(int nCurRow)
{
    int nRow = m_TableWidget->rowCount();
    if (nRow<=0)return;

    QPushButton *tmpBtn = new QPushButton(tr("新建"));
    m_TableWidget->setCellWidget(nCurRow,COL_CHANGEPOINT,tmpBtn);
    m_SetBtnChangePoint.insert(nCurRow,tmpBtn);//push_back(tmpBtn);//转变分数
    connect(tmpBtn,SIGNAL(clicked()),this,SLOT(SetBtnSLot()));

    tmpBtn = new QPushButton(tr("新建"));
    m_TableWidget->setCellWidget(nCurRow,COL_PHASEBALANCE,tmpBtn);
    m_SetBtnPhaseBalance.insert(nCurRow,tmpBtn);//push_back(tmpBtn);//相位平衡、
    connect(tmpBtn,SIGNAL(clicked()),this,SLOT(SetBtnSLot())); 

    tmpBtn = new QPushButton(tr("新建"));
    m_TableWidget->setCellWidget(nCurRow,COL_YUNYUQI,tmpBtn);
    m_SetBtnYunYuQi.insert(nCurRow,tmpBtn);//push_back(tmpBtn);//孕育期
    connect(tmpBtn,SIGNAL(clicked()),this,SLOT(SetBtnSLot())); 
    tmpBtn->setEnabled(m_CheckBoxGestation->isChecked());

    tmpBtn = new QPushButton(tr("新建"));
    m_TableWidget->setCellWidget(nCurRow,COL_DAORE,tmpBtn);
    m_SetBtnDaoRe.insert(nCurRow,tmpBtn);//push_back(tmpBtn);//导热
    connect(tmpBtn,SIGNAL(clicked()),this,SLOT(SetBtnSLot())); 
    tmpBtn->setEnabled(m_CheckBoxReWuXing->isChecked());

    tmpBtn = new QPushButton(tr("新建"));
    m_TableWidget->setCellWidget(nCurRow,COL_BIRE,tmpBtn);
    m_SetBtnBiRe.insert(nCurRow,tmpBtn);//push_back(tmpBtn);//比热
    connect(tmpBtn,SIGNAL(clicked()),this,SLOT(SetBtnSLot())); 
    tmpBtn->setEnabled(m_CheckBoxReWuXing->isChecked());

    //初始值
    QTableWidgetItem *tmpItem2=new QTableWidgetItem(tr("0.01"));
    m_TableWidget->setItem(nCurRow,COL_INIT,tmpItem2);

    tmpItem2=new QTableWidgetItem(tr("0.01"));//潜热
    m_TableWidget->setItem(nCurRow,COL_QIANRE,tmpItem2);
    tmpItem2=new QTableWidgetItem(tr("0.01"));//硬度
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
    Table新增行时，对改行combox进行ITEM赋值；
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
    tmpCombox->addItem("新建");//增加
    return false;
}
/*
    对Table中COMBOX设置true/false；
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
//--增加
void QHPTTTDlg::AddLineBtnSlot()
{
    int nRow = m_TableWidget->rowCount();
    m_TableWidget->setRowCount(nRow+1);
    TTTAddLine(nRow);
}
//--插入
void QHPTTTDlg::InsertLineBtnSlot()
{
    int curRow = m_TableWidget->currentRow();
    m_TableWidget->insertRow(curRow);
    TTTAddLine(curRow);
}
//--删除
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
		qDebug()<<"打开文件时出错";
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
			if(Reader.name()=="相变数据") 
				break;
		}
		if (type == QXmlStreamReader::StartElement)
		{
			if (strtmp=="相变数据")
			{
				DATA=ParseEntry(Reader);
			}
		}	
	}
	SetInpData(DATA);
	file.close();
	if (Reader.hasError())//解析出错时
	{
	}
}
//导出
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
	writer.writeStartElement("相变数据");
	int nRow = m_TableWidget->rowCount();
	int nCol = m_TableWidget->columnCount();
	QPushButton *tmpBtn=NULL;
	QStringList strHeadParam;
	QStringList strData; 
	QString     strData1;  
	for (int i=0; i<nRow; i++){
		writer.writeStartElement("第"+QString::number(i+1,10)+"个相的数据");
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
						writer.writeStartElement("TTTCCT曲线");
						for (int jj=0; jj<iTabNum; jj++)
						{
							int DataNum = m_changePointDlg[id]->GetDataNum(jj);
							QString strTmp = tr("%1").arg(i+1)+","+//相序
								tr("%1").arg(jj+1)+","+//
								tr("%1").arg(DataNum)+","+//数据点数
								m_changePointDlg[id]->GetCurveThPoint(jj)+","+//分数
								m_TableWidget->item(i,COL_INIT)->text();
							writer.writeTextElement("第"+QString::number(jj+1,10)+"条曲线", strTmp);
							if (DataNum){
								strData1=m_changePointDlg[id]->GetPlotData(jj);
								strData=strData1.split(";");
								int IIK=strData.size();
								for(int IJK=0;IJK<IIK;IJK++){;
								writer.writeTextElement("曲线值", strData.at(IJK));
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
						strTmp = tr("%1").arg(i+1)+","+//相序
							tr("%1").arg(DataNum);//数据点数
						writer.writeStartElement("相平衡");
						writer.writeTextElement("第"+QString::number(i+1,10)+"个相平衡", strTmp);
						if (DataNum) {
							strData1=m_phaseBalanceDlg[id]->GetPlotData();
							strData=strData1.split(";");
							int IIK=strData.size();
							for(int IJK=0;IJK<IIK;IJK++){
								writer.writeTextElement("相平衡值", strData.at(IJK));
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
						strTmp = tr("%1").arg(i+1)+","+//相序
							tr("%1").arg(DataNum);//数据点数
						writer.writeStartElement("孕育期");
						writer.writeTextElement("第"+QString::number(i+1,10)+"个相孕育期", strTmp);
						if (DataNum) {
							strData1=m_IncubationPeriodDlg[id]->GetPlotData();
							strData=strData1.split(";");
							int IIK=strData.size();
							for(int IJK=0;IJK<IIK;IJK++){
								writer.writeTextElement("孕育期值", strData.at(IJK));
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
						strTmp = tr("%1").arg(i+1)+","+//相序
							tr("%1").arg(DataNum);//数据点数
						writer.writeStartElement("导热");
						writer.writeTextElement("第"+QString::number(i+1,10)+"个相导热系数", strTmp);
						if (DataNum) {
							strData1=m_PhasePropCondDlg[id]->GetPlotData();
							strData=strData1.split(";");
							int IIK=strData.size();
							for(int IJK=0;IJK<IIK;IJK++){
								writer.writeTextElement("导热系数值", strData.at(IJK));
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
						strTmp = tr("%1").arg(i+1)+","+//相序
							tr("%1").arg(DataNum);//数据点数
						writer.writeStartElement("比热");
						writer.writeTextElement("第"+QString::number(i+1,10)+"个相比热", strTmp);
						if (DataNum){
							strData1=m_PhasePropSpecDlg[id]->GetPlotData();
							strData=strData1.split(";");
							int IIK=strData.size();
							for(int IJK=0;IJK<IIK;IJK++){
								writer.writeTextElement("比热值", strData.at(IJK));
							}
						}  
						writer.writeEndElement();
					}
					break;
				case 6:
					{
						QString strDataVal = m_TableWidget->item(i,COL_QIANRE)->text();     
						QString strTmp;
						strTmp = tr("%1").arg(i+1)+","+//相序
							tr("%1").arg(strDataVal);//数值
						writer.writeStartElement("潜热");
						writer.writeTextElement("潜热值", strTmp);    
						writer.writeEndElement();
					}
					break;
				case 7:
					{
						QString strDataVal = m_TableWidget->item(i,COL_HARD)->text();     
						QString strTmp;
						strTmp = tr("%1").arg(i+1)+","+//相序
							tr("%1").arg(strDataVal);//数值
                        writer.writeStartElement("硬度");
						writer.writeTextElement("硬度值", strTmp);
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
//--确定
void QHPTTTDlg::OkBtnSlot()
{
    this->accept();
}
//--考虑孕育期间
void QHPTTTDlg::GestationSlot()
{
    if (m_CheckBoxGestation->isChecked()){//选择考虑孕育期间
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
//--计算孕育如果选择，则不能编辑孕育
void QHPTTTDlg::CalYunyuSlot()
{
	SetTableComboxEnable(COL_YUNYUQI,!m_CheckBoxCalculate->isChecked());
}
//--子母晶粒尺寸
void QHPTTTDlg::CrystalSizeSlot()
{
    if (m_CheckBoxCrystalSize->isChecked()){
        m_gbBox->show();
    }else{
        m_gbBox->hide();
    }
}
//--潜热checkbox
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
//--热物性
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
	int PhaseN,NCurveNumber;//相编号、曲线编号
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
			if(strName.mid(2)=="个相的数据") 
				break;
		}
		if (type == QXmlStreamReader::StartElement)
		{
			while(!reader.atEnd()){
				type = reader.readNext();
				QString strRef = reader.name().toString();
				if(type == QXmlStreamReader::EndElement)
				{
					if(strRef=="TTTCCT曲线"||strRef=="相平衡"||strRef=="导热"||strRef=="比热"||strRef=="潜热"||strRef=="硬度") 
						continue;
				}
				if(type == QXmlStreamReader::EndElement)
				{
					if(strRef.mid(2)=="个相的数据"){ 
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
							if(strRef=="TTTCCT曲线"||strRef=="潜热"||strRef=="硬度")break;
							if(strRef=="相平衡"){
								PDATA.PhaseBalance.insert(PhaseN,PDSB);
								break;
							}else if(strRef=="导热"){
								PDATA.PhasePropCond.insert(PhaseN,PDSB);
								break;
							}else if(strRef=="比热"){
								PDATA.PhasePropSpec.insert(PhaseN,PDSB);
								break;
							}else if(strRef=="孕育期"){
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
											PhaseN=Data11.at(0).toInt();//相序
											NCurveNumber=Data11.at(1).toInt();//曲线序
											TmpPhaseCurve.iNum=Data11.at(2).toInt();//数据点数
											TmpPhaseCurve.strChangeP=Data11.at(3);//曲线分数
											TmpPhaseCurve.strData.clear();
											PDATA.strInitVal.insert(PhaseN,Data11.at(4));//初始值
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
											int PhaseN1=Data22.at(0).toInt();//相序
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
											int PhaseN1=Data22.at(0).toInt();//相序
											PDATA.strHard.insert(PhaseN1,Data22.at(1));
											Data22.clear();}
									}
									break;
								default:;
							}
							if(strRef.mid(2)=="条曲线"&&Data11.at(2).toInt()==0){
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
	if(strRef1=="第")strRef=strRef.mid(2);
	if (strRef=="条曲线"){return 1;
	}else if (strRef=="曲线值"){return 2;
    }else if (strRef=="个相平衡"||strRef=="个相导热系数"||strRef=="个相比热"||strRef=="个相孕育期"){return 3;
	}else if (strRef=="相平衡值"||strRef=="导热系数值"||strRef=="比热值"||strRef=="孕育期值"){return 4;
	}else if (strRef=="潜热值"){return 9;
	}else if (strRef=="硬度值"){return 10;
	}else if (strRef=="TTTCCT曲线"||strRef=="相平衡"||strRef=="导热"||strRef=="比热"||strRef=="硬度"){return 11;
	}else if (strRef==""){return 12;}
	return -1;
}
////--

//--写入到inp中;
HPTTTInps QHPTTTDlg::WriteHPTTTToInp()
{ 
   int nRow = m_TableWidget->rowCount();
   int nCol = m_TableWidget->columnCount();

   HPTTTInps TotalTTTInps;
   HPphaseCurveInps tmpChangePointInps;
   HPphaseBalanceInps tmpPhaseBalanceInps;
   HPincubatePeriodInps tmpIncuPeriodInps;
   HPphaseElateHeatInps tmpPhaseElateHeatInps;//潜热
   HPphaseHardInps tmpPhaseHardInps;//硬度
   HPPhasePropSpecInps tmpPhasePropSpecInps;//比热
   HPPhasePropCondInps tmpPhasePropCondInps;//导热
   HPphaseCtrolInps tmpPhaseCtrolInps;
   HPPhaseYSInps tmpPhaseYSInps;//屈服强度
   HPPhaseGSInps tmpPhaseGSInps;//晶粒尺寸参数

   QStringList strHeadParam;
   QStringList strData; 

   QPushButton *tmpBtn=NULL;

   //-----转变积分
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
           QString strTmp = tr("%1").arg(i+1)+","+//相序
                            tr("%1").arg(jj+1)+","+//
                            tr("%1").arg(DataNum)+","+//数据点数
                            m_changePointDlg[id]->GetCurveThPoint(jj)+","+//分数
                            m_TableWidget->item(i,COL_INIT)->text();       
             strHeadParam<<strTmp;
             if (DataNum)strData<<m_changePointDlg[id]->GetPlotData(jj);  
        }     
    }  
    tmpChangePointInps.strName="PHASECURVE";
    tmpChangePointInps.strPhaseCurveType="TTT";
    tmpChangePointInps.strHeadParam=strHeadParam;
    tmpChangePointInps.strData=strData;

   //-----相平衡
   strHeadParam.clear();strData.clear();
   for (int i=0; i<nRow; i++){ 
       tmpBtn=(QPushButton *)m_TableWidget->cellWidget(i,COL_PHASEBALANCE);
       if (tmpBtn==NULL) continue;;
       int id=m_SetBtnPhaseBalance.key(tmpBtn);
       if (id==-1)continue;;
       if (m_phaseBalanceDlg.find(id)==m_phaseBalanceDlg.end()){continue;}

       int DataNum=m_phaseBalanceDlg[id]->GetDataNum();
       QString strTmp;
       strTmp = tr("%1").arg(i+1)+","+//相序
                tr("%1").arg(DataNum);//数据点数
       strHeadParam<<strTmp;
       if (DataNum) strData<<m_phaseBalanceDlg[id]->GetPlotData();            
   }  
   tmpPhaseBalanceInps.strName="PHASEEQUILIBRIUM";
   tmpPhaseBalanceInps.strHeadParam=strHeadParam;
   tmpPhaseBalanceInps.strData=strData;

   //--热处理方式控制
   QString strCntlAnalyStyle;
   strCntlAnalyStyle ="1";//不考虑孕育期
   if (m_CheckBoxGestation->isChecked())
       strCntlAnalyStyle="2";//考虑孕育期
   if (m_CheckBoxCrystalSize->isChecked())
       strCntlAnalyStyle="3";//母子相晶粒尺寸

   QString strCntrPeriod;
   strCntrPeriod="0";
   if (strCntlAnalyStyle=="1"&&m_CheckBoxCalculate->isChecked())
       strCntrPeriod="1";

   QString strCntrLatentheat;
   strCntrLatentheat="0";//是否考虑潜热
   if (m_CheckBoxQianRe->isChecked())
       strCntrLatentheat="1";

   QString strCntrProp;
   strCntrProp="0";//是否考虑热物性
   if (m_CheckBoxReWuXing->isChecked())
       strCntrProp="1";
   tmpPhaseCtrolInps.strName="PHASECTROL";
   tmpPhaseCtrolInps.strHeadParam = strCntlAnalyStyle+","+strCntrLatentheat+","+
       strCntrProp+","+strCntrPeriod+",0";

   //-----潜热
   strHeadParam.clear();
   if (strCntrLatentheat.toInt()==1)
   {
       for (int i=0; i<nRow; i++){ 
           QString strDataVal = m_TableWidget->item(i,COL_QIANRE)->text();     
           QString strTmp;
           strTmp = tr("%1").arg(i+1)+","+//相序
                    tr("%1").arg(strDataVal);//数值
           strHeadParam<<strTmp;      
       } 
        tmpPhaseElateHeatInps.strName="PHASELATENTHEAT";
        tmpPhaseElateHeatInps.strHeadParam=strHeadParam;
   }
   //--硬度
   strHeadParam.clear();
   if (strCntrLatentheat.toInt()==1)
   {
       for (int i=0; i<nRow; i++){ 
           QString strDataVal = m_TableWidget->item(i,COL_HARD)->text();     
           QString strTmp;
           strTmp = tr("%1").arg(i+1)+","+//相序
               tr("%1").arg(strDataVal);//数值
           strHeadParam<<strTmp;      
       } 
       tmpPhaseHardInps.strName="PHASEHARDNESS";
       tmpPhaseHardInps.strHeadParam=strHeadParam;
   }


   ////-----孕育期
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
           strTmp = tr("%1").arg(i+1)+","+//相序
               tr("%1").arg(DataNum);//数据点数
           strHeadParam<<strTmp;
           if (DataNum) strData<<m_IncubationPeriodDlg[id]->GetPlotData();            
       }  
       tmpIncuPeriodInps.strName="INCUBATIONPERIOD";
       tmpIncuPeriodInps.strHeadParam=strHeadParam;
       tmpIncuPeriodInps.strData=strData;
   }
   

   // //-----比热
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
            strTmp = tr("%1").arg(i+1)+","+//相序
                tr("%1").arg(DataNum);//数据点数
            strHeadParam<<strTmp;
            if (DataNum) strData<<m_PhasePropSpecDlg[id]->GetPlotData();            
        }  

        tmpPhasePropSpecInps.strName="PHASEPROP";
        tmpPhasePropSpecInps.strType="SPEC";
        tmpPhasePropSpecInps.strHeadParam=strHeadParam;
        tmpPhasePropSpecInps.strData=strData;
       // //-----导热
       strHeadParam.clear();strData.clear();
       for (int i=0; i<nRow; i++){ 
           tmpBtn=(QPushButton *)m_TableWidget->cellWidget(i,COL_DAORE);
           if (tmpBtn==NULL) continue;;
           int id=m_SetBtnDaoRe.key(tmpBtn);
           if (id==-1)continue;;
           if (m_PhasePropCondDlg.find(id)==m_PhasePropCondDlg.end()){continue;}

           int DataNum=m_PhasePropCondDlg[id]->GetDataNum();
           QString strTmp;
           strTmp = tr("%1").arg(i+1)+","+//相序
               tr("%1").arg(DataNum);//数据点数
           strHeadParam<<strTmp;
           if (DataNum) strData<<m_PhasePropCondDlg[id]->GetPlotData();            
       }  

       tmpPhasePropCondInps.strName="PHASEPROP";
       tmpPhasePropCondInps.strType="COND";
       tmpPhasePropCondInps.strHeadParam=strHeadParam;
       tmpPhasePropCondInps.strData=strData;
    }
   //
    
    //--屈服强度参数
    if(m_gbBoxTab2->isChecked()==true)
    {
        tmpPhaseYSInps.strName="PHASEYS";
        tmpPhaseYSInps.strHeadParam= m_EditTab2P11->text()+","+m_EditTab2P12->text()+","+m_EditTab2P13->text()+","+
            m_EditTab2P21->text()+","+m_EditTab2P22->text()+","+m_EditTab2P23->text()+","+                         
            m_EditTab2P31->text()+","+m_EditTab2P32->text()+","+m_EditTab2P33->text()+
            m_EditTab2P41->text()+","+m_EditTab2P42->text()+","+m_EditTab2P43->text();                               

    }
 //--晶粒尺寸参数
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
                tmpBtn->setText(tr("编辑"));
            }
        }else{//new dlg
            QChangePointDialog *NewDlg = new QChangePointDialog(this);
            if (NewDlg->exec()){
                m_changePointDlg.insert(id,NewDlg);
                tmpBtn->setText(tr("编辑"));
            }
        }
       }break;
    case COL_PHASEBALANCE:{
        int id=m_SetBtnPhaseBalance.key(tmpBtn);
        if (id==-1)return;
        if (m_phaseBalanceDlg.find(id)!=m_phaseBalanceDlg.end()){
            if (m_phaseBalanceDlg[id]->exec()){
                m_phaseBalanceDlg.insert(id,m_phaseBalanceDlg[id]);
                tmpBtn->setText(tr("编辑"));
            }
        }else{//new dlg
            QPhaseBalanceDialog *NewDlg = new QPhaseBalanceDialog(this);
			QStringList strList;
			strList<<tr("温度")<<tr("分数");
			NewDlg->m_TCurveDataPlot->ChangeLable(strList);
            if (NewDlg->exec()){
                m_phaseBalanceDlg.insert(id,NewDlg);
                tmpBtn->setText(tr("编辑"));
            }
        }
       }break;
    case COL_YUNYUQI:{
        int id=m_SetBtnYunYuQi.key(tmpBtn);
        if (id==-1)return;
        if (m_IncubationPeriodDlg.find(id)!=m_IncubationPeriodDlg.end()){
            if (m_IncubationPeriodDlg[id]->exec()){
                m_IncubationPeriodDlg.insert(id,m_IncubationPeriodDlg[id]);
                tmpBtn->setText(tr("编辑"));
            }
        }else{//new dlg
            QIncubationPeriod *NewDlg = new QIncubationPeriod(this);
            if (NewDlg->exec()){
                m_IncubationPeriodDlg.insert(id,NewDlg);
                tmpBtn->setText(tr("编辑"));
            }
        }
       }break;
    case COL_DAORE:{//导热
        int id=m_SetBtnDaoRe.key(tmpBtn);
        if (id==-1)return;
        if (m_PhasePropCondDlg.find(id)!=m_PhasePropCondDlg.end()){
            if(m_PhasePropCondDlg[id]->exec()){
               m_PhasePropCondDlg.insert(id,m_PhasePropCondDlg[id]);
               tmpBtn->setText(tr("编辑"));
            }
        }else{//new dlg
            QPhasePropDialog *NewDlg = new QPhasePropDialog(this);
            NewDlg->setWindowTitle(tr("导热"));
            QStringList strList;
            strList<<tr("温度")<<tr("导热值");
            NewDlg->m_TCurveDataPlot->ChangeLable(strList);
            if(NewDlg->exec()){
                m_PhasePropCondDlg.insert(id,NewDlg);
                tmpBtn->setText(tr("编辑"));
            }
        }
      }break;
    case COL_BIRE:{//比热
        int id=m_SetBtnBiRe.key(tmpBtn);
        if (id==-1)return;
        if (m_PhasePropSpecDlg.find(id)!=m_PhasePropSpecDlg.end()){
            if (m_PhasePropSpecDlg[id]->exec()){
                m_PhasePropSpecDlg.insert(id,m_PhasePropSpecDlg[id]);
                tmpBtn->setText(tr("编辑"));
            }
        }else{//new dlg
            QPhasePropDialog *NewDlg = new QPhasePropDialog(this);
            NewDlg->setWindowTitle(tr("比热"));
            QStringList strList;
            strList<<tr("温度")<<tr("比热值");
            NewDlg->m_TCurveDataPlot->ChangeLable(strList);
            if(NewDlg->exec()){
                m_PhasePropSpecDlg.insert(id,NewDlg);
                tmpBtn->setText(tr("编辑"));
            }
        }
      }break;    
    default:break;
    }
}


//将读取的INP数据显示在当前的TTT中
void QHPTTTDlg::SetInpData(TTTSetDataS DATA)
{
    if (DATA.iPhaseNum<=0){//清除TTT中所有数据
        /*int nRow = m_TableWidget->rowCount();
        int nCol = m_TableWidget->columnCount();
        for (int tt=0;tt<nRow;tt++){
         m_TableWidget->removeRow(0);
        }*/
       
        return;
    }
        
    int iPhaseNum=DATA.iPhaseNum;
//--清除所有项
    int nRow=m_TableWidget->rowCount();
    for (int i=0;i<nRow;i++){
         m_TableWidget->removeRow(i);
    }
    m_SetBtnChangePoint.clear();
//--创建几个项   
    for (int i=0;i<iPhaseNum;i++){
        m_TableWidget->setRowCount(i+1);
        TTTAddLine(i);
    }
    
//--写入相变积分部分(初始值)
    for (int i=0;i<=iPhaseNum;i++)
    {
       if(DATA.strInitVal.find(i)==DATA.strInitVal.end())continue;
       QString str=DATA.strInitVal[i];
       if (i==0)continue;
       m_TableWidget->item(i-1,COL_INIT)->setText(str);//setItem(i,COL_INIT,tmpItem2);
    }
//--写入潜热(相数1,2,3,4,包含四)
    for (int i=0;i<=iPhaseNum;i++)
    {
        if(DATA.strLatenTheat.find(i)==DATA.strLatenTheat.end())continue;
        QString str=DATA.strLatenTheat[i];
        if (i==0)continue;
        m_TableWidget->item(i-1,COL_QIANRE)->setText(str);//setItem(i,COL_INIT,tmpItem2);
    }
//--写入硬度(相数1,2,3,4,包含四)
    for (int i=0;i<=iPhaseNum;i++)
    {
        if(DATA.strHard.find(i)==DATA.strHard.end())continue;
        QString str=DATA.strHard[i];
        if (i==0)continue;
        m_TableWidget->item(i-1,COL_HARD)->setText(str);//setItem(i,COL_INIT,tmpItem2);
    }

//--写入相平衡
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
            tmpBtn->setText(tr("编辑"));
        }else{//new dlg
            QPhaseBalanceDialog *NewDlg = new QPhaseBalanceDialog(this);
            QStringList strList;
            strList<<tr("温度")<<tr("分数");
            NewDlg->m_TCurveDataPlot->ChangeLable(strList);
            NewDlg->m_TCurveDataPlot->SetData(tmpBaseItem.iNum,tmpBaseItem.strData);
            //设置数据保存进当前界面中
            m_phaseBalanceDlg.insert(id,NewDlg);
            tmpBtn->setText(tr("编辑"));
        }
    }
//--写入比热
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
            tmpBtn->setText(tr("编辑"));
        }else{//new dlg
            QPhasePropDialog *NewDlg = new QPhasePropDialog(this);
            NewDlg->setWindowTitle(tr("比热"));
            QStringList strList;
            strList<<tr("温度")<<tr("比热值");
            NewDlg->m_TCurveDataPlot->ChangeLable(strList);
            NewDlg->m_TCurveDataPlot->SetData(tmpBaseItem.iNum,tmpBaseItem.strData);
            //设置数据保存进当前界面中
            m_PhasePropSpecDlg.insert(id,NewDlg);
            tmpBtn->setText(tr("编辑"));
        }
    }

//--写入导热
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
            tmpBtn->setText(tr("编辑"));
        }else{//new dlg
            QPhasePropDialog *NewDlg = new QPhasePropDialog(this);
            NewDlg->setWindowTitle(tr("导热"));
            QStringList strList;
            strList<<tr("温度")<<tr("导热值");
            NewDlg->m_TCurveDataPlot->ChangeLable(strList);
            NewDlg->m_TCurveDataPlot->SetData(tmpBaseItem.iNum,tmpBaseItem.strData);
            //设置数据保存进当前界面中
            m_PhasePropCondDlg.insert(id,NewDlg);
            tmpBtn->setText(tr("编辑"));
        }
    }

//--写入孕育期
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
            tmpBtn->setText(tr("编辑"));
        }else{//new dlg
            QIncubationPeriod *NewDlg = new QIncubationPeriod(this);
            QStringList strList;
            strList<<tr("温度")<<tr("时间");
            NewDlg->m_TCurveDataPlot->ChangeLable(strList);
            NewDlg->m_TCurveDataPlot->SetData(tmpBaseItem.iNum,tmpBaseItem.strData);
            //设置数据保存进当前界面中
            m_IncubationPeriodDlg.insert(id,NewDlg);
            tmpBtn->setText(tr("编辑"));
        }
    }
    //--写入转变分数
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
            tmpBtn->setText(tr("编辑"));
        }else{//new dlg
            QChangePointDialog *NewDlg = new QChangePointDialog(this);
            NewDlg->setData(TmpPhaseChangeTab);
            m_changePointDlg.insert(id,NewDlg);
            tmpBtn->setText(tr("编辑"));
        }
    }
    //--写入屈服强度
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
    //--写入晶粒尺寸
     QStringList strCrystalList=DATA.strGS.split(",");
     if (strCrystalList.size()==3)
     {
         m_EditTab1P1->setText(strCrystalList.at(0).simplified());
         m_EditTab1P2->setText(strCrystalList.at(1).simplified());
         m_EditTab1P3->setText(strCrystalList.at(2).simplified());
     }

    //--写入热处理方式
      QStringList strPhaseControl=DATA.strPhaseCtrol.split(",");
      int strCntrAnalyStyle,
              strCntrLatentheat,
              strCntrProp,
              strCntrEquilibrium,
              strCntrTTT;//StrTTT,默认设置为1
      if (strPhaseControl.size()==5)
      {
          strCntrAnalyStyle=strPhaseControl.at(0).toInt();
          strCntrLatentheat=strPhaseControl.at(1).toInt();
          strCntrProp=strPhaseControl.at(2).toInt();
          strCntrEquilibrium=strPhaseControl.at(3).toInt();
          strCntrTTT=strPhaseControl.at(4).toInt();
      }

      if (strCntrAnalyStyle==1){//不考虑孕育期
          m_CheckBoxGestation->setChecked(false);
      }else if(strCntrAnalyStyle==2){//考虑孕育期
          m_CheckBoxGestation->setChecked(true);
      }else if(strCntrAnalyStyle==3){//母子相晶粒尺寸
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

//--将TTT属性进行保存
void QHPTTTDlg::ImportInpTTT(ReadInpResultS TTTMaterial)
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