#ifndef UDPSERVER_H
#define UDPSERVER_H
#include <QObject>
#include <QUdpSocket>
#include <QThread>
#include "sessmessage.h"
#include "udpsession.h"
#include <QTimer>
class UdpServer:public QObject
{
    Q_OBJECT
public:
    UdpServer();
    bool stop();
    bool start(int port);
signals:
    void ListenResult(bool result);
    void HasStop();
    void Message(SessMessage msg);
protected slots:
    void readPendingDatagrams();
    void socketError(QAbstractSocket::SocketError err);
private slots:
    void onStart();
private:
    bool stopped;
    int _port;
    QThread thread;
    QUdpSocket  *socket;
    UdpSession* udp_sess;

};

#endif // UDPSERVER_H
