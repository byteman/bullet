#include "utils.h"
#include <QDebug>
Utils::Utils()
{

}



TimeStamp::TimeStamp(QString _tag):
    tag(_tag)
{
    time = QTime::currentTime();
}

TimeStamp::~TimeStamp()
{
    qDebug() << tag << " elapsed " << time.elapsed();
}
