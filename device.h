#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include "protomessage.h"
#include "wavefile.h"
#include "csvfile.h"
#include "session.h"
#include <QTimer>
#include <QJsonDocument>
#include "syncfile.h"
#include "models.h"
#include <QQueue>
struct DeviceChannel{
    QQueue<SensorData> values;
    DeviceChnConfig config;

};
class Device:public QObject
{
    Q_OBJECT
public:
    Device();
    bool online() const;
    void setOnline(bool online);

    //检查设备是否在线.
    void checkOnline();
    QString name() const;
    void setName(const QString &name);
    void onMessage(ProtoMessage &reqest, ProtoMessage &response);
    QString id() const;
    void setId(const QString &id);
    void RemoveFile(QString file);
    ISession *sess() const;
    void setSess(ISession *sess);
    void setHostPort(QHostAddress host,quint16 port);

    bool LoadWaveFile(QString file, MsgWaveData &wvd);
    void ReadParam();
    void WriteParam(MsgDevicePara &para);
    void ReadRt();
    qint64 WriteCmd(quint8 cmd, QByteArray &buf);

    bool Reset(quint8 delay_s);
    bool calib(quint8 chan,  int point,int weigh);
    quint32 ad() const;
    void setAd(const qint32 &ad);
    qint64 SendData(QByteArray &data);
    qint32 weight() const;
    bool IsPaused(int chan);
    void UpdateState(int chan,bool pause);
    void UpdateChanConfig(int chan, DeviceChnConfig& cfg);
    QQueue<SensorData>* GetHistoryData(int chan);

private:
    QMap<int,DeviceChannel> m_channels;
    qint32 m_ad;
    qint32 m_weight;
    QString m_dev_id;
    quint16 m_serial_id;
    QString m_name;
    QString m_dir;
    QHostAddress m_host;
    quint16 m_port;
    bool m_start_send;
    bool m_use_sys_time;
    bool m_online; //设备是否在线
    bool m_paused; //是否暂停录制.
    int  m_timeout; //在线超时定时器.
    int  m_packet_count; //接收包计数器。
    quint16 m_sess_id; //波形回应中的当前会话.
    ISession* m_sess;
    QString CreateDir();
    QString GetFileName();
    void DevNotify(QString msg);

    void ProcessWave(int index, QByteArray &data);

    QTimer timer;

    void SaveWave(ProtoMessage &msg);

    void WriteValues(SensorData &data);
signals:
    void CommResult(Device* dev,int cmd, int result);
    void showWave(Device* dev, MsgWaveData wave);
    void OnSensorData(Device* dev, MsgSensorData wave);
    void Progress(Device* dev,QString progress);
    void Notify(QString msg);
    void ReadParam(Device* dev,MsgDevicePara para);
    void WriteParam(Device* dev, bool result);
    void CalibResult(Device* dev, int chan, int index, int result);
    void RealTimeResult(Device* dev,RT_AD_RESULT result);
    void ResetResult(Device* dev,bool result);
private slots:
    void timeout();
};

#endif // DEVICE_H
