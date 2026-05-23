#ifndef QSECTIONPROP_H
#define QSECTIONPROP_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QGroupBox>
#include <QRadioButton>
#include <QListWidget>
#include <QLabel>
#include <QButtonGroup>
#include "ADefineQ.h"
#include "QSectionDataWig.h"


class QSectionPropDlg : public QDialog
{
	Q_OBJECT
public:
	QSectionPropDlg(QWidget *parent=NULL);
	~QSectionPropDlg();
private:
    QString m_Name;
    void CreatGUI();
public://--控件
    QLineEdit*   m_NameEidt;
    QPushButton* m_BtnOK;
    QPushButton* m_BtnCancel;
    QRadioButton *m_RadioSolid,*m_RadioShell;
    QButtonGroup *m_btnCategory;
    QListWidget *m_ListWidget1,*m_ListWidget2;
    QStringList m_strListWidget1,m_strListWidget2;
     QGroupBox *m_gBox4;
    //--
    SectionS m_para;
    int m_IndexSectionName;
    QSectionDataWig *m_SetionWidData;
public:
    void SetData(SectionS para);
    void SetData(QStringList m_comList);
    void PutData(SectionS &para);
signals:
    void emitSectionParam(SectionS);
public slots:
    void BtnOkSlot();
    void BtnCancelSlot();
    void SectionListSlot1(int Row);
    void SectionListSlot2(int Row);
    void SetCategorySlot(int Index);
};

#endif // QSECTIONPROP_H
