#include "wavewidget.h"


WaveWidget::WaveWidget(QCustomPlot *plot, int num):
    m_plot(plot)
{
//    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
//    timeTicker->setTimeFormat("%h:%m:%s");
//    m_plot->xAxis->setTicker(timeTicker);
    m_plot->axisRect()->setupFullAxesBox();
    m_plot->legend->setVisible(true);
    m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    SetChannel(num);
}

WaveWidget::~WaveWidget()
{

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

        channels[i]->AppendDataArray(wvd.channels[i]);
    }


}
void WaveWidget::DisplayAllChannel()
{
    for(int i = 0; i < channels.size();i++)
    {
        channels[i]->Display(true);
        m_plot->xAxis->setRange(0,channels[i]->GetSize());
    }
    m_plot->replot();
}

void WaveWidget::DisplayChannel(int chan)
{
    int index = (chan >= channels.size())?0:chan;

    for(int i = 0; i < channels.size();i++)
    {
        channels[i]->Display(index==i);
    }
    //channels[index]->GetMaxMin(max,min);

    //m_plot->xAxis->setRange(0, channels[0]->GetSize(), Qt::AlignLeft);

    m_plot->replot();

}

void WaveWidget::SetChannel(int num)
{

    channels.clear();
    for(int i = 0; i < num; i++)
    {
        channels.push_back(new QwtChannel(i,m_plot->addGraph()));
    }

}

void WaveWidget::CloseAll()
{
    for(int i = 0; i < channels.size();i++)
    {
        channels[i]->Display(false);
    }
}

void WaveWidget::Clear()
{
    for(int i = 0; i < channels.size();i++)
    {
        channels[i]->Clear();
    }
}

