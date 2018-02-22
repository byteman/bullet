#include "udpserver.h"
#include "udpsession.h"
UdpServer::UdpServer()
{
    udp_sess = new UdpSession(&socket);
    msg.setSession(udp_sess);
}

bool UdpServer::start(int port)
{

    if(!socket.bind(QHostAddress::Any, port))
    {
        return false;
    }

    connect(&socket, SIGNAL(readyRead()),
               this, SLOT(readPendingDatagrams()));

}

bool UdpServer::stop()
{
    disconnect(&socket,SIGNAL(readyRead()),0,0);
    socket.close();
    return true;
}

void UdpServer::readPendingDatagrams()
{
    while (socket.hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket.pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        socket.readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        udp_sess->setHostPort(sender,senderPort);
        msg.setData(datagram);

        emit Message(msg);

    }
}
