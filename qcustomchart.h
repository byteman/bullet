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
public:
    virtual void DisplayChannel(int chan=0,bool bshow=true);
    virtual void DisplayAllChannel(bool show);
    // ILineChart interface

};

#endif // QCUSTOMCHART_H
