#include "qwtchannel.h"
//Qt::white	3	White (#ffffff)
//Qt::black	2	Black (#000000)
//Qt::red	7	Red (#ff0000)
//Qt::darkRed	13	Dark red (#800000)
//Qt::green	8	Green (#00ff00)
//Qt::darkGreen	14	Dark green (#008000)
//Qt::blue	9	Blue (#0000ff)
//Qt::darkBlue	15	Dark blue (#000080)
//Qt::cyan	10	Cyan (#00ffff)
//Qt::darkCyan	16	Dark cyan (#008080)
static QColor colors[8] = {Qt::darkCyan,Qt::black,Qt::red,Qt::darkRed,Qt::green,Qt::darkGreen,Qt::blue,Qt::cyan};

QwtChannel::QwtChannel(int index,QCPGraph* graph,QObject *parent):
    QObject(parent),
    m_index(index),
    m_graph(graph)
{
    m_max = -10000;
    m_min = 10000;
    QPen pen(colors[index]);
    pen.setWidth(2);
    graph->setPen(pen);

    //graph_>setTitle(QString("通道%1").arg(index+1));//曲线名字

    //curve.setPen( colors[index], 3 );//曲线的颜色 宽度;

}

void QwtChannel::SetData(QVector<double> &samples)
{
    QVector<double> keys;
    double key = 0;
    for(int i = 0; i < samples.size();i++)
    {
       keys.push_back(key++);
    }
    qDebug() << "SetData count=" << samples.size();
    for(int i = 0; i <samples.size(); i++ )
    {
        if(samples[i] > 5)
            samples[i] =5;
        //qDebug() << samples[i];
    }
    m_graph->setData(keys,samples);

}

void QwtChannel::AppendData(double key, double sample)
{

    m_graph->addData(m_graph->dataCount()+1,sample);
}

void QwtChannel::AppendDataArray(QVector<double> &samples)
{
    QVector<double> keys;
    for(int i = 0; i < samples.size();i++)
    {
       keys.push_back(m_graph->dataCount()+1);
    }
    m_graph->addData(keys,samples);

}

void QwtChannel::Clear()
{

}

void QwtChannel::Display(bool show)
{

     m_graph->setVisible(show);
     //if(show)
     //m_graph->rescaleValueAxis();

}

void QwtChannel::GetMaxMin(double &max, double &min)
{

}

int QwtChannel::GetSize()
{
    int size = m_graph->dataCount();
    return size;
}


