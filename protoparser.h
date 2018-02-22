#ifndef PROTOPARSER_H
#define PROTOPARSER_H
#include "sessmessage.h"
#include "parser.h"
#include "protomessage.h"
class ProtoParser:public QObject
{
    Q_OBJECT
public:
    ProtoParser();
    bool parseData(QByteArray &data,ProtoMessage& prot_msg);

};

#endif // PROTOPARSER_H
