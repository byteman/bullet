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

private:
    ISession* m_sess;
    QString m_id;
    QByteArray m_data;
};



#endif // SESSMESSAGE_H
