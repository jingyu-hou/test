#include "QInpActorWidge.h"

QInpActorWidge::QInpActorWidge(QWidget *parent)
	: QDialog(parent)
{
	creatGui();
}

QInpActorWidge::~QInpActorWidge()
{

}
void QInpActorWidge::creatGui()
{
	this->setWindowTitle(tr("部件隐藏/显示"));

	WidgetElSetInpList_ = new QListWidget();
	BtnOK_=new QPushButton(tr("确定"));
	checkBox_ = new QCheckBox(tr("全选"));
	
	QHBoxLayout *hlayout1=new QHBoxLayout();
	hlayout1->addWidget(checkBox_);
	hlayout1->addWidget(BtnOK_);

	QHBoxLayout *hlayout2=new QHBoxLayout();
	hlayout2->addWidget(WidgetElSetInpList_);


	QVBoxLayout *mainlayout=new QVBoxLayout(this);
	mainlayout->addLayout(hlayout1);
	mainlayout->addLayout(hlayout2);
	mainlayout->addStretch();
	this->setLayout(mainlayout);
	connect(checkBox_,SIGNAL(clicked()),this,SLOT(checkBoxSelectAllSlot()));
	connect(BtnOK_,SIGNAL(clicked()),this,SLOT(BtnOK()));

	//inital
	checkBox_->setCheckable(true);
	checkBox_->setChecked(true);

}
void QInpActorWidge::SetInpData(ElSetInpS ElSetData)
{
	QStringList strList=ElSetData.strElSetName;
	WidgetElSetInpList_->clear();
	for (int kk=0;kk<strList.size();kk++)
	{
		QListWidgetItem *item=new QListWidgetItem(WidgetElSetInpList_);
		QCheckBox *box=new QCheckBox(strList.at(kk));
		box->setCheckable(true);
		box->setChecked(true);
		WidgetElSetInpList_->addItem(item);
		WidgetElSetInpList_->setItemWidget(item,box);
	}
}
void QInpActorWidge::checkBoxSelectAllSlot()
{
	bool f=checkBox_->isChecked();
	int nSize=WidgetElSetInpList_->count();
	if (nSize<=0)return;
	
	for (int kk=0;kk<nSize;kk++)
	{
		QCheckBox *box =(QCheckBox *)WidgetElSetInpList_->itemWidget(WidgetElSetInpList_->item(kk));	
		box->setChecked(f);
	}
}
//--send
void QInpActorWidge::BtnOK()
{
	int nSize=WidgetElSetInpList_->count();
	if (nSize<=0)return;
	QStringList strListData;
	for (int kk=0;kk<nSize;kk++)
	{
		QCheckBox *box =(QCheckBox *)WidgetElSetInpList_->itemWidget(WidgetElSetInpList_->item(kk));	
		QString strT;
		strT="0,";
		if (box->isChecked())strT="1,";
		strT+=box->text();
		strListData<<strT;
	}
	emit emitCheckActor(strListData);
}