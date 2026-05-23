#include "QHPCCTDlg.h"
#include <QScrollArea>
QHPCCTDlg* QHPCCTDlg::instance = 0;
QHPCCTDlg::QHPCCTDlg(QWidget *parent)
    : QDialog(parent)
{
    CreatGUI();
}

QHPCCTDlg::~QHPCCTDlg()
{

}
QHPCCTDlg* QHPCCTDlg::GetInstance(QWidget *parent)
{
    if (instance == 0)
    {
        instance = new QHPCCTDlg(parent);
    }

    return instance;
}
void QHPCCTDlg::CreatGUI()
{
    setWindowTitle("CCT");
    //--Tab1
    m_Tab1=new QWidget();//(tr("ddd1"));
    m_ImageFormula = new QPushButton(QIcon(":/images/NewIcon.png"), tr("&New"),m_Tab1);
    m_CheckBoxGestation = new QCheckBox(tr("考虑孕育期间"),m_Tab1);
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
    Gridlayout1->addWidget(m_ImageFormula,0,1,3,3);
    m_Tab1->setLayout(Gridlayout1);

    //--Tab2
    m_Tab2=new QWidget();//(tr("ddd2"));
    m_gbBoxTab2 = new QGroupBox(m_Tab2);
    m_gbBoxTab2->setCheckable(true);
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
    //HlayoutTab2_1->setMargin(20);
    //HlayoutTab2_1->setSpacing(20);

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
    QPushButton *img = new QPushButton("img");
    img->setFlat(true);
    QGridLayout *Gridlayout2 = new QGridLayout(m_Tab2);
    Gridlayout2->addWidget(m_gbBoxTab2,0,0,1,1);
    Gridlayout2->addWidget(img,0,1,4,6);
    m_Tab2->setLayout(Gridlayout2);


    //--Tab3
    m_Tab3=new QWidget();//(tr("ddd3"));
    m_CheckBoxQianRe = new QCheckBox(tr("潜热"));
    m_CheckBoxReWuXing = new QCheckBox(tr("热物性"));
    m_CheckBoxCCTemptCal = new QCheckBox(tr("CCT等温计算"));
    QVBoxLayout *Gridlayout3 = new QVBoxLayout(m_Tab3);
    Gridlayout3->addWidget(m_CheckBoxQianRe);
    Gridlayout3->addWidget(m_CheckBoxReWuXing);
    Gridlayout3->addWidget(m_CheckBoxCCTemptCal);
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

    m_TableWidget = new QTableWidget();
    m_TableWidget->setColumnCount(9);
    m_TableWidget->setRowCount(2);
    //m_TableWidget->verticalHeaderItem(1);
    QStringList strList;
    strList.append(tr("相"));
    strList<<tr("转变分数")<<tr("冷速")<<tr("初始值")<<tr("相平衡")<<tr("孕育期")<<tr("导热")<<tr("比热")<<tr("潜热");
    m_TableWidget->setHorizontalHeaderLabels(strList);

    //--combox
    for (int i=0; i<2; i++){
        MyCombox *tmpCombox = new MyCombox(this,i);
        //tmpCombox->addItem("1-10%");
        //tmpCombox->addItem("2-50%");
        //tmpCombox->addItem("3");
        tmpCombox->addItem("新建");//增加
        m_TableWidget->setCellWidget(i,COL_CCT_CHANGEPOINT,tmpCombox);
        m_ComboxChangePoint.push_back(tmpCombox);//转变分数
        connect(tmpCombox,SIGNAL(RightClicked(int,int)),this,SLOT(comBoxRChangePointSlot(int,int)));
        connect(tmpCombox,SIGNAL(clicked(int,int)),this,SLOT(comBoxNewChangePointSlot(int,int)));

        tmpCombox = new MyCombox(this,i);
        //tmpCombox->addItem("1");
        tmpCombox->addItem("新建");//增加
        m_TableWidget->setCellWidget(i,COL_CCT_PHASEBALANCE,tmpCombox);
        m_ComboxPhaseBalance.push_back(tmpCombox);//相位平衡、
        connect(tmpCombox,SIGNAL(RightClicked(int,int)),this,SLOT(comBoxRPhaseSlot(int,int)));
        connect(tmpCombox,SIGNAL(clicked(int,int)),this,SLOT(comBoxNewPhaseSlot(int,int)));

        tmpCombox = new MyCombox(this,i);
        //tmpCombox->addItem("1");
        tmpCombox->addItem("新建");//增加
        m_TableWidget->setCellWidget(i,COL_CCT_YUNYUQI,tmpCombox);
        m_ComboxYunYuQi.push_back(tmpCombox);//孕育期
        connect(tmpCombox,SIGNAL(RightClicked(int,int)),this,SLOT(comBoxRYunYuSlot(int,int)));
        connect(tmpCombox,SIGNAL(clicked(int,int)),this,SLOT(comBoxNewYunYuSlot(int,int)));
        tmpCombox->setDisabled(true);

        tmpCombox = new MyCombox(this,i);
        //tmpCombox->addItem("1");
        tmpCombox->addItem("新建");//增加
        m_TableWidget->setCellWidget(i,COL_CCT_DAORE,tmpCombox);
        m_ComboxDaoRe.push_back(tmpCombox);//导热
        connect(tmpCombox,SIGNAL(RightClicked(int,int)),this,SLOT(comBoxRDaoReSlot(int,int)));
        connect(tmpCombox,SIGNAL(clicked(int,int)),this,SLOT(comBoxNewDaoReSlot(int,int)));
        tmpCombox->setDisabled(true);

        tmpCombox = new MyCombox(this,i);
       // tmpCombox->addItem("1");
        tmpCombox->addItem("新建");//增加
        m_TableWidget->setCellWidget(i,COL_CCT_BIRE,tmpCombox);
        m_ComboxBiRe.push_back(tmpCombox);//比热
        connect(tmpCombox,SIGNAL(RightClicked(int,int)),this,SLOT(comBoxRBiReSlot(int,int)));
        connect(tmpCombox,SIGNAL(clicked(int,int)),this,SLOT(comBoxNewBiReSlot(int,int)));
        tmpCombox->setDisabled(true);

        tmpCombox = new MyCombox(this,i);
        //tmpCombox->addItem("1");
        tmpCombox->addItem("新建");//增加
        m_TableWidget->setCellWidget(i,COL_CCT_QIANRE,tmpCombox);
        m_ComboxQianRe.push_back(tmpCombox);//潜热
        connect(tmpCombox,SIGNAL(RightClicked(int,int)),this,SLOT(comBoxRQianReSlot(int,int)));
        connect(tmpCombox,SIGNAL(clicked(int,int)),this,SLOT(comBoxNewQianReSlot(int,int)));
        tmpCombox->setDisabled(true);

        tmpCombox = new MyCombox(this,i);
        //tmpCombox->addItem("1-5");
        //tmpCombox->addItem("2-10");
        //tmpCombox->addItem("3-20");
        tmpCombox->addItem("新建");//增加
        m_TableWidget->setCellWidget(i,COL_CCT_LENGSU,tmpCombox);
        m_ComboxLensu.push_back(tmpCombox);//lengsu
        connect(tmpCombox,SIGNAL(RightClicked(int,int)),this,SLOT(comBoxRLengSuSlot(int,int)));
        connect(tmpCombox,SIGNAL(clicked(int,int)),this,SLOT(comBoxNewLengSuSlot(int,int)));
        //相
        QTableWidgetItem *tmpItem2=new QTableWidgetItem(QString("%1").arg(i+1));
        m_TableWidget->setItem(i,0,tmpItem2);
        //初始值
        tmpItem2=new QTableWidgetItem(tr("0.01"));
        m_TableWidget->setItem(i,3,tmpItem2);
    }

    m_TableWidget->setMinimumHeight(300);

    m_AddLineBtn = new QPushButton(tr("增加"));
    m_DelLineBtn = new QPushButton(tr("删除"));
    m_InsertLineBtn = new QPushButton(tr("插入"));
    m_OkBtn = new QPushButton("确定");

    QGridLayout *mainLayout =new QGridLayout();
    mainLayout->addWidget(areaInfomation_,0,0,4,2);
    mainLayout->addWidget(m_TableWidget,4,0,4,1);
    mainLayout->addWidget(m_AddLineBtn,4,1,1,1);
    mainLayout->addWidget(m_DelLineBtn,5,1,1,1);
    mainLayout->addWidget(m_InsertLineBtn,6,1,1,1);
    mainLayout->addWidget(m_OkBtn,7,1,1,1);

    this->setLayout(mainLayout);
    this->setMinimumWidth(800);

    connect(m_CheckBoxGestation,SIGNAL(clicked()),this,SLOT(GestationSlot()));
    connect(m_CheckBoxCrystalSize,SIGNAL(clicked()),this,SLOT(CrystalSizeSlot()));
    connect(m_CheckBoxQianRe,SIGNAL(clicked()),this,SLOT(QianReSlot()));
    connect(m_CheckBoxReWuXing,SIGNAL(clicked()),this,SLOT(ReWuXingSlot()));

    connect(m_AddLineBtn,SIGNAL(clicked()),this,SLOT(AddLineBtnSlot()));//增加
    connect(m_InsertLineBtn,SIGNAL(clicked()),this,SLOT(InsertLineBtnSlot()));//插入
    connect(m_DelLineBtn,SIGNAL(clicked()),this,SLOT(DelLineBtnSlot()));//删除
    connect(m_OkBtn,SIGNAL(clicked()),this,SLOT(OkBtnSlot()));//确定
    m_ImageFormula->setFlat(true);
    m_CheckBoxCrystalSize->setEnabled(false);
    m_CheckBoxCalculate->setEnabled(false);
  //  m_gbBox->hide();
}
void QHPCCTDlg::CCTAddLine(int nCurRow)
{
    int nRow = m_TableWidget->rowCount();
    if (nRow<=0)return;
    MyCombox *tmpCombox = new MyCombox(this,m_ComboxChangePoint.size());
    AddExistComboxText(tmpCombox,COL_CCT_CHANGEPOINT);
    m_TableWidget->setCellWidget(nCurRow,COL_CCT_CHANGEPOINT,tmpCombox);
    m_ComboxChangePoint.push_back(tmpCombox);//转变分数
    connect(tmpCombox,SIGNAL(RightClicked(int,int)),this,SLOT(comBoxRChangePointSlot(int,int)));
    connect(tmpCombox,SIGNAL(clicked(int,int)),this,SLOT(comBoxNewChangePointSlot(int,int)));

    tmpCombox = new MyCombox(this,m_ComboxPhaseBalance.size());
    AddExistComboxText(tmpCombox,COL_CCT_PHASEBALANCE);
    m_TableWidget->setCellWidget(nCurRow,COL_CCT_PHASEBALANCE,tmpCombox);
    m_ComboxPhaseBalance.push_back(tmpCombox);//相位平衡、
    connect(tmpCombox,SIGNAL(RightClicked(int,int)),this,SLOT(comBoxRPhaseSlot(int,int)));
    connect(tmpCombox,SIGNAL(clicked(int,int)),this,SLOT(comBoxNewPhaseSlot(int,int)));

    tmpCombox = new MyCombox(this,m_ComboxYunYuQi.size());
    AddExistComboxText(tmpCombox,COL_CCT_YUNYUQI);
    m_TableWidget->setCellWidget(nCurRow,COL_CCT_YUNYUQI,tmpCombox);
    m_ComboxYunYuQi.push_back(tmpCombox);//孕育期
    connect(tmpCombox,SIGNAL(RightClicked(int,int)),this,SLOT(comBoxRYunYuSlot(int,int)));
    connect(tmpCombox,SIGNAL(clicked(int,int)),this,SLOT(comBoxNewYunYuSlot(int,int)));
    tmpCombox->setDisabled(true);

    tmpCombox = new MyCombox(this,m_ComboxDaoRe.size());
    AddExistComboxText(tmpCombox,COL_CCT_DAORE);
    m_TableWidget->setCellWidget(nCurRow,COL_CCT_DAORE,tmpCombox);
    m_ComboxDaoRe.push_back(tmpCombox);//导热
    connect(tmpCombox,SIGNAL(RightClicked(int,int)),this,SLOT(comBoxRDaoReSlot(int,int)));
    connect(tmpCombox,SIGNAL(clicked(int,int)),this,SLOT(comBoxNewDaoReSlot(int,int)));
    tmpCombox->setDisabled(true);

    tmpCombox = new MyCombox(this,m_ComboxBiRe.size());
    AddExistComboxText(tmpCombox,COL_CCT_BIRE);
    m_TableWidget->setCellWidget(nCurRow,COL_CCT_BIRE,tmpCombox);
    m_ComboxBiRe.push_back(tmpCombox);//比热
    connect(tmpCombox,SIGNAL(RightClicked(int,int)),this,SLOT(comBoxRBiReSlot(int,int)));
    connect(tmpCombox,SIGNAL(clicked(int,int)),this,SLOT(comBoxNewBiReSlot(int,int)));
    tmpCombox->setDisabled(true);

    tmpCombox = new MyCombox(this,m_ComboxQianRe.size());
    AddExistComboxText(tmpCombox,COL_CCT_QIANRE);
    m_TableWidget->setCellWidget(nCurRow,COL_CCT_QIANRE,tmpCombox);
    m_ComboxQianRe.push_back(tmpCombox);//潜热
    connect(tmpCombox,SIGNAL(RightClicked(int,int)),this,SLOT(comBoxRQianReSlot(int,int)));
    connect(tmpCombox,SIGNAL(clicked(int,int)),this,SLOT(comBoxNewQianReSlot(int,int)));
    tmpCombox->setDisabled(true);

    tmpCombox = new MyCombox(this,m_ComboxQianRe.size());
    AddExistComboxText(tmpCombox,COL_CCT_LENGSU);
    m_TableWidget->setCellWidget(nCurRow,COL_CCT_LENGSU,tmpCombox);
    m_ComboxQianRe.push_back(tmpCombox);//冷速
    connect(tmpCombox,SIGNAL(RightClicked(int,int)),this,SLOT(comBoxRQianReSlot(int,int)));
    connect(tmpCombox,SIGNAL(clicked(int,int)),this,SLOT(comBoxNewQianReSlot(int,int)));
    tmpCombox->setDisabled(true);
    ////相
    //QTableWidgetItem *tmpItem2=new QTableWidgetItem(QString("%1").arg(nCurRow+1));
    //m_TableWidget->setItem(nCurRow,0,tmpItem2);
    ////初始值
    //tmpItem2=new QTableWidgetItem(tr("10%"));
    //m_TableWidget->setItem(nCurRow,2,tmpItem2);

    //相
    QTableWidgetItem *tmpItem2=new QTableWidgetItem(QString("%1").arg(nCurRow+1));
    m_TableWidget->setItem(nCurRow,0,tmpItem2);
    //初始值
    tmpItem2=new QTableWidgetItem(tr("0.01"));
    m_TableWidget->setItem(nCurRow,3,tmpItem2);
}
/*
    Table新增行时，对改行combox进行ITEM赋值；
*/
bool QHPCCTDlg::AddExistComboxText(MyCombox *tmpCombox, int curCol)
{
    int nRow=m_TableWidget->rowCount();
    MyCombox *ff = new MyCombox(this,m_ComboxChangePoint.size());
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
void QHPCCTDlg::SetTableComboxEnable(int curCol,bool bEnable)
{
    int nRow=m_TableWidget->rowCount();
    for (int i=0;i<nRow;i++){
        MyCombox *tmpCombox=(MyCombox *)m_TableWidget->cellWidget(i,curCol);
        if (tmpCombox!=NULL){
            tmpCombox->setDisabled(!bEnable);
        }
    }
}
//--增加
void QHPCCTDlg::AddLineBtnSlot()
{
    int nRow = m_TableWidget->rowCount();
    m_TableWidget->setRowCount(nRow+1);
    CCTAddLine(nRow);
}
//--插入
void QHPCCTDlg::InsertLineBtnSlot()
{
    int curRow = m_TableWidget->currentRow();
    m_TableWidget->insertRow(curRow);
    CCTAddLine(curRow);
}
//--删除
void QHPCCTDlg::DelLineBtnSlot()
{
    int curRow = m_TableWidget->currentRow();
    if (curRow<0){
        return;
    }
    m_TableWidget->removeRow(curRow);
}
//--确定
void QHPCCTDlg::OkBtnSlot()
{
    this->accept();
}
//--考虑孕育期间
void QHPCCTDlg::GestationSlot()
{
    if(m_CheckBoxGestation->isChecked()){//选择考虑孕育期间
        m_CheckBoxCrystalSize->setEnabled(true);//
        SetTableComboxEnable(COL_CCT_YUNYUQI,true);
    }else{
        m_CheckBoxCrystalSize->setChecked(false);
        SetTableComboxEnable(COL_CCT_YUNYUQI,false);
        m_gbBox->hide();
    }
}
//--子母晶粒尺寸
void QHPCCTDlg::CrystalSizeSlot()
{
    if (m_CheckBoxCrystalSize->isChecked()){
        m_gbBox->show();
    }else{
        m_gbBox->hide();
    }
}
//--潜热
void QHPCCTDlg::QianReSlot()
{
   SetTableComboxEnable(COL_CCT_QIANRE,m_CheckBoxQianRe->isChecked());
}
//--热物性
void QHPCCTDlg::ReWuXingSlot()
{
    SetTableComboxEnable(COL_CCT_DAORE,m_CheckBoxReWuXing->isChecked());
    SetTableComboxEnable(COL_CCT_BIRE,m_CheckBoxReWuXing->isChecked());   
}
//
void QHPCCTDlg::comBoxRChangePointSlot(int RowId,int CurrentId)
{
    if(RowId<0 || CurrentId<0)return;
   
    QString str =m_ComboxChangePoint.at(RowId)->currentText();
    if (str.toLower()=="新建"){
        return;
    }
    m_CurTabRow = RowId;
    m_CurTabCol = COL_CCT_CHANGEPOINT;
    m_ComboxCurId = m_ComboxChangePoint.at(RowId)->currentIndex();
    QMenu *menu=new QMenu();
    menu->addAction(tr("删除"),this,SLOT(ContextDelSlot()));
    menu->exec(QCursor::pos());
    delete menu;
}
//--
void QHPCCTDlg::comBoxRPhaseSlot(int RowId,int CurrentId)
{
    if(RowId<0 || CurrentId<0) return;
    
    QString str =m_ComboxPhaseBalance.at(RowId)->currentText();
    if (str.toLower()=="新建"){
        return;
    }
    m_CurTabRow = RowId;
    m_CurTabCol = COL_CCT_PHASEBALANCE;
    m_ComboxCurId = m_ComboxPhaseBalance.at(RowId)->currentIndex();
    QMenu *menu=new QMenu();
    menu->addAction(tr("删除"),this,SLOT(ContextDelSlot()));
    menu->exec(QCursor::pos());
    delete menu;
}

void QHPCCTDlg::comBoxRYunYuSlot(int RowId,int CurrentId)
{
    if(RowId<0 || CurrentId<0) return;
   
    QString str =m_ComboxYunYuQi.at(RowId)->currentText();
    if (str.toLower()=="新建"){
        return;
    }
    m_CurTabRow = RowId;
    m_CurTabCol = COL_CCT_YUNYUQI;
    m_ComboxCurId = m_ComboxYunYuQi.at(RowId)->currentIndex();
    QMenu *menu=new QMenu();
    menu->addAction(tr("删除"),this,SLOT(ContextDelSlot()));
    menu->exec(QCursor::pos());
    delete menu;
}

void QHPCCTDlg::comBoxRDaoReSlot(int RowId,int CurrentId)
{
    if(RowId<0 || CurrentId<0) return;
    QString str =m_ComboxDaoRe.at(RowId)->currentText();
    if (str.toLower()=="新建"){
        return;
    }
    m_CurTabRow = RowId;
    m_CurTabCol = COL_CCT_DAORE;
    m_ComboxCurId = m_ComboxDaoRe.at(RowId)->currentIndex();
    QMenu *menu=new QMenu();
    menu->addAction(tr("删除"),this,SLOT(ContextDelSlot()));
    menu->exec(QCursor::pos());
    delete menu;
}

void QHPCCTDlg::comBoxRBiReSlot(int RowId,int CurrentId)
{
    if(RowId<0 || CurrentId<0) return;
   
    QString str =m_ComboxBiRe.at(RowId)->currentText();
    if (str.toLower()=="新建"){
        return;
    }
    m_CurTabRow = RowId;
    m_CurTabCol = COL_CCT_BIRE;
    m_ComboxCurId = m_ComboxBiRe.at(RowId)->currentIndex();
    QMenu *menu=new QMenu();
    menu->addAction(tr("删除"),this,SLOT(ContextDelSlot()));
    menu->exec(QCursor::pos());
    delete menu;
}

void QHPCCTDlg::comBoxRQianReSlot(int RowId,int CurrentId)
{
    if(RowId<0 || CurrentId<0) return;

    QString str =m_ComboxQianRe.at(RowId)->currentText();
    if (str.toLower()=="新建"){
        return;
    }
    m_CurTabRow = RowId;
    m_CurTabCol = COL_CCT_QIANRE;
    m_ComboxCurId = m_ComboxQianRe.at(RowId)->currentIndex();
    QMenu *menu=new QMenu();
    menu->addAction(tr("删除"),this,SLOT(ContextDelSlot()));
    menu->exec(QCursor::pos());
    delete menu;
}
void QHPCCTDlg::comBoxRLengSuSlot(int RowId,int CurrentId)
{
    if(RowId<0 || CurrentId<0)return;

    QString str =m_ComboxLensu.at(RowId)->currentText();
    if (str.toLower()=="新建"){
        return;
    }
    m_CurTabRow = RowId;
    m_CurTabCol = COL_CCT_QIANRE;
    m_ComboxCurId = m_ComboxLensu.at(RowId)->currentIndex();
    QMenu *menu=new QMenu();
    menu->addAction(tr("删除"),this,SLOT(ContextDelSlot()));
    menu->exec(QCursor::pos());
    delete menu;
}
//--New
void QHPCCTDlg::comBoxNewChangePointSlot(int RowId,int CurrentId)
{
    if(RowId<0 || CurrentId<0)return;

    QString str = m_ComboxChangePoint.at(RowId)->currentText();

    if (str.toLower()=="新建"){
        QChangePointDialog *NewDlg = new QChangePointDialog(this);
        if(NewDlg->exec()){
            str = NewDlg->GetEditText();
        }
        m_changePointDlg.insert(str,NewDlg);
        if(str == "" || m_ComboxChangePoint.at(RowId)->findText(str)!=-1){
            return;
        }
        m_ComboxChangePoint.at(RowId)->insertItem(CurrentId,str);
        m_ComboxChangePoint.at(RowId)->setCurrentIndex(CurrentId);
        //-其他combox同步其中内容
        for (int i=0;i<m_ComboxChangePoint.size();i++){
            if (i==RowId)continue;
            m_ComboxChangePoint.at(i)->insertItem(CurrentId,str);
        }
    }else{
        if (m_changePointDlg.find(str) == m_changePointDlg.end()){//不存在退出
            return;
        }
        if(m_changePointDlg[str]->exec()){
            QString strNew=m_changePointDlg[str]->GetEditText();
            if (str==strNew)return;
            m_changePointDlg.insert(strNew,m_changePointDlg[str]);
            for (int i=0;i<m_TableWidget->rowCount();i++){
                MyCombox *tmpCombox=(MyCombox *)m_TableWidget->cellWidget(i,COL_CCT_CHANGEPOINT);
                if (tmpCombox!=NULL){
                    int id =tmpCombox->findText(str);
                    if (id>=0)tmpCombox->setItemText(id,strNew);
                }
            }              
        }
    } 
}
void QHPCCTDlg::comBoxNewPhaseSlot(int RowId,int CurrentId)
{
    if(RowId<0 || CurrentId<0)return;
    QString str = m_ComboxPhaseBalance.at(RowId)->currentText();


    if (str.toLower()=="新建"){
        QPhaseBalanceDialog *NewDlg = new QPhaseBalanceDialog(this);
        if(NewDlg->exec()){
            //str = NewDlg->GetEditText();
        }
        m_phaseBalanceDlg.insert(str,NewDlg);
        if(str == "" || m_ComboxPhaseBalance.at(RowId)->findText(str)!=-1){
            return;
        }
        m_ComboxPhaseBalance.at(RowId)->insertItem(CurrentId,str);
        m_ComboxPhaseBalance.at(RowId)->setCurrentIndex(CurrentId);
        //-其他combox同步其中内容
        for (int i=0;i<m_ComboxPhaseBalance.size();i++){
            if (i==RowId)continue;
            m_ComboxPhaseBalance.at(i)->insertItem(CurrentId,str);
        }
    }else{
        if (m_phaseBalanceDlg.find(str) == m_phaseBalanceDlg.end()){//不存在退出
            return;
        }
        if(m_phaseBalanceDlg[str]->exec()){
            QString strNew;//=m_phaseBalanceDlg[str]->GetEditText();
            if (str==strNew)return;
            m_phaseBalanceDlg.insert(strNew,m_phaseBalanceDlg[str]);
            for (int i=0;i<m_TableWidget->rowCount();i++){
                MyCombox *tmpCombox=(MyCombox *)m_TableWidget->cellWidget(i,COL_CCT_PHASEBALANCE);
                if (tmpCombox!=NULL){
                    int id =tmpCombox->findText(str);
                    if (id>=0)tmpCombox->setItemText(id,strNew);
                }
            }              
        }
    } 
}

void QHPCCTDlg::comBoxNewYunYuSlot(int RowId,int CurrentId)
{
    if(RowId<0 || CurrentId<0) return;
    
    QString str = m_ComboxYunYuQi.at(RowId)->currentText();

    if (str.toLower()=="新建"){
        QIncubationPeriod *NewDlg = new QIncubationPeriod(this);
        if(NewDlg->exec()){
           // str = NewDlg->GetEditText();
        }
        m_IncubationPeriodDlg.insert(str,NewDlg);
        if(str == "" || m_ComboxYunYuQi.at(RowId)->findText(str)!=-1){
            return;
        }
        m_ComboxYunYuQi.at(RowId)->insertItem(CurrentId,str);
        m_ComboxYunYuQi.at(RowId)->setCurrentIndex(CurrentId);
        //-其他combox同步其中内容
        for (int i=0;i<m_ComboxYunYuQi.size();i++){
            if (i==RowId)continue;
            m_ComboxYunYuQi.at(i)->insertItem(CurrentId,str);
        }
    }else{
        if (m_IncubationPeriodDlg.find(str) == m_IncubationPeriodDlg.end()){//不存在退出
            return;
        }
        if(m_IncubationPeriodDlg[str]->exec()){
            QString strNew;//=m_IncubationPeriodDlg[str]->GetEditText();
            if (str==strNew)return;
            m_IncubationPeriodDlg.insert(strNew,m_IncubationPeriodDlg[str]);
            for (int i=0;i<m_TableWidget->rowCount();i++){
                MyCombox *tmpCombox=(MyCombox *)m_TableWidget->cellWidget(i,COL_CCT_YUNYUQI);
                if (tmpCombox!=NULL){
                    int id =tmpCombox->findText(str);
                    if (id>=0)tmpCombox->setItemText(id,strNew);
                }
            }              
        }
    } 
}

void QHPCCTDlg::comBoxNewDaoReSlot(int RowId,int CurrentId)
{
    if(RowId<0 || CurrentId<0)return;

    QString str = m_ComboxDaoRe.at(RowId)->currentText();

    if (str.toLower()=="新建"){
        QPhasePropDialog *NewDlg = new QPhasePropDialog(this);
        if(NewDlg->exec()){
            //str = NewDlg->GetEditText();
        }
        m_PhasePropCondDlg.insert(str,NewDlg);
        if(str == "" || m_ComboxDaoRe.at(RowId)->findText(str)!=-1){
            return;
        }
        m_ComboxDaoRe.at(RowId)->insertItem(CurrentId,str);
        m_ComboxDaoRe.at(RowId)->setCurrentIndex(CurrentId);
        //-其他combox同步其中内容
        for (int i=0;i<m_ComboxDaoRe.size();i++){
            if (i==RowId)continue;
            m_ComboxDaoRe.at(i)->insertItem(CurrentId,str);
        }
    }else{
        if (m_PhasePropCondDlg.find(str) == m_PhasePropCondDlg.end()){//不存在退出
            return;
        }
        if(m_PhasePropCondDlg[str]->exec()){
            QString strNew;//=m_PhasePropCondDlg[str]->GetEditText();
            if (str==strNew)return;
            m_PhasePropCondDlg.insert(strNew,m_PhasePropCondDlg[str]);
            for (int i=0;i<m_TableWidget->rowCount();i++){
                MyCombox *tmpCombox=(MyCombox *)m_TableWidget->cellWidget(i,COL_CCT_DAORE);
                if (tmpCombox!=NULL){
                    int id =tmpCombox->findText(str);
                    if (id>=0)tmpCombox->setItemText(id,strNew);
                }
            }              
        }
    } 
}

void QHPCCTDlg::comBoxNewBiReSlot(int RowId,int CurrentId)
{
    if(RowId<0 || CurrentId<0)return;

    QString str = m_ComboxBiRe.at(RowId)->currentText();

    if (str.toLower()=="新建"){
        QPhasePropDialog *NewDlg = new QPhasePropDialog(this);
        if(NewDlg->exec()){
            //str = NewDlg->GetEditText();
        }
        m_PhasePropSpecDlg.insert(str,NewDlg);
        if(str == "" || m_ComboxBiRe.at(RowId)->findText(str)!=-1){
            return;
        }
        m_ComboxBiRe.at(RowId)->insertItem(CurrentId,str);
        m_ComboxBiRe.at(RowId)->setCurrentIndex(CurrentId);
        //-其他combox同步其中内容
        for (int i=0;i<m_ComboxBiRe.size();i++){
            if (i==RowId)continue;
            m_ComboxBiRe.at(i)->insertItem(CurrentId,str);
        }
    }else{
        if (m_PhasePropSpecDlg.find(str) == m_PhasePropSpecDlg.end()){//不存在退出
            return;
        }
        if(m_PhasePropSpecDlg[str]->exec()){
            QString strNew;//=m_PhasePropSpecDlg[str]->GetEditText();
            if (str==strNew)return;
            m_PhasePropSpecDlg.insert(strNew,m_PhasePropSpecDlg[str]);
            for (int i=0;i<m_TableWidget->rowCount();i++){
                MyCombox *tmpCombox=(MyCombox *)m_TableWidget->cellWidget(i,COL_CCT_BIRE);
                if (tmpCombox!=NULL){
                    int id =tmpCombox->findText(str);
                    if (id>=0)tmpCombox->setItemText(id,strNew);
                }
            }              
        }
    }
}
void QHPCCTDlg::comBoxNewQianReSlot(int RowId,int CurrentId)
{
    if(RowId<0 || CurrentId<0)return;

    QString str = m_ComboxQianRe.at(RowId)->currentText();

    if (str.toLower()=="新建"){
        QPhaseLatentHeatDialog *NewDlg = new QPhaseLatentHeatDialog(this);
        if(NewDlg->exec()){
            str = NewDlg->GetEditText();
        }
        m_phaseLatentHeatDlg.insert(str,NewDlg);
        if(str == "" || m_ComboxChangePoint.at(RowId)->findText(str)!=-1){
            return;
        }
        m_ComboxQianRe.at(RowId)->insertItem(CurrentId,str);
        m_ComboxQianRe.at(RowId)->setCurrentIndex(CurrentId);
        //-其他combox同步其中内容
        for (int i=0;i<m_ComboxQianRe.size();i++){
            if (i==RowId)continue;
            m_ComboxQianRe.at(i)->insertItem(CurrentId,str);
        }
    }else{
        if (m_phaseLatentHeatDlg.find(str) == m_phaseLatentHeatDlg.end()){//不存在退出
            return;
        }
        if(m_phaseLatentHeatDlg[str]->exec()){
            QString strNew=m_phaseLatentHeatDlg[str]->GetEditText();
            if (str==strNew)return;
            m_phaseLatentHeatDlg.insert(strNew,m_phaseLatentHeatDlg[str]);
            for (int i=0;i<m_TableWidget->rowCount();i++){
                MyCombox *tmpCombox=(MyCombox *)m_TableWidget->cellWidget(i,COL_CCT_QIANRE);
                if (tmpCombox!=NULL){
                    int id =tmpCombox->findText(str);
                    if (id>=0)tmpCombox->setItemText(id,strNew);
                }
            }              
        }
    } 
}
void QHPCCTDlg::comBoxNewLengSuSlot(int RowId,int CurrentId)
{
    if(RowId<0 || CurrentId<0){
        return;
    }
    QString str = m_ComboxLensu.at(RowId)->currentText();
    /*if (str.toLower()!="新建"){
        return;
    }
    QNewDialog *NewDlg = new QNewDialog(this);
    if(NewDlg->exec()){
        str = NewDlg->GetEditText();
    }
    delete NewDlg;

    if(str == "" || m_ComboxLensu.at(RowId)->findText(str)!=-1){
        return;
    }
    m_ComboxLensu.at(RowId)->insertItem(CurrentId,str);
    m_ComboxLensu.at(RowId)->setCurrentIndex(CurrentId);*/
    if (str.toLower()=="新建"){
        QPhaseZBFDialog *NewDlg = new QPhaseZBFDialog(this);
        if(NewDlg->exec()){
            str = NewDlg->GetEditText();
        }
        m_PhaseZBFDlg.insert(str,NewDlg);
        if(str == "" || m_ComboxLensu.at(RowId)->findText(str)!=-1){
            return;
        }
        m_ComboxLensu.at(RowId)->insertItem(CurrentId,str);
        m_ComboxLensu.at(RowId)->setCurrentIndex(CurrentId);
        //-其他combox同步其中内容
        for (int i=0;i<m_ComboxChangePoint.size();i++){
            if (i==RowId)continue;
            m_ComboxLensu.at(i)->insertItem(CurrentId,str);
        }
    }else{
        if (m_PhaseZBFDlg.find(str) == m_PhaseZBFDlg.end()){//不存在退出
            return;
        }
        if(m_PhaseZBFDlg[str]->exec()){
            QString strNew=m_PhaseZBFDlg[str]->GetEditText();
            if (str==strNew)return;
            m_PhaseZBFDlg.insert(strNew,m_PhaseZBFDlg[str]);
            for (int i=0;i<m_TableWidget->rowCount();i++){
                MyCombox *tmpCombox=(MyCombox *)m_TableWidget->cellWidget(i,COL_CCT_LENGSU);
                if (tmpCombox!=NULL){
                    int id =tmpCombox->findText(str);
                    if (id>=0)tmpCombox->setItemText(id,strNew);
                }
            }              
        }
    } 
}
//--删除
void QHPCCTDlg::ContextDelSlot()
{
    int id = m_CurTabCol;
    if (id<=0) return;
   
    QMessageBox::StandardButton bt= QMessageBox::question(this, tr("删除"), tr("确定删除？"), QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);

    if(bt == QMessageBox::No) return;
    
    QString str="";
    switch(id){
     case COL_CCT_CHANGEPOINT:{
         MyCombox *tmpCombox=(MyCombox *)m_TableWidget->cellWidget(m_CurTabRow,COL_CCT_CHANGEPOINT);
         if (tmpCombox==NULL)return;
         str=tmpCombox->currentText();
         if (m_changePointDlg.find(str)!=m_changePointDlg.end()){ 
             m_changePointDlg.remove(str);
             int id=tmpCombox->findText(str);
             if(id>=0)tmpCombox->removeItem(id);
         }   
       }break;
     case COL_CCT_PHASEBALANCE:{
         MyCombox *tmpCombox=(MyCombox *)m_TableWidget->cellWidget(m_CurTabRow,COL_CCT_PHASEBALANCE);
         if (tmpCombox==NULL)return;
         str=tmpCombox->currentText();
         if (m_phaseBalanceDlg.find(str)!=m_phaseBalanceDlg.end()){ 
             m_phaseBalanceDlg.remove(str);
             int id=tmpCombox->findText(str);
             if(id>=0)tmpCombox->removeItem(id);
         }                    
       }break;
     case COL_CCT_YUNYUQI:{
         MyCombox *tmpCombox=(MyCombox *)m_TableWidget->cellWidget(m_CurTabRow,COL_CCT_YUNYUQI);
         if (tmpCombox==NULL)return;
         str=tmpCombox->currentText();
         if (m_IncubationPeriodDlg.find(str)!=m_IncubationPeriodDlg.end()){ 
             m_IncubationPeriodDlg.remove(str);
             int id=tmpCombox->findText(str);
             if(id>=0)tmpCombox->removeItem(id);
         }       
       }break;
     case COL_CCT_DAORE:{
         MyCombox *tmpCombox=(MyCombox *)m_TableWidget->cellWidget(m_CurTabRow,COL_CCT_DAORE);
         if (tmpCombox==NULL)return;
         str=tmpCombox->currentText();
         if (m_PhasePropCondDlg.find(str)!=m_PhasePropCondDlg.end()){ 
             m_PhasePropCondDlg.remove(str);
             int id=tmpCombox->findText(str);
             if(id>=0)tmpCombox->removeItem(id);
         }                    
       }break;
     case COL_CCT_BIRE:{
         MyCombox *tmpCombox=(MyCombox *)m_TableWidget->cellWidget(m_CurTabRow,COL_CCT_BIRE);
         if (tmpCombox==NULL)return;
         str=tmpCombox->currentText();
         if (m_PhasePropSpecDlg.find(str)!=m_PhasePropSpecDlg.end()){ 
             m_PhasePropSpecDlg.remove(str);
             int id=tmpCombox->findText(str);
             if(id>=0)tmpCombox->removeItem(id);
         }                    
       }break;
     case COL_CCT_QIANRE:{
         MyCombox *tmpCombox=(MyCombox *)m_TableWidget->cellWidget(m_CurTabRow,COL_CCT_QIANRE);
         if (tmpCombox==NULL)return;
         str=tmpCombox->currentText();
         if (m_phaseLatentHeatDlg.find(str)!=m_phaseLatentHeatDlg.end()){ 
             m_phaseLatentHeatDlg.remove(str);
             int id=tmpCombox->findText(str);
             if(id>=0)tmpCombox->removeItem(id);
         }                    
      }break;
     case COL_CCT_LENGSU:{
         MyCombox *tmpCombox=(MyCombox *)m_TableWidget->cellWidget(m_CurTabRow,COL_CCT_LENGSU);
         if (tmpCombox==NULL)return;
         str=tmpCombox->currentText();
         if (m_PhaseZBFDlg.find(str)!=m_PhaseZBFDlg.end()){ 
             m_PhaseZBFDlg.remove(str);
             int id=tmpCombox->findText(str);
             if(id>=0)tmpCombox->removeItem(id);
         }                  
         m_ComboxLensu.at(m_CurTabRow)->removeItem(m_ComboxCurId); break;
       }break;
    default: break;
    }

}
//--写入到inp中;
HPTTTInps QHPCCTDlg::WriteHPCCToInp()
{
    int nRow = m_TableWidget->rowCount();
    int nCol = m_TableWidget->columnCount();

    HPTTTInps TotalTTTInps;
    HPphaseCurveInps tmpChangePointInps;
    HPphaseBalanceInps tmpPhaseBalanceInps;
    HPincubatePeriodInps tmpIncuPeriodInps;
    HPphaseElateHeatInps tmpPhaseElateHeatInps;//潜热
    HPPhasePropSpecInps tmpPhasePropSpecInps;//比热
    HPPhasePropCondInps tmpPhasePropCondInps;//导热
    HPphaseCtrolInps tmpPhaseCtrolInps;
    HPPhaseYSInps tmpPhaseYSInps;//屈服强度
    HPPhaseGSInps tmpPhaseGSInps;//晶粒尺寸参数
    HPPhaseZBFInps tmpPhaseZBFInps;//冷速

    QStringList strHeadParam;
    QStringList strData;  
    MyCombox *tmpCombox=NULL;

    //-----转变积分
    //for (int i=0; i<nRow; i++){
    //    tmpCombox=(MyCombox *)m_TableWidget->cellWidget(i,COL_CCT_CHANGEPOINT);
    //    if (tmpCombox==NULL) break;
    //    QString strName = tmpCombox->currentText();
    //    if (m_changePointDlg.find(strName)!=m_changePointDlg.end()){
    //        int DataNum=m_changePointDlg[strName]->GetDataNum();
    //        QString strTmp;
    //        strTmp=m_changePointDlg[strName]->m_LineEditName->text()+","+//相序
    //            m_changePointDlg[strName]->m_LineEditCurveTh->text()+","+//第几条曲线
    //            tr("%1").arg(DataNum)+","+//数据点数
    //            m_changePointDlg[strName]->m_LineEditChangeP->text()+","+//分数
    //            m_TableWidget->item(i,3)->text();
    //        strHeadParam<<strTmp;
    //        if (DataNum) strData<<m_changePointDlg[strName]->GetPlotData();   
    //    }      
    //}  
    tmpChangePointInps.strName="PHASECURAVE";
    tmpChangePointInps.strPhaseCurveType="TTT";
    tmpChangePointInps.strHeadParam=strHeadParam;
    tmpChangePointInps.strData=strData;

    //-----相平衡
    strHeadParam.clear();strData.clear();
    for (int i=0; i<nRow; i++){ 
        tmpCombox=(MyCombox *)m_TableWidget->cellWidget(i,COL_CCT_PHASEBALANCE);
        if (tmpCombox==NULL) break;
        QString strName = tmpCombox->currentText();
        if (m_phaseBalanceDlg.find(strName)!=m_phaseBalanceDlg.end()){
            int DataNum=m_phaseBalanceDlg[strName]->GetDataNum();
            QString strTmp;
            //strTmp=m_phaseBalanceDlg[strName]->m_LineName->text()+","+//相序
            //    tr("%1").arg(DataNum);//数据点数
           if (strHeadParam.indexOf(strTmp)!=-1) continue;            
            strHeadParam<<strTmp;
            if (DataNum) strData<<m_phaseBalanceDlg[strName]->GetPlotData();
        }      
    }  
    tmpPhaseBalanceInps.strName="PHASEEQUILIBRIUM";
    tmpPhaseBalanceInps.strHeadParam=strHeadParam;
    tmpPhaseBalanceInps.strData=strData;

    //-----潜热
    strHeadParam.clear();
    for (int i=0; i<nRow; i++){ 
        tmpCombox=(MyCombox *)m_TableWidget->cellWidget(i,COL_CCT_QIANRE);
        if (tmpCombox==NULL) break;
        QString strName = tmpCombox->currentText();
        if (m_phaseLatentHeatDlg.find(strName)!=m_phaseLatentHeatDlg.end()){
            QString strTmp;
            strTmp = m_phaseLatentHeatDlg[strName]->m_LineName->text()+","+//相序
                m_phaseLatentHeatDlg[strName]->m_LineValue->text();//数值
            if (strHeadParam.indexOf(strTmp)!=-1) continue;
            strHeadParam<<strTmp;
        }      
    } 
    tmpPhaseElateHeatInps.strName="PHASELATENTHEAT";
    tmpPhaseElateHeatInps.strHeadParam=strHeadParam;

    //-----孕育期
    strHeadParam.clear();strData.clear();
    for (int i=0; i<nRow; i++){ 
        tmpCombox=(MyCombox *)m_TableWidget->cellWidget(i,COL_CCT_YUNYUQI);
        if (tmpCombox==NULL) break;
        QString strName = tmpCombox->currentText();
        if (m_IncubationPeriodDlg.find(strName)!=m_IncubationPeriodDlg.end()){
            int DataNum=m_IncubationPeriodDlg[strName]->GetDataNum();
            QString strTmp;
            //strTmp=m_IncubationPeriodDlg[strName]->m_LineName->text()+","+//相序
             //   tr("%1").arg(DataNum);//数据点数
            if (strHeadParam.indexOf(strTmp)!=-1) continue;
            strHeadParam<<strTmp;
            if (DataNum) strData<<m_IncubationPeriodDlg[strName]->GetPlotData();
        }      
    }  
    tmpIncuPeriodInps.strName="INCUBATIONPERIOD";
    tmpIncuPeriodInps.strHeadParam=strHeadParam;
    tmpIncuPeriodInps.strData=strData;

    //-----比热
    strHeadParam.clear();strData.clear();
    for (int i=0; i<nRow; i++){ 
        tmpCombox=(MyCombox *)m_TableWidget->cellWidget(i,COL_CCT_BIRE);
        if (tmpCombox==NULL) break;
        QString strName = tmpCombox->currentText();

        if (m_PhasePropSpecDlg.find(strName)!=m_PhasePropSpecDlg.end()){
            int DataNum=m_PhasePropSpecDlg[strName]->GetDataNum();
            QString strTmp;
            //strTmp=m_PhasePropSpecDlg[strName]->m_LineName->text()+","+//相序
            //    tr("%1").arg(DataNum);//数据点数
            if (strHeadParam.indexOf(strTmp)!=-1) continue;
            strHeadParam<<strTmp;
            if (DataNum) strData<<m_PhasePropSpecDlg[strName]->GetPlotData();
        }      
    } 

    tmpPhasePropSpecInps.strName="PHASEPROP";
    tmpPhasePropSpecInps.strType="SPEC";
    tmpPhasePropSpecInps.strHeadParam=strHeadParam;
    tmpPhasePropSpecInps.strData=strData;

    //-----导热
    strHeadParam.clear();strData.clear();
    for (int i=0; i<nRow; i++){ 
        tmpCombox=(MyCombox *)m_TableWidget->cellWidget(i,COL_CCT_DAORE);
        if (tmpCombox==NULL) break;
        QString strName = tmpCombox->currentText();

        if (m_PhasePropCondDlg.find(strName) != m_PhasePropCondDlg.end()){
            int DataNum=m_PhasePropCondDlg[strName]->GetDataNum();
            QString strTmp;
            //strTmp=m_PhasePropCondDlg[strName]->m_LineName->text()+","+//相序
            //    tr("%1").arg(DataNum);//数据点数
            if (strHeadParam.indexOf(strTmp)!=-1) continue;
            strHeadParam<<strTmp;
            if (DataNum) strData<<m_PhasePropCondDlg[strName]->GetPlotData();
        }      
    } 
    tmpPhasePropCondInps.strName="PHASEPROP";
    tmpPhasePropCondInps.strType="COND";
    tmpPhasePropCondInps.strHeadParam=strHeadParam;
    tmpPhasePropCondInps.strData=strData;

    //--冷速
    strHeadParam.clear();strData.clear();
    for (int i=0; i<nRow; i++){
        tmpCombox=(MyCombox *)m_TableWidget->cellWidget(i,COL_CCT_LENGSU);
        if (tmpCombox==NULL) break;
        QString strName = tmpCombox->currentText();
        if (m_PhaseZBFDlg.find(strName)!=m_PhaseZBFDlg.end()){
            int DataNum=m_PhaseZBFDlg[strName]->GetDataNum();
            QString strTmp;
            strTmp=m_PhaseZBFDlg[strName]->m_LineEditName->text()+","+//相序
                m_PhaseZBFDlg[strName]->m_LineEditCurveTh->text()+","+//第几条曲线
                tr("%1").arg(DataNum)+","+//数据点数
                m_PhaseZBFDlg[strName]->m_LineEditChangeP->text();//分数
            if (strHeadParam.indexOf(strTmp)!=-1) continue;
            strHeadParam<<strTmp;
            if (DataNum) strData<<m_PhaseZBFDlg[strName]->GetPlotData();   
        }      
    }  
    tmpPhaseZBFInps.strName="PHASEZBF";
    tmpPhaseZBFInps.strHeadParam=strHeadParam;
    tmpPhaseZBFInps.strData=strData;


    //--热处理方式控制
    QString strAnaly;
    strAnaly ="1";//不考虑孕育期
    if (m_CheckBoxGestation->isChecked())
        strAnaly="2";//考虑孕育期
    if (m_CheckBoxCrystalSize->isChecked())
        strAnaly="3";//母子相晶粒尺寸

    QString strPeriod;
    strPeriod="0";
    if (strAnaly=="1"&&m_CheckBoxCalculate->isChecked())
        strPeriod="1";

    QString strElatentHeat;
    strElatentHeat="0";//是否考虑潜热
    if (m_CheckBoxQianRe->isChecked())
        strElatentHeat="1";

    QString strHeatStyle;
    strHeatStyle="0";//是否考虑热物性
    if (m_CheckBoxReWuXing->isChecked())
        strHeatStyle="1";
    tmpPhaseCtrolInps.strName="PHASECTROL";
    tmpPhaseCtrolInps.strHeadParam = strAnaly+","+strElatentHeat+","+
        strHeatStyle+","+strPeriod+",0";

    //--屈服强度参数
    tmpPhaseYSInps.strName="PHASEYS";
    tmpPhaseYSInps.strHeadParam= m_EditTab2P11->text()+","+m_EditTab2P12->text()+","+m_EditTab2P13->text()+
        m_EditTab2P21->text()+","+m_EditTab2P22->text()+","+m_EditTab2P23->text()+
        m_EditTab2P31->text()+","+m_EditTab2P32->text()+","+m_EditTab2P33->text()+
        m_EditTab2P41->text()+","+m_EditTab2P42->text()+","+m_EditTab2P43->text();                               
    //--晶粒尺寸参数
    tmpPhaseYSInps.strName="PHASEGS";
    tmpPhaseYSInps.strHeadParam=m_EditTab1P1->text()+","+m_EditTab1P2->text()+","+m_EditTab1P3->text();

    TotalTTTInps.PhaseCurveInps=tmpChangePointInps;
    TotalTTTInps.PhaseBalanceInps=tmpPhaseBalanceInps;
    TotalTTTInps.PhaseElateHeatInps=tmpPhaseElateHeatInps;
    TotalTTTInps.PhaseCtrolInps=tmpPhaseCtrolInps;
    TotalTTTInps.IncubatePeriodInps=tmpIncuPeriodInps;
    TotalTTTInps.PhasePropCondInps=tmpPhasePropCondInps;
    TotalTTTInps.PhasePropSpecInps=tmpPhasePropSpecInps;
    TotalTTTInps.PhaseYSInps=tmpPhaseYSInps;
    TotalTTTInps.PhaseGSInps=tmpPhaseGSInps;
    TotalTTTInps.PhaseZBFInps=tmpPhaseZBFInps;
    return TotalTTTInps;
}
