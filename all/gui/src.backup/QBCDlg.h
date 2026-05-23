#ifndef QBCDLG_H
#define QBCDLG_H

#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QComboBox>
#include "ADefineQ.h"
#include "QMyCombox.h"
#include "CRWManage.h"

class QBCDlg : public QDialog
{
    Q_OBJECT

public:
    QBCDlg(QWidget *parent=NULL);
    ~QBCDlg();

public:
    QPushButton *m_ApplyBtn, *m_AddBtn, *m_DelBtn;

    QTableWidget* m_TableWiget;
    QVector<MyCombox*> m_comboxPset;//[20];
    QVector<MyCombox*> m_comboxStyle;//位置,x方向固定，y方向固定，z方向固定;

    QList<BCInpS> m_nodeBCList;
public:
    void SetInpData(ReadInpResultS Data);//加载时写入到界面中
    void WriteToBcInps();
private:

public slots:
    void ApplyBtnSlot();
    void AddBtnSlot();//增加行
    void DelBtnSlot();//删除行
    void comBoxSlot(int comWigId,int index);
signals:
    void emitPsetHighLight(NElSurfChsS);

};

#endif // QBCDLG_H
