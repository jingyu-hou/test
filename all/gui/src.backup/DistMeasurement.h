#ifndef DistMeasurement_H
#define DistMeasurement_H
class vtkObject;
class vtkCallbackCommand;
#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QTableWidget>
#include <QMouseEvent>
#include <QLineEdit>
#include <QSplitter>
#include <QFrame>
#include <QTextEdit>
#include <QLabel>
#include <QCheckBox>
#include <QButtonGroup>
#include <vtkCallbackCommand.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPointPicker.h>
#include <qmath.h>
#include <QHeaderView>
#include "QMyCombox.h"
#include "ADefineQ.h"
#include "CRWManage.h"
#include "InpDataVIS.h"
#include "vtkPolyDataMapper.h"
#include "Information_Widget.h"
#include "vtkProperty.h"


class DistMeasurement : public QDialog
{
    Q_OBJECT

public:
     DistMeasurement(QWidget *parent=NULL);
    ~ DistMeasurement();
public:
	QLineEdit *DistText;
    QPushButton *FirstPointBtn, *SecondPointBtn,*DistanceBtn,*ClearBtn;
	QTableWidget* m_TableWiget;
	int btnId0;
public:
	static void Callback_PickPoint3(vtkObject *caller, unsigned long, void *clientdata, void*);
private:
	void setTex(vtkActor *actor,QString str,int pointId, double x, double y, double z);
	void PointSetSelectedPick2();
    InpDataVIS *inpObj_;  
	vtkCallbackCommand *pointPickCallback_;
private slots:
	void PickPointSlot(int);
	void ClearSlot();
	void DistantSlot();
public slots:
	void InitInpDataSlot2(InpDataVIS *InpObj);
};

#endif // DistMeasurement_H