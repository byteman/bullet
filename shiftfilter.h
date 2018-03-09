#ifndef SHIFT_FILTER_H
#define SHIFT_FILTER_H
#include <QVector>
#include "ifilter.h"
class ShiftFilter:public IValueFilter
{
public:
    ShiftFilter(int bits=8);
    virtual void setLength(int bits);
    virtual void Reset();
    virtual double filter(double v);
private:

    int m_bits;
};

#endif // IFILTER_H
