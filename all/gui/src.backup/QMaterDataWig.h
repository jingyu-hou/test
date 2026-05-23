#ifndef QMATERDATAWIG_H
#define QMATERDATAWIG_H
#include "ADefineQ.h"
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QTableWidget>
#include <QCheckBox>
#include <QMap>
#include <QHeaderView>
#include <QFile>
#include <QFileDialog>
//#include <QMessageBox>
#include <QtXml/QtXml>
#include <QtXml/QDomComment>
#include <QLineEdit>

#include "qchartviewer.h"
#include "democharts.h"


//--chart some
const int noOfChartViewers = 8;
//end chart some
#define  TYPENUM 10

typedef struct BaseItem
{
    QLabel* label;
    QLineEdit* edit;
    
    BaseItem()
    {
        label=NULL;
        edit=NULL;
    }
}BaseItem;
typedef struct _S_MDATALISTS_S_ 
{
	QString strName;
    QString strStyleParam;
	QString strStyleParam1;
	QString Data1;
	QString Data2;
	QString TempData;
	_S_MDATALISTS_S_()
	{
		strName="";
		Data1="";
		Data2="";
		TempData="";
        strStyleParam="";
		strStyleParam1="";
	}
	void clear(){
		strName="";
		Data1="";
		Data2="";
		TempData="";
		strStyleParam="";
		strStyleParam1="";
	}
}MdataList,*pMdataList;

typedef struct _S_MCELL_S_ 
{
    QString strName;
    QString Data;
    _S_MCELL_S_()
    {
        strName="";
        Data="";
    }
}MCell,*pMCell;

typedef struct _S_MATERCOMPS_S_
{
	int id;//pageIndex:0,1,2,3, m_enum_desity
    QString FileName;
    QString strStyle;//"density,elastic..."
    QString strStyleParam;
	QString strStyleParam1;
    int dataColNum;
    int dataRowNum;
    int dataColNum2;
    int dataRowNum2;
    int PageIndex;
	vector<MCell> data;
    bool bTemper;
    _S_MATERCOMPS_S_()
    {
        dataColNum=0;
        dataRowNum=0;
        PageIndex=0;
        dataColNum2=0;
        dataRowNum2=0;
        strStyleParam="";
		strStyleParam1="";
        bTemper = false;
    }
}MCompoment,*pMCompoment;

class QMaterDataWig : public QWidget
{
	Q_OBJECT

public:
	QMaterDataWig(QWidget *parent);
	~QMaterDataWig();

public:
	MdataList m_DataList[TYPENUM];
	int  PageIndex;
	QMap <int,MCompoment> Mdata;
    QMap<QString,QMap <int,MCompoment> > MFileData;
	QMap<int,QMap <int,MCompoment> > MPhaseTC;
	QMap<int,QMap <int,MCompoment> > MPhaseParameters;
    MCompoment tmpMcmp;
public:
    QWidget *m_Widge, *m_Widge1;
	QHBoxLayout *m_Hlayout1;
	QGridLayout *m_Gridlayout1;
    QHBoxLayout *m_mainLayout,*m_Hlayout;
	QTableWidget *m_TableWidget[TYPENUM];
    QTableWidget *m_widMetalPowderParam ;
	//QCheckBox *m_CheckTemp[TYPENUM];
	//QPushButton	*m_AddBtn[TYPENUM], *m_DelBtn[TYPENUM], *m_InsertBtn[TYPENUM];


    QTabWidget *m_tabWidMetalPowder;
    //

    QChartViewer *chartViewers[noOfChartViewers];

    double *dataTmpY ;
    QString *dataTmpX;
    QString *dataTmpT;
    QString m_NowFileName;

    //--
    BaseItem m_EditLabExpandZero,m_DamageModetype,m_RDPModetype;
    
public:
	void setTabWidgetHeader(int showIndex);
	void HideALL();
	void SetTabWidgetData(int);
    void ParseEntry(QXmlStreamReader &reader);
    int  chgTheNameToInt(QString strRef);
    bool ImportData();
    void ExportData();
    void ParamGet(int,QXmlStreamReader&);
    void WriteImportDataToMFile(QString m_NowFileName, QMap<int,MCompoment> Mdata);
    //--
    void flowLayoutCharts();
    void ChartShow2XY(StringArray dataX, vector<DoubleArray> dataY, const char *imageMap);
    void ChartShow2XY(StringArray dataX, vector<DoubleArray> dataY, const char *imageMap,_WELCME_CHART_PROP_ prop);
    void ChartShowDoubleY(StringArray dataX, vector<DoubleArray> dataY, const char *imageMap,_WELCME_CHART_PROP_ prop);

signals:
    void emitMaterList(QMap<int, MCompoment>);//MdataList *dataList);
public slots:
    void ChgWidgetSlot(int ShowIndex,QString strName,QString strfileName);
private:
	
};

#endif // QMATERDATAWIG_H
