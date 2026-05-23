#ifndef QThermalBoundaryDlg_H
#define QThermalBoundaryDlg_H

#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QComboBox>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QMouseEvent>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QHeaderView>
#include "ADefineQ.h"
#include "QMyCombox.h"
#include "CRWManage.h"
#include "QCurveDataPlot.h"
#include "Information_Widget.h"

typedef struct _SURF_FR_S0_{
    QStringList strName;//surf22,surf22,surf33,surf33,surf33;
    QStringList strNum;//2,3
    QStringList strInclude;//surf22_s1;surf22_s2;surf33_s1;surf33_s2;surf33_s3
    QStringList strIncludeSurfNum;//s1,s2,s3,s4,s5,s6;
    _SURF_FR_S0_(){
        strName.clear();
        strNum.clear();
        strInclude.clear();
        strIncludeSurfNum.clear();
    }
}SurfFRS0;

class QTBConst:public QDialog
{   
   Q_OBJECT   
public:
    QTBConst(QWidget *parent1=NULL);
    ~QTBConst();
public:
    QPushButton *m_OKBtn, *m_CancelBtn;//确定，取消
	QLineEdit *ConstMotionParameters;
	QString   CMPOutPut;//CMP:ConstMotionParameters
	void SetInpDataC(FSystem FSData);//加载时写入到界面中
    void WriteToFSystemInps();//输出到inp文件
public slots:
   void m_OKBtnS();
   void m_CancelBtnS();
};

class ThermalBoundary : public QDialog
{
    Q_OBJECT

public:
    ThermalBoundary(QWidget *parent);
    ~ThermalBoundary();

public:
    QPushButton *m_HBPreBtn, *m_HBNextBtn;
    QPushButton *m_DelBtn,*m_AddBtn,*m_ApplyBtn;
    QTableWidget* m_TableWiget;
   // QListWidget* m_ListWidget;

    //QVector<MyCombox*> m_comboxPSetSurf1;
    QMap<int,MyCombox*> m_comboxPSetSurf1;//对流表面集合
	QMap<int,MyCombox*> m_comboxPSetSurf2;//辐射表面集合
    QMap<int,MyCombox*> m_ComboxFilm;
    QMap<int,MyCombox*> m_ComboxRadiate;
	QMap<int,MyCombox*> m_AmbientTemperatureC;
	QMap<int,MyCombox*> m_AmbientTemperatureR;
	QMap<int,MyCombox*> m_ConvectionCoefficient;
	QMap<int,MyCombox*> m_RadiationCoefficient;
	QMap<int,QString>TB;

	QList<QTBConst*>  m_ATCurveDataListDlgC;//环境温度为常数
	QList<QCurveDataPlot*>  m_ATCurveDataListDlgC02;//环境温度随时间变化
	QList<QTBConst*>  m_ATCurveDataListDlgR;//辐射环境温度为常数
	QList<QCurveDataPlot*>  m_ATCurveDataListDlgR02;//辐射环境温度随时间变化
	QList<QTBConst*>  m_CCCurveDataListDlg;//对流系数为常数
	QList<QCurveDataPlot*>  m_CCCurveDataListDlg02;//对流系数随时间变化
	QList<QTBConst*>  m_RCCurveDataListDlg;//辐射系数为常数
	QList<QCurveDataPlot*>  m_RCCurveDataListDlg02;//辐射系数随温度变化
	QList<InpCurveInpS> m_CurveList;
public:
    //QList<InpCurveInpS>   m_nodeCurveList;
    QStringList           m_strListSurf;// S1,2,3,4,5,6-->用于获取到当前对应的面 
    SurfFRS0              m_surfFRS;
    QStringList           m_strListCurveName;//曲线数据combox
    QStringList           m_PSetList,m_ThermalBoundaryName2;//集合显示combox
	QVector<QString>      m_ThermalBoundaryName;//保存所有热边界名称
	QList<HPSystemFInpS>  m_OutPutTBFList;//保存对流输出信息
	QList<HPSystemRInpS>  m_OutPutTBRList;//保存辐射输出信息
public:
    int count01[2][20];//对流环境温度
	int IndexNumber01[40];//保存行索引号
	int count02[2][20];//对流边界
	int IndexNumber02[40];//保存行索引号
	int count03[2][20];//辐射环境温度
	int IndexNumber03[40];//保存行索引号
	int count04[2][20];//辐射边界
	int IndexNumber04[40];//保存行索引号
    /*QMap<QString ,QTemperateDialog*> m_TemperateDlg;
    QMap<QString ,QRadiateDialog*>m_RadiateDlg;
    QMap<QString ,QFilmDialog*>m_FilmDlg;*/
public:
    void SetInpDataHB(ReadInpResultS Data);//加载时写入到界面中
    void WriteHBToInp();//将界面上的写入到inp
signals:
	void emitPsetHBHighLight(NElSurfChsS);
	void emitHotName(m_TBName);
	void emitHotName2(QMap<int,QString>);
public slots:
        void AddBtnHBSlot();
        void DelBtnHBSlot();
        void ApplyHBSlot();
        void CancelHBSlot();
		void ConstantChangesSlot();
		void NextHBBtnSlot();
        void PreHBBtnSlot();
		void comboxSlot(int comWigId,int index0);
		void AppendSurfSetComboxTBSlot(QString);
};

#endif // QThermalBoundaryDlg_H