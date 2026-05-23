#ifndef Gravity_H
#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QLineEdit>
#include <QTextEdit>
#include <QHeaderView>
#include <QLabel>
#include "ADefineQ.h"
#include "QMyCombox.h"
#include "CRWManage.h"

class Gravity : public QDialog
{
    Q_OBJECT
public:
     Gravity(QWidget *parent=NULL);
    ~ Gravity();
public:
	QLineEdit *Gravity_Value,*Gravity_Value1,*Gravity_Value2,*Gravity_Value3;//Gravity_Direction
    QPushButton *ApplyBtn, *CancelBtn,*NextBtn;
	QComboBox *Gravity_Direction;
	Gravity_Inp  m_OutputGravity_InfS;//用于保存输出信息
	QGridLayout *MainLayout0;
public:
   void SetInpData(ReadInpResultS Data);//加载时写入到界面中
    void WriteGravityInps();//输出到inp文件
public slots:
    void Gravity_DirectionSlot(int);
	void ApplySlot();
	void CanleSlot();
	void NextSlot();
	void ShowHideSlot(int);
};

#endif // Gravity_H