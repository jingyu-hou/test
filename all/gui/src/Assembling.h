#ifndef Assembling_H
#define Assembling_H
class vtkObject;
class vtkCallbackCommand;
#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>
#include <QStringList>
#include <QVector>
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
#include "QMyCombox.h"
#include "ADefineQ.h"
#include "CRWManage.h"
#include "InpDataVIS.h"
#include "vtkPolyDataMapper.h"
#include "Information_Widget.h"
#include "vtkProperty.h"


class Assembling : public QDialog
{
    Q_OBJECT

public:
     Assembling(QWidget *parent=NULL);
    ~ Assembling();
public:
	QLineEdit *P1,*P2,*MX,*MY,*MZ,*RX,*RY,*RZ,*ModelScaler;
	QCheckBox *Activation;
    QPushButton *m_ApplyBtn, *m_cancel,*pon1,*pon2;
	MyCombox *Nset;
	int btnId0;
public:
	QStringList m_PSetList;
    void SetInpData(ReadInpResultS Data);//加载时写入到界面中
public:
	static void Callback_PickPoint2(vtkObject *caller, unsigned long, void *clientdata, void*);
private:
	void setTex(vtkActor *actor,QString str,int pointId, double x, double y, double z);
	void PointSetSelectedPick2();
    InpDataVIS *inpObj_;  
	vtkCallbackCommand *pointPickCallback_;
private slots:
	void PickPointSlot(int);
public slots:
	void ApplySlot();
	void CancelSlot();
	void HighLightSlot();
	void ActivationSlot();
	void InitInpDataSlot1(InpDataVIS *InpObj);
signals:
   void emitPsetHighLight(NElSurfChsS);
   void emitPsetHighLight1(NElSurfChsS);
   void emitAssemblingS(AssemblingS_ZP);
};

#endif // Assembling_H