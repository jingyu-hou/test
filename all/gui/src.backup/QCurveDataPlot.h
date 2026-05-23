#ifndef QCurveDataPlot_H
#define QCurveDataPlot_H

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
#include <QFileDialog>
//#include <QFile>
#include <QMenu>
#include <QApplication>
#include <QClipboard>
#include "qchartviewer.h"
#include "democharts.h"
#include "ADefineQ.h"
#include "CRWManage.h"


class QCurveDataPlot:public QDialog
{   
    Q_OBJECT

public:
    QCurveDataPlot(QWidget *parent);
    ~QCurveDataPlot();

public:
    QPushButton *m_OKBtn, *m_CancelBtn,*m_ImportBtn,*m_ExportBtn;//确定，应用，取消
    QLineEdit *m_CurveName;

    QPushButton *m_TabAddBtn,*m_TabDelBtn,*m_TabInsertBtn,*m_TabPlotBtn;//增加，删除，插入
    QTableWidget* m_TabCurveWiget;
    QChartViewer *chartViewers;
    InpCurveInpS m_HIPCurveNode;
    QMenu *m_popMenu;
public:
            
    void SetData(InpCurveInpS m_HIPCurveNode);//设置表格及内容
    void PutData();//读取表格及内容
    void writeToCurveInpData();

private:
    void Plot();
    void ChartShow2XY(StringArray dataX, vector<DoubleArray> dataY, const char *imageMap);
    void flowLayoutCharts();
public slots:
    void CurveApplyBtnSlot();
    void CurveOkBtnSlot();
	void CurveCanleBtnSlot();
    void CurveAddBtnSlot();
    void CurveInsertBtnSlot();
    void CurveDelBtnSlot();
    void contextTableMenuSlot(const QPoint &p);
    void TableCopyBtnSlot();
    void TablePasteBtnSlot();
    void TableClearRBtnSlot();//clear ;
	void ImportSlot();
	void ExportSlot();

};

#endif // QCurveDataPlot_H
