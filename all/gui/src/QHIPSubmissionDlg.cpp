#include "QHIPSubmissionDlg.h"

QHIPSubmissionDlg::QHIPSubmissionDlg(QWidget *parent)
	: QDialog(parent)
{
  CreatGUI();
}

QHIPSubmissionDlg::~QHIPSubmissionDlg()
{
    setWindowTitle(tr("提交计算"));
}

void QHIPSubmissionDlg::CreatGUI()
{
	m_SaveSolveFileBtn = new QPushButton(tr("保存求解文件"));
	m_SubmissionBtn = new QPushButton(tr("提交计算"));
	QGridLayout *mainLayout = new QGridLayout();
	//mainLayout->addWidget(m_tabView,0,0,1,2);
	mainLayout->addWidget(m_SaveSolveFileBtn,1,0,1,1);
	mainLayout->addWidget(m_SubmissionBtn,1,1,1,1);
	this->setLayout(mainLayout);
	m_SubmissionBtn->setMaximumWidth(100);
	m_SaveSolveFileBtn->setMaximumWidth(100);

	connect(m_SaveSolveFileBtn,SIGNAL(clicked()),this,SLOT(SaveHIPInpSlot()));
	connect(m_SubmissionBtn,SIGNAL(clicked()),this,SLOT(RunSolverSlot()));
}

//--向主窗口发送 保存求解命令
void QHIPSubmissionDlg::SaveHIPInpSlot()
{
	emit emitHipInpSave();
}
//--向主窗口发送运行求解器命令 
void QHIPSubmissionDlg::RunSolverSlot()
{
	emit emitHipInpRun();
}
