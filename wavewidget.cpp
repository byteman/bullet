#include "wavewidget.h"


WaveWidget::WaveWidget(QwtPlot *plot, int num):
    m_plot(plot)
{
    SetChannel(num);
}

WaveWidget::~WaveWidget()
{

}

void WaveWidget::DisplayAllChannel()
{
//    int num = (chan > wvd.channels.size())?wvd.channels.size():chan;
//    for(int i =0 ;i < num; i++)
//    {
//        channels[i].SetData(m_plot,wvd.channels[i]);
//    }
//    m_plot->setAxisScale(QwtPlot::yLeft,min,max);
    //    m_plot->setAxisScale(QwtPlot::xBottom,0,size);
}

void WaveWidget::SetData(MsgWaveData &wvd)
{
    for(int i = 0; i < channels.size();i++)
    {
        channels[i]->SetData(wvd.channels[i]);
    }
}

void WaveWidget::AppendData(MsgWaveData &wvd)
{
    for(int i = 0; i < channels.size();i++)
    {
        channels[i]->AppendData(wvd.channels[i]);
    }

}

void WaveWidget::DisplayChannel(int chan)
{
    int index = (chan >= channels.size())?0:chan;
    double min,max;
    for(int i = 0; i < channels.size();i++)
    {
        channels[i]->Display(m_plot,index==i);
    }
    channels[index]->GetMaxMin(max,min);
    int size = channels[index]->GetSize();
    m_plot->setAxisScale(QwtPlot::yLeft,min,max);
    m_plot->setAxisScale(QwtPlot::xBottom,0,size);

    m_plot->replot();

}

void WaveWidget::SetChannel(int num)
{

    channels.clear();
    for(int i = 0; i < num; i++)
    {
        channels.push_back(new QwtChannel(i));
    }

}

void WaveWidget::CloseAll()
{
    for(int i = 0; i < channels.size();i++)
    {
        channels[i]->Display(m_plot,false);
    }
}

void WaveWidget::Clear()
{
    for(int i = 0; i < channels.size();i++)
    {
        channels[i]->Clear();
    }
}

