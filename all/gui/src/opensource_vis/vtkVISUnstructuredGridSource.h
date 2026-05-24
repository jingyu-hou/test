// Open-source replacement for vtkVISUnstructuredGridSource
#ifndef VTK_VIS_UNSTRUCTURED_GRID_SOURCE_OS_H
#define VTK_VIS_UNSTRUCTURED_GRID_SOURCE_OS_H
#include <iostream>
using namespace std;

#include "Win32Header.h"
#include "vtkVISManager.h"
#include <vtkObjectFactory.h>
#include <vector>

#include <vtkUnstructuredGridReader.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkCellData.h>

#include <vtkIdList.h>
#include <vtkUnstructuredGridWriter.h>

class VTK_VIS_UNSTRUCTUREDGRID_EXPORT vtkVISUnstructuredGridSource : public vtkVISManager
{
public:
    static vtkVISUnstructuredGridSource *New();
    vtkTypeRevisionMacro(vtkVISUnstructuredGridSource, vtkVISManager);

    vtkVISUnstructuredGridSource();
    ~vtkVISUnstructuredGridSource();

    vtkUnstructuredGrid* unstruGrid;
    vtkDoubleArray** scalarSource;
    vtkDoubleArray** vectorSource;
    double** scalarRange;

    vtkDoubleArray** cellScalarSource_;
    vtkDoubleArray** cellVectorSource_;

    void LoadData(char* fileName);
    void LoadScalarData();
    void LoadVectorData();
    void LoadOneScalarData(const char* scalarName);
    void LoadOneVectorData(const char* vectorName);
    void LoadCellScalarData();
    void LoadCellVectorData();
    void LoadOneCellScalarData(const char* scalarName);
    void LoadOneCellVectorData(const char* vectorName);

    double* GetSourceBounds();
    double* GetSourceCenter();
    vtkUnstructuredGrid* GetGrid();
    double* GetScalarRange(const char* scalarName);
    void CleanUnGridData();
    vtkDoubleArray* GetScalarSource(int index);
    vtkDoubleArray* GetVectorSource(int index);

    void SetScalarNumber(int num);
    void SetScalarsName(const char** scalarsName);
    void SetSourceGrid(vtkUnstructuredGrid* source);
    void SetScalarsSource(vtkDoubleArray** scalarsSource);
    void SetScalarsRange(double** scalarsRange);

protected:
    vtkUnstructuredGridReader *unstruReader_;
    void DeleteOnePoint(vtkPoints* &points, int index);
};

#endif
