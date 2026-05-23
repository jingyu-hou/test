#ifndef QHPINITDLG_H
#define QHPINITDLG_H

#include <QDialog>
#include <QFileDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QGroupBox>
#include <QLineEdit>
#include <QLabel>
#include <QHeaderView>
#include "QMyCombox.h"
#include "ADefineQ.h"
#include "CRWManage.h"
class QHPInitDlg : public QDialog
{
    Q_OBJECT

public:
    QHPInitDlg(QWidget *parent=NULL);
    ~QHPInitDlg();
	QLineEdit *FileName;

public:
    QPushButton *m_ApplyBtn, *m_AddBtn, *m_DelBtn,*m_CancelBtn;
    QPushButton *m_NextInitBtn,*m_PreInitBtn;

    QTableWidget* m_TableWiget;
    QMap<int, MyCombox*> m_comboxPset;
public:
    QList<InitInpS>   m_nodeInitList;
public:
    void SetInpData(ReadInpResultS Data);//加载时写入到界面中
    void WriteToInps();//从界面写入到INP中;
    QStringList m_PSetList;//从inp获取的PSetList集合名称;
public slots:
        void ApplyBtnSlot();
        void AddBtnSlot();
        void DelBtnSlot();
        void NextBtnSlot();
        void PreBtnSlot();
        void comBoxChgSlot();
		void LoadEFSlot();
		void AppendPSetComboxIniSlot(QString str);
signals:
        void emitPsetHighLight(NElSurfChsS);

};

#endif // QINITDLG_H
