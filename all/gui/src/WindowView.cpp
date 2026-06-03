#include "WindowView.h"

#include "QMyVTK.h"
#include "AppLog.h"
#include "QTextEdit"
#include <QMessageBox>
#include <QTimer>

 int QWindowView::m_indexCnt=0;
QWindowView::QWindowView(QWidget *parent): QWidget(parent)//QDockWidget(parent)
{
    //------
     //viewVTK_= QMyVTK::GetInstance(0)->GetVTKWidget();
     //QHBoxLayout *mainLayout = new QHBoxLayout(this);
     //mainLayout->addWidget(viewVTK_);
     //setLayout(mainLayout);
     //mainLayout->setContentsMargins(0,0,0,0);//鍘婚櫎杈规
 /*
    int index =0;
    viewVTK_= QMyVTK::GetInstance()->GetVTKWidget();
    tabView_ = new QTabWidget(this);
    this->setCentralWidget(tabView_); //
    index=tabView_->addTab(viewVTK_,QIcon(":/images/NewIcon.png"),tr("Curve"));
    tabView_->setTabWhatsThis(index, "VTK");
    //tabView_->setCurrentIndex(2);
    //tabView_->setWindowTitle("ok");
    this->setAcceptDrops(true);
    */
    viewPostVTK_= QMyVTK::GetInstance(0)->GetVTKWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(viewPostVTK_);
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0,0,0,0);//鍘婚櫎杈规
  
}

QWindowView::QWindowView(QWidget *parent,int index): QWidget(parent)//QDockWidget(parent)
{
    //------
    //viewVTK_= QMyVTK::GetInstance(index)->GetVTKWidget();

    //viewPostVTK_ = QMyVTK::GetInstance(index)->GetVTKWidget();
    //QHBoxLayout *mainLayout = new QHBoxLayout(this);
    //mainLayout->addWidget(viewPostVTK_);
    //setLayout(mainLayout);
    //mainLayout->setContentsMargins(0,0,0,0);//鍘婚櫎杈规   
    //this->setWindowTitle("niho");
    m_CurrentIndex = index;
    viewPostVTK_= QMyVTK::GetInstance(index)->GetVTKWidget();
    viewPreVTK_= QMyVTK::GetInstance(index+1)->GetVTKWidget();
    tabView_ = new QTabWidget(this);//QTabWidget(this);
    //this->setCentralWidget(tabView_); //
    index=tabView_->addTab(viewPostVTK_,QIcon(":/images/NewIcon.png"),QString::fromUtf8("\345\220\216\345\244\204\347\220\206"));
    tabView_->addTab(viewPreVTK_,QIcon(":/images/NewIcon.png"),QString::fromUtf8("\345\211\215\345\244\204\347\220\206"));
    tabView_->setTabEnabled(0,true);
    tabView_->setTabEnabled(1,true);
    tabView_->setTabWhatsThis(index, "VTK");
	//tabView_->setStyleSheet("QTabBar::tab:disable{width:0;color:transparent}");
    //tabView_->setCurrentIndex(0);
	//TabViewSlot(0);
    tabView_->setWindowTitle("ok");
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(tabView_);
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0,0,0,0);//鍘婚櫎杈规
    this->setAcceptDrops(true);
    m_indexCnt++;
	connect(tabView_,SIGNAL(currentChanged(int)),this,SLOT(TabViewSlot()));
}
//--
QWindowView::~QWindowView()
{
    //if (viewPostVTK_ != NULL){
    //    //delete viewVTK_;
    //    viewPostVTK_ = NULL;
    //    delete viewPostVTK_;
    //}
    //if (viewPreVTK_ != NULL){
    //    //delete viewVTK_;
    //    viewPreVTK_ = NULL;
    //    delete viewPreVTK_;
    //   
    //}
}
//--close
void QWindowView::closeEvent(QCloseEvent *event)
{
    if (m_indexCnt==1){ 
        QMessageBox::warning(this, tr("View"),tr("Cannot close,this is the last one."));
        event->ignore();
    }else{
        m_indexCnt--;
        event->accept();
    }
    
}
void QWindowView::TabViewSlot()
{
    if (!tabView_) {
        AppLog::Write("VIEW", "tab switch ignored: tabView is null");
        return;
    }
    tabView_->setTabEnabled(0,true);
    tabView_->setTabEnabled(1,true);
    AppLog::Write("VIEW", QString("tab switched index=%1").arg(tabView_->currentIndex()));
}

void QWindowView::RenderCurrentTabSlot()
{
    AppLog::Write("VIEW", "explicit tab render skipped");
}
//--center window change tab hide/shown
void QWindowView::TabView(int index)
{
    tabView_->setTabEnabled(0,true);
    tabView_->setTabEnabled(1,true);
	switch (index)
	{
		case 0:{
			tabView_->setCurrentIndex(0);//post process
		}break;
		case 1:{
			tabView_->setCurrentIndex(1);//pre process
		}break;
		default:	break;
	}
    tabView_->setTabEnabled(0,true);
    tabView_->setTabEnabled(1,true);
    TabViewSlot();
}

//--Inp File Mesh Show
void QWindowView::ShowCurPreData(ReadInpResultS InpData)
{
    tabView_->setTabEnabled(0,true);
    tabView_->setTabEnabled(1,true);
    inpVIS_.InitRenderer(QMyVTK::GetInstance(1)->GetRenderer());
    inpVIS_.LoadInpData(&InpData);
    VTKColorS m_ClrInit;
    m_ClrInit.r=0;m_ClrInit.g=1;m_ClrInit.b=0;
    inpVIS_.SetMeshVisible(true,m_ClrInit);
    inpVIS_.Update();
    emit emitInpDataOk(&inpVIS_);//
    //inpVIS_.SetBCVisible(true,"nihao",InpData);
}
//楂樹寒鏄剧ず
void QWindowView::HightLightPSet(NElSurfChsS data)
{
    inpVIS_.SetBCVisible(true,data);
    inpVIS_.Update();
}
//鍗曞厓闆嗗悎checkBox閫夋嫨鏄剧ず
void QWindowView::ActorElSetCheckShow(QStringList strListData)
{
	int nSize=strListData.size();
	if (nSize<0)return;
	NElSurfChsS data;
	data.iType=1;
	for (int kk=0;kk<nSize;kk++)
	{
		QStringList strL = strListData.at(kk).split(",");
		bool f=false;
		if(strL[0].toInt()==1)f=true;
		data.strName=strL[1];
		inpVIS_.SetBCCheckVisible(f,data);
	}
	inpVIS_.SetMeshActorShow(false);
	inpVIS_.Update();
}


