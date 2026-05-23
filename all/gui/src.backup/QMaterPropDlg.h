#ifndef QMATERPROPDLG_H
#define QMATERPROPDLG_H
#include "ADefineQ.h"
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QListWidget>
#include <QComboBox>
#include <QDialog>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QTabBar>
#include "QTree_Model.h"
#include "QTreeMangerView.h"
#include "QMaterDataWig.h"
#include "CRWManage.h"
#include "QMaterNewDlg.h"
#include "QHPTTTDlg.h"
#include "QHPCCTDlg.h"


class QMaterPropDlg : public QDialog
{
	Q_OBJECT

public:
	QMaterPropDlg(QWidget *parent=NULL);
	~QMaterPropDlg();

//--控件
public:
	QLineEdit  *m_NameEdit;
	QPushButton *m_BtnApplay, *m_BtnCancel,*m_BtnImport,*m_BtnExport;
	QListWidget *m_ListWiget;
    QTableWidget *m_ListWiget2;//view形式显示
    QHPTTTDlg *m_WidTTT;
    QHPCCTDlg *m_WidCCT;
    

	//QPushButton* m_BtnDelList;
	QGroupBox*   m_gbGroup2;//界面改变
		
	//--Action & Menu
	//QAction *m_DensityAct, *m_ElasticAct, *m_PlasticAct, *m_ExpansionAct,* m_ConductivityAct;;
	//QAction *m_SpecificHeatAct,* m_MatelPowderAct;
     QAction *m_NewMaterialAct_;

	//QMenu*	 m_GeneralMenu;
	//QMenu*	 m_MechanicalMenu;
	//QMenu*	 m_TermalMenu;
    //QMenu*   m_MatelPowderMenu;//金属粉末
	//QMenu*   m_MaterMenu;
	//QMenu*   m_subMeEMenu;
	//QMenu*   m_subMePMenu;
	QMenuBar* m_BarMenu;
    QMenu*  m_NewMaterialMenu;//新建


    QTabWidget *tabMaterView_;
    QTabWidget *tabTreeView_;

    QTree_Model      *m_TreeModel;
    QTreeMangerView  *m_TreeView,*m_TreeView1;
    QMap<QString,QMaterNewDlg*> m_NewMap;
    

    int m_IndexMaterName;//material - 1(向ManagerDlg界面写入的名字)
    QString m_NowDelFileName;//需要删除的文件名
    QString m_NowShowFileName;//需要显示的文件名字

	QMaterDataWig *m_MaterDataWig;//材料定义
public:
    QString m_Name;
//--变量
private:

	QStringList m_comList;
    static QMaterPropDlg* instance;
//--
public:
	void CreatGUI();
	void SetData(SectionS para);
	void PutData(MaterialS &para);
	void ListShowSet(int index);
    void ImportInpMaterial(QList<MaterialInpS> MaterialInps);
    void ImportInpTTT(ReadInpResultS TTTMaterial);
    static QMaterPropDlg* GetInstance();
    //--
signals:
    //void emitMaterParam(MaterialS);
    void emitMaterParam(SectionS);
//--
public slots:
	void BtnApplaySlot();
	void BtnCancelSlot();
	void BtnImportDataSlot();
	void BtnExportDataSlot();

	void BtnDeletSlot();
	void MaterListSlot(int Row);//ListWidget得到点击的是第几行


	//void DensitySlot();
	//void ElasticSlot();
	//void PlasticSlot();
	//void ExpansionSlot();
	//void ConductSlot();
	//void SpecificSlot();
 //   void MatelPowderSlot();

    void UpDataMaterListSLot(QMap<int, MCompoment> );
    void contextMaterMenuSlot(const QPoint &p);
    void DelFileNameSlot();
    void ChangeModelIndexSlot(const QModelIndex & index);
    void DataTestSlot(int row,int col);
    void NewMatialDlgSlot();
    void EditMaterialSlot();
    QMap<int,PhaseBaseItem> ProcessMapData(QList<HPphaseBalanceInps> BalanceList);
private:
};

#endif // QMATERPROPDLG_H
