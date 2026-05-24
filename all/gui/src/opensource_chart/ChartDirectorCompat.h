#ifndef CHARTDIRECTOR_COMPAT_H
#define CHARTDIRECTOR_COMPAT_H

#include <vector>
#include "democharts.h"
#include "ChartWidget.h"

using namespace std;

// ChartData factory functions
ChartData *OpenSingleXYplot(StringArray DataX, vector<DoubleArray> DataY, const char **imageMap);
ChartData *OpenSingleXYplotProp(StringArray DataX, vector<DoubleArray> DataY, const char **imageMap, _WELCME_CHART_PROP_ prop);
ChartData *OpenSingleHIPCURXYplot(StringArray DataX, vector<DoubleArray> DataY, const char **imageMap);
ChartData *OpenDoubleYplot(StringArray DataX, vector<DoubleArray> DataY, const char **imageMap, _WELCME_CHART_PROP_ prop);
ChartData *OpenUnevenPoints(vector<DoubleArray> DataX, vector<DoubleArray> DataY, const char **imageMap, _WELCME_CHART_PROP_ prop);

#endif // CHARTDIRECTOR_COMPAT_H
