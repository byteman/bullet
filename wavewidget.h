#ifndef WAVEWIDGET_H
#define WAVEWIDGET_H

#include <QObject>
#include <QVector>
#include "protomessage.h"
#include "qwtchannel.h"
class WaveWidget:public QObject
{
    Q_OBJECT
public:

    WaveWidget(QCustomPlot* plot,int num=1);
    ~WaveWidget();
    void DisplayChannel(int chan=0);
    void DisplayAllChannel();
    void SetData(MsgWaveData& wvd);
    void AppendData(MsgWaveData& wvd);
    void SetChannel(int num);
    void CloseAll();
    void Clear();

private:

    int m_num;
    QCustomPlot* m_plot;
    QVector<QwtChannel*> channels;

};

#endif // WAVEWIDGET_H
