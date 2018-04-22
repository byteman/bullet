#ifndef SESSION
#define SESSION
#include <QObject>
#include <QHostAddress>
struct ISession{
    virtual QString id() = 0;
    virtual qint64 send(QByteArray &data)=0;
    virtual qint64 send(QHostAddress host,quint16 port,QByteArray &data)=0;

};
#endif // SESSION

