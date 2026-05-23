#ifndef QHPPARTDLG_H
#define QHPPARTDLG_H

#include <QObject>
#include <QDialog>
#include <QHeaderView>
#include "QMaterManagerDlg.h"
#include "QSectionManagerDlg.h"
#include "QMyCombox.h"
#include "Information_Widget.h"
#include "ADefineQ.h"
#include "InpDataSource.h"
#include <QRadioButton>
#include <QGroupBox>

enum ENUMPARTSET{
    ENUM_PART_Type=1,
    ENUM_PART_Material=2,
    ENUM_PART_ESET=3,
	ENUM_PART_TTTCCT=4,
};

class QHPPartDlg : public QDialog
{
    Q_OBJECT
public:
	int ENumberS;
	QList<QString>EleTy;
	QStringList elementList;	
public:
    QHPPartDlg(QWidget *parent = NULL);
    ~QHPPartDlg();

    QPushButton *m_ImprotPart,*m_NewSectionBtn,*m_DelSectionBtn;//导入,新建,删除
    QPushButton *m_ApplaySectionBtn;//应用
    QPushButton *m_NextSectionBtn;//下一步

    QMap<int, MyCombox*> m_comboxMaterial;
    QMap<int, MyCombox*> m_comboxElset;
	QMap<int, MyCombox*> m_comboxPartType;
    QTableWidget* m_TableWiget;
	QWidget *m_Tab1;
	QRadioButton *D3Model,*AxialSymmetry,*PlaneStress,*PlaneStrain;

    QSectionManagerDlg *m_SectionManageDlg;
    QMaterManagerDlg *m_MaterManagerDlg;

  
    QList<SectionInpS>  m_nodeSList;//
    QList<MaterialInpS> m_nodeMList;//
    NodeELSetInps m_NodeElSetData;//节点,单元等的内容
    QList<NodeELSetInps> m_MeshData;//节点,单元,表面集合
    HPTTTInps m_HpTTTDataInps;//TTT相关的材料内容

    //int style;
    //QList<HPTTTInps> m_nodeTTTList;//TTT相关的材料内容
public:
    void SetInpData(ReadInpResultS ReadInpData);
    void writeUnitPSetToInp(ReadInpResultS ReadInpData);
    void WritePartDataToInp();
    void ShowPartDlgStyle(int style);//不同工艺类型部件
private slots:
    void SHPPropSlot();//新增
    void DeletePropSlot();
    void ApplayPropSlot();//应用按钮
    void NextBtnSlot();
    void comBoxMChgSlot();
    void comBoxDoubleSlot();
    void ComboElSlot();
	void MergeSlot();
    void AppendElSetComboxSlot(QString str);
    void AppendPElData(QString str, int iStyle,DecodeGenerateS data);
	void AMOptions(bool);//Analyze model options
signals:
    void emitElsetHighLight(NElSurfChsS);
    void emitMerge();
};

#endif // QHIPPARTDLG_H
