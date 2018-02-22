#ifndef PARSER
#define PARSER
#include <QObject>
struct IParser
{
    virtual void parse(QByteArray data)=0;
};
#endif // PARSER

