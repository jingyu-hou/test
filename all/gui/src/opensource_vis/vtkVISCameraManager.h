// Open-source stub for vtkVISCameraManager (not directly used by consumers)
#ifndef VTK_VIS_CAMERA_MANAGER_OS_H
#define VTK_VIS_CAMERA_MANAGER_OS_H

#include "Win32Header.h"
#include "vtkVISManager.h"
#include <vtkCamera.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkTransform.h>

class VTK_VIS_COMMON_EXPORT vtkVISCameraManager : public vtkVISManager
{
public:
    static vtkVISCameraManager *New();
    vtkTypeRevisionMacro(vtkVISCameraManager, vtkVISManager);
    vtkVISCameraManager();
    ~vtkVISCameraManager();

    void GetCamera();
    void ZoomOut();
    void ZoomIn();
    void Relocate();
    void RelocateToCenter();
    void AlignToPlusAxisX();
    void AlignToMinusAxisX();
    void AlignToPlusAxisY();
    void AlignToMinusAxisY();
    void AlignToPlusAxisZ();
    void AlignToMinusAxisZ();
    void TransformToLeft();
    void TransformToRight();
    void TransformToUp();
    void TransformToDown();
    void TransformByXYZ(double x, double y, double z);
    void ScaleByXYZ(double x, double y, double z);
    void RotateWXYZ(double angle, double x, double y, double z);
    void RotateX(double angle);
    void RotateY(double angle);
    void RotateZ(double angle);
    void InteractorOn();
    void InteractorOff();
    int GetInteractorStatus();
    void SetInteractorStyleToJoystickCamera();
    void SetInteractorStyleToTrackballCamera();
    void SetInteractorStyleToJoystickActor();
    void SetInteractorStyleToTrackballActor();
    void ParallelProjectionOn();
    void ParallelProjectionOff();
    int GetProjectionMethod();

protected:
    vtkCamera *camera_;
    int xIndex, yIndex;
    vtkTransform *trans_;
    int dFlag;
};

#endif
