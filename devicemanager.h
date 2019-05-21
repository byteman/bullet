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
//初始化
    bool Init();
    void Uninit();
//复位
    bool ResetAllDevice(quint8 delay_s);
    bool ResetDevice(QString dev_id,quint8 delay_s);
    void ResetDeviceCount(QString dev_id);
    int  GetDeviceCount(QString dev_id);
//设备管理接口.
    bool SetSaveInt(int s);
    bool RemoveDevice(QString dev_id);
    bool AddDevice(QString dev_id,QString dev_name);
    bool UpdateDevice(QString dev_id,QString dev_name);
    void ListDevice(QList<Device*> &devices);
    Device* GetDevice(QString dev_id);
//设备参数读取
    void ReadParam(QString dev_id);
    void WriteParam(QString dev_id , MsgDevicePara &para);
//设备通道管理.
    bool UpdateDeviceChan(QString dev_id, int chan, DeviceChnConfig &cfg);
    bool GetDeviceChan(QString dev_id, int chan, DeviceChnConfig &cfg);
    //设备通道标定.
    void calib(QString dev_id,quint8 chan,quint8 index,int weight);
    //控制某个设备通道是否暂停或者启用.
    bool ControlDeviceChan(QString dev_id, int chan,bool pause);
//获取最后的错误.
    QString GetLastError();
    //bool UpdateRecState(QString &dev_id, int chan, bool pause);
    bool SetChanConfig(QString &dev_id, int chan,DeviceChnConfig& cfg);
    int DeviceCount();
    void Sync();
public slots:
    void onCommResult(Device* dev,int cmd, int result);
    void onWaveMsg(Device* dev,MsgWaveData wvData);
    void onSensorMsg(Device* dev,MsgSensorData msData);
    void onReadParam(Device* dev,MsgDevicePara para);
    void onWriteParam(Device* dev, bool result);
    void Message(SessMessage msg);
    void onNotify(QString msg);
    void onCalibResult(Device* dev, int chan, int index, int result);
    void onRealTimeResult(Device* dev,RT_AD_RESULT result);
    void onResetResult(Device* dev, bool result);
private:

    QString m_last_err;
    QMap<QString,Device*> dev_map;
    QMutex dev_lock;
    ProtoParser parser;
    bool m_use_sys_time;
    int  m_save_int_s;


    bool addOneDevice(QString &serialNo, QString &name);
    void removeOneDevice(Device *dev);
    void WriteValues(Device* dev,MsgSensorData &msg);
signals:
    void CommResult(Device* dev,int cmd, int result);
    void ReadParam(Device* dev,MsgDevicePara para);
    void WriteParam(Device* dev, bool result);
    void SendData(SessMessage msg);
    void DevOnline(Device* dev);
    void DevOffline(Device* dev);
    void Notify(QString msg);
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
