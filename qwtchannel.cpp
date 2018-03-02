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
static QColor colors[8] = {Qt::white,Qt::black,Qt::red,Qt::darkRed,Qt::green,Qt::darkGreen,Qt::blue,Qt::cyan};
QwtChannel::QwtChannel(int index,QObject *parent):
    QObject(parent),
    m_index(index)
{
    m_max = -10000;
    m_min = 10000;
    curve.setTitle(QString("通道%1").arg(index+1));//曲线名字

    curve.setPen( colors[index], 3 );//曲线的颜色 宽度;

}

void QwtChannel::SetData(QVector<double> &samples)
{
    int size = samples.size();


    double max=0,min=0;
    if(size>0){//有数据传入
       xdata.clear();
       ydata.clear();
        //ydata.erase(ydata.begin(),ydata.begin()+size);//擦除多余的数据
        max = min= samples[0];
        double value = 0;
        for(int i=0;i<size;i++){
            xdata.append(i);

            value = samples[i];
            ydata.append(value);
            if(value > max) max = value;
            if(value < min) min = value;
        }

    }
    m_max= max;
    m_min = min;


}

void QwtChannel::AppendData(QVector<double> &samples)
{
    int size = samples.size();


    double max=0,min=0;
    if(size>0){//有数据传入

        //ydata.erase(ydata.begin(),ydata.begin()+size);//擦除多余的数据
        max = min= samples[0];
        double value = 0;
        for(int i=0;i<size;i++){
            xdata.append(xdata.size()+1);

            value = samples[i];
            ydata.append(value);
            if(value > max) max = value;
            if(value < min) min = value;
        }

    }
    if(max > m_max) m_max = max;
    if(min < m_min) m_min = min;


}

void QwtChannel::Clear()
{
    xdata.clear();
    ydata.clear();
    m_max = -10000;
    m_min = 10000;
}

void QwtChannel::Display(QwtPlot *plot,bool show)
{

    curve.setSamples(xdata,ydata);
    if(show)
        curve.attach(plot);
    else
        curve.detach();
}

void QwtChannel::GetMaxMin(double &max, double &min)
{
    max = m_max;
    min = m_min;
}

int QwtChannel::GetSize()
{
    return xdata.size();
}


