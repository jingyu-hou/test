/*=================================================================================

! Module name = Visualization
!
! File name = vtkVISCaptureFigure.h
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

#ifndef __vtkVISCaptureFigure_h
#define __vtkVISCaptureFigure_h
#include <iostream>

using namespace std;

#include <string>

#include "vtkObject.h"
#include "vtkVISCommonWin32Header.h"
#include "vtkVISManager.h"
#include "vtkRenderLargeImage.h"
#include "vtkJPEGWriter.h"
#include "vtkBMPWriter.h"
#include "vtkPNGWriter.h"
#include "vtkTIFFWriter.h"
#include "vtkPNMWriter.h"

// --------------------------------------------------------------------------
/// vtkVISCapture Class
/// 
/// @author ZHU Qin
/// @date 2010-06-04
/// @reviser 
class VTK_VIS_COMMON_EXPORT vtkVISCaptureFigure : public vtkVISManager
{
public:
    static vtkVISCaptureFigure *New();
    vtkTypeRevisionMacro(vtkVISCaptureFigure, vtkVISManager);
    
    vtkVISCaptureFigure();
    ~vtkVISCaptureFigure();    
    
    void SetCaptureFigureSource(vtkRenderer *render);
    void ModifyMagnificationFactor(int factor);
    int GetMagnificationFactor();
        
    void CaptureRenderFigureAsJPEG(char* imageName);
    void CaptureRenderFigureAsBMP(char* imageName);
    void CaptureRenderFigureAsPNG(char* imageName);
    void CaptureRenderFigureAsTIFF(char* imageName);
    void CaptureRenderFigureAsPNM(char* imageName);
         
protected:
    
    vtkRenderLargeImage *renderImage_;
        
    int magFactor_;
    
private:
    
    
};

#endif 
