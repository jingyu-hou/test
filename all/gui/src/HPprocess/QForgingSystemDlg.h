#ifndef QForgingSystemDlg_H
#define QForgingSystemDlg_H

#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QComboBox>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QMouseEvent>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QHeaderView>
#include "ADefineQ.h"
#include "QMyCombox.h"
#include "CRWManage.h"
#include "QCurveDataPlot.h"
#include "Information_Widget.h"
#include "QThermalBoundaryDlg.h"


class QFSConst:public QDialog
{   
   Q_OBJECT
public:
    QFSConst(QWidget *parent1=NULL);
    ~QFSConst();
public:
    QPushButton *m_OKBtn, *m_CancelBtn;//确定，取消
	QLineEdit *ConstMotionParameters;
	QString   CMPOutPut;//CMP:ConstMotionParameters
	void SetInpDataC(FSystem FSData);//加载时写入到界面中
    void WriteToFSystemInps();//输出到inp文件
public slots:
   void m_OKBtnS();
   void m_CancelBtnS();
};

class QForgingSystemDlg : public QDialog
{
    Q_OBJECT
public:
    QForgingSystemDlg(QWidget *parent=NULL);
    ~QForgingSystemDlg();
private:
	int FSNumber;//FS:ForgingSystem
public:
    QPushButton *m_ApplyBtn, *m_AddBtn, *m_DelBtn;
    QPushButton *m_NextSystemBtn,*m_PreSystemBtn;
    QTableWidget* m_TableWiget;
public:
	int count01[2][20];//记录每行热交换类型，目前先分配20行，当行数大于20时需要扩展。
	int IndexNumber[40];//保存行索引号
	int count02[20];//热边界。
public:
	 ReadInpResultS ReadInpData0;
	 QVector<MyCombox*> m_NodeSet;
	 QVector<MyCombox*> m_SportType;
     QVector<MyCombox*> m_Direction;
	 QVector<MyCombox*>m_ForgingSystem;
	 QMap<int,MyCombox*> m_ParameterType;
	 //QMap<int,MyCombox*> m_ThB2;
	 //QMap<int,QString>m_ThB1;
	 QList<QFSConst*>  m_FSCurveDataListDlg;//FS:ForgingSystem
	 QList<QCurveDataPlot*>  m_FSCurveDataListDlg02;
	 QList<FSystem>   m_OutPutFSystemInfS;//用于保存输出信息
	 QStringList Nodeset;
	 QList<InpCurveInpS> m_CurveList;
	 QStringList           m_ThBName;
public:	
    void SetInpData(ReadInpResultS Data);//加载时写入到界面中
    void WriteToFSInps();//将锻造制度写到inp
	void ShowFS(QStringList m_ThermalBoundaryName);
	void ShowFS2(QMap<int,QString>);
	void ShowFS3(int);
public slots:
	//SA:SystemActs
    void ApplySABtnSlot();
    void NextSABtnSlot();
    void PreSABtnSlot();
    void AddSABtnSlot();//增加行
    void DelSABtnSlot();//删除行
	//void NewDelFSBtnSlot();//制度增加与删除
	void MotionParameterSlot();
	void ComboxSlotHighlight(int comWigId,int index0);
	void ComboxSlotHighlight2(NElSurfChsS);
	//void ThermalBoundarySlot();
	
signals:
    void emitPsetHighLight(NElSurfChsS);
	void emitForgingsystemNext(int);
};

#endif // QForgingSystemDlg_H