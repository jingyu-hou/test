#ifndef QCHARTVIEWER_OPENSOURCE_H
#define QCHARTVIEWER_OPENSOURCE_H

#include <QWidget>
#include <QLabel>
#include <QColor>
#include <QString>
#include <QMouseEvent>
#include <QWheelEvent>
#include "ChartWidget.h"

// Stub namespace Chart for MouseUsage constants (compatibility)
namespace Chart
{
    enum {
        MouseUsageDefault = 0,
        MouseUsageScroll  = 1,
        MouseUsageZoomIn  = 3,
        MouseUsageZoomOut = 4
    };
}

// Replacement for ChartDirector's QChartViewer.
// Wraps ChartWidget for QPainter-based chart rendering.
class QChartViewer : public QWidget
{
    Q_OBJECT
public:
    explicit QChartViewer(QWidget *parent = 0);
    ~QChartViewer();

    // ChartDirector-compatible API
    virtual void setChart(ChartData *c);
    virtual ChartData *getChart();

    virtual void setImageMap(const char *imageMap);
    virtual void *getImageMapHandler() { return 0; }

    virtual void setDefaultToolTip(QString text) { Q_UNUSED(text); }

    virtual void setSelectionBorderWidth(int width) { Q_UNUSED(width); }
    virtual int getSelectionBorderWidth() { return 0; }

    virtual void setSelectionBorderColor(QColor c) { Q_UNUSED(c); }
    virtual QColor getSelectionBorderColor() { return QColor(); }

    virtual void setMouseUsage(int mouseUsage) { Q_UNUSED(mouseUsage); }
    virtual int getMouseUsage() { return Chart::MouseUsageDefault; }

    virtual void setZoomDirection(int direction) { Q_UNUSED(direction); }
    virtual int getZoomDirection() { return 0; }

    virtual void setScrollDirection(int direction) { Q_UNUSED(direction); }
    virtual int getScrollDirection() { return 0; }

    virtual void setZoomInRatio(double ratio) { Q_UNUSED(ratio); }
    virtual double getZoomInRatio() { return 0; }

    virtual void setZoomOutRatio(double ratio) { Q_UNUSED(ratio); }
    virtual double getZoomOutRatio() { return 0; }

    virtual void setMouseWheelZoomRatio(double ratio) { Q_UNUSED(ratio); }
    virtual double getMouseWheelZoomRatio() { return 0; }

    virtual void setMinimumDrag(int offset) { Q_UNUSED(offset); }
    virtual int getMinimumDrag() { return 0; }

    virtual void setUpdateInterval(int interval) { Q_UNUSED(interval); }
    virtual int getUpdateInterval() { return 0; }

    virtual bool needUpdateChart() { return false; }
    virtual bool needUpdateImageMap() { return false; }
    virtual bool isMouseOnPlotArea() { return false; }
    virtual bool isInMouseMoveEvent() { return false; }
    virtual bool isInViewPortChangedEvent() { return false; }
    virtual bool isMouseDragging() { return false; }

    virtual void updateViewPort(bool needUpdateChart, bool needUpdateImageMap) { Q_UNUSED(needUpdateChart); Q_UNUSED(needUpdateImageMap); }
    virtual void updateDisplay();
    virtual bool onMouseWheelZoom(int x, int y, int zDelta) { Q_UNUSED(x); Q_UNUSED(y); Q_UNUSED(zDelta); return false; }
    virtual void removeDynamicLayer(const char *msg) { Q_UNUSED(msg); }

    virtual int getChartMouseX() { return 0; }
    virtual int getChartMouseY() { return 0; }
    virtual int getPlotAreaMouseX() { return 0; }
    virtual int getPlotAreaMouseY() { return 0; }

signals:
    void viewPortChanged();
    void clicked(QMouseEvent *event);
    void mouseMove(QMouseEvent *event);
    void mouseWheel(QWheelEvent *event);
    void mouseMoveChart(QMouseEvent *event);
    void mouseLeaveChart(QEvent *event);
    void mouseMovePlotArea(QMouseEvent *event);
    void mouseLeavePlotArea(QEvent *event);

private:
    ChartData *m_currentChart;
    ChartWidget *m_chartWidget;
};

// Replacement for QViewPortControl (thumbnail overview)
class QViewPortControl : public QWidget
{
    Q_OBJECT
public:
    explicit QViewPortControl(QWidget *parent = 0);
    ~QViewPortControl();

    virtual void setChart(ChartData *c);
    virtual ChartData *getChart();
    virtual void setViewer(QChartViewer *viewer);
    virtual QChartViewer *getViewer();
    virtual void updateDisplay();

private:
    ChartData *m_Chart;
    QChartViewer *m_ChartViewer;
};

#endif // QCHARTVIEWER_OPENSOURCE_H
