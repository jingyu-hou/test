/*=================================================================================

! Module name = Visualization
!
! File name = vtkVISXYPlot.h
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

#ifndef __vtkVISXYPlot_h
#define __vtkVISXYPlot_h
#include <iostream>

using namespace std;

#include "vtkVISXYPlotWin32Header.h"
#include "vtkVISManager.h"
#include "vtkObjectFactory.h"

#include "vtkXYPlotActor.h"
#include "vtkProperty2D.h"
#include "vtkTextProperty.h"
#include "vtkDataSet.h"
#include "vtkGlyphSource2D.h"
#include "vtkSphereSource.h"
#include "vtkFieldData.h"
#include "vtkDataObject.h"
#include "vtkFloatArray.h"
#include "vtkProperty2D.h"
#include "vtkLegendBoxActor.h"

#include "vtkConeSource.h"
#include "vtkCubeSource.h"
#include "vtkLineSource.h"

// --------------------------------------------------------------------------
/// Class that manage xy plot figure rendering
/// 
/// CreateBlankXYPlotActor() first 
/// so we get an vtkXYPlotActor XYPlot_ \n
/// Then adds data via AddXYPlotDataObject \n
/// Set the properties afterwards.
/// 
/// @author ZHU Qin
/// @date 2010-06-01
/// @reviser 
class VTK_VIS_XYPLOT_EXPORT vtkVISXYPlot : public vtkVISManager
{
public:

    static vtkVISXYPlot *New();
    vtkTypeRevisionMacro(vtkVISXYPlot, vtkVISManager);
    
    vtkVISXYPlot();
    ~vtkVISXYPlot();
    
    void CreateBlankXYPlotActor();
        
    void AddXYPlotData(vtkDataSet* data);
    void RemoveXYPlotData(vtkDataSet *data);

    void AddXYPlotDataObject(int index, vtkDataObject *dataObj);
    void RemoveXYPlotDataObject(vtkDataObject *dataObj);
    
    void SetPositionCoordinate(double a, double b);
    void SetPosition2Coordinate(double a, double b);
           
    void SetXYPlotTitle(char* title);
    void SetXYPlotXTitle(char* xTitle);
    void SetXYPlotYTitle(char* yTitle);
    
    void SetXYPlotActorColor(double r, double g, double b);
    
    void SetXLabelsNumber(int num);
    void SetYLabelsNumber(int num);
    
    void SetXValuesTo(char* xValue);
    void SetGlyphTypeTo(int index, char* type);
    
    void SetXYPlotLable(int index, char* label);
    void SetXYPlotColor(int index, double r, double g, double b);
    
    void SetPlotXRange(double min, double max);
    void SetPlotYRange(double min, double max);
    
    void ModifyPlotLineWidth(double Value);
 
    void SetXYPlotLegendPosition(double a, double b);
    void SetXYPlotLegendPosition2(double a, double b);
    
    void SetXYPlotTitlePosition(double a, double b);
    
    const char* GetXYPlotLable(int index);
    double* GetXYPlotColor(int index);
    double GetPlotLineWidth();
    
    void SetXYPlotGlyphSize(double size);
    double GetXYPlotGlyphSize();
       
    void RemoveXYPlotAllInputs();
    
    void SetXYPlotNumber(int num);
    int GetXYPlotNumber();
    
    void RemoveXYPlotActor();

	//----------------------------------------------------
	// ZHUQin 2011.01.21

	void  CreateLegendBoxActor();
    //------------------------------------------------------
    // kongjin 2014.04.28 
    void ShowOn();
    void ShowOff();
    vtkXYPlotActor* GetXYPlotActor();

protected:

    vtkXYPlotActor* XYPlot_;
    
    //vtkFieldData* fieldData_;
    
    string xValues[4];
    
    int plotNumber_;
    
    int GetXValueIndex(char* type);

	vtkLegendBoxActor* legend_;
    
    
private:

};

#endif 
