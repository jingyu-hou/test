/*=================================================================================

! Module name = Visualization
!
! File name = vtkVISManager.h
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

#ifndef __vtkVISManager_h
#define __vtkVISManager_h
#include <iostream>

using namespace std;

#define MAXLEVEL 50

#include <string>

#include "vtkObject.h"
#include "vtkVISCommonWin32Header.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkDoubleArray.h"
#include "vtkObjectFactory.h"
#include <sstream>


// --------------------------------------------------------------------------
/// Base class of all vtkVIS modules
/// 
/// @author HUANG Jiaqi
/// @date 2010-05-31
/// @reviser 
class VTK_VIS_COMMON_EXPORT vtkVISManager : public vtkObject
{
public:
    static vtkVISManager *New();
    vtkTypeRevisionMacro(vtkVISManager, vtkObject);
    
    vtkVISManager();
    ~vtkVISManager();    
    void SetDataType (char* type);
    void SetRenderTo (vtkRenderer *ren, vtkRenderWindow *renWin); 

    void Translate_TkColorCode_To_VTKColorCode_All ( );

	int scalarNumber_;
	int vectorNumber_;

    const char** scalarName;
	const char** vectorName;
	
    int GetScalarNumber();
    int GetVectorNumber();
    
    int GetScalarIndex(const char* scalar);
    int GetVectorIndex(const char* vector); 
    
    const char* GetScalarName(int i);
    const char* GetVectorName(int i);
    
    int* scalarNumber;
    int* vectorNumber;

	int GetScalarIndex(int scalarNum);
    int GetVectorIndex(int vectorNum);

    void ErrorInfo(bool error, string message);

protected:
    
    string _dataType;
    
    string colorPalette[20];
    double vtkColorPalette[20][3];
    
    vtkRenderer *_renderer;
    vtkRenderWindow *_renWin;

private:
    
    
};

// --------------------------------------------------------------------------
/// Prints the error or info messages to stdout/stderr
/// 
/// @param error
/// @li 0 info
/// @li 1 error
/// @param message
/// 
/// @return 
/// 
/// @author HUANG Jiaqi
/// @date 2010-05-31
/// @reviser 
inline void vtkVISManager::ErrorInfo(bool error, string message)
{
	if (error)
	{
		cerr<<"VISError: "<<this->GetClassName()<<": "<<message<<endl;
	}
//#ifndef NDEBUG
	else
	{
		cout<<"VISInfo:  "<<this->GetClassName()<<": "<<message<<endl;
	}
//#endif
}

#endif 
