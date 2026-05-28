#ifndef QMYVTK_H
#define QMYVTK_H
#include <QDialog>
#include <QPushButton>
#include <QColor>
#include <QColorDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QDialogButtonBox>
#include <QLabel>
#include <QMouseEvent>

#include "QVTKWidget.h"
#include "vtkActor.h"
#include "vtkAxesActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkProperty.h"

#include "vtkUnstructuredGrid.h"
#include "vtkDataSetMapper.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"
#include "QFrdDataPro.h"
//#include "vtkOutlineFilter.h"
//#include "vtkInteractorStyleTrackball.h"
//#include "vtkInteractorStyleTrackballCamera.h"
//#include "vtkVISPointPicker.h"

class QSafeVTKWidget : public QVTKWidget
{
public:
    explicit QSafeVTKWidget(QWidget *parent = 0) : QVTKWidget(parent) {}

protected:
    void mouseMoveEvent(QMouseEvent *event)
    {
        if (event && event->buttons() == Qt::NoButton) {
            event->ignore();
            return;
        }
        QVTKWidget::mouseMoveEvent(event);
    }
};



#include "vtkQuad.h"
#include "vtkTriangle.h"
#include "vtkTetra.h"//4节点4面体
#include "vtkWedge.h"//6节点楔形
#include "vtkHexahedron.h"//6面体

#include "vtkQuadraticHexahedron.h"
#include "vtkQuadraticWedge.h"//15节点楔形
#include "vtkQuadraticTetra.h"//10节点4面体
#include "vtkQuadraticTriangle.h"//6节点三角形
#include "vtkQuadraticQuad.h"//8节点四边形
#include "vtkCamera.h"

enum ELEMENT_STYLE_ENUM{
	ENUM_NO_ELEM_STYLE = 0,
	ENUM_8NODEL_HEXAHEDRON = 1,//8节点6面体
	ENUM_6NODEL_WEDGE = 2,//6节点楔形
	ENUM_4NODEL_TETRAHEDRON = 3,//4节点4面体
	ENUM_20NODEL_HEXAHEDRON = 4,//20节点6面体(not same)
	ENUM_15NODEL_WEDGE = 5,//15节点楔形(not same)
	ENUM_10NODEL_TETRAHEDRON = 6,//10节点4面体
	ENUM_3NODEL_TRI = 7,//3节点4三角形
	ENUM_6NODEL_TRI = 8,//6节点三角形
	ENUM_4NODEL_QUAD = 9,//4节点四边形(逆時針)
	ENUM_8NODEL_QUAD = 10//8节点四边形
};

class QMyVTK 
{
public:
    ~QMyVTK();

public:
    static QMyVTK* GetInstance();
    static QMyVTK* GetInstance(int);
    QVTKWidget* GetVTKWidget();
    vtkRenderer* GetRenderer();
    void Zoom(double factor);
    void Reset();
    void ChangeViewDirection(const QString &direction);
    void clear();

    void Orient();
    void BackColor();
public:
    static  QMap<int,QMyVTK*>  m_QVTKWidgetObjectMap;
    void ViewChange(int index);
    //QVTKWidget  *m_VTKWidget;
private:
    QMyVTK();
    void ModifyVTKRendererCamera(vtkRenderer *renderer, const QString &action);
    

private:
    static QMyVTK* instance;
    int m_curId;
     //QMap<int ,QVTKWidget*> m_VTKWidget;
    
    QVTKWidget  *m_VTKWidget;
    vtkUnstructuredGrid *m_unGrid_;
    vtkRenderer *render_;
    vtkRenderer *m_axisRenderer;
    vtkAxesActor *m_axisActor;
};

class Dialog_BG : public QDialog
{
    Q_OBJECT

public:
    static Dialog_BG* GetInstance();
    ~Dialog_BG();

private:
    Dialog_BG(QWidget *parent = 0);
    void ChangeBackground(bool isGradient, double *rgb1, double *rgb2);
    void ModifyBackground(vtkRenderer *renderer, bool isGradient, double *rgb1, double *rgb2);

    private slots:
        void BGModeChanged();
        void ChooseColor();
        void UpdateBG();
        void OKSlot();
private:
    static Dialog_BG *instance;
    QDialogButtonBox *buttonBox;
    QPushButton *OKBtn, *ApplyBtn, *CloseBtn;
    QRadioButton  *radioButton_singleColor,*radioButton_gradientColor;
    QPushButton   *pushButton_color1,*pushButton_color2;
    QColor color1_;
    QColor color2_;
};


#endif // QMYVTK_H
