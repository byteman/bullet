#ifndef IFILTER_H
#define IFILTER_H
#include <QVector>

class IValueFilter
{
public:
    IValueFilter(int size=8);
    virtual void setLength(int size);
    virtual void Reset();
    virtual double filter(double v);
private:
    QVector<double> buffer;
    int m_size;
};

#endif // IFILTER_H
