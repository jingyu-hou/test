// Open-source implementation of vtkVISUnShadeMesh
#include "vtkVISUnShadeMesh.h"
#include <vtkObjectFactory.h>
#include <vtkProperty.h>
#include <vtkUnstructuredGrid.h>
#include <string.h>

vtkStandardNewMacro(vtkVISUnShadeMesh);
vtkCxxRevisionMacro(vtkVISUnShadeMesh, "$Revision: 1.0 $");

vtkVISUnShadeMesh::vtkVISUnShadeMesh()
    : meshLineWidth_(1.0), shadeFlag_(false), cellActor_(0), _shadeActor(0), _meshActor(0)
{
}

vtkVISUnShadeMesh::~vtkVISUnShadeMesh()
{
    DeleteObjects();
}

void vtkVISUnShadeMesh::CreateShadeMeshDisplay(bool flag)
{
    shadeFlag_ = flag;
    if (!_source || !_source->unstruGrid) return;

    if (flag) {
        // Shade display: geometry filter -> mapper -> actor
        vtkGeometryFilter* geo = vtkGeometryFilter::New();
        geo->SetInput(_source->unstruGrid);

        vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
        mapper->SetInputConnection(geo->GetOutputPort());
        mapper->ScalarVisibilityOff();

        vtkActor* actor = vtkActor::New();
        actor->SetMapper(mapper);
        actor->GetProperty()->SetRepresentationToSurface();

        if (_renderer) _renderer->AddActor(actor);
        if (_unActor) { _renderer->RemoveActor(_unActor); _unActor->Delete(); }
        _unActor = actor;
        _shadeActor = actor;

        if (!_vtkObjects) {
            _vtkObjectsNum = 2;
            _vtkObjects = new vtkObject*[2];
            _vtkObjects[0] = geo;
            _vtkObjects[1] = mapper;
        }
    } else {
        // Mesh (all edges) display: extract edges -> mapper -> actor
        vtkExtractEdges* edges = vtkExtractEdges::New();
        edges->SetInput(_source->unstruGrid);

        vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
        mapper->SetInputConnection(edges->GetOutputPort());
        mapper->ScalarVisibilityOff();

        vtkActor* actor = vtkActor::New();
        actor->SetMapper(mapper);
        actor->GetProperty()->SetRepresentationToWireframe();

        if (_renderer) _renderer->AddActor(actor);
        if (_unActor) { _renderer->RemoveActor(_unActor); _unActor->Delete(); }
        _unActor = actor;
        _meshActor = actor;

        if (!_vtkObjects) {
            _vtkObjectsNum = 2;
            _vtkObjects = new vtkObject*[2];
            _vtkObjects[0] = edges;
            _vtkObjects[1] = mapper;
        }
    }
}

void vtkVISUnShadeMesh::CreateShadeMeshSurfaceDisplay(bool flag)
{
    if (!_source || !_source->unstruGrid) return;

    vtkGeometryFilter* geo = vtkGeometryFilter::New();
    geo->SetInput(_source->unstruGrid);

    vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
    mapper->SetInputConnection(geo->GetOutputPort());
    mapper->ScalarVisibilityOff();

    vtkActor* actor = vtkActor::New();
    actor->SetMapper(mapper);

    if (flag) {
        // Shade surface
        actor->GetProperty()->SetRepresentationToSurface();
    } else {
        // Mesh surface
        actor->GetProperty()->SetRepresentationToWireframe();
    }

    if (_renderer) _renderer->AddActor(actor);
    if (_unActor) { _renderer->RemoveActor(_unActor); _unActor->Delete(); }
    _unActor = actor;

    if (!_vtkObjects) {
        _vtkObjectsNum = 2;
        _vtkObjects = new vtkObject*[2];
        _vtkObjects[0] = geo;
        _vtkObjects[1] = mapper;
    }
}

void vtkVISUnShadeMesh::SetMeshLineWidth(double widthValue)
{
    meshLineWidth_ = widthValue;
    if (_unActor) _unActor->GetProperty()->SetLineWidth(widthValue);
}

double vtkVISUnShadeMesh::GetMeshLineWidth()
{
    return meshLineWidth_;
}

void vtkVISUnShadeMesh::ChangeMeshLinePattern(char* pattern)
{
    if (!_unActor || !pattern) return;
    if (strcmp(pattern, "Solid") == 0)
        _unActor->GetProperty()->SetLineStipplePattern(0xFFFF);
    else if (strcmp(pattern, "Dash") == 0)
        _unActor->GetProperty()->SetLineStipplePattern(0xFF00);
    else if (strcmp(pattern, "Dot") == 0)
        _unActor->GetProperty()->SetLineStipplePattern(0xAAAA);
}

vtkActor* vtkVISUnShadeMesh::CreateCellIdDisplay(int id)
{
    // Simplified: create a single-cell highlight actor
    if (!_source || !_source->unstruGrid) return 0;

    vtkGeometryFilter* geo = vtkGeometryFilter::New();
    geo->SetInput(_source->unstruGrid);

    vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
    mapper->SetInputConnection(geo->GetOutputPort());
    mapper->ScalarVisibilityOff();

    cellActor_ = vtkActor::New();
    cellActor_->SetMapper(mapper);
    cellActor_->GetProperty()->SetColor(1, 0, 0);

    if (_renderer) _renderer->AddActor(cellActor_);
    return cellActor_;
}

void vtkVISUnShadeMesh::ModifiedCellDisplayColor(double r, double g, double b)
{
    if (cellActor_) cellActor_->GetProperty()->SetColor(r, g, b);
}
