#ifndef _INPDATAVIS_H
#define _INPDATAVIS_H

#include "ADefineQ.h"
#include <map>
using namespace std;
#include <QString>
#include <QStringList>
#include "CRWManage.h"
//****
#include "InpDataSource.h"
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkPointPicker.h>
#include <vtkRendererCollection.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include <vtkVectorText.h>
#include <vtkVISUnstructuredGridSource.h>
#include <vtkGeometryFilter.h>
#include <vtkFeatureEdges.h>
#include <vtkObject.h>
//***
class vtkRenderer;
class vtkRenderWindow;

class FrdDataSource;
class vtkVISUnShadeMesh;
class vtkVISUnEdge;
//class vtkVISUnContour;

typedef struct _HIS_POINT_S1_{
    vtkSphereSource *sphere;
    vtkPolyDataMapper *pointMapMaper;
    vtkPolyDataMapper *textMaper;
    vtkVectorText* text;
    vtkTransform* t;
    vtkTransformFilter* tf;
    vtkActor *MapActor;
    vtkActor *LabActor;
    double x;double y;double z;
}HisPointS1;

class InpDataVIS
{
public:
    InpDataVIS();
    ~InpDataVIS();
    vtkRenderer* GetBindedRenderer();

    void InitRenderer(vtkRenderer *renderer);
    bool LoadInpData(ReadInpResultS *frd);
    void ShallowCopyFrdData(InpDataVIS *source);
    void SetMeshVisible(bool visible,VTKColorS tmpClr);
    void Update();
    //--BCHighLight---
    void SetBCVisible(bool visible, NElSurfChsS);//QString str);
	void SetBCCheckVisible(bool visible, NElSurfChsS Settype)/*QString str) */;
    vtkVISUnstructuredGridSource* GetSourceData();
    //--pelsfsetdlg
    vtkUnstructuredGrid* GetWholeData(QString str);
    vtkUnstructuredGrid* GetSurfGridData();
    void SetNset(QString,DecodeGenerateS);//Add Nset
    void SetElset(QString,DecodeGenerateS);//Add Elset
    void SetSurfset(QString,QStringList);//Add Surfset
    const QMap<QString, QString>* GetElementSurfFilterMap();
    void SetMeshActorShow(bool bShow);
	bool SetHisPointVtkShow(int PointId,double x, double y, double z);//luo
	void ClearHisPointVtkShow();//luo
	int GetCellBaseId();
private:
    void Clean();
    vtkVISUnShadeMesh* CreateShadeMeshObject(int type);
    vtkVISUnShadeMesh* CreateBCShadeMeshObject(int type, QString str);
	void PointVisibility(bool f);//luo
	
	double factor_;//系数（大小）luo
	VTKColorS PointColor_;//颜色luo
	 bool bPointLab_;//luo
	 QMap<int, HisPointS1> m_PointProp;//luo
    
private:
    vtkRenderer *renderer_;
    vtkRenderWindow *renWin_;
    InpDataSource *InpSource_;
    vtkVISUnShadeMesh *mesh_;//whole Actor
    //--QY 
    QMap<QString,vtkVISUnShadeMesh*>BCmeshMap_;//Elemetn Actor
	vtkVISUnShadeMesh *BCmesh_;//only chose oneElemetn Actor show
};

#endif // INPDATAVIS_H
