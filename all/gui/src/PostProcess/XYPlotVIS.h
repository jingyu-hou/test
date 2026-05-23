#ifndef XYPLOTVIS_H
#define XYPLOTVIS_H

#include <vector>
using namespace std;
class vtkRenderer;
class vtkRenderWindow;
class vtkVISXYPlot;
class vtkDoubleArray;
class vtkDataObject;

class XYPlot
{
public:
    XYPlot();
    ~XYPlot();

    void SetVtkRender(vtkRenderer *renderer);
    void Update();
    void AllocateXYPlot(int totalCurveNum);
    void CreateXYPlot(vector<bool> &curveShowOnFlagVec);
    void AddPointXValue(double step, bool updateXRange = false);
    void AddPointYValue(int curveId, double value);
    void DestroyXYPlot();
    void GetXRange(double &xmin, double &xmax);
    ////////////////xyplot property setting//////////
    void SetXAxisTitle(char *title);
    void SetYAxisTitle(char *title);
    void SetGlyphType(int curveId, char *type);
    void SetColor(int curveId, double r, double g, double b);
    void SetPlotLines(bool flag);
    void SetPlotCurveLines(bool flag);
    void SetVisiable(bool visiable);
    void SetLegendLabel(int curveId, char *lable);
    void SetLineStipple(int pattern);

    void SetCurveVisible(vector<int> &visibleCurveIds);

private:
    vtkRenderer *renderer_;
    vtkRenderWindow *renderWindow_;
    vtkVISXYPlot *xyPlot_;
    int curveNum_;

    vtkDoubleArray *xValues_;
    vtkDoubleArray **yValues_;
    vtkDataObject **curveDataObject_;
};

#endif
