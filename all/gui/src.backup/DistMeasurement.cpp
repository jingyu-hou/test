#include "DistMeasurement.h"

DistMeasurement::DistMeasurement(QWidget *parent1)
      : QDialog(parent1)
{
	setWindowTitle("�������");
	QHBoxLayout *Hlayout = new QHBoxLayout();
	QHBoxLayout *Hlayout2 = new QHBoxLayout();
	FirstPointBtn = new QPushButton(tr("��һ����"));
	SecondPointBtn = new QPushButton(tr("�ڶ�����"));
	ClearBtn = new QPushButton(tr("���"));
	DistanceBtn = new QPushButton(tr("����"));
	DistText=new QLineEdit();
	QButtonGroup *groupSurfBtn=new QButtonGroup();
	groupSurfBtn->addButton(FirstPointBtn,0);
	groupSurfBtn->addButton(SecondPointBtn,1);

	Hlayout->addStretch();
    Hlayout->addWidget(FirstPointBtn);
	Hlayout->addStretch();
    Hlayout->addWidget(SecondPointBtn);
	Hlayout->addStretch();
	Hlayout->addWidget(ClearBtn);
    Hlayout->addStretch();
    Hlayout2->addWidget(DistanceBtn);
    Hlayout2->addWidget(DistText);

	QVBoxLayout *VLayout = new QVBoxLayout();
	m_TableWiget = new QTableWidget(this);
	m_TableWiget->setColumnCount(4);
	m_TableWiget->setRowCount(2);
	m_TableWiget->verticalHeaderItem(1);
	QStringList strList;
	strList<<"���"<<"X"<<"Y"<<"Z";
	m_TableWiget->setHorizontalHeaderLabels(strList);

    m_TableWiget->setItem(0,0,new QTableWidgetItem(tr("��һ����")));
	m_TableWiget->setItem(1,0,new QTableWidgetItem(tr("�ڶ�����")));
	m_TableWiget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	m_TableWiget->horizontalHeader()->setResizeMode(0, QHeaderView::Interactive);
	
	/*m_TableWiget->setItem(0,0,new QTableWidgetItem());
    m_TableWiget->item(0,0)->setText("��һ����");
	m_TableWiget->setItem(1,0,new QTableWidgetItem());
    m_TableWiget->item(1,0)->setText("�ڶ�����");*/

	VLayout->addWidget(m_TableWiget);
    QVBoxLayout *MainLayout = new QVBoxLayout(this);
    MainLayout->addLayout(VLayout);
    MainLayout->addLayout(Hlayout);
    MainLayout->addLayout(Hlayout2);
    setLayout(MainLayout);

	pointPickCallback_=0;
	btnId0=3;
	connect(groupSurfBtn,SIGNAL(buttonClicked(int)),this,SLOT(PickPointSlot(int)));
	connect(ClearBtn,SIGNAL(clicked()),this,SLOT(ClearSlot()));
	connect(DistanceBtn,SIGNAL(clicked()),this,SLOT(DistantSlot()));
}

DistMeasurement::~ DistMeasurement()
 {
	 delete(DistText);
 }

void DistMeasurement::ClearSlot()
 {
	 int row=m_TableWiget->rowCount();
	 if(row<=0)return;
	 for(int i=0;i<row;i++){
		 m_TableWiget->setItem(i,1,new QTableWidgetItem());
		 m_TableWiget->item(i,1)->setText("");
		 m_TableWiget->setItem(i,2,new QTableWidgetItem());
		 m_TableWiget->item(i,2)->setText("");
		 m_TableWiget->setItem(i,3,new QTableWidgetItem());
		 m_TableWiget->item(i,3)->setText("");
	 }
	 DistText->setText("");
	 //inpObj_->ClearHisPointVtkShow();
 }

void DistMeasurement::DistantSlot()
 {
	float p1[3],p2[3],df;
	p1[0]=m_TableWiget->item(0,1)->text().toFloat();
	p1[1]=m_TableWiget->item(0,2)->text().toFloat();
	p1[2]=m_TableWiget->item(0,3)->text().toFloat();
	p2[0]=m_TableWiget->item(1,1)->text().toFloat();
	p2[1]=m_TableWiget->item(1,2)->text().toFloat();
	p2[2]=m_TableWiget->item(1,3)->text().toFloat();
	df=qSqrt((p1[0]-p2[0])*(p1[0]-p2[0])+(p1[1]-p2[1])*(p1[1]-p2[1])+(p1[2]-p2[2])*(p1[2]-p2[2]));
	DistText->setText(QString("%1").arg(df));
 }

void DistMeasurement::PickPointSlot(int btnId)
{
	bool flag=false;
	if (!inpObj_)  return;
	btnId0=btnId;
	vtkRenderWindow *m_renWin = inpObj_->GetBindedRenderer()->GetRenderWindow();
	vtkRenderWindowInteractor *m_ir = m_renWin->GetInteractor();
	if (pointPickCallback_ == 0){
		pointPickCallback_ = vtkCallbackCommand::New();
		pointPickCallback_->SetCallback(Callback_PickPoint3);
		pointPickCallback_->SetClientData(this);
		flag=true;
	}
	/*if (pointPickCallback_ == 0&&btnId==0){
		pointPickCallback_ = vtkCallbackCommand::New();
		pointPickCallback_->SetCallback(Callback_PickPoint3);
		pointPickCallback_->SetClientData(this); 
		flag=true;
	}else if (pointPickCallback_ == 0&&btnId==1){
		pointPickCallback_ = vtkCallbackCommand::New();
		pointPickCallback_->SetCallback(Callback_PickPoint3);
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


void DistMeasurement::Callback_PickPoint3(vtkObject *caller, unsigned long, void *clientdata,void*)
{
	vtkRenderWindowInteractor *ir = vtkRenderWindowInteractor::SafeDownCast(caller);
    vtkRenderWindow *renWin = ir->GetRenderWindow();
	DistMeasurement *w = static_cast<DistMeasurement*>(clientdata);
	if (ir == 0|| w == 0)  return;
	int winx, winy;
	ir->GetEventPosition(winx, winy);
	vtkPointPicker *picker = vtkPointPicker::New();
	picker->SetTolerance(0.01);
	vtkRenderer *renderer = renWin->GetRenderers()->GetFirstRenderer();
	picker->Pick(winx, winy, 0, renderer);
	int pointId = picker->GetPointId();//�ñ�Ų��ǽڵ��ţ����Ҫ�õ��ڵ�����Ҫ����xyzֵ��ڵ�������в���ƥ�䡣
	double xyz[3];
	picker->GetPickPosition(xyz);
	if (pointId<0){
		Information_Widget::GetInstance()->ShowInformation("û�е㱻ѡ�У���������Ҫ�ı�ѡ�еĵ�");
		return;
	}
	QString str=picker->GetMapper()->GetClassName();
	w->setTex(picker->GetActor(),str,pointId+1, xyz[0], xyz[1], xyz[2]);
	//ir->RemoveObservers(vtkCommand::LeftButtonPressEvent);
}

void DistMeasurement::InitInpDataSlot2(InpDataVIS *InpObj)
{
    inpObj_=InpObj;
}
void DistMeasurement::setTex(vtkActor *actor,QString str,int pointId, double x, double y, double z)
{
	m_TableWiget->setItem(btnId0,1,new QTableWidgetItem());
	m_TableWiget->item(btnId0,1)->setText(QString("%1").arg(x));
	m_TableWiget->setItem(btnId0,2,new QTableWidgetItem());
	m_TableWiget->item(btnId0,2)->setText(QString("%1").arg(y));
	m_TableWiget->setItem(btnId0,3,new QTableWidgetItem());
	m_TableWiget->item(btnId0,3)->setText(QString("%1").arg(z));
	inpObj_->ClearHisPointVtkShow();
	inpObj_->SetHisPointVtkShow(pointId, x, y, z);
}