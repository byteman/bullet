#ifndef UDPSERVER_H
#define UDPSERVER_H
#include <QObject>
#include <QUdpSocket>
#include "sessmessage.h"
#include "udpsession.h"
#include <QTimer>
class UdpServer:public QObject
{
    Q_OBJECT
public:
    UdpServer();
    bool start(int port);
    bool stop();
signals:
    void ListenResult(bool result);
    void HasStop();
    void Message(SessMessage msg);
protected slots:
    void readPendingDatagrams();
    void timeout();
private:
    QUdpSocket  socket;
    UdpSession* udp_sess;
    QTimer timer;

};

#endif // UDPSERVER_H
