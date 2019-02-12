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
    m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    m_plot->xAxis->scaleRange(0,200);
    m_plot->yAxis->scaleRange(0,2000);


    m_plot->xAxis->setLabel(QString::fromLocal8Bit("时间"));
    m_plot->yAxis->setLabel(QString::fromLocal8Bit("牛"));

    QSharedPointer<QCPAxisTickerDateTime> timeTicker(new QCPAxisTickerDateTime);
    timeTicker->setDateTimeFormat("yyyy-M-d h:m");
    timeTicker->setTickCount(24);
    m_plot->xAxis->setTickLabelRotation(30);
    timeTicker->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    m_plot->xAxis->setSubTicks(false);
    m_plot->xAxis->setRange(QDateTime::currentMSecsSinceEpoch()/1000, QDateTime::currentMSecsSinceEpoch()/1000+3600*24);//x轴范围，从当前时间起往后推24小时

    m_plot->xAxis->setTicker(timeTicker);
    m_plot->yAxis->setRange(-5, 5);

    m_plot->yAxis->setScaleRatio(m_plot->yAxis,1.3);

    Init();
    AddChannel();
    InitTracker(DataTracer);
}
void HistWaveWidget::InitTracker(TracerType type)
{
        tracer = new QCPItemTracer(m_plot);
          tracer->setStyle(QCPItemTracer::tsCircle);
          tracer->setPen(QPen(Qt::red));
          tracer->setBrush(Qt::red);
          tracer->setSize(10);

          label = new QCPItemText(m_plot);
          label->setLayer("overlay");
          label->setClipToAxisRect(false);
          label->setPadding(QMargins(5, 5, 5, 5));
          label->setBrush(QBrush(QColor(244, 244, 244, 100)));
          label->setPen(QPen(Qt::blue));
          label->position->setParentAnchor(tracer->position);
          label->setFont(QFont("宋体", 10));

          arrow = new QCPItemLine(m_plot);
          arrow->setLayer("overlay");
          arrow->setClipToAxisRect(false);
          arrow->setHead(QCPLineEnding::esSpikeArrow);
          m_type = DataTracer;
          switch (type) {
          case XAxisTracer:
          {
              tracer->position->setTypeX(QCPItemPosition::ptPlotCoords);
              tracer->position->setTypeY(QCPItemPosition::ptAxisRectRatio);

              label->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);

              arrow->end->setParentAnchor(tracer->position);
              arrow->start->setParentAnchor(arrow->end);
              arrow->start->setCoords(20, 0);//偏移量
              break;
          }
          case YAxisTracer:
          {
              tracer->position->setTypeX(QCPItemPosition::ptAxisRectRatio);
              tracer->position->setTypeY(QCPItemPosition::ptPlotCoords);

              label->setPositionAlignment(Qt::AlignRight|Qt::AlignHCenter);

              arrow->end->setParentAnchor(tracer->position);
              arrow->start->setParentAnchor(label->position);
              arrow->start->setCoords(-20, 0);//偏移量
              break;
          }
          case DataTracer:
          {
              tracer->position->setTypeX(QCPItemPosition::ptPlotCoords);
              tracer->position->setTypeY(QCPItemPosition::ptPlotCoords);

              label->setPositionAlignment(Qt::AlignLeft|Qt::AlignVCenter);

              arrow->end->setParentAnchor(tracer->position);
              arrow->start->setParentAnchor(arrow->end);
              arrow->start->setCoords(25, 0);
              break;
          }

          default:
              break;
          }

          setVisible(false);

}
void HistWaveWidget::Clear()
{
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
       //qDebug() << i << ddl[i].timestamp << " v:" << ddl[i].value;;
       yData.push_back((double)ddl[i].value);
       m_keys[ddl[i].timestamp] = i;
    }
    if(ddl.size() > 0){
        qDebug() <<ddl[0].timestamp << " - " << ddl[ddl.size()-1].timestamp;
        m_plot->xAxis->setRange(ddl[0].timestamp, ddl[ddl.size()-1].timestamp);
    }

    m_plot->graph(0)->setName(QString(QStringLiteral("通道%1")).arg(chan));
    m_plot->graph(0)->setData(xData, yData);//显示数据
    m_plot->rescaleAxes();
    m_plot->replot();
}

void HistWaveWidget::setPen(const QPen &pen)
{
    tracer->setPen(pen);
    arrow->setPen(pen);
}

void HistWaveWidget::setBrush(const QBrush &brush)
{
    tracer->setBrush(brush);
}

void HistWaveWidget::setLabelPen(const QPen &pen)
{
    label->setPen(pen);
}

void HistWaveWidget::setText(const QString &text)
{
    label->setText(text);
}

void HistWaveWidget::setVisible(bool visible)
{
    tracer->setVisible(visible);
    label->setVisible(visible);
    arrow->setVisible(visible);
}
//void HistWaveWidget::drawCross()
//{
//        vx<<0<<x<<ui->customPlot->xAxis->range().maxRange;
//      vy<<y<<y<<y;
//      m_plot->graph(1)->setData(vx,vy);
//      m_plot->graph(1)->setPen(QPen(Qt::red));
//      vx.clear();
//      vy.clear();
//      vx<<x<<x<<x;
//      vy<<0<<y<<ui->customPlot->yAxis->range().maxRange;
//      m_plot->graph(2)->setData(vx,vy);
//      m_plot->graph(2)->setPen(QPen(Qt::red));

//}
void HistWaveWidget::updatePosition(double xValue, double yValue)
{
    if (!visible)
    {
        setVisible(true);
        visible = true;
    }
    if (yValue > m_plot->yAxis->range().upper)
        yValue = m_plot->yAxis->range().upper;
    switch (m_type) {
    case XAxisTracer:
    {
        tracer->position->setCoords(xValue, 1);
        label->position->setCoords(0, 15);
        arrow->start->setCoords(0, 15);
        arrow->end->setCoords(0, 0);

        break;
    }
    case YAxisTracer:
    {
        tracer->position->setCoords(1, yValue);
        label->position->setCoords(-20, 0);
        break;
    }
    case DataTracer:
    {
        tracer->position->setCoords(xValue, yValue);
        label->position->setCoords(25, 0);
        break;
    }
    default:
        break;
    }
    m_plot->replot();
}

int HistWaveWidget::GetKeyIndex(qint64 key)
{

    return m_keys[key];
}
