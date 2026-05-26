#include "QResolveDlg.h"

QResolveDlg::QResolveDlg(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(QString::fromUtf8("求解器设置"));
    QWidget *Hwid1 = new QWidget(this);
    QHBoxLayout *Hlayout = new QHBoxLayout();
    QVBoxLayout *Vlayout = new QVBoxLayout();
    m_OKBtn = new QPushButton(tr("应用"));
    m_CancelBtn = new QPushButton(tr("取消"));
    m_PreBtn = new QPushButton(tr("上一步"));
    m_NextBtn = new QPushButton(tr("下一步"));
    m_TableWig = new QTableWidget();
    m_TableWig->setColumnCount(7);
    m_TableWig->setRowCount(1);
    m_TableWig->verticalHeaderItem(1);
    QStringList strList;
    strList<<"模拟总时长"<<"初始步长"<<"最小步长"<<"最大步长"<<"最大增量步数"<<"最大温度增量"<<"耦合算法";
    m_TableWig->setHorizontalHeaderLabels(strList);
    QTableWidgetItem *tmpItem1=new QTableWidgetItem(tr("1000"));
    m_TableWig->setItem(0,0,tmpItem1);
    QTableWidgetItem *tmpItem2=new QTableWidgetItem(tr("10"));
    m_TableWig->setItem(0,1,tmpItem2);
    QTableWidgetItem *tmpItem3=new QTableWidgetItem(tr("0.01"));
    m_TableWig->setItem(0,2,tmpItem3);
    QTableWidgetItem *tmpItem4=new QTableWidgetItem(tr("20"));
    m_TableWig->setItem(0,3,tmpItem4);
    QTableWidgetItem *tmpItem5=new QTableWidgetItem(tr("50"));
    m_TableWig->setItem(0,4,tmpItem5);
    QTableWidgetItem *tmpItem6=new QTableWidgetItem(tr("1000"));
    m_TableWig->setItem(0,5,tmpItem6);

    m_combox = new QComboBox();
    m_combox->addItem("完全耦合");
    m_combox->addItem("不完全耦合");
    m_combox->setFrame(false);
    m_TableWig->setCellWidget(0,6,m_combox);

    Vlayout->addWidget(m_TableWig); 
    Hlayout->addWidget(m_PreBtn);
    Hlayout->addWidget(m_CancelBtn);
    Hlayout->addWidget(m_OKBtn);
    Hlayout->addWidget(m_NextBtn);
    //Hlayout->addWidget(m_SectionBtn);
    //Hlayout->addWidget(m_AlloyBtn);
    QVBoxLayout *MainLayout = new QVBoxLayout(this);
 
    MainLayout->addLayout(Vlayout);  
    MainLayout->addLayout(Hlayout);
    setLayout(MainLayout);
     this->setMinimumWidth(750);
    connect(m_OKBtn,SIGNAL(clicked()),this,SLOT(ApplayBtnSlot()));
    connect(m_NextBtn,SIGNAL(clicked()),this,SLOT(NextBtnSlot()));
    m_PreBtn->hide();
    m_NextBtn->hide();
}

QResolveDlg::~QResolveDlg()
{

}

//--应用
void QResolveDlg::ApplayBtnSlot()
{
    WriteSloveToInp();
    accept();
}
void QResolveDlg::WriteSloveToInp()
{
    HIPSolveInpS nodeHIPList;
    int TabRowCnt=m_TableWig->rowCount();
    int TabColCnt=m_TableWig->columnCount();
    if (TabRowCnt>0){m_nodeHIPSolveList.clear();}
    nodeHIPList.strStepInc=m_TableWig->item(0,4)->text();
    int comIndex=m_combox->currentIndex();
    if ( m_combox->currentIndex()==0){
        nodeHIPList.strNlgeom = "YES";
        nodeHIPList.strNlgeomSTR = "coupled Temperature-displacement";
    }else if(m_combox->currentIndex()==1){
        nodeHIPList.strNlgeom = "NO";
        nodeHIPList.strNlgeomSTR = "uncoupled Temperature-displacement";
	}else if(m_combox->currentIndex()==2){
		nodeHIPList.strNlgeom = "YES";
		nodeHIPList.strNlgeomSTR = "Dynamic,Direct,Explicit";
    }
    nodeHIPList.strDelMX = m_TableWig->item(0,5)->text();
    nodeHIPList.strInitStep = m_TableWig->item(0,1)->text();
    nodeHIPList.strTotalTime =  m_TableWig->item(0,0)->text();
    nodeHIPList.strMinStep = m_TableWig->item(0,2)->text();
    nodeHIPList.strMaxStep = m_TableWig->item(0,3)->text();
    m_nodeHIPSolveList.append(nodeHIPList);
}
void QResolveDlg::NextBtnSlot()
{
   

}
//将读取的INP数据显示在当前的表格中
void QResolveDlg::SetInpData(ReadInpResultS ReadInpData)
{
    int nRowSize = m_TableWig->rowCount();
    int nSolveRowSize = ReadInpData.TmpSolveInps.size();
    if (nSolveRowSize<=0){return;}
    //只有一行
    QString strTmp=ReadInpData.TmpSolveInps.at(0).strTotalTime;
    QTableWidgetItem * tmpItem=new QTableWidgetItem(strTmp);
    m_TableWig->setItem(0,0,tmpItem);
    strTmp=ReadInpData.TmpSolveInps.at(0).strInitStep;
    tmpItem=new QTableWidgetItem(strTmp);
    m_TableWig->setItem(0,1,tmpItem);
    strTmp=ReadInpData.TmpSolveInps.at(0).strMinStep;
    tmpItem=new QTableWidgetItem(strTmp);
    m_TableWig->setItem(0,2,tmpItem);
    strTmp=ReadInpData.TmpSolveInps.at(0).strMaxStep;
    tmpItem=new QTableWidgetItem(strTmp);
    m_TableWig->setItem(0,3,tmpItem);

    strTmp=ReadInpData.TmpSolveInps.at(0).strStepInc;//最大增益步数
    tmpItem=new QTableWidgetItem(strTmp);
    m_TableWig->setItem(0,4,tmpItem);
    strTmp=ReadInpData.TmpSolveInps.at(0).strDelMX;//最大温度增益
    tmpItem=new QTableWidgetItem(strTmp);
    m_TableWig->setItem(0,5,tmpItem);
    strTmp=ReadInpData.TmpSolveInps.at(0).strNlgeom;
    if (strTmp=="YES"){
        m_combox->setCurrentIndex(0);
    }else{
        m_combox->setCurrentIndex(1);
    }
    WriteSloveToInp();
}
