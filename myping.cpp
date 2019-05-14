#include "myping.h"
#include <QDebug>
#include <iostream>
#include <ostream>
#include <sstream>      // std::ostringstream
MyPing::MyPing():
    _icmpClient(IPAddress::IPv4)
{

    _icmpClient.pingBegin += Poco::delegate(this, &MyPing::onBegin);
    _icmpClient.pingReply += Poco::delegate(this, &MyPing::onReply);
    _icmpClient.pingError += Poco::delegate(this, &MyPing::onError);
    _icmpClient.pingEnd   += Poco::delegate(this, &MyPing::onEnd);
}

void MyPing::ping(QString target, int repet)
{
     static std::string host = target.toStdString();
     try{
         _icmpClient.ping(host, repet);
     }catch(Poco::Exception& e){
         qDebug() << e.displayText().c_str();
     }

}
void MyPing::onBegin(const void* pSender, ICMPEventArgs& args)
{
//    std::ostringstream os;
//    os << "Pinging " << args.hostName() << " [" << args.hostAddress() << "] with " << args.dataSize() << " bytes of data:"
//       << std::endl << "---------------------------------------------" << std::endl;
//    std::string msg = os.str();
//    emit onReply(msg.c_str());
}

void MyPing::onReply(const void* pSender, ICMPEventArgs& args)
{
//    std::ostringstream os;
//    os << "Reply from " << args.hostAddress()
//       << " bytes=" << args.dataSize()
//       << " time=" << args.replyTime() << "ms"
//       << " TTL=" << args.ttl();

//    std::string msg = os.str();
//    emit onReply(msg.c_str());
}

void MyPing::onError(const void* pSender, ICMPEventArgs& args)
{

//    std::ostringstream os;
//    os << args.error();
//    std::string msg = os.str();
//    emit onReply(msg.c_str());
}

void MyPing::onEnd(const void* pSender, ICMPEventArgs& args)
{
//    std::ostringstream os;
//    os << std::endl << "--- Ping statistics for " << args.hostName() << " ---"
//       << std::endl << "Packets: Sent=" << args.sent() << ", Received=" << args.received()
//       << " Lost=" << args.repetitions() - args.received() << " (" << 100.0 - args.percent() << "% loss),"
//       << std::endl << "Approximate round trip times in milliseconds: " << std::endl
//       << "Minimum=" << args.minRTT() << "ms, Maximum=" << args.maxRTT()
//       << "ms, Average=" << args.avgRTT() << "ms"
//       << std::endl << "------------------------------------------";
//    std::string msg = os.str();
//    emit onReply(msg.c_str());
}

