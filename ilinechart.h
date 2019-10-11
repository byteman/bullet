﻿#ifndef ILINECHART_H
#define ILINECHART_H

#include <QWidget>
#include "igraphicchannel.h"

struct ILineChart{

    virtual void InitChannel(QWidget* parent,int num);

    virtual void DisplayChannel(int chan=0,bool bshow=true);
    virtual void DisplayAllChannel(bool bshow);
    virtual void SetDataArray(QVector<QVector<double> > &samples);
    virtual void SetChanDataArray(int chan ,QVector<double> &samples);
    virtual void AppendDataArray(QVector<QVector<double> > &samples);
    virtual void AppendData(int chan,float value);
    virtual void AppendData2(int chan,double key,double value);
    virtual void CloseAll();
    virtual void Clear();
    virtual void SetChannel(int start,int num);
    virtual void GetValueRange(double &min, double &max);
    virtual void SetFilter(IValueFilter* filter);
    virtual void SetRange(double key, int range);
    virtual void AddLine(QString name,QString label, bool up,double x, double y);
protected:
    QVector<IGraphicChannel*> channels;
    double filter(double value);
    int m_min,m_max;
    //double  lastPointKey;
};
#endif // ILINECHART_H


