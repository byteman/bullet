#include "myplottrace.h"
#include <QDebug>
void MyPlotTrace::AddTrace(QCustomPlot *plot,int ch)
{
    Tracer trace;
    trace.m_xTracer       = new MyTracer(plot, MyTracer::XAxisTracer,plot);//x轴
    trace.m_tracer1       = new MyTracer(plot, MyTracer::DataTracer, plot);
    trace.m_lineTracer    = new MyTracer(plot, MyTracer::CrossLine,  plot);//直线
    trace.m_tracer1->setChan(ch+1);
    trace.m_enable = false;
    m_tracers.push_back(trace);
}

MyPlotTrace::MyPlotTrace(QCustomPlot *plot, int max):
    m_plot(plot)
{
    for(int i = 0; i < max; i++){
        AddTrace(plot,i);

    }
}

void MyPlotTrace::Show(int ch, bool en)
{

    if(ch >= m_tracers.size())   {
        return;
    }
     qDebug() << "show " << ch << "=>" << en;
    m_tracers[ch].m_xTracer->setVisible(en);
    m_tracers[ch].m_tracer1->setVisible(en);
    m_tracers[ch].m_lineTracer->setVisible(en);
    m_tracers[ch].m_enable = en;
}

void MyPlotTrace::ShowAll(bool en)
{
    for(int i = 0; i < m_tracers.size();i++)
    {
        Show(i,en);
    }
}


void MyPlotTrace::UpdatePosition(double x_val, double y_val)
{
    if(m_plot==NULL)return;
    for(int i = 0; i < m_tracers.size();i++)
    {
        if(!m_tracers[i].m_enable){
            continue;
        }
        //qDebug() << "update" << i;
        m_tracers[i].m_xTracer->updatePosition(x_val, y_val);


        auto iter = m_plot->graph(i)->data()->findBegin(x_val);
        double value1 = iter->mainValue();

        m_tracers[i].m_tracer1->updatePosition(x_val, value1);

        m_tracers[i].m_lineTracer->updatePosition(x_val, y_val);
    }

}

