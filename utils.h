#ifndef UTILS_H
#define UTILS_H

#include <QTime>
class Utils
{
public:
    Utils();
};
class TimeStamp{
public:
    TimeStamp(QString _tag);
    ~TimeStamp();
private:
    QTime time;
    QString tag;
};
#endif // UTILS_H
