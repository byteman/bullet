﻿#ifndef WAVEWIDGET_H
#define WAVEWIDGET_H

#include <QObject>
#include <QVector>
//#include "qwtchannel.h"
#include "ilinechart.h"
#include "wavefile.h"
//typedef QVector<float> ChannelData;
//typedef QVector<ChannelData> ChannelDataArray;

class WaveWidget:public QObject
{
    Q_OBJECT
public:

    WaveWidget(QWidget* parent,int num=1);
    //WaveWidget(QCustomPlot* parent,int num=1);
    ~WaveWidget();
    void DisplayChannel(int chan=0,bool bshow=false);
    void DisplayAllChannel(bool show);

    //void SetData(ChannelsData& waves);
    //void AppendDataArr(ChannelDataArray &wvd);
    void AppendData(int addr, float value);
    void SetChannel(int start,int num);

    void CloseAll();
    void Clear();
    void SaveWave();
    void Hide();
    void Show();
    void SetRange(int range);
    void AppendTimeData(int addr, double value);
    void AppendTimeData(int addr, double time,double value);
    void AddLine(QString name,QString label, bool up,double x, double y);
private:

    int m_num;
    int m_start;
    int m_range;
    QWidget* m_parent;
    ILineChart* m_chart;


};

#endif // WAVEWIDGET_H
