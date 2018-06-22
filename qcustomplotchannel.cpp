#include "qcustomplotchannel.h"
static QColor colors[8] = {Qt::darkCyan,Qt::black,Qt::red,Qt::darkRed,Qt::green,Qt::darkGreen,Qt::blue,Qt::cyan};

QCustomPlotChannel::QCustomPlotChannel(int index,QCPGraph* graph):
    m_graph(graph)
{
//    m_max = -10000;
//    m_min = 10000;
    m_graph->setName(QString("通道%1").arg(index+1));
    QPen pen(colors[index]);
    pen.setWidth(2);
    graph->setPen(pen);
}

void QCustomPlotChannel::SetDataArray(QVector<double> &values)
{

    QVector<double> keys;
    QVector<double> values2;
    keys.reserve(values.size());
    for(int i = 0; i < values.size();i++)
    {
        keys.append(i);

    }
    for(int i = 0; i < values.size();i++)
    {
        double v = values[i];
        if(m_filter!=NULL)
        {
            v = m_filter->filter(v);
        }
        values2.push_back(v);
    }

    m_graph->setData(keys,values2,true);
}

void QCustomPlotChannel::AddData(double key, double value)
{

}

void QCustomPlotChannel::AddDataArray(QVector<double> &samples)
{

    QVector<double> keys;
    QVector<double> values;
    m_max = -100000;
    int index = m_graph->dataCount();
    for(int i = 0; i < 1; i++)
    {
        keys.push_back(index++);
        double v = 0;
        if(m_filter!=NULL)
        {
            v = m_filter->filter(samples[i]);
        }else{
            v = samples[i];

        }
        if(v > m_max ){
            m_max = v;
        }
        values.push_back(v);
    }

    m_graph->addData(keys,values);

}

void QCustomPlotChannel::Clear()
{

    QVector<double> keys;

    m_graph->setData(keys,keys);
}


void QCustomPlotChannel::SetPen(QPen &pen)
{

}

void QCustomPlotChannel::GetValueRange(double &min, double &max)
{
    max = m_max;
    min = m_min;
}

void QCustomPlotChannel::GetKeyRange(double &min, double &max)
{

}

void QCustomPlotChannel::Display(bool show)
{
     m_graph->setVisible(show);
}


int QCustomPlotChannel::Size()
{
    int size = m_graph->dataCount();
    return size;
}

