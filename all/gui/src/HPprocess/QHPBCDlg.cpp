#include "QHPBCDlg.h"

QHPBCDlg::QHPBCDlg(QWidget *parent)
      : QDialog(parent)
{
    setWindowTitle(QString::fromUtf8("固定边界"));
    QWidget *Hwid1 = new QWidget(this);
    QHBoxLayout *Hlayout = new QHBoxLayout();
    QHBoxLayout *Hlayout2 = new QHBoxLayout();
    m_ApplyBtn = new QPushButton(tr("应用"));
    m_AddBtn = new QPushButton(tr("增加"));
    m_DelBtn = new QPushButton(tr("删除"));
    m_PreBCBtn = new QPushButton(tr("上一步"));
    m_NextBCBtn = new QPushButton(tr("下一步"));
    Hlayout->addStretch();
    Hlayout->addWidget(m_AddBtn);
    Hlayout->addWidget(m_DelBtn);
    Hlayout->addStretch();
    Hlayout2->addWidget(m_ApplyBtn);
    Hlayout2->addWidget(m_PreBCBtn);
    Hlayout2->addWidget(m_NextBCBtn);

    QVBoxLayout *VLayout = new QVBoxLayout();
    m_TableWiget = new QTableWidget();
    m_TableWiget->setColumnCount(3);
    m_TableWiget->setRowCount(0);
    m_TableWiget->verticalHeaderItem(1);
    QStringList strList;
    //strList<<"位移"<<"点集合";
	strList<<"部件"<<"位移"<<"点集合";
    m_TableWiget->setHorizontalHeaderLabels(strList);
	/*for (int j=0;j<strList.size();j++){
		m_TableWiget->horizontalHeader()->setResizeMode(j,QHeaderView::ResizeToContents);
	}*/

   /* QTableWidgetItem *tmpItem=new QTableWidgetItem(tr("x向固定"));
    m_TableWiget->setItem(0,0,tmpItem);

    QTableWidgetItem *tmpItem2=new QTableWidgetItem(tr("y向固定"));
    m_TableWiget->setItem(1,0,tmpItem2);*/
  //  for (int i=0; i<5;i++)
  //  {
		//QTableWidgetItem *tmpItem=new QTableWidgetItem(tr("部件"));
  //      m_TableWiget->setItem(i,0,tmpItem);

  //      MyCombox *TmpcomboxPset=new MyCombox(this, i);
  //      /*TmpcomboxPset->addItem("Point Set1");
  //      TmpcomboxPset->addItem("Point Set2");*/
  //      TmpcomboxPset->setFrame(false);
  //      m_TableWiget->setCellWidget(i,2,TmpcomboxPset);
  //      m_comboxPset.insert(i,TmpcomboxPset);//.push_back(TmpcomboxPset);


  //      MyCombox *Tmpcombox=new MyCombox(this, i);
  //      Tmpcombox->addItem("x向固定");
  //      Tmpcombox->addItem("y向固定");
  //      Tmpcombox->addItem("z向固定");
		//Tmpcombox->addItem("固定");
  //      Tmpcombox->setFrame(false);
  //      m_TableWiget->setCellWidget(i,1,Tmpcombox);
  //      m_comboxStyle.insert(i,Tmpcombox);//push_back(Tmpcombox);
  //      //connect(m_comboxPset.at(i),SIGNAL(clicked(int,int)),this,SLOT(comBoxSlot(int,int)));
  //  }

    m_TableWiget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	//m_TableWiget->setColumnWidth(0, 100);
	m_TableWiget->horizontalHeader()->setResizeMode(0, QHeaderView::Interactive);
	m_TableWiget->horizontalHeader()->setResizeMode(1, QHeaderView::Interactive);
	
	
    VLayout->addWidget(m_TableWiget);
    QVBoxLayout *MainLayout = new QVBoxLayout(this);
    MainLayout->addLayout(VLayout);
    MainLayout->addLayout(Hlayout);
    MainLayout->addLayout(Hlayout2);
    setLayout(MainLayout);
    connect(m_ApplyBtn,SIGNAL(clicked()),this,SLOT(ApplyBtnSlot()));
    connect(m_AddBtn,SIGNAL(clicked()),this,SLOT(AddBtnSlot()));
    connect(m_DelBtn,SIGNAL(clicked()),this,SLOT(DelBtnSlot()));
    connect(m_NextBCBtn,SIGNAL(clicked()),this,SLOT(NextBtnSlot()));
    connect(m_PreBCBtn,SIGNAL(clicked()),this,SLOT(PreBtnSlot()));  
	this->setMinimumWidth(100);
	
}

void QHPBCDlg::ShowBC(int styleB)
{
    QStringList strList;
    strList.clear();
    strList<<"部件"<<"位移"<<"点集合";
    m_TableWiget->setHorizontalHeaderLabels(strList);
	int NRow=m_TableWiget->rowCount();
	int Number=m_nodeBCList.size();
    if (styleB != 2&&Number==0){//热等静压 ;热处理;
		m_TableWiget->setColumnHidden(0,true);
		if(NRow>=1)m_TableWiget->setRowHidden(0,false);
		if(NRow>=2)m_TableWiget->setRowHidden(1,true);
		if(NRow>=3)m_TableWiget->setRowHidden(2,true);
		if(NRow>=4)m_TableWiget->setRowHidden(3,true);
		if(NRow>=5)m_TableWiget->setRowHidden(4,true);  
	}else if(styleB != 2&&Number!=0){
		m_TableWiget->setColumnHidden(0,true);
	}else if(styleB == 2&&Number==0&&NRow>0){//锻造
       if(NRow>=1)m_TableWiget->item(0,0)->setText(tr("模具1"));
	   if(NRow>=2)m_TableWiget->item(1,0)->setText(tr("锻造件"));
	   if(NRow>=3)m_TableWiget->item(2,0)->setText(tr("锻造件"));
	   if(NRow>=4)m_TableWiget->item(3,0)->setText(tr("锻造件"));
	   if(NRow>=5)m_TableWiget->item(4,0)->setText(tr("模具2"));
	   m_TableWiget->setColumnHidden(0,false);
	   if(NRow>=1)m_TableWiget->setRowHidden(0,false);
	   if(NRow>=2)m_TableWiget->setRowHidden(1,false);
	   if(NRow>=3)m_TableWiget->setRowHidden(2,false);
	   if(NRow>=4)m_TableWiget->setRowHidden(3,false);
	   if(NRow>=5)m_TableWiget->setRowHidden(4,false);
	}
}


QHPBCDlg::~QHPBCDlg()
{
    m_comboxPset.clear();
    m_comboxStyle.clear();
    m_PSetList.clear();
}

//将读取的INP数据显示在当前的表格中
void QHPBCDlg::SetInpData(ReadInpResultS ReadInpData)
{
    int nRowSize = m_TableWiget->rowCount();
    int nBcRowSize =0;
	int nBcRowSize1 =ReadInpData.TmpBCInps.size();
	for(int i=0;i<nBcRowSize1;i++){
		QString str=ReadInpData.TmpBCInps.at(i).SportType;
		QString str1=ReadInpData.TmpBCInps.at(i).strURstyle;
		QString str2=ReadInpData.TmpBCInps.at(i).SolutionName;
		if(str=="无"&&str1!="11,11"&&str2=="")nBcRowSize++;
	}
    if (nRowSize<nBcRowSize){//读取的多于现有的创建Tab的行数，则增加行数
        m_TableWiget->setRowCount(nBcRowSize);
        for (int i=nRowSize; i<nBcRowSize;i++)
        {
			QTableWidgetItem *tmpItem=new QTableWidgetItem(tr("部件"));
			m_TableWiget->setItem(i,0,tmpItem);
            MyCombox *TmpcomboxPset=new MyCombox(this, i);
            TmpcomboxPset->setFrame(false);
            m_TableWiget->setCellWidget(i,2,TmpcomboxPset);
            m_comboxPset.insert(i,TmpcomboxPset);//.push_back(TmpcomboxPset);

            MyCombox *Tmpcombox=new MyCombox(this, i);
            Tmpcombox->addItem("x向固定");
            Tmpcombox->addItem("y向固定");
            Tmpcombox->addItem("z向固定");
			Tmpcombox->addItem("固定");
            Tmpcombox->setFrame(false);
            m_TableWiget->setCellWidget(i,1,Tmpcombox);
            m_comboxStyle.insert(i,Tmpcombox);//push_back(Tmpcombox);
        } 
    }else if (nRowSize>nBcRowSize&&nBcRowSize>0){
        for (int i=nBcRowSize;i<nRowSize;i++){
            m_TableWiget->removeRow(nBcRowSize);
        }
    }
    nRowSize = m_TableWiget->rowCount();
    //--
    m_PSetList=ReadInpData.TmpNsetInps.strNSetName;
    if (nRowSize >= nBcRowSize)
    {
       //QStringList m_PSetList;
       //PSetList=ReadInpData.TmpNsetInps.strNSetName;
       

       for (int i=0;i<nRowSize;i++){ 
           MyCombox *TmpComboxPset=(MyCombox*)m_TableWiget->cellWidget(i,ENUM_BC_PSET);
           TmpComboxPset->clear();
           TmpComboxPset->addItems(m_PSetList);
           TmpComboxPset->addItem("新建");
           m_comboxPset.insert(i,TmpComboxPset);
		   connect(m_comboxPset[i],SIGNAL(clicked(int,int)),this,SLOT(comBoxSlot()));
       } 
        int Nrow=0; 
        for (int j=0;j<nBcRowSize1;j++)
        {  
			QString str=ReadInpData.TmpBCInps.at(j).SportType;
			QString str1=ReadInpData.TmpBCInps.at(j).strURstyle;
			QString str2=ReadInpData.TmpBCInps.at(j).SolutionName;
			if(str=="无"&&str1!="11,11"&&str2==""){
            QString strTmpName = ReadInpData.TmpBCInps.at(j).strPSetName;
				int id=m_comboxPset[Nrow]->findText(strTmpName);
				m_comboxPset[Nrow]->setCurrentIndex(id); 
				Nrow++;
			}
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
           }else if (tmp=="1,3"||tmp=="1,6"){
               tmp=tr("固定");
           }
           //if(comboxStyleList.indexOf(tmp)==-1){
               comboxStyleList<<tmp;//}
       }
       for (int kk=0;kk<comboxStyleList.size();kk++){
           QString strTmpName =comboxStyleList.at(kk);
           int id=m_comboxStyle[kk]->findText(strTmpName);
           m_comboxStyle[kk]->setCurrentIndex(id);  
       }  
    }
    WriteToBcInps();
}
void QHPBCDlg::WriteToBcInps()
{
    BCInpS nodeBCList;
    int TabRowCnt=m_TableWiget->rowCount();
    int TabColCnt=m_TableWiget->columnCount();
    if (TabRowCnt>0){m_nodeBCList.clear();}
    for(int i=0; i<TabRowCnt; i++){
        nodeBCList.strCommentaryName="边界条件";
        nodeBCList.strBoundaryName="Boundary";
        nodeBCList.strPSetName=((MyCombox*)m_TableWiget->cellWidget(i,ENUM_BC_PSET))->currentText();//m_comboxPset.at(i)->currentText();
        QString str = ((MyCombox*)m_TableWiget->cellWidget(i,ENUM_BC_XYZ))->currentText();//m_comboxStyle.at(i)->currentText();
        if (str=="x向固定"){
            nodeBCList.strURstyle="1,1";
        }else if (str=="y向固定"){
            nodeBCList.strURstyle="2,2";
        }else if (str=="z向固定"){
            nodeBCList.strURstyle="3,3";
        }else if (str=="固定"){
            nodeBCList.strURstyle="1,3";
        }

        m_nodeBCList.append(nodeBCList);
    }
}
void QHPBCDlg::ApplyBtnSlot()
{  
    WriteToBcInps();
}

void QHPBCDlg::AddBtnSlot()//增加行
{
    int nRow =m_TableWiget->rowCount();
    m_TableWiget->insertRow(nRow);

    MyCombox *NewcomboxPset = new MyCombox(this,nRow);  //添加新行的控件
    m_TableWiget->setCellWidget(nRow,ENUM_BC_PSET,NewcomboxPset);

	m_TableWiget->setItem(nRow,0,new QTableWidgetItem());
    m_TableWiget->item(nRow,0)->setText("Part");
 
    if (m_PSetList.count()){
        NewcomboxPset->addItems(m_PSetList);
    }else{
        MyCombox *TmpcomboxPset = (MyCombox*)m_TableWiget->cellWidget(0, ENUM_BC_PSET);//new MyCombox(this, nRow);
        for (int i=0;i<TmpcomboxPset->count();i++){//遍历所有，进行显示；
            NewcomboxPset->addItem(TmpcomboxPset->itemText(i));
        } 
    }
    m_comboxPset.insert(nRow,NewcomboxPset);
    connect(NewcomboxPset,SIGNAL(clicked(int,int)),this,SLOT(comBoxSlot()));

    MyCombox *Tmpcombox=new MyCombox(this, nRow);
    Tmpcombox->addItem("x向固定");
    Tmpcombox->addItem("y向固定");
    Tmpcombox->addItem("z向固定");
	Tmpcombox->addItem("固定");
    Tmpcombox->setFrame(false);
    m_TableWiget->setCellWidget(nRow,1,Tmpcombox);
    m_comboxStyle.insert(nRow,Tmpcombox);
    
}
void QHPBCDlg::DelBtnSlot()//删除行
{
    int curRow =m_TableWiget->currentRow();
	if (curRow==0)curRow =m_TableWiget->rowCount()-1;
    if (curRow!=-1){
        m_TableWiget->removeRow(curRow);
    }
}
//--
//1.index:List中第几个
//2.comWigId:第几个ComboBox
//--
void QHPBCDlg::comBoxSlot()
{
    MyCombox *btn=(MyCombox*) sender();
    int x=btn->mapToParent(QPoint(0,0)).x();
    int y=btn->mapToParent(QPoint(0,0)).y();
    QModelIndex index= m_TableWiget->indexAt(QPoint(x,y));

    int iRow=index.row();
    int iCol=index.column();

    MyCombox *tmpBtn = (MyCombox*)m_TableWiget->cellWidget(iRow, iCol);
    int id=m_comboxPset.key(tmpBtn);
    if (id==-1)return;
    int ind= m_comboxPset[id]->currentIndex();
    QString str =m_comboxPset[id]->currentText();
    int iTmp = m_comboxPset[id]->itemData(ind,Qt::UserRole+3).toInt();
    //--设置属性
    NElSurfChsS tmpChs;
    tmpChs.strName =str;
    tmpChs.iType=COM_ENUM_NSET;
    emit emitPsetHighLight(tmpChs);
}
//--下一步
void QHPBCDlg::NextBtnSlot()
{
    WriteToBcInps();
    this->accept();
}
//--上一步
void QHPBCDlg::PreBtnSlot()
{
    WriteToBcInps();
    this->accept();
}
//--psetdlg create point set;
//--
void QHPBCDlg::AppendPSetComboxSlot(QString str)
{
    int nRow =m_TableWiget->rowCount();
    if(m_PSetList.indexOf(str) == -1){
        m_PSetList.append(str);
        for (int i=0;i<nRow;i++){
            MyCombox *TmpcomboxPset = (MyCombox*)m_TableWiget->cellWidget(i, ENUM_BC_PSET);//new MyCombox(this, nRow);
            TmpcomboxPset->addItem(str);      
        }
    }
    
}