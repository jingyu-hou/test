#include "QNewDialog.h"
#include <QMessageBox>

QNewDialog::QNewDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("新建"));
    QHBoxLayout *Hlayout1 = new QHBoxLayout();
    QLabel *lab = new QLabel(tr("数据:"));
    m_LineEdit = new QLineEdit();
    Hlayout1->addWidget(lab);
    Hlayout1->addWidget(m_LineEdit);
    Hlayout1->addStretch();
    
    QHBoxLayout *Hlayout2 = new QHBoxLayout();
    m_OKBtn = new QPushButton(tr("确定"));
    m_CancelBtn = new QPushButton(tr("取消"));
    Hlayout2->addStretch();
    Hlayout2->addWidget(m_OKBtn);
    Hlayout2->addWidget(m_CancelBtn);


    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(Hlayout1);
    mainLayout->addLayout(Hlayout2);
    this->setLayout(mainLayout);

    connect(m_OKBtn,SIGNAL(clicked()),this,SLOT(accept()));
    connect(m_CancelBtn,SIGNAL(clicked()),this,SLOT(reject()));

}

QNewDialog::~QNewDialog()
{

}

QString QNewDialog::GetEditText()
{
    return m_LineEdit->text();
}

//--------------------------------------
//--------------  相变积分 -------------
//--------------------------------------

QChangePointDialog::QChangePointDialog(QWidget *parent)
: QDialog(parent)
{
    setWindowTitle(tr("相变积分"));
    QWidget *Widge1=new QWidget(this);
    QWidget *Widge2=new QWidget(this);
    QWidget *Widge3=new QWidget(this);
    m_AddTabBtn=new QPushButton(tr("增加曲线"),Widge1);
    m_DelTabBtn=new QPushButton(tr("删除曲线"),Widge1);
    m_AutoReName = new QPushButton(tr("自动命名"),Widge1);
    m_PlotBtn=new QPushButton(tr("绘图"),Widge1);

    QAddTab *TmpTab = new QAddTab(Widge1);
    m_addNewTab.insert(1, TmpTab);
    m_tabView = new QTabWidget(Widge1);
    m_tabView->addTab(TmpTab, tr("曲线1"));
    m_tabView->setMovable(true);
    m_tabView->setUsesScrollButtons(true);
    //m_ChartProp.strListLabel<<tr("曲线1");
    //m_groupBoxPlot=new QGroupBox();

    QGridLayout *W1Layout = new QGridLayout(Widge1);
    W1Layout->addWidget(m_tabView,1,0,2,4);
    W1Layout->addWidget(m_AddTabBtn,0,0,1,1);
    W1Layout->addWidget(m_DelTabBtn,0,1,1,1);
    W1Layout->addWidget(m_AutoReName,0,2,1,1);
    W1Layout->addWidget(m_PlotBtn,0,3,1,1);
    //W1Layout->addWidget(m_tabView,1,0,2,4);
    //W1Layout->addWidget(tmpWidget,0,4,2,4);
    //this->setLayout(W1Layout);

    chartViewers =new QChartViewer(Widge2);
    Widge2->setMaximumHeight(300);
    QGridLayout *HLastLayout=new QGridLayout(Widge2);
    HLastLayout->addWidget(chartViewers,0,0,3,2);
    //HLastLayout->addWidget(m_OKBtn,4,0,1,1);
    //HLastLayout->addWidget(m_CancelBtn,4,1,1,1);
    QVBoxLayout *vlayoutW3 = new QVBoxLayout(Widge3);
    m_OKBtn = new QPushButton(tr("确定"),Widge3);
    m_CancelBtn = new QPushButton(tr("取消"),Widge3);
    m_ImportBtn = new QPushButton(tr("导入"),Widge3);
    m_ExportBtn = new QPushButton(tr("导出"),Widge3);
    vlayoutW3->addStretch();
    //vlayoutW3->addWidget(m_ImportBtn);//luo
    //vlayoutW3->addWidget(m_ExportBtn);//luo
    vlayoutW3->addWidget(m_OKBtn);
    vlayoutW3->addWidget(m_CancelBtn);

    QHBoxLayout *m_MainLayout = new QHBoxLayout(this);
    m_MainLayout->addWidget(Widge1);
    m_MainLayout->addWidget(Widge2);
    m_MainLayout->addWidget(Widge3);
    setLayout(m_MainLayout);
    
    connect(m_AddTabBtn,SIGNAL(clicked()),this,SLOT(AddTabBtnSlot()));
    connect(m_DelTabBtn,SIGNAL(clicked()),this,SLOT(DelTabBtnSlot()));
    connect(m_AutoReName,SIGNAL(clicked()),this,SLOT(AutoRenameSlot()));
    connect(m_OKBtn,SIGNAL(clicked()),this,SLOT(OkBtnSlot()));
    connect(m_PlotBtn,SIGNAL(clicked()),this,SLOT(PlotBtnSlot()));
    connect(m_CancelBtn,SIGNAL(clicked()),this,SLOT(reject()));
    PlotBtnSlot();
    QStringList strList;
    strList<<tr("时间")<<tr("温度");
    TmpTab->m_TabData->ChangeLable(strList);
    m_ChartProp.xLabel=strList.at(0);
    m_ChartProp.yLabel=strList.at(1);
}

QChangePointDialog::~QChangePointDialog()
{
      
}
void QChangePointDialog::OkBtnSlot()
{
    this->PlotBtnSlot();
    this->accept();
}
QString QChangePointDialog::GetEditText()
{
    QString str;
    str = m_LineEditName->text()+"-"+m_LineEditCurveTh->text()+"-"+m_LineEditChangeP->text();
    return str;
}
//--增加Tab
void QChangePointDialog::AddTabBtnSlot()
{
    QAddTab *TmpTab = new QAddTab(this);
    QStringList strList;
    strList<<tr("时间")<<tr("温度");
    TmpTab->m_TabData->ChangeLable(strList);

    int nSize=m_tabView->count();
    m_tabView->addTab(TmpTab,(tr("曲线")+QString("%1").arg(nSize+1)));
    m_addNewTab.insert(nSize+1,TmpTab);
    //m_ChartProp.strListLabel<<(tr("曲线")+QString("%1").arg(nSize+1));
}
//--删除Tab
void QChangePointDialog::DelTabBtnSlot()
{
    int iPage=m_tabView->currentIndex();
    if (iPage<0)return;
    QMessageBox::StandardButton bt= QMessageBox::question(this, tr("删除"), tr("确定删除？"), QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);
    if (bt==QMessageBox::No)return;
    m_tabView->removeTab(iPage);
}
//--自动命名Tab
void QChangePointDialog::AutoRenameSlot()
{
    int nSize =m_tabView->count();
    m_ChartProp.strListLabel.clear();
    for (int i=0; i<nSize; i++){
        m_tabView->setTabText(i, tr("曲线%1").arg(i+1));
        //m_ChartProp.strListLabel<<(tr("曲线")+QString("%1").arg(i+1));
    }
    
}
//相变积分数据点数获取
int QChangePointDialog::GetDataNum(int iPage)
{
    QAddTab *TmpTab=(QAddTab *)m_tabView->widget(iPage);
    int iCnt=TmpTab->m_TabData->m_TabCurveWiget->rowCount();
    int iPointNum=0;
    for (int i=0; i<iCnt; i++){
        QString strTmpX,strTmpY;
        strTmpX = TmpTab->m_TabData->m_TabCurveWiget->item(i,0)->text();
        strTmpY = TmpTab->m_TabData->m_TabCurveWiget->item(i,1)->text();
        if (strTmpX==""&&strTmpY=="")continue;//均为"",则无数据   
        iPointNum++;
    }
    return iPointNum;//m_TCurveDataPlot->m_TabCurveWiget->rowCount();
}

//
//格式: dataX1,dataY1; dataX2,dataY2; dataX3,dataY3;
//
QString QChangePointDialog::GetPlotData(int iPage)
{
    QAddTab *TmpTab=(QAddTab *)m_tabView->widget(iPage);
    int nRow = TmpTab->m_TabData->m_TabCurveWiget->rowCount();
    int nCol = TmpTab->m_TabData->m_TabCurveWiget->columnCount();
    QString strData; QStringList strList;
    for (int i=0; i<nRow; i++){
        QString strTmpX,strTmpY;
        strTmpX = TmpTab->m_TabData->m_TabCurveWiget->item(i,0)->text();
        strTmpY = TmpTab->m_TabData->m_TabCurveWiget->item(i,1)->text();
        if (strTmpX==""&&strTmpY=="")continue;//均为"",则无数据 
        strList<<strTmpX+","+strTmpY;   
     }
    return strData=strList.join(";");
}

//--绘图
void QChangePointDialog::PlotBtnSlot()
{
    /*int curId = m_tabView->currentIndex();
    QWidget *curWidget=m_tabView->currentWidget();

    QAddTab*tmpTab = (QAddTab*)(curWidget);
    int id=m_addNewTab.key(tmpTab); */ 
    //获取所有Tab中数据进行绘制
    int nSize =m_tabView->count();
    m_ChartProp.strListLabel.clear();
    for (int kk=0;kk<nSize;kk++){
     m_ChartProp.strListLabel<<m_tabView->tabText(kk);
    }
    
    QAddTab *tmpTab=NULL;
    vector<DoubleArray> dataX,dataY;
    for (int i=0; i<nSize; i++){
        tmpTab=(QAddTab*) m_tabView->widget(i);
        //int id=m_addNewTab.key(tmpTab);  
        tmpTab->m_TabData->m_TabCurveWiget->sortByColumn(1, Qt::AscendingOrder);//默认第1列是温度，其以升序排列；
       
        int nRow=tmpTab->m_TabData->m_TabCurveWiget->rowCount();
        int nCol=tmpTab->m_TabData->m_TabCurveWiget->columnCount();
        
        int num=nRow;
        double *dataTmpX = new double[num];
        double *dataTmpY = new double[num]; 
        for (int i=0; i<nRow; i++){
            dataTmpX[i] =tmpTab->m_TabData->m_TabCurveWiget->item(i,0)->text().toDouble();
            dataTmpY[i] =tmpTab->m_TabData->m_TabCurveWiget->item(i,1)->text().toDouble();
        }
        dataX.push_back(DoubleArray(dataTmpX, num));
        dataY.push_back(DoubleArray(dataTmpY, num));
    }
    const char *imageMap = 0;
    
    ChartShow2XY(dataX,dataY,imageMap,m_ChartProp);
}


void QChangePointDialog::ChartShow2XY(vector<DoubleArray> dataX, vector<DoubleArray> dataY, const char *imageMap,_WELCME_CHART_PROP_ prop)
{
    // Each demo module can display a number of charts. We display all of them. 
    unEVENChart *d = unEvenChart;//demoCharts + 2;//demoIndex;
    for (int tt=0;tt<d->noOfCharts;tt++)
    {
        imageMap = 0;
        chartViewers->setChart(d->createChart(dataX, dataY, &imageMap,prop));
        chartViewers->setImageMap(imageMap);
        delete chartViewers->getChart();

    }

    chartViewers->setVisible(d->noOfCharts);
}

//--获取当前ipage下转变分数
QString QChangePointDialog::GetCurveThPoint(int iPage)
{
    QString str;
    QAddTab *TmpTab=(QAddTab *)m_tabView->widget(iPage);
    str=TmpTab->m_Edit->text();
  
    return str;
}

int QChangePointDialog::GetCurveNum()
{
    return m_tabView->count();
}

//bool QChangePointDialog::setData(QString strChangeP, QStringList strData)

bool QChangePointDialog::setData(QMap<int,NewPhaseChangeS> MapData)
{
    m_addNewTab.clear();
    m_tabView->clear();
    QMap<int, NewPhaseChangeS>::Iterator it = MapData.begin();
    int i=0;
    for (it,i;it!=MapData.end();it++,i++)
    {
        AddTabBtnSlot();//添加一个Tab;
        m_addNewTab[i+1]->m_Edit->setText(it->strChangeP);
        m_addNewTab[i+1]->m_TabData->SetData(it->iNum,it->strData);
    }
    return true;
}

//--------------------------------------
//---------------  相平衡 --------------
//--------------------------------------
QPhaseBalanceDialog::QPhaseBalanceDialog(QWidget *parent)
: QDialog(parent)
{
    setWindowTitle(tr("相平衡"));

    QHBoxLayout *HPlotlayout = new QHBoxLayout();
    m_TCurveDataPlot = new QTCurveDataPlot(this);
    HPlotlayout->addWidget(m_TCurveDataPlot);
 
    QHBoxLayout *HlayoutLast = new QHBoxLayout();
    m_OKBtn = new QPushButton(tr("确定"));
    m_CancelBtn = new QPushButton(tr("取消"));
    HlayoutLast->addStretch();
    HlayoutLast->addWidget(m_OKBtn);
    HlayoutLast->addWidget(m_CancelBtn);
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(HPlotlayout);
    mainLayout->addLayout(HlayoutLast);
    this->setLayout(mainLayout);

    connect(m_OKBtn,SIGNAL(clicked()),this,SLOT(OkBtnSlot()));
    connect(m_CancelBtn,SIGNAL(clicked()),this,SLOT(reject()));
}

QPhaseBalanceDialog::~QPhaseBalanceDialog()
{

}
void QPhaseBalanceDialog::OkBtnSlot()
{
    m_TCurveDataPlot->Plot();
    this->accept();
}
int QPhaseBalanceDialog::GetDataNum()
{
    int nRow=m_TCurveDataPlot->m_TabCurveWiget->rowCount();
    int iPointNum=0;
    for (int i=0; i<nRow; i++){
        QString strTmpX,strTmpY;
        strTmpX = m_TCurveDataPlot->m_TabCurveWiget->item(i,0)->text();
        strTmpY = m_TCurveDataPlot->m_TabCurveWiget->item(i,1)->text();
        if (strTmpX==""&&strTmpY=="")continue;//均为"",则无数据   
        iPointNum++;
    }
 return iPointNum;
}

QString QPhaseBalanceDialog::GetPlotData()
{
    int nRow = m_TCurveDataPlot->m_TabCurveWiget->rowCount();
    int nCol = m_TCurveDataPlot->m_TabCurveWiget->columnCount();
    QString strData; QStringList strList;
    for (int i=0; i<nRow; i++){
        QString strTmpX,strTmpY;
        strTmpX = m_TCurveDataPlot->m_TabCurveWiget->item(i,0)->text();
        strTmpY = m_TCurveDataPlot->m_TabCurveWiget->item(i,1)->text();
        if (strTmpX==""&&strTmpY=="")continue;
        strList<<strTmpX+","+strTmpY;
    }
    return strData=strList.join(";");
}

//--------------------------------------
//----------------  潜热 ---------------
//--------------------------------------
QPhaseLatentHeatDialog::QPhaseLatentHeatDialog(QWidget *parent)
: QDialog(parent)
{
    setWindowTitle(tr("潜热"));
    QHBoxLayout *Hlayout1 = new QHBoxLayout();
    QLabel *lab = new QLabel(tr("项:"));
    m_LineName = new QLineEdit();
    Hlayout1->addWidget(lab);
    Hlayout1->addWidget(m_LineName);
    Hlayout1->addStretch();

    QHBoxLayout *Hlayout2 = new QHBoxLayout();
    QLabel *lab1 = new QLabel(tr("潜热值:"));
    m_LineValue = new QLineEdit();
    Hlayout2->addWidget(lab1);
    Hlayout2->addWidget(m_LineValue);
    Hlayout2->addStretch();

   /* QHBoxLayout *HPlotlayout = new QHBoxLayout();
    QTCurveDataPlot *tmpCurvePlot = new QTCurveDataPlot(this);
    HPlotlayout->addWidget(tmpCurvePlot);
    m_TCurveDataPlot.push_back(tmpCurvePlot);*/

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

QPhaseLatentHeatDialog::~QPhaseLatentHeatDialog()
{

}
void QPhaseLatentHeatDialog::OkBtnSlot()
{
    this->accept();
}
QString QPhaseLatentHeatDialog::GetEditText()
{
    QString str;
    str = m_LineName->text();
    return str;
}

//--------------------------------------
//--------------比热、导热--------------
//--------------------------------------
QPhasePropDialog::QPhasePropDialog(QWidget *parent)
: QDialog(parent)
{
    //setWindowTitle(tr(""));
    QHBoxLayout *HPlotlayout = new QHBoxLayout();
    m_TCurveDataPlot = new QTCurveDataPlot(this);
    HPlotlayout->addWidget(m_TCurveDataPlot);


    QHBoxLayout *HlayoutLast = new QHBoxLayout();
    m_OKBtn = new QPushButton(tr("确定"));
    m_CancelBtn = new QPushButton(tr("取消"));
    HlayoutLast->addStretch();
    HlayoutLast->addWidget(m_OKBtn);
    HlayoutLast->addWidget(m_CancelBtn);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(HPlotlayout);
    mainLayout->addLayout(HlayoutLast);
    this->setLayout(mainLayout);

    connect(m_OKBtn,SIGNAL(clicked()),this,SLOT(OkBtnSlot()));
    connect(m_CancelBtn,SIGNAL(clicked()),this,SLOT(reject()));

}

QPhasePropDialog::~QPhasePropDialog()
{
 
}
void QPhasePropDialog::OkBtnSlot()
{
    m_TCurveDataPlot->Plot();
    this->accept();
}

int QPhasePropDialog::GetDataNum()
{
    int nRow=m_TCurveDataPlot->m_TabCurveWiget->rowCount();
    int iPointNum=0;
    for (int i=0; i<nRow; i++){
        QString strTmpX,strTmpY;
        strTmpX = m_TCurveDataPlot->m_TabCurveWiget->item(i,0)->text();
        strTmpY = m_TCurveDataPlot->m_TabCurveWiget->item(i,1)->text();
        if (strTmpX==""&&strTmpY=="")continue;//均为"",则无数据   
        iPointNum++;
    }
    return iPointNum;
}

QString QPhasePropDialog::GetPlotData()
{
    int nRow = m_TCurveDataPlot->m_TabCurveWiget->rowCount();
    int nCol = m_TCurveDataPlot->m_TabCurveWiget->columnCount();
    QString strData; QStringList strList;
    for (int i=0; i<nRow; i++){
        QString strTmpX,strTmpY;
        strTmpX = m_TCurveDataPlot->m_TabCurveWiget->item(i,0)->text();
        strTmpY = m_TCurveDataPlot->m_TabCurveWiget->item(i,1)->text();
        if (strTmpX==""&&strTmpY=="")continue;
        strList<<strTmpX+","+strTmpY; 
    }
    return strData=strList.join(";");
}

//--------------------------------------------
//--------孕育期INCUBATIONPERIOD--------------
//--------------------------------------------
QIncubationPeriod::QIncubationPeriod(QWidget *parent)
: QDialog(parent)
{
    setWindowTitle(tr("孕育期"));
 
    QHBoxLayout *HPlotlayout = new QHBoxLayout();
    m_TCurveDataPlot = new QTCurveDataPlot(this);
    HPlotlayout->addWidget(m_TCurveDataPlot);


    QHBoxLayout *HlayoutLast = new QHBoxLayout();
    m_OKBtn = new QPushButton(tr("确定"));
    m_CancelBtn = new QPushButton(tr("取消"));
    HlayoutLast->addStretch();
    HlayoutLast->addWidget(m_OKBtn);
    HlayoutLast->addWidget(m_CancelBtn);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(HPlotlayout);
    mainLayout->addLayout(HlayoutLast);
    this->setLayout(mainLayout);

    connect(m_OKBtn,SIGNAL(clicked()),this,SLOT(OkBtnSlot()));
    connect(m_CancelBtn,SIGNAL(clicked()),this,SLOT(reject()));

    QStringList strList;
    strList<<tr("温度")<<tr("时间");
    m_TCurveDataPlot->ChangeLable(strList);
}

QIncubationPeriod::~QIncubationPeriod()
{

}
void QIncubationPeriod::OkBtnSlot()
{
    m_TCurveDataPlot->Plot();
    this->accept();
}

int QIncubationPeriod::GetDataNum()
{
    int nRow=m_TCurveDataPlot->m_TabCurveWiget->rowCount();
    int iPointNum=0;
    for (int i=0; i<nRow; i++){
        QString strTmpX,strTmpY;
        strTmpX = m_TCurveDataPlot->m_TabCurveWiget->item(i,0)->text();
        strTmpY = m_TCurveDataPlot->m_TabCurveWiget->item(i,1)->text();
        if (strTmpX==""&&strTmpY=="")continue;//均为"",则无数据   
        iPointNum++;
    }
    return iPointNum;
}

QString QIncubationPeriod::GetPlotData()
{
    int nRow = m_TCurveDataPlot->m_TabCurveWiget->rowCount();
    int nCol = m_TCurveDataPlot->m_TabCurveWiget->columnCount();
    QString strData; QStringList strList;
    for (int i=0; i<nRow; i++){
        QString strTmpX,strTmpY;
        strTmpX = m_TCurveDataPlot->m_TabCurveWiget->item(i,0)->text();
        strTmpY = m_TCurveDataPlot->m_TabCurveWiget->item(i,1)->text();
        if (strTmpX==""&&strTmpY=="")continue;
        strList<<strTmpX+","+strTmpY;
    }
    return strData=strList.join(";");
}
//--------------------------------------
//--------------  曲线及显示 -------------
//--------------------------------------
QTCurveDataPlot::QTCurveDataPlot(QWidget *parent)
: QWidget(parent)
{  
    //setMaximumHeight(400);//去掉黄色logo
    m_Hwid1=new QWidget(this);
    m_HwidCommon=new QWidget(this);
    setWindowTitle(QString::fromUtf8("新建"));
    m_Hwid1->setMaximumHeight(300);//去掉黄色logo
    m_HLayout3= new QHBoxLayout(m_HwidCommon);
    m_TabAddBtn = new QPushButton(tr("增加"),m_HwidCommon);
    m_TabDelBtn = new QPushButton(tr("删除"),m_HwidCommon);
    m_TabInsertBtn = new QPushButton(tr("插入"),m_HwidCommon);
    m_ApplyBtn = new QPushButton(tr("绘图"),m_HwidCommon);
    m_HLayout3->addWidget(m_TabAddBtn);
    m_HLayout3->addWidget(m_TabDelBtn);
    m_HLayout3->addWidget(m_TabInsertBtn);
    m_HLayout3->addWidget(m_ApplyBtn);
    m_HLayout3->addStretch();

    m_HLayout2= new QHBoxLayout(m_Hwid1);

    m_TabCurveWiget = new QTableWidget(m_Hwid1);
    m_TabCurveWiget->setColumnCount(2);
    m_TabCurveWiget->setRowCount(1);
    m_TabCurveWiget->verticalHeaderItem(1);
    QTableWidgetItem *itemX =new QTableWidgetItem();  
    QTableWidgetItem *itemY =new QTableWidgetItem();  
    QVariant data=0;
    itemX->setData(Qt::DisplayRole, data.toDouble());
    itemY->setData(Qt::DisplayRole, data.toDouble());
    m_TabCurveWiget->setItem(0,0,itemX);
    m_TabCurveWiget->setItem(0,1,itemY);
    //m_TabCurveWiget->setItem(0,0,new QTableWidgetItem(tr("")));
    //m_TabCurveWiget->setItem(0,1,new QTableWidgetItem(tr("")));
    m_TabCurveWiget->setMinimumWidth(300);
    m_TabCurveWiget->setMinimumHeight(150);
    m_TabCurveWiget->setSelectionMode(QAbstractItemView::ContiguousSelection);
    m_HLayout2->addWidget(m_TabCurveWiget);
    QStringList strList;
    strList<<"x"<<"y";
    m_TabCurveWiget->setHorizontalHeaderLabels(strList);
    m_TabCurveWiget->setFocusPolicy(Qt::NoFocus);
//--menu
    m_TabCurveWiget->setContextMenuPolicy(Qt::CustomContextMenu);
    m_popMenu=new QMenu;
    chartViewers = new QChartViewer(m_Hwid1);
    m_HLayout2->addWidget(chartViewers);
    //chartViewers[i]->hide();
   
    m_MainLayout = new QVBoxLayout(this);
  
    m_MainLayout->addWidget(m_HwidCommon);
    m_MainLayout->addWidget(m_Hwid1);

    setLayout(m_MainLayout);
    Plot();
    
    //connect(m_OKBtn,SIGNAL(clicked()),this,SLOT(CurveOkBtnSlot()));
    m_popMenu->addAction(tr("Copy"),this,SLOT(TableCopyBtnSlot()));//(ExpandAllSlot()));
    m_popMenu->addAction(tr("Paste"),this,SLOT(TablePasteBtnSlot()));//(CollapseAllSlot()));
    m_popMenu->addAction(tr("Clear"),this,SLOT(TableClearRBtnSlot()));
    connect(m_TabCurveWiget, SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(contextTableMenuSlot(const QPoint &)));
    connect(m_ApplyBtn,SIGNAL(clicked()),this,SLOT(CurveApplyBtnSlot()));
    connect(m_TabAddBtn,SIGNAL(clicked()),this,SLOT(CurveAddBtnSlot()));
    connect(m_TabInsertBtn,SIGNAL(clicked()),this,SLOT(CurveInsertBtnSlot()));
    connect(m_TabDelBtn,SIGNAL(clicked()),this,SLOT(CurveDelBtnSlot()));
}
QTCurveDataPlot::QTCurveDataPlot(QWidget *parent, int PlotId)
: QWidget(parent)
{
    
    m_Hwid1=new QWidget(this);
    m_HwidCommon=new QWidget(this);
    m_Hwid1->setMaximumHeight(300);//去掉黄色logo
    setWindowTitle(QString::fromUtf8("新建"));
    m_HLayout3= new QHBoxLayout(m_HwidCommon);
    m_TabAddBtn = new QPushButton(tr("增加"),m_HwidCommon);
    m_TabDelBtn = new QPushButton(tr("删除"),m_HwidCommon);
    m_TabInsertBtn = new QPushButton(tr("插入"),m_HwidCommon);
    m_ApplyBtn = new QPushButton(tr("绘图"),m_HwidCommon);
    m_HLayout3->addWidget(m_TabAddBtn);
    m_HLayout3->addWidget(m_TabDelBtn);
    m_HLayout3->addWidget(m_TabInsertBtn);
    m_HLayout3->addWidget(m_ApplyBtn);
    m_HLayout3->addStretch();

    m_HLayout2= new QHBoxLayout(m_Hwid1);

    m_TabCurveWiget = new QTableWidget(m_Hwid1);
    m_TabCurveWiget->setColumnCount(2);
    m_TabCurveWiget->setRowCount(1);
    m_TabCurveWiget->verticalHeaderItem(1);
    QTableWidgetItem *itemX =new QTableWidgetItem();  
    QTableWidgetItem *itemY =new QTableWidgetItem();  
    QVariant data=0;
    itemX->setData(Qt::DisplayRole, data.toDouble());
    itemY->setData(Qt::DisplayRole, data.toDouble());
    m_TabCurveWiget->setItem(0,0,itemX);
    m_TabCurveWiget->setItem(0,1,itemY);
   /* m_TabCurveWiget->setItem(0,0,new QTableWidgetItem(tr("")));
    m_TabCurveWiget->setItem(0,1,new QTableWidgetItem(tr("")));*/
    m_TabCurveWiget->setMinimumWidth(300);
    m_TabCurveWiget->setMinimumHeight(150);
    m_TabCurveWiget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_HLayout2->addWidget(m_TabCurveWiget);
    QStringList strList;
    strList<<"x"<<"y";
    m_TabCurveWiget->setHorizontalHeaderLabels(strList);
    m_TabCurveWiget->setFocusPolicy(Qt::NoFocus);
    //--menu
    m_TabCurveWiget->setContextMenuPolicy(Qt::CustomContextMenu);
    m_popMenu=new QMenu;

    chartViewers = new QChartViewer(m_Hwid1);
    m_HLayout2->addWidget(chartViewers);
    //chartViewers[i]->hide();

    m_MainLayout = new QVBoxLayout(this);

    m_MainLayout->addWidget(m_HwidCommon);
    m_MainLayout->addWidget(m_Hwid1);

    setLayout(m_MainLayout);
   // Plot();
    //connect(m_OKBtn,SIGNAL(clicked()),this,SLOT(CurveOkBtnSlot()));
    m_popMenu->addAction(tr("Copy"),this,SLOT(TableCopyBtnSlot()));//(ExpandAllSlot()));
    m_popMenu->addAction(tr("Paste"),this,SLOT(TablePasteBtnSlot()));//(CollapseAllSlot()));
    m_popMenu->addAction(tr("Clear"),this,SLOT(TableClearRBtnSlot()));
    
    connect(m_TabCurveWiget, SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(contextTableMenuSlot(const QPoint &)));
    connect(m_ApplyBtn,SIGNAL(clicked()),this,SLOT(CurveApplyBtnSlot()));
    connect(m_TabAddBtn,SIGNAL(clicked()),this,SLOT(CurveAddBtnSlot()));
    connect(m_TabInsertBtn,SIGNAL(clicked()),this,SLOT(CurveInsertBtnSlot()));
    connect(m_TabDelBtn,SIGNAL(clicked()),this,SLOT(CurveDelBtnSlot()));

    m_ApplyBtn->hide();
}
QTCurveDataPlot::~QTCurveDataPlot()
{

}
//应用，将参数写入Inp
void QTCurveDataPlot::CurveApplyBtnSlot()
{
    Plot();
}
//--tab增加行
void QTCurveDataPlot::CurveAddBtnSlot()
{
    int nRow = m_TabCurveWiget->rowCount();
    if (nRow >= 0){
        m_TabCurveWiget->insertRow(nRow);
        for(int j=0;j<m_TabCurveWiget->columnCount();j++){
            QTableWidgetItem *itemX =new QTableWidgetItem();  
            QVariant data;
            itemX->setData(Qt::DisplayRole, data.toDouble());
            m_TabCurveWiget->setItem(nRow,j,itemX);
        }
    }
}
//--tab插入行(当前行的上一行增加)
void QTCurveDataPlot::CurveInsertBtnSlot()
{
    int curRow = m_TabCurveWiget->currentRow();
    if (curRow<0)curRow=0;
    m_TabCurveWiget->insertRow(curRow);
    for(int j=0;j<m_TabCurveWiget->columnCount();j++){
        QTableWidgetItem *itemX =new QTableWidgetItem();  
        QVariant data;
        itemX->setData(Qt::DisplayRole, data.toDouble());
        m_TabCurveWiget->setItem(curRow,j,itemX);
    }
}
//--tab删除行
void QTCurveDataPlot::CurveDelBtnSlot()
{
    int curRow = m_TabCurveWiget->currentRow();

    if (curRow!=-1){
        m_TabCurveWiget->removeRow(curRow);
    }
}
//-绘图(数据进行排序+绘制)
void QTCurveDataPlot::Plot()
{ 
    int nRow=m_TabCurveWiget->rowCount();
    int nCol=m_TabCurveWiget->columnCount();
   // m_TabCurveWiget->setSortingEnabled(true);
    m_TabCurveWiget->sortByColumn(0, Qt::AscendingOrder);//默认第零列是温度，其以升序排列；
    int num=nRow;
    QString *dataTmpX = new QString[num];
    double *dataTmpY = new double[num];   
    for (int i=0; i<nRow; i++){
        dataTmpX[i] = m_TabCurveWiget->item(i,0)->text();
        dataTmpY[i] = m_TabCurveWiget->item(i,1)->text().toDouble();
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
    
    ChartShow2XY(dataX,dataY,imageMap,m_ChartProp);
    dataTmpX = NULL;dataTmpY = NULL;b=NULL;tmp=NULL;
    delete(dataTmpY);delete(dataTmpX);delete(tmp);delete(b);
   //  m_TabCurveWiget->setSortingEnabled(false);
}
void QTCurveDataPlot::ChartShow2XY(StringArray dataX, vector<DoubleArray> dataY, const char *imageMap,_WELCME_CHART_PROP_ prop)
{
    // Each demo module can display a number of charts. We display all of them. 
    MaterialChart *d = materialCharts+0;//demoCharts + 2;//demoIndex;
    for (int tt=0;tt<d->noOfCharts;tt++)
    {
        imageMap = 0;
        chartViewers->setChart(d->propChart(dataX, dataY, &imageMap,prop));
        chartViewers->setImageMap(imageMap);
        delete chartViewers->getChart();
       
    }

    chartViewers->setVisible(d->noOfCharts);
}
//--将当前表头标签进行修改
void QTCurveDataPlot::ChangeLable(QStringList strList)
{
    if(strList.isEmpty())
     strList<<tr("时间")<<tr("温度℃");
     m_TabCurveWiget->setHorizontalHeaderLabels(strList);
     m_ChartProp.xLabel=strList.at(0);m_ChartProp.yLabel=strList.at(1);

}
//--将数据写入Table中
//nRow写入行数，strData写入的数据
//              strData格式：strData={{600,0.1093}{640,0.1014}{680,0.1014}...}
void QTCurveDataPlot::SetData(int nRow, QStringList strData)
{
  /* int nOrigRow= m_TabCurveWiget->rowCount();
   for (int i=0;i<nOrigRow;i++){
       m_TabCurveWiget->removeRow(nOrigRow);
   }*/
    m_TabCurveWiget->clearContents();//清除全部内容
    m_TabCurveWiget->setRowCount(nRow);
    QString DataX,DataY;
    for (int i=0;i<nRow;i++)//按行写入
    {
        QStringList tmpstrList=strData.at(i).split(",");
        DataX=tmpstrList.at(0).simplified();
        DataY=tmpstrList.at(1).simplified();
        QTableWidgetItem *itemX =new QTableWidgetItem();
        itemX->setData(Qt::DisplayRole,DataX.toDouble());
        m_TabCurveWiget->setItem(i,0,itemX);
        QTableWidgetItem *itemY =new QTableWidgetItem();
        itemY->setData(Qt::DisplayRole,DataY.toDouble());
        m_TabCurveWiget->setItem(i,1,itemY);
        //m_TabCurveWiget->setItem(i,1,new QTableWidgetItem(QString::number(DataY.toDouble())));
    }

}

void QTCurveDataPlot::contextTableMenuSlot(const QPoint &p)
{
    m_popMenu->exec(QCursor::pos());
}
void QTCurveDataPlot::TableCopyBtnSlot()
{
    QList<QTableWidgetSelectionRange> ranges=m_TabCurveWiget->selectedRanges();
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
              strSelectText+=m_TabCurveWiget->item(j,kk)->text();
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
void QTCurveDataPlot::TablePasteBtnSlot()
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

    QList<QTableWidgetSelectionRange> ranges=m_TabCurveWiget->selectedRanges();
    int nCount=ranges.count();
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
                    if (mm>=nPasteCol)
                        continue;
                    m_TabCurveWiget->item(j,kk)->setData(Qt::DisplayRole,strColumnList.at(mm).toDouble());    
                } 
            }
        }else{//增加table行数，进行数值填充
            int nRow = m_TabCurveWiget->rowCount();
            int iInSertRow=nPasteRow+topRow-nRow;//需要进行增行数
            for (int i = nRow,tt=0; i<nRow+iInSertRow; i++,tt++){//增加Table行数
                m_TabCurveWiget->insertRow(i);
                for(int j=0;j<=m_TabCurveWiget->columnCount();j++){
                    QTableWidgetItem *itemX =new QTableWidgetItem();  
                    QVariant data;
                    itemX->setData(Qt::DisplayRole, data.toDouble());
                    m_TabCurveWiget->setItem(i,j,itemX);
                }              
            }
            //进行赋值及填充
            for (int j=topRow,tt=0;j<=topRow+nPasteRow;j++,tt++){  
                if (tt>=nPasteRow)continue;
                QStringList strColumnList=strRowList.at(tt).split("\t",QString::SkipEmptyParts);
                for (int kk=leftColumn,mm=0;kk<=rightColumn;kk++,mm++){
                    if (mm>=nPasteCol) continue;
                    m_TabCurveWiget->item(j,kk)->setData(Qt::DisplayRole,strColumnList.at(mm).toDouble());    
                } 
            }
        }
    }
}

void QTCurveDataPlot::TableClearRBtnSlot()//clear 
{
    //m_TableData->clearContents();
    QList<QTableWidgetSelectionRange> ranges=m_TabCurveWiget->selectedRanges();
    int nCount=ranges.count();
    if (nCount<=0)return;

    for (int i=0;i<nCount;i++){
        int topRow=ranges.at(i).topRow();//第几行开始
        int bottomRow=ranges.at(i).bottomRow();//第几行结束
        int leftColumn=ranges.at(i).leftColumn();
        int rightColumn=ranges.at(i).rightColumn();

        for (int j=topRow;j<=bottomRow;j++){
            for (int kk=leftColumn;kk<=rightColumn;kk++){
                m_TabCurveWiget->item(j,kk)->setText("");
            }    
        }
    }
}
//--------------------------------------
//----------------  冷速 ---------------
//--------------------------------------
QPhaseZBFDialog::QPhaseZBFDialog(QWidget *parent)
{
    setWindowTitle(tr("相变积分"));
    QHBoxLayout *Hlayout0 = new QHBoxLayout();
    QLabel *lab0=new QLabel(tr("相:"));
    m_LineEditName=new QLineEdit();
    Hlayout0->addWidget(lab0);
    Hlayout0->addWidget(m_LineEditName);
    Hlayout0->addStretch();


    QHBoxLayout *Hlayout1 = new QHBoxLayout();
    QLabel *lab = new QLabel(tr("冷速序:"));
    m_LineEditCurveTh = new QLineEdit();
    Hlayout1->addWidget(lab);
    Hlayout1->addWidget(m_LineEditCurveTh);
    //Hlayout1->addStretch();
    //QHBoxLayout *Hlayout2 = new QHBoxLayout();
    QLabel *lab1 = new QLabel(tr("冷速:"));
    m_LineEditChangeP = new QLineEdit();
    Hlayout1->addWidget(lab1);
    Hlayout1->addWidget(m_LineEditChangeP);
    Hlayout1->addStretch();

    QHBoxLayout *HPlotlayout = new QHBoxLayout();
    m_TCurveDataPlot = new QTCurveDataPlot(this);
    HPlotlayout->addWidget(m_TCurveDataPlot);
    //m_TCurveDataPlot.push_back(tmpCurvePlot);

    QHBoxLayout *HlayoutLast = new QHBoxLayout();
    m_OKBtn = new QPushButton(tr("确定"));
    m_CancelBtn = new QPushButton(tr("取消"));
    HlayoutLast->addStretch();
    HlayoutLast->addWidget(m_OKBtn);
    HlayoutLast->addWidget(m_CancelBtn);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(Hlayout0);
    mainLayout->addLayout(Hlayout1);
    //mainLayout->addLayout(Hlayout2);
    mainLayout->addLayout(HPlotlayout);
    mainLayout->addLayout(HlayoutLast);
    this->setLayout(mainLayout);

    connect(m_OKBtn,SIGNAL(clicked()),this,SLOT(OkBtnSlot()));
    connect(m_CancelBtn,SIGNAL(clicked()),this,SLOT(reject()));
}

QPhaseZBFDialog::~QPhaseZBFDialog()
{

}
void QPhaseZBFDialog::OkBtnSlot()
{
    this->accept();
}
QString QPhaseZBFDialog::GetEditText()
{
    QString str;
    str = m_LineEditName->text()+"-"+m_LineEditCurveTh->text()+"-"+m_LineEditChangeP->text();
    return str;
}
//相变积分数据点数获取
int QPhaseZBFDialog::GetDataNum()
{
    int nRow=m_TCurveDataPlot->m_TabCurveWiget->rowCount();
    int iPointNum=0;
    for (int i=0; i<nRow; i++){
        QString strTmpX,strTmpY;
        strTmpX = m_TCurveDataPlot->m_TabCurveWiget->item(i,0)->text();
        strTmpY = m_TCurveDataPlot->m_TabCurveWiget->item(i,1)->text();
        if (strTmpX==""&&strTmpY=="")continue;//均为"",则无数据   
        iPointNum++;
    }
    return iPointNum;
}
//
//格式: dataX1,dataY1; dataX2,dataY2; dataX3,dataY3;
//
QString QPhaseZBFDialog::GetPlotData()
{
    int nRow = m_TCurveDataPlot->m_TabCurveWiget->rowCount();
    int nCol = m_TCurveDataPlot->m_TabCurveWiget->columnCount();
    QString strData; QStringList strList;
    for (int i=0; i<nRow; i++){
        QString strTmpX,strTmpY;
        strTmpX = m_TCurveDataPlot->m_TabCurveWiget->item(i,0)->text();
        strTmpY = m_TCurveDataPlot->m_TabCurveWiget->item(i,1)->text();
        if (strTmpX==""&&strTmpY=="")continue;
        strList<<strTmpX+","+strTmpY;
    }
    return strData=strList.join(";");
}

QAddTab::QAddTab(QWidget *parent)
: QWidget(parent)
{
    QWidget *TmpTab = new QWidget(); 
    QLabel *tmpLab = new QLabel(tr("转变分数"));
    m_Edit= new QLineEdit(tr("0.01"));
    m_TabData = new QTCurveDataPlot(this,1);
    QVBoxLayout *Gridlayout3 = new QVBoxLayout(TmpTab);
    Gridlayout3->addWidget(tmpLab);
    Gridlayout3->addWidget(m_Edit);
    Gridlayout3->addWidget(m_TabData);
    setLayout(Gridlayout3);
}
QAddTab::~QAddTab()
{

}
