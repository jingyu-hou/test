// Open-source implementation of vtkVISUnstructuredGridManager
#include "vtkVISUnstructuredGridManager.h"
#include <vtkObjectFactory.h>
#include <vtkProperty.h>
#include <string.h>

vtkStandardNewMacro(vtkVISUnstructuredGridManager);
vtkCxxRevisionMacro(vtkVISUnstructuredGridManager, "$Revision: 1.0 $");

vtkVISUnstructuredGridManager::vtkVISUnstructuredGridManager()
    : _source(0), _vtkObjectsNum(0), _unActor(0), _vtkObjects(0)
{
}

vtkVISUnstructuredGridManager::~vtkVISUnstructuredGridManager()
{
    DeleteObjects();
    if (_source) {
        _source->UnRegister(this);
        _source = 0;
    }
}

void vtkVISUnstructuredGridManager::SetDataSource(vtkVISUnstructuredGridSource* object)
{
    if (_source == object) return;
    if (_source) {
        _source->UnRegister(this);
        _source = 0;
    }
    _source = object;
    if (_source) _source->Register(this);
}

void vtkVISUnstructuredGridManager::ShowOn()
{
    if (_unActor) _unActor->VisibilityOn();
}

void vtkVISUnstructuredGridManager::ShowOff()
{
    if (_unActor) _unActor->VisibilityOff();
}

void vtkVISUnstructuredGridManager::ChangeRepresentation(char* style)
{
    if (!_unActor) return;
    if (strcmp(style, "Points") == 0)
        _unActor->GetProperty()->SetRepresentationToPoints();
    else if (strcmp(style, "Wireframe") == 0)
        _unActor->GetProperty()->SetRepresentationToWireframe();
    else
        _unActor->GetProperty()->SetRepresentationToSurface();
}

void vtkVISUnstructuredGridManager::SetOpacity(double value)
{
    if (_unActor) _unActor->GetProperty()->SetOpacity(value);
}

double vtkVISUnstructuredGridManager::GetOpacity()
{
    if (_unActor) return _unActor->GetProperty()->GetOpacity();
    return 1.0;
}

void vtkVISUnstructuredGridManager::SetColor(double r, double g, double b)
{
    if (_unActor) _unActor->GetProperty()->SetColor(r, g, b);
}

double* vtkVISUnstructuredGridManager::GetColor()
{
    static double c[3] = {1,1,1};
    if (_unActor) _unActor->GetProperty()->GetColor(c);
    return c;
}

void vtkVISUnstructuredGridManager::DeleteObjects()
{
    if (_unActor) {
        if (_renderer) _renderer->RemoveActor(_unActor);
        _unActor->Delete();
        _unActor = 0;
    }
    if (_vtkObjects) {
        for (int i = 0; i < _vtkObjectsNum; ++i) {
            if (_vtkObjects[i]) _vtkObjects[i]->Delete();
        }
        delete[] _vtkObjects;
        _vtkObjects = 0;
        _vtkObjectsNum = 0;
    }
}

vtkActor* vtkVISUnstructuredGridManager::GetActor()
{
    return _unActor;
}
