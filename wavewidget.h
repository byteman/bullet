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

    WaveWidget(QwtPlot* plot,int num=1);
    void DisplayChannel(int chan=0);
    void DisplayAllChannel();
    void SetData(MsgWaveData& wvd);
    void SetChannel(int num);

private:

    int m_num;
    QwtPlot* m_plot;
    QVector<QwtChannel*> channels;

};

#endif // WAVEWIDGET_H
