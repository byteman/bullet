#include "protoparser.h"

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
