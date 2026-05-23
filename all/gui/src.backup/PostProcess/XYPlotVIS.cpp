#include <vtkVISXYPlot.h>
#include <vtkDoubleArray.h>
#include "XYPlotVIS.h"

double ColorList[20][3]={{0.0,0.5019607843137255,1.0},
        {0.1333333333333333,0.6941176470588235,0.2980392156862745},
        {1.0,0.0,0.0},
        {0.6941176470588235,0.3568627450980392,1.0},
        {0.1568627450980392,0.1568627450980392,1.0},
        {0.1333333333333333,0.6941176470588235,0.2980392156862745},
        {1.0,0.5019607843137255,0.0},
        {0.5254901960784314,0.0,1.0},
        {0.8666666666666667,0.0,0.4352941176470588},
        {1.0, 0.3764705882352941,0.6862745098039216},
        {0.1568627450980392,0.5803921568627451,1.0},
        {0.9764705882352941,0.9764705882352941,0.0},
        {0.0,0.9254901960784314,0.0},
        {1.0,0.34509803921568627,0.03529411764705882},
        {0.6039215686274509,1.0,0.00784313725490196},
        {0.5254901960784314,0.0,1.0},
        {1.0,0.26666666666666666,1.0},
        {1.0,1.0,0.21568627450980393},
        {0.6039215686274509,1.0,0.00784313725490196},
        {1.0,0.3764705882352941,0.00784313725490196}};

XYPlot::XYPlot() : renderer_(0), renderWindow_(0), xyPlot_(0), curveNum_(0), xValues_(0), yValues_(0), curveDataObject_(0)
{
}

XYPlot::~XYPlot()
{
    DestroyXYPlot();
}

void XYPlot::SetVtkRender(vtkRenderer *renderer)
{
    renderer_ = renderer;
    renderWindow_ = renderer_->GetRenderWindow();
}

void XYPlot::Update()
{
    if (renderWindow_)  renderWindow_->Render();
}

void XYPlot::AllocateXYPlot(int totalCurveNum)
{
    curveNum_ = totalCurveNum;
    xValues_ = vtkDoubleArray::New();
    yValues_ = new vtkDoubleArray*[curveNum_];
    curveDataObject_ = new vtkDataObject*[curveNum_];
    for (int i = 0; i < curveNum_; ++i)
    {
        yValues_[i] = vtkDoubleArray::New();

        vtkFieldData *fd = vtkFieldData::New();
        fd->AllocateArrays(2);
        fd->AddArray(xValues_);
        fd->AddArray(yValues_[i]);
        curveDataObject_[i] = vtkDataObject::New();
        curveDataObject_[i]->SetFieldData(fd);

        fd->Delete();
    }
}

void XYPlot::DestroyXYPlot()
{
    if (xyPlot_ != 0)
    {
        xyPlot_->RemoveXYPlotActor();
        xyPlot_->RemoveXYPlotAllInputs();
        xyPlot_->Delete();
        xyPlot_ = 0;
    }
    if (xValues_ != 0)
    {
        xValues_->Delete();
        xValues_ = 0;
    }
    for (int i = 0; i < curveNum_; ++i)
    {
        yValues_[i]->Delete();
        curveDataObject_[i]->Delete();
    }
    delete[] yValues_;
    yValues_ = 0;
    delete[] curveDataObject_;
    curveDataObject_ = 0;
}

void XYPlot::CreateXYPlot(vector<bool> &curveShowOnFlagVec)
{
    int n = curveShowOnFlagVec.size();
    if ( n != curveNum_)  return;
    vector<int> visibleCurveIds;
    for (int i = 0; i < n; ++i)
    {
        if (curveShowOnFlagVec.at(i))
        {
            visibleCurveIds.push_back(i);
        }
    }
    int visibleNum = visibleCurveIds.size();
    xyPlot_ = vtkVISXYPlot::New();
    xyPlot_->SetRenderTo(renderer_, renderWindow_);
    xyPlot_->CreateBlankXYPlotActor();
    xyPlot_->SetPositionCoordinate(0.05, 0.05);
    xyPlot_->SetPosition2Coordinate(0.8, 0.8);
    xyPlot_->SetXYPlotTitlePosition(0.5, 0.9);
    xyPlot_->SetXLabelsNumber(8);
    xyPlot_->SetYLabelsNumber(6);
    xyPlot_->SetXYPlotXTitle(const_cast<char*>("Step"));
    xyPlot_->SetXYPlotYTitle(const_cast<char*>(""));
    xyPlot_->SetXYPlotActorColor(0.0, 0.0, 0.0);
    xyPlot_->SetXValuesTo(const_cast<char*>("Value"));
    xyPlot_->CreateLegendBoxActor();
    xyPlot_->SetXYPlotLegendPosition(0.85, 0.55-0.05*visibleNum/2);
    xyPlot_->SetXYPlotLegendPosition2(0.12, 0.06*visibleNum);
    xyPlot_->SetXYPlotNumber(visibleNum);
    for (int i = 0; i < visibleNum; ++i)
    {
        xyPlot_->AddXYPlotDataObject(i, curveDataObject_[visibleCurveIds.at(i)]);
        xyPlot_->SetGlyphTypeTo(i, const_cast<char*>("Point"));
        xyPlot_->SetXYPlotColor(i, ColorList[i%20][0], ColorList[i%20][1], ColorList[i%20][2]);
    }
}

void XYPlot::AddPointXValue(double step, bool updateXRange)
{
    xValues_->InsertNextValue(step);
    if (updateXRange)
    {
        if (step > 49)  xyPlot_->SetPlotXRange(1, step);
    }
}

void XYPlot::AddPointYValue(int curveId, double value)
{
    if (curveId < 0 || curveId >= curveNum_)  return;
    yValues_[curveId]->InsertNextValue(value);
}
////////////property setting//////////////
void XYPlot::SetXAxisTitle(char *title)
{
    if (xyPlot_)  xyPlot_->SetXYPlotXTitle(title);
}
void XYPlot::SetYAxisTitle(char *title)
{
    if (xyPlot_)  xyPlot_->SetXYPlotYTitle(title);
}
void XYPlot::SetGlyphType(int curveId, char *title)
{
    if (curveId < 0 || curveId >= curveNum_)  return;
    if (xyPlot_)  xyPlot_->SetGlyphTypeTo(curveId, title);
}
void XYPlot::SetColor(int curveId, double r, double g, double b)
{
    if (curveId < 0 || curveId >= curveNum_)  return;
    if (xyPlot_)  xyPlot_->SetXYPlotColor(curveId, r, g, b);
}
void XYPlot::SetPlotCurveLines(bool flag)
{
    if (xyPlot_)
    {
        vtkXYPlotActor *plotActor = xyPlot_->GetXYPlotActor();
        if (flag)  plotActor->PlotCurveLinesOn();
        else  plotActor->PlotCurveLinesOff();
    }
}
void XYPlot::SetPlotLines(bool flag)
{
    if (xyPlot_)
    {
        vtkXYPlotActor *plotActor = xyPlot_->GetXYPlotActor();
        if (flag)  plotActor->PlotLinesOn();
        else  plotActor->PlotLinesOff();
    }
}
void XYPlot::SetVisiable(bool visiable)
{
    if (xyPlot_)
    {
        if (visiable)  xyPlot_->ShowOn();
        else  xyPlot_->ShowOff();
    }
}
void XYPlot::SetLegendLabel(int curveId, char *lable)
{
    if (curveId < 0 || curveId >= curveNum_)  return;
    if (xyPlot_)  xyPlot_->SetXYPlotLable(curveId, lable);
}
void XYPlot::SetLineStipple(int pattern)
{
    if (xyPlot_)
    {
        vtkXYPlotActor *plotActor = xyPlot_->GetXYPlotActor();
        plotActor->GetProperty()->SetLineStipplePattern(pattern);
    }
}
/////////////////////////////////////
void XYPlot::GetXRange(double &xmin, double &xmax)
{
    if (xValues_)
    {
        double range[2];
        xValues_->GetRange(range);
        xmin = range[0];
        xmax = range[1];
    }
}

void XYPlot::SetCurveVisible(vector<int> &visibleCurveIds)
{
    if (xyPlot_)
    {
        for (int i = 0; i < curveNum_; ++i)
        {
            xyPlot_->RemoveXYPlotDataObject(curveDataObject_[i]);
        }
        int n = visibleCurveIds.size();
        xyPlot_->SetXYPlotNumber(n);
        for (int i = 0; i < n; ++i)
        {
            int curveId = visibleCurveIds[i];
            xyPlot_->AddXYPlotDataObject(i, curveDataObject_[curveId]);
            xyPlot_->SetGlyphTypeTo(i, const_cast<char*>("Line"));
            xyPlot_->SetXYPlotColor(i, ColorList[curveId%20][0], ColorList[curveId%20][1], ColorList[curveId%20][2]);
        }
    }
}

