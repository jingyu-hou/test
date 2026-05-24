#ifndef DEMOCHARTS_HDR_OPENSOURCE
#define DEMOCHARTS_HDR_OPENSOURCE

#include <QVector>
#include <QString>
#include <vector>
#include "ADefineQ.h"

using namespace std;

struct ChartData;

// ChartDirector-compatible StringArray: wraps QVector<QString> with
// a constructor that matches ChartDirector's StringArray(const char**, int)
class StringArray : public QVector<QString>
{
public:
    StringArray() : QVector<QString>() {}
    StringArray(const char* const* _data, int _len) {
        for (int i = 0; i < _len; ++i)
            append(QString(_data[i]));
    }
};

// ChartDirector-compatible DoubleArray: wraps QVector<double> with
// a constructor that matches ChartDirector's DoubleArray(const double*, int)
class DoubleArray : public QVector<double>
{
public:
    DoubleArray() : QVector<double>() {}
    DoubleArray(const double* _data, int _len) {
        for (int i = 0; i < _len; ++i)
            append(_data[i]);
    }
};

struct DemoChart
{
    const char *name;
    int noOfCharts;
    ChartData* (*createChart)(int chartNo, const char **imageMap);
};

struct MaterialChart
{
    const char *name;
    int noOfCharts;
    ChartData* (*createChart)(StringArray dataX, vector<DoubleArray> dataY, const char **imageMap);
    ChartData* (*propChart)(StringArray dataX, vector<DoubleArray> dataY, const char **imageMap, _WELCME_CHART_PROP_ propCh);
};

struct unEVENChart
{
    const char *name;
    int noOfCharts;
    ChartData* (*createChart)(vector<DoubleArray> dataX, vector<DoubleArray> dataY, const char **imageMap, _WELCME_CHART_PROP_ propCh);
};

enum _COLOR_ENUM_{
    COLOR_ENUM_BLUE1 = 1,
    COLOR_ENUM_YELLOW1 = 2,
    COLOR_ENUM_GREEN1 = 3,
};
struct _WELCMN_COLOR_{
    unsigned long int wls_clr[100];
    _WELCMN_COLOR_(){
        wls_clr[0] = 0x5588cc;
        wls_clr[1] = 0xee9944;
        wls_clr[2] = 0x99bb55;
        wls_clr[3] = 0xff80ff;
        wls_clr[4] = 0x0000ff;
        wls_clr[5] = 0xff0000;
        wls_clr[6] = 0x00ff00;
    }
};

extern DemoChart demoCharts[];
extern MaterialChart materialCharts[];
extern unEVENChart unEvenChart[];

#endif // DEMOCHARTS_HDR_OPENSOURCE
