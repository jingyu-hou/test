#ifndef QPOSTPRC_H
#define QPOSTPRC_H
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QHeaderView>
#include <QStackedWidget>
#include <QXmlStreamReader>
#include <QtXml>
#include <QMessageBox>
#include "QPostWigResultOutput.h"
#include "QTreeMangerView.h"
#include "QPostWigFile.h" 
#include "QPostWigReadResultDlg.h"
#include "QPostWidPlotOptDlg.h"
#include "./PostProcess/FrdDataVIS.h"
#include "QStepPlayWidget.h"
#include "QCfgFileManage.h"
#include "./PostProcess/XYPlot_Panel.h"

class XYPlot_Panel;
//--
class QPostPrc : public QWidget//QDockWidget
{
	Q_OBJECT

public:
	QPostPrc(QWidget *parent=NULL);//(QDockWidget *parent=NULL);
	~QPostPrc();
public:
	QTreeMangerView *m_TreeView ;
	QTree_Model		*m_TreeModel;
	QPostWigFile	*m_PosWigFile;
	QPostWigResultOutput *m_PostWigResultOut;
	QPostWigReadResultDlg *m_PosWigReadResultDlg;
	QPostWidPlotOptDlg *m_PostWidPlotOptDlg;
     XYPlot_Panel *xyplotPanel_;
    
	QString			currentTreeNodeType_;
	QStackedWidget  *m_StackedWidget;
    QCfgFileManage  *m_CfgFile;
    VTKColorS m_WHColor,m_FSColor,m_CSColor,m_HisPointClr;
    double m_PointSizeFactor;
    vector<int> m_ActorListData;
    vector<int> m_WholeActorData;
public:
    QString m_FileScalarName;
	void setStyle();
	void LoadConfigFile(QString FilePath);
	void ParseEntry(QXmlStreamReader &Reader);
	int chgTheNameToInt(QStringRef strRef);
    void SetOptionS(OptDlgS, const vector<int>*);
  
private:
    VTKColorS ChangeColorToDouble(QString str);
    
public slots:
	void ChangeModelIndexSlot(const QModelIndex & index);
	void UpDataVisSlot(OptDlgS m_optDlgs);
    void UpVTKTextVisSlot(ReadResultDlgS);
	//void ShowTheTreeItemSlot();
    void UpDataMenuListSlot(const map<QString, QStringList> *);
    void UpDataScalar(ResultVisS param);
    void SetActorSlot(vector<int> dataId);
    //--∆ √Ê
    void SectionCutAddSlot(int CutId);
signals:
    void emitStepPlayMenu(ResultOutputS);
   


private:
	OptDlgS m_optDlgs;//
    ReadResultDlgS m_readReDlg;
   
};

#endif // QBACKWARD_H

