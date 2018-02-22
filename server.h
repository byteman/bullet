#ifndef SERVER_H
#define SERVER_H


#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QNetworkInterface>
#include "sessmessage.h"
#include "tcpsession.h"
class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    ~Server();

    bool init(int port);
    void sendData(QString ip, QString data);
    void sendData(QString ip, uchar *rawData);
    void test();

    void stopServer();
signals:
    void ListenResult(bool result);
    void HasStop();
    void NewConnection(SessMessage msg);
    void DisConnecting(SessMessage msg);
    void Message(SessMessage msg,QByteArray data);

private slots:
    void uninit();
    void onStarted();
    void newConnectSlot();
    void readMessage();
    void removeUserFormList();

private:
    int m_port;
    QThread     m_thread;
    QTcpServer *m_tcpServer;
    QMap<QString, TcpSession *> m_mapClient;
};


#endif // SERVER_H
