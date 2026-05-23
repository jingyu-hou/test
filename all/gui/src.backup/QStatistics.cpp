#include "QStatistics.h"
QStatistics::QStatistics(QWidget *parent1)
      : QDialog(parent1)
{
  setWindowTitle("数量统计");
  NodeNumber=0;ElentNumber=0;
  listWidget = new QListWidget(this);
  QStringList list;
  Node=new QListWidgetItem("节点数量");
  Element=new QListWidgetItem("单元数量");
 /* list<<"节点数量"<<"单元数量";
  listWidget->addItems(list);*/
  listWidget->addItem(Node);
  listWidget->addItem(Element);

  QHBoxLayout *mainlayout = new QHBoxLayout;
  mainlayout->addWidget(listWidget);
 
  this->setMinimumWidth(300);
  this->setMaximumWidth(300);

  this->setMinimumHeight(200);
  this->setMaximumHeight(200);
  //connect(listWidget,SIGNAL(itemClicked(QString)),this,SLOT(ShowSlot(QString)));
  connect(listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(ShowSlot(QListWidgetItem*)));
}

void QStatistics::SetInpData(ReadInpResultS ReadInpData)
{
	int NodeNumber1,ElentNumber1;
	NodeNumber1=ReadInpData.TmpNodeInpS.strData.size();
	ElentNumber1=ReadInpData.TmpElInpS.strData.size();
    if(NodeNumber1>0)NodeNumber=NodeNumber1;
	if(ElentNumber1>0)ElentNumber=ElentNumber1;

}
void QStatistics::ShowSlot(QListWidgetItem* item)
{ 
	Information_Widget *infoW = Information_Widget::GetInstance();
	QString sdvd=item->text();
	if(sdvd=="节点数量"&&NodeNumber>0){
		infoW->ShowInformation("节点数量为: "+QString::number(NodeNumber,10));
	}else if(sdvd=="单元数量"&&ElentNumber>0){
		infoW->ShowInformation("单元数量为: "+QString::number(ElentNumber,10));
	}
}


QStatistics::~ QStatistics()
 {
	 
 }
