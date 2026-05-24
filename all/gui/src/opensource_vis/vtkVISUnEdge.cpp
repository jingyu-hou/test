// Open-source implementation of vtkVISUnEdge
#include "vtkVISUnEdge.h"
#include <vtkObjectFactory.h>
#include <vtkProperty.h>
#include <vtkUnstructuredGrid.h>

vtkStandardNewMacro(vtkVISUnEdge);
vtkCxxRevisionMacro(vtkVISUnEdge, "$Revision: 1.0 $");

vtkVISUnEdge::vtkVISUnEdge()
    : lineWidth(1.0)
{
}

vtkVISUnEdge::~vtkVISUnEdge()
{
    DeleteObjects();
}

void vtkVISUnEdge::CreateEdgeDisplay()
{
    if (!_source || !_source->unstruGrid) return;

    vtkFeatureEdges* edges = vtkFeatureEdges::New();
    edges->SetInput(_source->unstruGrid);
    edges->BoundaryEdgesOn();
    edges->FeatureEdgesOff();
    edges->NonManifoldEdgesOff();
    edges->ManifoldEdgesOff();
    edges->ColoringOff();

    vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
    mapper->SetInputConnection(edges->GetOutputPort());
    mapper->ScalarVisibilityOff();

    vtkActor* actor = vtkActor::New();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetLineWidth(lineWidth);
    actor->GetProperty()->SetRepresentationToWireframe();

    if (_renderer) _renderer->AddActor(actor);
    if (_unActor) { _renderer->RemoveActor(_unActor); _unActor->Delete(); }
    _unActor = actor;

    if (!_vtkObjects) {
        _vtkObjectsNum = 2;
        _vtkObjects = new vtkObject*[2];
        _vtkObjects[0] = edges;
        _vtkObjects[1] = mapper;
    }
}

void vtkVISUnEdge::ChangeLineWidth(double widthValue)
{
    lineWidth = widthValue;
    if (_unActor) _unActor->GetProperty()->SetLineWidth(widthValue);
}

double vtkVISUnEdge::GetLineWidth()
{
    return lineWidth;
}
