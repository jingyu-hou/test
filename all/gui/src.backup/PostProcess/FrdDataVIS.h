#ifndef FRDDATAVIS_H
#define FRDDATAVIS_H

#include "../ADefineQ.h"
#include <map>
using namespace std;
#include <QString>
#include <QStringList>

#include <vtkVISUnstructuredGridSource.h>
#include <vtkGeometryFilter.h>
#include <vtkFeatureEdges.h>
#include <vtkObject.h>

#ifdef _MSC_VER
	#include <vtkAVIWriter.h>
#else
	#include<vtkFFMPEGWriter.h>
#endif

//#include <vtkGenericMovieWriter.h>
#include <vtkWindowToImageFilter.h>
class vtkRenderer;
class vtkRenderWindow;
struct _PointXYZ_S;
struct _RESULT_ELEM_S;
struct _RESULT_STEP_S_;
class FrdDataSource;
class vtkVISUnShadeMesh;
class vtkVISUnEdge;
class vtkVISUnContour;
class vtkCallbackCommand;
//class vtkVISUnSlice;
//class QVTKUnClip
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkPointPicker.h>
#include <vtkRendererCollection.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include <vtkVectorText.h>
#include "QVTKUnClip.h"
//#include "qvtkavi.h"
//#include <vtkFFMPEWriter.h>
typedef struct _HIS_POINT_S_{
    vtkSphereSource *sphere;
    vtkPolyDataMapper *pointMapMaper;
    vtkPolyDataMapper *textMaper;
    vtkVectorText* text;
    vtkTransform* t;
    vtkTransformFilter* tf;
    vtkActor *MapActor;
    vtkActor *LabActor;
    double x;double y;double z;
}HisPointS;
typedef struct _OUTLINE_S_{
    vtkActor *vActor;
    vtkVISUnstructuredGridSource *SourceData;
    vtkFeatureEdges *edges;
}OutLineS;
class FrdDataVIS
{
    public:
        FrdDataVIS();
        ~FrdDataVIS();
        void InitRenderer(vtkRenderer *renderer);
        vtkRenderer* GetBindedRenderer();
        /////
        bool Step1_LoadFrdPoints(_PointXYZ_S *pStruct);
        bool Step2_LoadFrdCells(_RESULT_ELEM_S *cStruct);
        bool Step3_LoadFrdResults(_RESULT_STEP_S_ *rStruct);
        bool Step4_SetupFrd();
        /////
        void ShallowCopyFrdData(FrdDataVIS *source);
        void SetShadeVisible(int gridId, bool visible,VTKColorS tmpClr);
        void SetMeshVisible(int gridId, bool visible,VTKColorS tmpClr);
        void SetAllEdgeMeshVisible(int gridId, bool visible,VTKColorS tmpClr);
        void SetOutlineVisible(int gridId, bool visible,VTKColorS tmpClr);
        //contour
        void SetContourVisible(int gridId, const QString &scalar, bool visible, const QString &header = "original");
        void SetContourVariable(int gridId, const QString &scalar, const QString &header = "original");
        void SetContourType(int gridId, const QString &scalar, int type, const QString &header = "original");
        void SetContourLevel(int gridId, int level, const QString &header = "original");
        void SetContourOpacity(int gridId, double opacity, const QString &header = "original");
        void SetContourRange(int gridId, double minV, double maxV, const QString &header = "original");
        void SetContourScalarbarVisible(int gridId, bool visible, const QString &header = "original");
        //displacement
        void SetDisplacementVisible(int gridId, const QString &header, bool visible);
        void SetDisplacementColor(VTKColorS Color); 
        VTKColorS m_gridShowColor;
        //--fit call actor;
        void ResetCamera();
        //
        void SetShadeOpacity(double factor, const QString &header);
        //
        const map<QString, QStringList>* GetScalarInfo();
        const vector<int>* GetGridIds();
        const QVector<QString>* GetScalarVectorInfo();
        //get xyplot data
        map<double, double> GetPointScalar_TimeValueMap(int pointId, const QString &scalarName);  //key type = double.  for sorting
        void Update();
        //get textVIS data
        TextStepIncTimeS GetText(const QString &scalar, const QString &header = "original");
        //属性
        void SetWidth(int LineW);

        bool SetHisPointVtkShow(int PointId,double x, double y, double z);
        bool RemoveHisPointVtkShow(vtkActor *actor);
        void ClearHisPointVtkShow();
        void SetColorSize(double factor,VTKColorS);
        void SetPickPointLabVisible(bool f);
        //--outline
         static void Callback_PickPoint(vtkObject *caller, unsigned long, void *clientdata, void*);
        QStringList m_ListData;  
        void ActorPick(bool flag);
        void AcotrVisibility(bool f);
        //--pickerPoind ID
        //--get pointId
        int GetPointId(QString s,double x, double y, double z);
        //cut api begin
        void CreateCutObjects(int,int cutId, bool bInsiedeOut, QString &scalar, const QString &header="original");
        void AddCutCallBack(int,int cutId, vtkCallbackCommand *callback);
        void SetCutZoneVisible(int,int cutId, bool visible);
		void SetCutZoneInsideOutVisible(int,int cutId, bool visible);
        void SetCutVTKWidgetVisible(int,int cutId, bool visible);
        void SetCutPlaneNormal(int,int cutId, double x, double y, double z);
        void SetCutPlanePosition(int,int cutId, double x, double y, double z);
        bool GetCutPlanePosition(int,int cutId, double *xyz);
        void SetCutContourState(int,int cutId, bool visible, int type);
        void SetCutContourLevel(int,int cutId, int level);
        void SetCutContourVariable(int,int cutId, const QString &scalar);

        void DeleteCut(int,int cutId);
        bool GetCutZoneBounds(int,int cutId, double *bounds);
        //cut api end

		//AVI Set
		int StartAVI(QString name,int rate);
		void ModifiedAVI();
		void EndAVI();
private:
        void Clean();
        vtkVISUnShadeMesh* CreateShadeMeshObject(int gridId, int type);
        vtkVISUnEdge* CreateOutlineObject(int gridId);
        vtkVISUnContour* CreateContourObject(int gridId, const QString &scalar, const QString &header);
        void PointVisibility(bool f);
        void createActor();
        //--VTK HIS point
        QMap<int, HisPointS> m_PointProp;
        void UpdataSelectActor(vtkActor *);

private:
        vtkRenderer *renderer_;
        vtkRenderWindow *renWin_;
        FrdDataSource *frdSource_;
        map<int, vtkVISUnShadeMesh*> shadeMap_;
        map<int, vtkVISUnShadeMesh*> meshMap_;
        map<int, vtkVISUnShadeMesh*> allEdgeMeshMap_;
        map<int, vtkVISUnEdge*> outlineMap_;
		//map<int, vtkVISUnSlice*> cutMap_;
        bool scalarResultLoaded_;
        map<QString, map<int, vtkVISUnShadeMesh*> > headerDisplacementsMap_;  //header fmt = L101-DISP
        map<QString, map<int, vtkVISUnContour*> > headerContoursMap_;  //contour on original-grid and deformed-grids.  key= "original" or "Lnnn-DISP"
        //map<int, map<int, vtkVISUnSlice*> >GridcutIdMap_;//gridId,cutId,vtkVISUnSlice
		map<int, map<int, QVTKUnClip*> >GridcutIdMap_;//gridId,cutId,vtkVISUnSlice
		//map<int, QVTKUnClip*> cutMap_;
        map<int, QString> cutZoneHeaderMap_;
        //--size color
        VTKColorS PointColor_;//颜色
        double factor_;//系数（大小）
        bool bPointLab_;
        //--output
        vtkCallbackCommand *pointPickCallback_;
        QMap<int ,OutLineS> m_OutLineS; 
        vtkGeometryFilter *m_geoFilter;

        //--vtk 
        vtkActor *m_tmpActor;

		//--avi
		vtkWindowToImageFilter *ImageFilter_;
		//vtkGenericMovieWriter *vtkWriter_;
		//QVTKMovier *vtkWriter_;
		//vtkMP4Writer *vtkWriter_;
#ifdef _MSC_VER
		vtkAVIWriter* vtkWriter_;
#else
        vtkFFMPEGWriter *vtkWriter_;
#endif
		
	

};

#endif

