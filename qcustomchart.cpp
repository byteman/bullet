#include "qcustomchart.h"
#include "qcustomplotchannel.h"
QCustomChart::QCustomChart(QCustomPlot *parent, int num):
    m_plot(parent)
{
   m_plot->axisRect()->setupFullAxesBox();
   m_plot->legend->setVisible(true);

   m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
   m_plot->xAxis->scaleRange(0,200);
   m_plot->yAxis->scaleRange(0,200);
   m_plot->xAxis->setLabel("时间");
   //m_plot->yAxis->setLabel("牛顿");

   QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
   timeTicker->setTimeFormat("%h:%m:%s");
   m_plot->xAxis->setTicker(timeTicker);
   m_plot->axisRect()->setupFullAxesBox();
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
    //m_plot->rescaleAxes();
    m_plot->replot();
}

void QCustomChart::DisplayAllChannel(bool show)
{
    //static QTime t = QTime::currentTime();
    //t = QTime::currentTime();
    ILineChart::DisplayAllChannel(show);
    //qDebug() << "display " << t.elapsed();
    //t = QTime::currentTime();
    //m_plot->rescaleAxes();
    //qDebug() << "rescaleAxes " << t.elapsed();
    //t = QTime::currentTime();
    m_plot->replot();
    //qDebug() << "replot " << t.elapsed();

}

void QCustomChart::Display()
{
    //m_plot->rescaleAxes();
    m_plot->replot();
}


void QCustomChart::Clear()
{
    ILineChart::Clear();

    m_plot->replot();

}
