#include "qwtchannel.h"
static int colors[8];
QwtChannel::QwtChannel(int index,QObject *parent):
    QObject(parent),
    m_index(index)
{
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


