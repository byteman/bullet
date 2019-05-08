#include "machinechecker.h"
#include <QNetworkAddressEntry>
#include <QNetworkInterface>
MachineChecker::MachineChecker()
{

}

bool MachineChecker::CheckMac(QString mac)
{
    return true;
    foreach (QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
    {
        //设备名
        qDebug() << "Device:" << netInterface.name();

        //MAC地址
        qDebug() << "HardwareAddress:" << netInterface.hardwareAddress();
        if(netInterface.hardwareAddress() == mac){
            return true;
        }


    }
    return false;
}

