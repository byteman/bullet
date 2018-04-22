#ifndef GPSSERVER_H
#define GPSSERVER_H


#include <QThread>
//#include <QUdpSocket>
#include <QMap>
#include <QMutex>
#include "session.h"
#include "sessmessage.h"



class GPServer:public QObject
{
    Q_OBJECT
public:
   GPServer(QObject *parent=0);
   ~GPServer();

   bool start(int port);
   int  stop();
   bool IsStarted();


public slots:
    void onStart();
signals:
    void finished();
    void error(QString err);
    void onPacket(QString msg);
    void Message(SessMessage msg);

private:

   QThread thread;
   //QUdpSocket *socket;

   qint64 _lastTs;
   int _port;
   bool    _isStarted;
   bool    _quit;
   bool    _enableLog;
   ISession* udp_sess;
   bool startThread();

   bool initSocket(int port);


};


#endif // UDPSERVER_H
