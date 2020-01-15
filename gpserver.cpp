#include "gpserver.h"
#include <QDebug>
#include <QDateTime>
#include <QHostAddress>
#include "Poco/Net/DatagramSocket.h"
#include "gpsession.h"

static Poco::Net::DatagramSocket *_socket = NULL;

GPServer::GPServer(QObject *parent):
    QObject(parent),
    _isStarted(false),
    _quit(true)
{


}

GPServer::~GPServer()
{
}


bool GPServer::initSocket(int port)
{

    try{
        _socket->bind(Poco::Net::SocketAddress(port), true);

    }
    catch(Poco::Exception& e)
    {
        QString err = e.displayText().c_str();
        qDebug() << "ccuserver init err: " << err;
        emit onError(err);
        return false;
    }

    return true;
}


static void sendDatagrams(Poco::Net::SocketAddress sender,QByteArray &datagram)
{
    if(_socket!=NULL)
    {
        _socket->sendTo(datagram.data(),datagram.length(),sender);
    }
}

bool GPServer::start(int port)
{

    _port = port;
    _isStarted = startThread();
    return _isStarted;

}

int GPServer::stop()
{
    qDebug() << "stop" << QThread::currentThreadId();
    _quit = true;
    thread.quit();
    thread.wait();
    return true;

}

bool GPServer::IsStarted()
{
    return thread.isRunning();
}

void GPServer::onStart()
{
    if(_socket!=NULL)
    {
        delete _socket;
        _socket = NULL;
    }
    _socket = new Poco::Net::DatagramSocket();
    if(!initSocket(_port))
    {
        return;
    }
    udp_sess = new GpsSession(_socket);
    _quit = false;
    Poco::Timespan span(100000);
    while(!_quit){

        if(_socket->poll(span, Poco::Net::Socket::SELECT_READ))
        {
            try
            {
                char buffer[4096];
                Poco::Net::SocketAddress sender;
                int n = _socket->receiveFrom(buffer, sizeof(buffer), sender);
                QByteArray data(buffer,n);

                SessMessage msg;
                msg.setSession(udp_sess);
                msg.setData(data);
                QHostAddress host(sender.addr());

                msg.setHost(host);
                msg.setPort(sender.port());

                emit Message(msg);
                //qDebug()<<time.elapsed()<<"ms";

            }
            catch (Poco::Exception& exc)
            {
                QString err = exc.displayText().c_str();
                onError(QString("gpserver err: %1").arg(exc.displayText().c_str()));
                qDebug() << "gpserver" << err ;
                //std::cerr << "UDPEchoServer: " << exc.displayText() << std::endl;
            }
        }
    }
    _socket->close();
    disconnect(&thread, SIGNAL(started()), this, SLOT(onStart()));
    qDebug() << "gps server stop";
}

bool GPServer::startThread()
{

    qDebug() << "gps startThread" << QThread::currentThreadId();

    this->moveToThread(&thread);
    // 处理数据
    connect(&thread, SIGNAL(started()), this, SLOT(onStart()));

    // 退出、删除
    //connect(this, SIGNAL(finished()), &thread, SLOT(quit()));
    //connect(&thread, &QThread::finished, socket, &QObject::deleteLater);
    // 启动线程
    thread.start();

    return true;

}
