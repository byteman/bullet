#ifndef IMSGINTERFACE_H
#define IMSGINTERFACE_H
struct IMsgInterface{
    virtual void devOnline(QString client)=0;
    virtual void devOffline(QString client)=0;
    virtual void recvFromServer(QString client, QString msg)=0;
};
#endif // IMSGINTERFACE_H

