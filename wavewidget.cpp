#include "wavewidget.h"
//#include <QValueAxis>
//#include "qchartlinechart.h"
#include "qcustomchart.h"
#include "shiftfilter.h"
WaveWidget::WaveWidget(QWidget *parent, int num,int shift):
    m_parent(parent)
{

    //m_chart = new QChartLineChart(parent,num);

    m_chart = new QCustomChart((QCustomPlot*)parent,num);
    m_chart->SetFilter(new ShiftFilter(shift));

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
void WaveWidget::DisplayAllChannel(bool show)
{
    m_chart->DisplayAllChannel(show);


}
void WaveWidget::GetValueRange(int chan, double &min, double &max)
{

    m_chart->GetValueRange(chan,min,max);
    //channels[index]->GetValueRange(min,max);
}
void WaveWidget::Display()
{
    m_chart->Display();
}

void WaveWidget::DisplayChannel(int chan,bool bshow)
{
    m_chart->DisplayChannel(chan,bshow);

}

void WaveWidget::CloseAll()
{
    m_chart->CloseAll();
}

void WaveWidget::Clear()
{
    m_chart->Clear();

}

void WaveWidget::Hide()
{
    if(m_parent){
        //m_parent->setGeometry(0,0,0,0);
           m_parent->hide();
    }

}
void WaveWidget::Show()
{
    if(m_parent){
        //m_parent->setGeometry(0,0,1000,1000);
     m_parent->show();
    }

}


void WaveWidget::AppendData(int addr, float value)
{
    m_chart->AppendData(chan,value);
}
