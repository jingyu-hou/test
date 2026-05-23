#include "QForgingSystemDlg.h"

QFSConst::QFSConst(QWidget *parent1)
      : QDialog(parent1)
{
  setWindowTitle("�˶�����");
  ConstMotionParameters=new QLineEdit();
  ConstMotionParameters->setText("");

  QLabel *text=new QLabel();
  text->setText("������");
  
  QHBoxLayout *Hlayout = new QHBoxLayout();
  QHBoxLayout *Hlayout2 = new QHBoxLayout();
  m_OKBtn = new QPushButton(tr("ȷ��"));
  m_CancelBtn = new QPushButton(tr("ȡ��"));

  Hlayout->addStretch();
  Hlayout->addWidget(text);
  Hlayout->addWidget(ConstMotionParameters);
  Hlayout->addStretch();

  Hlayout2->addStretch();
  Hlayout2->addWidget(m_OKBtn);
  Hlayout2->addWidget(m_CancelBtn);
  Hlayout2->addStretch();

  QVBoxLayout *MainLayout0 = new QVBoxLayout(this);
  MainLayout0->addLayout(Hlayout);
  MainLayout0->addLayout(Hlayout2);
  setLayout(MainLayout0);

  connect(m_OKBtn,SIGNAL(clicked()),this,SLOT(m_OKBtnS()));
  connect(m_CancelBtn,SIGNAL(clicked()),this,SLOT(m_CancelBtnS()));
}

void QFSConst::SetInpDataC(FSystem FSData){
    ConstMotionParameters->setText(FSData.MotionParameter);
	WriteToFSystemInps();
}

void QFSConst::WriteToFSystemInps(){
	CMPOutPut=ConstMotionParameters->text();
}

void QFSConst:: m_OKBtnS()
{
  WriteToFSystemInps();
  this->accept();
}

void QFSConst:: m_CancelBtnS()
{
  this->close();
}

QFSConst::~QFSConst()
{

}


QForgingSystemDlg::QForgingSystemDlg(QWidget *parent)
        : QDialog(parent)
{
   	//��ʼ��ʼ������
	int JJII=0;
	for(int II=0;II<20;II++){
		count02[II]=0;
		for(int JJ=0;JJ<1;JJ++){
           count01[JJ][II]=0;
		   IndexNumber[JJII]=0;
           JJII++;
		}
	}
	//��ʼ����������
	FSNumber=0;

	setWindowTitle("�����ƶ�");
	QHBoxLayout *Hlayout1= new QHBoxLayout();
    QHBoxLayout *Hlayout2= new QHBoxLayout();
    QVBoxLayout *Vlayout = new QVBoxLayout();
    
    m_AddBtn = new QPushButton(tr("����"));
    m_DelBtn = new QPushButton(tr("ɾ��"));
    m_ApplyBtn = new QPushButton(tr("Ӧ��"));
    m_PreSystemBtn = new QPushButton(tr("��һ��"));
	m_NextSystemBtn = new QPushButton(tr("��һ��"));
    Hlayout1->addStretch();
    Hlayout1->addWidget(m_AddBtn);
    Hlayout1->addWidget(m_DelBtn);
    Hlayout1->addStretch();
    Hlayout2->addWidget(m_ApplyBtn);
    Hlayout2->addWidget(m_PreSystemBtn);
	Hlayout2->addWidget(m_NextSystemBtn);

    //--�����б�
    int iniRowCnt=1;
    QStringList strList;
	strList<<"����"<<"�ڵ㼯��"<<"�˶�����"<<"�˶�����"<<"�˶���������"<<"�˶�����"<<"�ȱ߽�����";
    QVBoxLayout *VLayout = new QVBoxLayout();
    m_TableWiget = new QTableWidget();
    m_TableWiget->setColumnCount(strList.size());
    m_TableWiget->setRowCount(iniRowCnt);
    m_TableWiget->verticalHeaderItem(1);
    m_TableWiget->setHorizontalHeaderLabels(strList);
	/*for (int j=0;j<strList.size();j++){
		m_TableWiget->horizontalHeader()->setResizeMode(j,QHeaderView::ResizeToContents);
	}*/

    QTableWidgetItem *tmpItem2=new QTableWidgetItem(tr("����"));
    m_TableWiget->setItem(0,0,tmpItem2);

	MyCombox *tmpCombox01=new MyCombox(this,0);
    tmpCombox01->addItem(tr("�½�"));
    tmpCombox01->setFrame(false);
    m_TableWiget->setCellWidget(0,1,tmpCombox01);
	m_NodeSet.insert(0,tmpCombox01);

    MyCombox *tmpCombox1=new MyCombox(this,0);
	tmpCombox1->addItem(tr("���˶�"));
    tmpCombox1->addItem(tr("λ��"));
	tmpCombox1->addItem(tr("�ٶ�"));
    tmpCombox1->setFrame(false);
    m_TableWiget->setCellWidget(0,2,tmpCombox1);
	m_SportType.push_back(tmpCombox1);

	MyCombox *tmpCombox2=new MyCombox(this,0);
	tmpCombox2->addItem(tr("No"));
    tmpCombox2->addItem(tr("X"));
	tmpCombox2->addItem(tr("Y"));
	tmpCombox2->addItem(tr("Z"));
    tmpCombox2->setFrame(false);
    m_TableWiget->setCellWidget(0,3,tmpCombox2);
	m_Direction.push_back(tmpCombox2);

	MyCombox *tmpCombox3=new MyCombox(this,0);
	tmpCombox3->addItem(tr("No"));
    tmpCombox3->addItem(tr("����"));
	tmpCombox3->addItem(tr("��ʱ��仯"));
    tmpCombox3->setFrame(false);
    m_TableWiget->setCellWidget(0,4,tmpCombox3);
    m_ParameterType.insert(0,tmpCombox3);

	QIcon icon;
	icon.addFile(":/images/edit.png");
    QPushButton* m_Parameter = new QPushButton();
	m_Parameter->setIcon(icon);
	m_TableWiget->setCellWidget(0,5,m_Parameter);
	connect(m_Parameter,SIGNAL(clicked()),this,SLOT(MotionParameterSlot()));

	/*tmpItem2=new QTableWidgetItem(tr(""));
    m_TableWiget->setItem(0,6,tmpItem2);*/

	MyCombox *tmpCombox04=new MyCombox(this,0);
    tmpCombox04->addItem(tr("��"));
	//tmpCombox04->addItems(ThermalBoundary::m_ThermalBoundaryName);
	//tmpCombox04->addItems(m_ThermalBoundaryName);
    tmpCombox04->setFrame(false);
    m_TableWiget->setCellWidget(0,6,tmpCombox04);
	//m_ThB2.insert(0,tmpCombox04);

	//m_NodeSet.insert(0,tmpCombox04);
	//connect(tmpCombox04,SIGNAL(clicked()),this,SLOT(ThermalBoundarySlot()));

	m_TableWiget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	m_TableWiget->horizontalHeader()->setResizeMode(0, QHeaderView::Interactive);
	m_TableWiget->horizontalHeader()->setResizeMode(2, QHeaderView::Interactive);
	m_TableWiget->horizontalHeader()->setResizeMode(3, QHeaderView::Interactive);
	m_TableWiget->horizontalHeader()->setResizeMode(4, QHeaderView::Interactive);
	m_TableWiget->horizontalHeader()->setResizeMode(5, QHeaderView::Interactive);
	VLayout->addWidget(m_TableWiget);
    QVBoxLayout *MainLayout = new QVBoxLayout(this);
    MainLayout->addLayout(VLayout);
    MainLayout->addLayout(Hlayout1);
    MainLayout->addLayout(Hlayout2);
    setLayout(MainLayout);
    this->setMinimumWidth(750);

	connect(m_ApplyBtn,SIGNAL(clicked()),this,SLOT(ApplySABtnSlot()));
    connect(m_AddBtn,SIGNAL(clicked()),this,SLOT(AddSABtnSlot()));
    connect(m_DelBtn,SIGNAL(clicked()),this,SLOT(DelSABtnSlot()));
    connect(m_NextSystemBtn,SIGNAL(clicked()),this,SLOT(NextSABtnSlot()));
    connect(m_PreSystemBtn,SIGNAL(clicked()),this,SLOT(PreSABtnSlot())); 
}
void QForgingSystemDlg::ApplySABtnSlot()
{
	WriteToFSInps();
}

void QForgingSystemDlg::NextSABtnSlot()
{
    int nRow =m_TableWiget->rowCount();
	emitForgingsystemNext(nRow);
	WriteToFSInps();
	this->accept();
}

void QForgingSystemDlg::PreSABtnSlot()
{
	WriteToFSInps();
	this->accept();
}
void QForgingSystemDlg::AddSABtnSlot()
{
    int nRow =m_TableWiget->rowCount();
    m_TableWiget->insertRow(nRow);

	QTableWidgetItem *tmpItem2=new QTableWidgetItem(tr("����"));
    m_TableWiget->setItem(nRow,0,tmpItem2);

	MyCombox *tmpCombox01=new MyCombox(this,nRow);
    tmpCombox01->addItem(tr("�½�"));
    tmpCombox01->setFrame(false);
    m_TableWiget->setCellWidget(nRow,1,tmpCombox01);
	m_NodeSet.insert(nRow,tmpCombox01);

    MyCombox *tmpCombox1=new MyCombox(this,nRow);
	tmpCombox1->addItem(tr("���˶�"));
    tmpCombox1->addItem(tr("λ��"));
	tmpCombox1->addItem(tr("�ٶ�"));
    tmpCombox1->setFrame(false);
    m_TableWiget->setCellWidget(nRow,2,tmpCombox1);
	m_SportType.push_back(tmpCombox1);

	MyCombox *tmpCombox2=new MyCombox(this,nRow);
	tmpCombox2->addItem(tr("No"));
    tmpCombox2->addItem(tr("X"));
	tmpCombox2->addItem(tr("Y"));
	tmpCombox2->addItem(tr("Z"));
    tmpCombox2->setFrame(false);
    m_TableWiget->setCellWidget(nRow,3,tmpCombox2);
	m_Direction.push_back(tmpCombox2);

	MyCombox *tmpCombox3=new MyCombox(this,nRow);
    tmpCombox3->addItem(tr("No"));
    tmpCombox3->addItem(tr("����"));
	tmpCombox3->addItem(tr("��ʱ��仯"));
    tmpCombox3->setFrame(false);
    m_TableWiget->setCellWidget(nRow,4,tmpCombox3);
	m_ParameterType.insert(nRow,tmpCombox3);

	QIcon icon;
	icon.addFile(":/images/edit.png");
    QPushButton* m_Parameter = new QPushButton();
	m_Parameter->setIcon(icon);
	m_TableWiget->setCellWidget(nRow,5,m_Parameter);
	connect(m_Parameter,SIGNAL(clicked()),this,SLOT(MotionParameterSlot()));

	/*tmpItem2=new QTableWidgetItem(tr(""));
    m_TableWiget->setItem(nRow,6,tmpItem2);*/

	MyCombox *tmpCombox04=new MyCombox(this,nRow);
    tmpCombox04->addItem(tr("��"));
	//tmpCombox04->addItems(ThermalBoundary::m_ThermalBoundaryName);
	tmpCombox04->addItems(m_ThBName);
    tmpCombox04->setFrame(false);
    m_TableWiget->setCellWidget(nRow,6,tmpCombox04);
	//m_ThB2.insert(nRow,tmpCombox04);
	//connect(tmpCombox04,SIGNAL(clicked()),this,SLOT(ThermalBoundarySlot()));

	if (Nodeset.count()){
		m_NodeSet.at(nRow)->addItems(Nodeset);}
    //}else{
    //    MyCombox *TmpcomboxPset = (MyCombox*)m_TableWiget->cellWidget(0, 1);//new MyCombox(this, nRow);
    //    for (int i=0;i<TmpcomboxPset->count();i++){//�������У�������ʾ��
    //        tmpCombox01->addItem(TmpcomboxPset->itemText(i));
    //    } 
    //}
    //m_comboxPset.insert(nRow,NewcomboxPset);
	connect(tmpCombox01,SIGNAL(clicked(int,int)),this,SLOT(ComboxSlotHighlight(int,int)));

}

void QForgingSystemDlg::DelSABtnSlot()
{
    int curRow =m_TableWiget->currentRow();
	if(curRow==0)curRow=m_TableWiget->rowCount()-1;
	int INumber,INumber2,Num,Num2;
    if (curRow!=-1){
		m_TableWiget->removeRow(curRow);

		INumber=IndexNumber[2*curRow+1];
		INumber2=IndexNumber[2*curRow];
		Num=count01[1][curRow];
		Num2=count01[0][curRow];
		if(Num==2){
			count01[1][curRow]=0;
			m_FSCurveDataListDlg02.takeAt(INumber);
		}
		if(Num2==1){
			count01[0][curRow]=0;
			m_FSCurveDataListDlg.takeAt(INumber2);
		}	

	}
}

void QForgingSystemDlg::MotionParameterSlot()
{
	int Row,index,x,y;
	QString strIdName;
	QPushButton *pushBtn = dynamic_cast<QPushButton*>(this->sender());
	x=pushBtn->frameGeometry().x();
	y=pushBtn->frameGeometry().y();
	QModelIndex index02=m_TableWiget->indexAt(QPoint(x,y));
	Row=index02.row();
	if (Row <0){return;}
	strIdName=m_ParameterType[Row]->currentText();
	if(strIdName=="����"){
		int sign=count01[0][Row];
		if(sign!=1){
			QFSConst *FSConstShow=new QFSConst(this);
			m_FSCurveDataListDlg.append(FSConstShow);
			FSConstShow->show();
			IndexNumber[2*Row]=m_FSCurveDataListDlg.size()-1;
			count01[0][Row]=1;
		}else{
			int IN,IN1;
			IN1=2*Row;
			IN=IndexNumber[IN1];
			m_FSCurveDataListDlg.at(IN)->exec();
		}
	}else if(strIdName=="��ʱ��仯"){
		int sign=count01[1][Row];
		if(sign!=2){
			QCurveDataPlot *tmpCurve = new QCurveDataPlot(this);
			QString strName;
			int ret = tmpCurve->exec();   
			if (ret == QDialog::Accepted){//ok
				strName=tmpCurve->m_HIPCurveNode.strName;
				if (strName == ""){return;}
				m_FSCurveDataListDlg02.append(tmpCurve);
				IndexNumber[2*Row+1]=m_FSCurveDataListDlg02.size()-1;
				count01[1][Row]=2;
			}
		}else{
			int IN,IN1;
			IN1=2*Row+1;
			IN=IndexNumber[IN1];
			m_FSCurveDataListDlg02.at(IN)->exec();
		}	   
	}else{
		return;
	}
}

void QForgingSystemDlg::SetInpData(ReadInpResultS ReadInpData)
{
	int nRowSize = m_TableWiget->rowCount();
	//���¼����ж��м��������ƶ�
	int nFSystem=-1;
	int nBcRowSize1 =ReadInpData.TmpBCInps.size();
	for(int i=0;i<nBcRowSize1;i++){
		QString str=ReadInpData.TmpBCInps.at(i).SolutionName;
		QString str2=ReadInpData.TmpBCInps.at(i).StepNumber;
		int Step=str2.toInt()+1;
		//if(str=="6"&&str2!=""&&Step>nFSystem)nFSystem=Step;//�๤����Ҫȡ������ע��
		if(str!=""&&str2!=""&&Step>nFSystem)nFSystem=Step;//�๤����Ҫע�͸���
		}
		//nFSystem=ReadInpData.TmpFSystem.size();

	if(nFSystem<0){
         Nodeset=ReadInpData.TmpNsetInps.strNSetName;
	     int II=0;
		 for(II=0;II<nRowSize;II++){
		     m_NodeSet.at(II)->clear();
		     m_NodeSet.at(II)->addItem("�½�");
	     	 m_NodeSet.at(II)->addItems(Nodeset);
		    connect(m_NodeSet.at(II),SIGNAL(clicked(int,int)),this,SLOT(ComboxSlotHighlight(int,int)));
		 }
		return;
	}
	//���ϼ����ж��м��������ƶ�

	//���¼���ʹ�������е�����������ƶ��������
	if (nRowSize<nFSystem)
	{     
		for (int i=nRowSize;i<nFSystem;i++){
			 m_TableWiget->insertRow(i);

	         QTableWidgetItem *tmpItem2=new QTableWidgetItem(tr("����"));
             m_TableWiget->setItem(i,0,tmpItem2);

	         MyCombox *tmpCombox01=new MyCombox(this,i);
             tmpCombox01->addItem(tr("�½�"));
             tmpCombox01->setFrame(false);
             m_TableWiget->setCellWidget(i,1,tmpCombox01);
	         m_NodeSet.insert(i,tmpCombox01);

             MyCombox *tmpCombox1=new MyCombox(this,i);
			 tmpCombox1->addItem(tr("���˶�"));
             tmpCombox1->addItem(tr("λ��"));
	         tmpCombox1->addItem(tr("�ٶ�"));
             tmpCombox1->setFrame(false);
             m_TableWiget->setCellWidget(i,2,tmpCombox1);
			 m_SportType.push_back(tmpCombox1);

	         MyCombox *tmpCombox2=new MyCombox(this,i);
			 tmpCombox2->addItem(tr("No"));
             tmpCombox2->addItem(tr("X"));
	         tmpCombox2->addItem(tr("Y"));
	         tmpCombox2->addItem(tr("Z"));
             tmpCombox2->setFrame(false);
             m_TableWiget->setCellWidget(i,3,tmpCombox2);
			 m_Direction.push_back(tmpCombox2);

	         MyCombox *tmpCombox3=new MyCombox(this,i);
			 tmpCombox3->addItem(tr("No"));
             tmpCombox3->addItem(tr("����"));
	         tmpCombox3->addItem(tr("��ʱ��仯"));
             tmpCombox3->setFrame(false);
             m_TableWiget->setCellWidget(i,4,tmpCombox3);
	         m_ParameterType.insert(i,tmpCombox3);

	         QIcon icon;
	         icon.addFile(":/images/edit.png");
             QPushButton* m_Parameter = new QPushButton();
	         m_Parameter->setIcon(icon);
	         m_TableWiget->setCellWidget(i,5,m_Parameter);
	         connect(m_Parameter,SIGNAL(clicked()),this,SLOT(MotionParameterSlot()));

			/*tmpItem2=new QTableWidgetItem(tr(""));
			m_TableWiget->setItem(i,6,tmpItem2);*/

			MyCombox *tmpCombox04=new MyCombox(this,i);
			tmpCombox04->addItem(tr("��"));
			//tmpCombox04->addItems(ThermalBoundary::m_ThermalBoundaryName);
			//tmpCombox04->addItems(m_ThermalBoundaryName);
			tmpCombox04->setFrame(false);
			m_TableWiget->setCellWidget(i,6,tmpCombox04);
			//m_ThB2.insert(i,tmpCombox04);
			//connect(tmpCombox04,SIGNAL(clicked()),this,SLOT(ThermalBoundarySlot()));
		 }
	 }else if (nRowSize>nFSystem){ //clear and remove;
        for (int i=nFSystem;i<nRowSize;i++){
            m_TableWiget->removeRow(i);
		}
	}
	//���ϼ���ʹ�������е�����������ƶ��������

	//���Ӽ��ϵ�������
    nRowSize = m_TableWiget->rowCount();
    Nodeset=ReadInpData.TmpNsetInps.strNSetName;
	int II=0;
	for(II=0;II<nRowSize;II++){
		m_NodeSet.at(II)->clear();
		m_NodeSet.at(II)->addItem("�½�");
		m_NodeSet.at(II)->addItems(Nodeset);
		//m_NodeSet.at(II)->setItemData(II,"0",(Qt::UserRole+3));//0�����㼯��,1������Ԫ����,2�����漯��
		connect(m_NodeSet.at(II),SIGNAL(clicked(int,int)),this,SLOT(ComboxSlotHighlight(int,int)));
	}

	//���Ҽ��ϡ��˶����͡��˶������˶��������͡��˶��������Ͳ���ʾ
	m_FSCurveDataListDlg.clear();
	m_FSCurveDataListDlg02.clear(); 
	//nRowSize=ReadInpData.TmpFSystem.size();
	for(II=0;II<nBcRowSize1;II++){
		QString str=ReadInpData.TmpBCInps.at(II).SolutionName;
		QString str2=ReadInpData.TmpBCInps.at(II).StepNumber;
		if(str!="6"&&str2=="")continue;
		int j=str2.toInt();
		//���Ҽ���
		QString strTmpName = ReadInpData.TmpBCInps.at(II).strPSetName;
        int id=m_NodeSet.at(j)->findText(strTmpName);
        m_NodeSet.at(j)->setCurrentIndex(id); 
		//�˶�����
		QString Str1=ReadInpData.TmpBCInps.at(II).SportType;
		int index0=m_SportType.at(j)->findText(Str1);
		m_SportType.at(j)->setCurrentIndex(index0);
		//�˶�����
		QString Str2=ReadInpData.TmpBCInps.at(II).strURstyle;
		if(Str2=="1,1")Str2="X";
		else if(Str2=="2,2")Str2="Y";
		else if(Str2=="3,3")Str2="Z";
		else Str2="No";
		int index1=m_Direction.at(j)->findText(Str2);
		m_Direction.at(j)->setCurrentIndex(index1);
		//�˶���������/�˶�����
	    QString Str3=ReadInpData.TmpBCInps.at(II).ParametreType;
	    int id0=m_ParameterType[j]->findText(Str3);
	    m_ParameterType[j]->setCurrentIndex(id0);
		FSystem FSData;
		if(Str3=="����"){
			QFSConst *QFSConstShow=new QFSConst(this);
			FSData.MotionParameter=ReadInpData.TmpBCInps.at(II).Parametre;
			count01[0][j]=1;
			QFSConstShow->SetInpDataC(FSData);
			IndexNumber[2*j]=m_FSCurveDataListDlg.size();
			m_FSCurveDataListDlg.append(QFSConstShow);
		}else{
			QCurveDataPlot *tmpCurve = new QCurveDataPlot(this);
            InpCurveInpS tmpCurData;
			QString ST1=ReadInpData.TmpBCInps.at(II).strAmplitudeName;
            tmpCurData.strName=ST1;
			count01[1][j]=2;
			IndexNumber[2*j+1]=m_FSCurveDataListDlg02.size();
			int jj2=ReadInpData.TmpCurveInps.size();
			for(int I1=0;I1<jj2;I1++){
				QString st=ReadInpData.TmpCurveInps.at(I1).strName.simplified().toLower();
				if(st==ST1){
					tmpCurData.strDataX=ReadInpData.TmpCurveInps.at(I1).strDataX;
                    tmpCurData.strDataY=ReadInpData.TmpCurveInps.at(I1).strDataY;
                    tmpCurve->SetData(tmpCurData);
                    m_FSCurveDataListDlg02.append(tmpCurve);
				}	
			}		
		}
		//�ȱ߽�
		//*8*
		//m_ThB2[II]->setCurrentIndex(0);
	}
	ReadInpData0=ReadInpData;
	WriteToFSInps();
}

void QForgingSystemDlg::ComboxSlotHighlight(int comWigId,int index0)
{  
   QString str,str0,str1;
   int iTmp;
   MyCombox *btn=(MyCombox*)sender();
   int x=btn->mapToParent(QPoint(0,0)).x();//��ȡ�������x����
   int y=btn->mapToParent(QPoint(0,0)).y();//��ȡ�������y����
   QModelIndex index= m_TableWiget->indexAt(QPoint(x,y));
   int col=index.column();//��ȡ������б��������
   if (index0<0){return;}

   str =((MyCombox*)m_TableWiget->cellWidget(comWigId, 1))->currentText();
    
  if (str==""||str=="�½�")return;
  //--��������
  iTmp= m_NodeSet.at(comWigId)->itemData(index0,Qt::UserRole+3).toInt();//��ǰ��ѡcombox

  NElSurfChsS tmpChs;
  tmpChs.strName =str;
  tmpChs.iType =COM_ENUM_NSET;
  emit emitPsetHighLight(tmpChs);
}

void QForgingSystemDlg::ComboxSlotHighlight2(NElSurfChsS tmpChs)
{
	emit emitPsetHighLight(tmpChs);
}

void QForgingSystemDlg::WriteToFSInps()
{
	FSystem OputFSInf;
	InpCurveInpS    nodeCurveList;
	int TabRowCnt = m_TableWiget->rowCount();
    int TabColCnt = m_TableWiget->columnCount();
	if (TabRowCnt>0){
		m_OutPutFSystemInfS.clear();
	}
	int Num=m_FSCurveDataListDlg.size();
	int Num2=m_FSCurveDataListDlg02.size();
	for(int i=0; i<TabRowCnt; i++){
		QString j33=QString::number(i,10);//������תΪQString
		OputFSInf.NodeSetName<<m_NodeSet.at(i)->currentText();
        OputFSInf.SportType=m_SportType.at(i)->currentText();
        OputFSInf.Direction=m_Direction.at(i)->currentText();
        OputFSInf.MotionParameterType=m_ParameterType[i]->currentText();
		MyCombox *ff1=(MyCombox*)(m_TableWiget->cellWidget(i, 6));
		OputFSInf.TBName=ff1->currentText();
		if(OputFSInf.MotionParameterType=="����"){
			int Ii1=IndexNumber[2*i];
			if(Num!=0)OputFSInf.MotionParameter=m_FSCurveDataListDlg.at(Ii1)->CMPOutPut;
			OputFSInf.CurveName="";
			OputFSInf.ScalingFactor="";
		}else{
			int Ii2=IndexNumber[2*i+1];
			if(Num2!=0)OputFSInf.CurveName=m_FSCurveDataListDlg02.at(Ii2)->m_CurveName->text();
			OputFSInf.MotionParameter="";
			OputFSInf.ScalingFactor="1";
		}
        OputFSInf.ForgingSystemName=j33;
		OputFSInf.SolutionName="6";
		m_OutPutFSystemInfS.append(OputFSInf);
	}

	int DX=m_FSCurveDataListDlg02.size();
	if(DX>0)m_CurveList.clear();
	for(int II=0;II<DX;II++){
		nodeCurveList=m_FSCurveDataListDlg02.at(II)->m_HIPCurveNode;
		m_CurveList.append(nodeCurveList);
	}
}

QForgingSystemDlg::~QForgingSystemDlg()
{
	Nodeset.clear();
 }

 void QForgingSystemDlg::ShowFS(QStringList m_ThermalBoundaryName)
{
    QStringList strList;
    strList.clear();
    strList<<"����"<<"�ڵ㼯��"<<"�˶�����"<<"�˶�����"<<"�˶���������"<<"�˶�����"<<"�ȱ߽�����";
    m_TableWiget->setHorizontalHeaderLabels(strList);
	int NRow=m_TableWiget->rowCount();
	for(int i=0;i<NRow;i++){
		MyCombox *ff1=(MyCombox*)(m_TableWiget->cellWidget(i, 6));
		QString dfc=ff1->currentText();
		ff1->clear();
		ff1->addItem("��");
		ff1->addItems(m_ThermalBoundaryName);
        int id=ff1->findText(dfc);
		if(id==-1)id=ff1->findText("��");
		ff1->setCurrentIndex(id);//���ݸ���������ʹ��combox��ʾ�������µ�ֵ
	}
    m_ThBName=m_ThermalBoundaryName;
}

 void QForgingSystemDlg::ShowFS2(QMap<int,QString> m_ThB)
{
	QStringList strList;
	int step=0;
	strList.clear();
	strList<<"����"<<"�ڵ㼯��"<<"�˶�����"<<"�˶�����"<<"�˶���������"<<"�˶�����"<<"�ȱ߽�����";
	m_TableWiget->setHorizontalHeaderLabels(strList);
	int NRow=m_TableWiget->rowCount();
	QMap<int,QString>::Iterator it=m_ThB.begin();
	while(it!=m_ThB.end()){
		if(it.key()>step)step=it.key();
		it++;
	}
	if(NRow<step){
		for (int i=NRow;i<step;i++){
			m_TableWiget->insertRow(i);

			QTableWidgetItem *tmpItem2=new QTableWidgetItem(tr("����"));
			m_TableWiget->setItem(i,0,tmpItem2);

			MyCombox *tmpCombox01=new MyCombox(this,i);
			tmpCombox01->addItem(tr("�½�"));
			tmpCombox01->setFrame(false);
			m_TableWiget->setCellWidget(i,1,tmpCombox01);
			m_NodeSet.insert(i,tmpCombox01);

			MyCombox *tmpCombox1=new MyCombox(this,i);
			tmpCombox1->addItem(tr("���˶�"));
			tmpCombox1->addItem(tr("λ��"));
			tmpCombox1->addItem(tr("�ٶ�"));
			tmpCombox1->setFrame(false);
			m_TableWiget->setCellWidget(i,2,tmpCombox1);
			m_SportType.push_back(tmpCombox1);

			MyCombox *tmpCombox2=new MyCombox(this,i);
			tmpCombox2->addItem(tr("No"));
			tmpCombox2->addItem(tr("X"));
			tmpCombox2->addItem(tr("Y"));
			tmpCombox2->addItem(tr("Z"));
			tmpCombox2->setFrame(false);
			m_TableWiget->setCellWidget(i,3,tmpCombox2);
			m_Direction.push_back(tmpCombox2);

			MyCombox *tmpCombox3=new MyCombox(this,i);
			tmpCombox3->addItem(tr("No"));
			tmpCombox3->addItem(tr("����"));
			tmpCombox3->addItem(tr("��ʱ��仯"));
			tmpCombox3->setFrame(false);
			m_TableWiget->setCellWidget(i,4,tmpCombox3);
			m_ParameterType.insert(i,tmpCombox3);

			QIcon icon;
			icon.addFile(":/images/edit.png");
			QPushButton* m_Parameter = new QPushButton();
			m_Parameter->setIcon(icon);
			m_TableWiget->setCellWidget(i,5,m_Parameter);
			connect(m_Parameter,SIGNAL(clicked()),this,SLOT(MotionParameterSlot()));

			MyCombox *tmpCombox04=new MyCombox(this,i);
			tmpCombox04->addItem(tr("��"));
			tmpCombox04->setFrame(false);
			m_TableWiget->setCellWidget(i,6,tmpCombox04);
		}
	}
	NRow=m_TableWiget->rowCount();
	QString StrLP="";
	for(int i=0;i<NRow;i++){
		if(m_ThB.find(i+1)!= m_ThB.end()){
			MyCombox *ff1=(MyCombox*)(m_TableWiget->cellWidget(i, 6));
			QString dfc=m_ThB.value(i+1);
			ff1->addItem(dfc);
			int id=ff1->findText(dfc);
			ff1->setCurrentIndex(id);
		}
	}
}
void QForgingSystemDlg::ShowFS3(int styleB)
{
	
	QStringList strList;
	strList.clear();
	strList<<"����"<<"�ڵ㼯��"<<"�˶�����"<<"�˶�����"<<"�˶���������"<<"�˶�����"<<"�ȱ߽�����";
	m_TableWiget->setHorizontalHeaderLabels(strList);
	int NRow=m_TableWiget->rowCount();
	if (styleB ==0){//����;
		setWindowTitle("�˶��߽�");
		m_TableWiget->setColumnHidden(0,false);
		m_TableWiget->setColumnHidden(1,false);
		m_TableWiget->setColumnHidden(2,false);
		m_TableWiget->setColumnHidden(3,false);
		m_TableWiget->setColumnHidden(4,false);
		m_TableWiget->setColumnHidden(5,false);
		m_TableWiget->setColumnHidden(6,true);
	}else if(styleB==2){//����
		setWindowTitle("�����ƶ�");
		m_TableWiget->setColumnHidden(0,false);
		m_TableWiget->setColumnHidden(1,false);
		m_TableWiget->setColumnHidden(2,false);
		m_TableWiget->setColumnHidden(3,false);
		m_TableWiget->setColumnHidden(4,false);
		m_TableWiget->setColumnHidden(5,false);
		m_TableWiget->setColumnHidden(6,false);
	}
}
// void QForgingSystemDlg::ThermalBoundarySlot()
//{
//	int Row,index,x,y;
//	QString strIdName;
//	QPushButton *pushBtn = dynamic_cast<QPushButton*>(this->sender());
//	x=pushBtn->frameGeometry().x();
//	y=pushBtn->frameGeometry().y();
//	QModelIndex index02=m_TableWiget->indexAt(QPoint(x,y));
//	Row=index02.row();
//	if (Row <0){return;}
//
//	strIdName=m_ParameterType[Row]->currentText();
//	int sign=count02[Row];
//	if(sign!=1){
//		ThermalBoundary *ThermalBoundaryShow=new ThermalBoundary(this);
//		m_ThermalBoundaryListDlg.append(ThermalBoundaryShow);
//		ThermalBoundaryShow->show();
//		count02[Row]=1;
//	}else{
//		m_ThermalBoundaryListDlg.at(Row)->exec();
//	}
//}
