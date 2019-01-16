#include "qcustomplotchannel.h"
static QColor colors[8] = {Qt::darkCyan,Qt::black,Qt::red,Qt::darkRed,Qt::green,Qt::darkGreen,Qt::blue,Qt::cyan};

QCustomPlotChannel::QCustomPlotChannel(int index,QCPGraph* graph):
    m_graph(graph)
{
//    m_max = -10000;
//    m_min = 10000;
    if(index < 8)
    {
        QPen pen(colors[index]);
        pen.setWidth(2);
        graph->setPen(pen);
    }


    Clear();

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
    //int index = m_graph->dataCount();
    //time = QTime::currentTime();
    // calculate two new data points:
    key = time.elapsed()/1000.0;

    m_graph->addData(key,value);
}

void QCustomPlotChannel::AddDataArray(QVector<double> &samples)
{

    QVector<double> keys;
    QVector<double> values;
    int index = m_graph->dataCount();
    for(int i = 0; i < samples.size(); i++)
    {
        keys.push_back(index++);
        if(m_filter!=NULL)
        {
            values.push_back(m_filter->filter(samples[i]));
        }else{
            values.push_back(samples[i]);
        }
    }
    m_graph->addData(keys,values);

}

void QCustomPlotChannel::Clear()
{

    QVector<double> keys;
    time = QTime::currentTime();
    m_graph->setData(keys,keys);
}


void QCustomPlotChannel::SetPen(QPen &pen)
{

}

void QCustomPlotChannel::SetName(QString &name)
{
    m_graph->setName(name);
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

