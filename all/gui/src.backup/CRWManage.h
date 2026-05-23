#ifndef CRWMANAGE_H
#define CRWMANAGE_H

#include "ADefineQ.h"
#include <QFile>
#include <QList>
#include <QTextStream>
#include <QTime>
#include <QSet>

typedef struct _READ_INP_S_{
    NodeInpS TmpNodeInpS;
    ELInpS TmpElInpS;
    NSetInpS TmpNsetInps;
    ElSetInpS TmpElSetInps;
    SurfaceInpS TmpSurfaceInps;
    QList<SectionInpS> TmpSectionInps;
    QList<MaterialInpS> TmpMaterialInps;
    QList<HIPSolveInpS> TmpSolveInps;
    QList<InpCurveInpS> TmpCurveInps;
    QList<HIPSystemInpS> TmpHIPSystemInps;
    QList<BCInpS> TmpBCInps;
    QList<InitInpS> TmpInitInps;
    QList<VariableInpS> TmpVariableInps;

    //--热处理
   // QList<HPSystemInpS> TmpHPSystemInps;
    QList<HPSystemFInpS> TmpHPSystemFInps;
    QList<HPSystemRInpS> TmpHPSystemRInps;
	QList<HPphaseCurveInps> TmpHPphaseCurveInps;
	QList<HPphaseBalanceInps> TmpHPphaseBalanceInps;
	QList<HPphaseCtrolInps>	TmpHPphaseCtrolInps;
	QList<HPincubatePeriodInps> TmpHPincubatePeriodInps;
	QList<HPphaseElateHeatInps> TmpHPphaseElateHeatInps;
    QList<HPphaseHardInps> TmpHPphaseHardInps;
	QList<HPPhasePropSpecInps> TmpHPPhasePropSpecInps;
	QList<HPPhasePropCondInps> TmpHPPhasePropCondInps;
	QList<HPPhaseYSInps> TmpHPPhaseYSInps;
	QList<HPPhaseGSInps> TmpHPPhaseGSInps;
	QList<HPPhaseZBFInps> TmpHPPhaseZBFInps;

	//接触
	QList<CPPInInp> TmpCPPInInp;
	QList<CPInInp> TmpCPInInp;
	QList<CPTieInp> TmpCPTieInp;
	//锻造制度
	QList<FSystem> TmpFSystem;
	//重力
	Gravity_Inp TmpGravityInps;

}ReadInpResultS;
//--
typedef struct _ELEMENT_ELSET_S_{
    short iStyle;//iStyle= (1:体,2:面)
    QString strTypeName;//体、面名字（C3D8,CPS4）
    QString strElsetName;//第三个可选参数名称elset；
}GmshElsetEl;

class CRWManage
{
public:
    CRWManage(void);
    ~CRWManage(void);

    ReadInpResultS m_ReadInpResult;
	//ReadInpResultS m_ReadInpResult02;//luo 用于保存合并inp文件信息
//--Write Inp File From GUI
	bool WritePartFile(QFile *file, NodeELSetInps m_NElData,QList<SectionInpS> m_nodeList);
    bool WriteUnitElInpFile(QFile *file, NodeELSetInps m_NElData,QList<SectionInpS> m_nodeList);
    bool WriteSectionInpFile(QFile *file, QList<SectionInpS> m_nodeList);
    bool WriteMaterialInpFile(QFile *file, QList<MaterialInpS> m_nodeList,QList<SectionInpS> m_nodeList1,QList<HPPhyInpS> m_nodeHPPhyList);
    bool WriteHIPSolveInpFile(QFile *file, QList<HIPSolveInpS> m_nodeList);
    bool WriteHIPCurveInpFile(QFile *file, QList<InpCurveInpS> m_nodeList);
    bool WriteHIPSystemInpFile(QFile *file, QList<HIPSystemInpS> m_nodeList);
    bool WriteInitInpFile(QFile *file, QList<InitInpS> m_nodeList);
    bool WriteVariableInpFile(QFile *file, QList<VariableInpS> m_nodeList);
    bool WriteBCInpFile(QFile *file, QList<BCInpS> m_nodeList);
    bool WriteResultOIntervalInpFile(QFile *file, QList<VariableInpS> m_VarList,QList<HIPSolveInpS> m_Solvelist);
    //--热处理中   
    bool WriteHPTTTCCT(QFile *file, HPTTTInps m_nodeList);
    bool WriteHPSolveVarInpFile(QFile *file, QList<VariableInpS> m_VarList,QList<HPSolveInpS> m_Solvelist, QList<HPPhyInpS> m_nodeHPPhyList);
    bool WriteHPSolveVarInpFile(QFile *file, QList<VariableInpS> m_VarList,QList<HPSolveInpS> m_Solvelist, 
		QList<HPPhyInpS> m_nodeHPPhyList,QList<HPSystemRInpS> m_nodeRList,QList<HPSystemFInpS> m_nodeFList,
		QList<HIPSystemInpS> m_nodeHIPList,QList<FSystem> m_OPFS,QList<BCInpS> m_nodeList,QList<HPSystemFInpS> m_nodeFList2,QList<HPSystemRInpS> m_nodeRList2);    
    //--Read Inp to show on GUI
    bool ReadSectionInpFile(QFile *file,QString);
	bool ReadSectionInpFile02(QFile *file,int NodeNumber,int ElementNumber,QString);
    //int ReadNextLine(QFile *file,QString &DataLine);
    // QList<SetionInpS*> m_SectionInp;
	bool WriteContactProperties(QFile *file,QList<OCIInpS> m_OPCS,QList<InpCurveInpS> m_nodeList);
	bool WriteUniformLoadInpFile(QFile *file,Gravity_Inp,QList<SectionInpS> m_nodeList);

private:   
    int ReadNextLine(QTextStream &TextStreamData,QString &DataLine);
    bool changeGmshToNormal(NodeELSetInps &saveData,GmshInpS &curData);
    int getCellType(QString strType);
    bool ELinclude(QString strType,QString strSrc, QString strDis, int &surf);
    int CalcSn(QList<int> indX,int Num,QString strType);
	QString sort(int number,QString Str);
	QString FaceNumber(QString,QString,QStringList);
	QString CalcSn2(int,QString,QString,QStringList);
	QMap<QString, QString> surfelement(QString strELType ,GmshInpS &curData,int number0,int kk);
};

#endif // CRWMANAGE_H
