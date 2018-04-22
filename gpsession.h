#ifndef GPS_SESSION_H
#define GPS_SESSION_H

#include "session.h"
#include "Poco/Net/DatagramSocket.h"
class GpsSession:public ISession
{
public:
    GpsSession(Poco::Net::DatagramSocket* socket);
    virtual QString id();
    void setHostPort(QHostAddress addr, quint16 port);
    virtual qint64 send(QByteArray &data);
    virtual qint64 send(QHostAddress host,quint16 port,QByteArray &data);
private:
    Poco::Net::DatagramSocket* m_socket;
    Poco::Net::SocketAddress m_host;
    //quint16 m_port;
};

#endif // TCPSESSION_H
