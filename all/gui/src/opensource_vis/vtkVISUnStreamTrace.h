// Open-source stub for vtkVISUnStreamTrace (not directly used by consumers)
#ifndef VTK_VIS_UN_STREAM_TRACE_OS_H
#define VTK_VIS_UN_STREAM_TRACE_OS_H

#include "Win32Header.h"
#include "vtkVISUnstructuredGridManager.h"
#include <vtkStreamTracer.h>
#include <vtkRungeKutta4.h>
#include <vtkTubeFilter.h>
#include <vtkRibbonFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkCollection.h>
#include <vtkActorCollection.h>
#include <vtkPolyData.h>
#include <vtkPointWidget.h>
#include <vtkLineWidget.h>
#include <vtkPlaneWidget.h>
#include <vtkSphereWidget.h>
#include <string>

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
    void SetSeedsType(char* type);
    void ModifyStreamStyle(char* type);
    void ModifyIntegraDirection(char* direction);
    void ModifyLineAlignAxis(char* axis);
    void ModifyLineSeedsNumber(int num);
    int GetLineSeedsNumber();
    void ModifyMinimumStep(double val);
    void ModifyMaxPropagation(double val);
    void ModifyInitialStep(double val);
    void ModifyTubeSides(int num);
    void ModifyTubeRadius(double val);
    void ModifyRibbonWidth(double val);
    void ModifyRibbonFactor(double val);
    void ModifySphereTheta(int val);
    void ModifySpherePhi(int val);
    void ModifySphereRadius(double val);
    void ShowOnStreamWidget();
    void ShowOffStreamWidget();
    void ModifySeedsWidgetHandleSize(double val);
    void ShowOnStreamTraceLines();
    void ShowOffStreamTraceLines();
    void SetStreamLineStartPosition(double p0, double p1, double p2);
    void ModifyDispalyScalarRange(double min, double max);

    vtkGetVector3Macro(startPosition, double);
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

protected:
    int lineSeedsNumber;
    double MinimumStep, MaxPropagation, InitialStep;
    double tubeRadius, ribbonWidth, ribbonFactor, sphereRadius;
    int tubeSides, sphereTheta, spherePhi;
    double startPosition[3];
    int scalarIndex_, vectorIndex_;
    std::string streamStyle_, InteDirection_, lineAlignAxis_, seedstype_;
    vtkUnstructuredGrid* streamSource_;
    vtkRungeKutta4* integrator4_;
    vtkStreamTracer* streamTracer_;
    vtkTubeFilter* tubeFilter_;
    vtkRibbonFilter* ribbonFilter_;
    vtkPolyDataMapper* streamMapper_;
    vtkActor* streamActor_;
    vtkCollection* oneIntegrator4_;
    vtkCollection* oneStreamMapper_;
    vtkActorCollection* oneStreamActor_;
    vtkCollection* oneStreamTracer_;
    vtkCollection* oneTubeFilter_;
    vtkCollection* oneRibbonFilter_;
    vtkPolyData* streamSeeds_;
    vtkPointWidget* pointWidget_;
    vtkLineWidget* lineWidget_;
    vtkPlaneWidget* planeWidget_;
    vtkSphereWidget* sphereWidget_;
    static void GenerateStreamLines(vtkObject*, unsigned long, void*, void*);
    static void AlignLineWidget(vtkObject*, unsigned long, void*, void*);
};

#endif
