/*=================================================================================

! Module name = Visualization
!
! File name = vtkVISUnstructuredGridManager.h
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

#ifndef __vtkVISUnstructuredGridManager_h
#define __vtkVISUnstructuredGridManager_h
#include <iostream>
using namespace std;

#include "vtkVISManager.h"
#include "vtkObjectFactory.h"
#include "vtkVISUnstructuredGridWin32Header.h"
#include "vtkVISUnstructuredGridSource.h"
#include "vtkProperty.h"
#include "vtkActor.h"

// --------------------------------------------------------------------------
/// Base class of the unstructured grid render classes
/// 
/// To use any child of this class, remember to SetDataSource() first
/// 
/// @author ZHU Qin
/// @date 2010-06-01
/// @reviser 
class VTK_VIS_UNSTRUCTUREDGRID_EXPORT vtkVISUnstructuredGridManager : public vtkVISManager
{
public:

    static vtkVISUnstructuredGridManager *New();
    vtkTypeRevisionMacro(vtkVISUnstructuredGridManager, vtkVISManager);

    vtkVISUnstructuredGridManager();
    ~vtkVISUnstructuredGridManager();
    
    void SetDataSource(vtkVISUnstructuredGridSource* object);
    
    void ShowOn();
    void ShowOff();

	void ChangeRepresentation(char* style);      //2012.8.7 hebingli add
    
    void SetOpacity(double value);
    double GetOpacity();
    
    void SetColor(double r, double g, double b);
    double* GetColor();
    
    void DeleteObjects();
     // kongjin modify funtion GetActor access protected to public 
    vtkActor* GetActor();   
protected:
    
    vtkVISUnstructuredGridSource* _source;
    
    int _vtkObjectsNum;
    
    vtkActor* _unActor;       
    vtkObject** _vtkObjects;
	
	
       
private:

};

#endif 
