#include "qcustomchart.h"
#include "qcustomplotchannel.h"
void QCustomChart::Init()
{
   QColor TextColor(255,255,255);
   int TextWidth = 2;


   QLinearGradient plotGradient;
   plotGradient.setStart(0, 0);
   plotGradient.setFinalStop(0, 350);
   plotGradient.setColorAt(0, QColor(80, 80, 80));
   plotGradient.setColorAt(1, QColor(50, 50, 50));
   m_plot->setBackground(plotGradient);

   QLinearGradient axisRectGradient;
   axisRectGradient.setStart(0, 0);
   axisRectGradient.setFinalStop(0, 350);
   axisRectGradient.setColorAt(0, QColor(80, 80, 80));
   axisRectGradient.setColorAt(1, QColor(30, 30, 30));
   m_plot->axisRect()->setBackground(axisRectGradient);


   m_plot->yAxis->setLabelColor(TextColor);
   m_plot->xAxis->setLabelColor(TextColor);
   m_plot->xAxis->setLabelFont(QFont("微软雅黑",16,QFont::Bold));
   m_plot->yAxis->setLabelFont(QFont("微软雅黑",16,QFont::Bold));
   m_plot->xAxis->setTickLabelColor(TextColor);
   m_plot->yAxis->setTickLabelColor(TextColor);

   //m_plot->legend->setTextColor(TextColor);

   m_plot->xAxis->setBasePen(QPen(TextColor, TextWidth));
   m_plot->yAxis->setBasePen(QPen(TextColor, TextWidth));
   m_plot->xAxis->setTickPen(QPen(TextColor, TextWidth));
   m_plot->yAxis->setTickPen(QPen(TextColor, TextWidth));
   m_plot->xAxis->setSubTickPen(QPen(TextColor, TextWidth));
   m_plot->yAxis->setSubTickPen(QPen(TextColor, TextWidth));

}
void QCustomChart::myInit()
{
    //customPlot->addGraph(); // blue line
    //customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    //customPlot->addGraph(); // red line
    //customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    m_plot->xAxis->setTicker(timeTicker);
    m_plot->axisRect()->setupFullAxesBox();
    m_plot->yAxis->setRange(-1000, 1000);
    //customPlot->setOpenGl(true);
    // make left and bottom axes transfer their ranges to right and top axes:
    QObject::connect(m_plot->xAxis, SIGNAL(rangeChanged(QCPRange)), m_plot->xAxis2, SLOT(setRange(QCPRange)));
    QObject::connect(m_plot->yAxis, SIGNAL(rangeChanged(QCPRange)), m_plot->yAxis2, SLOT(setRange(QCPRange)));

}
QCustomChart::QCustomChart(QCustomPlot *parent, int num):
    m_plot(parent)
{

   m_plot->axisRect()->setupFullAxesBox();
   //m_plot->legend->setVisible(true);
   m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
   m_plot->xAxis->scaleRange(0,200);
   m_plot->yAxis->scaleRange(0,2000);


   m_plot->xAxis->setLabel(QString::fromLocal8Bit("时间"));
   m_plot->yAxis->setLabel(QString::fromLocal8Bit("牛"));

   QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
   timeTicker->setTimeFormat("%h:%m:%s");
   m_plot->xAxis->setTicker(timeTicker);
   //m_plot->axisRect()->setupFullAxesBox();
   m_plot->yAxis->setRange(-5, 5);

   m_plot->yAxis->setScaleRatio(m_plot->yAxis,1.3);

   Init();
   //m_plot->setOpenGl(true);开这句要崩溃

}

void QCustomChart::SetChannel(int start,int num)
{

    channels.clear();
    m_plot->clearGraphs();
    for(int i = 0; i < num; i++)
    {
        QCustomPlotChannel* chan = new QCustomPlotChannel(i,m_plot->addGraph());
        QString name = QString("addr:%1").arg(start+i+1);
        chan->SetName(name);
        channels.push_back(chan);
    }

}
void QCustomChart::DisplayChannel(int chan,bool show)
{
    ILineChart::DisplayChannel(chan,show);
    m_plot->rescaleAxes();
    m_plot->replot(QCustomPlot::rpQueuedReplot);
}

void QCustomChart::DisplayAllChannel(bool show)
{

    ILineChart::DisplayAllChannel(show);

    //如果数值一直不变，会导致Y轴持续变大.
    //m_plot->xAxis->setRange()
    //m_plot->yAxis->setScaleRatio(m_plot->yAxis,1.3);
    m_plot->replot();
}

void QCustomChart::SetRange(double key, int range)
{
    //rescale一定要放到setRange的前面，否则setRange没有作用.
    m_plot->rescaleAxes();
    m_plot->yAxis->scaleRange(1.2);
    m_plot->xAxis->setRange(key,range,Qt::AlignRight);

}


void QCustomChart::Clear()
{
    ILineChart::Clear();

    m_plot->replot();

}
