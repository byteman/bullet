#include "wavewidget.h"
#include "qcustomchart.h"
WaveWidget::WaveWidget(QWidget *parent, int num):
    m_parent(parent),
    m_start(0),
    m_num(0)
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

//void WaveWidget::SetData(ChannelsData &waves)
//{
//    QMapIterator<int, ChannelSample> i(waves);
//    int start=33,num=0;

//    while (i.hasNext()) {
//        i.next();
//        if(i.key() < start)
//        {
//            start = i.key();
//        }
//        num++;

//       // m_chart->SetChanDataArray(chan,waves[i.key()]);
//        //cout << i.key() << ": " << i.value() << endl;
//    }
//    if(num > 0)
//    {
//        m_chart->SetChannel(start,num);
//        for(int i = 0; i < num; i++)
//        {
//            m_chart->SetChanDataArray(i,waves[start+i]);
//        }
//    }

//}

void WaveWidget::AppendData(int addr, float value)
{
    //if(m_start == 0) return;
    int chan = addr - m_start;
    if(chan < 0){
        return;
    }
    m_chart->AppendData(chan,value);
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
