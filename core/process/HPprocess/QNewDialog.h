#ifndef QNEWDIALOG_H
#define QNEWDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QGroupBox>
#include <QMenu>
#include <QApplication>
#include <QClipboard>

#include "qchartviewer.h"
#include "democharts.h"

//--------------------------------------
//--------------  曲线显示 -------------
//--------------------------------------
class QTCurveDataPlot:public QWidget
{   
    Q_OBJECT

public:
    QTCurveDataPlot(QWidget *parent);
    QTCurveDataPlot(QWidget *parent,int iPlotId);
    ~QTCurveDataPlot();

public:
    QPushButton  *m_ApplyBtn;//确定，应用，取消
    QPushButton *m_TabAddBtn,*m_TabDelBtn,*m_TabInsertBtn;//增加，删除，插入
    QTableWidget* m_TabCurveWiget;
    QChartViewer *chartViewers;
    QWidget *m_Hwid1,*m_HTabWid1,*m_HwidCommon;
    QHBoxLayout *m_HLayout3,*m_HLayout2;
    QVBoxLayout *m_MainLayout;
    QMenu *m_popMenu;
    _WELCME_CHART_PROP_ m_ChartProp;
public:
    void ChangeLable(QStringList);
    void Plot();//绘图并排序;
private:
    void ChartShow2XY(StringArray dataX, vector<DoubleArray> dataY, const char *imageMap,_WELCME_CHART_PROP_);
    //bool getSelectedRow(QVector<int*>);
public slots:
    void CurveApplyBtnSlot();
    void CurveAddBtnSlot();
    void CurveInsertBtnSlot();
    void CurveDelBtnSlot();
    void SetData(int nRow, QStringList strData);
    void contextTableMenuSlot(const QPoint &p);
    void TableCopyBtnSlot();
    void TablePasteBtnSlot();
    void TableClearRBtnSlot();//clear;
};
//--------------------------------------
//--------------  空name -------------
//--------------------------------------
class QNewDialog : public QDialog
{
    Q_OBJECT

public:
    QNewDialog(QWidget *parent);
    ~QNewDialog();

    QLineEdit *m_LineEdit;
    QPushButton *m_OKBtn,*m_CancelBtn;

    QString GetEditText();
private:
    
};

typedef struct _ADD_NewPhaseChange_S_{
    int iNum;//strData中数据个数
    QString strChangeP;//分数
    QStringList strData;//
}NewPhaseChangeS;
//--相变积分内曲线
class QAddTab:public QWidget
{
    Q_OBJECT
public:
    QAddTab(QWidget *parent);
    ~QAddTab();
   QTCurveDataPlot *m_TabData;  
   QLineEdit *m_Edit;
};
//--------------------------------------
//--------------  相变积分 -------------
//--------------------------------------
class QChangePointDialog : public QDialog
{
    Q_OBJECT

public:
    QChangePointDialog(QWidget *parent);
    ~QChangePointDialog();

    QLineEdit *m_LineEditName;
    QLineEdit *m_LineEditCurveTh;//第几条曲线
    QLineEdit *m_LineEditChangeP;//分数

    QChartViewer *chartViewers;

    QPushButton *m_OKBtn,*m_CancelBtn,*m_ImportBtn,*m_ExportBtn;
    QPushButton *m_AddTabBtn,*m_DelTabBtn,*m_PlotBtn,*m_AutoReName;
    QGroupBox *m_groupBoxPlot;

    //QMap<int ,QTCurveDataPlot*>m_TCurveDataPlot;//曲线数对应的Tab及图
    QString GetEditText();
    QTabWidget *m_tabView;
    QMap<int ,QAddTab*> m_addNewTab;
    
    QString GetPlotData(int );//绘图中的数据;格式: dataX1,dataY1; dataX2,dataY2; dataX3,dataY3;
  
    int GetDataNum(int );//得到数据的点数
    int GetCurveNum();//得到曲线总数；
    QString GetCurveThPoint(int );//获取当前Tab1下的第几条曲线及转变分数
    //bool setInpData(int);//几个Tab,每个Tab的积分数，数据项；
    //bool setData(QString strChangeP, QStringList strData);
    bool setData(QMap<int,NewPhaseChangeS> MapData);
private:
    _WELCME_CHART_PROP_ m_ChartProp;
    void ChartShow2XY(vector<DoubleArray> dataX, vector<DoubleArray> dataY, const char *imageMap,_WELCME_CHART_PROP_);
private slots:
    void OkBtnSlot();
    void AddTabBtnSlot();
    void PlotBtnSlot();
    void DelTabBtnSlot();
    void AutoRenameSlot();
};

//--------------------------------------
//---------------  相平衡 --------------
//--------------------------------------
class QPhaseBalanceDialog : public QDialog
{
    Q_OBJECT

public:
    QPhaseBalanceDialog(QWidget *parent);
    ~QPhaseBalanceDialog();

  
    QPushButton *m_OKBtn,*m_CancelBtn;

    QTCurveDataPlot* m_TCurveDataPlot;
    int m_iCurveNum;//曲线点数
    int GetDataNum();//得到数据的点数
    QString GetPlotData();//绘图中的数据;格式: dataX1,dataY1; dataX2,dataY2; dataX3,dataY3;
private:
private slots:
    void OkBtnSlot();
};
//--------------------------------------
//----------------  潜热 ---------------
//--------------------------------------
class QPhaseLatentHeatDialog : public QDialog
{
    Q_OBJECT

public:
    QPhaseLatentHeatDialog(QWidget *parent);
    ~QPhaseLatentHeatDialog();

    QLineEdit *m_LineName,*m_LineValue;//名字（相）、潜热值
    QPushButton *m_OKBtn,*m_CancelBtn;
    QString GetEditText();

private:
private slots:
    void OkBtnSlot();
};
//--------------------------------------
//---------------比热、导热-------------QPhasePropDlg
//--------------------------------------
class QPhasePropDialog : public QDialog
{
    Q_OBJECT

public:
    QPhasePropDialog(QWidget *parent);
    ~QPhasePropDialog();

    QPushButton *m_OKBtn,*m_CancelBtn;
    QTCurveDataPlot* m_TCurveDataPlot;
   
    int GetDataNum();//得到数据的点数
    QString GetPlotData();//绘图中的数据;格式: dataX1,dataY1; dataX2,dataY2; dataX3,dataY3;
private:
private slots:
    void OkBtnSlot();
};
//------------------------------------------------------
//----------------  孕育期INCUBATIONPERIOD -------------
//------------------------------------------------------
class QIncubationPeriod : public QDialog
{
    Q_OBJECT

public:
    QIncubationPeriod(QWidget *parent);
    ~QIncubationPeriod();

    QPushButton *m_OKBtn,*m_CancelBtn;
    QTCurveDataPlot* m_TCurveDataPlot;
    int GetDataNum();//得到数据的点数
    QString GetPlotData();//绘图中的数据;格式: dataX1,dataY1; dataX2,dataY2; dataX3,dataY3;
private:
    private slots:
        void OkBtnSlot();
};

//--------------------------------------
//----------------  冷速 ---------------
//--------------------------------------
class QPhaseZBFDialog : public QDialog
{
    Q_OBJECT

public:
    QPhaseZBFDialog(QWidget *parent);
    ~QPhaseZBFDialog();

    QLineEdit *m_LineEditName;//相序
    QLineEdit *m_LineEditCurveTh;//第几条曲线（第几个冷速，冷速由低到高）
    QLineEdit *m_LineEditChangeP;//冷速
    QPushButton *m_OKBtn,*m_CancelBtn;

    QTCurveDataPlot* m_TCurveDataPlot;
    QString GetEditText();

    int GetDataNum();//得到数据的点数
    QString GetPlotData();//绘图中的数据;格式: dataX1,dataY1; dataX2,dataY2; dataX3,dataY3;
private:
    private slots:
        void OkBtnSlot();
};


#endif // QNEWDIALOG_H
