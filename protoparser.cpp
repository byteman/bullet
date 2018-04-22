#include "protoparser.h"
#include <QDebug>
ProtoParser::ProtoParser()
{

}

bool ProtoParser::parseData(QByteArray &data,ProtoMessage& prot_msg)
{
    while(data.size() > sizeof(ProtoHeader)){
        quint8 x1=0xFE;
        quint8 x2 = 0x7F;
        quint8 v1 = data[0];
        quint8 v2 = data[1];

        if( (v1 == x1) && (v2 == x2)){
            memcpy((void*)&prot_msg.head,data.data(),sizeof(ProtoHeader));

            prot_msg.is_ack = (prot_msg.head.cmd_id&0x80);
            prot_msg.head.cmd_id = (prot_msg.head.cmd_id&0x7F);

            if(prot_msg.head.length > data.size() ||
               prot_msg.head.length < 1     )
            {
                //qDebug() << "size error";
                //qDebug() << data.toHex();
                data.remove(0,1);
                continue;
            }
            if(!checkSum(data,prot_msg.head.length))
            {
                qDebug() << "checkSum error";
                data.remove(0,1);
                continue;
            }
            data.remove(0,sizeof(ProtoHeader));
            int len =  prot_msg.head.length -sizeof(ProtoHeader)-1;

            if(data.size() < (len+1))
            {
                data.clear();
                return false;
            }
            QByteArray temp = data.mid(0 ,len);
            data.remove(0,len+1);
            prot_msg.data.push_back(temp);
            return true;
        }
        data.remove(0,1);
    }

    return false;
}
bool ProtoParser::checkSum(QByteArray& data,int size)
{
    quint8 sum = 0;
    for(int i = 0; i < size-1; i++)
    {
        quint8 v = data[i];
        sum+=v;
    }
    quint8 v2 = data[size-1];
    return sum == v2;
}
bool ProtoParser::parseData(ProtoMessage &prot_msg)
{
    //qDebug() << "sz=" << m_data.size() << " size=" << sizeof(ProtoHeader);
//    if(m_data.size() <= sizeof(ProtoHeader))
//    {
//        return false;
//    }

    while(m_data.size() > sizeof(ProtoHeader)+1){
        quint8 x1=0xFE;
        quint8 x2 = 0x7F;
        quint8 v1 = m_data[0];
        quint8 v2 = m_data[1];

        if( (v1 == x1) && (v2 == x2)){
            memcpy((void*)&prot_msg.head,m_data.data(),sizeof(ProtoHeader));

            prot_msg.is_ack = (prot_msg.head.cmd_id&0x80);
            prot_msg.head.cmd_id = (prot_msg.head.cmd_id&0x7F);
            if(prot_msg.head.length > m_data.size() ||
                    prot_msg.head.length < 1     )
            {
                //qDebug() << "size error";
                m_data.remove(0,1);
                continue;
            }
#if 1
            if(!checkSum(m_data,prot_msg.head.length))
            {
                qDebug() << "checkSum error";
                m_data.remove(0,1);
                continue;
            }
#endif
            m_data.remove(0,sizeof(ProtoHeader));
            int len =  prot_msg.head.length -sizeof(ProtoHeader)-1;

            if(m_data.size() < (len+1))
            {
                m_data.clear();
                return false;
            }
            QByteArray temp = m_data.mid(0 ,len);
            m_data.remove(0,len+1);
            prot_msg.data.push_back(temp);
            return true;
        }
        m_data.remove(0,1);
    }

    return false;

}

void ProtoParser::pushData(QByteArray &data)
{
    m_data.push_back(data);
}
