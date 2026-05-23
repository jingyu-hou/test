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
        connect(box,SIGNAL(clicked()),this,SLOT(ActorSlot()));
        Da.push_back(data->at(kk));
     }
    emit emitClickAcotrListWig(Da);
}
void QFrdActorWidge:: ActorSlot()
{
    QCheckBox *checkBox=(QCheckBox*) sender();
    int x=checkBox->mapToParent(QPoint(0,0)).x();
    int y=checkBox->mapToParent(QPoint(0,0)).y();
    QModelIndex index=widgetList_->indexAt(QPoint(x,y));
    int iRow=index.row();
    if (iRow<0 ||widgetList_->count()<=0)return;

    vector<int> Da;
    for (int kk=0; kk<widgetList_->count(); kk++)  { 
        QCheckBox *box =(QCheckBox *)widgetList_->itemWidget(widgetList_->item(kk));
        if(box->isChecked()){
            Da.push_back(box->text().remove("Element--").toInt());
        } 
    }
    emit emitClickAcotrListWig(Da);
} 