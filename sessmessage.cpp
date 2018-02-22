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

