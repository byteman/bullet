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
//日志数据
struct LogData{
    QString serialNo; //设备编号.
    QString name; //设备名称.
    qint64  timestamp; //操作时间戳
    QString oper; //操作的名称.
    int     result ;// 操作的结果
    QString err; //错误原因.
};
typedef QVector<LogData> LogDataList;
typedef QVector<DeviceData> DeviceDataList;
typedef QMap<int,DeviceDataList> MultiDeviceDataMap;
#endif // MODELS_H
