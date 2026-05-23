/*=================================================================================

! Module name = Visualization
!
! File name = vtkVISCameraManager.h
!
! Version: ASTE-P 2.0
!
! Time of release: 
!
!       All rights reserved by Advanced Dynamics Corporation, any person or organization
!       must get written permission from Advanced Dynamics Corporation to copy a part or
!       whole of this code except US Army, all NASA Centers and Governmental Agencies 
!       such as USAF, Navy, and DARPA etc. The code is provided as it is and Advanced 
!       Dynamics Corporation will not be responsible for any liability for usage of the 
!       code and any technical support, including the loss of any kind due to the use of 
!       this code, and the user uses it at your own risk.
!
!                            Copyright is protected from 2009-2030
!--------------------------------------------------------------------------------------
! Modified records:
! Date            reviser 
! YYYY/MM/DD      XXXX
!  
! -------------------------------------------------------------------------------------
! Indentation:
!   tab = 4
!
=====================================================================================*/

#ifndef __vtkVISCameraManager_h
#define __vtkVISCameraManager_h
#include <iostream>

using namespace std;

#include <string>

#include "vtkVISManager.h"
#include "vtkVISCommonWin32Header.h"
#include "vtkRenderer.h"
#include "vtkCamera.h"
#include "vtkTransform.h"
#include "vtkObjectFactory.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleSwitch.h"
//#include "vtkDoubleArray.h"
#include "vtkObjectFactory.h"

// --------------------------------------------------------------------------
/// A class that controls the active camera
///
/// Use functions in this class so we can control the camera via 
/// buttons in ASTE-P2.0 gui.
/// Instantiate after renderer created
/// 
/// @author HUANG Jiaqi
/// @date 2010-05-31
/// @reviser
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
    void TransformByXYZ(double x,double y, double z);
    void ScaleByXYZ(double x,double y, double z);
    void RotateWXYZ(double angle,double x,double y, double z);
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
    //void GetCurrentInteractorStyle();

    void ParallelProjectionOn();
    void ParallelProjectionOff();
    int GetProjectionMethod();
    
protected:

    vtkCamera *camera_;
    int xIndex;
    int yIndex;
    vtkTransform *trans_;

    /// direction flag
    /// @li 0 - X
    /// @li 1 - Y
    /// @li 2 - Z
    int dFlag;
    //double transIncr;
};

#endif
