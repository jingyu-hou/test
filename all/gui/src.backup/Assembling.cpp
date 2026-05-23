#include "Assembling.h"

Assembling::Assembling(QWidget *parent1)
      : QDialog(parent1)
{
	/*setFixedHeight(280);
	setFixedWidth(350);*/
	/*setMinimumHeight(280);
	setMinimumWidth(350);*/
  setWindowTitle("装配/缩放");
  QLabel *text=new QLabel();
  text->setText("点集合：");
  Nset=new MyCombox(this,0);
  connect(Nset,SIGNAL(clicked(int,int)),this,SLOT(HighLightSlot()));
  QLabel *text0=new QLabel();
  text0->setText("缩放比例：");
  ModelScaler=new QLineEdit();
  ModelScaler->setText("1.0");
  ModelScaler->setEnabled(false);
  Activation = new QCheckBox(tr("激活"),this);
  connect(Activation,SIGNAL(clicked()),this,SLOT(ActivationSlot()));
  QLabel *pointtext=new QLabel();
  pointtext->setText("选择平移操作中需要重合的两个点：");
  QLabel *point=new QLabel();
  point->setText("选择两个点：");
  QButtonGroup *groupSurfBtn = new QButtonGroup();
  QLabel *point1=new QLabel();
  point1->setText("平 移 起 点：");
  P1=new QLineEdit();
  P1->setText("");
  pon1=new QPushButton(tr("选择"));
  QLabel *point2=new QLabel();
  point2->setText("平 移 终 点：");
  P2=new QLineEdit();
  P2->setText("");
  pon2=new QPushButton(tr("选择"));
  groupSurfBtn->addButton(pon1,0);
  groupSurfBtn->addButton(pon2,1);
  QLabel *move=new QLabel();
  move->setText("平 动：X,Y,Z");
  MX=new QLineEdit();
  MX->setText("");
  MY=new QLineEdit();
  MY->setText("");
  MZ=new QLineEdit();
  MZ->setText("");

  QLabel *rotate=new QLabel();
  rotate->setText("转 动：RX,RY,RZ");
  RX=new QLineEdit();
  RX->setText("");
  RY=new QLineEdit();
  RY->setText("");
  RZ=new QLineEdit();
  RZ->setText("");

  m_ApplyBtn=new QPushButton(tr("应用"));
  m_cancel=new QPushButton(tr("取消"));

  QGridLayout *mainLayout =new QGridLayout();
  mainLayout->addWidget(text,0,0,1,1);
  mainLayout->addWidget(Nset,0,1,1,1);
  mainLayout->addWidget(text0,1,0,1,1);
  mainLayout->addWidget(ModelScaler,1,1,1,1);
  mainLayout->addWidget(Activation,1,2,1,1);
  //mainLayout->addWidget(pointtext,2,0,1,2);

  mainLayout->addWidget(point1,2,0,1,1);
  mainLayout->addWidget(P1,2,1,1,1);
  mainLayout->addWidget(pon1,2,2,1,1);

  mainLayout->addWidget(point2,3,0,1,1);
  mainLayout->addWidget(P2,3,1,1,1);
  mainLayout->addWidget(pon2,3,2,1,1);

  mainLayout->addWidget(move,4,0,1,1);
  mainLayout->addWidget(MX,4,1,1,1);
  mainLayout->addWidget(MY,4,2,1,1);
  mainLayout->addWidget(MZ,4,3,1,1);
  mainLayout->addWidget(rotate,5,0,1,1);
  mainLayout->addWidget(RX,5,1,1,1);
  mainLayout->addWidget(RY,5,2,1,1);
  mainLayout->addWidget(RZ,5,3,1,1);

  mainLayout->addWidget(m_ApplyBtn,6,1,1,1);
  mainLayout->addWidget(m_cancel,6,2,1,1);
  this->setLayout(mainLayout);

  /*QHBoxLayout *Hlayout = new QHBoxLayout();
  QHBoxLayout *Hlayout0 = new QHBoxLayout();
  QHBoxLayout *Hlayout1 = new QHBoxLayout();
  QHBoxLayout *Hlayout12 = new QHBoxLayout();
  QHBoxLayout *Hlayout2 = new QHBoxLayout();
  QHBoxLayout *Hlayout3 = new QHBoxLayout();
  QHBoxLayout *Hlayout4 = new QHBoxLayout();
  QHBoxLayout *Hlayout5 = new QHBoxLayout();

  Hlayout->addWidget(text);
  Hlayout->addWidget(Nset);
  Hlayout->addStretch();

  Hlayout0->addWidget(text0);
  Hlayout0->addWidget(ModelScaler);
  Hlayout0->addWidget(Activation);
  Hlayout0->addStretch();

  Hlayout12->addWidget(pointtext);

  Hlayout1->addWidget(point1);
  Hlayout1->addWidget(P1);
  Hlayout1->addWidget(pon1);
  Hlayout1->addStretch();

  Hlayout2->addWidget(point2);
  Hlayout2->addWidget(P2);
  Hlayout2->addWidget(pon2);
  Hlayout2->addStretch();

  Hlayout3->addWidget(move);
  Hlayout3->addWidget(MX);
  Hlayout3->addWidget(MY);
  Hlayout3->addWidget(MZ);
  Hlayout3->addStretch();
  

  Hlayout4->addWidget(rotate);
  Hlayout4->addWidget(RX);
  Hlayout4->addWidget(RY);
  Hlayout4->addWidget(RZ);
  Hlayout4->addStretch();

  Hlayout5->addStretch();
  Hlayout5->addWidget(m_ApplyBtn);
  Hlayout5->addStretch();
  Hlayout5->addWidget(m_cancel);
  Hlayout5->addStretch();

  QVBoxLayout *MainLayout = new QVBoxLayout(this);
  MainLayout->addLayout(Hlayout);
  MainLayout->addLayout(Hlayout0);
  MainLayout->addLayout(Hlayout12);
  MainLayout->addLayout(Hlayout1);
  MainLayout->addLayout(Hlayout2);
  MainLayout->addLayout(Hlayout3);
  MainLayout->addLayout(Hlayout4);
  MainLayout->addLayout(Hlayout5);
  setLayout(MainLayout);*/
  this->setMinimumWidth(500);
  this->setMaximumWidth(1000);

  this->setMinimumHeight(350);
  this->setMaximumHeight(500);

  pointPickCallback_=0;
  btnId0=3;

  connect(groupSurfBtn,SIGNAL(buttonClicked(int)),this,SLOT(PickPointSlot(int)));
  connect(m_ApplyBtn,SIGNAL(clicked()),this,SLOT(ApplySlot()));
  connect(m_cancel,SIGNAL(clicked()),this,SLOT(CancelSlot()));
}

Assembling::~ Assembling()
 {
	 m_PSetList.clear();
	 delete(P1);
	 delete(P2);
	 delete(MX);
	 delete(MY);
	 delete(MZ);
	 delete(RX);
	 delete(RY);
	 delete(RZ);
	 delete(ModelScaler);
 }

void Assembling::SetInpData(ReadInpResultS ReadInpData)
{
	m_PSetList=ReadInpData.TmpNsetInps.strNSetName;
	Nset->clear();
	Nset->addItems(m_PSetList);
}

void Assembling::PickPointSlot(int btnId)
{
	bool flag=false;
	if (!inpObj_)  return;
	btnId0=btnId;
	vtkRenderWindow *m_renWin = inpObj_->GetBindedRenderer()->GetRenderWindow();
	vtkRenderWindowInteractor *m_ir = m_renWin->GetInteractor();
	if (pointPickCallback_ == 0){
		pointPickCallback_ = vtkCallbackCommand::New();
		pointPickCallback_->SetCallback(Callback_PickPoint2);
		pointPickCallback_->SetClientData(this); 
		flag=true;
	}
	/*if (pointPickCallback_ == 0&&btnId==0){
		pointPickCallback_ = vtkCallbackCommand::New();
		pointPickCallback_->SetCallback(Callback_PickPoint2);
		pointPickCallback_->SetClientData(this); 
		flag=true;
	}else if (pointPickCallback_ == 0&&btnId==1){
		pointPickCallback_ = vtkCallbackCommand::New();
		pointPickCallback_->SetCallback(Callback_PickPoint2);
		pointPickCallback_->SetClientData(this); 
		flag=true;
	}*/
	if(flag)m_ir->AddObserver(vtkCommand::LeftButtonPressEvent, pointPickCallback_);
	pointPickCallback_=0;
	/*if(!flag ){
		m_ir->RemoveObservers(vtkCommand::LeftButtonPressEvent, pointPickCallback_);
	}else{
		m_ir->AddObserver(vtkCommand::LeftButtonPressEvent, pointPickCallback_);
	}*/
}

void Assembling::ApplySlot()
{
	
	AssemblingS_ZP tmMRoveA;
	tmMRoveA.strLingA=Nset->currentText();
	if(tmMRoveA.strLingA==""){
		P1->setText("");P2->setText("");MX->setText("");
		MY->setText("");MZ->setText("");RX->setText("");
		RY->setText("");RZ->setText("");
		return;
	}
	inpObj_->ClearHisPointVtkShow();
	QString L1=P1->text();
	QString L2=P2->text();
	if(L1!=""&&L2!=""){
		QStringList p1,p2;
		float nm=-L1.split(",",QString::SkipEmptyParts).at(0).toFloat()+L2.split(",",QString::SkipEmptyParts).at(0).toFloat();
		MX->setText(QString("%1").arg(nm));
		nm=-L1.split(",",QString::SkipEmptyParts).at(1).toFloat()+L2.split(",",QString::SkipEmptyParts).at(1).toFloat();
		MY->setText(QString("%1").arg(nm));
        nm=-L1.split(",",QString::SkipEmptyParts).at(2).toFloat()+L2.split(",",QString::SkipEmptyParts).at(2).toFloat();
		MZ->setText(QString("%1").arg(nm));
	}else if(L1!=""||L2!=""){
		Information_Widget::GetInstance()->ShowInformation("请选择另外一个点");
	}
	tmMRoveA.MRoveA[0]=MX->text().toFloat();
	tmMRoveA.MRoveA[1]=MY->text().toFloat();
	tmMRoveA.MRoveA[2]=MZ->text().toFloat();
	tmMRoveA.MRoveA[3]=RX->text().toFloat();
	tmMRoveA.MRoveA[4]=RY->text().toFloat();
	tmMRoveA.MRoveA[5]=RZ->text().toFloat();
	bool RE=Activation->checkState();
	if(RE){
		tmMRoveA.MRoveA[6]=ModelScaler->text().toFloat();
	}else{
        tmMRoveA.MRoveA[6]=0.0;
	}
	if(MX->text()==""&&MY->text()==""&&MZ->text()==""&&
		RX->text()==""&&RY->text()==""&&RZ->text()==""&&tmMRoveA.MRoveA[6]==0.0){
			P1->setText("");P2->setText("");MX->setText("");
			MY->setText("");MZ->setText("");RX->setText("");
			RY->setText("");RZ->setText("");
			return;
	}
	P1->setText("");P2->setText("");MX->setText("");
	MY->setText("");MZ->setText("");RX->setText("");
	RY->setText("");RZ->setText("");
	emit emitAssemblingS(tmMRoveA);
}

void Assembling::CancelSlot()
{
	inpObj_->ClearHisPointVtkShow();
	P1->clear();
	P2->clear();
	MX->clear();
	MY->clear();
	MZ->clear();
	RX->clear();
	RY->clear();
	RZ->clear();
	this->close();
}
void Assembling::HighLightSlot()
{
	QString str=Nset->currentText();
	NElSurfChsS tmpChs;
	tmpChs.strName =str;
    tmpChs.iType =0;
	emit emitPsetHighLight(tmpChs);
}

void Assembling::ActivationSlot()
{
	QCheckBox *btn=(QCheckBox*)sender();

	bool ActivateOrNot=btn->checkState();//true代表被选中  false代表没被选中

	if(ActivateOrNot){
		ModelScaler->setEnabled(true);
		P1->setEnabled(false);
		P2->setEnabled(false);
		MX->setEnabled(false);
		MY->setEnabled(false);
		MZ->setEnabled(false);
		RX->setEnabled(false);
		RY->setEnabled(false);
		RZ->setEnabled(false);
	}else{
		ModelScaler->setEnabled(false);
		P1->setEnabled(true);
		P2->setEnabled(true);
		MX->setEnabled(true);
		MY->setEnabled(true);
		MZ->setEnabled(true);
		RX->setEnabled(true);
		RY->setEnabled(true);
		RZ->setEnabled(true);	
	} 
}

void Assembling::Callback_PickPoint2(vtkObject *caller, unsigned long, void *clientdata,void*)
{
	vtkRenderWindowInteractor *ir = vtkRenderWindowInteractor::SafeDownCast(caller);
    vtkRenderWindow *renWin = ir->GetRenderWindow();
	Assembling *w = static_cast<Assembling*>(clientdata);
	if (ir == 0|| w == 0)  return;
	int winx, winy;
	ir->GetEventPosition(winx, winy);
	vtkPointPicker *picker = vtkPointPicker::New();
	picker->SetTolerance(0.01);
	vtkRenderer *renderer = renWin->GetRenderers()->GetFirstRenderer();
	picker->Pick(winx, winy, 0, renderer);
	int pointId = picker->GetPointId();//该编号不是节点编号，如果要得到节点编号需要根据xyz值与节点坐标进行查找匹配。
	double xyz[3];
	picker->GetPickPosition(xyz);
	if (pointId<0){
		Information_Widget::GetInstance()->ShowInformation("没有点被选中，请点击其需要的被选中的点");
		return;
	}
	QString str=picker->GetMapper()->GetClassName();
	w->setTex(picker->GetActor(),str,pointId+1, xyz[0], xyz[1], xyz[2]);
	//ir->RemoveObservers(vtkCommand::LeftButtonPressEvent);
}

void Assembling::InitInpDataSlot1(InpDataVIS *InpObj)
{
    inpObj_=InpObj;
}
void Assembling::setTex(vtkActor *actor,QString str,int pointId, double x, double y, double z)
{
	QString sTr=QString("%1").arg(x)+","+QString("%1").arg(y)+","+QString("%1").arg(z);
	if(btnId0==0)P1->setText(sTr);
	else if(btnId0==1)P2->setText(sTr);

	inpObj_->ClearHisPointVtkShow();
	inpObj_->SetHisPointVtkShow(pointId, x, y, z);
	

}