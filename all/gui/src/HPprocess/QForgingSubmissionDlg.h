#ifndef QForgingSubmissionDlg_H
#define QForgingSubmissionDlg_H

#include <QDialog>
#include <QPushButton>
#include <QGridLayout>

class QForgingSubmissionDlg : public QDialog
{
	Q_OBJECT

public:
	QForgingSubmissionDlg(QWidget *parent=NULL);
	~QForgingSubmissionDlg();
	void CreatGUI();
	QPushButton *m_SaveSolveFileBtn;
	QPushButton *m_SubmissionBtn;
private:
public slots:

	void SaveForgingInpSlot();
	void RunSolverSlot();
signals:
	void emitForgingInpSave();
	void emitForgingInpRun();
};

#endif // QForgingSubmissionDlg_H