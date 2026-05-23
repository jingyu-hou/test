#ifndef QPOSTWIDPLOTOPT_H
#define QPOSTWIDPLOTOPT_H

#include <QObject>
#include <QPushButton>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QRadioButton>
#include <QGroupBox>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QColorDialog>
#include <QButtonGroup>
#include <QComboBox>
#include <QCheckBox>
#include <QCoreApplication>
#include <QXmlStreamWriter>
#include <QFile>
#include "ADefineQ.h"


//--Plot Option 
class QPostWidPlotOpt_Basic;
class QPostWidPlotOpt_Color;
class QPostWidPlotOpt_Labels;
class QPostWidPlotOpt_Normals;
class QPostWidPlotOpt_Other;


typedef struct _OptDlg_S{
    int BaseRenderIndex; //BaseIndex
    int BaseScaleInder;
    int BaseVisIndex;
    float BaseValue;

    QString ClrEdgesWHPlot;//Color
    QString ClrEdgesFSPlot;
    QString ClrEdgesCSPLot;
    QString ClrHisCuvPoint;
    QString ClrHisPointfactor;//拾取点大小控制
    int ClrEdgeAStyIndex;
    int ClrEdgeAThikIndex;

    int LabElemChk;//Labels
    int LabFaceChk;
    int LabNodeChk;
    int LabNodSymbolChk;
    QString LabClrShowElem;//
    QString LabClrShowFace;
    QString LabClrShowNode;
    QString LabClrShowNodeSymb;
    int LabNodSymbolIndex;
    int LabNodSizeIndex;

    int NormShowChk;		//Normals
    int NormRadioElemOrSur;
    QString NormClrFacePlot;
    QString NormClrBeamTangPlot;
    QString NormClrBeamN1Plot;
    QString NormClrBeamN2Plot;
    int NormStyLenIndex;
    int NormStyLinThickIndex;
    int NormStyArrowHeadIndex;

    _OptDlg_S(){
        BaseRenderIndex=0; //BaseIndex
        BaseScaleInder=0;
        BaseVisIndex=0;
        BaseValue=0;

        ClrEdgesWHPlot="rgb(0,0,0)";//Color
        ClrEdgesFSPlot="rgb(0,0,0)";
        ClrEdgesCSPLot="rgb(0,0,0)";
        ClrHisCuvPoint="rgb(1,0,0)";
        ClrEdgeAStyIndex=0;
        ClrEdgeAThikIndex=0;
        ClrHisPointfactor="0.02";

        LabElemChk=0;//Labels
        LabFaceChk=0;
        LabNodeChk=0;
        LabNodSymbolChk=0;
        LabClrShowElem="rgb(0,0,0)";//
        LabClrShowFace="rgb(0,0,0)";
        LabClrShowNode="rgb(0,0,0)";
        LabClrShowNodeSymb="rgb(0,0,0)";
        LabNodSymbolIndex=0;
        LabNodSizeIndex=0;

        NormShowChk=0;		//Normals
        NormRadioElemOrSur=0;
        NormClrFacePlot="rgb(0,0,0)";
        NormClrBeamTangPlot="rgb(0,0,0)";
        NormClrBeamN1Plot="rgb(0,0,0)";
        NormClrBeamN2Plot="rgb(0,0,0)";
        NormStyLenIndex=0;
        NormStyLinThickIndex=0;
        NormStyArrowHeadIndex=0;
    }
}OptDlgS;

class QPostWidPlotOptDlg : public QDialog
{
    Q_OBJECT

public:
    QPostWidPlotOptDlg(QWidget *parent=NULL);
    ~QPostWidPlotOptDlg();

public:
    QPostWidPlotOpt_Basic *TabPage1;
    QPostWidPlotOpt_Color *TabPage2;
    QPostWidPlotOpt_Labels *TabPage3;
    QPostWidPlotOpt_Normals *TabPage4;
    QPostWidPlotOpt_Other	*TabPage5;
    QPushButton *m_BtnOK,*m_BtnCancel,*m_BtnApply,*m_BtnDefaults;
    OptDlgS m_OptDlgS;
    void SetData(OptDlgS Data);
    void PutData();
    void ExportData();

private:
    QTabWidget *m_TabWig;
public slots:
	void BtnCancelSlot();
	void BtnOkSlot();
	void BtnDefaultsSlot();
	void BtnApplySlot();
signals:
	void emitParam(OptDlgS m_OptDlgS);

};

//--PlotOption _Basic
class QPostWidPlotOpt_Basic:public QWidget
{
    Q_OBJECT
public:
    QPostWidPlotOpt_Basic(QWidget *parent);
    ~QPostWidPlotOpt_Basic();
public:
	int m_RenderIndex,m_ScaleIndex,m_VisibleIndex;
	float m_BaseValue;
	void setDefault();
	void setData();
private:
    QButtonGroup *m_btnGrpRender,*m_btnGrpScale,*m_btnGrpVisible;
	QRadioButton *m_RadioRender[6],*m_RadioScale[3],*m_RadioVisible[5];
				//QRadioButton *RwireFrame,*RHidden,*RFilled,*RShaded;
    QLabel    *m_valLab;
    QLineEdit *m_valEdit;
private slots:
	void setRenderSlot(int Index);
	void setScaleSlot(int Index);
	void setVisibleSlot(int Index);
};

//--PlotOption _Color & style
class QPostWidPlotOpt_Color:public QWidget
{
    Q_OBJECT
public:
    QPostWidPlotOpt_Color(QWidget *parent);
    ~QPostWidPlotOpt_Color();
public:
    QString m_ClrEdgeWHPlot,m_ClrEdgeFSOPlot,m_ClrEdgeCFSPlot,m_ClrHisCuvPonit;
    QString m_ClrHisCuvPonitSize;
	int m_ClrEdgeAStyIndex,m_ClrEdgeAThikIndex;
	void setDefault();
	void setData();

	QButtonGroup	*m_btnGroup;
	QPushButton		*m_ColorBt[4];
	QComboBox	*m_comboxSty;
	QComboBox	*m_comboxThick;
    QLineEdit *m_EditPointSize;
private:
	//QTabWidget *m_TabWig;
	QLabel *m_valLab;
	QLineEdit *m_valEdit;
	QString ColorToString(const QColor &color, const QString type);
private slots:
	void setColorScale(int btnId);
};

//--PlotOption _ Labels
class QPostWidPlotOpt_Labels:public QWidget
{
    Q_OBJECT
public:
    QPostWidPlotOpt_Labels(QWidget *parent);
    ~QPostWidPlotOpt_Labels();
public:
    QString m_LabClrShowElem,m_LabClrShowFace,m_LabClrShowNode,m_LabClrShowNodeSymb;
    int m_LabShowElemChk,m_LabShowFaceChk,m_LabShowNodeChk,m_LabShowNodeSymbChk;
    int  m_symbolsStyIndex,m_symbolsSizeIndex;

	void setDefault();
	void setData();

    QButtonGroup	*m_btnGroup;
    QPushButton		*m_ColorBt[4];
	QComboBox	*m_comboxSymbol,*m_comboxSize;
	QCheckBox *m_chk1,*m_chk2,*m_chk3,*m_chk4;
	QLabel *m_LabSymbol,*m_LabSize,*m_Lab1,*m_Lab2,*m_Lab3,*m_Lab4;
private:
	QString ColorToString(const QColor &color, const QString type);
private slots:
	void setColorScaleSlot(int btnId);
	void ChkShowNodeSysSlot();//
	void ChkShowElemLabSlot();
	void ChkShowFaceLabSlot();
	void ChkShowNodeLabSlot();
};

//--PlotOption _Normals
class QPostWidPlotOpt_Normals:public QWidget
{
	Q_OBJECT
public:
	QPostWidPlotOpt_Normals(QWidget *parent);
	~QPostWidPlotOpt_Normals();
public:
    int m_NormShowChk,m_NormRadioElemOrSur;		//Normals
    int m_NormStyLenIndex,m_NormStyLinThickIndex,m_NormStyArrowHeadIndex;
    QString m_NormClrFacePlot,m_NormClrBeamTangPlot,m_NormClrBeamN1Plot,m_NormClrBeamN2Plot;

    void setDefault();
    void setData();

    QGroupBox *m_gbBox1,*m_gbBox2; 
    QPushButton	*m_ColorBt[4];
    QCheckBox *m_chkShowNom;
    QRadioButton *m_RadioOnElem,*m_RadioOnSur;
    QLabel *m_Lab1,*m_Lab2,*m_Lab3,*m_Lab4,
		   *m_LabSty1,*m_LabSty2,*m_LabSty3,*m_LabSty4;
    QComboBox *m_comboxLen ,*m_comboxThick,*m_comboxArrowHead;
private:
	QString ColorToString(const QColor &color, const QString type);
private slots:
	void setColorScaleSlot(int btnId);
	void ChkShowNomSlot();
	void setVisibleSlot(int Index);
};

//--PlotOption _Other
class QPostWidPlotOpt_Other:public QWidget
{
	Q_OBJECT
public:
	QPostWidPlotOpt_Other(QWidget *parent);
	~QPostWidPlotOpt_Other();
public:
	void setDefault();
	void setData();
	QLabel *m_valLab;
	QLineEdit *m_valEdit;
private:
	//QTabWidget *m_TabWig;
};


#endif // QPOSTWIDPLOTOPT_H
