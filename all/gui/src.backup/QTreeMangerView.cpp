#include "QTreeMangerView.h"
#include "QTree_Model.h"
#include <QHeaderView>
QTreeMangerView::QTreeMangerView(QWidget *parent)
: QTreeView(parent)
{
	
//--1.Test1
	//QStandardItemModel* TreeModel_;
	//QTreeView* TreeView_ ;
	////item->setEnabled(enabled);
	//// item->setEditable(editable);
	//// item->setSelectable(enabled);
	// QTreeWidget *treeWidget = new QTreeWidget(this);
	// treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
	// treeWidget->setGeometry(QRect(0, 0, 256, 192));
	// treeWidget->setColumnCount(1);
	// 
	// QList<QTreeWidgetItem *> items;
	// for (int i = 0; i < 10; ++i){
	//     items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("item: %1").arg(i))));

	// }
	//treeWidget->insertTopLevelItems(0, items);
	//////treeWidget->insertTopLevelItems(3, items);
//--Test2

	/*QTreeView* TreeView_ = new QTreeView(this);
	treeModel_ = new QTree_Model(this);
	TreeView_->header()->hide();
	TreeView_->setAutoScroll(true);
	TreeView_->setGeometry(0,0,500,500);
	TreeView_->setModel(treeModel_);
	TreeView_->expandToDepth(3);*/
	treeModel_ = NULL;
	//this->setMinimumWidth(200);
	m_ContexShowStyle = 0;
}

QTreeMangerView::QTreeMangerView( QTree_Model *treeModel, QWidget *parent )
: QTreeView(parent)
{
	treeModel_ = treeModel;
	this->setModel(treeModel_);

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(contextMenuPostSlot(const QPoint &)));
    m_ContexShowStyle=M_TREE_POST;
}

QTreeMangerView::QTreeMangerView( QTree_Model *treeModel, QWidget *parent ,int ContexShowStyle)
: QTreeView(parent)
{
    treeModel_ = treeModel;
    this->setModel(treeModel_);
    m_ContexShowStyle=ContexShowStyle;
    
    if (ContexShowStyle == M_TREE_POST ){
        this->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(contextMenuPostSlot(const QPoint &)));
    }else if (ContexShowStyle==M_TREE_MATER){   
       // this->setContextMenuPolicy(Qt::CustomContextMenu);
       // connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(contextMaterMenuSlot(const QPoint &)));
    }else if (ContexShowStyle == M_TREE_PRE){
        this->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),this,SLOT(contextMenuPreSlot(const QPoint &)));
    }
   
}
QTreeMangerView::~QTreeMangerView()
{

}
//--
void QTreeMangerView::SetModel( QTree_Model *treeModel )
{
	treeModel_ = treeModel;
	this->setModel(treeModel_);
}

//--
//void QTreeMangerView::paintEvent( QPaintEvent *event )
//{
//	QTreeView::paintEvent(event);
//}


//--Pre Menu Slot
void QTreeMangerView::contextMenuPreSlot(const QPoint &p)
{
    m_CurveRow=-1;
    m_CurveRowInd=-1;
    m_strCurName="";
    QModelIndex index = this->currentIndex();
    QString fileName=this->model()->data(index,Qt::UserRole+3).toString();
    QMenu *RightMenu = new QMenu;
    RightMenu->addAction(tr("ExpandAll"),this,SLOT(expandAll()));//(ExpandAllSlot()));
    RightMenu->addAction(tr("CollapseAll"),this,SLOT(collapseAll()));//(CollapseAllSlot()));
    
    
    QModelIndex parentIndex=this->model()->parent(index);
    //QString parentStr=this->model()->data(parentIndex)->parent();//.toString();
    QString parentStr=this->model()->data(parentIndex,Qt::UserRole+3).toString();
    if (parentStr=="AmpCurve")
    {
        m_CurveRow=treeModel_->item(0)->child(3,0)->rowCount();
        m_CurveRowInd=treeModel_->item(0)->child(3,0)->row();
        m_strCurName=fileName;
        RightMenu->addAction(tr("Edit"),this,SLOT(EditCurveSlot()));
        RightMenu->addAction(tr("删除"),this,SLOT(DelCurveSlot()));
        RightMenu->addAction(tr("新建"),this,SLOT(NewCurveSlot()));
    }
    if (fileName=="AmpCurve")//增加新的CURVE;
    {
         m_CurveRow=treeModel_->item(0)->child(3,0)->rowCount();
         RightMenu->addAction(tr("新建"),this,SLOT(NewCurveSlot()));
    }
    RightMenu->exec(QCursor::pos());
}
//--新曲线添加function
void QTreeMangerView::NewCurveSlot()
{
   if (m_CurveRow==-1)return; 
  emit(EmitNewCurve(0,m_CurveRow));
}
//--编辑曲线
void QTreeMangerView::EditCurveSlot()
{
    if (m_CurveRowInd==-1)return;
    emit(EmitCurveEdit(m_strCurName));
    
}
//--删除曲线
void QTreeMangerView::DelCurveSlot()
{
    if (m_CurveRowInd==-1)return;
    emit(EmitCurveDel(m_strCurName));
}
//--SLOT
void QTreeMangerView::contextMenuPostSlot(const QPoint &p)
{
    QModelIndex index = this->currentIndex();
    QString fileName=this->model()->data(index).toString();

    QMenu *RightMenu = new QMenu;
    RightMenu->addAction(tr("ExpandAll"),this,SLOT(expandAll()));//(ExpandAllSlot()));
    RightMenu->addAction(tr("CollapseAll"),this,SLOT(collapseAll()));//(CollapseAllSlot()));
    RightMenu->addAction(tr("Test"),this,SLOT(TestSlot()));
    RightMenu->exec(QCursor::pos());
}

void QTreeMangerView::TestSlot()
{
    //this->expandAll();
}
/*
//@Function  mousePressEvent(QMouseEvent *event) 
//@description  tree,Left click
//
//@param  strParent-->pre node(parent)
//              strList-->curentList
//@return void
//@author   qiyan
//@date     2021/01/05
*/
void QTreeMangerView::mousePressEvent(QMouseEvent *event)
{
    QTreeView::mousePressEvent(event);
    QModelIndex index =currentIndex();
    int mm=0;
    if (event->buttons()==Qt::LeftButton)
    {
       //mm=1;
        if(m_ContexShowStyle ==M_TREE_POST){
            emit(EmitChangeModelIndex(index));//前后处理，左键对应的function
        }else if(m_ContexShowStyle == M_TREE_MATER){
            emit(EmitChangeModeIndexMater(index));//材料模块
        }else if (m_ContexShowStyle == M_TREE_PRE){
            QRect rect=visualRect(index);
            QRect expandORCollape=QRect(rect.left()-20,rect.top(),20,rect.height());
            if (!expandORCollape.contains(event->pos())){
              ChangeModeIndexPre(index);
            }
        }
    
    }else if (event->buttons()==Qt::RightButton)
    { 

    }
}

//--前处理
//--QModelIndex
void QTreeMangerView::ChangeModeIndexPre( const QModelIndex &index)
{
    
    QString type = index.data(Qt::UserRole+3).toString();
    QModelIndex parentIndex=this->model()->parent(index);
    QString parentStr=this->model()->data(parentIndex,Qt::UserRole+3).toString();
    if (parentStr == "AmpCurve")//曲线定义
    {
        emit(EmitCurveEdit(type));
    }
    
}
/*
//@Function  upDataTreeNameSlot(QString strParent, QStringList strList) 
//@description  UpData TreeName curve;
//
//@param  strParent-->pre node(parent)
//              strList-->curentList
//@return void
//@author   qiyan
//@date     2021/01/05
*/
void QTreeMangerView::upDataTreeNameSlot(QString strParent, QStringList strList)
{
  int rows1 = treeModel_->rowCount();
    QString name;
    for (int i=0;i<rows1;i++){
        QStandardItem* pItem = treeModel_->item(i,0);//root
        if (pItem==NULL)continue;
        int rows2 = pItem->rowCount();
        for (int j=0;j<rows2;j++){
            QStandardItem* childItem1 = pItem->child(j,0);//second
            if (childItem1==NULL)continue;
            name = childItem1->index().data(Qt::UserRole+3).toString();
            int rows3 = childItem1->rowCount();
            if (name == "AmpCurve"){//insert amplitude   
                childItem1->removeRows(0,rows3);//删除所有项，进行添加
                for (int kk=0; kk<strList.size(); kk++){
                    childItem1->setChild(kk,0,treeModel_->CreateModelItem(childItem1,strList.at(kk),false,true,strList.at(kk)));
                }               
            }              
           /* else if (name =="PlotOpt")childItem1->setText("Plot Options");
            else if (name =="RestOpt")childItem1->setText("Result Options");
            else if (name =="RestOut")childItem1->setText("Result output");*/
        }
    }
}


