#include "QBCDlg.h"

QBCDlg::QBCDlg(QWidget *parent)
      : QDialog(parent)
{
    setWindowTitle("边界设置");
    QWidget *Hwid1 = new QWidget(this);
    QHBoxLayout *Hlayout = new QHBoxLayout();
    m_ApplyBtn = new QPushButton(tr("应用"));
    m_AddBtn = new QPushButton(tr("增加"));
    m_DelBtn = new QPushButton(tr("删除"));
    Hlayout->addWidget(m_ApplyBtn);
    Hlayout->addWidget(m_AddBtn);
    Hlayout->addWidget(m_DelBtn);

    QVBoxLayout *VLayout = new QVBoxLayout();
    m_TableWiget = new QTableWidget();
    m_TableWiget->setColumnCount(2);
    m_TableWiget->setRowCount(2);
    m_TableWiget->verticalHeaderItem(1);
    QStringList strList;
    strList<<"位移"<<"点集合";
    m_TableWiget->setHorizontalHeaderLabels(strList);

   /* QTableWidgetItem *tmpItem=new QTableWidgetItem(tr("x向固定"));
    m_TableWiget->setItem(0,0,tmpItem);

    QTableWidgetItem *tmpItem2=new QTableWidgetItem(tr("y向固定"));
    m_TableWiget->setItem(1,0,tmpItem2);*/
    for (int i=0; i<2;i++)
    {
        MyCombox *TmpcomboxPset=new MyCombox(this, i);
        TmpcomboxPset->addItem("Point Set1");
        TmpcomboxPset->addItem("Point Set2");
        TmpcomboxPset->setFrame(false);
        m_TableWiget->setCellWidget(i,1,TmpcomboxPset);
        m_comboxPset.push_back(TmpcomboxPset);

        MyCombox *Tmpcombox=new MyCombox(this, i);
        Tmpcombox->addItem("x向固定");
        Tmpcombox->addItem("y向固定");
        Tmpcombox->addItem("z向固定");
        Tmpcombox->setFrame(false);
        m_TableWiget->setCellWidget(i,0,Tmpcombox);
        m_comboxStyle.push_back(Tmpcombox);
        //connect(m_comboxPset.at(i),SIGNAL(clicked(int,int)),this,SLOT(comBoxSlot(int,int)));
    }
   
    VLayout->addWidget(m_TableWiget);
    QVBoxLayout *MainLayout = new QVBoxLayout(this);
    MainLayout->addLayout(VLayout);
    MainLayout->addLayout(Hlayout);
    setLayout(MainLayout);
    connect(m_ApplyBtn,SIGNAL(clicked()),this,SLOT(ApplyBtnSlot()));
    connect(m_AddBtn,SIGNAL(clicked()),this,SLOT(AddBtnSlot()));
    connect(m_DelBtn,SIGNAL(clicked()),this,SLOT(DelBtnSlot()));
    
}

QBCDlg::~QBCDlg()
{

}

//将读取的INP数据显示在当前的表格中
void QBCDlg::SetInpData(ReadInpResultS ReadInpData)
{
    int nRowSize = m_TableWiget->rowCount();
    int nBcRowSize = ReadInpData.TmpBCInps.size();
    if (nRowSize<nBcRowSize){//读取的多于现有的创建Tab的行数，则增加行数
        m_TableWiget->setRowCount(nBcRowSize);
        for (int i=nRowSize; i<nBcRowSize;i++)
        {
            MyCombox *TmpcomboxPset=new MyCombox(this, i);
            TmpcomboxPset->setFrame(false);
            m_TableWiget->setCellWidget(i,1,TmpcomboxPset);
            m_comboxPset.push_back(TmpcomboxPset);

            MyCombox *Tmpcombox=new MyCombox(this, i);
            Tmpcombox->addItem("x向固定");
            Tmpcombox->addItem("y向固定");
            Tmpcombox->addItem("z向固定");
            Tmpcombox->setFrame(false);
            m_TableWiget->setCellWidget(i,0,Tmpcombox);
            m_comboxStyle.push_back(Tmpcombox);
        }
        
    }
    nRowSize = m_TableWiget->rowCount();

    if (nRowSize >= nBcRowSize)
    {
       QStringList PSetList;
       PSetList=ReadInpData.TmpNsetInps.strNSetName;

       for (int i=0;i<nRowSize;i++){        
           m_comboxPset.at(i)->clear();
           m_comboxPset.at(i)->addItems(PSetList);
           m_comboxPset.at(i)->addItem("新建");
		   connect(m_comboxPset.at(i),SIGNAL(clicked(int,int)),this,SLOT(comBoxSlot(int,int)));
       } 
          
        for (int j=0;j<ReadInpData.TmpBCInps.size();j++)   
        {  
            QString strTmpName = ReadInpData.TmpBCInps.at(j).strPSetName;
            int id=m_comboxPset.at(j)->findText(strTmpName);
            m_comboxPset.at(j)->setCurrentIndex(id);  
        }
           
      
       QStringList comboxStyleList;
       for (int i=0;i<nBcRowSize;i++){//得到列表，并判断是否重复出现
           QString tmp=ReadInpData.TmpBCInps.at(i).strURstyle;
           if(tmp=="1,1"){
              tmp=tr("x向固定");
           }else if (tmp=="2,2"){
               tmp=tr("y向固定");
           }else if (tmp=="3,3"){
               tmp=tr("z向固定"); 
           }
           //if(comboxStyleList.indexOf(tmp)==-1){
               comboxStyleList<<tmp;//}
       }
       for (int kk=0;kk<comboxStyleList.size();kk++){
           QString strTmpName =comboxStyleList.at(kk);
           int id=m_comboxStyle.at(kk)->findText(strTmpName);
           m_comboxStyle.at(kk)->setCurrentIndex(id);  
       }  
    }
    WriteToBcInps();
}
void QBCDlg::WriteToBcInps()
{
    BCInpS nodeBCList;
    int TabRowCnt=m_TableWiget->rowCount();
    int TabColCnt=m_TableWiget->columnCount();
    if (TabRowCnt>0){m_nodeBCList.clear();}
    for(int i=0; i<TabRowCnt; i++){
        nodeBCList.strCommentaryName="边界条件";
        nodeBCList.strBoundaryName="Boundary";
        nodeBCList.strPSetName=m_comboxPset.at(i)->currentText();
        QString str = m_comboxStyle.at(i)->currentText();
        if (str=="x向固定"){
            nodeBCList.strURstyle="1,1";
        }else if (str=="y向固定"){
            nodeBCList.strURstyle="2,2";
        }else if (str=="z向固定"){
            nodeBCList.strURstyle="3,3";
        }

        m_nodeBCList.append(nodeBCList);
    }
}
void QBCDlg::ApplyBtnSlot()
{  
    WriteToBcInps();
}

void QBCDlg::AddBtnSlot()//增加行
{
    int nRow =m_TableWiget->rowCount();
    m_TableWiget->insertRow(nRow);


    MyCombox *TmpcomboxPset=new MyCombox(this, nRow);
    for (int i=0;i<m_comboxPset.at(0)->count();i++){//遍历所有，进行显示；
        TmpcomboxPset->addItem(m_comboxPset.at(0)->itemText(i));
    } 
    connect(TmpcomboxPset,SIGNAL(clicked(int,int)),this,SLOT(comBoxSlot(int,int)));
    TmpcomboxPset->setFrame(false);
    m_TableWiget->setCellWidget(nRow,1,TmpcomboxPset);
    m_comboxPset.push_back(TmpcomboxPset);

    MyCombox *Tmpcombox=new MyCombox(this, nRow);
    Tmpcombox->addItem("x向固定");
    Tmpcombox->addItem("y向固定");
    Tmpcombox->addItem("z向固定");
    Tmpcombox->setFrame(false);
    m_TableWiget->setCellWidget(nRow,0,Tmpcombox);
    m_comboxStyle.push_back(Tmpcombox);
    
}
void QBCDlg::DelBtnSlot()//删除行
{
    int curRow =m_TableWiget->currentRow();
    if (curRow!=-1){
        m_TableWiget->removeRow(curRow);
    }
   
}
//--
//1.index:List中第几个
//2.comWigId:第几个ComboBox
//--
void QBCDlg::comBoxSlot(int comWigId,int index)
{
    if (index<0){return;}
    //--
    QString str = m_comboxPset.at(comWigId)->currentText();//当前所选combox
   if (str=="")return;
    //--设置属性
    NElSurfChsS tmpChs;
    tmpChs.strName =str;
    tmpChs.iType=COM_ENUM_NSET;
    emit emitPsetHighLight(tmpChs);
}
