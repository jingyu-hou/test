#ifndef QINPACTORWIDGE_H
#define QINPACTORWIDGE_H

#include <QDialog>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include "ADefineQ.h"

class QInpActorWidge : public QDialog
{
	Q_OBJECT

public:
	QInpActorWidge(QWidget *parent=NULL);
	~QInpActorWidge();
public:
	void SetInpData(ElSetInpS ElSetData);
private:
	QListWidget *WidgetElSetInpList_;
	QCheckBox *checkBox_;
	QPushButton *BtnOK_;
	void creatGui();

private slots:
	void checkBoxSelectAllSlot();
	void BtnOK();
signals:
	void emitCheckActor(QStringList);

private:
	
};

#endif // QINPACTORWIDGE_H
