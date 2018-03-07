#include "wavewidget.h"
#include <QValueAxis>
#include "qchartlinechart.h"
#include "qcustomchart.h"
#include "ifilter.h"
WaveWidget::WaveWidget(QWidget *parent, int num)
{

    m_chart = new QChartLineChart(parent,num);
    m_chart->SetFilter(new IValueFilter(8));
    //m_chart = new QCustomChart(parent,num);

}
//WaveWidget::WaveWidget(QCustomPlot* parent,int num)
//{
//    m_chart = new QCustomChart(parent,num);
//}
WaveWidget::~WaveWidget()
{

}
void WaveWidget::SetData(MsgWaveData &wvd)
{
    m_chart->SetDataArray(wvd.channels);

}

void WaveWidget::AppendData(MsgWaveData &wvd)
{
    m_chart->AppendDataArray(wvd.channels);
}
void WaveWidget::DisplayAllChannel()
{
    m_chart->DisplayAllChannel();
}

void WaveWidget::DisplayChannel(int chan)
{
    m_chart->DisplayChannel(chan);

}

void WaveWidget::CloseAll()
{
    m_chart->CloseAll();
}

void WaveWidget::Clear()
{
    m_chart->Clear();
}

