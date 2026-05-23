#ifndef QHPSYSTEMDLG_H
#define QHPSYSTEMDLG_H


#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QListWidget>
#include <QComboBox>
#include <QMyCombox.h>
#include <QLineEdit>
#include <QLabel>
#include "qchartviewer.h"
#include "democharts.h"
#include "ADefineQ.h"
#include "CRWManage.h"
#include "QNewDialog.h"//使用曲线

typedef struct _SURF_FR_S_{
    QStringList strName;//surf22,surf22,surf33,surf33,surf33;
    QStringList strNum;//2,3
    QStringList strInclude;//surf22_s1;surf22_s2;surf33_s1;surf33_s2;surf33_s3
    QStringList strIncludeSurfNum;//s1,s2,s3,s4,s5,s6;
    _SURF_FR_S_(){
        strName.clear();
        strNum.clear();
        strInclude.clear();
        strIncludeSurfNum.clear();
    }
}SurfFRS;
enum _COMBOX_HPSYSTEMID_{
    COL_SYS_PSET = 0,
    COL_SYS_TEMPER=1,
    COL_SYS_FILM = 2,
    COL_SYS_RADIATE = 3,
    COL_SYS_SYSTEM=4,
};

class QTemperateDialog;
class QFilmDialog;
class QRadiateDialog;
class QDelSysDialog;

class QHPSystemDlg : public QDialog
{
    Q_OBJECT

public:
    QHPSystemDlg(QWidget *parent);
    ~QHPSystemDlg();

public:
    //QPushButton *m_OKBtn, *m_ApplyBtn, 
    QPushButton *m_DelBtn,*m_AddBtn,*m_InsertBtn,*m_NextBtn,*m_PreBtn;
    QTableWidget* m_TableWiget;
    QListWidget* m_ListWidget;

    //QVector<MyCombox*> m_comboxPSetSurf1;
    QMap<int,MyCombox*> m_comboxPSetSurf1;
    QMap<int,MyCombox*> m_ComboxTemper;
    QMap<int,MyCombox*> m_ComboxFilm;
    QMap<int,MyCombox*> m_ComboxRadiate;
    QMap<int,MyCombox*> m_ComboxSystem;
    //QList<QTCurveDataPlot*> m_CurveDataListDlg;
public:
    QList<HPSystemRInpS>  m_nodeHPSystemRList;//
    QList<HPSystemFInpS>  m_nodeHPSystemFList;//
    QList<InpCurveInpS>   m_nodeCurveList;
    QStringList           m_strListSurf;// S1,2,3,4,5,6-->用于获取到当前对应的面 
    SurfFRS m_surfFRS;
    QStringList           m_strListCurveName;//曲线数据combox
    QStringList           m_PSetList;//集合显示combox
    //QStringList           m_stepList;//step包含
    //QStringList           m_temperateList;//温度列表
    //QStringList           m_filmList;//对流列表
    //QStringList           m_radiateList;//辐射列表

    //QMap<int ,QTemperateDialog*>m_TemperateDlg;
    QMap<QString ,QTemperateDialog*> m_TemperateDlg;
    QMap<QString ,QRadiateDialog*>m_RadiateDlg;
    QMap<QString ,QFilmDialog*>m_FilmDlg;
private:
    QMenu *m_popMenu;
    QStringList m_strListHpSysStep;//求解器名字；
public:
    void SetInpData(ReadInpResultS Data);//加载时写入到界面中
    void WriteHPsysToInp();//将界面上的写入到inp
private:
    void AddHPSysLine(int nCurRow);
    void insertToCurveList(InpCurveInpS CurveData);
    void comboxAppendText(MyCombox* nowBtn, QString strName);
    void comboxChangeText(QString strOldName, QString strNewName);
signals:
        void emitPsetHighLight(NElSurfChsS);
        void emitHPsystemNext(int);//step有几步
        void emitHPCurve(QStringList,InpCurveInpS);//更新整个curve数据
private slots:
        //void ApplyHPSlot();
        void AddBtnHPSlot();
        void DelBtnHPSlot();
        void NextBtnHPSlot();
        void comboxSlot();
        void PreBtnHPSlot();
        void InsertBtnHPSlot();
        void DelRBtnSlot();
        void contextListWidgetMenuSlot(const QPoint &p);
        void comboxChangeSlot();
        void updateHPCurveDataSlot(QStringList,InpCurveInpS);//    
      

};

//--------------------------------------
//----------------  温度 ---------------
//--------------------------------------
class QTemperateDialog : public  QDialog{

    Q_OBJECT
public:
    QTemperateDialog(QWidget *parent);
    ~QTemperateDialog();
    QLabel *m_lab,*m_NameLab;
    QLineEdit *m_LineValue,*m_LineName;
    QPushButton *m_OKBtn,*m_CancelBtn;
    QTCurveDataPlot* m_TCurveDataPlot;

    void SetShowStyle(int iStyle);
    void setData(int,QString);
    void setData(int,InpCurveInpS);
    QString GetData(int style);
    InpCurveInpS GetData(int style,InpCurveInpS);
private:
 //   private slots:
public slots:
    void OkBtnSlot();

};
//--------------------------------------
//----------------  对流 ---------------
//--------------------------------------
class QFilmDialog : public  QDialog{

    Q_OBJECT
public:
    QFilmDialog(QWidget *parent);
    ~QFilmDialog();
    QLabel *m_lab,*m_NameLab;
    QLineEdit *m_LineValue,*m_LineName;
    QPushButton *m_OKBtn,*m_CancelBtn;
    QTCurveDataPlot* m_TCurveDataPlot;

    void SetShowStyle(int iStyle);
    void setData(int,QString);
    void setData(int,InpCurveInpS);
     QString GetData(int style);
     InpCurveInpS GetData(int style,InpCurveInpS data);
private:
    private slots:
        void OkBtnSlot();
       
};
//--------------------------------------
//----------------  辐射 ---------------
//--------------------------------------
class QRadiateDialog : public  QDialog{

    Q_OBJECT
public:
    QRadiateDialog(QWidget *parent);
    ~QRadiateDialog();
    QLabel *m_lab,*m_NameLab;
    QLineEdit *m_LineValue,*m_LineName;
    QPushButton *m_OKBtn,*m_CancelBtn;
    QTCurveDataPlot* m_TCurveDataPlot;

    void SetShowStyle(int iStyle);
    void setData(int,QString);
    void setData(int,InpCurveInpS);
    QString GetData(int style);
    InpCurveInpS GetData(int style,InpCurveInpS data);
private:
private slots:
        void OkBtnSlot();

};

#endif // QHPSYSTEMDLG_H
