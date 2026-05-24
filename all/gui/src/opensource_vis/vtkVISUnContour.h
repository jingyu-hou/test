// Open-source replacement for vtkVISUnContour
#ifndef VTK_VIS_UN_CONTOUR_OS_H
#define VTK_VIS_UN_CONTOUR_OS_H

#include <iostream>
#include <string>

using namespace std;

#include "Win32Header.h"
#include "vtkVISUnstructuredGridManager.h"

#include <vtkGeometryFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkFloatArray.h>

#include <vtkWarpVector.h>
#include <vtkVectorDot.h>

#include <vtkScalarBarWidget.h>
#include <vtkScalarBarActor.h>

#include <vtkLookupTable.h>
#include <vtkTextProperty.h>
#include <vtkCellData.h>
#include <vtkDataSetMapper.h>
#include <vtkScalarBarRepresentation.h>
#include <vtkContourFilter.h>
#include <vtkBandedPolyDataContourFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkActor.h>
#include <vtkPointData.h>
#include <vtkRenderWindowInteractor.h>

class VTK_VIS_UNSTRUCTUREDGRID_EXPORT vtkVISUnContour : public vtkVISUnstructuredGridManager
{
public:
    static vtkVISUnContour *New();
    vtkTypeRevisionMacro(vtkVISUnContour, vtkVISUnstructuredGridManager);

    vtkVISUnContour();
    ~vtkVISUnContour();

    void CreateContourDisplay(char* scalarName);
    void ModifyContourDisplay(char* scalarName);
    void CreateContourDisplay(char* scalarName, char* vectorName);
    void ModifyContourDisplay(char* scalarName, char* vectorName);
    void CreateScalarBar();
    void ShowOnScalarBar();
    void ShowOffScalarBar();
    void ModifyScalarBarFont(char *fontName);
    void ModifyScalarBarFontColor(double r, double g, double b);
    void ModifyScalarBarTitle(char *title);
    void DeleteScalarBar();
    void ModifyContourLevel(int level);

    void SetScalarBarEnable(bool enable);

    void SetContourType(int type);
    void ModifyContourDisplayType();

    void SetDeformation(int flag);
    void ModifyScaleFactor(double value);

    void SetContourRepresentationFlag(int flag);
    void ModifyContourRepresentation(int flag);

    double GetWarpScaleFactor(char* vectorName);
    void SetWarpScaleFactor(double value);

    void SetContourLevel(int level);
    void ClippingOnContourDisplay();
    void ClippingOffContourDisplay();

    void ModifyDisplayScalarRange(double min, double max);
    void DeleteObjects();

protected:
    vtkLookupTable* contourLookupTable_;
    vtkScalarBarWidget* scalarBarWidget_;

    int contourScalarIndex_;
    int vectorIndex_;
    int contourLevel_;
    double scaleFactor_;

    double scalarRange_[2];

    vtkWarpVector* csdVectorWarp_;
    vtkBandedPolyDataContourFilter* conFilter_;
    vtkContourFilter* lineContourFilter_;
    vtkActor* _contourActor;

    int contourType_;
    int deformation_;
    int representFlag_;
    bool barEnable_;
};

#endif
