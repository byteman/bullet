#include "qchartlinechart.h"
#include <QVBoxLayout>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QValueAxis>
#include "filter.h"
QChartLineChart::QChartLineChart(QWidget *parent, int num)
{
    m_chart = new QChart;

    chartView = new QChartView(m_chart);
    //chartView->setMinimumSize(800, 600);
    m_chart->legend()->hide();
    m_chart->setTitle("Data from the microphone");
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(chartView);
    parent->setLayout(mainLayout);
    axisX = new QValueAxis;
    axisX->setRange(0, 200);
    axisX->setLabelFormat("%g");
    axisX->setTitleText("Samples");
    axisY = new QValueAxis;
    axisY->setRange(0, 60000);
    axisY->setTitleText("Audio level");
    for(int i = 0; i < num; i++)
    {
        QLineSeries* qs = new QLineSeries;
        qs->setUseOpenGL(true);

        channels.push_back(new QChartChannel(i,qs));

        m_chart->addSeries(qs);

        m_chart->setAxisX(axisX, qs);
        m_chart->setAxisY(axisY, qs);
    }
}



void QChartLineChart::DisplayChannel(int chan,bool show)
{
    ILineChart::DisplayChannel(chan,show);
    double min,max;
    channels[chan]->GetValueRange(min,max);
    axisY->setRange(min,max);
    channels[chan]->GetKeyRange(min,max);
    axisX->setRange(min,max);
}
#include <QDebug>
void QChartLineChart::DisplayAllChannel(bool show)
{
    double min,max;
    ILineChart::DisplayAllChannel(show);

    channels[0]->GetKeyRange(min,max);

    qDebug() << "min=" << min << "max=" << max;
    axisX->setRange(min,max);

    GetValueRange(min,max);
    axisY->setRange(min,max);

}
