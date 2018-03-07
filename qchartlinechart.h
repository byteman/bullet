#ifndef QCHARTLINECHART_H
#define QCHARTLINECHART_H

#include "ilinechart.h"
#include <QtCharts/QChartGlobal>
#include <QChartView>
#include <QObject>
#include <QVector>
#include "qchartchannel.h"
#include <QValueAxis>
QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class QChartLineChart:public ILineChart
{
public:
    QChartLineChart(QWidget *parent, int num);

    // ILineChart interface
public:

private:
    int m_num;
    QChartView *chartView;
    QChart *m_chart;
    QLineSeries *m_series;
    QValueAxis *axisX ;
    QValueAxis *axisY;

    // ILineChart interface
public:
    virtual void DisplayChannel(int chan=0,bool bshow=true);
    virtual void DisplayAllChannel(bool bshow);
};

#endif // QCHARTLINECHART_H
