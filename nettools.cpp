#include "nettools.h"
#include <QNetworkAddressEntry>
NetTools::NetTools()
{

}
QStringList NetTools::get_devices(){
    QStringList addrs;
    foreach (QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
    {

            QList<QNetworkAddressEntry> ipAddressesList = netInterface.addressEntries();

            for (int i = 0; i < ipAddressesList.size(); ++i) {
                //if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                //    ipAddressesList.at(i).toIPv4Address()) {

                    addrs.push_back(QString("[%1]-[%2]-[%3]-[%4]-[%5]")
                                    .arg(netInterface.humanReadableName())
                                    .arg(netInterface.hardwareAddress())
                                    .arg(ipAddressesList.at(i).ip().toString())
                                    .arg(ipAddressesList.at(i).netmask().toString())
                                     .arg(ipAddressesList.at(i).broadcast().toString()));

                    //ipAddress.push_back(ipAddressesList.at(i).toString());

              //  }
            }




    }
    return addrs;
}

/*
 * 名称:get_localmachine_ip
 * 功能:获取本机的IP地址
 * 参数:no
 * 返回:QString
 */
QStringList NetTools::get_local_ip()
{
    QStringList ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress.push_back(ipAddressesList.at(i).toString());

        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress.push_back(QHostAddress(QHostAddress::LocalHost).toString());
    return ipAddress;
}
