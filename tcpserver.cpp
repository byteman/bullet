#include "tcpserver.h"
#include <QThread>

TcpServer::TcpServer(QObject *parent) : QObject(parent)
{
  m_tcpServer = new QTcpServer(this);
  //设置最大允许连接数，不设置的话默认为30
  m_tcpServer->setMaxPendingConnections(2000);
//    QDBG << m_tcpServer->maxPendingConnections();
  connect(m_tcpServer,SIGNAL(newConnection()),this,SLOT(newConnectSlot()));
  //connect(this,SIGNAL(stopServer()),this,SLOT(uninit()));
  if(thread()!=NULL)
  {
      qDebug() <<"Server construct thread id=" << QThread::currentThread();
  }
}

TcpServer::~TcpServer()
{

}

void TcpServer::uninit()
{
    qDebug() <<"Server uninit thread id=" << QThread::currentThread();

    disconnect(&m_thread,&QThread::finished, this,&TcpServer::uninit);

    m_tcpServer->close();
    emit HasStop();
}

bool TcpServer::init(int port)
{
    this->moveToThread(&m_thread);

    connect(&m_thread,&QThread::started, this,&TcpServer::onStarted);
    connect(&m_thread,&QThread::finished, this,&TcpServer::uninit);

    m_port = port;
    m_thread.start();
    return true;
}

void TcpServer::sendData(QString ip, QString data)
{

}
void TcpServer::sendData(QString ip, uchar *rawData)
{
   // m_mapClient.value(ip)->send(QByteArray((char*)rawData));
}

void TcpServer::test()
{
    qDebug() <<"Server test thread id=" << QThread::currentThread();
}

void TcpServer::stopServer()
{
    disconnect(&m_thread,&QThread::started, this,&TcpServer::onStarted);

    m_thread.quit();
    m_thread.wait();
}

void TcpServer::onStarted()
{
    if(thread()!=NULL)
    {
        qDebug() <<"Server onStarted thread id=" << QThread::currentThread();
    }
    if(m_tcpServer->listen(QHostAddress::Any, m_port)){
        emit ListenResult(true);
    }else{
        //QDBG << "listen error!";
        emit ListenResult(false);
    }

}

void TcpServer::newConnectSlot()
{
    QTcpSocket *tcp = m_tcpServer->nextPendingConnection();
    connect(tcp,SIGNAL(readyRead()),this,SLOT(readMessage()));
    TcpSession* sess = new TcpSession(tcp);
    m_mapClient.insert(tcp->peerAddress().toString(),sess );
    SessMessage msg;
    msg.setSession(sess);
    emit NewConnection(msg);

    connect(tcp,SIGNAL(disconnected()),this,SLOT(removeUserFormList()));

}

void TcpServer::readMessage()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());

    if(m_mapClient.contains(socket->peerAddress().toString()))
    {
        SessMessage msg;
        msg.setSession(m_mapClient[socket->peerAddress().toString()]);
        emit Message(msg , socket->readAll());
    }

}

void TcpServer::removeUserFormList()
{
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

    QMap<QString, TcpSession *>::iterator it;

    for(it=m_mapClient.begin();it!=m_mapClient.end();it++)
    {
        if(socket->peerAddress().toString() == it.key())
        {
            m_mapClient.erase(it);
            SessMessage msg;
            msg.setId(socket->peerAddress().toString());
            emit DisConnecting(msg);

            break;
        }
    }
}
