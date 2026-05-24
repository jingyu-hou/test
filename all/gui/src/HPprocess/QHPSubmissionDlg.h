#ifndef QHPSUBMISSIONDLG_H
#define QHPSUBMISSIONDLG_H

#include <QDialog>
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QTabWidget>

class QHPSubmissionDlg : public QDialog
{
    Q_OBJECT

public:
    QHPSubmissionDlg(QWidget *parent=NULL);
    ~QHPSubmissionDlg();
    void CreatGUI();

    QWidget *m_Tab1,*m_Tab2,*m_Tab3;
    QTabWidget *m_tabView;
    QLineEdit *m_EditCalcNum;
    QLineEdit *m_EditReset;
    QLineEdit *m_EditPath;
    QLineEdit *m_EditCommand;
    QPushButton *m_SaveSolveFileBtn,*m_CheckBtn;
    QPushButton *m_SubmissionBtn,*m_KillBtn;
    QPushButton *m_PathSetBtn;
public slots:
    void SetPathSlot();
    void SaveHPInpSlot();
    void RunSolverSlot();
	void CheckSlot();
	void KillolverSlot();
signals:
    void emitHpInpSave();
    void emitHpInpRun();
	void emitCheck();
	void emitHipInpKill();

};

#endif // QHPSUBMISSIONDLG_H
