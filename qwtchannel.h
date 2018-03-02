#ifndef QWTCHANNEL_H
#define QWTCHANNEL_H
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <math.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_grid.h>
#include <qwt_scale_draw.h>
#include <QVector>
class QwtChannel:public QObject
{
    Q_OBJECT
public:
    QwtChannel(int index,QObject* parent=NULL);


    void SetData(QVector<double> &samples);
    void AppendData(QVector<double> &samples);
    void Clear();
    void Display(QwtPlot *plot,bool show=true);
    void GetMaxMin(double &max, double &min);
    int  GetSize();
private:
    QwtPlotCurve curve;
    int m_index;
    double m_max,m_min;
    QVector<double> xdata,ydata;
};

#endif // QWTCHANNEL_H
