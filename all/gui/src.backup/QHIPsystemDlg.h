#ifndef QHIPSYSTERMDLG_H
#define QHIPSYSTERMDLG_H

#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QComboBox>
#include <QMyCombox.h>
#include <QLineEdit>
#include <QLabel>
#include <QMenu>
#include <QApplication>
#include <QClipboard>
#include <QHeaderView>
#include "qchartviewer.h"
#include "democharts.h"
#include "ADefineQ.h"
#include "CRWManage.h"
#include "QCurveDataPlot.h"

class QHIPsystemDlg : public QDialog
{
    Q_OBJECT

public:
    QHIPsystemDlg(QWidget *parent);
    ~QHIPsystemDlg();

public:
    QPushButton *m_NextHipSysBtn, *m_PreHipBtn, *m_DelBtn;
    QTableWidget* m_TableWiget;

    //QVector<MyCombox*> m_comboxCurveNew;
    //QVector<MyCombox*> m_comboxPSetSurf1;
    QMap<int,MyCombox*> m_comboxCurveNew;
    QMap<int,MyCombox*> m_comboxPSetSurf1;
    //QList<QCurveDataPlot*> m_CurveDataListDlg;
    QMap<QString,QCurveDataPlot*> m_CurveDataDlg;
public:
    QList<HIPSystemInpS>   m_nodeHIPSystemList;//
    QList<InpCurveInpS>    m_nodeCurveList;
public:
    void SetInpData(ReadInpResultS Data);//加载时写入到界面中
    void WriteHIPsysToInp();//将界面上的写入到inp
    
public slots:
    void comBoxSlot();
	void PreHipSysBtnSlot();
	void NextHipSysBtnSlot();
    void updateHIPCurveDataSlot(QStringList,InpCurveInpS CurveData);
    void NewCurve(int Row, int index);
    //from tree
    void updataFromTreeSlot(QString strName);
    void DelCurve(QString strName);
    void AppendSurfSetComboxSlot(QString str);
signals:
    void emitPsetHighLight(NElSurfChsS);
    void emitHIPCurveData(QStringList,InpCurveInpS);//更新整个curve数据
    void emitUpDataCurveTreeName(QString,QStringList );//编辑完成后更新当前曲线名字
 

private:
    
};




#endif // QHIPSYSTERMDLG_H
