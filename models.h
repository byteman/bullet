#ifndef MODELS_H
#define MODELS_H

#include <QString>
#include <QMap>
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
    int startTime; //开始时间
    int endTime; //结束时间
    DeviceChnConfig()
    {
        maxValue = 0;
        minValue = 0;
        chanNr = 0;
        chanName="";
        paused = 1;
        startTime = 0;
        endTime = 0;
    }
};

struct DeviceData{
    //QString serialNo; //设备编号.
    qint8  chan; //通道.
    qint64 timestamp; //时间戳
    qint32 value; //测力值.
};
typedef QVector<DeviceData> DeviceDataList;
typedef QMap<int,DeviceDataList> MultiDeviceDataMap;
#endif // MODELS_H
