#ifndef QCUSTOMCHART_H
#define QCUSTOMCHART_H

#include "ilinechart.h"
#include "qcustomplot.h"
class QCustomChart:public ILineChart
{
public:
    QCustomChart(QCustomPlot *parent, int num);
private:
    QCustomPlot *m_plot;

    // ILineChart interface
    void SetChannel(int num);

    void Init();
public:
    virtual void DisplayChannel(int chan=0,bool bshow=true);
    virtual void DisplayAllChannel(bool show);
    virtual void Display();
    void SetChannelName(int chan,QString name);

    // ILineChart interface


    // ILineChart interface
public:
    virtual void Clear();
    void SetRange(double key, int range);
};

#endif // QCUSTOMCHART_H
