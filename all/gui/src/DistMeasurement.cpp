#include "DistMeasurement.h"
#include "PostProcess/FrdDataVIS.h"
#include <vtkActor.h>
#include <vtkCellPicker.h>
#include <vtkMapper.h>

DistMeasurement::DistMeasurement(QWidget *parent1)
      : QDialog(parent1)
{
	setWindowTitle(QString::fromUtf8("距离测量"));
		setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
	QHBoxLayout *Hlayout = new QHBoxLayout();
	QHBoxLayout *Hlayout2 = new QHBoxLayout();
	FirstPointBtn = new QPushButton(tr("第一个点"));
	SecondPointBtn = new QPushButton(tr("第二个点"));
	ClearBtn = new QPushButton(tr("清除"));
	DistanceBtn = new QPushButton(tr("距离"));
	DistText=new QLineEdit();
	FirstPointBtn->setAutoDefault(false);
	SecondPointBtn->setAutoDefault(false);
	ClearBtn->setAutoDefault(false);
	DistanceBtn->setAutoDefault(false);
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
	strList<<"序号"<<"X"<<"Y"<<"Z";
	m_TableWiget->setHorizontalHeaderLabels(strList);

    m_TableWiget->setItem(0,0,new QTableWidgetItem(tr("第一个点")));
	m_TableWiget->setItem(1,0,new QTableWidgetItem(tr("第二个点")));
	m_TableWiget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	m_TableWiget->horizontalHeader()->setResizeMode(0, QHeaderView::Interactive);
	
	/*m_TableWiget->setItem(0,0,new QTableWidgetItem());
    m_TableWiget->item(0,0)->setText("第一个点");
	m_TableWiget->setItem(1,0,new QTableWidgetItem());
    m_TableWiget->item(1,0)->setText("第二个点");*/

	VLayout->addWidget(m_TableWiget);
    QVBoxLayout *MainLayout = new QVBoxLayout(this);
    MainLayout->addLayout(VLayout);
    MainLayout->addLayout(Hlayout);
    MainLayout->addLayout(Hlayout2);
    setLayout(MainLayout);

	inpObj_=0;
	frdVISObj_=0;
	pointPickCallback_=0;
	pointPickInteractor_=0;
	pointPickObserverId_=0;
	btnId0=3;
	connect(groupSurfBtn,SIGNAL(buttonClicked(int)),this,SLOT(PickPointSlot(int)));
	connect(ClearBtn,SIGNAL(clicked()),this,SLOT(ClearSlot()));
	connect(DistanceBtn,SIGNAL(clicked()),this,SLOT(DistantSlot()));
}

DistMeasurement::~ DistMeasurement()
 {
	 StopPointPicking();
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
	 if (inpObj_) inpObj_->ClearHisPointVtkShow();
	 if (frdVISObj_) frdVISObj_->ClearHisPointVtkShow();
 }

void DistMeasurement::DistantSlot()
 {
	float p1[3],p2[3],df;
	QTableWidgetItem *item00 = m_TableWiget->item(0,1);
	QTableWidgetItem *item01 = m_TableWiget->item(0,2);
	QTableWidgetItem *item02 = m_TableWiget->item(0,3);
	QTableWidgetItem *item10 = m_TableWiget->item(1,1);
	QTableWidgetItem *item11 = m_TableWiget->item(1,2);
	QTableWidgetItem *item12 = m_TableWiget->item(1,3);
	if (!item00 || !item01 || !item02 || !item10 || !item11 || !item12) {
		return;
	}
	p1[0]=item00->text().toFloat();
	p1[1]=item01->text().toFloat();
	p1[2]=item02->text().toFloat();
	p2[0]=item10->text().toFloat();
	p2[1]=item11->text().toFloat();
	p2[2]=item12->text().toFloat();
	df=qSqrt((p1[0]-p2[0])*(p1[0]-p2[0])+(p1[1]-p2[1])*(p1[1]-p2[1])+(p1[2]-p2[2])*(p1[2]-p2[2]));
	DistText->setText(QString("%1").arg(df));
 }

void DistMeasurement::PickPointSlot(int btnId)
{
	if (!inpObj_) {
		if (!frdVISObj_) {
			Information_Widget::GetInstance()->ShowInformation(tr("请先导入并显示前处理网格或后处理FRD结果，再进行距离测量。"));
			return;
		}
	}
	vtkRenderer *renderer = 0;
	if (frdVISObj_) {
		renderer = frdVISObj_->GetBindedRenderer();
	} else if (inpObj_) {
		renderer = inpObj_->GetBindedRenderer();
	}
	if (!renderer || !renderer->GetRenderWindow()) {
		Information_Widget::GetInstance()->ShowInformation(tr("当前视图未初始化，无法进行距离测量。"));
		return;
	}
	btnId0=btnId;
	vtkRenderWindow *m_renWin = renderer->GetRenderWindow();
	vtkRenderWindowInteractor *m_ir = m_renWin->GetInteractor();
	if (!m_ir) {
		Information_Widget::GetInstance()->ShowInformation(tr("当前视图未初始化，无法进行距离测量。"));
		return;
	}
	StopPointPicking();
	pointPickCallback_ = vtkCallbackCommand::New();
	pointPickCallback_->SetCallback(Callback_PickPoint3);
	pointPickCallback_->SetClientData(this);
	pointPickInteractor_ = m_ir;
	pointPickObserverId_ = m_ir->AddObserver(vtkCommand::LeftButtonPressEvent, pointPickCallback_);
	Information_Widget::GetInstance()->ShowInformation(tr("请在当前网格视图中点击需要测量的点。"));
	show();
	raise();
}


void DistMeasurement::Callback_PickPoint3(vtkObject *caller, unsigned long, void *clientdata,void*)
{
	vtkRenderWindowInteractor *ir = vtkRenderWindowInteractor::SafeDownCast(caller);
	DistMeasurement *w = static_cast<DistMeasurement*>(clientdata);
	if (ir == 0 || w == 0) return;
	vtkRenderWindow *renWin = ir->GetRenderWindow();
	if (renWin == 0) return;
	vtkRenderer *renderer = renWin->GetRenderers()->GetFirstRenderer();
	if (renderer == 0) return;

	int winx, winy;
	ir->GetEventPosition(winx, winy);
	double xyz[3] = {0.0, 0.0, 0.0};
	int pickedId = -1;
	vtkActor *pickedActor = 0;

	vtkPointPicker *picker = vtkPointPicker::New();
	picker->SetTolerance(0.01);
	picker->Pick(winx, winy, 0, renderer);
	if (picker->GetPointId() >= 0) {
		pickedId = picker->GetPointId();
		picker->GetPickPosition(xyz);
		pickedActor = picker->GetActor();
	}

	if (pickedId < 0) {
		vtkCellPicker *cellPicker = vtkCellPicker::New();
		cellPicker->SetTolerance(0.005);
		cellPicker->Pick(winx, winy, 0, renderer);
		if (cellPicker->GetCellId() >= 0) {
			pickedId = cellPicker->GetCellId();
			cellPicker->GetPickPosition(xyz);
			pickedActor = cellPicker->GetActor();
		}
		cellPicker->Delete();
	}

	if (pickedId < 0) {
		Information_Widget::GetInstance()->ShowInformation(tr("No point or element picked. Click inside the mesh area."));
		picker->Delete();
		return;
	}

	QString str;
	if (pickedActor && pickedActor->GetMapper()) str = pickedActor->GetMapper()->GetClassName();
	w->setTex(pickedActor, str, pickedId + 1, xyz[0], xyz[1], xyz[2]);
	picker->Delete();
}
void DistMeasurement::InitInpDataSlot2(InpDataVIS *InpObj)
{
    StopPointPicking();
    inpObj_=InpObj;
}
void DistMeasurement::InitFrdSlot2(FrdDataVIS *FrdObj)
{
    StopPointPicking();
    frdVISObj_=FrdObj;
}
void DistMeasurement::setTex(vtkActor *actor,QString str,int pointId, double x, double y, double z)
{
	m_TableWiget->setItem(btnId0,1,new QTableWidgetItem());
	m_TableWiget->item(btnId0,1)->setText(QString("%1").arg(x));
	m_TableWiget->setItem(btnId0,2,new QTableWidgetItem());
	m_TableWiget->item(btnId0,2)->setText(QString("%1").arg(y));
	m_TableWiget->setItem(btnId0,3,new QTableWidgetItem());
	m_TableWiget->item(btnId0,3)->setText(QString("%1").arg(z));
	if (frdVISObj_) {
		frdVISObj_->ClearHisPointVtkShow();
		frdVISObj_->SetHisPointVtkShow(pointId, x, y, z);
	} else if (inpObj_) {
		inpObj_->ClearHisPointVtkShow();
		inpObj_->SetHisPointVtkShow(pointId, x, y, z);
	}
	raise();
}

void DistMeasurement::StopPointPicking()
{
	if (pointPickInteractor_ && pointPickObserverId_ != 0) {
		pointPickInteractor_->RemoveObserver(pointPickObserverId_);
	}
	pointPickObserverId_ = 0;
	pointPickInteractor_ = 0;
	if (pointPickCallback_) {
		pointPickCallback_->Delete();
		pointPickCallback_ = 0;
	}
}
