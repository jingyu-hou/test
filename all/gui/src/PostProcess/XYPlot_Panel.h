#ifndef XYPLOT_PANEL
#define XYPLOT_PANEL
/**
* @file
*/
class QComboBox;
class QToolButton;
class QPushButton;
class FrdDataVIS;
class vtkCallbackCommand;
class vtkObject;
#include <QWidget>
#include "XYPlotVIS.h"
#include "XYPlot_RenderDialog.h"
#include <QMap>
#include <QButtonGroup>
#include <vtkActor.h>
#include <QGroupBox>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QTableWidget>
#include <QCheckBox>
#include "ADefineQ.h"
#include "QFrdDataPro.h"
#include "vtkSmartPointer.h"
typedef struct _DATA_XYPLOT_S 
{
    int selectedPointId_;
    double selectedPointX_;
    double selectedPointY_;
    double selectedPointZ_;
}SelectedPointS;
//class XYPlot_Panel : public QWidget
class XYPlot_Panel : public QDialog
{
    Q_OBJECT
public:
    XYPlot_Panel(QWidget *parent = 0);
    ~XYPlot_Panel();
    static void Callback_PickPoint(vtkObject *caller, unsigned long, void *clientdata, void*);
    void ReleaseCurrentPointPick();
    void InitPoint(double p,VTKColorS color);
    void UpDataComb(int ,QString);
    FrdDataVIS *frdVISObj_;
	resultDatS DatObj_;
    int m_CombIndex;//拾取 or del

    double m_pointSize;//拾取点大小；
    VTKColorS m_pointColor;//拾取点颜色；
   
private:
    void UpdataSelectedPoint(vtkActor *actor,QString str,int pointId, double x, double y, double z);
    void RemoveSelectedPoint(vtkActor *actor,int pointId, double x, double y, double z);
    bool clearAllDisp();
    void AddTableData(SelectedPointS data);
    void CreateXYPositionPlot();
    void CreateXYTimePlot();
	void CreateXYPressPlot();
public slots:
    void InitPlotData(FrdDataVIS *frdObj);
	void InitPlotDatData(resultDatS DatObj);
private slots:
    void SwitchPointPick(bool);
    void CreateXYPlot();
    void ChangeCombStyleSlot(int curId);
    void ChangeCombCurveSlot(int curId);
    void SaveDataSlot();
    void PointClrBtnSlot();
    void PointApplyBtnSlot();
    
private:
    //FrdDataVIS *frdVISObj_;
    XYPlot plot_;
    int selectedPointId_;
    //QComboBox *varComb_,*varPositionComb_;
    QToolButton *pickPointBT_;
    QPushButton *createPlotBT_;
    QComboBox *PickStyleComb_;//del or Add;
    QComboBox *PickCurveComb_;//时间曲线 or 位置曲线;
	QComboBox *SetNameComb_;//集合名称
	QList<QString> infNase;

    //--点设置
    QGroupBox *m_gbPoint;
    QLabel *m_pointSizeLab;
    QLineEdit *m_pointSizeEdit;
    QPushButton *m_pointClrBtn;
    QPushButton *m_PointApplyBtn;
    QCheckBox *m_PointLabChk;
    //--时间、位置历史曲线
    QButtonGroup *m_ChgxyzShow;
    QGroupBox *m_gbHisCurve;
    QTableWidget *m_TableWiget;
    QRadioButton *m_xRbtn,*m_yRbtn,*m_zRbtn,*m_disRbtn;
    XYPlot_RenderDialog *m_HisCurveDlg;//::GetInstance()
    
    vtkCallbackCommand *pointPickCallback_;
    QMap<int, SelectedPointS> m_SelectedPointS;
    //--combox updata
    QString m_varPositionComb,m_varCombParam;
};
#endif

