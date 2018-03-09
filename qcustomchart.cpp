#include "qcustomchart.h"
#include "qcustomplotchannel.h"
QCustomChart::QCustomChart(QCustomPlot *parent, int num):
    m_plot(parent)
{
   m_plot->axisRect()->setupFullAxesBox();
   m_plot->legend->setVisible(false);
   m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
   m_plot->xAxis->scaleRange(0,200);
   m_plot->yAxis->scaleRange(0,2000);
   m_plot->setOpenGl(true);
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
    m_plot->replot();

}
