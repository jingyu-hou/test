#ifndef QMATERMANAGEDLG_H
#define QMATERMANAGEDLG_H

#include <QObject>
#include "QMaterPropDlg.h"
#include <QMessageBox>
#include "QHPTTTDlg.h"


class QMaterManagerDlg : public QWidget//QDialog
{
    Q_OBJECT

public:
    QMaterManagerDlg(QWidget *parent=NULL);
    ~QMaterManagerDlg();
public:
    QListWidget* m_ListWiget;
    QPushButton* m_BtnEdit;
    QPushButton* m_BtnCreate;
    QPushButton* m_BtnDel;
    QList<QMaterPropDlg*> m_MaterProListDlg;
    QMaterPropDlg* m_MaterProDlg;
	QHPTTTDlg *m_TTTDlg;
    MaterialS m_para;

    static QMaterManagerDlg* GetInstance();
private:
    static QMaterManagerDlg* instance;
    void CreatGUI();
    void init();
public:
   bool EditMaterial(int index);
public slots:
    int BtnCreateSlot();
    void BtnDelSlot();
    void BtnEditSlot();
    void MaterManageListSlot(int Row);
    void UpDataListSlot(SectionS para);
signals:
   void emitUpMaterListData(SectionS);

};

#endif // QMATERMANAGEDLG_H
