﻿#include "mydevices.h"
#include <QGridLayout>
#include "utils.h"
#include <QDir>
#include <QDateTime>
#include <QDebug>
MyDevices::MyDevices(int max,QGroupBox* parent):
    m_container(parent),
    m_max(10),
    m_row(3),m_col(4),
    m_num(0),m_max_sample(5000),
    m_addr(0),
    m_zoom(false)
{
    for(int i = 0; i < max; i++ )
    {
        ChannelWidget* widget = new ChannelWidget(i,parent);
        connect(widget,SIGNAL(onDoubleClick(int,bool)),this,SLOT(onDoubleClick(int,bool)));
        connect(widget,SIGNAL(onConfigClick(int)),this,SLOT(onChannelConfigEvent(int)));
        connect(widget,SIGNAL(onPlayClick(int,bool)),this,SLOT(onPlayClickEvent(int,bool)));

        widgets.push_back(widget);
        widget->hide();
    }

}
void MyDevices::SetSaveInt(int rangeS)
{
//    for(int i = 0; i < widgets.size();i++)
//    {
//         widgets[i]->SetTimeRange(rangeS);
//    }

}

void MyDevices::SetOnline(bool online)
{
    for(int i = 0; i < widgets.size();i++)
    {
         widgets[i]->SetOnline(online);
    }
}

void MyDevices::SetSelectRecState(bool paused)
{
    for(int i = 0; i < widgets.size();i++)
    {
        if(widgets[i]->IsSelected()){
            widgets[i]->SetRecState(paused);
        }
    }


}

QVector<int> MyDevices::GetSelectChan()
{
    QVector<int> chans;
    for(int i = 0; i < widgets.size();i++)
    {
        if(widgets[i]->IsSelected()){
           chans.push_back(i);
        }
    }
    return chans;
}
void MyDevices::SetTimeRange(int rangeS)
{
    for(int i = 0; i < widgets.size();i++)
    {
         widgets[i]->SetTimeRange(rangeS);
    }

}
void MyDevices::zoomDevice(int addr)
{
    clearAll();
    int w  = m_container->width();
    int h = m_container->height();

    widgets[addr]->SetUnit(m_unit);
    widgets[addr]->setGeometry(0,0,w,h-30);
    widgets[addr]->Show();

}
void MyDevices::onDoubleClick(int addr,bool zoom)
{
    m_zoom = zoom;
    m_addr = addr;
    if(zoom){
       zoomDevice(addr);
    }else{

        Resize();
    }
    emit onWaveShow(addr,zoom);


}

void MyDevices::onChannelConfigEvent(int addr)
{
    emit onChannelConfig(addr);
}

void MyDevices::onPlayClickEvent(int addr, bool played)
{
    emit onPlayClick( addr,  played);
}
void MyDevices::clearAll()
{
    for(int i = 0; i < widgets.size();i++)
    {
         widgets[i]->hide();
    }
}
#define COL_S 5
#define ROW_S 20

#define MAR_L 5
#define MAR_T 70
#define MAR_R 5
#define MAR_B 20

void MyDevices::Resize()
{
    if(m_zoom){
       zoomDevice(m_addr);
       return;
    }
    int width  = m_container->width()  - (m_col-1)*COL_S - (MAR_L+MAR_R);
    int height = m_container->height() - (m_row-1)*ROW_S - (MAR_T+MAR_B);

    int w  = width / m_col;
    int h  = height / m_row;

    for(int i = 0; i < m_num; i++ )
    {
        int row = i/m_col ; //4
        int col = i%m_col;
        widgets[i+m_start]->Reset();
        widgets[i+m_start]->SetUnit(m_unit);
        widgets[i+m_start]->setGeometry(MAR_L+col*w+col*COL_S,MAR_T+row*h+row*ROW_S,w,h);
        widgets[i+m_start]->Show();
    }
}
void MyDevices::SetDeviceNum(int start, int num)
{
    clearAll();


    m_num = num;
    m_start = start;
    m_end = start+num ;
    m_values.clear();

}

void MyDevices::SetMaxSampleNum(int max)
{
    m_max_sample = max;
}

void MyDevices::Timeout(int addr)
{
    if(addr < widgets.size())
    {
        widgets[addr]->Timeout();
        //m_csv.Append(addr, "",m_end,false);
    }
}

void MyDevices::SetUnit(QString unit)
{
    m_unit = unit;
    for(int i = 0; i < widgets.size();i++)
    {
        widgets[i]->SetUnit(unit);
    }
}

void MyDevices::SetTitle(int addr, QString title)
{
    if(addr < widgets.size())
    {
        widgets[addr]->SetTitle(title);
        //m_csv.Append(addr, "",m_end,false);
    }
}

void MyDevices::SetChanSetting(int addr, DeviceChnConfig &cfg)
{
    if(addr < widgets.size() && addr >= 0)
    {

        widgets[addr]->SetChanSetting(cfg);
    }
}

void MyDevices::SetRecState(int addr, bool paused)
{
    if(addr < widgets.size() && addr >= 0)
    {
        widgets[addr]->SetRecState(paused);
    }
}

void MyDevices::DisplayDataQueue(int addr,QQueue<SensorData> &dataQueue)
{
    if(addr < widgets.size() && addr >= 0)
    {
        widgets[addr]->DisplayDataQueue(dataQueue);
    }
}

int MyDevices::GetZoomWidget()
{
    for(int i = 0; i < widgets.size();i++)
    {
        if(widgets[i]->IsZoom()){
            return widgets[i]->Addr();
        }
    }
    return -1;
}
void MyDevices::DisplayWeight(int addr, int weight, quint16 state, quint16 dot)
{
    if(addr < widgets.size())
    {

        widgets[addr]->DisplayWeight(weight,state,dot);

    }
}

void MyDevices::ClearDisplay()
{
    for(int i = 0; i < widgets.size();i++)
    {
        widgets[i]->ClearDisplay();
    }
}
void MyDevices::GetNum(int &start, int &num)
{
    start = m_start;
    num = m_num;
}

int MyDevices::GetEndAddr()
{
    return m_end -1;
}
