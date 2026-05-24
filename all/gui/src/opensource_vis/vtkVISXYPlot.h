// Open-source replacement for vtkVISXYPlot
#ifndef VTK_VIS_XYPLOT_OS_H
#define VTK_VIS_XYPLOT_OS_H
#include <iostream>

using namespace std;

#include "Win32Header.h"
#include "vtkVISManager.h"
#include <vtkObjectFactory.h>

#include <vtkXYPlotActor.h>
#include <vtkProperty2D.h>
#include <vtkTextProperty.h>
#include <vtkDataSet.h>
#include <vtkGlyphSource2D.h>
#include <vtkSphereSource.h>
#include <vtkFieldData.h>
#include <vtkDataObject.h>
#include <vtkFloatArray.h>
#include <vtkProperty2D.h>
#include <vtkLegendBoxActor.h>

#include <vtkConeSource.h>
#include <vtkCubeSource.h>
#include <vtkLineSource.h>

class VTK_VIS_XYPLOT_EXPORT vtkVISXYPlot : public vtkVISManager
{
public:
    static vtkVISXYPlot *New();
    vtkTypeRevisionMacro(vtkVISXYPlot, vtkVISManager);

    vtkVISXYPlot();
    ~vtkVISXYPlot();

    void CreateBlankXYPlotActor();
    void AddXYPlotData(vtkDataSet* data);
    void RemoveXYPlotData(vtkDataSet* data);
    void AddXYPlotDataObject(int index, vtkDataObject* dataObj);
    void RemoveXYPlotDataObject(vtkDataObject* dataObj);

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

    void CreateLegendBoxActor();
    void ShowOn();
    void ShowOff();
    vtkXYPlotActor* GetXYPlotActor();

protected:
    vtkXYPlotActor* XYPlot_;
    string xValues[4];
    int plotNumber_;
    vtkLegendBoxActor* legend_;
    int GetXValueIndex(char* type);
};

#endif
