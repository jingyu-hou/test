#ifndef QSTEPPLAYWIDGET_H
#define QSTEPPLAYWIDGET_H
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <QCheckBox>
#include <QGridLayout>
#include <QButtonGroup>
#include <QGroupBox>
#include <QTimer>
#include "ADefineQ.h"
#include "QPostWigResultOutput.h"


class QPlaySetDlg : public QDialog
{
	Q_OBJECT

public:
	QPlaySetDlg(QWidget *parent=NULL);
	~QPlaySetDlg(){;};
	
public:
	QPushButton *OKSetBtn_;
	QLineEdit *timeVal_;
	QLineEdit *movieName_;
	//QCheckBox *movieChk_;
	//QSlider *SliderRate_;
	//QLabel *rateLab_;
	void SetTimeValue(QString strV);
	static QPlaySetDlg* GetInstance();
	QString GetData();

private slots: 
	void OKSetBtnSlot();
	//void SliderMovieRateSlot(int);
public:
	static QPlaySetDlg* instance;
	//void BtnTimeSetSlot()
};



//--播放条
enum StepPlayENUM{
    Step_ENUM_NULL=0,
    Step_ENUM_1ST,
    Step_ENUM_PRE,
    Step_ENUM_PLAY,
    Step_ENUM_PAUSE,
    Step_ENUM_NEXT,
    Step_ENUM_LAST,  
	Step_ENUM_MOVIE,
};

typedef struct _S_STEPPLAY_VISParam_S
{
    QString strName;//需要显示的数据名称
    int m_iStyle;//0 云图 1等值线
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
	//bool bMovie;//movie set record or not;
	int movieSet;//0->default(do nothing);1->start;2->updata;9->end;
	QString movieName;//
	int movieNameRate;//默认速率；
	//--
    _S_STEPPLAY_VISParam_S()
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
		//bMovie=0;//false
		movieSet=0;
		movieName="WelCMEtest.avi";
		movieNameRate=15;
    }
}StepPlayVisS;
class QStepPlayWidget : public QWidget
{
    Q_OBJECT

public:
    static QStepPlayWidget* GetInstance();
    QStepPlayWidget(QWidget *parent=NULL);
    ~QStepPlayWidget();
    //--step L101-DISP-D1
public:
    QComboBox *m_ComBoParam01,*m_ComBoParam02,*m_ComBoParam03;
    QButtonGroup *m_PlayGroupBtn;
    QPushButton *StepPreBtn,*StepNextBtn,*Step1stBtn,*StepLastBtn,*StepPlayBtn,*StepPauseBtn,*SetPlayBtn,*MovieBtn;
    QPushButton *StepNowBtn;
    ResultOutputS m_ResultO;
    StepPlayVisS m_StepPlayParam;
    QTimer *m_timer;
	int TValue_;
    void updataCombox(ResultOutputS);
    void updataParam(StepPlayVisS mParam);
private:
    void SetPlayParam();
private slots: 
    void GroupBtnSlot(int);
    void ComboxParam01Slot(int);
    void ComboxParam02Slot(int id);
    void AutoPlaySlot();
    void NowOKBtnSlot();
	void SetUpBtnSlot();
signals:
    void emitPlayStepParam(StepPlayVisS ResultVis);
	
private:
    static QStepPlayWidget* instance;
};

#endif // QSTEPPLAYWIDGET_H
