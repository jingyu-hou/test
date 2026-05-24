// Open-source stub for vtkVISUnSlice (commented out in consumer code)
#ifndef VTK_VIS_UN_SLICE_OS_H
#define VTK_VIS_UN_SLICE_OS_H

#include "Win32Header.h"
#include "vtkVISUnstructuredGridManager.h"
#include <vtkPlane.h>
#include <vtkCutter.h>
#include <vtkImplicitPlaneWidget.h>
#include <vtkLookupTable.h>
#include <vtkBandedPolyDataContourFilter.h>
#include <vtkContourFilter.h>
#include <vtkFloatArray.h>
#include <vtkCollection.h>
#include <vtkGlyphSource2D.h>
#include <vtkGlyph3D.h>

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

    // Sub-display creation
    void CreateUnSliceContourDisplay();
    void ShowOnUnSliceContourDisplay();
    void ShowOffUnSliceContourDisplay();
    void DeleteUnSliceContourDisplay();
    void CreateUnSliceContourLinesDisplay();
    void ShowOnUnSliceContourLinesDisplay();
    void ShowOffUnSliceContourLinesDisplay();
    void DeleteUnSliceContourLinesDisplay();
    void CreateUnSliceContourLinesLabelDisplay();
    void DeleteUnSliceContourLinesLabelDisplay();
    void CreateUnSliceVectorDisplay();
    void ShowOnUnSliceVectorDisplay();
    void ShowOffUnSliceVectorDisplay();
    void DeleteUnSliceVectorDisplay();
    void CreateUnSliceShadeDisplay();
    void ShowOnUnSliceShadeDisplay();
    void ShowOffUnSliceShadeDisplay();
    void CreateUnSliceMeshDisplay();
    void ShowOnUnSliceMeshDisplay();
    void ShowOffUnSliceMeshDisplay();

    void SetUnSliceContourOpacity(double value);
    double GetUnSliceContourOpacity();
    void ModifyUnSliceVectorColorMode(char* mode);
    void ModifyUnSliceVectorScaleMode(char* mode);
    void ModifyUnSliceVectorScaleFactor(double factor);
    void SetUnSliceVectorFilledOn();
    void SetUnSliceVectorFilledOff();
    void ModifyUnSliceVectorPosition(double d0, double d1, double d2);
    void ModifyUnSliceShadeOpacity(double value);
    void ModifyUnSliceMeshLineWidth(double lineWidth);
    void ModifyUnSliceContourLinesWidth(double width);
    void ModifyUnSliceContourLevel(int level);
    void ModifyUnSliceShadeColor(double c0, double c1, double c2);
    void ModifyUnSliceMeshColor(double c0, double c1, double c2);

    vtkImplicitPlaneWidget* GetSlicePlaneWidget();
    vtkPlane* GetSlicePlane();
    void WriteScalarVectorData(const char* FileName);
    vtkFloatArray* GetCutScalarData();
    vtkFloatArray* GetCutVectorData(int index);
    vtkFloatArray* GetCutVectorData();
    vtkFloatArray* GetCutPointData(int index);
    void ModifyDisplayScalarRange(double min, double max);
    void ClippingOnSliceContourDisplay();
    void ClippingOffSliceContourDisplay();

    // FD methods
    void SetSliceSource_FD(char* scalar, char* vector);
    void CreateSliceWidget_FD();
    static void GenerateSliceProData_FD(vtkObject *caller, unsigned long, void *, void *);
    void CreateUnSliceContourDisplay_FD();
    void CreateUnSliceContourLinesDisplay_FD();
    void CreateUnSliceContourLinesLabelDisplay_FD();
    void ModifyDisplayScalarRange_FD(double min, double max);
    void ModifySliceSourceScalar_FD(char* scalar);
    bool GetSourceBounds_FD(double *bounds);

    bool SetBoundSliceSource(vtkUnstructuredGrid* uGrid, double y);
    bool GetXMaxPointData(double *xMax, vtkPoints* Points);

protected:
    double planeNormal_[3], planeOrigin_[3];
    int conLevel_, usedScalarIndex_, usedVectorIndex_;
    vtkUnstructuredGrid* gridSource_;
    vtkCutter* cutSource_;
    vtkActor* cutActor_;
    double scalarRange_[2];
    vtkBandedPolyDataContourFilter* bandedContourFilter_;
    vtkActor** sliceActors_;
    vtkCollection* sliceMapper_;
    vtkLookupTable** sliceTable_;
    vtkContourFilter* contourLinesFilter_;
    vtkContourFilter* contourLabelFilter_;
    vtkGlyphSource2D* sliceGlyph2D_;
    vtkGlyph3D* sliceGlyph3D_;
    double shadeColor_[3], meshColor_[3];
    vtkPlane* slicePlane_;
    vtkImplicitPlaneWidget* planeWidget_;
    static void GenerateSliceProData(vtkObject *caller, unsigned long, void *, void *);

private:
    vtkDoubleArray *scalarSource_;
};

#endif
