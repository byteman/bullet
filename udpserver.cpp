#include "udpserver.h"
#include "udpsession.h"
UdpServer::UdpServer():
    stopped(false),
    _port(8889),
    socket(NULL),
    udp_sess(NULL)
{


}

void UdpServer::onStart()
{
    if(socket!=NULL)
    {
        delete socket;
        socket = NULL;
    }
    socket = new QUdpSocket();

    bool ok = socket->bind(QHostAddress::Any, _port);

    if(!ok)
    {
        //emit error("init port failed");
        return;
    }
    udp_sess = new UdpSession(socket);
    connect(socket, SIGNAL(readyRead()),
            this, SLOT(readPendingDatagrams()));
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(socketError(QAbstractSocket::SocketError)));
}

void UdpServer::socketError(QAbstractSocket::SocketError err)
{
    qDebug() << "UdpServer error:code=" << err;
}
bool UdpServer::start(int port)
{

    qDebug() << "startThread" << QThread::currentThreadId();

    this->moveToThread(&thread);
    // 处理数据
    connect(&thread, SIGNAL(started()), this, SLOT(onStart()));
    _port = port;
    thread.start();

    return true;

}


bool UdpServer::stop()
{
    qDebug() << "stop" << QThread::currentThreadId();
    thread.quit();
    thread.wait();
    return true;
}

void UdpServer::readPendingDatagrams()
{
    while (socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        socket->readDatagram(datagram.data(), datagram.size(),
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
