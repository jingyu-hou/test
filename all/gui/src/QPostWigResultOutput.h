#ifndef QPOSTWIGRESULTOUTPUT_H
#define QPOSTWIGRESULTOUTPUT_H

#include <QWidget>
#include <QObject>
#include <QPushButton>
#include <QMenuBar>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMap>
#include <QLabel>
#include <QSpinBox>
#include <QSlider>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLineEdit>
#include <QColorDialog>
#include <QTabWidget>
#include <QComboBox>
#include "ADefineQ.h"
#include "PostProcess/FrdDataVIS.h"
typedef struct _S_RESULT_VISParam_S
{
    QString strName;//需要显示的数据名称
    int m_iStyle;//0 云图 2等值线
    int m_iClrScalar;//颜色等级
    double m_dTrans;//透明度
    int m_iOrigCurrentChg;//0:原始（形变前）/ 1:形变后
    bool bCurGrid;//1:CurrentGrid // 0: Don't Show;
    bool bContour;//0:云图/等值线不显示;1:云图/等值线显示
    bool bCutZoneVisible;//实体
    bool bCutVTKWidgetVisible;//切片框
    VTKColorS m_gridShowColor;
    double contourMin;
    double contourMax;
    bool bContourMinMax;
    QList<int> CutIdList;//总的截面数目
	//--动画播放
	int movieSet;//0->default(do nothing);1->start;2->updata;9->end;
	QString movieName;//动画名称
	int movieNameRate;//默认速率；
    _S_RESULT_VISParam_S()
    {
        strName="";
        m_iStyle=0;
        m_iClrScalar=13;
        m_dTrans=1.0;
        m_iOrigCurrentChg=1;
        bCurGrid =0;
        contourMin=0;
        contourMax=10;
        bContourMinMax=false;
        bContour=true;
        bCutZoneVisible=true;
        bCutVTKWidgetVisible=true;
        CutIdList.clear();
		movieSet=0;
		movieName="WelCMEtest.avi";
		movieNameRate=1;
    }
}ResultVisS;

/////////////////////////////////////////////////////////
//cutParamWidget
/////////////////////////////////////////////////////////
class vtkObject;

class CutParamWidget : public QWidget
{
    Q_OBJECT

public:
    CutParamWidget(int cutId, QWidget *parent = 0);
    ~CutParamWidget() {;}//{ delete ui; }

public:
    QGroupBox *m_gbBox0,*m_gbBox1,*m_gbBox2,*m_gbBox3;//
    QCheckBox *m_ZoneVisible,*m_OutLineVisible;
    QPushButton *m_NormalXBtn,*m_NormalYBtn,*m_NormalZBtn;
    QLineEdit *m_LineEditNormalX,*m_LineEditNormalY,*m_LineEditNormalZ;
    QLineEdit *m_LineEditPosX,*m_LineEditPosY,*m_LineEditPosZ;
    QSlider *m_SliderX,*m_SliderY,*m_SliderZ;
    QPushButton *m_ContourBtn,*m_ContourLineBtn;
    QLabel *m_CoutourClrLab;
    QSpinBox *m_CoutourLevelSpinB;

	QStringList ActorListId_;
private:

public:

    static void CallBack_ChangeCutUIValues(vtkObject *caller, unsigned long, void *clientdata, void*);

    void InitCutZones();
    void InitCutContourVariables(const QStringList &scalars);
    void InitBoundsValue(double *bounds);
    void SetCutPositionWidgetValue(double *xyz);

signals:
    void CutZoneStateChanged(int cutId, bool isShow);
    void CutVTKWidgetStateChanged(int cutId, bool isShow);
    void CutNormalChanged(int cutId, double x, double y, double z);
    void CutPositionChanged(int cutId, double x, double y, double z);
    void CutContourStateChanged(int cutId, bool isShow, int type);
    void CutValueStateChanged(int cutId, bool isShow);
    void CutContourVariableChanged(int cutId, const QString &scalar);
    void CutContourLevelChanged(int cutId, int level);

    public slots:

    private slots:
        void ChangeCutZoneState(bool isShow);
        void ChangeCutVTKWidgetState(bool isShow);
        void ChangeCutNormal(int);
        void ChangeCutNormal2();
        void MoveCut(int value);
        void ChangeCutPosition();
        void ChangeCutContourState();
        //void ChangeCutValueState(bool isShow);
        void ChangeCutContourType(int type);
        void ChangeCutContourVariable(const QString &scalar);
        void ChangeCutContourLevel(int level);
        void ChkSectionContourSlot();

private:
    const int cutId_;
    double bounds_[6];
};


//

class QPostWigResultOutput : public QWidget
{
	Q_OBJECT

public:
	QPostWigResultOutput(QWidget *parent);
	~QPostWigResultOutput();
	

public:
    QGroupBox *m_gbBox0,*m_gbBox1,*m_gbBox11,*m_gbBox12,*m_gbBox3;
	QPushButton *m_BtNephogram;//云图"0"
	QPushButton	*m_BtEqui;//等直线"2"
    QLineEdit *m_CounterMin;//云图设置
    QLineEdit *m_CounterMax;
    QSpinBox *m_SpinBoxClr;//color Level
    QSlider *m_SliderTranc;//透明度
    QCheckBox *m_GridShow;//网格变形前后显示；
    QPushButton *m_GridColorBtn;//网格颜色
    //QRadioButton *m_CurChgShow;//形变后
    //QRadioButton *m_OrigChgShow;//形变前

    QPushButton *m_SectionBtn,*m_SectionAddBtn,*m_SectionDelBtn;//剖面
	QCheckBox *m_InsideOut;//内/外切割
    QCheckBox *m_Undeformed;//无变形；==1 orig（无变形）
                                    //==0 current(有变形)

    int m_BtIndex;//0 云图 2等值线
    ResultVisS m_VisParam;
    QMap<int, CutParamWidget*> m_CutWidgetMap;//剖面
    FrdDataVIS *frdVISObj_;

    void upDateScalar(QString scalar);
    QString m_Var;
    int cutIdCnt;

	void setActorList(QStringList);
private:
    QStringList activeActorList() const;
    QTabWidget *m_tabView;
	QStringList ActorListId_;
signals:
    void emitScalar(ResultVisS ResultVis);
public slots:
    void InitSectionDataSlot(FrdDataVIS *frdObj);
private slots:
    void BtnEquiSlot();
    void BtnNephogramSlot();
    void BtnSectionSlot();
    void SpinBoxClrSlot(int iVal);
    void SliderTrancSlot(int dVal);
    void SetChgShowSlot();
    void ChkGridSlot();
    void gridColorBtnSlot();
    void BtnSectionAddSlot();
    void BtnSectionDelSlot();
    void SectionNormalXYSlot(int cutId,double x,double y,double z);
    void SectionPosSlot(int cutId,double x,double y, double z);
    void SectionOutLineSlot(int cutId,bool bShow);
    void SectionZoneSlot(int cutId,bool bShow);
    void SectionShowSlot(int cutId,bool bShow, int type);
    void SectionLevelSlot(int cutId,int level);
    
};
#endif // QPOSTWIGRESULTOUTPUT_H
