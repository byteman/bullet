#ifndef QCUSTOMPLOTCHANNEL_H
#define QCUSTOMPLOTCHANNEL_H

#include "igraphicchannel.h"
#include "qcustomplot.h"
class QCustomPlotChannel:public IGraphicChannel
{
public:
    QCustomPlotChannel(int index,QCPGraph* graph);
    virtual void SetDataArray(QVector<double> &values);
    virtual void AddDataArray(QVector<double> &values);
    virtual void AddData(double key, double value);
    virtual void Display(bool isShow);
    virtual void Clear();
    virtual int  Size();
    virtual void SetPen(QPen& pen);
    virtual void SetName(QString &name);
    virtual void GetValueRange(double& min,double &max);
    virtual void GetKeyRange(double& min,double &max);
private:
    QCPGraph* m_graph;
    QTime time;
};

#endif // QCUSTOMPLOTCHANNEL_H
