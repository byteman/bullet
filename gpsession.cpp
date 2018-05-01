#include "gpsession.h"
#include <QHostAddress>

GpsSession::GpsSession(Poco::Net::DatagramSocket *socket):
    m_socket(socket)
{

}

QString GpsSession::id()
{
    return "";
    //return m_host.toString() + QString(":%1").arg(m_port);
}

void GpsSession::setHostPort(QHostAddress addr, quint16 port)
{
    //qDebug() << "host " << addr << " port" << port;

    Poco::Net::IPAddress ipaddr(addr.toString().toStdString());
    m_host = Poco::Net::SocketAddress(ipaddr,port);
}

qint64 GpsSession::send(QByteArray &data)
{
    //qDebug() << "host " << m_host << " port" << m_port;
    return m_socket->sendTo(data.data(),data.size(),m_host);

}

qint64 GpsSession::send(QHostAddress host, quint16 port, QByteArray &data)
{
    Poco::Net::IPAddress ipaddr(host.toString().toStdString());
    m_host = Poco::Net::SocketAddress(ipaddr,port);
    return m_socket->sendTo( data.data(),data.size(),m_host);

}

