#ifndef QCFGFILEMANAGE_H
#define QCFGFILEMANAGE_H

#include <QObject>

#include "QPostWigReadResultDlg.h"
#include "QPostWidPlotOptDlg.h"


class QCfgFileManage: public QWidget
{ 
    Q_OBJECT
public:
    QCfgFileManage(QWidget *parent=NULL);
    ~QCfgFileManage();
    bool LoadConfigFile();
    bool LoadOptionFile(QString fileName, QString NameStart);
    void ParseEntry(QXmlStreamReader &reader);
    void ParseEntryVTKText(QXmlStreamReader &reader);
    int chgTheNameToInt(QStringRef strRef);
    OptDlgS m_optDlgs;//
    ReadResultDlgS m_readRltDlgs;//
private:
    
};

#endif // QCFGFILEMANAGE_H
