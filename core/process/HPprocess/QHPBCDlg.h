#ifndef QHPBCDLG_H
#define QHPBCDLG_H

#include <QDialog>
#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QComboBox>
#include <QHeaderView>
#include "ADefineQ.h"
#include "QMyCombox.h"
#include "CRWManage.h"

enum ENUMBCSET{
    ENUM_BC_XYZ=1,
    ENUM_BC_PSET=2,
};
class QHPBCDlg : public QDialog
{
    Q_OBJECT

public:
    QHPBCDlg(QWidget *parent=NULL);
    ~QHPBCDlg();

public:
    QPushButton *m_ApplyBtn, *m_AddBtn, *m_DelBtn;
    QPushButton *m_NextBCBtn,*m_PreBCBtn;

    QTableWidget* m_TableWiget;
    //QVector<MyCombox*> m_comboxPset;//[20];
    QMap<int, MyCombox*> m_comboxPset;
    //QVector<MyCombox*> m_comboxStyle;
    QMap<int, MyCombox*> m_comboxStyle;//位置,x方向固定，y方向固定，z方向固定;

    QList<BCInpS> m_nodeBCList;
    QStringList m_PSetList;
public:
    void SetInpData(ReadInpResultS Data);//加载时写入到界面中
    void WriteToBcInps();
	void ShowBC(int styleB);
public slots:
    void ApplyBtnSlot();
    void NextBtnSlot();
    void PreBtnSlot();
    void AddBtnSlot();//增加行
    void DelBtnSlot();//删除行
    void comBoxSlot();
    void AppendPSetComboxSlot(QString str);
signals:
    void emitPsetHighLight(NElSurfChsS);
    
};

#endif // QHPBCDLG_H
