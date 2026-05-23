#include "WindowView.h"

#include "QMyVTK.h"
#include "QTextEdit"
#include <QMessageBox>

 int QWindowView::m_indexCnt=0;
QWindowView::QWindowView(QWidget *parent): QWidget(parent)//QDockWidget(parent)
{
    //------
     //viewVTK_= QMyVTK::GetInstance(0)->GetVTKWidget();
     //QHBoxLayout *mainLayout = new QHBoxLayout(this);
     //mainLayout->addWidget(viewVTK_);
     //setLayout(mainLayout);
     //mainLayout->setContentsMargins(0,0,0,0);//去除边框
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
    mainLayout->setContentsMargins(0,0,0,0);//去除边框
  
}

QWindowView::QWindowView(QWidget *parent,int index): QWidget(parent)//QDockWidget(parent)
{
    //------
    //viewVTK_= QMyVTK::GetInstance(index)->GetVTKWidget();

    //viewPostVTK_ = QMyVTK::GetInstance(index)->GetVTKWidget();
    //QHBoxLayout *mainLayout = new QHBoxLayout(this);
    //mainLayout->addWidget(viewPostVTK_);
    //setLayout(mainLayout);
    //mainLayout->setContentsMargins(0,0,0,0);//去除边框   
    //this->setWindowTitle("niho");
    m_CurrentIndex = index;
    viewPostVTK_= QMyVTK::GetInstance(index)->GetVTKWidget();
    viewPreVTK_= QMyVTK::GetInstance(index+1)->GetVTKWidget();
    tabView_ = new QTabWidget(this);//QTabWidget(this);
    //this->setCentralWidget(tabView_); //
    index=tabView_->addTab(viewPostVTK_,QIcon(":/images/NewIcon.png"),tr("后处理"));
    tabView_->addTab(viewPreVTK_,QIcon(":/images/NewIcon.png"),tr("前处理"));
    tabView_->setTabWhatsThis(index, "VTK");
	//tabView_->setStyleSheet("QTabBar::tab:disable{width:0;color:transparent}");
    //tabView_->setCurrentIndex(0);
	//TabViewSlot(0);
    tabView_->setWindowTitle("ok");
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(tabView_);
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0,0,0,0);//去除边框
    this->setAcceptDrops(true);
    m_indexCnt++;
	connect(tabView_,SIGNAL(clicked()),this,SLOT(TabViewSlot()));
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
	int index=tabView_->currentIndex();

}
//--center window change tab hide/shown
void QWindowView::TabView(int index)
{
	switch (index)
	{
		case 0:{
			tabView_->setCurrentIndex(0);//post process
			tabView_->setTabEnabled(0,true);
			tabView_->setTabEnabled(1,false);
		}break;
		case 1:{
			tabView_->setCurrentIndex(1);//pre process
			tabView_->setTabEnabled(1,true);
			tabView_->setTabEnabled(0,false);
		}break;
		default:	break;
	}
}

//--Inp File Mesh Show
void QWindowView::ShowCurPreData(ReadInpResultS InpData)
{
    inpVIS_.InitRenderer(QMyVTK::GetInstance(1)->GetRenderer());
    inpVIS_.LoadInpData(&InpData);
    VTKColorS m_ClrInit;
    m_ClrInit.r=0;m_ClrInit.g=1;m_ClrInit.b=0;
    inpVIS_.SetMeshVisible(true,m_ClrInit);
    inpVIS_.Update();
    emit emitInpDataOk(&inpVIS_);//
    //inpVIS_.SetBCVisible(true,"nihao",InpData);
}
//高亮显示
void QWindowView::HightLightPSet(NElSurfChsS data)
{
    inpVIS_.SetBCVisible(true,data);
    inpVIS_.Update();
}
//单元集合checkBox选择显示
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
