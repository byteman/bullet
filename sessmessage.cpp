#include "sessmessage.h"

SessMessage::SessMessage():
    m_sess(NULL)
{

}

ISession *SessMessage::getSession()
{
 return m_sess;
}

void SessMessage::setSession(ISession *sess)
{
    m_sess = sess;
}

QString SessMessage::id()
{
    if(m_sess!=NULL)
    {
        return m_sess->id();
    }
    return "";
}

void SessMessage::setId(QString id)
{
    m_id = id;
}
QByteArray &SessMessage::getData()
{
    return m_data;
}
void SessMessage::setData(const QByteArray &data)
{
    m_data = data;
}
QHostAddress SessMessage::getHost() const
{
    return m_host;
}

void SessMessage::setHost(const QHostAddress &host)
{
    m_host = host;
}
quint16 SessMessage::getPort() const
{
    return m_port;
}

void SessMessage::setPort(const quint16 &port)
{
    m_port = port;
}



