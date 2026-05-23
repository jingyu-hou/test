/*=================================================================================

! Module name = Visualization
!
! File name = vtkVISUnStreamTrace.h
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

#ifndef __vtkVISUnStreamTrace_h
#define __vtkVISUnStreamTrace_h
#include <iostream>
using namespace std;

#include "vtkVISManager.h"
#include "vtkVISUnstructuredGridManager.h"

#include "vtkLineWidget.h"
#include "vtkMultiBlockDataSet.h"
#include "vtkRungeKutta4.h"
#include "vtkPolyData.h"
#include "vtkStreamTracer.h"
#include "vtkPolyDataMapper.h"

#include "vtkTextWidget.h"
#include "vtkRibbonFilter.h"
#include "vtkCallbackCommand.h"
#include "vtkCommand.h"
#include "vtkPointWidget.h"
#include "vtkTubeFilter.h"
#include "vtkDataSet.h"
#include "vtkPlaneWidget.h"
#include "vtkSphereWidget.h"
#include "vtkActorCollection.h"
#include "vtkCollection.h"
#include "vtkLookupTable.h"
//----------------------------------------------------------------------------
/// Class that manage the streamtrace rendering
/// 
/// To CreateStreamTraceDisplay(), you need to:\n
/// SetDataSource() --> SetStreamTraceSource() --> CreateOneStreamLine
/// --> CreateStreamTraceDisplay()
/// 
/// @author ZHU Qin
/// @date 2010-06-01
/// @reviser 
class VTK_VIS_UNSTRUCTUREDGRID_EXPORT vtkVISUnStreamTrace : public vtkVISUnstructuredGridManager
{
public:

    static vtkVISUnStreamTrace *New();
    vtkTypeRevisionMacro(vtkVISUnStreamTrace, vtkVISUnstructuredGridManager);

    vtkVISUnStreamTrace();
    ~vtkVISUnStreamTrace();
    
    void SetStreamTraceSource(char* scalar, char* vector);
    
    void ModifyStreamTraceScalar(char* scalar);
    void ModifyStreamTraceVector(char* vector);
    
    void CreateStreamTraceDisplay();
    vtkActor* CreateOneStreamLine();
    
    void SetSeedsType (char* type);
    
    void ModifyStreamStyle (char* type);
    void ModifyIntegraDirection (char* direction);
    void ModifyLineAlignAxis(char* axis);
    
    void ModifyLineSeedsNumber(int num);
    int GetLineSeedsNumber();
    
    void ModifyMinimumStep(double minStep);
    void ModifyMaxPropagation(double max);
    void ModifyInitialStep(double initiStep);
    
    void ModifyTubeSides(int num);
    void ModifyTubeRadius(double radius);
    void ModifyRibbonWidth(double width);
    void ModifyRibbonFactor(double factor);
    
    void ModifySphereTheta(int theta);
    void ModifySpherePhi(int phi);
    void ModifySphereRadius(double radius);
    
    void ShowOnStreamWidget();
    void ShowOffStreamWidget();
    
    void ModifySeedsWidgetHandleSize(double size);
    
    void ShowOnStreamTraceLines();
    void ShowOffStreamTraceLines();
    
    void SetStreamLineStartPosition(double p0, double p1, double p2);
    
    vtkGetVector3Macro(startPosition,double);
    
    vtkGetMacro(MinimumStep, double);
    vtkGetMacro(MaxPropagation, double);
    vtkGetMacro(InitialStep, double);
    
    vtkGetMacro(tubeRadius, double);
    vtkGetMacro(tubeSides, int);
    
    vtkGetMacro(ribbonWidth, double);
    vtkGetMacro(ribbonFactor, double);
    
    vtkGetMacro(sphereRadius, double);
    vtkGetMacro(sphereTheta, int);
    vtkGetMacro(spherePhi, int);

	//ZhuQin 2011.03.24
	void ModifyDispalyScalarRange(double min, double max);
 
protected:
    int lineSeedsNumber;    
    
    double MinimumStep;
    double MaxPropagation;
    double InitialStep;    
    
    double tubeRadius;
    int tubeSides;
    double ribbonWidth;
    double ribbonFactor; 
    
    int sphereTheta;
    int spherePhi;
    double sphereRadius;
    
    double startPosition[3];       
    
    int scalarIndex_;
    int vectorIndex_;
    
    string streamStyle_;
    string InteDirection_;
    
    string lineAlignAxis_;
    
    vtkUnstructuredGrid* streamSource_;
    vtkRungeKutta4* integrator4_;
    vtkStreamTracer* streamTracer_;
    vtkTubeFilter* tubeFilter_;
    vtkRibbonFilter* ribbonFilter_;
    vtkPolyDataMapper* streamMapper_;
    vtkActor* streamActor_;
    
 //   vtkActor* oneStreamActor_;
	//vtkPolyDataMapper* oneStreamMapper_;
	vtkCollection* oneIntegrator4_;
	vtkCollection* oneStreamMapper_;
	vtkActorCollection* oneStreamActor_;
	vtkCollection* oneStreamTracer_;
	vtkCollection* oneTubeFilter_;
	vtkCollection* oneRibbonFilter_;
    
    string seedstype_;
    vtkPolyData* streamSeeds_;
              
    vtkPointWidget* pointWidget_;
    vtkLineWidget* lineWidget_;
    vtkPlaneWidget* planeWidget_;
    vtkSphereWidget* sphereWidget_;
    
    static void GenerateStreamLines(vtkObject *caller, unsigned long, void *, void *);  
    static void AlignLineWidget(vtkObject *caller, unsigned long, void *, void *);         
 
private:

};

/*string vtkVISUnStreamTrace::seedstype_ = "Line Source";
vtkPolyData* vtkVISUnStreamTrace::streamSeeds_ = NULL;

vtkPointWidget* vtkVISUnStreamTrace::pointWidget_ = NULL;
vtkPlaneWidget* vtkVISUnStreamTrace::planeWidget_ = NULL;
vtkLineWidget* vtkVISUnStreamTrace::lineWidget_ = NULL;
vtkSphereWidget* vtkVISUnStreamTrace::sphereWidget_ = NULL;*/

#endif 
