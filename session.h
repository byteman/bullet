#ifndef SESSION
#define SESSION
#include <QObject>
struct ISession{
    virtual QString id() = 0;
    virtual qint64 send(QByteArray &data)=0;

};
#endif // SESSION

