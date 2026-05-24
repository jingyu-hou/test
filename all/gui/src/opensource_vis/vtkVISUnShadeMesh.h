// Open-source replacement for vtkVISUnShadeMesh
#ifndef VTK_VIS_UN_SHADE_MESH_OS_H
#define VTK_VIS_UN_SHADE_MESH_OS_H
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#include "Win32Header.h"
#include "vtkVISUnstructuredGridManager.h"

#include <vtkDataSetMapper.h>
#include <vtkGeometryFilter.h>
#include <vtkCell.h>
#include <vtkPoints.h>
#include <vtkIdList.h>
#include <vtkExtractEdges.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkActor.h>

class VTK_VIS_UNSTRUCTUREDGRID_EXPORT vtkVISUnShadeMesh : public vtkVISUnstructuredGridManager
{
public:
    static vtkVISUnShadeMesh *New();
    vtkTypeRevisionMacro(vtkVISUnShadeMesh, vtkVISUnstructuredGridManager);

    vtkVISUnShadeMesh();
    ~vtkVISUnShadeMesh();

    void CreateShadeMeshDisplay(bool flag);
    void CreateShadeMeshSurfaceDisplay(bool flag);
    void SetMeshLineWidth(double widthValue);
    double GetMeshLineWidth();
    void ChangeMeshLinePattern(char* pattern);
    vtkActor* CreateCellIdDisplay(int id);
    void ModifiedCellDisplayColor(double r, double g, double b);

protected:
    double meshLineWidth_;
    bool shadeFlag_;
    vtkActor* cellActor_;
    vtkActor* _shadeActor;
    vtkActor* _meshActor;
};

#endif
