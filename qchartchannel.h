#ifndef QCHARTCHANNEL_H
#define QCHARTCHANNEL_H
#include "igraphicchannel.h"
#include <QVector>
#include <QtCharts/QChartGlobal>

QT_CHARTS_BEGIN_NAMESPACE
class QXYSeries;
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE
class QChartChannel:public IGraphicChannel
{
public:
    QChartChannel(int index,QXYSeries* series);

    // IGraphicChannel interface
public:
    virtual void SetDataArray(QVector<double> &values);
    virtual void AddDataArray(QVector<double> &values);
    virtual void AddData(double key, double value);
    virtual void Display(bool isShow);
    virtual void Clear();
    virtual int Size();
    virtual void SetPen(QPen& pen);
    virtual void GetValueRange(double& min,double &max);
    virtual void GetKeyRange(double& min,double &max);

private:
    QXYSeries* m_series;
    int m_index;
    int m_max,m_min;
};

#endif // QCHARTCHANNEL_H
