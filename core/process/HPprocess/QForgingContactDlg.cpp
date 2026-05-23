#include "QForgingContactDlg.h"

QC_onst::QC_onst(QWidget *parent1)
      : QDialog(parent1)
{
  setWindowTitle("�Ƚ���");
  HExchangeLE=new QLineEdit();
  HExchangeLE->setText("");

  QLabel *text=new QLabel();
  text->setText("������");
   
  QHBoxLayout *Hlayout = new QHBoxLayout();
  QHBoxLayout *Hlayout2 = new QHBoxLayout();
  m_OKBtn = new QPushButton(tr("ȷ��"));
  m_CancelBtn = new QPushButton(tr("ȡ��"));

  Hlayout->addStretch();
  Hlayout->addWidget(text);
  Hlayout->addWidget(HExchangeLE);
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

void QC_onst::SetInpDataC(CPPInInp CPPData){
//�����Ƚ����Ķ�ȡ����ʾ
	HExchangeLE->setText(CPPData.HEParam);
	WriteToContactInpsC();
}

void QC_onst::WriteToContactInpsC(){
//�����Ƚ��������
	HECPOutPut=HExchangeLE->text();
	//HECPOutPut.append(HECPOutPut); 
}

void QC_onst:: m_OKBtnS()
{
    WriteToContactInpsC();
	this->accept();
}

void QC_onst:: m_CancelBtnS()
{
  this->close();
}

QC_onst::~QC_onst()
{

}

QAdvancedOptions::QAdvancedOptions(QWidget *parent1)
      : QDialog(parent1)
{
  setWindowTitle("�߼�ѡ��");
  QWidget *Data = new QWidget(this);
  QWidget *Data1 = new QWidget(this);
  QWidget *Data12 = new QWidget(this);
  QWidget *Data13 = new QWidget(this);

  QHBoxLayout *Hlayout = new QHBoxLayout(Data);
  QLabel *text=new QLabel(tr("����նȣ�"),Data);
  NormalStiffnessC=new QLineEdit(Data);
  NormalStiffnessC->setText("1E14");
  Hlayout->addStretch();
  Hlayout->addWidget(text);
  Hlayout->addWidget(NormalStiffnessC);
  Hlayout->addStretch();

  QHBoxLayout *Hlayout1 = new QHBoxLayout(Data1);
  QLabel *text1=new QLabel(tr("����նȣ�"),Data1);
  TangentialStiffnessC=new QLineEdit(Data1);
  TangentialStiffnessC->setText("1E12");
  Hlayout1->addWidget(text1);
  Hlayout1->addWidget(TangentialStiffnessC);

  QHBoxLayout *Hlayout12 = new QHBoxLayout(Data12);
  QLabel *text2=new QLabel(tr("Ħ�����ȣ�"),Data12);
  FrictionGeneratesHeatC=new QLineEdit(Data12);
  Hlayout12->addWidget(text2);
  Hlayout12->addWidget(FrictionGeneratesHeatC);

  QHBoxLayout *Hlayout13 = new QHBoxLayout(Data13);
  QLabel *text3=new QLabel(tr("�Ӵ����ͣ�"),Data13);
  ContactType=new QComboBox(Data13);
  ContactType->addItem("����Ӵ�");
  ContactType->addItem("����Ӵ�");
  Hlayout13->addWidget(text3);
  Hlayout13->addWidget(ContactType);

  QHBoxLayout *Hlayout2 = new QHBoxLayout();
  m_OKAOBtn = new QPushButton(tr("ȷ��"));
  m_CancelAOBtn = new QPushButton(tr("ȡ��"));

  Hlayout2->addStretch();
  Hlayout2->addWidget(m_OKAOBtn);
  Hlayout2->addWidget(m_CancelAOBtn);
  Hlayout2->addStretch();

  QVBoxLayout *MainLayout0 = new QVBoxLayout(this);
  MainLayout0->addWidget(Data);
  MainLayout0->addWidget(Data1);
  MainLayout0->addWidget(Data12);
  MainLayout0->addWidget(Data13);
  MainLayout0->addLayout(Hlayout2);
  
  setLayout(MainLayout0);
  connect(m_OKAOBtn,SIGNAL(clicked()),this,SLOT(m_OKAOBtnS()));
  connect(m_CancelAOBtn,SIGNAL(clicked()),this,SLOT(m_CancelAOBtnS()));
}

void QAdvancedOptions::SetInpDataAD(CPPInInp CPPData){
//���ڷ���նȡ�����նȡ�Ħ�����ȺͽӴ����͵Ķ�ȡ����ʾ
	NormalStiffnessC->setText(CPPData.NormalStiffness);
	TangentialStiffnessC->setText(CPPData.TangentialStiffness);
	FrictionGeneratesHeatC->setText(CPPData.FGenerHeat);
    
    QString Cont=CPPData.M_ContactType.toLower();
	QString Cont1;
	if(Cont=="surface to surface")Cont1="����Ӵ�";
	else Cont1="����Ӵ�";

	int id=ContactType->findText(Cont1);
	ContactType->setCurrentIndex(id);
	WriteToContactInpsAD();
}

void QAdvancedOptions::WriteToContactInpsAD(){
//���ڷ���նȡ�����նȡ�Ħ�����ȺͽӴ����͵����
	m_AdvancedSetting.NormalStiffness=NormalStiffnessC->text();
	m_AdvancedSetting.TangentialStiffness=TangentialStiffnessC->text();
	m_AdvancedSetting.FrictionGeneratesHeat=FrictionGeneratesHeatC->text();
	m_AdvancedSetting.m_ContactType=ContactType->currentText();
}

void QAdvancedOptions:: m_OKAOBtnS()
{
    WriteToContactInpsAD();
	this->accept();
}

void QAdvancedOptions:: m_CancelAOBtnS()
{
  this->close();
}

QAdvancedOptions::~QAdvancedOptions()
{

}

ContactHelp::ContactHelp(QWidget *parent1)
      : QDialog(parent1)
{
  setWindowTitle("��ʾ");
  QTextEdit *Value1=new QTextEdit();
  Value1->setText("�ٷ���նȺ�����նȣ����ϵֿ�������κͼ��б��ε�����,һ������²���Ҫ���ã�"
	  "��������Զ�����,��������͸����ɲ��ҹ��ڷ��������ն�ȡֵ˵��������ļ����߲鿴�����ĵ���"
	  "                                                                                          "
	  "��Ħ������:��ֵ��0~1֮��,�����ֵΪ0.3,�����Ħ��������������30%���ݵ��ͼ��ϣ���70%���ݵ�ģ�ߡ�"
	  "                                                                                          "
	  "�۰�:���ð�֮����������������һ�����壬Ħ��������նȺ�����նȵȾ����������á�");
  QHBoxLayout *Hlayout = new QHBoxLayout();
  Hlayout->addStretch();
  Hlayout->addWidget(Value1);
  Hlayout->addStretch();
  QVBoxLayout *MainLayout0 = new QVBoxLayout(this);
  MainLayout0->addLayout(Hlayout);
  setLayout(MainLayout0);
}

ContactHelp::~ContactHelp()
{

}

QForgingContactDlg::QForgingContactDlg(QWidget *parent)
      : QDialog(parent)
{
	//��ʼ��ʼ������
	int JJII=0;
	for(int II=0;II<50;II++){
		IndexNumberAO[II]=0;
		count02[II]=0;
		for(int JJ=0;JJ<3;JJ++){
           count01[JJ][II]=0;
		   IndexNumber[JJII]=0;
           JJII++;
		}
	}
	//��ʼ����������
    setWindowTitle("�Ӵ���ϵ");
    QHBoxLayout *Hlayout = new QHBoxLayout();
    QHBoxLayout *Hlayout2 = new QHBoxLayout();

    m_ApplyBtn = new QPushButton(tr("Ӧ��"));
    m_AddBtn = new QPushButton(tr("����"));
    m_DelBtn = new QPushButton(tr("ɾ��"));
    m_PreContactBtn = new QPushButton(tr("��һ��"));
    m_NextContactBtn = new QPushButton(tr("��һ��"));
	m_ContactAutogenerate = new QPushButton(tr("�Զ�����"));
    m_ContactHelp = new QPushButton(tr("����"));
    Hlayout->addStretch();
    Hlayout->addWidget(m_AddBtn);
    Hlayout->addWidget(m_DelBtn);
    Hlayout->addStretch();
    Hlayout2->addWidget(m_ApplyBtn);
    Hlayout2->addWidget(m_PreContactBtn);
    Hlayout2->addWidget(m_NextContactBtn);
	Hlayout2->addWidget(m_ContactHelp);

    QVBoxLayout *VLayout = new QVBoxLayout();
    m_TableWiget = new QTableWidget();
    m_TableWiget->setColumnCount(7);
    m_TableWiget->setRowCount(2);
    m_TableWiget->verticalHeaderItem(1);
    QStringList strList;
    strList<<"�����"<<"ģ��"<<"Ħ��ϵ��"<<"�Ƚ���"<<"�Ƚ�������"<<"�߼�"<<"�Ƿ��";
    m_TableWiget->setHorizontalHeaderLabels(strList);
	/*for (int j=0;j<strList.size();j++){
		m_TableWiget->horizontalHeader()->setResizeMode(j,QHeaderView::ResizeToContents);
	}*/
	//m_TableWiget->horizontalHeader()->setStretchLastSection(true);
	//this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	for (int i=0; i<2;i++)
    {          
		MyCombox *Part=new MyCombox(this, i);
        //Part->addItem("�½�");
        Part->setFrame(false);
        m_TableWiget->setCellWidget(i,0,Part);
		m_contactPart.push_back(Part);

		MyCombox *MJ=new MyCombox(this, i);
        //MJ->addItem("�½�");
        MJ->setFrame(false);
        m_TableWiget->setCellWidget(i,1,MJ);
		m_contactMJ.push_back(MJ);

	    QTableWidgetItem* tmpItem2=new QTableWidgetItem(tr(""));
        m_TableWiget->setItem(i,2,tmpItem2);
		
		MyCombox *tmpCombox01=new MyCombox(this,i);
		tmpCombox01->addItem(tr("no"));
        tmpCombox01->addItem(tr("����"));
        tmpCombox01->addItem(tr("��ѹ���仯"));
		tmpCombox01->addItem(tr("���¶ȱ仯"));
		//tmpCombox01->addItem(tr("���¶ȱ仯"));
        //tmpCombox01->addItem(tr("��ѹ���仯"));
        tmpCombox01->setFrame(false);
        m_TableWiget->setCellWidget(i,3,tmpCombox01);
		m_HECDType.insert(i,tmpCombox01);

        QIcon icon;
		icon.addFile(":/images/edit.png");
        QPushButton* m_Parameter = new QPushButton();
		m_Parameter->setIcon(icon);
	    m_TableWiget->setCellWidget(i,4,m_Parameter);
		connect(m_Parameter,SIGNAL(clicked()),this,SLOT(contactHeatExChangeSlot()));

		QPushButton* m_set = new QPushButton(tr("����"));
	    m_TableWiget->setCellWidget(i,5,m_set);
	    connect(m_set,SIGNAL(clicked()),this,SLOT(contacAOSlot()));

		QCheckBox *button1 = new QCheckBox(tr("��"),this);
		m_TableWiget->setCellWidget(i,6,button1);
        Bding.insert(i,false);
		connect(button1,SIGNAL(clicked()),this,SLOT(CurrentValueSlot()));	
    }
	VLayout->addWidget(m_TableWiget);
    QVBoxLayout *MainLayout = new QVBoxLayout(this);
    MainLayout->addLayout(VLayout);
    MainLayout->addLayout(Hlayout);
    MainLayout->addLayout(Hlayout2);
    setLayout(MainLayout);
	m_TableWiget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	/*m_TableWiget->horizontalHeader()->setResizeMode(3, QHeaderView::Interactive);
	m_TableWiget->horizontalHeader()->setResizeMode(4, QHeaderView::Interactive);
	m_TableWiget->horizontalHeader()->setResizeMode(5, QHeaderView::Interactive);
	m_TableWiget->horizontalHeader()->setResizeMode(6, QHeaderView::Interactive);*/

	connect(m_ApplyBtn,SIGNAL(clicked()),this,SLOT(ApplyContactBtnSlot()));
    connect(m_AddBtn,SIGNAL(clicked()),this,SLOT(AddContactBtnSlot()));
    connect(m_DelBtn,SIGNAL(clicked()),this,SLOT(DelContactBtnSlot()));
    connect(m_NextContactBtn,SIGNAL(clicked()),this,SLOT(NextContactBtnSlot()));
    connect(m_PreContactBtn,SIGNAL(clicked()),this,SLOT(PreContactBtnSlot())); 
	connect(m_ContactHelp,SIGNAL(clicked()),this,SLOT(HelpShow()));
	this->setMinimumWidth(700);
}
void QForgingContactDlg::CurrentValueSlot()
{
   QCheckBox *btn=(QCheckBox*)sender();
   QTableWidgetItem* tmpItem2=new QTableWidgetItem(tr(""));
  
   bool ActivateOrNot=btn->checkState();//true������ѡ��  false����û��ѡ��
   int x=btn->mapToParent(QPoint(0,0)).x();//��ȡ�������x����
   int y=btn->mapToParent(QPoint(0,0)).y();//��ȡ�������y����
   QModelIndex index= m_TableWiget->indexAt(QPoint(x,y));

   int Nrow=index.row();
   if(ActivateOrNot){
	   QTableWidgetItem* tmpItem2 = m_TableWiget->item(Nrow, 2);//��ȡ��Ԫ��
	   tmpItem2->setFlags((Qt::ItemFlags)32);
	   tmpItem2->setBackgroundColor(QColor(200,200,200));//
	   int id=m_HECDType[Nrow]->findText("no");
	   m_HECDType[Nrow]->setCurrentIndex(id);
	   Bding.insert(Nrow,true);
   }else{
	 QTableWidgetItem* tmpItem2 = m_TableWiget->item(Nrow, 2);//��ȡ��Ԫ��
	 tmpItem2->setFlags((Qt::ItemFlags)63);
	 tmpItem2->setBackgroundColor(QColor(255,255,255,0));//��������ɫ����Ϊ͸��
	 Bding.insert(Nrow,false);
   }   
}

void QForgingContactDlg:: ApplyContactBtnSlot()
{
	QString info =(tr("�Ӵ���Ϣ�����ӵ�Inp������"));
    Information_Widget::GetInstance()->ShowInformation(QString(info));
    WriteToContactInps();
}
void QForgingContactDlg::NextContactBtnSlot()
{
    WriteToContactInps();
	this->accept();
}
void QForgingContactDlg::PreContactBtnSlot()
{
	WriteToContactInps();
	this->accept();
}

void QForgingContactDlg::AddContactBtnSlot()
{
	int nRow =m_TableWiget->rowCount();
    m_TableWiget->insertRow(nRow);

	MyCombox *Part=new MyCombox(this, nRow);
    //Part->addItem("�½�");
    Part->setFrame(false);
    m_TableWiget->setCellWidget(nRow,0,Part);
    m_contactPart.push_back(Part);

    MyCombox *MJ=new MyCombox(this, nRow);
    //MJ->addItem("�½�");
    MJ->setFrame(false);
    m_TableWiget->setCellWidget(nRow,1,MJ);
	m_contactMJ.push_back(MJ);
    //���������沢��ʾ
    MyCombox *ff = new MyCombox(this,m_contactPart.count());
	for (int i=0;i<=nRow;i++){
		ff=(MyCombox*)(m_TableWiget->cellWidget(i,0));
		int ccdd=ff->count();
		if (ff!=NULL&&ccdd>0){
			for (int kk=0;kk<ff->count();kk++){
				Part->addItem(ff->itemText(kk));
				MJ->addItem(ff->itemText(kk));
                Part->setItemData(kk,"2",(Qt::UserRole+3));
				MJ->setItemData(kk,"2",(Qt::UserRole+3));
			}
            break;
		}else{
			Part->addItems(Surfaceset);
			MJ->addItems(Surfaceset);
			for(int kk=0;kk<Surfaceset.size();kk++){
                Part->setItemData(kk,"2",(Qt::UserRole+3));
				MJ->setItemData(kk,"2",(Qt::UserRole+3));
			}
            break;
		}
	}
	m_contactPart.insert(nRow,Part);
	m_contactMJ.insert(nRow,MJ);

	QTableWidgetItem* tmpItem2=new QTableWidgetItem(tr(""));
    m_TableWiget->setItem(nRow,2,tmpItem2);

	MyCombox *tmpCombox01=new MyCombox(this,nRow);
	tmpCombox01->addItem(tr("no"));
    tmpCombox01->addItem(tr("����"));
    tmpCombox01->addItem(tr("��ѹ���仯"));
	tmpCombox01->addItem(tr("���¶ȱ仯"));
	//tmpCombox01->addItem(tr("���¶ȱ仯"));
    //tmpCombox01->addItem(tr("��ѹ���仯"));
    tmpCombox01->setFrame(false);
    m_TableWiget->setCellWidget(nRow,3,tmpCombox01);
	m_HECDType.insert(nRow,tmpCombox01);

	QIcon icon;
	icon.addFile(":/images/edit.png");
    QPushButton* m_Parameter = new QPushButton();
	m_Parameter->setIcon(icon);
	m_TableWiget->setCellWidget(nRow,4,m_Parameter);
	connect(m_Parameter,SIGNAL(clicked()),this,SLOT(contactHeatExChangeSlot()));
	//tmpCombox01=new MyCombox(this,1);
	
    QPushButton* m_set = new QPushButton(tr("����"));
	m_TableWiget->setCellWidget(nRow,5,m_set);
	connect(m_set,SIGNAL(clicked()),this,SLOT(contacAOSlot()));

	QCheckBox *button1 = new QCheckBox(tr("��"),this);
	m_TableWiget->setCellWidget(nRow,6,button1);
    Bding.insert(nRow,false);
	connect(button1,SIGNAL(clicked()),this,SLOT(CurrentValueSlot()));
	m_contactMJ.at(nRow)->setItemData(nRow,"2",(Qt::UserRole+3));
	m_contactPart.at(nRow)->setItemData(nRow,"2",(Qt::UserRole+3));//0�����㼯��,1������Ԫ����,2�����漯��
	connect(m_contactMJ.at(nRow),SIGNAL(clicked(int,int)),this,SLOT(ComboxSlotHighlight(int,int)));
	connect(m_contactPart.at(nRow),SIGNAL(clicked(int,int)),this,SLOT(ComboxSlotHighlight(int,int)));

}

void QForgingContactDlg::DelContactBtnSlot()
{
	int curRow =m_TableWiget->currentRow();
	if(curRow==0)curRow=m_TableWiget->rowCount()-1;

	/*int curRow =m_TableWiget->currentRow();
    if (curRow==-1)curRow =m_TableWiget->rowCount()-1;*/
	int INumber,INumber1,INumber2,INumber3,INumber4;
    if (curRow!=-1){
		m_TableWiget->removeRow(curRow);
		INumber=IndexNumber[4*curRow];
		INumber1=IndexNumber[4*curRow+1];
		INumber2=IndexNumber[4*curRow+2];
		INumber3=IndexNumber[4*curRow+3];
		INumber4=IndexNumberAO[curRow];
		int Num=count01[0][curRow];
		int Num1=count01[1][curRow];
		int Num2=count01[2][curRow];
		int Num3=count01[3][curRow];
		int Num4=count02[curRow];
		if(Num==1){
			count01[0][curRow]=0;
			m_cHEC_CurveDataListDlg02.takeAt(INumber);
		}
		if(Num1==2){
			count01[1][curRow]=0;
			m_cHEC_CurveDataListDlg.takeAt(INumber1);
		}
		if(Num2==3){
			count01[2][curRow]=0;
			m_cHEC_CurveDataListDlg.takeAt(INumber2);
		}
		if(Num3==4){
			count01[3][curRow]=0;
			m_cHEC_CurveDataListDlg.takeAt(INumber3);
		}
		if(Num4==1){
			count02[curRow]=0;
			m_QAdvancedOptionsListDlg.takeAt(INumber4);
		}
		
		
	}
}
void QForgingContactDlg::SetInpData(ReadInpResultS ReadInpData)
{
	int nRowSize = m_TableWiget->rowCount();
	//���¼����ж��м����Ӵ��Ի��߰󶨶�
	int nContactPart=0;
    if (ReadInpData.TmpCPInInp.size()>0||ReadInpData.TmpCPTieInp.size()>0)
    {
        nContactPart=ReadInpData.TmpCPInInp.size()+ReadInpData.TmpCPTieInp.size();
	}else{
		 nRowSize = m_TableWiget->rowCount();
         Surfaceset=ReadInpData.TmpSurfaceInps.strSurfaceName;
	     int II=0,JJ=0;
	     for(II=0;II<nRowSize;II++){
		     m_contactMJ.at(II)->clear();
		     m_contactPart.at(II)->clear();
		    for(JJ=0;JJ<Surfaceset.size();JJ++){
			    m_contactMJ.at(II)->addItem(Surfaceset.at(JJ));
			    m_contactPart.at(II)->addItem(Surfaceset.at(JJ));
			    m_contactMJ.at(II)->setItemData(JJ,"2",(Qt::UserRole+3));
			    m_contactPart.at(II)->setItemData(JJ,"2",(Qt::UserRole+3));//0�����㼯��,1������Ԫ����,2�����漯��
			    connect(m_contactMJ.at(II),SIGNAL(clicked(int,int)),this,SLOT(ComboxSlotHighlight(int,int)));
			    connect(m_contactPart.at(II),SIGNAL(clicked(int,int)),this,SLOT(ComboxSlotHighlight(int,int)));
			}
		 }
		return;
	}
	//���ϼ����ж��м����Ӵ��Ի��߰󶨶�

	//���¼���ʹ�������е�������Ӵ��ԺͰ󶨵��������
	if (nRowSize<nContactPart)
     {     
		 for (int i=nRowSize;i<nContactPart;i++){
			m_TableWiget->insertRow(i);
            MyCombox *Part=new MyCombox(this, i);
            //Part->addItem("�½�");
            Part->setFrame(false);
            m_TableWiget->setCellWidget(i,0,Part);
		    m_contactPart.push_back(Part);

            MyCombox *MJ=new MyCombox(this, i);
            //MJ->addItem("�½�");
            MJ->setFrame(false);
            m_TableWiget->setCellWidget(i,1,MJ);
		    m_contactMJ.push_back(MJ);	

	        QTableWidgetItem* tmpItem2=new QTableWidgetItem(tr(""));
            m_TableWiget->setItem(i,2,tmpItem2);

		    MyCombox *tmpCombox01=new MyCombox(this,i);
		    tmpCombox01->addItem(tr("no"));
            tmpCombox01->addItem(tr("����"));
            tmpCombox01->addItem(tr("��ѹ���仯"));
		    tmpCombox01->addItem(tr("���¶ȱ仯"));
		    //tmpCombox01->addItem(tr("���¶ȱ仯"));
            //tmpCombox01->addItem(tr("��ѹ���仯"));
            tmpCombox01->setFrame(false);
            m_TableWiget->setCellWidget(i,3,tmpCombox01);
		    m_HECDType.insert(i,tmpCombox01);

            QIcon icon;
		    icon.addFile(":/images/edit.png");
            QPushButton* m_Parameter = new QPushButton();
		    m_Parameter->setIcon(icon);
	        m_TableWiget->setCellWidget(i,4,m_Parameter);
		    connect(m_Parameter,SIGNAL(clicked()),this,SLOT(contactHeatExChangeSlot()));

		    QPushButton* m_set = new QPushButton(tr("����"));
	        m_TableWiget->setCellWidget(i,5,m_set);
	        connect(m_set,SIGNAL(clicked()),this,SLOT(contacAOSlot()));
		 }
	}else if (nRowSize>nContactPart){ //clear and remove;
        for (int i=nContactPart;i<nRowSize;i++){
            m_TableWiget->removeRow(i);
		}
	}
	//���ϼ���ʹ�������е�������Ӵ��ԺͰ󶨵��������

	//���Ӽ��ϵ�������
    nRowSize = m_TableWiget->rowCount();
    //QStringList Surfaceset;
    Surfaceset=ReadInpData.TmpSurfaceInps.strSurfaceName;
	int II=0,JJ=0;
	for(II=0;II<nRowSize;II++){
		m_contactMJ.at(II)->clear();
		m_contactPart.at(II)->clear();
		//m_contactMJ.at(II)->addItem("�½�");
		//m_contactPart.at(II)->addItem("�½�");
		for(JJ=0;JJ<Surfaceset.size();JJ++){
			m_contactMJ.at(II)->addItem(Surfaceset.at(JJ));
			m_contactPart.at(II)->addItem(Surfaceset.at(JJ));
			m_contactMJ.at(II)->setItemData(JJ,"2",(Qt::UserRole+3));
			m_contactPart.at(II)->setItemData(JJ,"2",(Qt::UserRole+3));//0�����㼯��,1������Ԫ����,2�����漯��
			connect(m_contactMJ.at(II),SIGNAL(clicked(int,int)),this,SLOT(ComboxSlotHighlight(int,int)));
			connect(m_contactPart.at(II),SIGNAL(clicked(int,int)),this,SLOT(ComboxSlotHighlight(int,int)));
		}
	}
	//���¼��в��������Ǹ���,����ʾ
	m_cHEC_CurveDataListDlg02.clear(); 
	EOFCONumber=ReadInpData.TmpCPInInp.size();
	for (int i=0;i<EOFCONumber;i++){//--���Ҳ���ʾ
            QString strTmpName = ReadInpData.TmpCPInInp.at(i).MainSurface;
            int id=m_contactMJ.at(i)->findText(strTmpName);
            m_contactMJ.at(i)->setCurrentIndex(id); 

			QString strTmpName2 = ReadInpData.TmpCPInInp.at(i).SlaveSurface;
            int id2=m_contactPart.at(i)->findText(strTmpName2);
            m_contactPart.at(i)->setCurrentIndex(id2);
			//���¼���������ʾĦ��ϵ��
			QString PartName=ReadInpData.TmpCPInInp.at(i).CCPName.simplified ();
			int indexsize=ReadInpData.TmpCPPInInp.size();
			for(int j=0;j<indexsize;j++){
				QString ContactPartName=ReadInpData.TmpCPPInInp.at(j).CPPName.simplified ().toLower();
				if(ContactPartName==PartName){
					QString FrictionC=ReadInpData.TmpCPPInInp.at(j).FrictionCoefficient;
					if(FrictionC!=""){
						QTableWidgetItem *tmpItem=new QTableWidgetItem(FrictionC);
						m_TableWiget->setItem(i,2,tmpItem);
					}
				}
			}
			//���ϼ���������ʾĦ��ϵ��


			//���¼��������Ƚ�������ʾ
			if(ReadInpData.TmpCPPInInp.at(i).HEParam.simplified()!=""){
				int id=m_HECDType[i]->findText("����");
				m_HECDType[i]->setCurrentIndex(id);
				QC_onst *C_constShow1=new QC_onst(this);
				CPPInInp CData;
				CData.HEParam=ReadInpData.TmpCPPInInp.at(i).HEParam;
				count01[0][i]=1;
				IndexNumber[4*i]=m_cHEC_CurveDataListDlg02.size();
				C_constShow1->SetInpDataC(CData);
	            m_cHEC_CurveDataListDlg02.append(C_constShow1);	
			}else if(ReadInpData.TmpCPPInInp.at(i).HeatExchangeType=="��ѹ���仯"){
				int id=m_HECDType[i]->findText("��ѹ���仯");
				m_HECDType[i]->setCurrentIndex(id);
				QCurveDataPlot *tmpCurve = new QCurveDataPlot(this);
				InpCurveInpS CData;
				CData.strName=ReadInpData.TmpCPPInInp.at(i).strCurveName;
				count01[1][i]=2;
				IndexNumber[4*i+1]=m_cHEC_CurveDataListDlg.size();
				CData.strDataX=ReadInpData.TmpCPPInInp.at(i).strDataX;
				CData.strDataY=ReadInpData.TmpCPPInInp.at(i).strDataY;
                tmpCurve->SetData(CData);
				m_cHEC_CurveDataListDlg.append(tmpCurve);
			}else if(ReadInpData.TmpCPPInInp.at(i).HeatExchangeType=="���¶ȱ仯"){
				int id=m_HECDType[i]->findText("���¶ȱ仯");
				m_HECDType[i]->setCurrentIndex(id);
				QCurveDataPlot *tmpCurve = new QCurveDataPlot(this);
				InpCurveInpS CData;
				CData.strName=ReadInpData.TmpCPPInInp.at(i).strCurveName;
				count01[2][i]=3;
				IndexNumber[4*i+2]=m_cHEC_CurveDataListDlg.size();
				CData.strDataX=ReadInpData.TmpCPPInInp.at(i).strDataX;
				CData.strDataY=ReadInpData.TmpCPPInInp.at(i).strDataY;
				tmpCurve->SetData(CData);
				m_cHEC_CurveDataListDlg.append(tmpCurve);
			}
			//���ϼ��������Ƚ�������ʾ

			//���¼��������Ƿ�󶨲���
			QCheckBox *button1 = new QCheckBox(tr("��"),this);
	        m_TableWiget->setCellWidget(i,6,button1);
            button1->setChecked(false);
			Bding.insert(i,false);
	        connect(button1,SIGNAL(clicked()),this,SLOT(CurrentValueSlot()));
	}
	int j1=ReadInpData.TmpCPInInp.size();
	for (int i=0;i<ReadInpData.TmpCPTieInp.size();i++){//--���Ҳ���ʾ
		    
            QString strTmpName = ReadInpData.TmpCPTieInp.at(i).MainSurface;
            int id=m_contactMJ.at(j1)->findText(strTmpName);
            m_contactMJ.at(j1)->setCurrentIndex(id); 

			QString strTmpName2 =ReadInpData.TmpCPTieInp.at(i).SlaveSurface;
            int id2=m_contactPart.at(j1)->findText(strTmpName2);
            m_contactPart.at(j1)->setCurrentIndex(id2);
            //���¼��������Ƿ�󶨲���
			QCheckBox *button1 = new QCheckBox(tr("��"),this);
	        m_TableWiget->setCellWidget(j1,6,button1);
            button1->setChecked(true);
			QTableWidgetItem* tmpItem2 = m_TableWiget->item(j1, 2);//��ȡ��Ԫ��
	        tmpItem2->setFlags((Qt::ItemFlags)32);
	        tmpItem2->setBackgroundColor(QColor(200,200,200));//
			Bding.insert(j1,true);
	        connect(button1,SIGNAL(clicked()),this,SLOT(CurrentValueSlot()));
			j1++;	
	}
	//���ϼ��в��������Ǹ���,����ʾ

	//�������ʾ�߼�����
	nRowSize = m_TableWiget->rowCount();
	m_QAdvancedOptionsListDlg.clear();
	//ReadInpData.TmpCPInInp.size();
	int j2=ReadInpData.TmpCPInInp.size();
	for(int Ii=0;Ii<nRowSize;Ii++){
		QAdvancedOptions *QAdvancedOptionsShow=new QAdvancedOptions(this);
		CPPInInp AOData;
		if(!Bding[Ii]){
			AOData.NormalStiffness=ReadInpData.TmpCPPInInp.at(Ii).NormalStiffness;
		    AOData.TangentialStiffness=ReadInpData.TmpCPPInInp.at(Ii).TangentialStiffness;
		    AOData.FGenerHeat=ReadInpData.TmpCPPInInp.at(Ii).FGenerHeat;
			AOData.M_ContactType=ReadInpData.TmpCPInInp.at(Ii).SurfaceType;
		    count02[Ii]=1;
		}else{
			AOData.NormalStiffness="";
		    AOData.TangentialStiffness="";
		    AOData.FGenerHeat="";
			AOData.M_ContactType="";
		    count02[Ii]=1;  
		}
		IndexNumberAO[Ii]=m_QAdvancedOptionsListDlg.size();
		QAdvancedOptionsShow->SetInpDataAD(AOData);
	    m_QAdvancedOptionsListDlg.append(QAdvancedOptionsShow);
		if(Ii>=j2){
			    QC_onst *C_constShow1=new QC_onst(this);
				CPPInInp CData;
				CData.HEParam="";
				count01[0][Ii]=1;
				C_constShow1->SetInpDataC(CData);
				IndexNumber[4*Ii]=m_cHEC_CurveDataListDlg02.size();
	            m_cHEC_CurveDataListDlg02.append(C_constShow1);
		}
	}
	WriteToContactInps();
}

void QForgingContactDlg::WriteToContactInps()
{
	OCIInpS  OputInf;
	InpCurveInpS    nodeCurveList;

	int TabRowCnt = m_TableWiget->rowCount();
    int TabColCnt = m_TableWiget->columnCount();
	if (TabRowCnt>0){
		m_OutPutContInfS.clear();
	}	

	int Num2=m_cHEC_CurveDataListDlg.size();
	for(int i=0; i<TabRowCnt; i++){
		OputInf.MJ=m_contactMJ.at(i)->currentText();
		OputInf.Part=m_contactPart.at(i)->currentText();
        OputInf.FrictionCoefficient=m_TableWiget->item(i,2)->text();
		OputInf.HeatExchangeType=m_HECDType[i]->currentText();

		if(m_HECDType[i]->currentText()=="no"){
			OputInf.HeatExchangeParameter="";
		}else if(m_HECDType[i]->currentText()=="����"&&count01[0][i]==1){
			OputInf.HeatExchangeParameter=m_cHEC_CurveDataListDlg02.at(i)->HECPOutPut;	
		}else if(m_HECDType[i]->currentText()=="��ѹ���仯"&&count01[1][i]==2){
            int IJ=IndexNumber[4*i+1];
			if(Num2!=0){
				OputInf.AmplitudeName=m_cHEC_CurveDataListDlg.at(IJ)->m_CurveName->text();
			}
		}else if(m_HECDType[i]->currentText()=="���¶ȱ仯"&&count01[2][i]==3){
			int IJ=IndexNumber[4*i+2];
			if(Num2!=0){
				OputInf.AmplitudeName=m_cHEC_CurveDataListDlg.at(IJ)->m_CurveName->text();	
			}
		}

		if(m_QAdvancedOptionsListDlg.size()==0){
			OputInf.NormalStiffness="";
			OputInf.TangentialStiffness="";
			OputInf.FrictionGeneratesHeat="";
			OputInf.m_ContactType="";
		}else if(IndexNumberAO[i]==i){
			OputInf.NormalStiffness=m_QAdvancedOptionsListDlg.at(i)->m_AdvancedSetting.NormalStiffness;
		    OputInf.TangentialStiffness=m_QAdvancedOptionsListDlg.at(i)->m_AdvancedSetting.TangentialStiffness;
		    OputInf.FrictionGeneratesHeat=m_QAdvancedOptionsListDlg.at(i)->m_AdvancedSetting.FrictionGeneratesHeat;
			OputInf.m_ContactType=m_QAdvancedOptionsListDlg.at(i)->m_AdvancedSetting.m_ContactType;
		}
		
		OputInf.strCofficient="1";
		OputInf.Binding=Bding[i];
		 
		m_OutPutContInfS.append(OputInf);
	}
	int DX=m_cHEC_CurveDataListDlg.size();
	if(DX>0)m_nodeCurveList.clear();
	for(int II=0;II<DX;II++){
		nodeCurveList=m_cHEC_CurveDataListDlg.at(II)->m_HIPCurveNode;
		m_nodeCurveList.append(nodeCurveList);
	}
}

void QForgingContactDlg::contactHeatExChangeSlot()
{
  int Row,index,x,y;
  QString strIdName;
  QPushButton *pushBtn = dynamic_cast<QPushButton*>(this->sender());
  x=pushBtn->frameGeometry().x();
  y=pushBtn->frameGeometry().y();
  QModelIndex index02=m_TableWiget->indexAt(QPoint(x,y));
  Row=index02.row();
  if (Row <0){return;}
  //strIdName=m_contactHeatExChange.at(Row)->currentText();
  strIdName=m_HECDType[Row]->currentText();
  //index=m_contactHeatExChange.at(Row)->count();
  if(strIdName=="����"){
	 int sign=count01[0][Row];
     if(sign!=1){
       QC_onst *C_constShow=new QC_onst(this);
	   m_cHEC_CurveDataListDlg02.append(C_constShow);
	   C_constShow->show();
	   IndexNumber[4*Row]=m_cHEC_CurveDataListDlg02.size()-1;
	   count01[0][Row]=1;
	 }else{
	   int IN,IN1;
       IN1=4*Row;
	   IN=IndexNumber[IN1];
	   m_cHEC_CurveDataListDlg02.at(IN)->exec(); 
	 }
  }else if(strIdName == "��ѹ���仯"){
	 int sign=count01[1][Row];
     if(sign!=2){
		QCurveDataPlot *tmpCurve = new QCurveDataPlot(this);
        QString strName;
        int ret = tmpCurve->exec();   
		if (ret == QDialog::Accepted){//ok
            strName=tmpCurve->m_HIPCurveNode.strName;
            if (strName == ""){return;}
			m_cHEC_CurveDataListDlg.append(tmpCurve);
			IndexNumber[4*Row+1]=m_cHEC_CurveDataListDlg.size()-1;
			count01[1][Row]=2;
		}
	 }else{
	    int IN,IN1;
        IN1=4*Row+1;
		IN=IndexNumber[IN1];
		m_cHEC_CurveDataListDlg.at(IN)->exec();
	 }
  }else if(strIdName == "���¶ȱ仯"){
	  int sign=count01[2][Row];
	  if(sign!=3){
		QCurveDataPlot *tmpCurve = new QCurveDataPlot(this);
        QString strName;
        int ret = tmpCurve->exec();   
		if (ret == QDialog::Accepted){//ok
            strName=tmpCurve->m_HIPCurveNode.strName;
            if (strName == ""){return;}
			m_cHEC_CurveDataListDlg.append(tmpCurve);
			IndexNumber[4*Row+2]=m_cHEC_CurveDataListDlg.size()-1;
			count01[2][Row]=3;
		}
	 }else{	
	    int IN,IN1;
        IN1=4*Row+2;
		IN=IndexNumber[IN1];
		m_cHEC_CurveDataListDlg.at(IN)->exec();
	  }
  }else if(strIdName == "��ʱ��仯"){
	  int sign=count01[3][Row];
	  if(sign!=4){
		QCurveDataPlot *tmpCurve = new QCurveDataPlot(this);
        QString strName;
        int ret = tmpCurve->exec();   
		if (ret == QDialog::Accepted){//ok
            strName=tmpCurve->m_HIPCurveNode.strName;
            if (strName == ""){return;}
			m_cHEC_CurveDataListDlg.append(tmpCurve);
			IndexNumber[4*Row+4]=m_cHEC_CurveDataListDlg.size()-1;
			count01[3][Row]=4;
		}
	 }else{
		int IN,IN1;
        IN1=4*Row+4;
        IN=IndexNumber[IN1];
        m_cHEC_CurveDataListDlg.at(IN)->exec();
	 }
  }
}

void QForgingContactDlg::contacAOSlot()
{
  int Row,index,x,y;
  QString strIdName;
  QPushButton *pushBtn = dynamic_cast<QPushButton*>(this->sender());
  x=pushBtn->frameGeometry().x();
  y=pushBtn->frameGeometry().y();
  QModelIndex index02=m_TableWiget->indexAt(QPoint(x,y));
  Row=index02.row();
  if (Row <0||Bding[Row]){return;}
  int sign=count02[Row];
  if(sign!=1){
    QAdvancedOptions *QAdvancedOptionsShow=new QAdvancedOptions(this);
	m_QAdvancedOptionsListDlg.append(QAdvancedOptionsShow);
	QAdvancedOptionsShow->show();
	IndexNumberAO[Row]=m_QAdvancedOptionsListDlg.size()-1;
	count02[Row]=1;
   }else{
	int IN,IN1;
    IN1=Row; 
	IN=IndexNumberAO[IN1];
	m_QAdvancedOptionsListDlg.at(IN)->exec();
	 }
  }

void QForgingContactDlg::HelpShow()
{
  ContactHelp *ContactHelp01=new ContactHelp(this);
  ContactHelp01->show();
}

void QForgingContactDlg::ComboxSlotHighlight(int comWigId,int index0)
{  
   QString str,str0,str1;
   int iTmp;
   MyCombox *btn=(MyCombox*)sender();
   int x=btn->mapToParent(QPoint(0,0)).x();//��ȡ�������x����
   int y=btn->mapToParent(QPoint(0,0)).y();//��ȡ�������y����
   QModelIndex index= m_TableWiget->indexAt(QPoint(x,y));
   int col=index.column();//��ȡ������б��������
   if (index0<0){return;}
   if(col==0){
	    str =((MyCombox*)m_TableWiget->cellWidget(comWigId, 0))->currentText();
       //str = m_contactMJ.at(comWigId)->currentText();//��ǰ��ѡcombox
   }else if(col==1){
       str =((MyCombox*)m_TableWiget->cellWidget(comWigId, 1))->currentText();
       //str1 = m_contactPart.at(comWigId)->currentText();//��ǰ��ѡcombox
       //str1 =((MyCombox*)m_TableWiget->cellWidget(comWigId,1))->currentText();
   }
    
  if (str==""||str=="�½�")return;
  //--��������
  if(col==0){
	    iTmp= m_contactMJ.at(comWigId)->itemData(index0,Qt::UserRole+3).toInt();//��ǰ��ѡcombox
  }else if(col==1){
	    iTmp= m_contactPart.at(comWigId)->itemData(index0,Qt::UserRole+3).toInt();//��ǰ��ѡcombox
  }

  NElSurfChsS tmpChs;
  tmpChs.strName =str;
  tmpChs.iType = 2;
  emit emitPsetHighLight(tmpChs);
}

QForgingContactDlg::~QForgingContactDlg()
{
	m_contactMJ.clear();
    m_contactPart.clear();
    m_contactFriction.clear();
    //m_contactHeatExChange.clear();
	m_contactSet.clear();
}

void QForgingContactDlg::AppendSurfSetComboxContactSlot(QString str)
{
    int nRow = m_TableWiget->rowCount();
    for (int i=0; i<nRow; i++)
	{
		MyCombox *TmpcomboxPset0 = (MyCombox*)m_TableWiget->cellWidget(i, 0);
        MyCombox *TmpcomboxPset1 = (MyCombox*)m_TableWiget->cellWidget(i, 1);//new MyCombox(this, nRow);
        if (TmpcomboxPset0->findText(str) == -1&&TmpcomboxPset1->findText(str) == -1)
		{
           TmpcomboxPset0->addItem(str);
		   TmpcomboxPset1->addItem(str);
           
        }
    }
}

void QForgingContactDlg::ShowContact(int styleB)
{
	QStringList strList;
	strList.clear();
	strList<<"�����"<<"ģ��"<<"Ħ��ϵ��"<<"�Ƚ���"<<"�Ƚ�������"<<"�߼�"<<"�Ƿ��";
	m_TableWiget->setHorizontalHeaderLabels(strList);
	int NRow=m_TableWiget->rowCount();
	if (styleB ==0){//����;
		m_TableWiget->setColumnHidden(0,false);
		m_TableWiget->setColumnHidden(1,false);
		m_TableWiget->setColumnHidden(2,true);
		m_TableWiget->setColumnHidden(3,false);
		m_TableWiget->setColumnHidden(4,false);
		m_TableWiget->setColumnHidden(5,true);
		m_TableWiget->setColumnHidden(6,true);
	}else if(styleB==2){//����
		m_TableWiget->setColumnHidden(0,false);
		m_TableWiget->setColumnHidden(1,false);
		m_TableWiget->setColumnHidden(2,false);
		m_TableWiget->setColumnHidden(3,false);
		m_TableWiget->setColumnHidden(4,false);
		m_TableWiget->setColumnHidden(5,false);
		m_TableWiget->setColumnHidden(6,false);
	}
}