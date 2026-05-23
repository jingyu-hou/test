#ifndef QSECTIONDATAWIG_H
#define QSECTIONDATAWIG_H

#include <QWidget>
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
#include <QComboBox>
#include "QMaterPropDlg.h"
#include "QMaterManagerDlg.h"

class QSectionDataWig : public QWidget
{
    Q_OBJECT

public:
    QSectionDataWig(QWidget *parent);
    ~QSectionDataWig();

public:
    QComboBox  *m_Combox;
    QPushButton *m_BtnMaterCreate;
    QStringList m_comList;
    int m_IndexWig;

    SectionS m_para;
    QMaterPropDlg* m_MaterProDlg; 
    //QList<QMaterPropDlg*> m_MaterProDlg; 
    QMaterManagerDlg *m_MaterManagerDlg;
public:
    void SetData(SectionS para);
    void CreatGUI(int index);

private:
public slots:  
    void BtnMaterCreateSlot();
};

#endif // QSECTIONDATAWIG_H
