/*=================================================================================

! Module name = Visualization
!
! File name = vtkVISUnGlyph.h
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

#ifndef __vtkVISUnGlyph_h
#define __vtkVISUnGlyph_h
#include <iostream>
using namespace std;

#include "vtkVISManager.h"
#include "vtkVISUnstructuredGridManager.h"

#include "vtkOutlineFilter.h"
#include "vtkDataSetMapper.h"

#include "vtkGlyph3D.h"
#include "vtkGlyphSource2D.h"
#include "vtkLookupTable.h"

class VTK_VIS_UNSTRUCTUREDGRID_EXPORT vtkVISUnGlyph : public vtkVISUnstructuredGridManager
{
public:

    static vtkVISUnGlyph *New();
    vtkTypeRevisionMacro(vtkVISUnGlyph, vtkVISUnstructuredGridManager);

    vtkVISUnGlyph();
    ~vtkVISUnGlyph();
       
    void CreateGlyphDisplay(bool flag, char* scalarName, char* vectorName);
    void ModifyGlyphScalar(char* scalarName);
    void ModifyGlyphVector(char* vectorName);
    void SetScalarVisibilityOn();
    void SetScalarVisibilityOff();
    void SetGlyphFilledOn();
    void SetGlyphFilledOff();
    void SetGlyphPosition(double p1, double p2, double p3);

    void SetGlyphScaleMode(char *scaleMode);
    void SetGlyphScaleFactor(double factor);

    void SetGlyphColorMode(char *colorMode);
    void SetGlyphVectorMode(char *vectorMode);
    void SetGlyphIndexMode(char *indexMode);
    void SetGlyph2DType(char *glyphType);
    
    void ChangeGlyphLineThickness(double value);
    double GetGlyphLineThickness();

    void SetGlyphClampingOn();
    void SetGlyphClampingOff();
    void SetGlyphOrientOn();
    void SetGlyphOrientOff();
    void SetGlyphScalingOn();
    void SetGlyphScalingOff();

	//------------------------------
	//ZHUQin 2011.01.24

	double GetScaleFactor(char* vectorName);

	//ZhuQin 2011.03.23
	void ModifyDisplayScalarRange(double minScalar, double maxScalar);
    
protected:
    bool vectorFlag_;
    double scaleFactor_;
    double glyphPos_[3];
    double lineThickness_;

    int glyphTypeIndex_;

    vtkGlyph3D* glyphVtkObjects_;

    string glyph2DType[13];
    string scaleMode[4];
    string colorMode[3];
    string indexMode[3];
    string vectorMode[3];

    int GetScaleModeIndex(string Mode);      
    int GetColorModeIndex(string Mode);      
    int GetVectorModeIndex(string Mode);      
    int GetIndexModeIndex(string Mode);  
    int GetGlyph2DTypeIndex(string type);
       
private:

};

#endif 
