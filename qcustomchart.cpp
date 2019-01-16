#include "qcustomchart.h"
#include "qcustomplotchannel.h"
QCustomChart::QCustomChart(QCustomPlot *parent, int num):
    m_plot(parent)
{
   m_plot->axisRect()->setupFullAxesBox();
   //m_plot->legend->setVisible(true);
   m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
   m_plot->xAxis->scaleRange(0,200);
   m_plot->yAxis->scaleRange(0,2000);

   m_plot->xAxis->setLabel("时间");
   //m_plot->yAxis->setLabel("牛顿");

   QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
   timeTicker->setTimeFormat("%h:%m:%s");
   m_plot->xAxis->setTicker(timeTicker);
   m_plot->axisRect()->setupFullAxesBox();
   //customPlot->yAxis->setRange(-1.2, 1.2);

   m_plot->yAxis->setScaleRatio(m_plot->yAxis,1.3);
     //ui.ImgQCustomPlot->yAxis->setScaleRatio(ui.ImgQCustomPlot->xAxis, 1);
   m_plot->setOpenGl(true);
   //SetChannel(0,num);
}

void QCustomChart::SetChannel(int start,int num)
{

    channels.clear();
    m_plot->clearGraphs();
    for(int i = 0; i < num; i++)
    {
        QCustomPlotChannel* chan = new QCustomPlotChannel(i,m_plot->addGraph());
        chan->SetName(QString("addr:%1").arg(start+i));
        channels.push_back(chan);
    }

}




void QCustomChart::DisplayChannel(int chan,bool show)
{
    ILineChart::DisplayChannel(chan,show);
    m_plot->rescaleAxes();
    m_plot->replot();
}

void QCustomChart::DisplayAllChannel(bool show)
{

    ILineChart::DisplayAllChannel(show);

    m_plot->rescaleAxes();
    //如果数值一直不变，会导致Y轴持续变大.
    //m_plot->yAxis->setScaleRatio(m_plot->yAxis,1.3);
    m_plot->replot();
}


void QCustomChart::Clear()
{
    ILineChart::Clear();

    m_plot->replot();

}
