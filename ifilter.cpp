#include "ifilter.h"
#include <QDebug>
IValueFilter::IValueFilter(int size):
    m_size(size)
{

}

void IValueFilter::setLength(int size)
{
    buffer.clear();
    buffer.reserve(size);
    m_size = size;
}

double IValueFilter::filter(double v)
{
    qDebug() << "fi " <<v;
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
