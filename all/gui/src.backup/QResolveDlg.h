#ifndef QRESOLVEDLG_H
#define QRESOLVEDLG_H

#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QComboBox>

#include "ADefineQ.h"
#include "CRWManage.h"

class QResolveDlg : public QDialog
{
    Q_OBJECT

public:
    QResolveDlg(QWidget *parent=NULL);
    ~QResolveDlg();

public:
    QPushButton *m_OKBtn,*m_CancelBtn,*m_NextBtn,*m_PreBtn;
    QTableWidget  *m_TableWig ;

    QList<HIPSolveInpS> m_nodeHIPSolveList;//
    QComboBox *m_combox;

public:
    void SetInpData(ReadInpResultS Data);//加载时写入到界面中
    void WriteSloveToInp();
public slots:
    void NextBtnSlot();
    void ApplayBtnSlot();

private:
    
};
#endif // QINITDLG_H
