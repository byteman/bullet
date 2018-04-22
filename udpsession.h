#ifndef UDPSESSION_H
#define UDPSESSION_H

#include "session.h"
#include <QUdpSocket>
class UdpSession:public ISession
{
public:
    UdpSession(QUdpSocket* socket);
    virtual QString id();
    void setHostPort(QHostAddress addr, quint16 port);
    virtual qint64 send(QByteArray &data);
    virtual qint64 send(QHostAddress host,quint16 port,QByteArray &data);
private:
    QUdpSocket* m_socket;
    QHostAddress m_host;
    quint16 m_port;
};

#endif // TCPSESSION_H
