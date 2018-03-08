#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>
#include "sessmessage.h"
#include "protomessage.h"
#include "protoparser.h"
#include "device.h"
#include <QMap>
#include <QList>
class DeviceManager:public QObject
{
Q_OBJECT
public:
    DeviceManager();
    bool start();
    void stop();
    void load(int index, int channel);
    //发送全部设备最近的波形文件
    bool SendAllWave();
    bool StartAll(bool start);

    bool ResetAllDevice(quint8 delay_s);
    bool ResetDevice(quint32 dev_id,quint8 delay_s);
    bool ListFiles(quint32 dev_id);
    void ReadParam(quint32 dev_id);
    void WriteParam(quint32 dev_id , MsgDevicePara &para);
    void SetStation(QString station);
    void ListDevice(QList<Device*> &devices);
    Device* GetDevice(quint32 dev_id);
    void GetDeviceWaveFiles(quint32 dev_id,QStringList &files);
    bool LoadWaveFile(quint32 dev_id, QString file, MsgWaveData& wvd);

public slots:
    void onWaveMsg(Device* dev,MsgWaveData wvData);
    void onReadParam(Device* dev,MsgDevicePara para);
    void onWriteParam(Device* dev, bool result);
    void Message(SessMessage msg);
    void onNotify(QString msg);
    void onEnumFiles(Device* dev,MsgFileList files);
    void onProgress(Device* dev,QString progress);

private:
    int m_dev_num;
    QMap<quint32,QString> id_name_map;
    QMap<quint32,Device*> dev_map;
    QStringList names;
    QStringList ids;
    ProtoParser parser;
    quint16 m_serial_id;
    quint16 m_session_id;
signals:
    void ReadParam(Device* dev,MsgDevicePara para);
    void WriteParam(Device* dev, bool result);
    void SendData(SessMessage msg);
    void DevOnline(Device* dev);
    void DevOffline(Device* dev);
    void Notify(QString msg);
    void Progress(Device* dev,QString progress);
    void EnumFiles(Device* dev,MsgFileList files);
    void WaveMsg(Device* dev, MsgWaveData data);
    // QObject interface
protected:
    void timerEvent(QTimerEvent *);
};

#endif // DEVICEMANAGER_H
