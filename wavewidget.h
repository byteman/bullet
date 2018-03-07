#ifndef WAVEWIDGET_H
#define WAVEWIDGET_H

#include <QObject>
#include <QVector>
#include "protomessage.h"
#include "qwtchannel.h"
#include "ilinechart.h"

class WaveWidget:public QObject
{
    Q_OBJECT
public:

    WaveWidget(QWidget* parent,int num=1);
    //WaveWidget(QCustomPlot* parent,int num=1);
    ~WaveWidget();
    void DisplayChannel(int chan=0);
    void DisplayAllChannel();

    void SetChannel(int num);
    void CloseAll();
    void Clear();

    void SetData(MsgWaveData &wvd);
    void AppendData(MsgWaveData &wvd);
private:

    int m_num;
    ILineChart* m_chart;

};

#endif // WAVEWIDGET_H
