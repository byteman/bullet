#ifndef IGRAPHICCHANNEL_H
#define IGRAPHICCHANNEL_H
#include <QPen>
#include "ifilter.h"
struct IGraphicChannel
{
public:
    IGraphicChannel();
    virtual ~IGraphicChannel();
    virtual void SetFilter(IValueFilter* filter);
    virtual void SetDataArray(QVector<double> &values)=0;
    virtual void AddDataArray(QVector<double> &values)=0;
    virtual void AddData(double key, double value)=0;
    virtual void Display(bool isShow)=0;
    virtual void Clear()=0;
    virtual int  Size()=0;
    virtual void SetPen(QPen& pen)=0;
    virtual void SetName(QString &name) = 0;
    virtual void GetKeyRange(double& min,double &max)=0;
    virtual void GetValueRange(double& min,double &max)=0;
protected:
    IValueFilter* m_filter;
};

#endif // IGRAPHICCHANNEL_H
