#include "wavewidget.h"
#include "qcustomchart.h"
WaveWidget::WaveWidget(QWidget *parent, int num):
    m_parent(parent),
    m_start(0),
    m_num(0),
    m_range(10)
{

    m_chart = new QCustomChart((QCustomPlot*)parent,num);

}
WaveWidget::~WaveWidget()
{

}

void WaveWidget::DisplayAllChannel(bool show)
{
    m_chart->DisplayAllChannel(show);
}
void WaveWidget::SetRange(int range){
   m_range = range;
}

void WaveWidget::AppendData(int addr, float value)
{
    //if(m_start == 0) return;
    int chan = addr - m_start;
    if(chan < 0){
        return;
    }
    m_chart->AppendData(chan,value);
}
#include <QTime>
void WaveWidget::AppendTimeData(int addr, double value)
{

    static QTime time(QTime::currentTime());
    //static double lastPointKey = 0;
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {
        int chan = addr - m_start;
        if(chan < 0){
            return;
        }
        m_chart->AppendData2(chan,key,value);
    }
    m_chart->SetRange(key,m_range);
}

void WaveWidget::AppendTimeData(int addr, double time, double value)
{
    int chan = addr - m_start;
    m_chart->AppendData2(chan,time,value);
    m_chart->SetRange(time,m_range);
}
void WaveWidget::SetChannel(int start,int num)
{

    if((num == m_num) && (start==m_start)) return;
    m_start = start;
    m_num   = num;
    if(m_chart!=NULL){
        m_chart->SetChannel(start,num);
    }
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
//void WaveWidget::AppendDataArr(ChannelDataArray &wvd)
//{
//    //m_chart->AppendDataArray(wvd);
//}
void WaveWidget::Hide()
{
    if(m_parent){
         m_parent->hide();
    }

}
void WaveWidget::Show()
{
    if(m_parent){
        m_parent->show();
    }

}
