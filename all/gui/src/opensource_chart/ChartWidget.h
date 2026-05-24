#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include <QVector>
#include <QString>
#include <QColor>
#include <QPointF>

struct ChartSeries
{
    QVector<QPointF> points;
    QString name;
    QColor color;
    int symbolType;
    int lineWidth;

    ChartSeries() : color(Qt::blue), symbolType(1), lineWidth(2) {}
};

struct ChartData
{
    QString title;
    QString xLabel;
    QString yLabel;
    QString y2Label;
    QVector<ChartSeries> series;
    QVector<ChartSeries> series2;
    bool dualYAxis;
    int width;
    int height;

    ChartData() : dualYAxis(false), width(500), height(300) {}
};

class ChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChartWidget(QWidget *parent = 0);
    ~ChartWidget();

    void setChartData(const ChartData &data);
    const ChartData &chartData() const { return m_data; }

    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);

private:
    void drawTitle(QPainter &p);
    void drawAxes(QPainter &p, const QRect &plotRect,
                  double xMin, double xMax, double yMin, double yMax,
                  double y2Min, double y2Max);
    void drawLegend(QPainter &p, const QRect &plotRect);
    void drawSeries(QPainter &p, const QRect &plotRect,
                    double xMin, double xMax, double yMin, double yMax,
                    double y2Min, double y2Max);
    void computeRanges(double &xMin, double &xMax,
                       double &yMin, double &yMax,
                       double &y2Min, double &y2Max);
    QRect plotAreaRect() const;

    ChartData m_data;

    static const int MARGIN_LEFT = 70;
    static const int MARGIN_RIGHT = 20;
    static const int MARGIN_TOP = 50;
    static const int MARGIN_BOTTOM = 60;
    static const int DUAL_AXIS_RIGHT = 70;
};

#endif // CHARTWIDGET_H
