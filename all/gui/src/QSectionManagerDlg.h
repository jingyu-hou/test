#ifndef QSECTIONMANAGERDLG_H
#define QSECTIONMANAGERDLG_H

#include <QObject>
#include <QDialog>
#include <QMessageBox>

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QWidget>

#include "ADefineQ.h"
#include "QSectionPropDlg.h"
class QSectionManagerDlg : public QWidget//QDialog
{
    Q_OBJECT

public:
    QSectionManagerDlg(QWidget *parent=NULL);
    ~QSectionManagerDlg();
public:
    QTableWidget* m_TabWiget;
    QPushButton* m_BtnEdit;
    QPushButton* m_BtnCreate;
    QPushButton* m_BtnDel;
    QList<QSectionPropDlg*> m_SectionProListDlg;
    QSectionPropDlg* m_SectionDlg;
    SectionS m_para;

    static QSectionManagerDlg* GetInstance();
private:
    static QSectionManagerDlg* instance;
    void CreatGUI();
public slots:
    int BtnCreateSlot();
    void BtnDelSlot();
    void BtnEditSlot();
   // void SectionManageListSlot(int Row);
    void UpDataListSlot(SectionS para);
    void UpDataMater(SectionS);
};

#endif // QSECTIONMANAGERDLG_H
