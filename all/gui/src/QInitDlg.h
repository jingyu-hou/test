#ifndef QINITDLG_H
#define QINITDLG_H

#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "QMyCombox.h"
#include "ADefineQ.h"
#include "CRWManage.h"

class QInitDlg : public QDialog
{
    Q_OBJECT

public:
    QInitDlg(QWidget *parent=NULL);
    ~QInitDlg();

public:
    QPushButton *m_ApplyBtn, *m_AddBtn, *m_DelBtn,*m_CancelBtn;

    QTableWidget* m_TableWiget;
    QVector<MyCombox*> m_comboxPset;

public:
    QList<InitInpS>m_nodeInitList;
	QStringList m_PSetList;
public:
   void SetInpData(ReadInpResultS Data);//加载时写入到界面中
   void WriteToInps();//从界面写入到INP中;
public slots:
     void ApplyBtnSlot();
     void AddBtnSlot();
     void CancelBtnSlot();
     void DelBtnSlot();
     void comBoxChgSlot(int,int);
     void comBoxDoubleSlot(int, int);
	 void AppendPSetComboxIniSlot(QString str);
signals:
     void emitPsetHighLight(NElSurfChsS);
};

#endif // QINITDLG_H
