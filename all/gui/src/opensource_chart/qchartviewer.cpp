#include "qchartviewer.h"
#include <QVBoxLayout>

QChartViewer::QChartViewer(QWidget *parent)
    : QWidget(parent), m_currentChart(0)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    m_chartWidget = new ChartWidget(this);
    layout->addWidget(m_chartWidget);
    setLayout(layout);
}

QChartViewer::~QChartViewer()
{
    delete m_currentChart;
}

void QChartViewer::setChart(ChartData *c)
{
    if (m_currentChart && m_currentChart != c)
        delete m_currentChart;
    m_currentChart = c;
    if (c) {
        m_chartWidget->setChartData(*c);
        m_chartWidget->show();
    } else {
        m_chartWidget->hide();
    }
}

ChartData *QChartViewer::getChart()
{
    // Transfer ownership to caller: consumers do "delete chartViewer->getChart()"
    ChartData *ret = m_currentChart;
    m_currentChart = 0;
    return ret;
}

void QChartViewer::setImageMap(const char *imageMap)
{
    Q_UNUSED(imageMap);
}

void QChartViewer::updateDisplay()
{
    if (m_currentChart) {
        m_chartWidget->setChartData(*m_currentChart);
    }
}

// ---- QViewPortControl ----

QViewPortControl::QViewPortControl(QWidget *parent)
    : QWidget(parent), m_Chart(0), m_ChartViewer(0)
{
    setMinimumSize(100, 60);
}

QViewPortControl::~QViewPortControl()
{
    delete m_Chart;
}

void QViewPortControl::setChart(ChartData *c)
{
    if (m_Chart && m_Chart != c)
        delete m_Chart;
    m_Chart = c;
    update();
}

ChartData *QViewPortControl::getChart()
{
    return m_Chart;
}

void QViewPortControl::setViewer(QChartViewer *viewer)
{
    m_ChartViewer = viewer;
}

QChartViewer *QViewPortControl::getViewer()
{
    return m_ChartViewer;
}

void QViewPortControl::updateDisplay()
{
    update();
}
