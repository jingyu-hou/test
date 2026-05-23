#include "QForgingSubmissionDlg.h"

QForgingSubmissionDlg::QForgingSubmissionDlg(QWidget *parent)
	: QDialog(parent)
{
  CreatGUI();
}

QForgingSubmissionDlg::~QForgingSubmissionDlg()
{
    setWindowTitle(tr("提交计算"));
}

void QForgingSubmissionDlg::CreatGUI()
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

	connect(m_SaveSolveFileBtn,SIGNAL(clicked()),this,SLOT(SaveForgingInpSlot()));
	connect(m_SubmissionBtn,SIGNAL(clicked()),this,SLOT(RunSolverSlot()));

///*************************************************************************************
	//setWindowTitle(tr("提交计算"));
	//setMinimumSize(300,150);
	//setMaximumSize(400,150);
 //   m_Tab1=new QWidget();//(tr("ddd1"));
 //   QLabel *lab1=new QLabel(tr("CPU核数"),m_Tab1);
 //   //QLabel *lab2=new QLabel(tr("GPU核数"),m_Tab1);
 //   m_EditCalcNum = new QLineEdit(tr("2"),m_Tab1);
 //   //m_EditReset = new QLineEdit(tr("100"),m_Tab1);
 //   QGridLayout *gridLayout = new QGridLayout();
 //   gridLayout->addWidget(lab1,0,0,1,1);
 //   gridLayout->addWidget(m_EditCalcNum,0,1,1,1);
 //   //gridLayout->addWidget(lab2,1,0,1,1);
 //   //gridLayout->addWidget(m_EditReset,1,1,1,1);
 //   m_EditCalcNum->setMaximumWidth(100);
 //   m_Tab1->setLayout(gridLayout);

 //   m_Tab2=new QWidget();//(tr("ddd2"))
 //   lab1=new QLabel(tr("路径"),m_Tab2);
 //   //QLabel *lab2=new QLabel(tr("命令行"),m_Tab2);
 //   m_EditPath = new QLineEdit(m_Tab2);
 //   //m_EditCommand= new QLineEdit(m_Tab2);
 //   m_PathSetBtn = new QPushButton(tr("加载路径"),m_Tab2);
 //   QGridLayout *gridLayout2 = new QGridLayout();
 //   gridLayout2->addWidget(lab1,0,0,1,1);
 //   gridLayout2->addWidget(m_EditPath,0,1,1,1);
 //   gridLayout2->addWidget(m_PathSetBtn,0,2,1,1);
 //   //gridLayout2->addWidget(lab2,1,0,1,1);
 //   //gridLayout2->addWidget(m_EditCommand,1,1,1,1);
 //   m_Tab2->setLayout(gridLayout2);

 //   m_Tab3=new QWidget();//(tr("ddd2"))

 //   m_tabView = new QTabWidget();
 //   m_tabView->addTab(m_Tab1, tr("并行"));
 //   m_tabView->addTab(m_Tab2, tr("子程序"));
 //   m_tabView->addTab(m_Tab3, tr("高级"));
 //   m_SaveSolveFileBtn = new QPushButton(tr("输出Inp文件"));
 //   m_SubmissionBtn = new QPushButton(tr("提交计算"));
	//m_CheckBtn = new QPushButton(tr("检查"));
 //   QGridLayout *mainLayout = new QGridLayout();
 //   //mainLayout->addWidget(m_tabView,0,0,1,2);
	//mainLayout->addWidget(m_CheckBtn,1,0,1,1);
 //   mainLayout->addWidget(m_SaveSolveFileBtn,2,0,1,1);
 //   mainLayout->addWidget(m_SubmissionBtn,3,0,1,1);
	//mainLayout->addWidget(m_tabView,1,4,3,3);
 //   this->setLayout(mainLayout);
	//m_CheckBtn->setMaximumWidth(100);
 //   m_SubmissionBtn->setMaximumWidth(100);
 //   m_SaveSolveFileBtn->setMaximumWidth(100);
	////m_tabView->hide();

	//connect(m_CheckBtn,SIGNAL(clicked()),this,SLOT(CheckSlot()));
 //   connect(m_PathSetBtn,SIGNAL(clicked()),this,SLOT(SetPathSlot()));
 //   connect(m_SaveSolveFileBtn,SIGNAL(clicked()),this,SLOT(SaveHPInpSlot()));
 //   connect(m_SubmissionBtn,SIGNAL(clicked()),this,SLOT(RunSolverSlot()));
}

//--向主窗口发送 保存求解命令
void QForgingSubmissionDlg::SaveForgingInpSlot()
{
	emit emitForgingInpSave();
}
//--向主窗口发送运行求解器命令 
void QForgingSubmissionDlg::RunSolverSlot()
{
	emit emitForgingInpRun();
}
