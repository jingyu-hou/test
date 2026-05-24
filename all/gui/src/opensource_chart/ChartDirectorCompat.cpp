#include "democharts.h"
#include "ChartDirectorCompat.h"
#include "ChartWidget.h"
#include <cmath>

static const unsigned long g_wls_clr[] = {
    0x5588cc, 0xee9944, 0x99bb55, 0xff80ff,
    0x0000ff, 0xff0000, 0x00ff00,
};

static QColor makeColor(unsigned long rgb)
{
    return QColor((rgb >> 16) & 0xff, (rgb >> 8) & 0xff, rgb & 0xff);
}

ChartData *OpenSingleXYplotProp(StringArray DataX, vector<DoubleArray> DataY,
                                 const char **imageMap, _WELCME_CHART_PROP_ prop)
{
    if (imageMap) *imageMap = 0;
    ChartData *d = new ChartData();
    d->width = 400; d->height = 300;
    d->title = prop.Title;
    d->xLabel = prop.xLabel;
    d->yLabel = prop.yLabel;
    d->dualYAxis = false;

    int dataSize = (int)DataY.size();
    for (int kk = 0; kk < dataSize; ++kk) {
        ChartSeries series;
        series.color = makeColor(g_wls_clr[kk % 7]);
        series.lineWidth = 3;
        series.symbolType = kk + 1;
        int n = qMin(DataX.size(), DataY[kk].size());
        for (int i = 0; i < n; ++i) {
            series.points.append(QPointF(DataX[i].toDouble(), DataY[kk][i]));
        }
        if (prop.strListLabel.size() > kk)
            series.name = prop.strListLabel.at(kk);
        else if (dataSize == 1)
            series.name = QString("Series");
        else
            series.name = QString("Series %1").arg(kk + 1);
        d->series.append(series);
    }
    return d;
}

ChartData *OpenSingleXYplot(StringArray DataX, vector<DoubleArray> DataY, const char **imageMap)
{
    if (imageMap) *imageMap = 0;
    ChartData *d = new ChartData();
    d->width = 400; d->height = 300;
    d->dualYAxis = false;

    int dataSize = (int)DataY.size();
    for (int kk = 0; kk < dataSize; ++kk) {
        ChartSeries series;
        series.color = makeColor(g_wls_clr[kk % 7]);
        series.lineWidth = 3;
        series.symbolType = kk + 1;
        series.name = QString("T=%1").arg(kk);
        int n = qMin(DataX.size(), DataY[kk].size());
        for (int i = 0; i < n; ++i) {
            series.points.append(QPointF(DataX[i].toDouble(), DataY[kk][i]));
        }
        d->series.append(series);
    }
    return d;
}

ChartData *OpenSingleHIPCURXYplot(StringArray DataX, vector<DoubleArray> DataY, const char **imageMap)
{
    if (imageMap) *imageMap = 0;
    ChartData *d = new ChartData();
    d->width = 400; d->height = 300;
    d->title = QString("Chart");
    d->yLabel = QString("Amplification");
    d->dualYAxis = false;

    int dataSize = (int)DataY.size();
    for (int kk = 0; kk < dataSize; ++kk) {
        ChartSeries series;
        series.color = makeColor(g_wls_clr[kk % 7]);
        series.lineWidth = 3;
        series.symbolType = kk + 1;
        series.name = QString("T");
        int n = qMin(DataX.size(), DataY[kk].size());
        for (int i = 0; i < n; ++i) {
            series.points.append(QPointF((double)i, DataY[kk][i]));
        }
        d->series.append(series);
    }
    return d;
}

ChartData *OpenDoubleYplot(StringArray DataX, vector<DoubleArray> DataY,
                            const char **imageMap, _WELCME_CHART_PROP_ prop)
{
    if (imageMap) *imageMap = 0;
    ChartData *d = new ChartData();
    d->width = 500; d->height = 300;
    d->title = prop.Title;
    d->xLabel = prop.xLabel;
    d->yLabel = prop.yLabel;
    d->y2Label = prop.yLabel2;
    d->dualYAxis = true;

    int dataSize = (int)DataY.size();
    for (int kk = 0; kk < dataSize; ++kk) {
        ChartSeries series;
        series.color = makeColor(g_wls_clr[kk % 7]);
        series.lineWidth = 3;
        series.symbolType = kk + 1;
        if (kk < prop.strListLabel.size())
            series.name = prop.strListLabel.at(kk);
        else
            series.name = QString("Series %1").arg(kk + 1);

        int n = qMin(DataX.size(), DataY[kk].size());
        for (int i = 0; i < n; ++i) {
            series.points.append(QPointF(DataX[i].toDouble(), DataY[kk][i]));
        }

        if (kk == 0)
            d->series2.append(series);
        else
            d->series.append(series);
    }
    return d;
}

ChartData *OpenUnevenPoints(vector<DoubleArray> DataX, vector<DoubleArray> DataY,
                             const char **imageMap, _WELCME_CHART_PROP_ prop)
{
    if (imageMap) *imageMap = 0;
    ChartData *d = new ChartData();
    d->width = 400; d->height = 300;
    d->title = prop.Title;
    d->xLabel = prop.xLabel;
    d->yLabel = prop.yLabel;
    d->dualYAxis = false;

    int dataSize = qMin((int)DataY.size(), (int)DataX.size());
    for (int kk = 0; kk < dataSize; ++kk) {
        ChartSeries series;
        series.color = makeColor(g_wls_clr[kk % 7]);
        series.lineWidth = 3;
        series.symbolType = kk + 1;

        if (prop.strListLabel.size() > kk)
            series.name = prop.strListLabel.at(kk);
        else
            series.name = QString("Curve-%1").arg(kk + 1);

        int n = qMin(DataX[kk].size(), DataY[kk].size());
        for (int i = 0; i < n; ++i) {
            series.points.append(QPointF(DataX[kk][i], DataY[kk][i]));
        }
        d->series.append(series);
    }
    return d;
}

// Demo chart array (empty — demo charts are not used)
DemoChart demoCharts[] = {
    {0, 0, 0}
};

// Material chart array (replaces ChartDirector materialCharts[])
MaterialChart materialCharts[] = {
    {"Multi-Line Chart (2)", 1, OpenSingleXYplot, OpenSingleXYplotProp},
    {"Multi-Line Chart (1)", 1, OpenSingleHIPCURXYplot, 0},
    {"Dual Y-Axis", 1, OpenSingleXYplot, OpenDoubleYplot},
    {0, 0, 0, 0}
};

// Uneven data chart array
unEVENChart unEvenChart[] = {
    {"Uneven Data Points ", 1, OpenUnevenPoints},
    {0, 0, 0}
};
