#include "histwavewidget.h"
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

#if 0
    QDateTime dateTime = QDateTime::currentDateTime();
    double  now = dateTime.toTime_t();//当前时间转化为秒

    QVector<double> yData, xData;//生成数据
    now = 1550037151;



    for (int i = 0; i <= 60; i++)
    {
       double x = now + i * 60.0;

       xData.push_back(x);
       yData.push_back(pow(double(i), 2) + 550.0*sin(double(i)/4.0));
       //yData.push_back(i);
    }
    m_plot->xAxis->setRange(now, now+3600*1);//x轴范围，从当前时间起往后推24小时


    DeviceDataList ddl;
    QSqlError err = DAO::instance().DeviceDataQuery("304a265189b1",1,1550037151,1550037602, ddl);

    for (int i = 0; i < ddl.size(); i++)
    {
       xData.push_back(now + i*360+i);
       //xData.push_back((double)ddl[i].timestamp); //3600s
       //qDebug() << "time=" << ddl[i].timestamp << " v:" << ddl[i].value;;
       //yData.push_back((double)ddl[i].value);
       yData.push_back(i);

    }
    if(ddl.size() > 0){
        qDebug() <<ddl[0].timestamp << " - " << ddl[ddl.size()-1].timestamp;
        //m_plot->xAxis->setRange(now, now+3600*24);
        m_plot->xAxis->setRange(1550037151,1550037151+60*60*24);
    }
#endif

    m_plot->xAxis->setTicker(timeTicker);
    m_plot->yAxis->setRange(-2000, 2000);

    m_plot->yAxis->setScaleRatio(m_plot->yAxis,1.3);

    Init();
    AddChannel();
    //m_plot->graph(0)->setData(xData, yData);//显示数据
}

void HistWaveWidget::Clear()
{
    return;
    QVector<double> yData, xData;//生成数据
    m_plot->graph(0)->setData(xData, yData);//显示数据
    m_plot->rescaleAxes();
    m_plot->replot();
}
void HistWaveWidget::SetTitle(QString title)
{
    m_plot->xAxis2->setLabel(title);
    m_plot->replot();
}
void HistWaveWidget::AddChannel()
{
    static QColor colors[8] = {Qt::darkCyan,Qt::black,Qt::red,Qt::darkRed,Qt::green,Qt::darkGreen,Qt::blue,Qt::cyan};

    m_plot->clearGraphs();
    QCPGraph* graph =  m_plot->addGraph();
    QPen pen(colors[4]);
    pen.setWidth(1);
    graph->setPen(pen);

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
    //Clear();
    //m_plot->graph(0)->setName(QString(QStringLiteral("通道%1")).arg(chan));
    m_plot->graph(0)->setData(xData, yData,true);//显示数据
    //m_plot->xAxis->setRange(QDateTime::currentMSecsSinceEpoch()/1000, QDateTime::currentMSecsSinceEpoch()/1000+3600*24);//x轴范围，从当前时间起往后推24小时

    m_plot->rescaleAxes();
    m_plot->replot();
}
