#ifndef QTREE_MODEL_H
#define QTREE_MODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QString>
#include <QStandardItemModel>
#include "clinguist.h"
#include "CRWManage.h"

enum TREE_PAGE{
    M_TREE_POST=0,
    M_TREE_PRE=1,
    M_TREE_MATER=20
   
};
//--pre中的树
typedef struct _BaseItem_S_{
    int iDepth;//所处级别
    QStandardItem* item;//item
    QString name; //tree名字
    QString nameAct;//tree名字对应的action
    _BaseItem_S_(){
        iDepth=0;//0,则为根级别（1-第一级，2-第二级，3-第三级）
        item=NULL;
        name="";
        nameAct="";
    }
   /* bool operator < (const _BaseItem_S_&item)
    {
        return this->name<item.name&&item.iDepth&&item.nameAct&&item.item;
    }*/
}BaseTreeItem;
//曲线
typedef struct _WelCMN_CURVE_S_
{
    BaseTreeItem item;
    QMap<QString, InpCurveInpS> CurveData;
}WelCMN_curveS;
class WelCME_TreePreList
{
public:
    uint rowNum;
    uint colNum;
    
    BaseTreeItem itemRoot1;//根<一>item 模型
    WelCMN_curveS treeCurve;//曲线
public:
    //InsertItem();

public:
    void TreeListinit()
    {
       rowNum = 0;
        colNum = 0;
      
        //TreeBaseS.clear();
    }
};


//--材料中的树
class WelCME_MaterTreeList
{

public:
    uint rowNum;
    uint colNum;
    QStandardItem* item;//根item
    QStandardItem* p_item;//子item
    QStringList name; //tree名字
    QStringList nameAct;//tree名字对应的action
public:
    void TreeListinit()
    {
        rowNum = 0;
        colNum = 0;
        item = NULL;
        p_item = NULL;
        name.clear();
        nameAct.clear();
    }
};

//--
class QTree_Model : public QStandardItemModel
{
	Q_OBJECT

public:
	QTree_Model(QObject *parent=NULL);
    QTree_Model(QObject *parent, int ModeId);
	~QTree_Model();


public:

    int PreHIPRootNum;//前处理（目前热等静压）

    WelCME_MaterTreeList  m_TreeMaterList;
    QMap<BaseTreeItem, WelCME_TreePreList> m_TreePreList;
public:
	void CreatePostModelTree();
    void CreatePreModelTree();

    void CreateMaterTree();//材料中的tree
    void AddMaterialUserName(QString name);//增加材料模块tree用户列表
    int DelMaterialUserName(QString name);//删除材料模块tree用户列表
	void ChangeMaterialUserName(QString OldName,QString NewName,QModelIndex &);//删除材料模块tree用户列表

    QStandardItem* getTopParent(QStandardItem* item);
    void InsertRootPreModelTree();//增加根部节点
    void UpDataWidgetLanguage(int iLanguage);

    //--前处理树图
    void InsertCurveData();
    void setInpData(ReadInpResultS);
    QStandardItem* CreateModelItem(QStandardItem *itemp,QString itemText,bool editable,bool enabled,
        QString tree);

private:
   // QStandardItem* CreateModelItem(QStandardItem *itemp,QString itemText,bool editable,bool enabled,
    //    QString tree);
	
};

#endif // QTREE_MODEL_H
