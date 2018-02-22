#ifndef TCPSESSION_H
#define TCPSESSION_H

#include "session.h"
#include <QTcpSocket>
class TcpSession:public ISession
{
public:
    TcpSession(QTcpSocket* socket);
    virtual QString id();
    virtual qint64 send(QByteArray &data);
private:
    QTcpSocket* m_socket;
};

#endif // TCPSESSION_H
