#ifndef QVARIABLEOUTPUT_H
#define QVARIABLEOUTPUT_H

#include <QDialog>

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QStringList>
#include "ADefineQ.h"
#include "CRWManage.h"

class QVariableOutputDlg : public QDialog
{
    Q_OBJECT

public:
    QVariableOutputDlg(QWidget *parent);
    ~QVariableOutputDlg();
   
public:
    QPushButton *m_OKBtn,*m_NextBtn,*m_CancelBtn,*m_DefaultBtn;
    QCheckBox  *m_CheckBox[10],*m_AllCheckBox,*m_OutputDatCheckBox;
    QComboBox *m_Combox;//,*m_ComboxTime;
    QLineEdit *m_TimeEdit;//时间编辑框
    QLabel *m_timLabel,*m_freqLabel;//时间、频率
    QStringList m_TimeFreqStr;
    
    QList<VariableInpS> m_nodeVarList;//变量 
public:
    void SetInpData(ReadInpResultS Data);//加载时写入到界面中  
    void WriteVarToInp();
private slots:
    void AllChooseSlot();
    void ChangeTimeFreqSlot(int Index);
    void OKBtnSlot();
private:
    
};

#endif // QVARIABLEOUTPUT_H
