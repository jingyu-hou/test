/*=================================================================================

! Module name = Visualization
!
! File name = vtkVISUnSlice.h
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

#ifndef __vtkVISUnSlice_h
#define __vtkVISUnSlice_h
#include <iostream>
using namespace std;

#include "vtkVISManager.h"
#include "vtkVISUnstructuredGridManager.h"

#include "vtkPlane.h"
#include "vtkCutter.h"
#include "vtkImplicitPlaneWidget.h"
#include "vtkCallbackCommand.h"
#include "vtkExtractCells.h"
#include "vtkExtractEdges.h"
#include "vtkDataSetMapper.h"
#include "vtkContourFilter.h"
#include "vtkLookupTable.h"
#include "vtkLabeledDataMapper.h"
#include "vtkTextProperty.h"
#include "vtkPoints.h"
#include "vtkMaskPoints.h"
#include "vtkSelectVisiblePoints.h"
#include "vtkActor2D.h"
#include "vtkCollection.h"
#include "vtkGlyphSource2D.h"
#include "vtkGlyph3D.h"
#include "vtkUnstructuredGridGeometryFilter.h"
#include "vtkBandedPolyDataContourFilter.h"
#include "vtkGeometryFilter.h"
#include "vtkFloatArray.h"
//#include "vtkVISUnPFrontAndPBehindSource.h"
// --------------------------------------------------------------------------
/// Class that manage slice rendering
/// 
/// To CreateSliceDisplay(), you need to do these first \n
/// SetDataSource(), then SetSliceSource(),CreateSliceWidget() 
/// 
/// @author HUANG Jiaqi
/// @date 2010-06-01
/// @reviser 
class VTK_VIS_UNSTRUCTUREDGRID_EXPORT vtkVISUnSlice : public vtkVISUnstructuredGridManager
{
public:

    static vtkVISUnSlice *New();
    vtkTypeRevisionMacro(vtkVISUnSlice, vtkVISUnstructuredGridManager);

    vtkVISUnSlice();
    ~vtkVISUnSlice();
    
    void SetSliceSource(char* scalar, char* vector);
    void ModifySliceSourceScalar(char* scalar);
    void ModifySliceSourceVector(char* vector);
    
    void CreateSliceWidget();
    
    void ShowOnSlicePlane();
    void ShowOffSlicePlane();
    
    void ShowOnSlicePlaneWidget();
    void ShowOffSlicePlaneWidget();
    
    void ModifySlicePlaneWidgetHandleSize(double size);
    
    void SetSlicePlaneNormal(double nx, double ny, double nz);
    void SetSlicePlaneNormalToX();
    void SetSlicePlaneNormalToY();
    void SetSlicePlaneNormalToZ();
    
    double* GetSlicePlaneNormal();
    
    void SetSlicePlaneOrigin(double ox, double oy, double oz);
    double* GetSlicePlaneOrigin();
    
	//--------------------------------------------
	// 2011.01.08

    //void CreateSliceDisplay();
	void CreateUnSliceContourDisplay();
	void ShowOnUnSliceContourDisplay();
	void ShowOffUnSliceContourDisplay();
	void DeleteUnSliceContourDisplay();

	void CreateUnSliceContourLinesDisplay();
	void ShowOnUnSliceContourLinesDisplay();
	void ShowOffUnSliceContourLinesDisplay();
	void DeleteUnSliceCOntourLinesDisplay();
	
	
	void CreateUnSliceContourLinesLabelDisplay();
	void ShowOnUnSliceContourLinesLabelDisplay();
	void ShowOffUnSliceContourLinesLabelDisplay();
	void DeleteUnSliceContourLinesLabelDisplay();

	
	void CreateUnSliceVectorDisplay();
	void ShowOnUnSliceVectorDisplay();
	void ShowOffUnSliceVectorDisplay();
	void DeleteUnSliceVectorDisplay();


	void CreateUnSliceShadeDisplay();
	void ModifyUnSliceShadeColor(double c0, double c1, double c2);
	void ShowOnUnSliceShade();
	void ShowOffUnSliceShade();
    
    
	void CreateUnSliceMeshDisplay();
    void ModifyUnSliceMeshColor(double c0, double c1, double c2);
    void ShowOnUnSliceMesh();
	void ShowOffUnSliceMesh();


	void SetUnSliceContourOpacity(double value);
    double GetUnSliceContourOpacity();
    
    void ModifyUnSliceVectorColorMode(char* mode);
    void ModifyUnSliceVectorScaleMode(char* mode);
    void ModifyUnSliceVectorScaleFactor(double scale);
    
    void SetUnSliceVectorFilledOn();
    void SetUnSliceVectorFilledOff();
    
    void ModifyUnSliceVectorPosition(double d0, double d1, double d2);
    
    void ModifyUnSliceShadeOpacity(double value);
    void ModifyUnSliceMeshLineWidth(double lineWidth);
    void ModifyUnSliceContourLevel(int level);
    
    void ModifyUnSliceContourLinesWidth(double lineWidth);
	
	vtkImplicitPlaneWidget* GetSlicePlaneWidget();
	vtkPlane* GetSlicePlane();
	

    void WriteScalarVectorData(const char* FileName);
    vtkFloatArray* GetCutScalarData(void);
     /*
     * cutVector[index]
     * 0    v0
     * 1    v1
     * 2    v2
     */
    vtkFloatArray* GetCutVectorData(int index);
    vtkFloatArray* GetCutVectorData(void);
    /*
     * cutPointData[index]
     * 0    x
     * 1    y
     * 2    z
     */
    vtkFloatArray* GetCutPointData(int index);

	//ZHUQin 2011.03.24
	void ModifyDisplayScalarRange(double min,double max);
	void ClippingOnSliceContourDisplay();
	void ClippingOffSliceContourDisplay();
	//add by HaoJingjing 2015.12.11 begin.
	bool SetBoundSliceSource(vtkUnstructuredGrid* uGrid,double y);
	bool GetXMaxPointData(double *xMax,vtkPoints* Points);
	//add by HaoJingjing 2015.12.11 end.
    
    ///////
    void SetSliceSource_FD(char* scalar, char* vector);
    void CreateSliceWidget_FD();
    static void GenerateSliceProData_FD(vtkObject *caller, unsigned long, void *, void *);
    void CreateUnSliceContourDisplay_FD();
    void CreateUnSliceContourLinesDisplay_FD();
    void CreateUnSliceContourLinesLabelDisplay_FD();
    void ModifyDisplayScalarRange_FD(double min,double max);
    void ModifySliceSourceScalar_FD(char* scalar);
    bool GetSourceBounds_FD(double *bounds);
    ///////
protected:

    double planeNormal_[3];
    double planeOrigin_[3];
    
    int conLevel_;
    int usedScalarIndex_;
	int usedVectorIndex_;
	
    vtkUnstructuredGrid* gridSource_;
    vtkCutter* cutSource_;
	vtkActor* cutActor_;
	//vtkDataSetMapper* cutMapper_;

	double scalarRange_[2];
	vtkBandedPolyDataContourFilter* bandedContourFilter_;

	/*
     * sliceActor[blockIndex]
     * 0 contour
     * 1 contourlines
     * 2 contourlineslabel
     * 3 vector
     * 4 shade
     * 5 mesh
     */
	vtkActor** sliceActors_;
	vtkCollection* sliceMapper_;
	vtkLookupTable** sliceTable_;


    vtkContourFilter* contourLinesFilter_;
    vtkContourFilter* contourLabelFilter_;

	vtkGlyphSource2D* sliceGlyph2D_;
    vtkGlyph3D* sliceGlyph3D_;

	double shadeColor_[3];
    double meshColor_[3];

    
    vtkPlane* slicePlane_;
    vtkImplicitPlaneWidget* planeWidget_;
    static void GenerateSliceProData(vtkObject *caller, unsigned long, void *, void *);

       
private:
    vtkDoubleArray *scalarSource_;
};

//vtkPlane *vtkVISUnSlice::slicePlane_ = NULL;
//vtkImplicitPlaneWidget *vtkVISUnSlice::planeWidget_ = NULL;

#endif 
