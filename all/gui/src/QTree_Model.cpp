#include "QTree_Model.h"
#include <QMessageBox>
QTree_Model::QTree_Model(QObject *parent)
	: QStandardItemModel(parent)
{
     CreatePostModelTree();
}
//--------------------------------------
//@Function   QTree_Model::QTree_Model(QObject *parent,int ModPageId)  
//@description  create tree mode depend ID
//              (ID==0,PostProcess)
//              (ID==1,ProProcess)
//@param  parent,  ModPageId
//@return void
//@author   qiyan
//@date     2020/03/25
//--------------------------------------
QTree_Model::QTree_Model(QObject *parent,int ModPageId): QStandardItemModel(parent)
{
    if (M_TREE_POST == ModPageId){
      CreatePostModelTree();
    }else if(M_TREE_PRE == ModPageId){
      //CreatePreModelTree();//隐藏前处理模型树 luo
    }else if(M_TREE_MATER == ModPageId){
      CreateMaterTree();
    }  
}

QTree_Model::~QTree_Model()
{

}

void QTree_Model::CreatePostModelTree()
{
	this->setRowCount(1);
	this->setColumnCount(1);
	QStandardItem* item = NULL;
	QStandardItem* p_item = NULL;
	QString name ="General Postproc";

	item = CreateModelItem(NULL,name,false,true,"Project");
	this->setItem(0,0,item);

	p_item = this->item(0,0);
    item = CreateModelItem(p_item,tr("Data&File Opts"),false,true,"DatafrdOpt");//0
	item = CreateModelItem(p_item,tr("Plot Options"),false,true,"PlotOpt");//1
	item = CreateModelItem(p_item,tr("Result Options"),false,true,"RestOpt");//2
	item = CreateModelItem(p_item->child(2,0),tr("Read Results"),false,true,"ReadRest");

	item = CreateModelItem(p_item,tr("Result output"),false,true,"RestOut");//3
	item = CreateModelItem(p_item->child(3,0),tr("Plot Results"),false,true,"PlotRest");
	item = CreateModelItem(p_item->child(3,0),tr("List Results"),false,true,"ListRest");
	item = CreateModelItem(p_item->child(3,0),tr("Time history curve"),false,true,"TimHisCur");
	item = CreateModelItem(p_item->child(3,0),tr("Re-meshing"),false,true,"RMesh");
	item = CreateModelItem(p_item->child(3,0),tr("other"),false,true,"Other");
	item->setEnabled(false);
}
QStandardItem* QTree_Model::CreateModelItem( QStandardItem *itemp,QString itemText,bool editable, bool enabled, QString tree )
{
	QStandardItem* item = new QStandardItem(itemText);
	item->setData(tree,(Qt::UserRole+3));
	if (itemp)
	{
		itemp->appendRow(item);
	}
	item->setEnabled(enabled);
	item->setEditable(editable);
	item->setSelectable(enabled);
	return item;
}
void QTree_Model::CreatePreModelTree()
{
    this->setRowCount(2);
    this->setColumnCount(1);
    QStandardItem* item = NULL;
    QStandardItem* p_item = NULL;
    QString name = tr("模型");

    item = CreateModelItem(NULL,name,false,true,"Model");
    item->setIcon(QIcon(":/images/save.png"));
    this->setItem(0,0,item);
   
    BaseTreeItem tmpMap;
    tmpMap.item=item;
    tmpMap.iDepth=0;//0级
    tmpMap.name=name;
    tmpMap.nameAct="Model";
    PreHIPRootNum=0;
    p_item = this->item(0,0);
  
   /* item = CreateModelItem(p_item,tr("单元"),false,true,"DatafrdOpt");
    p_item->child(0,0)->setIcon(QIcon(":/images/open.png"));

    item = CreateModelItem(p_item->child(0,0),tr("part1"),false,true,"PlotOpt");

    item = CreateModelItem(p_item,tr("区域"),false,true,"RestOpt");
    item = CreateModelItem(p_item->child(1,0),tr("region1"),false,true,"ReadRest");
    item->setEnabled(false);

    name = tr("边界条件");
    item = CreateModelItem(NULL,name,false,true,"Project");

    this->setItem(1, 0, item);
    PreHIPRootNum=1;
    p_item = this->item(1,0);

    item = CreateModelItem(p_item,tr("运动"),false,true,"DatafrdOpt");
    item = CreateModelItem(p_item->child(0,0),tr("part1"),false,true,"PlotOpt");
    item = CreateModelItem(p_item->child(0,0),tr("part2"),false,true,"PlotOpt");
    item = CreateModelItem(p_item->child(0,0),tr("part3"),false,true,"PlotOpt");
    PreHIPRootNum++;*/
  

    item = CreateModelItem(p_item,tr("单元"),false,true,"DatafrdOpt");
    p_item->child(0,0)->setIcon(QIcon(":/images/open.png"));

    item = CreateModelItem(p_item->child(0,0),tr("单元类型1"),false,true,"PlotOpt1");
    p_item->child(0,0)->child(0,0)->setIcon(QIcon(":/images/save.png"));
    item = CreateModelItem(p_item->child(0,0)->child(0,0),tr("数量"),false,true,"PlotOpt11");
    //p_item->child(0,0)->child(0,0)->setIcon(QIcon(":/images/save.png"));
    item = CreateModelItem(p_item->child(0,0),tr("单元类型2"),false,true,"PlotOpt2");

    item = CreateModelItem(p_item,tr("节点"),false,true,"RestOpt");
    item = CreateModelItem(p_item->child(1,0),tr("数量"),false,true,"RestOpt1");
    item->setEnabled(false);

    item = CreateModelItem(p_item,tr("集合"),false,true,"block");
    item = CreateModelItem(p_item->child(2,0),tr("节点"),false,true,"block1");
    item = CreateModelItem(p_item->child(2,0),tr("单元"),false,true,"block2");
    item = CreateModelItem(p_item->child(2,0),tr("面"),false,true,"block3");
    item = CreateModelItem(p_item->child(2,0),tr("面1"),false,true,"block4");
    item = CreateModelItem(p_item->child(2,0),tr("面2"),false,true,"block5");

    item = CreateModelItem(p_item,tr("幅值曲线"),false,true,"AmpCurve");


   /* name = tr("边界条件");
    item = CreateModelItem(NULL,name,false,true,"Project");

    this->setItem(1, 0, item);
    PreHIPRootNum=1;
    p_item = this->item(1,0);

    item = CreateModelItem(p_item,tr("运动"),false,true,"DatafrdOpt");
    item = CreateModelItem(p_item->child(0,0),tr("part1"),false,true,"PlotOpt");
    item = CreateModelItem(p_item->child(0,0),tr("part2"),false,true,"PlotOpt");
    item = CreateModelItem(p_item->child(0,0),tr("part3"),false,true,"PlotOpt");
    PreHIPRootNum++;*/
}
//--插入根部节点
void QTree_Model::InsertRootPreModelTree()
{
    /*根部
    QStandardItem* item = NULL;
    QStandardItem* p_item = NULL;

    QString name;
    name = tr("New %i").arg(PreHIPRootNum);
    item = CreateModelItem(NULL,name,false,true,"Project");
    this->setItem(PreHIPRootNum, 0, item);
    PreHIPRootNum++;
    */

    /*第一个
       
     */
    QStandardItem* item = NULL;
    QStandardItem* p_item = NULL;

    QString name;
    //name = tr("New %i").arg(PreHIPRootNum);
    //item = CreateModelItem(NULL,name,false,true,"Project");
   /* item=this->Item(0, 0);
    p_item = this->item(0,0);

    item = CreateModelItem(p_item,tr("部件"),false,true,"DatafrdOpt");
    p_item->child(0,0)->setIcon(QIcon(":/images/open.png"));

    item = CreateModelItem(p_item->child(0,0),tr("part1"),false,true,"PlotOpt");


    PreHIPRootNum++;*/
}
////-插入第一个根部节点的第一个节点
//void QTree_Model::InsertChild1PreModelTree()
//{
//    QStandardItem* item = NULL;
//    QStandardItem* p_item = NULL;
//
//    QString name;
//    //name = tr("New %i").arg(PreHIPRootNum);
//    //item = CreateModelItem(NULL,name,false,true,"Project");
//    
//    PreHIPRootNum++;
//}


//--得到根部节点
QStandardItem* QTree_Model::getTopParent(QStandardItem* item)
{
    QStandardItem* currentItem = item;
    while (item->parent() != 0)
    {
        currentItem = item->parent();
        item = currentItem;
    }
    if (currentItem->index().column() != 0)
    {
      //  QStandardItemModel* model = static_cast<QStandardItemModel*>(this->model());
       // currentItem = model->itemFromIndex(currentItem->index().sibling(currentItem->index().row(),0));
    }
    return currentItem;
}



//--材料数据Tree

void QTree_Model::CreateMaterTree()
{
 
    this->setRowCount(1);
    this->setColumnCount(1);
    m_TreeMaterList.item = NULL;
    QString name = tr("用户材料");
    m_TreeMaterList.TreeListinit();
    m_TreeMaterList.item = CreateModelItem(NULL,name,false,true,"Project");
    this->setItem(0,0,m_TreeMaterList.item);

    //p_item = this->item(0,0);
    //item = CreateModelItem(p_item,tr("用户材料"),false,true,"DatafrdOpt");//0
    //item = CreateModelItem(p_item,tr("Plot Options"),false,true,"PlotOpt");//1
    //item = CreateModelItem(p_item,tr("Result Options"),false,true,"RestOpt");//2
    //item = CreateModelItem(p_item->child(2,0),tr("Read Results"),false,true,"ReadRest");

    //item = CreateModelItem(p_item,tr("Result output"),false,true,"RestOut");//3
    //item = CreateModelItem(p_item->child(3,0),tr("Plot Results"),false,true,"PlotRest");
    //item = CreateModelItem(p_item->child(3,0),tr("List Results"),false,true,"ListRest");
    //item = CreateModelItem(p_item->child(3,0),tr("Animate"),false,true,"Anim");
    //item = CreateModelItem(p_item->child(3,0),tr("Time history curve"),false,true,"TimHisCur");
    //item = CreateModelItem(p_item->child(3,0),tr("Re-meshing"),false,true,"RMesh");
    //item = CreateModelItem(p_item->child(3,0),tr("other"),false,true,"Other");
    //item->setEnabled(false);
}
//材料模块在根部进行文件的增加
void QTree_Model::AddMaterialUserName(QString name)
{
    if (m_TreeMaterList.name.contains(name,Qt::CaseSensitive))
    {
        //QMessageBox::warning(this,tr("Load File"),tr("File is in the Project"),0x00000400,0);
        // QMessageBox::StandardButton bt = QMessageBox::question(this, tr("Load File"), tr("File is in the Project"), QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);
        return;
    }
    m_TreeMaterList.name.append(name);
    m_TreeMaterList.nameAct.append(name);
    m_TreeMaterList.p_item =this->item(0,0);
    //m_TreeMaterList.item = CreateModelItem(m_TreeMaterList.p_item, name,false,true,"DatafrdOpt");//0
    m_TreeMaterList.item = CreateModelItem(m_TreeMaterList.p_item, name,false,true,name);//0
    m_TreeMaterList.rowNum++;
}
//材料模块进行文件的删除操作
//-return -1:最后一个文档
int QTree_Model::DelMaterialUserName(QString name)
{
    QString nameTmp;
    if (m_TreeMaterList.rowNum<=0)
    {
        return 0;
    }
   
   for (int i=0; i<m_TreeMaterList.rowNum; i++)
   {
        nameTmp=m_TreeMaterList.name.at(i).toLocal8Bit().constData();
        if (nameTmp == name)
        {
            m_TreeMaterList.rowNum--;
            m_TreeMaterList.name.removeAt(i);
            break;
        }
       
   }

    this->clear();
       
    m_TreeMaterList.item = CreateModelItem(NULL, tr("用户材料"),false,true,"Project");
    this->setItem(0,0,m_TreeMaterList.item);

    m_TreeMaterList.p_item =this->item(0,0);
    for (int i=0; i<m_TreeMaterList.rowNum; i++)
    {   
        name=m_TreeMaterList.name.at(i).toLocal8Bit().constData();
        m_TreeMaterList.item = CreateModelItem(m_TreeMaterList.p_item, name,false,true,name);//0
    }
    if (m_TreeMaterList.rowNum==0)
    {
        return -1;
    }
    return 0;
}

//--changeName
void QTree_Model::ChangeMaterialUserName(QString strOldName,QString strNewName ,QModelIndex &index)
{
	QString nameTmp;
	if (m_TreeMaterList.rowNum<=0)
	{
		return ;
	}

	for (int i=0; i<m_TreeMaterList.rowNum; i++)
	{
		nameTmp=m_TreeMaterList.name.at(i).toLocal8Bit().constData();
		if (nameTmp == strOldName)
		{
			//m_TreeMaterList.name.at(i).replace(strNewName);
			m_TreeMaterList.name.replace(i,strNewName);
           
			break;
		}

	}
	

	this->clear();

	m_TreeMaterList.item = CreateModelItem(NULL, tr("用户材料"),false,true,"Project");
	this->setItem(0,0,m_TreeMaterList.item);

	m_TreeMaterList.p_item =this->item(0,0);
	for (int i=0; i<m_TreeMaterList.rowNum; i++)
	{   
		strOldName=m_TreeMaterList.name.at(i).toLocal8Bit().constData();
		m_TreeMaterList.item = CreateModelItem(m_TreeMaterList.p_item, strOldName,false,true,strOldName);//0
	}
    index=m_TreeMaterList.item->index();
    QString strM;
    strM=index.data(Qt::UserRole+3).toString();
}
//--change language
void QTree_Model::UpDataWidgetLanguage(int iLanguage)
{
    QString name;
    int rows1 = this->rowCount();
    if (iLanguage==zh_cn)//中文
    {
        for (int i=0;i<rows1;i++)
        {
            QStandardItem* pItem = item(i,0);
            if (pItem==NULL)continue;
            int rows2 = pItem->rowCount();
            QStandardItem* childItem0= pItem;
            pItem->setText("后处理过程");
            for (int j=0;j<rows2;j++)
            {
                QStandardItem* childItem1 = pItem->child(j,0);
                if (childItem1==NULL)continue;
                name = childItem1->index().data(Qt::UserRole+3).toString();
                //QString LanguageName = getTreeItemLanguage(name);
                //childItem1->setText(LanguageName);
                if (name =="DatafrdOpt")childItem1->setText("数据选择");
                else if (name =="PlotOpt")childItem1->setText("绘制选择");
                else if (name =="RestOpt")childItem1->setText("结果选择");
                else if (name =="RestOut")childItem1->setText("结果输出");
                

                int rows3 = childItem1->rowCount();
                for (int k=0;k<rows3;k++)
                {
                    QStandardItem* childItem2 = childItem1->child(k,0);
                    if (childItem2==NULL)continue;
                    name = childItem2->index().data(Qt::UserRole+3).toString();
                    //QString LanguageName = getTreeItemLanguage(name);
                   // childItem2->setText(LanguageName);
                    if (name =="ReadRest")childItem2->setText("读取结果");
                    else if (name =="PlotRest")childItem2->setText("绘制结果");
                    else if (name =="ListRest")childItem2->setText("列表结果");
                    else if (name =="TimHisCur")childItem2->setText("历史曲线");
                    else if (name =="RMesh")childItem2->setText("网格重划");
                    else if (name =="Other")childItem2->setText("其他");
                }
            }
        }
    }else{
        for (int i=0;i<rows1;i++)
        {
            QStandardItem* pItem = item(i,0);
            if (pItem==NULL)continue;
            int rows2 = pItem->rowCount();
            QStandardItem* childItem0= pItem;
            pItem->setText("General Postproc");
            for (int j=0;j<rows2;j++)
            {
                QStandardItem* childItem1 = pItem->child(j,0);
                if (childItem1==NULL)continue;
                name = childItem1->index().data(Qt::UserRole+3).toString();
                //QString LanguageName = getTreeItemLanguage(name);
                //childItem1->setText(LanguageName);
                if (name =="DatafrdOpt")childItem1->setText("Data&File Opts");
                else if (name =="PlotOpt")childItem1->setText("Plot Options");
                else if (name =="RestOpt")childItem1->setText("Result Options");
                else if (name =="RestOut")childItem1->setText("Result output");
                int rows3 = childItem1->rowCount();
                for (int k=0;k<rows3;k++)
                {
                    QStandardItem* childItem2 = childItem1->child(k,0);
                    if (childItem2==NULL)continue;
                    name = childItem2->index().data(Qt::UserRole+3).toString();
                    //QString LanguageName = getTreeItemLanguage(name);
                    //childItem2->setText(LanguageName);
                    if (name =="ReadRest")childItem2->setText("Read Results");
                    else if (name =="PlotRest")childItem2->setText("Plot Results");
                    else if (name =="ListRest")childItem2->setText("List Results");
                    else if (name =="TimHisCur")childItem2->setText("Time history curve");
                    else if (name =="RMesh")childItem2->setText("Re-meshing");
                    else if (name =="Other")childItem2->setText("Other");
                }
            }
        }
    } 
   
}
//--将inp中数据加载到TREE中
void QTree_Model::setInpData(ReadInpResultS data)
{
    int rows1 = this->rowCount();
    QString name;
    for (int i=0;i<rows1;i++){
        QStandardItem* pItem = item(i,0);//root
        if (pItem==NULL)continue;
        int rows2 = pItem->rowCount();
        for (int j=0;j<rows2;j++){
            QStandardItem* childItem1 = pItem->child(j,0);//second
            if (childItem1==NULL)continue;
            name = childItem1->index().data(Qt::UserRole+3).toString();
            int rows3 = childItem1->rowCount();
            if (name == "AmpCurve"){//insert amplitude   
                childItem1->removeRows(0,rows3);//删除所有项，进行添加
                for (int kk=0; kk<data.TmpCurveInps.size(); kk++){
                    childItem1->setChild(kk,0,CreateModelItem(childItem1,data.TmpCurveInps.at(kk).strName,false,true,data.TmpCurveInps.at(kk).strName));
                }               
            }              
           /* else if (name =="PlotOpt")childItem1->setText("Plot Options");
            else if (name =="RestOpt")childItem1->setText("Result Options");
            else if (name =="RestOut")childItem1->setText("Result output");*/
        }
    }
}
//--增加一个树的节点
//--row:第几个Row
//--curRow:当前节点下
//void QTree_Model::addNodeTree(int row,int curRow)
//{
//
//}