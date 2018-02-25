#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include "protomessage.h"
#include "wavefile.h"
#include "session.h"
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
    quint32 id() const;
    void setId(const quint32 &id);
    void SaveWave(ProtoMessage &msg);
    QString station() const;
    void setStation(const QString &station);
    void listWaveFiles(QStringList &files);

    ISession *sess() const;
    void setSess(ISession *sess);

    qint64 SendStartWave(quint16 sess_id);
    bool LoadWaveFile(QString file, MsgWaveData &wvd);
    void ReadParam();
    void WriteParam(MsgDevicePara &para);
    qint64 WriteCmd(quint8 cmd, QByteArray &buf);
    bool ListFiles();
    bool Reset(quint8 delay_s);
private:
    quint32 m_dev_id;
    quint16 m_serial_id;
    QString m_name;
    QString m_station;
    QString m_dir;
    bool m_online; //设备是否在线
    int  m_timeout; //在线超时定时器.
    quint16 m_sess_id; //波形回应中的当前会话.
    WaveFile *m_file;
    ISession* m_sess;
    QString CreateDir();
    QString GetFileName();
    void DevNotify(QString msg);
signals:
    void Notify(QString msg);
    void ReadParam(Device* dev,MsgDevicePara para);
    void WriteParam(Device* dev, bool result);
};

#endif // DEVICE_H
