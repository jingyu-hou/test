#ifndef QHPSOLVESETDLG_H
#define QHPSOLVESETDLG_H

#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QTabWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QComboBox>
#include <QGroupBox>
#include <QButtonGroup>
#include "ADefineQ.h"
#include "CRWManage.h"


class QHPSolveSetDlg : public QDialog
{
    Q_OBJECT

public:
    QHPSolveSetDlg(QWidget *parent=NULL);
    ~QHPSolveSetDlg();
    
private:
	int IJKL;
public:
    QWidget *m_Tab1,*m_Tab2;
    QTabWidget *m_tabView;
    QCheckBox *m_checkBoxHostStress;//主应力
    QCheckBox *m_checkBoxNodeTemper;//节点温度
    QCheckBox *m_checkBoxCrystalSize;//晶粒尺寸
    QCheckBox *m_checkBoxEquiStress;//等效应力
    QCheckBox *m_checkBoxTemperGrads;//温度梯度
    QCheckBox *m_checkBoxBendIntensity;//屈服强度
    QCheckBox *m_checkBoxPlasticStrain;//塑性应变
    QCheckBox *m_checkBoxHeatFlux;//热流密度
    QCheckBox *m_checkBoxVOF;//体积分数
    QCheckBox *m_checkBoxEquiStrain;//等效应变

    QCheckBox *m_checkBoxTemperMaxVar;//温度最大变化量

    QLineEdit *m_EditAbsZero,*m_NodeSetName;//绝对零度,节点集合名称
    QLineEdit *m_EditStefanBoltzman,*MassScalingFactor;//斯忒藩-玻尔兹曼常数
	QPushButton *m_AddBtn,*m_DeletBtn;

    QPushButton *m_NextBtn,*m_PreBtn;
    //--
    QCheckBox  *m_CheckBox[10],*m_AllCheckBox,*m_OutputDatCheckBox,*m_ExternalForcesCheckBox;
    QComboBox *m_Combox,*m_DatCombox;//,*m_ComboxTime;
    QLineEdit *m_TimeEdit;//时间编辑框
    QLabel *m_timLabel,*m_freqLabel;//时间、频率
    QStringList m_TimeFreqStr;
	QStringList m_PSetList;

    QList<VariableInpS> m_nodeVarList;//变量
    QTableWidget  *m_TableWig;
    QComboBox *m_comboxCoupled;
   
    QList<HPSolveInpS> m_nodeHPSolveList;//
    QList<HPPhyInpS> m_nodeHPPhyList;//
	QList<QString>NodeSetNameList;
private:
    void CreatGUI();
public:
    void SetInpData(ReadInpResultS Data);//加载时写入到界面中
    void WriteSloveToInp();
    void SetSloveStep(int istep);
    void SetOutPutTab(int);
public slots:
    void AllChooseSlot();
    void ChangeTimeFreqSlot(int Index);
    void ApplayBtnSlot();
    void PreBtnSlot();
	void ShowAndHideSlot(int id);
	void AppendPSetComboxSolvesetSlot(QString str); 
	void AddDeletSolt(int);
	void MCFSlot(int);
};

#endif // QHPSOLVESETDLG_H
