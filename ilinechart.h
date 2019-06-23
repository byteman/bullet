#ifndef ILINECHART_H
#define ILINECHART_H

#include <QWidget>
#include "igraphicchannel.h"

struct ILineChart{
    virtual void InitChannel(QWidget* parent,int num);
    virtual void DisplayChannel(int chan=0,bool bshow=true);
    virtual void DisplayAllChannel(bool bshow);
    virtual void Display();
    virtual void SetDataArray(QVector<QVector<double>> &samples);
    virtual void AppendDataArray(QVector<QVector<double>> &samples);
    virtual void CloseAll();
    virtual void Clear();
    virtual void SetChannelName(int chan,QString name);

    virtual void GetValueRange(int chan,double &min, double &max);
    virtual void SetFilter(IValueFilter* filter);
protected:
    QVector<IGraphicChannel*> channels;
    double filter(double value);
    int m_min,m_max;
};
#endif // ILINECHART_H


