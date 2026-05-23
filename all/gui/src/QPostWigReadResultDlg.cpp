#include "QPostWigReadResultDlg.h"
#include "ADefineQ.h"
#include <QXmlStreamWriter>
#include <QFile>
#include <QCoreApplication>
/*
描述：ReadResult对话框，主要完成云图界面中节本信息设定；
    1.Timer：当前是哪个时间节点
    2.Step：当前step步子
    3.Inc:
    3.Data：文本日期
    4.No：全部不显示
*/
QPostWigReadResultDlg::QPostWigReadResultDlg(QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle(tr("读取结果"));
	QTabWidget *m_TabWig = new QTabWidget(this);
	//QPostWigRResult_Basic *
	TabPage1 = new QPostWigRResult_Basic(m_TabWig);
	TabPage2 = new QPostWigRResult_RResults(m_TabWig);
	TabPage3 = new QPostWigRResult_BTN(m_TabWig);
	TabPage4 = new QPostWigRResult_Other(m_TabWig);

	int index = m_TabWig->addTab(TabPage1, "显示");
	m_TabWig->addTab(TabPage2,"读取结果");
	m_TabWig->addTab(TabPage3,"按钮");
	m_TabWig->addTab(TabPage4,"其他");
	QVBoxLayout *Vlayout = new QVBoxLayout ();
	Vlayout->addWidget(m_TabWig);
	//--button
	QPushButton *BtnOK = new QPushButton(tr("确定"));
	QPushButton *BtnCancel = new QPushButton(tr("取消"));
	QPushButton *BtnApply = new QPushButton(tr("应用"));

	QHBoxLayout *HLayout1 = new QHBoxLayout();
	HLayout1->addWidget(BtnOK);
	HLayout1->addWidget(BtnCancel);
	HLayout1->addWidget(BtnApply);

	QVBoxLayout *MainLayout = new QVBoxLayout();
	//lout->setContentsMargins(2, 2, 2, 2);
	MainLayout->addLayout(Vlayout);
	MainLayout->addLayout(HLayout1);

	this->setLayout(MainLayout);
	connect(BtnOK,SIGNAL(clicked()),this,SLOT(BtnOkSlot()));
	connect(BtnCancel,SIGNAL(clicked()),this,SLOT(BtnCancelSlot()));
	connect(BtnApply,SIGNAL(clicked()),this,SLOT(BtnApplySlot()));
}

QPostWigReadResultDlg::~QPostWigReadResultDlg()
{

}
void QPostWigReadResultDlg::BtnCancelSlot()
{
	this->close();
}
void QPostWigReadResultDlg::BtnOkSlot()
{
	PutData();
    ExportData();
    emit emitReadResultParam(m_ReadResultDlgS);
	this->close();
}
void QPostWigReadResultDlg::BtnApplySlot()
{
	//this->close();
	PutData();
    ExportData();
    emit emitReadResultParam(m_ReadResultDlgS);
}
//--从界面中获取数据
void QPostWigReadResultDlg::PutData()
{
    m_ReadResultDlgS.RadioTextIndex=0;
    if (TabPage1-> m_chkNo->isChecked()){
        m_ReadResultDlgS.RadioTextIndex=0;//不显示
        return;
    }  
    if (TabPage1->m_chkStepSub->isChecked()){   
        m_ReadResultDlgS.RadioTextIndex |= 0x01;//step显示
    }
    if (TabPage1->m_chkInc->isChecked()){
        m_ReadResultDlgS.RadioTextIndex |= 0x02;//Inc显示
    
    }
    if(TabPage1->m_chkTimes->isChecked()){
        m_ReadResultDlgS.RadioTextIndex |=0x04;//time显示
    }

}
//--写入到界面
void QPostWigReadResultDlg::SetData(ReadResultDlgS data)
{
    m_ReadResultDlgS = data;
    TabPage1->m_TextRadioIndex=m_ReadResultDlgS.RadioTextIndex;  
    TabPage1->SetData();
}
//--ExportData 将当前设置保存到xml配置文件
void QPostWigReadResultDlg::ExportData()
{
    QString FilePathName;
    FilePathName=QCoreApplication::applicationDirPath()+READRESULT;
    QFile file(FilePathName);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        return;
    }
    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartElement(READRESULTSTART);
    QString str;

    writer.writeStartElement("显示");
    str=QString("%1").arg(m_ReadResultDlgS.RadioTextIndex);
    writer.writeTextElement("Bs1",str);
    writer.writeEndElement();
    writer.writeEndDocument();
    file.close();
}

//*****************************************************
//*****************************************************
//--QPostWigRResult_Basic
//*****************************************************
//*****************************************************
QPostWigRResult_Basic::QPostWigRResult_Basic(QWidget *parent)
: QWidget(parent)
{
	////-1
	QGroupBox *gbBox1 = new QGroupBox("",this);
	m_chkStepSub = new QCheckBox(tr("Step"),gbBox1);
	m_chkInc = new QCheckBox(tr("Inc"),gbBox1);//(tr("Hidden"),gbBox1);
	m_chkType = new QCheckBox(tr("Type"),gbBox1);//(tr("Filled"),gbBox1);
	m_chkTimes = new QCheckBox(tr("Times"),gbBox1);
	m_chkNo = new QCheckBox(tr("No"),gbBox1);
	
	//gbBox1->setCheckable(true);
	//gbBox1->setChecked(true);

	QVBoxLayout *VLayout1= new QVBoxLayout(gbBox1);
	VLayout1->addWidget(m_chkStepSub);
	VLayout1->addWidget(m_chkInc);
	VLayout1->addWidget(m_chkType);
	VLayout1->addWidget(m_chkTimes);
	VLayout1->addWidget(m_chkNo);
	//connect(m_chkStepSub,SIGNAL(click()),this,SLOT(setRenderSlot(int)));
	connect(m_chkNo,SIGNAL(stateChanged(int)),this,SLOT(CheckNo()));			

	QGridLayout *MainLayout = new QGridLayout(this);
	MainLayout->addWidget(gbBox1,1,1);
	//MainLayout->addWidget(gbBox2,2,1);
	//MainLayout->addWidget(gbBox3,1,2,2,1);
	setLayout(MainLayout);

m_chkType->hide();

	//setChecked();
;
}

QPostWigRResult_Basic::~QPostWigRResult_Basic()
{
	;

}
void QPostWigRResult_Basic::LanguageUpData()
{
    m_chkStepSub->setText(tr("Step"));
    m_chkInc->setText(tr("Inc"));
    m_chkType->setText(tr("Type"));//(tr("Filled"),gbBox1);
    m_chkTimes->setText(tr("Times"));
    m_chkNo->setText(tr("No"));
}

//--SLOT
//--
//descrip: 
//			1.set the checkBox State and checked or not
//--
void QPostWigRResult_Basic::CheckNo()
{
	bool bFlag =	m_chkNo->isChecked();
	if (bFlag == true)
	{
			m_chkInc->setChecked(false);
			m_chkStepSub->setChecked(false);
			m_chkType->setChecked(false);
			m_chkTimes->setChecked(false);
			m_chkInc->setEnabled(false);
			m_chkStepSub->setEnabled(false);
			m_chkType->setEnabled(false);
			m_chkTimes->setEnabled(false);
	}else{
			m_chkInc->setEnabled(true);
			m_chkStepSub->setEnabled(true);
			m_chkType->setEnabled(true);
			m_chkTimes->setEnabled(true);
	}

}
void QPostWigRResult_Basic::SetData()
{
    if (m_TextRadioIndex ==0) {
        m_chkNo->setCheckState(Qt::Checked);
        return;
    }
    if ((m_TextRadioIndex & 0x01)==0x01)
    {
        m_chkStepSub->setEnabled(true);
        m_chkStepSub->setChecked(true);
    }
    if ((m_TextRadioIndex & 0x02)==0x02)
    {
        m_chkInc->setEnabled(true);
        m_chkInc->setChecked(true);
    }
    if ((m_TextRadioIndex & 0x04)==0x04)
    {
        m_chkTimes->setEnabled(true);
        m_chkTimes->setChecked(true);
    }
   
    
}
//*****************************************************
//*****************************************************
//--QPostWigRResult_RResults
//*****************************************************
//*****************************************************
QPostWigRResult_RResults::QPostWigRResult_RResults(QWidget *parent)
: QWidget(parent)
{	;
}

QPostWigRResult_RResults::~QPostWigRResult_RResults()
{
	;

}
//*****************************************************
//*****************************************************
//--QPostWigRResult_BTN
//*****************************************************
//*****************************************************
QPostWigRResult_BTN::QPostWigRResult_BTN(QWidget *parent)
: QWidget(parent)
{	;
}

QPostWigRResult_BTN::~QPostWigRResult_BTN()
{
	;

}
//*****************************************************
//*****************************************************
//--QPostWigRResult_Other
//*****************************************************
//*****************************************************
QPostWigRResult_Other::QPostWigRResult_Other(QWidget *parent)
: QWidget(parent)
{	
	;
}

QPostWigRResult_Other::~QPostWigRResult_Other()
{
	;

}

