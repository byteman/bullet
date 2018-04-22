#include "udpserver.h"
#include "udpsession.h"
UdpServer::UdpServer()
{
    udp_sess = new UdpSession(&socket);

}

bool UdpServer::start(int port)
{

    if(!socket.bind(QHostAddress::Any, port))
    {
        return false;
    }

    connect(&socket, SIGNAL(readyRead()),
               this, SLOT(readPendingDatagrams()));
    connect(&timer,SIGNAL(timeout()),this,SLOT(timeout()));
    timer.start(1000);

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

        SessMessage msg;
        msg.setSession(udp_sess);
        msg.setData(datagram);
        msg.setHost(sender);
        msg.setPort(senderPort);
        //qDebug() << "sender:" << sender.toString() + " port:" << senderPort << datagram.toHex();

        emit Message(msg);

    }
}

void UdpServer::timeout()
{
    qDebug() << "timeout";
    readPendingDatagrams();
}

