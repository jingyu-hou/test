/*=================================================================================

! Module name = Visualization
!
! File name = vtkVISUnEdge.h
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

#ifndef __vtkVISUnEdge_h
#define __vtkVISUnEdge_h
#include <iostream>
using namespace std;

#include "vtkVISManager.h"
#include "vtkVISUnstructuredGridManager.h"

#include "vtkOutlineFilter.h"
#include "vtkDataSetMapper.h"
#include "vtkGeometryFilter.h"
#include "vtkFeatureEdges.h"
#include "vtkPolyDataMapper.h"
// --------------------------------------------------------------------------
/// Class that manage edge displaying of unstructured grid
/// 
/// 
/// @author HUANG Jiaqi
/// @date 2010-05-31
/// @reviser 
class VTK_VIS_UNSTRUCTUREDGRID_EXPORT vtkVISUnEdge : public vtkVISUnstructuredGridManager
{
public:

    static vtkVISUnEdge *New();
    vtkTypeRevisionMacro(vtkVISUnEdge, vtkVISUnstructuredGridManager);

    vtkVISUnEdge();
    ~vtkVISUnEdge();
    
    void CreateEdgeDisplay();
    void ChangeLineWidth(double widthValue);
    double GetLineWidth();
    
    
protected:

    double lineWidth;
       
private:

};

#endif 
