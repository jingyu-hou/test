/*=================================================================================

! Module name = Visualization
!
! File name = vtkVISOrientationMarker.h
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

#ifndef __vtkVISOrientationMarker_h
#define __vtkVISOrientationMarker_h
#include <iostream>

using namespace std;

#include <string>

#include "vtkVISManager.h"
#include "vtkAnnotatedCubeActor.h"
#include "vtkAxesActor.h"
#include "vtkOrientationMarkerWidget.h"
#include "vtkProperty.h"
#include "vtkCaptionActor2D.h"
#include "vtkTextProperty.h"
#include "vtkPropAssembly.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkObjectFactory.h"


// --------------------------------------------------------------------------
/// Class that provides methods for axes marker displaying
/// 
/// Use after renderer created
/// 
/// @author HUANG Jiaqi
/// @date 2010-05-31
/// @reviser 
class VTK_VIS_COMMON_EXPORT vtkVISOrientationMarker : public vtkVISManager
{
public:
    static vtkVISOrientationMarker *New();
    vtkTypeRevisionMacro(vtkVISOrientationMarker, vtkVISManager);

    vtkVISOrientationMarker();
    ~vtkVISOrientationMarker();

    void CreateOMDisplay();
	//void DeleteOMDisplay();

    void CreateOMDisplay_FD();
    void ToggleAxesDisplay();
protected:

    vtkAnnotatedCubeActor* annActor_;
	vtkOrientationMarkerWidget *marker_;
    
    vtkAxesActor *markerAxes_;
};

#endif
