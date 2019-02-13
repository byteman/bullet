#ifndef QCUSTOMCHART_H
#define QCUSTOMCHART_H

#include "ilinechart.h"
#include "qcustomplot.h"
#include "MyTracer.h"
#include <QMap>

class QCustomChart:public ILineChart
{
public:
    QCustomChart(QCustomPlot *parent, int num);
private:
    QCustomPlot *m_plot;
    QColor m_color;

    QMap<QString,MyTracer*> m_tracker;
    //MyTracer *minTracker,*maxTracker;
    // ILineChart interface
    void SetChannel(int start,int num);
    void AddChannel(QString name);
    void Init();
    void myInit();

public:
    virtual void AddLine(QString name,QString label, bool dir,double x, double y);
    virtual void DisplayChannel(int chan=0,bool bshow=true);
    virtual void DisplayAllChannel(bool show);
    virtual void SetRange(double key, int range);
    // ILineChart interface


    // ILineChart interface
public:
    virtual void Clear();
};

#endif // QCUSTOMCHART_H
