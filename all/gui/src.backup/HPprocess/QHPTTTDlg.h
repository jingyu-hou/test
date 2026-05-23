#ifndef QHPTTTDLG_H
#define QHPTTTDLG_H

#include <QDialog>
#include <QFileDialog>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QCheckBox>
#include <QGroupBox>
#include <QtXml/QtXml>
#include <QtXml/QDomComment>
#include "QMyCombox.h"
#include "QNewDialog.h"
#include "ADefineQ.h"
#include "CRWManage.h"
#include "QMyTableWidget.h"


//enum _COMBOX_MENUID_{
//    COL_CHANGEPOINT = 1,
//    COL_PHASEBALANCE = 3,
//    COL_YUNYUQI = 4,
//    COL_DAORE=5,
//    COL_BIRE=6,
//    COL_QIANRE=7,
//};
enum _COMBOX_MENUID_{
    COL_CHANGEPOINT = 0,
    COL_INIT=1,
    COL_PHASEBALANCE = 2,
    COL_YUNYUQI = 3,
    COL_DAORE=4,
    COL_BIRE=5,
    COL_QIANRE=6,
    COL_HARD=7,
};
//typedef struct _TTTDLG_PhaseChange_S_{
//    QString strChangeP;//分数
//    QStringList strData;//
//}PhaseChangeTabS; 
typedef NewPhaseChangeS PhaseChangeTabS;
typedef struct _TTTDLG_BASEITEM_S_{
    int iNum;//数据点数 --eg:2,9(相序，总点数)中的9
    QStringList strData;//数据项
}PhaseBaseItem;

typedef struct _Inp_S_{
    int  iPhaseNum;//总的相序数
    QMap<int,QMap<int,PhaseChangeTabS> > PhaseCurve;//相变积分
    QMap<int,QString> strInitVal;//初始值
    QMap<int,QString> strLatenTheat;//潜热
    QMap<int,QString> strHard;//硬度
    QMap<int,PhaseBaseItem> PhaseIP;//孕育期
    QMap<int,PhaseBaseItem> PhaseBalance;//相平衡
    QMap<int,PhaseBaseItem> PhasePropSpec;//比热
    QMap<int,PhaseBaseItem> PhasePropCond;//导热
    QString strYS;//屈服强度
    QString strGS;//晶粒尺寸
    QString strPhaseCtrol;//热处理方式
}TTTSetDataS;
class QHPTTTDlg : public QDialog
{
    Q_OBJECT

public:
    QHPTTTDlg(QWidget *parent=NULL);
    ~QHPTTTDlg();
    QTabWidget* m_tabView;
    QWidget *m_Tab1,*m_Tab2,*m_Tab3;
//---Tab1
    QCheckBox *m_CheckBoxGestation;//考虑孕育期间
    QCheckBox *m_CheckBoxCrystalSize;//母子晶粒尺寸
    QCheckBox *m_CheckBoxCalculate;//计算孕育

//--Tab2//屈服强度
    QCheckBox *m_CheckBoxQufuQiangDu;
//--Tab3//其他
    QCheckBox *m_CheckBoxQianRe;//
    QCheckBox *m_CheckBoxReWuXing;//
    QCheckBox *m_CheckBoxCalcChg;//计算变形

    QLineEdit *m_EditTab1P1,*m_EditTab1P2,*m_EditTab1P3;
    QLineEdit *m_EditTab2P11,*m_EditTab2P12,*m_EditTab2P13,
              *m_EditTab2P21,*m_EditTab2P22,*m_EditTab2P23,
              *m_EditTab2P31,*m_EditTab2P32,*m_EditTab2P33,
              *m_EditTab2P41,*m_EditTab2P42,*m_EditTab2P43;

    QGroupBox *m_gbBox,*m_gbBoxTab2;
//---
    QPushButton *m_ImageFormula;
    
    QMap <int , QPushButton*> m_SetBtnChangePoint;//转变分数
    QMap <int , QPushButton*> m_SetBtnPhaseBalance;//相位平衡
    QMap <int , QPushButton*> m_SetBtnYunYuQi;//孕育期
    QMap <int , QPushButton*> m_SetBtnDaoRe;//导热
    QMap <int , QPushButton*> m_SetBtnBiRe;//比热

//--增、删、插入
    QPushButton *m_AddLineBtn,*m_DelLineBtn,*m_InsertLineBtn,*m_ImportLineBtn,*m_ExportLineBtn;//luo
    QPushButton *m_OkBtn;//确定

   //QTableWidget *m_TableWidget;
     QMyTableWidget *m_TableWidget;
    static QHPTTTDlg* GetInstance(QWidget *parent);
	static QHPTTTDlg* GetInstance2();
    QMap<int ,QChangePointDialog*>m_changePointDlg;
    QMap<int ,QPhaseBalanceDialog*>m_phaseBalanceDlg;
    QMap<int ,QPhaseLatentHeatDialog*>m_phaseLatentHeatDlg;
    QMap<int ,QPhasePropDialog*>m_PhasePropCondDlg;//导热
    QMap<int ,QPhasePropDialog*>m_PhasePropSpecDlg;//比热
    QMap<int ,QIncubationPeriod*>m_IncubationPeriodDlg;//孕育

    //--
    //HPTTTCCTInps m_TTTInps;
public:
QString m_NowFileName;
private:
   static QHPTTTDlg* instance;
   static QHPTTTDlg* instance2;
 
   int m_CurTabRow,m_CurTabCol,m_ComboxCurId;
public:
   HPTTTInps WriteHPTTTToInp();
   //void SetInpData(ReadInpResultS ReadInpData);
    void SetInpData(TTTSetDataS Data);
	TTTSetDataS ParseEntry(QXmlStreamReader &reader);
	void ImportInpTTT(ReadInpResultS TTTMaterial);//
	int chgTheNameToInt(QString strRef);
private:
   void CreatGUI();
   void TTTAddLine(int);
   bool AddExistComboxText(MyCombox *ff, int curCol);
   void SetTableComboxEnable(int curCol,bool bEnable);

private slots:
   void GestationSlot();
   void CrystalSizeSlot();
   void QianReSlot();
   void ReWuXingSlot();
   void InsertLineBtnSlot();
   void AddLineBtnSlot();
   void DelLineBtnSlot();
   void OkBtnSlot();
   void SetBtnSLot();
   void CalYunyuSlot();
   void ImportLineBtnSlot();
   void ExportLineBtnSlot();
public slots:
   QMap<int,PhaseBaseItem> ProcessMapData(QList<HPphaseBalanceInps> BalanceList);

};

#endif // QHPTTTDLG_H
