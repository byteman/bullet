#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>
#include "sessmessage.h"
#include "protomessage.h"
#include "protoparser.h"
#include "device.h"
#include "syncfile.h"
#include "models.h"
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
    bool SendAllWave(bool start);
    bool SendWave(QString dev_id,bool start);
    bool StartAll(bool start);

    bool SyncFile(QString dev_id,QString file);
    bool ResetAllDevice(quint8 delay_s);
    bool ResetDevice(QString dev_id,quint8 delay_s);
    bool ListFiles(QString dev_id,int page, int size);
    void calib(QString dev_id,quint8 chan,quint8 index,int weight);
    bool RemoveDevice(QString dev_id);
    bool AddDevice(QString dev_id,QString dev_name);
    bool UpdateDevice(QString dev_id,QString dev_name);

    void ReadParam(QString dev_id);
    void ReadRt(QString dev_id);
    void WriteParam(QString dev_id , MsgDevicePara &para);
    void SetStation(QString station);
    void ListDevice(QList<Device*> &devices);
    Device* GetDevice(QString dev_id);
    void GetDeviceWaveFiles(QString dev_id,QStringList &files);
    bool LoadWaveFile(QString dev_id, QString file, MsgWaveData& wvd);

    bool RemoveFile(QString dev_id, QString file);
    bool UpdateDeviceChan(QString dev_id, int chan, DeviceChnConfig &cfg);
    bool GetDeviceChan(QString dev_id, int chan, DeviceChnConfig &cfg);
public slots:
    void onCommResult(Device* dev,int cmd, int result);
    void onWaveMsg(Device* dev,MsgWaveData wvData);
    void onSensorMsg(Device* dev,MsgSensorData msData);
    void onReadParam(Device* dev,MsgDevicePara para);
    void onWriteParam(Device* dev, bool result);
    void Message(SessMessage msg);
    void onNotify(QString msg);
    void onEnumFiles(Device* dev,ENUM_FILE_RESP resp);
    void onProgress(Device* dev,QString progress);
    void onCalibResult(Device* dev, int chan, int index, int result);
    void onRealTimeResult(Device* dev,RT_AD_RESULT result);
    void onResetResult(Device* dev, bool result);
private:
    int m_dev_num;

    QMap<QString,Device*> dev_map;
    QMutex dev_lock;
    QStringList names;
    ProtoParser parser;
    quint16 m_serial_id;
    quint16 m_session_id;
signals:
    void CommResult(Device* dev,int cmd, int result);
    void ReadParam(Device* dev,MsgDevicePara para);
    void WriteParam(Device* dev, bool result);
    void SendData(SessMessage msg);
    void DevOnline(Device* dev);
    void DevOffline(Device* dev);
    void Notify(QString msg);
    void Progress(Device* dev,QString progress);
    void EnumFiles(Device* dev,ENUM_FILE_RESP resp);
    void WaveMsg(Device* dev, MsgWaveData data);
    void SensorMsg(Device*,MsgSensorData);
    void CalibResult(Device* dev, int chan, int index, int result);
    void RealTimeResult(Device* dev,RT_AD_RESULT result);
    void ResetResult(Device* dev, bool result);
    // QObject interface
protected:
    void timerEvent(QTimerEvent *);
};

#endif // DEVICEMANAGER_H
