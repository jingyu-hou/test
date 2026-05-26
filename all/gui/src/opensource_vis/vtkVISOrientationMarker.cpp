// Open-source implementation of vtkVISOrientationMarker
#include "vtkVISOrientationMarker.h"
#include <vtkObjectFactory.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>

vtkStandardNewMacro(vtkVISOrientationMarker);
vtkCxxRevisionMacro(vtkVISOrientationMarker, "$Revision: 1.0 $");

vtkVISOrientationMarker::vtkVISOrientationMarker()
    : annActor_(0), marker_(0), markerAxes_(0), boundRenWin_(0), boundInteractor_(0)
{
}

vtkVISOrientationMarker::~vtkVISOrientationMarker()
{
    ResetMarker();
    if (markerAxes_) { markerAxes_->Delete(); markerAxes_ = 0; }
    if (annActor_) { annActor_->Delete(); annActor_ = 0; }
}

bool vtkVISOrientationMarker::IsBoundTo(vtkRenderWindow *renWin, vtkRenderWindowInteractor *interactor) const
{
    return markerAxes_ && boundRenWin_ == renWin && boundInteractor_ == interactor;
}

bool vtkVISOrientationMarker::IsReady() const
{
    return markerAxes_ && boundRenWin_ && boundInteractor_;
}

void vtkVISOrientationMarker::ResetMarker()
{
    if (_renderer && markerAxes_) {
        _renderer->RemoveActor(markerAxes_);
    }
    boundRenWin_ = 0;
    boundInteractor_ = 0;
}

void vtkVISOrientationMarker::CreateOMDisplay()
{
    if (!_renderer || !_renWin) return;
    vtkRenderWindowInteractor *interactor = _renWin->GetInteractor();
    if (!interactor) return;
    if (IsBoundTo(_renWin, interactor)) return;

    if (boundRenWin_ || boundInteractor_) {
        ResetMarker();
    }

    if (!markerAxes_) {
        markerAxes_ = vtkAxesActor::New();
        markerAxes_->SetShaftTypeToCylinder();
        markerAxes_->SetXAxisLabelText("X");
        markerAxes_->SetYAxisLabelText("Y");
        markerAxes_->SetZAxisLabelText("Z");
        markerAxes_->SetTotalLength(1.5, 1.5, 1.5);
        markerAxes_->SetPosition(0.0, 0.0, 0.0);
    }

    _renderer->AddActor(markerAxes_);
    boundRenWin_ = _renWin;
    boundInteractor_ = interactor;
}

void vtkVISOrientationMarker::CreateOMDisplay_FD()
{
    CreateOMDisplay();
}

void vtkVISOrientationMarker::ToggleAxesDisplay()
{
    if (!_renWin) return;
    vtkRenderWindowInteractor *interactor = _renWin->GetInteractor();
    if (!interactor) return;

    if (!IsBoundTo(_renWin, interactor)) {
        CreateOMDisplay();
        return;
    }

    if (markerAxes_ && boundRenWin_ == _renWin && boundInteractor_ == interactor) {
        if (markerAxes_->GetVisibility()) {
            markerAxes_->VisibilityOff();
        } else {
            markerAxes_->VisibilityOn();
        }
    } else {
        CreateOMDisplay();
    }
}
