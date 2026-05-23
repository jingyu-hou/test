/*=================================================================================

! Module name = Visualization
!
! File name = vtkVISUnstructuredGridSource.h
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

#ifndef __vtkVISUnstructuredGridSource_h
#define __vtkVISUnstructuredGridSource_h
#include <iostream>
using namespace std;

#include "vtkVISManager.h"
#include "vtkObjectFactory.h"
#include "vtkVISUnstructuredGridWin32Header.h"
#include <vector>

#include "vtkUnstructuredGridReader.h"
#include "vtkUnstructuredGrid.h"
#include "vtkDoubleArray.h"
#include "vtkPointData.h"
#include "vtkCellData.h"

#include "vtkIdList.h"
#include "vtkUnstructuredGridWriter.h"

// --------------------------------------------------------------------------
/// Class that manage the unstructured grid data source
///
/// LoadData() once instantiate this class, this will 
/// loads all the data containning in the file.
/// 
/// @author ZHU Qin
/// @date 2010-06-01
/// @reviser 
class VTK_VIS_UNSTRUCTUREDGRID_EXPORT vtkVISUnstructuredGridSource : public vtkVISManager
{
public:

    static vtkVISUnstructuredGridSource *New();
    vtkTypeRevisionMacro(vtkVISUnstructuredGridSource, vtkVISManager);

    vtkVISUnstructuredGridSource();
    ~vtkVISUnstructuredGridSource();
    
    vtkUnstructuredGrid* unstruGrid;
    vtkDoubleArray** scalarSource;
    vtkDoubleArray** vectorSource;
    double** scalarRange;
    
    vtkDoubleArray** cellScalarSource_;
    vtkDoubleArray** cellVectorSource_;

    void LoadData(char* fileName);

    void LoadScalarData();
    void LoadVectorData();
    
    void LoadOneScalarData(const char* scalarName);
    void LoadOneVectorData(const char* vectorName);
    
    void LoadCellScalarData();
    void LoadCellVectorData();
    
    void LoadOneCellScalarData(const char* scalarName);
    void LoadOneCellVectorData(const char* vectorName);
    
	double* GetSourceBounds();
    double* GetSourceCenter();

	vtkUnstructuredGrid* GetGrid();

	//ZhuQin 2011.04.12
	double* GetScalarRange(const char* scalarName);

	//ZhuQin 2011.05.27
	void CleanUnGridData();

	vtkDoubleArray* GetScalarSource(int index);
    vtkDoubleArray* GetVectorSource(int index);

    /////////////////////////////////////////
    void SetScalarNumber(int num);
    void SetScalarsName(const char **scalarsName);
    void SetSourceGrid(vtkUnstructuredGrid* source);
    void SetScalarsSource(vtkDoubleArray** scalarsSource);
    void SetScalarsRange(double** scalarsRange);
    /////////////////////////////////////////
protected:

    vtkUnstructuredGridReader *unstruReader_;

	//ZhuQin 2011.05.27
	void DeleteOnePoint(vtkPoints* &points, int index);

private:

};

#endif 
