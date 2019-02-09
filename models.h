#ifndef MODELS_H
#define MODELS_H

#include <QString>
struct DeviceInfo{
    QString serialNo;
    QString name;
};
typedef QList<DeviceInfo> DeviceInfoList;
struct DeviceChnConfig{

    QString chanName; //通道的名称.
    int chanNr;
    int maxValue;
    int minValue;
    int paused; //是否暂停. 1 暂停 0 继续.
    DeviceChnConfig()
    {
        maxValue = 0;
        minValue = 0;
        chanNr = 0;
        chanName="0";
        paused = 0;
    }
};

struct DeviceData{
    //QString serialNo; //设备编号.
    qint8  chan; //通道.
    qint64 timestamp; //时间戳
    qint32 value; //测力值.
};
typedef QList<DeviceData> DeviceDataList;
#endif // MODELS_H
