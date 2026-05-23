#ifndef QForgingContactDlg_H
#define QForgingContactDlg_H

#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QComboBox>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QMouseEvent>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QTimer>
#include <QCheckBox>
#include "ADefineQ.h"
#include "QMyCombox.h"
#include "CRWManage.h"
#include "QCurveDataPlot.h"
#include "Information_Widget.h"

//#include "QAdvancedOptions.h"

class QC_onst:public QDialog
{   
   Q_OBJECT
public:
    QC_onst(QWidget *parent1=NULL);
    ~QC_onst();
public:
    QPushButton *m_OKBtn, *m_CancelBtn;//确定，取消
	QLineEdit *HExchangeLE;
	QString   HECPOutPut;//热交换参数的输出
public:
    void SetInpDataC(CPPInInp CPPData);//加载时写入到界面中
    void WriteToContactInpsC();//输出到inp文件
public slots:
   void m_OKBtnS();
   void m_CancelBtnS();
};

class QAdvancedOptions:public QDialog
{   
    Q_OBJECT
public:
    QAdvancedOptions(QWidget *parent1=NULL);
    ~QAdvancedOptions();
public:
    QPushButton *m_OKAOBtn, *m_CancelAOBtn;//确定，取消
	QLineEdit *NormalStiffnessC,*TangentialStiffnessC,*FrictionGeneratesHeatC;
	QComboBox *ContactType;
	OCIInpS2 m_AdvancedSetting;
public:
	void SetInpDataAD(CPPInInp CPPData);//加载时写入到界面中
    void WriteToContactInpsAD();//输出到inp文件
public slots:
   void m_OKAOBtnS();
   void m_CancelAOBtnS();
};

class ContactHelp:public QDialog
{   
public:
    ContactHelp(QWidget *parent1=NULL);
    ~ContactHelp();
};

class QForgingContactDlg : public QDialog
{
    Q_OBJECT

public:
    QForgingContactDlg(QWidget *parent=NULL);
    ~QForgingContactDlg();
private:
	int count01[4][50];//记录每行热交换类型，目前先分配50行，当行数大于50时需要扩展。
	int IndexNumber[200];//保存行索引号
	int count02[50];
	int IndexNumberAO[50];//保存行索引号
	int EOFCONumber;//摩擦接触对数量

public:
    QPushButton *m_ApplyBtn, *m_AddBtn, *m_DelBtn;
    QPushButton *m_NextContactBtn,*m_PreContactBtn;
	QPushButton *m_ContactAutogenerate,*m_ContactHelp;

    QTableWidget* m_TableWiget;
	QMap<int,bool>Bding;
	QMap<int,MyCombox*> m_HECDType;//m_contactHeatExChangeType
    QVector<MyCombox*> m_contactMJ;
    QVector<MyCombox*> m_contactPart;
    QVector<MyCombox*> m_contactFriction;
    //QVector<MyCombox*> m_contactHeatExChange;
	QVector<MyCombox*> m_contactSet;
	QList<InpCurveInpS>    m_nodeCurveList;
	QList<QCurveDataPlot*>  m_cHEC_CurveDataListDlg;//cHEC:contactHeatExChange
	QList<QC_onst*>  m_cHEC_CurveDataListDlg02;
	QList<QAdvancedOptions*>  m_QAdvancedOptionsListDlg;

	//QStringList  m_strListSurf;
	QList<OCIInpS>   m_OutPutContInfS;//用于保存输出信息
	QStringList Surfaceset;

public:
    void SetInpData(ReadInpResultS Data);//加载时写入到界面中
    void WriteToContactInps();//输出到inp文件
public slots:
    void ApplyContactBtnSlot();
    void NextContactBtnSlot();
    void PreContactBtnSlot();
    void AddContactBtnSlot();//增加行
    void DelContactBtnSlot();//删除行
	void contacAOSlot();//AO:QAdvancedOptions
	void contactHeatExChangeSlot();
    void HelpShow();
	void ComboxSlotHighlight(int,int);
	void CurrentValueSlot();
	void AppendSurfSetComboxContactSlot(QString);
	void ShowContact(int);

signals:
    void emitPsetHighLight(NElSurfChsS);
	
};

#endif // QForgingContactDlg_H