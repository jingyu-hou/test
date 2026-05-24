// Open-source implementation of vtkVISOrientationMarker
#include "vtkVISOrientationMarker.h"
#include <vtkObjectFactory.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>

vtkStandardNewMacro(vtkVISOrientationMarker);
vtkCxxRevisionMacro(vtkVISOrientationMarker, "$Revision: 1.0 $");

vtkVISOrientationMarker::vtkVISOrientationMarker()
    : annActor_(0), marker_(0), markerAxes_(0)
{
}

vtkVISOrientationMarker::~vtkVISOrientationMarker()
{
    if (marker_) {
        marker_->SetEnabled(0);
        marker_->Delete();
        marker_ = 0;
    }
    if (markerAxes_) { markerAxes_->Delete(); markerAxes_ = 0; }
    if (annActor_) { annActor_->Delete(); annActor_ = 0; }
}

void vtkVISOrientationMarker::CreateOMDisplay()
{
    if (!_renderer || !_renWin) return;

    if (markerAxes_) return; // already created

    markerAxes_ = vtkAxesActor::New();
    markerAxes_->SetShaftTypeToCylinder();
    markerAxes_->SetXAxisLabelText("X");
    markerAxes_->SetYAxisLabelText("Y");
    markerAxes_->SetZAxisLabelText("Z");
    markerAxes_->SetTotalLength(1.5, 1.5, 1.5);

    marker_ = vtkOrientationMarkerWidget::New();
    marker_->SetOrientationMarker(markerAxes_);
    marker_->SetInteractor(_renWin->GetInteractor());
    marker_->SetViewport(0.0, 0.0, 0.2, 0.2);
    marker_->SetEnabled(1);
    marker_->InteractiveOff();
}

void vtkVISOrientationMarker::CreateOMDisplay_FD()
{
    CreateOMDisplay();
}

void vtkVISOrientationMarker::ToggleAxesDisplay()
{
    if (marker_) {
        int enabled = marker_->GetEnabled();
        marker_->SetEnabled(enabled ? 0 : 1);
    } else {
        CreateOMDisplay();
    }
}
