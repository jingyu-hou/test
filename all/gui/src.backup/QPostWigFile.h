#ifndef QPOSTWIGFILE_H
#define QPOSTWIGFILE_H
#include <QWidget>
#include <QObject>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QFile>
#include <QFileDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include "QMyVTK.h"
#include "ADefineQ.h"
#include "QFrdDataPro.h"
#include "QFrdActorWidge.h"
#include "./PostProcess/FrdDataVIS.h"
#include "./PostProcess/TextVIS.h"
class QPostWigFile : public QWidget
{
	Q_OBJECT

public:
    QPostWigFile(QWidget *parent=NULL, VTKColorS *Clr =NULL);
	~QPostWigFile();
public:
    void PostFrdNew(int index);
    void LanguageUpData();

       QFrdActorWidge *m_ListActorWiget;
private:
	QPushButton *m_ReadBtn,*m_OutPutBtn;
    QToolButton *m_pickActorBtn;
    QLabel *m_LabName;
	QLineEdit   *m_ReadEdit;
	QFrdDataPro *m_FrdDataPro;
    int m_WigFrdNum;
    static int m_WigFrdNumOld;
    VTKColorS m_ClrInit;
 

    vtkCallbackCommand *pointPickCallback_;
private slots:
	void BtnOpenFrdSlot();
    void BtnOutPutSlot();
    void SwitchActorPickSlot(bool);
public:
    void init(VTKColorS* str);
    bool readOpenFrd(QString fileName);
	bool readOpenDat(QString fileName);
	//bool readOpenDat(QFile *file);
signals:
	//void emitDataOk();
    void emitDataMenu(const map<QString, QStringList>*);
    //void emitStepPlayMenu(resultFrdS);
    void frdDataOk(FrdDataVIS*);
	void DatOk(resultDatS);
public:
	FrdDataVIS frdVIS_;
    TextVIS frdTextVIS_;
    
};

#endif // QPOSTWIGFILE_H
