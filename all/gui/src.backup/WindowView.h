#ifndef WINDOWVIEW_H
#define WINDOWVIEW_H
//#include <QVTKWidget.h>
#include <QMainWindow>
#include <QPushButton>
#include <QApplication>
#include <QHBoxLayout>
#include <QCloseEvent>
#include <QVTKWidget.h>
#include "CRWManage.h"
#include "InpDataVIS.h"


#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <vtkUnstructuredGrid.h>
#include <vtkTriangle.h>
#include <vtkTetra.h>
class vtkTriangle;
class QVTKWidget;
class vtkRenderWindow;
class vtkRenderer;
class vtkCamera;
class vtkUnstructuredGrid;
class vtkTetra;
//class vtkVISOrientationMarker;
//class IV;


//class tabW:public QTabWidget
//{
//	  Q_OBJECT
//public:
//	explicit tabW(QWidget*p=NULL){};
//protected:
//	virtual void mousePressEvent(QMouseEvent *e)
//	{
//		if (e->button() == Qt::LeftButton)
//		{
//			;
//		}
//	}
//
//};
///////
class QWindowView : public QWidget//QDockWidget//QMainWindow
{
    Q_OBJECT

public:
    QWindowView(QWidget *parent = NULL);
    QWindowView(QWidget *parent = NULL, int index=0);
    ~QWindowView(void);


    //QList<QTabWidget*> tabView_;
    QTabWidget* tabView_;
    QVTKWidget* viewPostVTK_;
    QVTKWidget* viewPreVTK_;
   
    int m_CurrentIndex;
    void ShowCurPreData(ReadInpResultS);
	void TabView(int index);
protected:
    virtual void closeEvent(QCloseEvent *event);
    static int m_indexCnt;//全局变量，记录当前打开view 数;
public slots:
    void HightLightPSet(NElSurfChsS);
	void ActorElSetCheckShow(QStringList strListData);	
	void TabViewSlot();
signals:
	void emitInpDataOk(InpDataVIS*);

public:
    InpDataVIS inpVIS_;
    //FrdDataVIS frdVIS_;
private:
    
};

#endif // WINDOWVIEW_H
