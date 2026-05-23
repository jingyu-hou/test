/*=================================================================================

! Module name = Visualization
!
! File name = vtkVISUnContour.h
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

#ifndef __vtkVISUnContour_h
#define __vtkVISUnContour_h


#include <iostream>
#include <string>

using namespace std;

#include "vtkVISManager.h"
#include "vtkVISUnstructuredGridManager.h"

#include "vtkGeometryFilter.h"
#include "vtkPolyDataNormals.h"
#include "vtkFloatArray.h"

#include "vtkWarpVector.h"
#include "vtkVectorDot.h"

#include "vtkScalarBarWidget.h"
#include "vtkScalarBarActor.h"

#include "vtkLookupTable.h"
#include "vtkTextProperty.h"
#include "vtkCellData.h"
#include "vtkDataSetMapper.h"
#include "vtkScalarBarRepresentation.h"
#include "vtkContourFilter.h"
#include "vtkBandedPolyDataContourFilter.h"

// --------------------------------------------------------------------------
/// Class that manage contour display of the unstructured grid data
/// 
/// 
/// @author HUANG Jiaqi
/// @date 2010-05-31
/// @reviser 
class VTK_VIS_UNSTRUCTUREDGRID_EXPORT vtkVISUnContour : public vtkVISUnstructuredGridManager
{
public:

    static vtkVISUnContour *New();
    vtkTypeRevisionMacro(vtkVISUnContour, vtkVISUnstructuredGridManager);

    vtkVISUnContour();
    ~vtkVISUnContour();
       
    void CreateContourDisplay(char* scalarName);
    void ModifyContourDisplay(char* scalarName);

	void CreateContourDisplay(char* scalarName,char* vectorName);
    void ModifyContourDisplay(char* scalarName,char* vectorName);
    void CreateScalarBar();
    void ShowOnScalarBar();
    void ShowOffScalarBar();
    void ModifyScalarBarFont(char *fontName);
    void ModifyScalarBarFontColor(double r, double g, double b);
    void ModifyScalarBarTitle(char *title);
    void DeleteScalarBar();
    void ModifyContourLevel(int level);

	//2011.0.26
	void SetScalarBarEnable(bool enable);
    
	//---------------------------------------------
	//2011.01.08

	//0 -- banded
	//1 -- continuous
	//2 -- lines
	void SetContourType(int type);

	void ModifyContourDisplayType();

	void SetDeformation(int flag);
	void ModifyScaleFactor(double value);

	void SetContourRepresentationFlag(int flag);
	void ModifyContourRepresentation(int flag);

	//------------------------------
	//ZHUQin 2011.01.22

	double GetWarpScaleFactor(char* vectorName);

	void SetWarpScaleFactor(double value);

	//------------------------------
	//ZHUQin 2011.02.27
	void SetContourLevel(int level);

	//ZHUQin 2011.03.24
	void ClippingOnContourDisplay();
	void ClippingOffContourDisplay();

	void ModifyDisplayScalarRange(double min, double max);
    
    //reimplement parent class method.
    void DeleteObjects();

protected:
    vtkLookupTable* contourLookupTable_;
    vtkScalarBarWidget* scalarBarWidget_;

    int contourScalarIndex_;
	int vectorIndex_;
    int contourLevel_;
	double scaleFactor_;

   // double csdScaleFactor_;
	double scalarRange_[2];

    vtkWarpVector* csdVectorWarp_;

	vtkBandedPolyDataContourFilter* conFilter_;

	//---------------------------------------------
	//2011.01.08
    //0 -- banded
	//1 -- continuous
	//2 -- lines
	int contourType_;
    // 0 -- no deformation
    // 1 -- deformation
	int deformation_;

	// 0 -- points
    // 1 -- surface
    // 2 -- wireframe
	int representFlag_;

	// 0 -- off
	// 1 -- on
	bool barEnable_;

private:

};

#endif 
