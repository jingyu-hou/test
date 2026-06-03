#include "QFrdActorWidge.h"
#include <QPushButton>
#include <QModelIndex>

QFrdActorWidge::QFrdActorWidge(QWidget *parent)
    : QWidget(parent)
{
    creatGui();
}

QFrdActorWidge::~QFrdActorWidge()
{

}
void QFrdActorWidge::creatGui()
{
    widgetList_ = new QListWidget();   
    
	QVBoxLayout *MainLayout = new QVBoxLayout(this);
	MainLayout->addWidget(widgetList_);
	MainLayout->addStretch();

	this->setLayout(MainLayout);
}
void QFrdActorWidge::upDataListWidget(const vector<int>* data)
{
    if (data->empty())return;
    int nSize=data->size();
    widgetList_->clear();
    vector<int> Da;
    for (int kk=0; kk<nSize; kk++)
    {
        QListWidgetItem *item = new QListWidgetItem(widgetList_);
        QCheckBox *box =new QCheckBox("Element--" + QString("%1").arg(data->at(kk)));
        box->setCheckable(true);
        box->setChecked(true);
        widgetList_->addItem(item);
        widgetList_->setItemWidget(item, box);
        connect(box,SIGNAL(toggled(bool)),this,SLOT(ActorSlot()));
        Da.push_back(data->at(kk));
     }
    emit emitClickAcotrListWig(Da);
}
void QFrdActorWidge:: ActorSlot()
{
    if (widgetList_->count()<=0)return;

    vector<int> Da;
    for (int kk=0; kk<widgetList_->count(); kk++)  { 
        QCheckBox *box =(QCheckBox *)widgetList_->itemWidget(widgetList_->item(kk));
        if(box && box->isChecked()){
            Da.push_back(box->text().remove("Element--").toInt());
        } 
    }
    emit emitClickAcotrListWig(Da);
} 
