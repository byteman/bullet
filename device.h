#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include "protomessage.h"
#include "wavefile.h"
#include "csvfile.h"
#include "session.h"
#include <QTimer>
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
    void setHostPort(QHostAddress host,quint16 port);
    qint64 SendStartWave(quint16 sess_id,bool start);
    qint64 StartRecWave(quint16 sess_id,bool start);
    bool LoadWaveFile(QString file, MsgWaveData &wvd);
    void ReadParam();
    void WriteParam(MsgDevicePara &para);
    qint64 WriteCmd(quint8 cmd, QByteArray &buf);
    bool ListFiles();
    bool Reset(quint8 delay_s);
    bool calib(quint8 chan,  int point,int weigh);
private:
    quint32 m_dev_id;
    quint16 m_serial_id;
    QString m_name;
    QString m_station;
    QString m_dir;
    QHostAddress m_host;
    quint16 m_port;
    bool m_start_send;
    bool m_online; //设备是否在线
    int  m_timeout; //在线超时定时器.
    int  m_packet_count; //接收包计数器。
    quint16 m_sess_id; //波形回应中的当前会话.
    //WaveFile *m_file;
    CSVFile *m_file;
    ISession* m_sess;
    QString CreateDir();
    QString GetFileName();
    void DevNotify(QString msg);
    void sendProgress(int sample_start, int sample_total);
    void ProcessWave(int index, QByteArray &data);
    qint64 SendData(QByteArray &data);
    QTimer timer;
    void OpenFile();
    void CloseFile();
signals:
    void showWave(Device* dev, MsgWaveData wave);
    void Progress(Device* dev,QString progress);
    void Notify(QString msg);
    void ReadParam(Device* dev,MsgDevicePara para);
    void WriteParam(Device* dev, bool result);
    void EnumFiles(Device* dev,MsgFileList files);
private slots:
    void timeout();
};

#endif // DEVICE_H
