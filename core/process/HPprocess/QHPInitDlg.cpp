#include "QHPInitDlg.h"

QHPInitDlg::QHPInitDlg(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("��ʼ��");
    QWidget *Hwid1 = new QWidget(this);
    QHBoxLayout *Hlayout = new QHBoxLayout();
    QHBoxLayout *Hlayout2 = new QHBoxLayout();
    m_ApplyBtn = new QPushButton(tr("Ӧ��"));
    m_AddBtn = new QPushButton(tr("����"));
    m_DelBtn = new QPushButton(tr("ɾ��"));
    m_PreInitBtn = new QPushButton(tr("��һ��"));
    m_NextInitBtn = new QPushButton(tr("��һ��"));

    Hlayout->addStretch();
    Hlayout->addWidget(m_AddBtn);
    Hlayout->addWidget(m_DelBtn);
    Hlayout->addStretch();
    Hlayout2->addWidget(m_ApplyBtn);
    Hlayout2->addWidget(m_PreInitBtn);
    Hlayout2->addWidget(m_NextInitBtn);

    QVBoxLayout *VLayout = new QVBoxLayout();
    m_TableWiget = new QTableWidget();
    m_TableWiget->setColumnCount(5);
	m_TableWiget->setColumnWidth(4,200);//setColumnWidth����ָ���еĿ���
    m_TableWiget->setRowCount(1);
    m_TableWiget->verticalHeaderItem(1);
    QStringList strList;
    strList<<"������"<<"����"<<"��ֵ"<<"�ⲿ�ļ�"<<"·��";
    m_TableWiget->setHorizontalHeaderLabels(strList);
	/*for (int j=0;j<strList.size();j++){
		m_TableWiget->horizontalHeader()->setResizeMode(j,QHeaderView::ResizeToContents);
	}*/
    //�¶ȳ�
    //QTableWidgetItem *tmpItem=new QTableWidgetItem(tr("�¶ȳ�"));
    //m_TableWiget->setItem(0,0,tmpItem);
    ////������
    //QTableWidgetItem *tmpItem2=new QTableWidgetItem(tr("������"));
    ////m_TableWiget->setItem(1,0,tmpItem2);
    ////�����ȳߴ�
    //tmpItem2=new QTableWidgetItem(tr("�����ȳߴ�"));
    //m_TableWiget->setItem(2,0,tmpItem2);
    ////��ĩ�ܶ�
    //tmpItem2=new QTableWidgetItem(tr("��ĩ�ܶ�"));
    //m_TableWiget->setItem(3,0,tmpItem2);
    ////���ζ�
    //tmpItem2=new QTableWidgetItem(tr("���ζ�"));
    //m_TableWiget->setItem(4,0,tmpItem2);
    ////����
    //tmpItem2=new QTableWidgetItem(tr("����"));
    //m_TableWiget->setItem(5,0,tmpItem2);

    for (int i=0;i<1;i++){
		QTableWidgetItem *tmpItem2=new QTableWidgetItem(tr(""));
        m_TableWiget->setItem(i,2,tmpItem2);

		MyCombox* TmpcomboxPset0 = new MyCombox(this,i);
        TmpcomboxPset0->addItem("�¶ȳ�");
        TmpcomboxPset0->addItem("Ӧ����");
        TmpcomboxPset0->addItem("״̬����");
        TmpcomboxPset0->setFrame(false);
        m_TableWiget->setCellWidget(i,0,TmpcomboxPset0);

        MyCombox* TmpcomboxPset = new MyCombox(this,i);
        //TmpcomboxPset->addItem("Point Set1");
        //TmpcomboxPset->addItem("Point Set2");
        //TmpcomboxPset->addItem("�½�");
        TmpcomboxPset->setFrame(false);
        m_TableWiget->setCellWidget(i,1,TmpcomboxPset);
        m_comboxPset.insert(i,TmpcomboxPset);
		QPushButton *m_LoadEFBtn = new QPushButton(tr("����"));
		m_TableWiget->setCellWidget(i,3,m_LoadEFBtn);
		QTableWidgetItem *tmpItem1 =new QTableWidgetItem(tr(""));
		m_TableWiget->setItem(i,4,tmpItem1);
	    tmpItem1->setFlags((Qt::ItemFlags)32);
	    tmpItem1->setBackgroundColor(QColor(200,200,200));//
		connect(m_LoadEFBtn,SIGNAL(clicked()),this,SLOT(LoadEFSlot()));
        //connect(m_comboxPset[i],SIGNAL(clicked(int,int)),this,SLOT(comBoxChgSlot(int,int)));
        //connect(m_comboxPset[i],SIGNAL(doubleclicked(int,int)),this,SLOT(comBoxDoubleSlot(int,int)));
    }

	m_TableWiget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	m_TableWiget->horizontalHeader()->setResizeMode(0, QHeaderView::Interactive);
	m_TableWiget->horizontalHeader()->setResizeMode(3, QHeaderView::Interactive);
	this->setMinimumWidth(580);

    VLayout->addWidget(m_TableWiget);
    QVBoxLayout *MainLayout = new QVBoxLayout(this);
    MainLayout->addLayout(VLayout);
    MainLayout->addLayout(Hlayout);
    MainLayout->addLayout(Hlayout2);
    setLayout(MainLayout);
    connect(m_ApplyBtn,SIGNAL(clicked()),this,SLOT(ApplyBtnSlot()));
    connect(m_AddBtn,SIGNAL(clicked()),this,SLOT(AddBtnSlot()));
    connect(m_DelBtn,SIGNAL(clicked()),this,SLOT(DelBtnSlot()));
    connect(m_NextInitBtn,SIGNAL(clicked()),this,SLOT(NextBtnSlot()));
    connect(m_PreInitBtn,SIGNAL(clicked()),this,SLOT(PreBtnSlot()));
}

QHPInitDlg::~QHPInitDlg()
{
    m_comboxPset.clear();
}
//����ȡ��INP������ʾ�ڵ�ǰ�ı�����
void QHPInitDlg::SetInpData(ReadInpResultS ReadInpData)
{
    int nRowSize = m_TableWiget->rowCount();
    int nInitRowSize = ReadInpData.TmpInitInps.size();

    m_PSetList=ReadInpData.TmpNsetInps.strNSetName;

    if (nRowSize < nInitRowSize){
        for (int i=nRowSize;i<nInitRowSize;i++){
            m_TableWiget->insertRow(i);
            //�¶ȳ�
			MyCombox* TmpcomboxPset0 = new MyCombox(this,i);
			TmpcomboxPset0->addItem("�¶ȳ�");
			TmpcomboxPset0->addItem("Ӧ����");
			TmpcomboxPset0->addItem("״̬����");
			TmpcomboxPset0->setFrame(false);
			m_TableWiget->setCellWidget(i,0,TmpcomboxPset0);
            QTableWidgetItem *tmpItem2=new QTableWidgetItem(tr(""));
            m_TableWiget->setItem(i,2,tmpItem2);
      
            MyCombox* TmpcomboxPset = new MyCombox(this,i);
            //TmpcomboxPset->addItem("Point Set1");
            //TmpcomboxPset->addItem("Point Set2");
            //TmpcomboxPset->addItem("�½�");
            TmpcomboxPset->setFrame(false);
            m_TableWiget->setCellWidget(i,1,TmpcomboxPset);
            m_comboxPset.insert(i,TmpcomboxPset);
			QPushButton *m_LoadEFBtn = new QPushButton(tr("����"));
			m_TableWiget->setCellWidget(i,3,m_LoadEFBtn);
			QTableWidgetItem *tmpItem1 =new QTableWidgetItem(tr(""));
			m_TableWiget->setItem(i,4,tmpItem1);
			tmpItem1->setFlags((Qt::ItemFlags)32);
			tmpItem1->setBackgroundColor(QColor(200,200,200));
			connect(m_LoadEFBtn,SIGNAL(clicked()),this,SLOT(LoadEFSlot()));
        }
    }else if(nRowSize > nInitRowSize){//ֱ��ɾ��
        for (int i=nInitRowSize;i<nRowSize;i++){
            m_TableWiget->removeRow(nInitRowSize);
        }
    }
     nRowSize = m_TableWiget->rowCount();

     for (int i=0;i<nRowSize;i++){
         MyCombox *tmpCombox=(MyCombox*)m_TableWiget->cellWidget(i,1);
         tmpCombox->clear();
         tmpCombox->addItems(m_PSetList);
         //tmpCombox->addItem("�½�");
         m_comboxPset.insert(i,tmpCombox);
         if (ReadInpData.TmpInitInps.size()>0){
			 int id0=0;
			 QString strInitTypeName=ReadInpData.TmpInitInps.at(i).strInitTypeName;
			 if (strInitTypeName.toLower()=="temperature"){id0=0;
			 }else if (strInitTypeName.toLower()=="stress"){id0=1;
			 }else if (strInitTypeName.toLower()=="solution"){id0=2;}
			 ((MyCombox*)m_TableWiget->cellWidget(i,0))->setCurrentIndex(id0);
             QString strTmpName = ReadInpData.TmpInitInps.at(i).strPSetName;
             int id=m_comboxPset[i]->findText(strTmpName);
             m_comboxPset[i]->setCurrentIndex(id);   

             QString strVal=ReadInpData.TmpInitInps.at(i).strVal;
             QTableWidgetItem * tmpItem=new QTableWidgetItem(strVal);
             m_TableWiget->setItem(i,2,tmpItem);
         }
         connect(m_comboxPset[i],SIGNAL(clicked(int,int)),this,SLOT(comBoxChgSlot()));
     }   
    WriteToInps();
}
void QHPInitDlg::ApplyBtnSlot()
{
    WriteToInps();
}
void QHPInitDlg::NextBtnSlot()
{
    WriteToInps();
    this->accept();
}
void QHPInitDlg::PreBtnSlot()
{
    WriteToInps();
    this->accept();
}
void QHPInitDlg::WriteToInps()
{
	m_nodeInitList.clear();
    InitInpS initNode;
    int TabRowCnt=m_TableWiget->rowCount();
    int TabColCnt=m_TableWiget->columnCount();
    if (TabRowCnt>0){m_nodeInitList.clear();}

    QString strName;
    for(int i=0; i<TabRowCnt; i++){
        initNode.strCommentaryName = "��ʼ��";
        initNode.strInitCondition="Initial Conditions";
        strName =((MyCombox*)m_TableWiget->cellWidget(i,0))->currentText();
        if (strName =="�¶ȳ�"){initNode.strInitTypeName="TEMPERATURE";
		}else if (strName =="Ӧ����"){initNode.strInitTypeName="STRESS";
        }else if (strName =="״̬����"){initNode.strInitTypeName="SOLUTION";
        }else if (strName =="�����ȳߴ�"){initNode.strInitTypeName="CRYSTALSIZE";
        }else if (strName =="������"){initNode.strInitTypeName="CRYSTAL";
        }else if (strName =="��ĩ�ܶ�"){initNode.strInitTypeName="DUSTDENSITY";
        }else if (strName =="���ζ�"){initNode.strInitTypeName="SPHERICITY";     
        }else if (strName =="����"){initNode.strInitTypeName="GRAINRADIUS";       
        }
        initNode.strPSetName=((MyCombox*)m_TableWiget->cellWidget(i,1))->currentText();//m_comboxPset.at(i)->currentText();
        initNode.strVal = m_TableWiget->item(i,2)->text();
		if(initNode.strVal!="")m_nodeInitList.append(initNode);
        QString Name=m_TableWiget->item(i,4)->text().remove(QRegExp("\\s"));
		int Ininumber=Name.split(";").size();
		if(Name=="")Ininumber=0;
		for(int i=0;i<Ininumber;i++){
			initNode.strCommentaryName = "��ʼ��";
			initNode.strInitCondition="Initial Conditions";
			initNode.IncludeName=Name.split(";").at(i);
			QString type=initNode.IncludeName.right(2);
			if(type=="nt"){
				initNode.strInitTypeName="TEMPERATURE";
			}else if(type=="st"){
				initNode.strInitTypeName="STRESS";
			}else if(type.toLower()=="stdv"){
				initNode.strInitTypeName="SOLUTION";
			}
			initNode.strPSetName="";
			initNode.strVal="";
        m_nodeInitList.append(initNode);
		}
    }
}
void QHPInitDlg::AddBtnSlot()
{
    //--����
    //int curRow = m_TableWiget->currentRow();
    // m_TableWiget->insertRow(curRow);
    //--����
    int nRow = m_TableWiget->rowCount();

    m_TableWiget->setRowCount(nRow+1);
    QTableWidgetItem *tmpItem2=new QTableWidgetItem(tr(""));

    m_TableWiget->setItem(nRow,2,tmpItem2);
    if (nRow>=1){
		MyCombox* TmpcomboxPset0 = new MyCombox(this,nRow);
        TmpcomboxPset0->addItem("�¶ȳ�");
        TmpcomboxPset0->addItem("Ӧ����");
		TmpcomboxPset0->addItem("Ӧ�䳡");
        TmpcomboxPset0->addItem("״̬����");
        TmpcomboxPset0->setFrame(false);
        m_TableWiget->setCellWidget(nRow,0,TmpcomboxPset0);
        MyCombox *tmpCombox = new MyCombox(this,nRow);
        tmpCombox->setFrame(false);
        //m_comboxPset.at(nRow) = new MyCombox(this,nRow);
        //m_comboxPset.at(nRow)->setFrame(false);

        QStringList strTmpList;
        for (int i=0; i<((MyCombox*)m_TableWiget->cellWidget(0,1))->count(); i++){
            strTmpList<<((MyCombox*)m_TableWiget->cellWidget(0,1))->itemText(i);
        }
        tmpCombox->addItems(strTmpList);

        m_TableWiget->setCellWidget(nRow,1,tmpCombox);
        m_comboxPset.insert(nRow,tmpCombox);
        connect(m_comboxPset[nRow],SIGNAL(clicked(int,int)),this,SLOT(comBoxChgSlot()));
		QPushButton *m_LoadEFBtn = new QPushButton(tr("����"));
		m_TableWiget->setCellWidget(nRow,3,m_LoadEFBtn);
		QTableWidgetItem *tmpItem1 =new QTableWidgetItem(tr(""));
		m_TableWiget->setItem(nRow,4,tmpItem1);
	    tmpItem1->setFlags((Qt::ItemFlags)32);
	    tmpItem1->setBackgroundColor(QColor(200,200,200));//
		connect(m_LoadEFBtn,SIGNAL(clicked()),this,SLOT(LoadEFSlot()));
        //connect(m_comboxPset.at(nRow),SIGNAL(doubleclicked(int,int)),this,SLOT(comBoxDoubleSlot(int,int)));
    }else{
		MyCombox* TmpcomboxPset0 = new MyCombox(this,nRow);
        TmpcomboxPset0->addItem("�¶ȳ�");
        TmpcomboxPset0->addItem("Ӧ����");
		TmpcomboxPset0->addItem("Ӧ�䳡");
        TmpcomboxPset0->addItem("״̬����");
        TmpcomboxPset0->setFrame(false);
        m_TableWiget->setCellWidget(nRow,0,TmpcomboxPset0);
        MyCombox *tmpCombox = new MyCombox(this,nRow);
        tmpCombox->addItems(m_PSetList);
        //tmpCombox->addItem("�½�");
        tmpCombox->setFrame(false);
        m_TableWiget->setCellWidget(nRow,1,tmpCombox);
        m_comboxPset.insert(nRow,tmpCombox);
        connect(m_comboxPset[nRow],SIGNAL(clicked(int,int)),this,SLOT(comBoxChgSlot()));
		QPushButton *m_LoadEFBtn = new QPushButton(tr("����"));
		m_TableWiget->setCellWidget(nRow,3,m_LoadEFBtn);
		QTableWidgetItem *tmpItem1 =new QTableWidgetItem(tr(""));
		m_TableWiget->setItem(nRow,4,tmpItem1);
	    tmpItem1->setFlags((Qt::ItemFlags)32);
	    tmpItem1->setBackgroundColor(QColor(200,200,200));
		connect(m_LoadEFBtn,SIGNAL(clicked()),this,SLOT(LoadEFSlot()));
        // connect(m_comboxPset.at(nRow),SIGNAL(doubleclicked(int,int)),this,SLOT(comBoxDoubleSlot(int,int)));
    }



}

void QHPInitDlg::DelBtnSlot()
{
	int curRow =m_TableWiget->currentRow();
	if(curRow==0)curRow=m_TableWiget->rowCount()-1;

    if (curRow!=-1){
        m_TableWiget->removeRow(curRow);
    }
}

void QHPInitDlg::comBoxChgSlot()
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
    //--��������
    NElSurfChsS tmpChs;
    tmpChs.strName =str;
    tmpChs.iType=COM_ENUM_NSET;
    emit emitPsetHighLight(tmpChs);

}

void QHPInitDlg::LoadEFSlot()
{
	QString FName="";
	QStringList filename = QFileDialog::getOpenFileNames(this, "Read file...", "", "All Files(*.nt *.st *.stdv);;Temp Files (*.nt );;Stress Files (*.st);;Solution Files (*.stdv)");   
	if (filename.size()==0){
		return;
	}else{
		for(int i=0;i<filename.size();i++){
			if(i!=filename.size()-1)FName+=filename.at(i)+";";
			else FName+=filename.at(i);
		}
	}

   QPushButton *btn=(QPushButton*)sender();
   int x=btn->mapToParent(QPoint(0,0)).x();//��ȡ�������x����
   int y=btn->mapToParent(QPoint(0,0)).y();//��ȡ�������y����
   QModelIndex index= m_TableWiget->indexAt(QPoint(x,y));

   int Nrow=index.row();
   QTableWidgetItem* tmpItem1 = m_TableWiget->item(Nrow, 2);//��ȡ��Ԫ��
   tmpItem1->setFlags((Qt::ItemFlags)32);
   tmpItem1->setBackgroundColor(QColor(200,200,200));//
   QTableWidgetItem* tmpItem2 = m_TableWiget->item(Nrow, 4);//��ȡ��Ԫ��
   tmpItem2->setFlags((Qt::ItemFlags)63);
   tmpItem2->setBackgroundColor(QColor(255,255,255,0));//��������ɫ����Ϊ͸��
   m_TableWiget->item(Nrow,4)->setText(FName);
}

void QHPInitDlg::AppendPSetComboxIniSlot(QString str)
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
