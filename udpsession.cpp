#include "udpsession.h"
#include <QHostAddress>

UdpSession::UdpSession(QUdpSocket *socket):
    m_socket(socket)
{

}

QString UdpSession::id()
{
    return m_socket->peerAddress().toString();
}

void UdpSession::setHostPort(QHostAddress addr, quint16 port)
{
    qDebug() << "host " << addr << " port" << port;
    m_host = addr;
    m_port = port;
}

qint64 UdpSession::send(QByteArray &data)
{
    qDebug() << "host " << m_host << " port" << m_port;
    return m_socket->writeDatagram(data,m_host,m_port);

}

