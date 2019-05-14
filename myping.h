#ifndef MYPING_H
#define MYPING_H
#include <Poco/Net/ICMPClient.h>
#include <Poco/AutoPtr.h>
#include <Poco/NumberParser.h>
#include <Poco/Delegate.h>
#include <QObject>
using Poco::Net::ICMPSocket;
using Poco::Net::ICMPClient;
using Poco::Net::IPAddress;
using Poco::Net::ICMPEventArgs;
using Poco::AutoPtr;
using Poco::NumberParser;
using Poco::Delegate;

class MyPing:public QObject
{
     Q_OBJECT
public:
    MyPing();
    void ping(QString target,int repet);
    void onBegin(const void *pSender, ICMPEventArgs &args);
    void onReply(const void *pSender, ICMPEventArgs &args);
    void onError(const void *pSender, ICMPEventArgs &args);
    void onEnd(const void *pSender, ICMPEventArgs &args);
signals:

    void onReply(QString msg);
private:
    ICMPClient  _icmpClient;
};

#endif // MYPING_H
