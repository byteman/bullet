#include "qcustomplotchannel.h"
static QColor colors[8] = {Qt::darkCyan,Qt::black,Qt::red,Qt::darkRed,Qt::green,Qt::darkGreen,Qt::blue,Qt::cyan};

QCustomPlotChannel::QCustomPlotChannel(int index,QCPGraph* graph):
    m_graph(graph)
{
//    m_max = -10000;
//    m_min = 10000;
    QPen pen(colors[index]);
    pen.setWidth(2);
    graph->setPen(pen);
}

void QCustomPlotChannel::SetDataArray(QVector<double> &values)
{

    QVector<double> keys;
    keys.reserve(values.size());
    for(int i = 0; i < values.size();i++)
    {
        keys.append(i);
    }
    m_graph->setData(keys,values,true);
}

void QCustomPlotChannel::AddData(double key, double value)
{

}

void QCustomPlotChannel::AddDataArray(QVector<double> &keys, QVector<double> &samples)
{

    m_graph->addData(keys,samples);

}

void QCustomPlotChannel::Clear()
{

}


void QCustomPlotChannel::SetPen(QPen &pen)
{

}

void QCustomPlotChannel::GetValueRange(double &min, double &max)
{

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

