#ifndef MACHINECHECKER_H
#define MACHINECHECKER_H

#include <QObject>
class MachineChecker
{
public:
    MachineChecker();
    bool CheckMac(QString mac);
};

#endif // MACHINECHECKER_H
