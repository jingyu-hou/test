// Open-source replacement for vtkVISOrientationMarker
#ifndef VTK_VIS_ORIENTATION_MARKER_OS_H
#define VTK_VIS_ORIENTATION_MARKER_OS_H
#include <iostream>

using namespace std;

#include <string>

#include "Win32Header.h"
#include "vtkVISManager.h"
#include <vtkAnnotatedCubeActor.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkProperty.h>
#include <vtkCaptionActor2D.h>
#include <vtkTextProperty.h>
#include <vtkPropAssembly.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkObjectFactory.h>

class VTK_VIS_COMMON_EXPORT vtkVISOrientationMarker : public vtkVISManager
{
public:
    static vtkVISOrientationMarker *New();
    vtkTypeRevisionMacro(vtkVISOrientationMarker, vtkVISManager);

    vtkVISOrientationMarker();
    ~vtkVISOrientationMarker();

    void CreateOMDisplay();
    void CreateOMDisplay_FD();
    void ToggleAxesDisplay();

protected:
    vtkAnnotatedCubeActor* annActor_;
    vtkOrientationMarkerWidget *marker_;
    vtkAxesActor *markerAxes_;
};

#endif
