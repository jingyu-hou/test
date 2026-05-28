#ifndef __FRDDATASOURCE_H
#define __FRDDATASOURCE_H

#include "../ADefineQ.h"
#include <vector>
#include <map>
using namespace std;
#include <QString>
#include <QStringList>
#include <QSet>
class vtkDoubleArray;
class vtkUnstructuredGrid;
class vtkVISUnstructuredGridSource;
class vtkCell;
class vtkPoints;
struct _PointXYZ_S;
struct _RESULT_ELEM_S;
struct _RESULT_STEP_S_;

class FrdDataSource
{
public:
    FrdDataSource();
    ~FrdDataSource();

    /////
    bool Step1_InitGridPoints(_PointXYZ_S *pStruct);
    bool Step2_InitGridCells(_RESULT_ELEM_S *cStruct);
    bool Step3_InitResults(_RESULT_STEP_S_ *rStruct);
    bool Step4_SetupFrd();
    /////
    bool SetFrdDataSource(int gridId, const QString &header = "");
    const map<QString, QStringList>* GetScalarInfo();
    const vector<int>* GetGridIds();
    const QVector<QString>* GetScalarVectorInfo();
    vtkVISUnstructuredGridSource* GetSourceGrid();
    vtkVISUnstructuredGridSource* CreateSourceGrid(int gridId, const QString &header = "");
    bool IsScalarNameValid(const QString &scalar);
    bool IsScalarValuesDiff(const QString &scalar);
    bool GridHasScalarData(int gridId, const QString &scalar);
    map<double, double> GetPointScalar_TimeValueMap(int pointId, const QString &scalarName);  //key type = double.  for sorting
     TextStepIncTimeS GetStepTimeIncMap(int pointId, const QString &scalarName);
     int GetPointID(QString t,double x,double y,double z);
private:
    vtkCell* NewVTKCell(int frdCellType);
    vtkUnstructuredGrid* BuildDisplacementGrid(int gridId, const QString &header);
    
private:
    map<int, int> pointIdMap_;
    vtkPoints *frdPoints_;
    vector<int> gridIdsVec_;
    map<int, vtkUnstructuredGrid*> idGridMap_;
    vtkVISUnstructuredGridSource *gSource_;
    int scalarNumber_;
    const char **scalarName_;
    //
    vector<char*> scalarVec_;
    vector<vtkDoubleArray*> resultVec_;
    //
    QVector<QString> varHeaderList_;
    map<QString, QStringList> varHeaderScalarMap_;
    map<QString, QString> headerTimeStepMap_;
    map<QString, TextStepIncTimeS> StepTimeIncMap_;
    map<QString, QSet<int> > scalarValidPointMap_;
    vtkDoubleArray **pointResults_;
    double **scalarRange_;
    map<QString, map<int, vtkUnstructuredGrid*> > dispGridsMap_;
};

#endif
