// Open-source replacement for vtkVISUnEdge
#ifndef VTK_VIS_UN_EDGE_OS_H
#define VTK_VIS_UN_EDGE_OS_H
#include <iostream>
using namespace std;

#include "Win32Header.h"
#include "vtkVISUnstructuredGridManager.h"

#include <vtkOutlineFilter.h>
#include <vtkDataSetMapper.h>
#include <vtkGeometryFilter.h>
#include <vtkFeatureEdges.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkActor.h>

class VTK_VIS_UNSTRUCTUREDGRID_EXPORT vtkVISUnEdge : public vtkVISUnstructuredGridManager
{
public:
    static vtkVISUnEdge *New();
    vtkTypeRevisionMacro(vtkVISUnEdge, vtkVISUnstructuredGridManager);

    vtkVISUnEdge();
    ~vtkVISUnEdge();

    void CreateEdgeDisplay();
    void ChangeLineWidth(double widthValue);
    double GetLineWidth();

protected:
    double lineWidth;
};

#endif
