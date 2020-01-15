#include "histwavewidget.h"
#define MAX_CHAN 12
void HistWaveWidget::Init()
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
   m_plot->xAxis2->setLabelColor(TextColor);
   m_plot->xAxis->setLabelFont(QFont("微软雅黑",16,QFont::Bold));
   m_plot->xAxis2->setLabelFont(QFont("微软雅黑",16,QFont::Bold));
   m_plot->yAxis->setLabelFont(QFont("微软雅黑",16,QFont::Bold));
   m_plot->xAxis->setTickLabelColor(TextColor);
   m_plot->xAxis2->setTickLabelColor(TextColor);
   m_plot->yAxis->setTickLabelColor(TextColor);
    //m_plot->legend->setVisible(true);
   //m_plot->legend->setTextColor(TextColor);

   m_plot->xAxis->setBasePen(QPen(TextColor, TextWidth));
   m_plot->xAxis2->setBasePen(QPen(TextColor, TextWidth));
   m_plot->yAxis->setBasePen(QPen(TextColor, TextWidth));
   m_plot->xAxis->setTickPen(QPen(TextColor, TextWidth));

   m_plot->yAxis->setTickPen(QPen(TextColor, TextWidth));
   m_plot->xAxis->setSubTickPen(QPen(TextColor, TextWidth));
   m_plot->yAxis->setSubTickPen(QPen(TextColor, TextWidth));


}

HistWaveWidget::HistWaveWidget(QWidget *parent):
    QObject(parent)
{
    m_plot = (QCustomPlot*)parent;
    m_plot->axisRect()->setupFullAxesBox();
    //m_plot->legend->setVisible(true);
    //m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes
                                 |QCP::iSelectLegend | QCP::iSelectPlottables);


    //m_plot->xAxis->scaleRange(0,200);
    m_plot->yAxis->scaleRange(0,2000);


    m_plot->xAxis->setLabel(QString::fromLocal8Bit("时间"));
    m_plot->yAxis->setLabel(QString::fromLocal8Bit("牛"));

    QSharedPointer<QCPAxisTickerDateTime> timeTicker(new QCPAxisTickerDateTime);
    timeTicker->setDateTimeFormat("hh:mm:ss");
    //xAxis->setDateTimeSpec(Qt::LocalTime);
    timeTicker->setTickCount(24);

    m_plot->xAxis->setTickLabelRotation(30);
    timeTicker->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    m_plot->xAxis->setSubTicks(true);
    QDateTime dateTime = QDateTime::currentDateTime();
    double  now = dateTime.toTime_t();//当前时间转化为秒
    m_plot->xAxis->setRange(now, now+3600*1);//x轴范围，从当前时间起往后推24小时


    m_plot->xAxis->setTicker(timeTicker);
    m_plot->yAxis->setRange(-2000, 2000);


    m_plot->yAxis->scaleRange(1.3);

    Init();
    m_plot->clearGraphs();
    styles.push_back(QCPScatterStyle(QCPScatterStyle::ssStar , 3));
    styles.push_back(QCPScatterStyle(QCPScatterStyle::ssStar , 3));

    for(int i = 0; i < MAX_CHAN; i++)
    {
        AddChannel(i);
    }
    //m_plot->graph(0)->setData(xData, yData);//显示数据
}

void HistWaveWidget::Clear()
{

    QVector<double> yData, xData;//生成数据

    for(int i = 0; i < MAX_CHAN; i++)
    {
        m_plot->graph(i)->setData(xData, yData);//显示数据

    }

    m_plot->rescaleAxes();
    m_plot->replot();
}

void HistWaveWidget::ShowChan(int chan, bool en)
{
    if( (chan < 0) || chan >= m_plot->graphCount()){
        return;
    }
    m_plot->graph(chan)->setVisible(en);
}
void HistWaveWidget::SetTitle(QString title)
{
    m_plot->xAxis2->setLabel(title);
    m_plot->replot();
}
void HistWaveWidget::AddChannel(int index)
{
    static QColor colors[MAX_CHAN] = {
                                      Qt::white,
                                      Qt::yellow,
                                      Qt::red,
                                      Qt::magenta,
                                      Qt::green,
                                      Qt::darkGreen,
                                      Qt::blue,
                                      Qt::cyan,
        Qt::darkYellow,
        Qt::darkBlue,
        Qt::darkRed,
        Qt::lightGray

                                     };


    QCPGraph* graph =  m_plot->addGraph();
    QPen pen(colors[index]);
    pen.setWidth(1);
    graph->setPen(pen);
    graph->setLineStyle(QCPGraph::lsStepLeft);
    //graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssStar , 3));


}
//显示指定路的数据.
void HistWaveWidget::DisplayData(MultiDeviceDataMap &ddl)
{
    this->Clear();
    //创建n个通道
    QMapIterator<int,DeviceDataList> i(ddl);
    while (i.hasNext()) {
        i.next();
        int chan = i.key();
        DisplayData(i.key(), ddl[chan]);
    }
    m_plot->rescaleAxes();
    //在 rescaleAxes后，需要手工来设置scaleRange
    m_plot->yAxis->scaleRange(1.2);
    m_plot->replot();


}

void HistWaveWidget::SetStyle(QCPGraph::LineStyle line,
                              QCPScatterStyle::ScatterShape style,int size)
{
    for(int i = 0 ; i < m_plot->graphCount(); i++)
    {
        m_plot->graph(i)->setLineStyle(line);
        m_plot->graph(i)->setScatterStyle(QCPScatterStyle(style , size));

    }

}
void HistWaveWidget::DisplayData(int chan,DeviceDataList &ddl)
{

    QVector<double> yData, xData;//生成数据
    for (int i = 0; i < ddl.size(); i++)
    {
       xData.push_back((double)ddl[i].timestamp); //3600s
//       qDebug() << i << ddl[i].timestamp << " v:" << ddl[i].value;;
       yData.push_back((double)ddl[i].value);
    }
    if(ddl.size() > 0){
        qDebug() <<ddl[0].timestamp << " - " << ddl[ddl.size()-1].timestamp;
        //m_plot->xAxis->setRange(ddl[0].timestamp, ddl[ddl.size()-1].timestamp);
    }
    if(chan <=0 || chan > m_plot->graphCount() )
    {
        qDebug("chan=%d,count=%d",chan,m_plot->graphCount());
        return;
    }
    chan--;

    m_plot->graph(chan)->setName(QString(QStringLiteral("通道%1")).arg(chan));
    m_plot->graph(chan)->setData(xData, yData,true);//显示数据


}
