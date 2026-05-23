#ifndef QHPCCTDLG_H
#define QHPCCTDLG_H

#include <QDialog>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QCheckBox>
#include <QGroupBox>
#include "QMyCombox.h"
#include "QNewDialog.h"
#include <QMenu>
#include <QMessageBox>
#include "ADefineQ.h"

//enum _COMBOX_CCTMENUID_{
//    COL_CCT_CHANGEPOINT = 1,
//    COL_CCT_LENGSU=2,
//    COL_CCT_PHASEBALANCE = 4,
//    COL_CCT_YUNYUQI = 5,
//    COL_CCT_DAORE=6,
//    COL_CCT_BIRE=7,
//    COL_CCT_QIANRE=8,
//};
enum _COMBOX_CCTMENUID_{
    COL_CCT_CHANGEPOINT = 0,
    COL_CCT_LENGSU=1,
    COL_CCT_PHASEBALANCE = 3,
    COL_CCT_YUNYUQI = 4,
    COL_CCT_DAORE=5,
    COL_CCT_BIRE=6,
    COL_CCT_QIANRE=7,
    COL_CCT_HARD=8,
};
class QHPCCTDlg : public QDialog
{
    Q_OBJECT

public:
    QHPCCTDlg(QWidget *parent=NULL);
    ~QHPCCTDlg();
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
    QCheckBox *m_CheckBoxCCTemptCal;//cct等温计算


    QLineEdit *m_EditTab1P1,*m_EditTab1P2,*m_EditTab1P3;
    QLineEdit *m_EditTab2P11,*m_EditTab2P12,*m_EditTab2P13,
        *m_EditTab2P21,*m_EditTab2P22,*m_EditTab2P23,
        *m_EditTab2P31,*m_EditTab2P32,*m_EditTab2P33,
        *m_EditTab2P41,*m_EditTab2P42,*m_EditTab2P43;

    QGroupBox *m_gbBox,*m_gbBoxTab2;
    //---
    QPushButton *m_ImageFormula;


    QVector <MyCombox*> m_ComboxChangePoint;//转变分数
    QVector <MyCombox*> m_ComboxLensu;//冷速
    QVector <MyCombox*> m_ComboxPhaseBalance;//相位平衡
    QVector <MyCombox*> m_ComboxYunYuQi;//孕育期
    QVector <MyCombox*> m_ComboxDaoRe;//导热
    QVector <MyCombox*> m_ComboxBiRe;//比热
    QVector <MyCombox*> m_ComboxQianRe;//潜热

    //--增、删、插入
    QPushButton *m_AddLineBtn,*m_DelLineBtn,*m_InsertLineBtn;
    QPushButton *m_OkBtn;//确定

    QTableWidget *m_TableWidget;
    static QHPCCTDlg* GetInstance(QWidget *parent);
    QMap<QString ,QChangePointDialog*>m_changePointDlg;
    QMap<QString ,QPhaseBalanceDialog*>m_phaseBalanceDlg;
    QMap<QString ,QPhaseLatentHeatDialog*>m_phaseLatentHeatDlg;
    QMap<QString ,QPhasePropDialog*>m_PhasePropCondDlg;//导热
    QMap<QString ,QPhasePropDialog*>m_PhasePropSpecDlg;//比热
    QMap<QString ,QIncubationPeriod*>m_IncubationPeriodDlg;//孕育
    QMap<QString ,QPhaseZBFDialog*>m_PhaseZBFDlg;//冷速
private:
    static QHPCCTDlg* instance;
    int m_CurTabRow,m_CurTabCol,m_ComboxCurId;
private:
    void CreatGUI();
    void CCTAddLine(int nRow);
    bool AddExistComboxText(MyCombox *ff, int curCol);
    void SetTableComboxEnable(int curCol,bool bEnable);
public:
    HPTTTInps WriteHPCCToInp();
private slots:
    void GestationSlot();
    void CrystalSizeSlot();
    void QianReSlot();
    void ReWuXingSlot();
    void ContextDelSlot();
    void comBoxRYunYuSlot(int RowId,int CurrentId);
    void comBoxRChangePointSlot(int,int);
    void comBoxRPhaseSlot(int,int);
    void comBoxRDaoReSlot(int RowId,int CurrentId);
    void comBoxRBiReSlot(int RowId,int CurrentId);
    void comBoxRQianReSlot(int RowId,int CurrentId);
    void comBoxRLengSuSlot(int RowId,int CurrentId);
    void comBoxNewChangePointSlot(int RowId,int CurrentId);
    void comBoxNewPhaseSlot(int RowId,int CurrentId);
    void comBoxNewYunYuSlot(int RowId,int CurrentId);
    void comBoxNewDaoReSlot(int RowId,int CurrentId);
    void comBoxNewBiReSlot(int RowId,int CurrentId);
    void comBoxNewQianReSlot(int RowId,int CurrentId);
    void comBoxNewLengSuSlot(int RowId,int CurrentId);
    void InsertLineBtnSlot();
    void AddLineBtnSlot();
    void DelLineBtnSlot();
    void OkBtnSlot();

};

#endif // QHPCCTDLG_H
