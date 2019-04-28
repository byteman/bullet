#ifndef MYPLOTTRACE_H
#define MYPLOTTRACE_H

#include <qcustomplot.h>
#include "MyTracer.h"
#include <QVector>
class Tracer{
public:
    Tracer():
        m_xTracer(NULL),
        m_tracer1(NULL),
        m_lineTracer(NULL),
        m_enable(false)
    {

    }
    MyTracer* m_xTracer;
    MyTracer* m_tracer1;
    MyTracer* m_lineTracer;

    bool m_enable; //允许.
};
class MyPlotTrace
{
public:
    MyPlotTrace(QCustomPlot* plot,int max=8);
    void Show(int ch,bool en);
    void ShowAll(bool en);
    void UpdatePosition(double xValue, double yValue);
private:
    QVector<Tracer> m_tracers;
    QCustomPlot*    m_plot;

    void AddTrace(QCustomPlot *plot,int ch);
};

#endif // MYPLOTTRACE_H
