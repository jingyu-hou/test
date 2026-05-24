#include "ChartWidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QtCore/qmath.h>
#include <QFontMetrics>

ChartWidget::ChartWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(300, 200);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_data.width = 500;
    m_data.height = 300;
}

ChartWidget::~ChartWidget()
{
}

QSize ChartWidget::sizeHint() const
{
    return QSize(m_data.width, m_data.height);
}

void ChartWidget::setChartData(const ChartData &data)
{
    m_data = data;
    setFixedSize(data.width, data.height);
    update();
}

void ChartWidget::computeRanges(double &xMin, double &xMax,
                                 double &yMin, double &yMax,
                                 double &y2Min, double &y2Max)
{
    xMin = 0; xMax = 1;
    yMin = 0; yMax = 1;
    y2Min = 0; y2Max = 1;

    bool firstX = true, firstY = true, firstY2 = true;

    for (int s = 0; s < m_data.series.size(); ++s) {
        const ChartSeries &series = m_data.series[s];
        for (int i = 0; i < series.points.size(); ++i) {
            double x = series.points[i].x();
            double y = series.points[i].y();
            if (firstX) { xMin = xMax = x; firstX = false; }
            else { if (x < xMin) xMin = x; if (x > xMax) xMax = x; }
            if (firstY) { yMin = yMax = y; firstY = false; }
            else { if (y < yMin) yMin = y; if (y > yMax) yMax = y; }
        }
    }

    for (int s = 0; s < m_data.series2.size(); ++s) {
        const ChartSeries &series = m_data.series2[s];
        for (int i = 0; i < series.points.size(); ++i) {
            double x = series.points[i].x();
            double y = series.points[i].y();
            if (firstX) { xMin = xMax = x; firstX = false; }
            else { if (x < xMin) xMin = x; if (x > xMax) xMax = x; }
            if (firstY2) { y2Min = y2Max = y; firstY2 = false; }
            else { if (y < y2Min) y2Min = y; if (y > y2Max) y2Max = y; }
        }
    }

    if (xMax == xMin) { xMax = xMin + 1; }
    if (yMax == yMin) { yMax = yMin + 1; }
    if (y2Max == y2Min) { y2Max = y2Min + 1; }

    double xPad = (xMax - xMin) * 0.05;
    double yPad = (yMax - yMin) * 0.05;
    double y2Pad = (y2Max - y2Min) * 0.05;
    xMin -= xPad; xMax += xPad;
    yMin -= yPad; yMax += yPad;
    if (m_data.dualYAxis && !m_data.series2.isEmpty()) {
        y2Min -= y2Pad; y2Max += y2Pad;
    }
}

QRect ChartWidget::plotAreaRect() const
{
    int rightMargin = m_data.dualYAxis ? DUAL_AXIS_RIGHT : MARGIN_RIGHT;
    return QRect(MARGIN_LEFT, MARGIN_TOP,
                 qMax(50, width() - MARGIN_LEFT - rightMargin),
                 qMax(50, height() - MARGIN_TOP - MARGIN_BOTTOM));
}

static int autoTickCount(double range, int maxTicks)
{
    double rough = range / maxTicks;
    double magnitude = qPow(10.0, qFloor(qLn(rough) / qLn(10.0)));
    double normalized = rough / magnitude;
    int step;
    if (normalized <= 1.5) step = 1;
    else if (normalized <= 3.5) step = 2;
    else if (normalized <= 7.5) step = 5;
    else step = 10;
    double tickStep = step * magnitude;
    int count = (int)(range / tickStep) + 1;
    return qBound(2, count, 15);
}

static double tickStepSize(double range, int maxTicks)
{
    double rough = range / maxTicks;
    double magnitude = qPow(10.0, qFloor(qLn(rough) / qLn(10.0)));
    double normalized = rough / magnitude;
    int step;
    if (normalized <= 1.5) step = 1;
    else if (normalized <= 3.5) step = 2;
    else if (normalized <= 7.5) step = 5;
    else step = 10;
    return step * magnitude;
}

void ChartWidget::drawTitle(QPainter &p)
{
    if (m_data.title.isEmpty()) return;

    QFont titleFont = font();
    titleFont.setPointSize(11);
    titleFont.setBold(true);
    p.setFont(titleFont);
    p.setPen(QColor(0x55, 0x55, 0x55));

    QRect titleRect(0, 5, width(), 30);
    p.drawText(titleRect, Qt::AlignHCenter | Qt::AlignVCenter, m_data.title);
}

void ChartWidget::drawAxes(QPainter &p, const QRect &plotRect,
                            double xMin, double xMax, double yMin, double yMax,
                            double y2Min, double y2Max)
{
    QFont axisFont = font();
    axisFont.setPointSize(8);
    p.setFont(axisFont);

    // Draw plot area background
    p.fillRect(plotRect, Qt::white);
    p.setPen(QColor(0xcc, 0xcc, 0xcc));
    p.drawRect(plotRect);

    // X axis
    QColor axisColor(0xaa, 0xaa, 0xaa);
    p.setPen(axisColor);
    int xBottom = plotRect.bottom();
    p.drawLine(plotRect.left(), xBottom, plotRect.right(), xBottom);

    // Y axis
    p.drawLine(plotRect.left(), plotRect.top(), plotRect.left(), plotRect.bottom());

    // Dual Y axis
    if (m_data.dualYAxis && !m_data.series2.isEmpty()) {
        p.drawLine(plotRect.right(), plotRect.top(), plotRect.right(), plotRect.bottom());
    }

    // X ticks
    int nTicksX = autoTickCount(xMax - xMin, 8);
    double xStep = tickStepSize(xMax - xMin, 8);
    double xStart = qCeil(xMin / xStep) * xStep;

    p.setPen(QColor(0x55, 0x55, 0x55));
    for (int i = 0; i <= nTicksX; ++i) {
        double val = xStart + i * xStep;
        if (val > xMax + xStep * 0.5) break;
        int px = plotRect.left() + (int)((val - xMin) / (xMax - xMin) * plotRect.width());
        if (px < plotRect.left() || px > plotRect.right()) continue;
        p.setPen(axisColor);
        p.drawLine(px, xBottom, px, xBottom + 5);
        p.setPen(QColor(0x55, 0x55, 0x55));
        QRect tickLabel(px - 30, xBottom + 5, 60, 15);
        p.drawText(tickLabel, Qt::AlignHCenter | Qt::AlignTop, QString::number(val, 'g', 4));
    }

    // Y ticks (left axis)
    int nTicksY = autoTickCount(yMax - yMin, 6);
    double yStep = tickStepSize(yMax - yMin, 6);
    double yStart = qCeil(yMin / yStep) * yStep;

    for (int i = 0; i <= nTicksY; ++i) {
        double val = yStart + i * yStep;
        if (val > yMax + yStep * 0.5) break;
        int py = plotRect.bottom() - (int)((val - yMin) / (yMax - yMin) * plotRect.height());
        if (py < plotRect.top() || py > plotRect.bottom()) continue;
        p.setPen(axisColor);
        p.drawLine(plotRect.left() - 5, py, plotRect.left(), py);
        // Grid line
        p.setPen(QColor(0xe8, 0xe8, 0xe8));
        p.drawLine(plotRect.left() + 1, py, plotRect.right() - 1, py);
        p.setPen(QColor(0x55, 0x55, 0x55));
        QRect tickLabel(plotRect.left() - 65, py - 8, 60, 16);
        p.drawText(tickLabel, Qt::AlignRight | Qt::AlignVCenter, QString::number(val, 'g', 4));
    }

    // Y2 ticks (right axis)
    if (m_data.dualYAxis && !m_data.series2.isEmpty()) {
        int nTicksY2 = autoTickCount(y2Max - y2Min, 6);
        double y2Step = tickStepSize(y2Max - y2Min, 6);
        double y2Start = qCeil(y2Min / y2Step) * y2Step;

        for (int i = 0; i <= nTicksY2; ++i) {
            double val = y2Start + i * y2Step;
            if (val > y2Max + y2Step * 0.5) break;
            int py = plotRect.bottom() - (int)((val - y2Min) / (y2Max - y2Min) * plotRect.height());
            if (py < plotRect.top() || py > plotRect.bottom()) continue;
            p.setPen(axisColor);
            p.drawLine(plotRect.right(), py, plotRect.right() + 5, py);
            p.setPen(QColor(0x55, 0x55, 0x55));
            QRect tickLabel(plotRect.right() + 6, py - 8, 60, 16);
            p.drawText(tickLabel, Qt::AlignLeft | Qt::AlignVCenter, QString::number(val, 'g', 4));
        }
    }

    // Axis labels
    if (!m_data.xLabel.isEmpty()) {
        QFont labelFont = font();
        labelFont.setPointSize(9);
        p.setFont(labelFont);
        p.setPen(QColor(0x55, 0x55, 0x55));
        QRect xLabelRect(plotRect.left(), height() - 20, plotRect.width(), 18);
        p.drawText(xLabelRect, Qt::AlignHCenter | Qt::AlignVCenter, m_data.xLabel);
    }

    if (!m_data.yLabel.isEmpty()) {
        QFont labelFont = font();
        labelFont.setPointSize(9);
        p.setFont(labelFont);
        p.setPen(QColor(0x55, 0x55, 0x55));
        p.save();
        p.translate(15, plotRect.center().y());
        p.rotate(-90);
        QRect yLabelRect(-40, -10, 80, 20);
        p.drawText(yLabelRect, Qt::AlignHCenter | Qt::AlignVCenter, m_data.yLabel);
        p.restore();
    }

    if (m_data.dualYAxis && !m_data.y2Label.isEmpty()) {
        QFont labelFont = font();
        labelFont.setPointSize(9);
        p.setFont(labelFont);
        p.setPen(QColor(0x55, 0x55, 0x55));
        p.save();
        p.translate(width() - 15, plotRect.center().y());
        p.rotate(-90);
        QRect y2LabelRect(-40, -10, 80, 20);
        p.drawText(y2LabelRect, Qt::AlignHCenter | Qt::AlignVCenter, m_data.y2Label);
        p.restore();
    }
}

void ChartWidget::drawLegend(QPainter &p, const QRect &plotRect)
{
    int totalSeries = m_data.series.size() + m_data.series2.size();
    if (totalSeries <= 1 && m_data.series.size() <= 1)
        return;

    QFont legendFont = font();
    legendFont.setPointSize(8);
    p.setFont(legendFont);

    int x = plotRect.left() + 10;
    int y = plotRect.top() - 22;

    for (int s = 0; s < m_data.series.size(); ++s) {
        const ChartSeries &series = m_data.series[s];
        QColor c = series.color;
        p.fillRect(x, y, 20, 3, c);

        int symSize = 3;
        if (series.symbolType >= 0) {
            p.setPen(c);
            p.setBrush(c);
            p.drawEllipse(QPointF(x + 10, y + 1.5), symSize, symSize);
        }

        p.setPen(QColor(0x55, 0x55, 0x55));
        QRect labelRect(x + 24, y - 4, 100, 12);
        p.drawText(labelRect, Qt::AlignLeft | Qt::AlignVCenter, series.name);
        x += 24 + p.fontMetrics().width(series.name) + 16;
    }

    for (int s = 0; s < m_data.series2.size(); ++s) {
        const ChartSeries &series = m_data.series2[s];
        QColor c = series.color;
        p.fillRect(x, y, 20, 3, c);
        p.setPen(QColor(0x55, 0x55, 0x55));
        QRect labelRect(x + 24, y - 4, 100, 12);
        p.drawText(labelRect, Qt::AlignLeft | Qt::AlignVCenter, series.name);
        x += 24 + p.fontMetrics().width(series.name) + 16;
    }
}

void ChartWidget::drawSeries(QPainter &p, const QRect &plotRect,
                              double xMin, double xMax, double yMin, double yMax,
                              double y2Min, double y2Max)
{
    p.setRenderHint(QPainter::Antialiasing, true);

    double xScale = (xMax != xMin) ? plotRect.width() / (xMax - xMin) : 1;
    double yScale = (yMax != yMin) ? plotRect.height() / (yMax - yMin) : 1;
    double y2Scale = (y2Max != y2Min) ? plotRect.height() / (y2Max - y2Min) : 1;

    for (int s = 0; s < m_data.series.size(); ++s) {
        const ChartSeries &series = m_data.series[s];
        if (series.points.size() < 1) continue;

        QPen pen(series.color, series.lineWidth);
        p.setPen(pen);
        p.setBrush(Qt::NoBrush);

        QPainterPath path;
        bool first = true;
        for (int i = 0; i < series.points.size(); ++i) {
            int px = plotRect.left() + (int)((series.points[i].x() - xMin) * xScale);
            int py = plotRect.bottom() - (int)((series.points[i].y() - yMin) * yScale);
            if (first) { path.moveTo(px, py); first = false; }
            else path.lineTo(px, py);
        }
        p.drawPath(path);

        // Draw symbols
        p.setPen(series.color);
        p.setBrush(series.color);
        int symSize = 4;
        for (int i = 0; i < series.points.size(); ++i) {
            int px = plotRect.left() + (int)((series.points[i].x() - xMin) * xScale);
            int py = plotRect.bottom() - (int)((series.points[i].y() - yMin) * yScale);
            switch (series.symbolType) {
            case 1: p.drawEllipse(QPointF(px, py), symSize, symSize); break;
            case 2: p.drawRect(px - symSize, py - symSize, symSize * 2, symSize * 2); break;
            case 3: {
                QPointF tri[3] = { QPointF(px, py - symSize), QPointF(px - symSize, py + symSize), QPointF(px + symSize, py + symSize) };
                p.drawPolygon(tri, 3);
                break;
            }
            default: p.drawEllipse(QPointF(px, py), symSize, symSize); break;
            }
        }
    }

    // Draw series2 (right Y axis)
    for (int s = 0; s < m_data.series2.size(); ++s) {
        const ChartSeries &series = m_data.series2[s];
        if (series.points.size() < 1) continue;

        QPen pen(series.color, series.lineWidth);
        p.setPen(pen);
        p.setBrush(Qt::NoBrush);

        QPainterPath path;
        bool first = true;
        for (int i = 0; i < series.points.size(); ++i) {
            int px = plotRect.left() + (int)((series.points[i].x() - xMin) * xScale);
            int py = plotRect.bottom() - (int)((series.points[i].y() - y2Min) * y2Scale);
            if (first) { path.moveTo(px, py); first = false; }
            else path.lineTo(px, py);
        }
        p.drawPath(path);

        p.setPen(series.color);
        p.setBrush(series.color);
        int symSize = 4;
        for (int i = 0; i < series.points.size(); ++i) {
            int px = plotRect.left() + (int)((series.points[i].x() - xMin) * xScale);
            int py = plotRect.bottom() - (int)((series.points[i].y() - y2Min) * y2Scale);
            p.drawEllipse(QPointF(px, py), symSize, symSize);
        }
    }

    p.setRenderHint(QPainter::Antialiasing, false);
}

void ChartWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    p.fillRect(rect(), QColor(245, 245, 245));

    if (m_data.series.isEmpty() && m_data.series2.isEmpty()) {
        p.setPen(QColor(0x99, 0x99, 0x99));
        p.drawText(rect(), Qt::AlignCenter, QObject::tr("No data"));
        return;
    }

    double xMin, xMax, yMin, yMax, y2Min, y2Max;
    computeRanges(xMin, xMax, yMin, yMax, y2Min, y2Max);

    QRect plotRect = plotAreaRect();
    drawTitle(p);
    drawAxes(p, plotRect, xMin, xMax, yMin, yMax, y2Min, y2Max);
    drawLegend(p, plotRect);
    drawSeries(p, plotRect, xMin, xMax, yMin, yMax, y2Min, y2Max);
}
