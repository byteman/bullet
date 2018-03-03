#ifndef QWTCHANNEL_H
#define QWTCHANNEL_H
#include "qcustomplot.h"
#include <QVector>
class QwtChannel:public QObject
{
    Q_OBJECT
public:
    QwtChannel(int index,QCPGraph* graph,QObject* parent=NULL);


    void SetData(QVector<double> &samples);
    void AppendDataArray(QVector<double> &samples);
    void AppendData(double key, double sample);

    void Clear();
    void Display(bool show=true);
    void GetMaxMin(double &max, double &min);
    int  GetSize();
private:
    QCPGraph* m_graph;
    int m_index;
    double m_max,m_min;
    QVector<double> xdata,ydata;
};

#endif // QWTCHANNEL_H
