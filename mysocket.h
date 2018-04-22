#ifndef MYSOCKET_H
#define MYSOCKET_H

#include "Poco/Net/DatagramSocket.h"
class MySocket
{
public:
    MySocket();
    void bind(const Poco::Net::SocketAddress& address, bool reuseAddress = false);
};

#endif // MYSOCKET_H
