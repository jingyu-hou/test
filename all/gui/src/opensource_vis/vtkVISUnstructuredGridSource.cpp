// Open-source implementation of vtkVISUnstructuredGridSource
#include "vtkVISUnstructuredGridSource.h"
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkPoints.h>
#include <string.h>

vtkStandardNewMacro(vtkVISUnstructuredGridSource);
vtkCxxRevisionMacro(vtkVISUnstructuredGridSource, "$Revision: 1.0 $");

vtkVISUnstructuredGridSource::vtkVISUnstructuredGridSource()
    : unstruGrid(0), scalarSource(0), vectorSource(0), scalarRange(0)
    , cellScalarSource_(0), cellVectorSource_(0), unstruReader_(0)
{
}

vtkVISUnstructuredGridSource::~vtkVISUnstructuredGridSource()
{
    CleanUnGridData();
}

void vtkVISUnstructuredGridSource::LoadData(char* fileName)
{
    if (!unstruReader_) {
        unstruReader_ = vtkUnstructuredGridReader::New();
    }
    unstruReader_->SetFileName(fileName);
    unstruReader_->Update();
    unstruGrid = unstruReader_->GetOutput();
    if (unstruGrid) unstruGrid->Register(this);
}

void vtkVISUnstructuredGridSource::LoadScalarData() {}
void vtkVISUnstructuredGridSource::LoadOneScalarData(const char*) {}
void vtkVISUnstructuredGridSource::LoadVectorData() {}
void vtkVISUnstructuredGridSource::LoadOneVectorData(const char*) {}
void vtkVISUnstructuredGridSource::LoadCellScalarData() {}
void vtkVISUnstructuredGridSource::LoadOneCellScalarData(const char*) {}
void vtkVISUnstructuredGridSource::LoadCellVectorData() {}
void vtkVISUnstructuredGridSource::LoadOneCellVectorData(const char*) {}

double* vtkVISUnstructuredGridSource::GetSourceBounds()
{
    if (unstruGrid) return unstruGrid->GetBounds();
    static double b[6] = {0,0,0,0,0,0};
    return b;
}

double* vtkVISUnstructuredGridSource::GetSourceCenter()
{
    static double c[3] = {0,0,0};
    if (unstruGrid) {
        double* b = unstruGrid->GetBounds();
        c[0] = (b[0]+b[1])/2.0;
        c[1] = (b[2]+b[3])/2.0;
        c[2] = (b[4]+b[5])/2.0;
    }
    return c;
}

vtkUnstructuredGrid* vtkVISUnstructuredGridSource::GetGrid()
{
    return unstruGrid;
}

double* vtkVISUnstructuredGridSource::GetScalarRange(const char* scalarName)
{
    int idx = GetScalarIndex(scalarName);
    if (idx >= 0 && scalarRange && scalarRange[idx])
        return scalarRange[idx];
    static double r[2] = {0,0};
    return r;
}

void vtkVISUnstructuredGridSource::CleanUnGridData()
{
    // unstruGrid is owned by FrdDataSource::idGridMap_ or InpDataSource::wholeGrid_.
    // Do not call Delete() here — the owner manages its lifetime.
    unstruGrid = 0;
}

vtkDoubleArray* vtkVISUnstructuredGridSource::GetScalarSource(int index)
{
    if (index >= 0 && index < scalarNumber_ && scalarSource)
        return scalarSource[index];
    return 0;
}

vtkDoubleArray* vtkVISUnstructuredGridSource::GetVectorSource(int index)
{
    if (index >= 0 && index < vectorNumber_ && vectorSource)
        return vectorSource[index];
    return 0;
}

void vtkVISUnstructuredGridSource::SetScalarNumber(int num) { scalarNumber_ = num; }
void vtkVISUnstructuredGridSource::SetScalarsName(const char** names) { scalarName = names; }
void vtkVISUnstructuredGridSource::SetSourceGrid(vtkUnstructuredGrid* source) { unstruGrid = source; }
void vtkVISUnstructuredGridSource::SetScalarsSource(vtkDoubleArray** src) { scalarSource = src; }
void vtkVISUnstructuredGridSource::SetScalarsRange(double** range) { scalarRange = range; }

void vtkVISUnstructuredGridSource::DeleteOnePoint(vtkPoints* &points, int index)
{
    if (!points) return;
    int n = points->GetNumberOfPoints();
    if (index < 0 || index >= n) return;
    vtkPoints* newPts = vtkPoints::New();
    newPts->SetNumberOfPoints(n - 1);
    int j = 0;
    for (int i = 0; i < n; ++i) {
        if (i == index) continue;
        double p[3]; points->GetPoint(i, p);
        newPts->SetPoint(j++, p);
    }
    points->Delete();
    points = newPts;
}
