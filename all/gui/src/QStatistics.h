#ifndef QStatistics_H
//#define QStatistics_H
#include <QDialog>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "ADefineQ.h"
#include "Information_Widget.h"
#include "CRWManage.h"

class QStatistics : public QDialog
{
    Q_OBJECT

public:
     QStatistics(QWidget *parent=NULL);
    ~ QStatistics();
public:
   void SetInpData(ReadInpResultS Data);
   QListWidget *listWidget;
   QListWidgetItem *Node,*Element;
private:
	int NodeNumber,ElentNumber;
private slots:
	void ShowSlot(QListWidgetItem*);
};

#endif // QStatistics_H
