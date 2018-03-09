#include "qchartchannel.h"
#include <QtCharts/QLineSeries>
QChartChannel::QChartChannel(int index,QXYSeries* series):
    m_index(index),
    m_series(series)
{
    m_series->setUseOpenGL(true);
}


void QChartChannel::SetDataArray(QVector<double> &values)
{
    m_min = m_max = values[0];
    double v = 0;
    m_filter->Reset();
    QVector<QPointF> points;
    points.reserve(values.size());
    for(int i = 0; i < values.size();i++)
    {
        v = values[i];
        if(m_filter!=NULL)
        {
            v = m_filter->filter(v);
        }
        points.push_back(QPoint(i,v));
        if(v < m_min){
            m_min = v;
        }
        if(v > m_max){
            m_max = v;
        }
    }
    //批量的替换比一个一个的append快多了.
    m_series->replace(points);

}

void QChartChannel::AddDataArray(QVector<double> &values)
{
    m_min = m_max = values[0];
    double v = 0;
    //m_filter->Reset();
    QList<QPointF> points;
    points.reserve(values.size());
    for(int i = 0; i < values.size();i++)
    {
        v = values[i];
//        if(m_filter!=NULL)
//        {
//            v = m_filter->filter(v);
//        }
        points.push_back(QPoint(i,v));
        if(v < m_min){
            m_min = v;
        }
        if(v > m_max){
            m_max = v;
        }
    }
    //批量的替换比一个一个的append快多了.
    m_series->append(points);
}

void QChartChannel::AddData(double key, double value)
{
}

void QChartChannel::Display(bool isShow)
{
    if(isShow){
        m_series->show();
    }else{
        m_series->hide();
    }

}

void QChartChannel::Clear()
{
    m_series->clear();
}

int QChartChannel::Size()
{
}

void QChartChannel::SetPen(QPen &pen)
{
    //m_series->SetPen(pen);
}

void QChartChannel::GetKeyRange(double &min, double &max)
{
    max = m_series->count();
    min = 0;
}

void QChartChannel::GetValueRange(double &min, double &max)
{
    min = m_min ;
    max = m_max;
}
