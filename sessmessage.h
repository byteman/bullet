#ifndef SESSMESSAGE_H
#define SESSMESSAGE_H
#include <QObject>
#include "session.h"
class SessMessage
{
public:
    SessMessage();
    ISession* getSession();
    void setSession(ISession* sess);
    QString id();
    void setId(QString id);
    QByteArray& getData();
    void setData(const QByteArray &data);

    QHostAddress getHost() const;
    void setHost(const QHostAddress &host);

    quint16 getPort() const;
    void setPort(const quint16 &port);

private:
    ISession* m_sess;
    QString m_id;
    QByteArray m_data;
    QHostAddress m_host;
    quint16 m_port;
};



#endif // SESSMESSAGE_H
