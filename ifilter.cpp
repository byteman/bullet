#include "ifilter.h"
#include <QDebug>
IValueFilter::IValueFilter(int size):
    m_size(size)
{

}

void IValueFilter::setLength(int size)
{
    Reset();
    buffer.reserve(size);
    m_size = size;
}

void IValueFilter::Reset()
{
    buffer.clear();

}

double IValueFilter::filter(double v)
{
    //qDebug() << "fi " <<v;
    buffer.push_back(v);
    while(buffer.size() > m_size)
    {
        buffer.pop_front();
    }
    double sum = 0;
    for(int i = 0; i < buffer.size(); i++)
    {
        sum += buffer[i];
    }
    return sum / buffer.size();
}
