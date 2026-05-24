// Open-source replacement for vtkVISUnstructuredGridManager — base for all grid renderers
#ifndef VTK_VIS_UNSTRUCTURED_GRID_MANAGER_OS_H
#define VTK_VIS_UNSTRUCTURED_GRID_MANAGER_OS_H
#include <iostream>
using namespace std;

#include "Win32Header.h"
#include "vtkVISManager.h"
#include "vtkVISUnstructuredGridSource.h"
#include <vtkObjectFactory.h>
#include <vtkProperty.h>
#include <vtkActor.h>

class VTK_VIS_UNSTRUCTUREDGRID_EXPORT vtkVISUnstructuredGridManager : public vtkVISManager
{
public:
    static vtkVISUnstructuredGridManager *New();
    vtkTypeRevisionMacro(vtkVISUnstructuredGridManager, vtkVISManager);

    vtkVISUnstructuredGridManager();
    ~vtkVISUnstructuredGridManager();

    void SetDataSource(vtkVISUnstructuredGridSource* object);
    void ShowOn();
    void ShowOff();
    void ChangeRepresentation(char* style);
    void SetOpacity(double value);
    double GetOpacity();
    void SetColor(double r, double g, double b);
    double* GetColor();
    void DeleteObjects();
    vtkActor* GetActor();

protected:
    vtkVISUnstructuredGridSource* _source;
    int _vtkObjectsNum;
    vtkActor* _unActor;
    vtkObject** _vtkObjects;
};

#endif
