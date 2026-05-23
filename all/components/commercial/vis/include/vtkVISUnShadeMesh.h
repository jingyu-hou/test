/*=================================================================================

! Module name = Visualization
!
! File name = vtkVISUnShadeMesh.h
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

#ifndef __vtkVISUnShadeMesh_h
#define __vtkVISUnShadeMesh_h
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#include "vtkVISManager.h"
#include "vtkVISUnstructuredGridManager.h"

#include "vtkDataSetMapper.h"
#include "vtkGeometryFilter.h"
#include "vtkCell.h"
#include "vtkPoints.h"
#include "vtkIdList.h"
#include "vtkExtractEdges.h"

// --------------------------------------------------------------------------
/// Class that use to manage shade/mesh rendering
/// 
/// When CreateShadeMeshDisplay(), flag=0 will create the mesh, 1 for shade
/// 
/// @author ZHU Qin
/// @date 2010-06-01
/// @reviser 
class VTK_VIS_UNSTRUCTUREDGRID_EXPORT vtkVISUnShadeMesh : public vtkVISUnstructuredGridManager
{
public:

    static vtkVISUnShadeMesh *New();
    vtkTypeRevisionMacro(vtkVISUnShadeMesh, vtkVISUnstructuredGridManager);

    vtkVISUnShadeMesh();
    ~vtkVISUnShadeMesh();
    
    void CreateShadeMeshDisplay(bool flag);

	//2012.07.10	Shi Ting add begin
	void CreateShadeMeshSurfaceDisplay(bool flag);
	//2012.07.10	Shi Ting add end
    void SetMeshLineWidth(double widthValue);
    double GetMeshLineWidth();
    void ChangeMeshLinePattern(char* pattern);

	vtkActor* CreateCellIdDisplay(int id);
    void ModifiedCellDisplayColor(double r, double g, double b);
    
protected:

    double meshLineWidth_;
    bool shadeFlag_;
    
    vtkActor* cellActor_;
    
private:

};

#endif 
