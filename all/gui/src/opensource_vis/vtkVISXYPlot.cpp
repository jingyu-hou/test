// Open-source implementation of vtkVISXYPlot
#include "vtkVISXYPlot.h"
#include <vtkObjectFactory.h>
#include <vtkProperty2D.h>
#include <string.h>

vtkStandardNewMacro(vtkVISXYPlot);
vtkCxxRevisionMacro(vtkVISXYPlot, "$Revision: 1.0 $");

vtkVISXYPlot::vtkVISXYPlot()
    : XYPlot_(0), plotNumber_(0), legend_(0)
{
    xValues[0] = "Value";
    xValues[1] = "Index";
    xValues[2] = "ArcLength";
    xValues[3] = "NormalizedArcLength";
}

vtkVISXYPlot::~vtkVISXYPlot()
{
    RemoveXYPlotActor();
}

void vtkVISXYPlot::CreateBlankXYPlotActor()
{
    if (!XYPlot_) {
        XYPlot_ = vtkXYPlotActor::New();
        XYPlot_->PlotCurveLinesOn();
        XYPlot_->PlotLinesOn();
        XYPlot_->PlotPointsOff();
    }
}

void vtkVISXYPlot::AddXYPlotData(vtkDataSet* data)
{
    if (XYPlot_) XYPlot_->AddInput(data);
}

void vtkVISXYPlot::RemoveXYPlotData(vtkDataSet* data)
{
    if (XYPlot_) XYPlot_->RemoveInput(data);
}

void vtkVISXYPlot::AddXYPlotDataObject(int index, vtkDataObject* obj)
{
    if (XYPlot_) XYPlot_->AddDataObjectInput(obj);
}

void vtkVISXYPlot::RemoveXYPlotDataObject(vtkDataObject* obj)
{
    if (XYPlot_) XYPlot_->RemoveDataObjectInput(obj);
}

void vtkVISXYPlot::SetPositionCoordinate(double a, double b)
{
    if (XYPlot_) XYPlot_->SetPosition(a, b);
}

void vtkVISXYPlot::SetPosition2Coordinate(double a, double b)
{
    if (XYPlot_) XYPlot_->SetPosition2(a, b);
}

void vtkVISXYPlot::SetXYPlotTitle(char* title)
{
    if (XYPlot_) XYPlot_->SetTitle(title);
}

void vtkVISXYPlot::SetXYPlotXTitle(char* title)
{
    if (XYPlot_) XYPlot_->SetXTitle(title);
}

void vtkVISXYPlot::SetXYPlotYTitle(char* title)
{
    if (XYPlot_) XYPlot_->SetYTitle(title);
}

void vtkVISXYPlot::SetXYPlotActorColor(double r, double g, double b)
{
    if (XYPlot_) XYPlot_->GetProperty()->SetColor(r, g, b);
}

void vtkVISXYPlot::SetXLabelsNumber(int num)
{
    if (XYPlot_) XYPlot_->SetXLabelFormat("%g");
}

void vtkVISXYPlot::SetYLabelsNumber(int num)
{
    if (XYPlot_) XYPlot_->SetYLabelFormat("%g");
}

void vtkVISXYPlot::SetXValuesTo(char* xValue)
{
    if (XYPlot_) {
        int idx = GetXValueIndex(xValue);
        XYPlot_->SetXValues(idx);
    }
}

int vtkVISXYPlot::GetXValueIndex(char* type)
{
    if (!type) return 0;
    for (int i = 0; i < 4; ++i)
        if (xValues[i] == type) return i;
    return 0;
}

void vtkVISXYPlot::SetGlyphTypeTo(int index, char* type)
{
    if (!XYPlot_ || !type) return;
    vtkGlyphSource2D* glyph = vtkGlyphSource2D::New();
    if (strcmp(type, "Point") == 0)        glyph->SetGlyphTypeToVertex();
    else if (strcmp(type, "Line") == 0)    glyph->SetGlyphTypeToNone();
    else if (strcmp(type, "Dash") == 0)    glyph->SetGlyphTypeToDash();
    else if (strcmp(type, "Cross") == 0)   glyph->SetGlyphTypeToCross();
    else if (strcmp(type, "Square") == 0)  glyph->SetGlyphTypeToSquare();
    else if (strcmp(type, "Diamond") == 0) glyph->SetGlyphTypeToDiamond();
    else if (strcmp(type, "Circle") == 0)  glyph->SetGlyphTypeToCircle();
    else { glyph->Delete(); return; }
    glyph->Update();
    XYPlot_->SetPlotSymbol(index, glyph->GetOutput());
    glyph->Delete();
}

void vtkVISXYPlot::SetXYPlotLable(int index, char* label)
{
    if (XYPlot_) XYPlot_->SetPlotLabel(index, label);
}

void vtkVISXYPlot::SetXYPlotColor(int index, double r, double g, double b)
{
    if (XYPlot_) XYPlot_->SetPlotColor(index, r, g, b);
}

void vtkVISXYPlot::SetPlotXRange(double min, double max)
{
    if (XYPlot_) XYPlot_->SetXRange(min, max);
}

void vtkVISXYPlot::SetPlotYRange(double min, double max)
{
    if (XYPlot_) XYPlot_->SetYRange(min, max);
}

void vtkVISXYPlot::ModifyPlotLineWidth(double Value)
{
    if (XYPlot_) {
        XYPlot_->GetProperty()->SetLineWidth(Value);
        if (legend_) legend_->GetProperty()->SetLineWidth(Value);
    }
}

void vtkVISXYPlot::SetXYPlotLegendPosition(double a, double b)
{
    if (legend_) legend_->SetPosition(a, b);
}

void vtkVISXYPlot::SetXYPlotLegendPosition2(double a, double b)
{
    if (legend_) legend_->SetPosition2(a, b);
}

void vtkVISXYPlot::SetXYPlotTitlePosition(double a, double b)
{
    if (XYPlot_) XYPlot_->SetTitlePosition(a, b);
}

const char* vtkVISXYPlot::GetXYPlotLable(int index)
{
    if (XYPlot_) return XYPlot_->GetPlotLabel(index);
    return "";
}

double* vtkVISXYPlot::GetXYPlotColor(int index)
{
    static double c[3] = {0,0,0};
    double* pc = XYPlot_ ? XYPlot_->GetPlotColor(index) : 0;
    if (pc) { c[0] = pc[0]; c[1] = pc[1]; c[2] = pc[2]; }
    return c;
}

double vtkVISXYPlot::GetPlotLineWidth()
{
    return XYPlot_ ? XYPlot_->GetProperty()->GetLineWidth() : 1.0;
}

void vtkVISXYPlot::SetXYPlotGlyphSize(double size)
{
    if (XYPlot_) XYPlot_->SetGlyphSize(size);
}

double vtkVISXYPlot::GetXYPlotGlyphSize()
{
    return XYPlot_ ? XYPlot_->GetGlyphSize() : 1.0;
}

void vtkVISXYPlot::RemoveXYPlotAllInputs()
{
    if (XYPlot_) {
        XYPlot_->RemoveAllInputs();
        XYPlot_->PlotCurveLinesOn();
    }
}

void vtkVISXYPlot::RemoveXYPlotActor()
{
    legend_ = 0; // owned by XYPlot_, not separately allocated
    if (XYPlot_) {
        if (_renderer) _renderer->RemoveActor2D(XYPlot_);
        XYPlot_->Delete();
        XYPlot_ = 0;
    }
}

void vtkVISXYPlot::SetXYPlotNumber(int num) { plotNumber_ = num; }
int vtkVISXYPlot::GetXYPlotNumber() { return plotNumber_; }

void vtkVISXYPlot::CreateLegendBoxActor()
{
    if (XYPlot_) {
        XYPlot_->LegendOn();
        legend_ = XYPlot_->GetLegendActor();
    }
}

void vtkVISXYPlot::ShowOn()
{
    if (_renderer && XYPlot_) {
        _renderer->AddActor2D(XYPlot_);
        XYPlot_->VisibilityOn();
        if (legend_) {
            _renderer->AddActor2D(legend_);
            legend_->VisibilityOn();
        }
    }
}

void vtkVISXYPlot::ShowOff()
{
    if (XYPlot_) XYPlot_->VisibilityOff();
    if (legend_) legend_->VisibilityOff();
}

vtkXYPlotActor* vtkVISXYPlot::GetXYPlotActor()
{
    return XYPlot_;
}
