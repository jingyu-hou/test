#ifndef QPOSTWIGREADRESULT_H
#define QPOSTWIGREADRESULT_H

#include <QObject>
#include <QDialog>
#include <QPushButton>
#include <QWidget>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QRadioButton>
#include <QCheckBox>
//--Result Option
class QPostWigReadResult_Basic;
class QPostWigRResult_RResults;
class QPostWigRResult_BTN;
class QPostWigRResult_Other;

typedef struct _READRESULT_S{
    int RadioTextIndex;
    _READRESULT_S(){
        RadioTextIndex = 0;
    }
}ReadResultDlgS;


//--Read Results_Basic 1
class QPostWigRResult_Basic : public QWidget
{
	Q_OBJECT

public:
	QPostWigRResult_Basic(QWidget *parent);
	~QPostWigRResult_Basic();
private:
	
public:
    QCheckBox *m_chkStepSub,*m_chkInc,*m_chkType,*m_chkTimes,*m_chkNo;
    int m_TextRadioIndex;
    ReadResultDlgS m_ReadRltDlgS;
public:
    void LanguageUpData();
    void SetData();
private slots:
//	void CheckStepSub();
	void CheckNo();
		
};

//--Read Results_ReadResults 2
class QPostWigRResult_RResults : public QWidget
{
	Q_OBJECT

public:
	QPostWigRResult_RResults(QWidget *parent=NULL);
	~QPostWigRResult_RResults();

private:

};

//--Read Results_Button 2
class QPostWigRResult_BTN : public QWidget
{
	Q_OBJECT

public:
	QPostWigRResult_BTN(QWidget *parent=NULL);
	~QPostWigRResult_BTN();

private:

};
//--Read Results_Other 3
class QPostWigRResult_Other : public QWidget
{
	Q_OBJECT

public:
	QPostWigRResult_Other(QWidget *parent=NULL);
	~QPostWigRResult_Other();

private:

};

//----------------------------------------------
//--
//----------------------------------------------
class QPostWigReadResultDlg : public QDialog
{
	Q_OBJECT

public:
	QPostWigReadResultDlg(QWidget *parent=NULL);
	~QPostWigReadResultDlg();
public:
	QPostWigRResult_Basic *TabPage1;
	QPostWigRResult_RResults *TabPage2;
	QPostWigRResult_BTN *TabPage3;
	QPostWigRResult_Other *TabPage4;

    ReadResultDlgS m_ReadResultDlgS;
public:
    void PutData();
    void ExportData();
    void SetData(ReadResultDlgS);
private slots:
	void BtnCancelSlot();
	void BtnOkSlot();
	void BtnApplySlot();
signals:
    void emitReadResultParam(ReadResultDlgS );

};
#endif // QPOSTWIGREADRESULT_H
