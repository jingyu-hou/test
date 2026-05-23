#include "PreProcess_Panel.h"
#include "ADefineQ.h"


//*-热等静压
QPreHIPPrc::QPreHIPPrc(QWidget *parent)
	: QWidget(parent)
{
	//this->setWindowTitle(tr("前处理"));
	//this->setObjectName("前处理");
   /* m_PostWidPlotOptDlg = NULL;
    m_PosWigReadResultDlg = NULL;
    m_FileScalarName = "";*/

   // QString FilePathName;
    //FilePathName=QCoreApplication::applicationDirPath()+PLOTOPTNAME;
   // LoadConfigFile(FilePathName);	
}

QPreHIPPrc::~QPreHIPPrc()
{

}
void QPreHIPPrc::setStyle()
{ 
    m_TreeModel = new QTree_Model(this,1);
    m_TreeView = new QTreeMangerView(m_TreeModel,this,1);

    m_TreeView->header()->hide();
    m_TreeView->setAutoScroll(true);
    m_TreeView->setGeometry(0,0,500,500);
    m_TreeView->setModel(m_TreeModel);
    m_TreeView->expandToDepth(3);
    QVBoxLayout *MainLayout = new QVBoxLayout(this);
    MainLayout->addWidget(m_TreeView);
    MainLayout->addStretch();
    this->setLayout(MainLayout);
    connect(m_TreeView,SIGNAL(EmitChangeModeIndexPre(const QModelIndex &)),this,
        SLOT(ChangeModelIndexSlot(const QModelIndex &)));
}
//--插入Part到项目中，Tree
void QPreHIPPrc::InsertPart(QString strName)
{
   m_TreeModel->InsertRootPreModelTree();
}


//****************************************************
// Method:    单击树节点的响应;
// Returns:   void;
// Parameter: index     树节点
// Author:	  
//****************************************************
void QPreHIPPrc::ChangeModelIndexSlot( const QModelIndex & index )
{  
  //  Render_Widget *renWidget = Render_Widget::GetInstance();
    QString type = index.data(Qt::UserRole+3).toString();
    if (type == "AmpCurve")
    {
        //m_PostWidPlotOptDlg->show(); 
        //m_PostWidPlotOptDlg->raise();//最上层
        //m_PostWidPlotOptDlg->activateWindow();//激活
    }
    else if (type == "DatafrdOpt")//
    {
        //m_StackedWidget->setCurrentWidget(m_PosWigFile);
    }
	else if (type == "ReadRest")//
	{
		//m_StackedWidget->setCurrentWidget(m_PosWigReadResults);
	 
	   //m_PosWigReadResultDlg->show(); 
	   //m_PosWigReadResultDlg->raise();//最上层
	   //m_PosWigReadResultDlg->activateWindow();//激活
	}

}
//*-热挤压
QPreHEPrc::QPreHEPrc(QWidget *parent)
	: QWidget(parent)
{
	//this->setWindowTitle(tr("前处理"));
	//this->setObjectName("前处理");
   /* m_PostWidPlotOptDlg = NULL;
    m_PosWigReadResultDlg = NULL;
    m_FileScalarName = "";*/

   // QString FilePathName;
    //FilePathName=QCoreApplication::applicationDirPath()+PLOTOPTNAME;
   // LoadConfigFile(FilePathName);	
}

QPreHEPrc::~QPreHEPrc()
{

}
void QPreHEPrc::setStyle()
{ 
    m_TreeModel = new QTree_Model(this,1);
    m_TreeView = new QTreeMangerView(m_TreeModel,this);

    m_TreeView->header()->hide();
    m_TreeView->setAutoScroll(true);
    m_TreeView->setGeometry(0,0,500,500);
    m_TreeView->setModel(m_TreeModel);
    m_TreeView->expandToDepth(3);
    QVBoxLayout *MainLayout = new QVBoxLayout(this);
    MainLayout->addWidget(m_TreeView);
    MainLayout->addStretch();
    this->setLayout(MainLayout);
}


//*-热处理
QPreHPPrc::QPreHPPrc(QWidget *parent)
	: QWidget(parent)
{
	//this->setWindowTitle(tr("前处理"));
	//this->setObjectName("前处理");
   /* m_PostWidPlotOptDlg = NULL;
    m_PosWigReadResultDlg = NULL;
    m_FileScalarName = "";*/

   // QString FilePathName;
    //FilePathName=QCoreApplication::applicationDirPath()+PLOTOPTNAME;
   // LoadConfigFile(FilePathName);	
}

QPreHPPrc::~QPreHPPrc()
{

}
void QPreHPPrc::setStyle()
{ 
    m_TreeModel = new QTree_Model(this,1);
    m_TreeView = new QTreeMangerView(m_TreeModel,this);

    m_TreeView->header()->hide();
    m_TreeView->setAutoScroll(true);
    m_TreeView->setGeometry(0,0,500,500);
    m_TreeView->setModel(m_TreeModel);
    m_TreeView->expandToDepth(3);
    QVBoxLayout *MainLayout = new QVBoxLayout(this);
    MainLayout->addWidget(m_TreeView);
    MainLayout->addStretch();
    this->setLayout(MainLayout);
}