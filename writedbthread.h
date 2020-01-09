#ifndef WRITEDBTHREAD_H
#define WRITEDBTHREAD_H

#include <QObject>
#include <QThread>
#include "dao.h"
class WriteDBThread:public QObject
{
    Q_OBJECT
public:
    static WriteDBThread& instance();
    WriteDBThread();
    void startThread();
    void stopThread();
    void WriteData(QString serialNo, DeviceDataList data);
    void DeviceChanDataRemove(QString serialNo,int chan);
    void DeviceDataTblRemove(QString serialNo);
    void DeviceDataRemove(QString serialNo);
    void DeviceDataRemove(QString serialNo,qint64 from,qint64 to);

private slots:
    void _WriteData(QString serialNo, DeviceDataList data);
    void _DeviceChanDataRemove(QString serialNo,int chan);
    void _DeviceDataTblRemove(QString serialNo);
    void _DeviceDataRemove(QString serialNo);
    void _DeviceDataRemove(QString serialNo,qint64 from,qint64 to);
signals:
    //void _WriteData(QString serialNo, DeviceDataList &data);
private:
    QThread _thread;
};

#endif // WRITEDBTHREAD_H
