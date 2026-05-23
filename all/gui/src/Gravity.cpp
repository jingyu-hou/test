#include "Gravity.h"
Gravity::Gravity(QWidget *parent1)
      : QDialog(parent1)
{
	setWindowTitle("重力");
	QWidget *Data = new QWidget(this);

	QLabel *text=new QLabel(tr("重力值："),Data);
	Gravity_Value=new QLineEdit(Data);
	Gravity_Value->setText("");

	QLabel *text3=new QLabel(tr("方向："),Data);
	Gravity_Direction=new QComboBox(Data);
	Gravity_Direction->addItem("Z");
	Gravity_Direction->addItem("Y");
	Gravity_Direction->addItem("X");
	Gravity_Direction->addItem("自定义");
	
	connect(Gravity_Direction,SIGNAL(activated(int)),this,SLOT(Gravity_DirectionSlot(int)));

	ApplyBtn = new QPushButton(tr("应用"));
	CancelBtn = new QPushButton(tr("取消"));
	NextBtn = new QPushButton(tr("下一步"));
	NextBtn->hide();

	QLabel *text0=new QLabel(tr("矢量(X,Y,Z)："),Data);
	Gravity_Value1=new QLineEdit(Data);
	Gravity_Value1->setText("");
	Gravity_Value2=new QLineEdit(Data);
	Gravity_Value2->setText("");
	Gravity_Value3=new QLineEdit(Data);
	Gravity_Value3->setText("");
	Gravity_Value1->setEnabled(false);
	Gravity_Value2->setEnabled(false);
	Gravity_Value3->setEnabled(false);	

	MainLayout0 =new QGridLayout(this);
	MainLayout0->addWidget(text,0,0,1,1);
	MainLayout0->addWidget(Gravity_Value,0,1,1,1);
	MainLayout0->addWidget(text3,1,0,1,1);
	MainLayout0->addWidget(Gravity_Direction,1,1,1,1);

	MainLayout0->addWidget(text0,2,0,1,1);
	MainLayout0->addWidget(Gravity_Value1,2,1,1,1);
	MainLayout0->addWidget(Gravity_Value2,2,2,1,1);
	MainLayout0->addWidget(Gravity_Value3,2,3,1,1);

	MainLayout0->addWidget(ApplyBtn,3,1,1,1);
	MainLayout0->addWidget(CancelBtn,3,2,1,1);
	//MainLayout0->addWidget(NextBtn,3,3,1,1);

	this->setLayout(MainLayout0);

	connect(ApplyBtn,SIGNAL(clicked()),this,SLOT(ApplySlot()));
	connect(CancelBtn,SIGNAL(clicked()),this,SLOT(CanleSlot()));
	connect(NextBtn,SIGNAL(clicked()),this,SLOT(NextSlot()));
}

Gravity::~ Gravity()
{

}

void Gravity::ApplySlot()
{
	WriteGravityInps();
	//this->accept();
}

void Gravity::CanleSlot()
{
	this->close();

}
void Gravity::NextSlot()
{
	WriteGravityInps();
	this->accept();
}


void Gravity::SetInpData(ReadInpResultS ReadInpData)
{
	QString Value=ReadInpData.TmpGravityInps.Value;
	QString direation=ReadInpData.TmpGravityInps.direation;
	if(Value==""||direation=="")return;
	Gravity_Value->setText(Value);
	if(direation=="0,0,1")Gravity_Direction->setCurrentIndex(0);
	else if(direation=="0,1,0")Gravity_Direction->setCurrentIndex(1);
	else if(direation=="1,0,0")Gravity_Direction->setCurrentIndex(2);
	else{
		Gravity_Direction->setCurrentIndex(3);
		QStringList readline;
		Gravity_Value1->setEnabled(true);
		Gravity_Value2->setEnabled(true);
		Gravity_Value3->setEnabled(true);
		readline=direation.split(",");
		int Nsize=readline.size();
		Gravity_Value1->setText(readline.at(0));
		if(Nsize>=1)Gravity_Value2->setText(readline.at(1));
		if(Nsize>=2)Gravity_Value3->setText(readline.at(2));
	}
}

void Gravity::WriteGravityInps()
{
	QString Gravity_ValueStr=Gravity_Value->text();
	if(Gravity_ValueStr=="")return;
	m_OutputGravity_InfS.Value=Gravity_ValueStr;
	int ind=Gravity_Direction->currentIndex();
	if(ind==0)m_OutputGravity_InfS.direation="0,0,1";
	else if(ind==1)m_OutputGravity_InfS.direation="0,1,0";
	else if(ind==2)m_OutputGravity_InfS.direation="1,0,0";
	else if(ind==3)m_OutputGravity_InfS.direation=Gravity_Value1->text()+","+Gravity_Value2->text()+","+Gravity_Value3->text();
	if(m_OutputGravity_InfS.direation==",,"){
		m_OutputGravity_InfS.direation="";
		m_OutputGravity_InfS.Value="";
	}
}

void Gravity::Gravity_DirectionSlot(int i)
{
	if(i==0||i==1||i==2){
		Gravity_Value1->setEnabled(false);
		Gravity_Value2->setEnabled(false);
		Gravity_Value3->setEnabled(false);	
		return;
	}
	Gravity_Value1->setEnabled(true);
	Gravity_Value2->setEnabled(true);
	Gravity_Value3->setEnabled(true);
}

void Gravity::ShowHideSlot(int i)
{
	if(i==0){
		MainLayout0->addWidget(CancelBtn,3,2,1,1);
		CancelBtn->show();
		NextBtn->hide();
	}else if(i==1){
		MainLayout0->addWidget(NextBtn,3,2,1,1);
		CancelBtn->hide();
		NextBtn->show();
	}
}
