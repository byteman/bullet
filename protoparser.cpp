#include "protoparser.h"
#include <QDebug>
ProtoParser::ProtoParser()
{

}

bool ProtoParser::parseData(QByteArray &data,ProtoMessage& prot_msg)
{
    //13header + nbytes + 1checksum
    if(data.size() <= sizeof(ProtoHeader))
    {
        return false;
    }
    memcpy((void*)&prot_msg.head,data.data(),sizeof(ProtoHeader));


    prot_msg.is_ack = (prot_msg.head.cmd_id&0x80);
    prot_msg.head.cmd_id = (prot_msg.head.cmd_id&0x7F);
    int index = sizeof(ProtoHeader);
    int len = data.size()-sizeof(ProtoHeader)-1;
    QByteArray temp = data.mid(index ,len);
    prot_msg.data.push_back(temp);
    return true;
}
bool ProtoParser::checkSum(QByteArray& data,int size)
{
    quint8 sum = 0;
    for(int i = 0; i < size-1; i++)
    {
        sum+=data[i];
    }
    return sum == data[size-1];
}
bool ProtoParser::parseData(ProtoMessage &prot_msg)
{
    qDebug() << "sz=" << m_data.size() << " size=" << sizeof(ProtoHeader);
//    if(m_data.size() <= sizeof(ProtoHeader))
//    {
//        return false;
//    }

    while(m_data.size() > sizeof(ProtoHeader)+1){
        if( (m_data[0] == 0xFE) && (m_data[1] == 0x7F)){
            memcpy((void*)&prot_msg.head,m_data.data(),sizeof(ProtoHeader));

            prot_msg.is_ack = (prot_msg.head.cmd_id&0x80);
            prot_msg.head.cmd_id = (prot_msg.head.cmd_id&0x7F);

//            if(!checkSum(m_data,prot_msg.head.length))
//            {
//                m_data.clear();
//                return false;
//            }
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
