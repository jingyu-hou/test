#ifndef QASSEMBLEDLG_H
#define QASSEMBLEDLG_H

#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QComboBox>

class QAssembleDlg : public QWidget
{
      Q_OBJECT
public:
    QAssembleDlg(QWidget *parent = NULL);
    ~QAssembleDlg();
public:
     QTableWidget* m_TableWiget;
public:
     static QAssembleDlg* GetInstance();
private:
     static QAssembleDlg* instance;
    
private:
    
};

#endif // QASSEMBLEDLG_H
