#include "tcpsession.h"
#include <QHostAddress>

TcpSession::TcpSession(QTcpSocket *socket):
    m_socket(socket)
{

}

QString TcpSession::id()
{
    return m_socket->peerAddress().toString();
}

qint64 TcpSession::send(QByteArray &data)
{
    return data.size();
}

