/*=================================================================================

! Module name = Visualization
!
! File name = vtkVISCommonWin32Header.h
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

#ifndef __vtkVISCommonWin32Header_h
#define __vtkVISCommonWin32Header_h

#include <vtkVISConfigure.h>

#if defined(WIN32) && !defined(VTKVIS_STATIC)
#if defined(vtkVISCommon_EXPORTS)
#define VTK_VIS_COMMON_EXPORT __declspec( dllexport ) 
#else
#define VTK_VIS_COMMON_EXPORT __declspec( dllimport ) 
#endif
#else
#define VTK_VIS_COMMON_EXPORT
#endif

#endif
