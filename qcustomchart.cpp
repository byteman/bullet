#include "qcustomchart.h"
#include "qcustomplotchannel.h"
QCustomChart::QCustomChart(QCustomPlot *parent, int num):
    m_plot(parent)
{
   m_plot->axisRect()->setupFullAxesBox();
   m_plot->legend->setVisible(true);
   m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
   m_plot->xAxis->scaleRange(0,200);
   m_plot->yAxis->scaleRange(0,2000);
   SetChannel(num);
}

void QCustomChart::SetChannel(int num)
{

    channels.clear();
    for(int i = 0; i < num; i++)
    {
        channels.push_back(new QCustomPlotChannel(i,m_plot->addGraph()));
    }

}


void QCustomChart::DisplayChannel(int chan)
{
    ILineChart::DisplayChannel(chan);
    m_plot->rescaleAxes();
    m_plot->replot();
}
