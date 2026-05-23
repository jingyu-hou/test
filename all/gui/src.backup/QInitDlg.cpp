#include "QInitDlg.h"

QInitDlg::QInitDlg(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("初始化");
    QWidget *Hwid1 = new QWidget(this);
    QHBoxLayout *Hlayout = new QHBoxLayout();
    m_ApplyBtn = new QPushButton(tr("应用"));
    m_AddBtn = new QPushButton(tr("增加"));
    m_DelBtn = new QPushButton(tr("删除"));
    m_CancelBtn = new QPushButton(tr("取消"));
    Hlayout->addWidget(m_ApplyBtn);
    Hlayout->addWidget(m_AddBtn);
    Hlayout->addWidget(m_DelBtn);
    Hlayout->addWidget(m_CancelBtn);


    QVBoxLayout *VLayout = new QVBoxLayout();
    m_TableWiget = new QTableWidget();
    m_TableWiget->setColumnCount(3);
    m_TableWiget->setRowCount(1);
    m_TableWiget->verticalHeaderItem(1);
    QStringList strList;
    strList<<"变量名"<<"集合"<<"数值";
    m_TableWiget->setHorizontalHeaderLabels(strList);

    //温度场
    QTableWidgetItem *tmpItem=new QTableWidgetItem(tr("温度场"));
    m_TableWiget->setItem(0,0,tmpItem);
    ////晶粒度
    QTableWidgetItem *tmpItem2=new QTableWidgetItem(tr("晶粒度"));
     ////m_TableWiget->setItem(1,0,tmpItem2);
    ////晶粒度尺寸
    //tmpItem2=new QTableWidgetItem(tr("晶粒度尺寸"));
    //m_TableWiget->setItem(2,0,tmpItem2);
    ////粉末密度
    //tmpItem2=new QTableWidgetItem(tr("粉末密度"));
    //m_TableWiget->setItem(3,0,tmpItem2);
    ////球形度
    //tmpItem2=new QTableWidgetItem(tr("球形度"));
    //m_TableWiget->setItem(4,0,tmpItem2);
    ////粒径
    //tmpItem2=new QTableWidgetItem(tr("粒径"));
    //m_TableWiget->setItem(5,0,tmpItem2);
    
    for (int i=0;i<1;i++){
      tmpItem2=new QTableWidgetItem(tr(""));
      m_TableWiget->setItem(i,2,tmpItem2);
    }
    

    for (int i=0; i<1; i++){
        MyCombox* TmpcomboxPset = new MyCombox(this,i);
        TmpcomboxPset->addItem("Point Set1");
        TmpcomboxPset->addItem("Point Set2");
        TmpcomboxPset->addItem("新建");
        TmpcomboxPset->setFrame(false);
        m_TableWiget->setCellWidget(i,1,TmpcomboxPset);
        m_comboxPset.push_back(TmpcomboxPset);
        //connect(m_comboxPset[i],SIGNAL(clicked(int,int)),this,SLOT(comBoxChgSlot(int,int)));
        //connect(m_comboxPset[i],SIGNAL(doubleclicked(int,int)),this,SLOT(comBoxDoubleSlot(int,int)));
    }
  
    VLayout->addWidget(m_TableWiget);
    QVBoxLayout *MainLayout = new QVBoxLayout(this);
    MainLayout->addLayout(VLayout);
    MainLayout->addLayout(Hlayout);
    setLayout(MainLayout);
    connect(m_ApplyBtn,SIGNAL(clicked()),this,SLOT(ApplyBtnSlot()));
    connect(m_AddBtn,SIGNAL(clicked()),this,SLOT(AddBtnSlot()));
    connect(m_CancelBtn,SIGNAL(clicked()),this,SLOT(CancelBtnSlot()));
    connect(m_DelBtn,SIGNAL(clicked()),this,SLOT(DelBtnSlot()));
}

QInitDlg::~QInitDlg()
{

}
//将读取的INP数据显示在当前的表格中
void QInitDlg::SetInpData(ReadInpResultS ReadInpData)
{
    int nRowSize = m_TableWiget->rowCount();
    int nInitRowSize = ReadInpData.TmpInitInps.size();

    QStringList PSetList=ReadInpData.TmpNsetInps.strNSetName;

    if (nRowSize>=nInitRowSize)
    { 
        //for (int kk=0; kk<nInitRowSize; kk++){
         //   PSetList<<ReadInpData.TmpInitInps.at(kk).strPSetName;
       // } 
        for (int i=0;i<nRowSize;i++){
            m_comboxPset.at(i)->clear();
            m_comboxPset.at(i)->addItems(PSetList);
            m_comboxPset.at(i)->addItem("新建");
            if (ReadInpData.TmpInitInps.size()>0){
                QString strTmpName = ReadInpData.TmpInitInps.at(i).strPSetName;
                int id=m_comboxPset.at(i)->findText(strTmpName);
                m_comboxPset.at(i)->setCurrentIndex(id);   

                QString strVal=ReadInpData.TmpInitInps.at(i).strVal;
                QTableWidgetItem * tmpItem=new QTableWidgetItem(strVal);
                m_TableWiget->setItem(i,2,tmpItem);
            }
            connect(m_comboxPset.at(i),SIGNAL(clicked(int,int)),this,SLOT(comBoxChgSlot(int,int)));
            
        }   
    }

    WriteToInps();
}
void QInitDlg::ApplyBtnSlot()
{
   WriteToInps();
}
void QInitDlg::WriteToInps()
{
    InitInpS initNode;
    int TabRowCnt=m_TableWiget->rowCount();
    int TabColCnt=m_TableWiget->columnCount();
    if (TabRowCnt>0){m_nodeInitList.clear();}

    QString strName;
    for(int i=0; i<TabRowCnt; i++){
        initNode.strCommentaryName = "初始化";
        initNode.strInitCondition="Initial Conditions";
        strName = m_TableWiget->item(i,0)->text();
        if (strName =="温度场"){initNode.strInitTypeName="TEMPERATURE";
        }else if (strName =="晶粒度尺寸"){initNode.strInitTypeName="CRYSTALSIZE";
        }else if (strName =="晶粒度"){initNode.strInitTypeName="CRYSTAL";
        }else if (strName =="粉末密度"){initNode.strInitTypeName="DUSTDENSITY";
        }else if (strName =="球形度"){initNode.strInitTypeName="SPHERICITY";
        }else if (strName =="粒径"){initNode.strInitTypeName="GRAINRADIUS";
        }
        initNode.strPSetName=m_comboxPset.at(i)->currentText();
        initNode.strVal = m_TableWiget->item(i,2)->text();
        m_nodeInitList.append(initNode);
    }
}
void QInitDlg::CancelBtnSlot()
{

}
void QInitDlg::AddBtnSlot()
{
    //--插入
    //int curRow = m_TableWiget->currentRow();
    // m_TableWiget->insertRow(curRow);
    //--增加
    int nRow = m_TableWiget->rowCount();
 
    m_TableWiget->setRowCount(nRow+1);
    QTableWidgetItem *tmpItem2=new QTableWidgetItem(tr("Name"));
    m_TableWiget->setItem(nRow,0,tmpItem2);

    tmpItem2=new QTableWidgetItem(tr(""));
    m_TableWiget->setItem(nRow,2,tmpItem2);
    if (nRow>=1){
        MyCombox *tmpCombox = new MyCombox(this,nRow);
        tmpCombox->setFrame(false);
        //m_comboxPset.at(nRow) = new MyCombox(this,nRow);
        //m_comboxPset.at(nRow)->setFrame(false);
        
        QStringList strTmpList;
        for (int i=0; i<m_comboxPset.at(0)->count(); i++){
            strTmpList<<m_comboxPset.at(0)->itemText(i);
        }
        tmpCombox->addItems(strTmpList);

        m_TableWiget->setCellWidget(nRow,1,tmpCombox);
        m_comboxPset.push_back(tmpCombox);
        connect(m_comboxPset.at(nRow),SIGNAL(clicked(int,int)),this,SLOT(comBoxChgSlot(int,int)));
        //connect(m_comboxPset.at(nRow),SIGNAL(doubleclicked(int,int)),this,SLOT(comBoxDoubleSlot(int,int)));
    }else{
        MyCombox *tmpCombox = new MyCombox(this,nRow);
        tmpCombox->addItem("新建");
        tmpCombox->setFrame(false);
        m_TableWiget->setCellWidget(nRow,1,tmpCombox);
        m_comboxPset.push_back(tmpCombox);
        connect(m_comboxPset.at(nRow),SIGNAL(clicked(int,int)),this,SLOT(comBoxChgSlot(int,int)));
       // connect(m_comboxPset.at(nRow),SIGNAL(doubleclicked(int,int)),this,SLOT(comBoxDoubleSlot(int,int)));
    }
   
    

}

void QInitDlg::DelBtnSlot()
{
    int curRow = m_TableWiget->currentRow();

    if (curRow!=-1){
        m_TableWiget->removeRow(curRow);
    }
}

void QInitDlg::comBoxChgSlot(int comWigId,int index)
{
    QString strIdName;
    int Row = comWigId;//= m_TableWiget->currentRow();

    if (Row <0){return;}

    strIdName = m_comboxPset.at(Row)->currentText();
    if (strIdName.toLower() =="新建")
    {
       // m_comboxPset[Row]->setEditable(true);

    }
    if(strIdName=="")return;
    //--设置属性
    NElSurfChsS tmpChs;
    tmpChs.strName =strIdName;
    tmpChs.iType=COM_ENUM_NSET;
    emit emitPsetHighLight(tmpChs);

}
void QInitDlg::comBoxDoubleSlot(int, int)
{

}

//--psetdlg create point set;
//--
void QInitDlg::AppendPSetComboxIniSlot(QString str)
{
    int nRow =m_TableWiget->rowCount();
    if(m_PSetList.indexOf(str) == -1){
        m_PSetList.append(str);
        for (int i=0;i<nRow;i++){
            MyCombox *TmpcomboxPset = (MyCombox*)m_TableWiget->cellWidget(i, 1);//new MyCombox(this, nRow);
            TmpcomboxPset->addItem(str);      
        }
    }
    
}

