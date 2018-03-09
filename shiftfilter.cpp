#include "shiftfilter.h"
#include <QDebug>
ShiftFilter::ShiftFilter(int bits):
    m_bits(bits)
{

}

void ShiftFilter::setLength(int bits)
{

    m_bits = bits;
}

void ShiftFilter::Reset()
{


}

double ShiftFilter::filter(double v)
{
    //qDebug() << "fi " <<v;
    qint16 v2 = v;
    qint16 v3 = (v2>>m_bits);
    return v3;
}
