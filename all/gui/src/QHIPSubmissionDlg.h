#ifndef QHIPSUBMISSIONDLG_H
#define QHIPSUBMISSIONDLG_H

#include <QDialog>
#include <QPushButton>
#include <QGridLayout>

class QHIPSubmissionDlg : public QDialog
{
	Q_OBJECT

public:
	QHIPSubmissionDlg(QWidget *parent=NULL);
	~QHIPSubmissionDlg();
	void CreatGUI();
	QPushButton *m_SaveSolveFileBtn;
	QPushButton *m_SubmissionBtn;
	//QPushButton *m_KillBtn;
private:
public slots:

	void SaveHIPInpSlot();
	void RunSolverSlot();
	//void KillolverSlot();
signals:
	void emitHipInpSave();
	void emitHipInpRun();
	
};

#endif // QHIPSUBMISSIONDLG_H
