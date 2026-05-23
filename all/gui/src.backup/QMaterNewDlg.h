#ifndef QMATERNEWDLG_H
#define QMATERNEWDLG_H

#include <QListWidget>
#include <QComboBox>
#include <QDialog>
#include <QGroupBox>
#include <QCheckBox>
#include <QMenu>
#include <QMenuBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVector>
#include <QTableWidget>
#include <QMenu>
#include <QApplication>
#include <QClipboard>
#include "ADefineQ.h"
#include "qchartviewer.h"
#include "democharts.h"
#include "QHPTTTDlg.h"
//------
//------
//------
#include "QMaterDataWig.h"
#define TYPENUM 10


//typedef struct _S_MCELL_S_ 
//{
//    QString strName;
//    QString Data;
//    _S_MCELL_S_()
//    {
//        strName="";
//        Data="";
//    }
//}MCell,*pMCell;
//
//typedef struct _S_MATERCOMPS_S_
//{
//    int id;
//    QString FileName;//文件名字
//    QString strStyle;//"density,elastic..."
//    int dataColNum;//数据列个数
//    int dataRowNum;//数据行个数
//    int dataColNum2;//分了Tab的数据，第二页的数据列
//    int dataRowNum2;
//    int PageIndex; //页面
//    vector<MCell> data;//数据"mass density,Temp"
//    bool bTemper;
//    _S_MATERCOMPS_S_()
//    {
//        dataColNum=0;
//        dataRowNum=0;
//        PageIndex=0;
//        dataColNum2=0;
//        dataRowNum2=0;
//        bTemper = false;
//    }
//}MCompoment,*pMCompoment;


class QTableCurveDataPlot:public QWidget
{
    Q_OBJECT
public:
    QTableCurveDataPlot(QWidget *parent,int type);
    ~QTableCurveDataPlot();
 
public:
	QGridLayout *mailayout;
    //QVector<QTableWidget*> m_TableData;
    QTableWidget *m_TableData;
    QTableWidget *m_TableData1;//金属粉末参数
	QTableWidget *m_TableData2;
    QChartViewer *m_chartViewers;
    QTabWidget *m_TabWidget;
    QLineEdit *m_editZero,*PlasticWorkValue;
    QMenu *m_popMenu;
    QLabel *m_LabRfTemperate;
	QLabel *m_Rate_Dependent;
	QLabel *m_Damage;
	QComboBox *N_Rate_Dependent,*N_Damage;
	QCheckBox *PlasticWorkCheck;
public:
    QPushButton *m_AddBtn,*m_DelBtn,*m_InsertBtn;
    QPushButton *m_PlotBtn;//绘图
    void LanguageUpData();
    void SetTableCurveData(MCompoment data);
    void Plot();
    void ChartShow2XY(StringArray dataX, vector<DoubleArray> dataY, const char *imageMap);
private:
    void TableDataPaste();
    void TableData1Paste();
    void TableDataClear();
    void TableData1Clear();
    void TableDataCopy();
    void TableData1Copy();

public slots:
    void AddLineBtnSlot();
    void DelLineBtnSlot();
    void InsertLineBtnSlot();
    void PlotBtnSlot();
    void contextTableMenuSlot(const QPoint &p);
    void TableCopyRBtnSlot();
    void TablePasteRBtnSlot();
    void TableClearRBtnSlot();//clear all data;
	void RateDependentSlot01(int);
	void DamageSlot01(int);
	void PlasticWorkValueSlot();
};

//------
//------
//------
class QMaterNewDlg: public QDialog
{
    Q_OBJECT
public:
    QMaterNewDlg(QWidget *parent=NULL);
    ~QMaterNewDlg();

public:
	QHPTTTDlg  *m_TTTDlg;
  //--Action & Menu
    QAction *m_DensityAct, *m_ElasticAct, *m_PlasticAct, *m_ExpansionAct,* m_ConductivityAct,*m_RateDependent;
    QAction *m_SpecificHeatAct,*m_MatelPowderAct,*m_Damage,*m_DynamicRecrystallization,*m_TTTAct;
    QMenu *m_GeneralMenu, *m_MechanicalMenu,*m_TermalMenu,*m_MaterMenu;
    QMenu *m_subMeEMenu,*m_subMePMenu;
    QMenu *m_MatelPowderMenu;//金属粉末
    QMenuBar* m_BarMenu;
    QListWidget *m_ListWiget;
    QPushButton* m_BtnDelList;//删除


    QPushButton *m_CancelBtn;
    QPushButton *m_OkBtn;
    QLineEdit *m_NameEdit;
    QLabel *m_LName;

    QGroupBox *m_gbCurGroup; 

    
    QVector<QTableCurveDataPlot*> m_TabCurDataWidget;
    QMap<int,MCompoment> Mdata;
    QMap<QString,QMap<int,MCompoment> > MFileData;

    QStringList m_strList;
    void creatGUI();
    void InitTabCurveGUI();
    void ListShowSet(int index);
    void setWidgetVisible(int type);
    bool TabDataToSave();//保存界面中的所有参数
    void setNewData();//写入数据；

	void UpDataMaterList(QMap<int, MCompoment> Mdata);//更新List中的数据
    void LanguageUpData();
 public slots:
    void DensitySlot();
    void ElasticSlot();
    void PlasticSlot();
	void RateDependentSlot();
	void DamageSlot();
	void DynamicRecrystallizationSlot();
    void ExpansionSlot();
    void ConductSlot();
    void SpecificSlot();
    void MatelPowderSlot();
	void TTTSlot();

    void MaterListSlot(int);
    void OkBtnSlot();
    void CancelBtnSlot();
    void DelMaterialSlot();

};



#endif // QMATERNEWDLG_H
