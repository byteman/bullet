#ifndef NETTOOLS_H
#define NETTOOLS_H

#include <QObject>
class NetTools
{
public:
    NetTools();
    static QStringList get_local_ip();
    static QStringList get_devices();
};

#endif // NETTOOLS_H
