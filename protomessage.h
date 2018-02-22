#ifndef PROTOMESSAGE_H
#define PROTOMESSAGE_H

#include <QObject>
#include <QVector>
enum MessageID{
   MSG_START_WAVE=0x1,
   MSG_WAVE_DATA,
   MSG_HEART,
   MSG_READ_PARAM,
   MSG_WRITE_PARAM
};
#pragma pack(push)
#pragma pack(1)
struct DevicePara{
    quint32 host_ip;    //主机ip
    quint16 host_port; //主机端口
    quint32 dev_id; //设备唯一编号
};

struct WaveDataHead{
    quint8 samplebits;
    quint8 nchannel;
    quint32 totalSamples;
    quint32 startSample;
    quint16 nSample;
};
struct ProtoHeader{
    quint16 magic; // 0xFE 0x7F
    quint16 length;// 整条数据包的总长度
    quint32 device_id; //设备编号
    quint16 serial_id; //序号
    quint16 sesson_id; //会话编号
    quint8  cmd_id; //命令编号
};
#pragma pack(pop)

struct ProtoMessage
{
public:
    ProtoMessage();
    void appendSum(QByteArray& output)
    {
        quint8 sum;
        for(int i = 0; i < output.size(); i++)
        {
            sum += output[i];
        }
        output.append(sum);
    }
    void toByteArray(QByteArray& output)
    {
        output.append(0xFE);
        output.append(0x7F);
        head.length = sizeof(ProtoHeader) + 1+ data.size();
        output.append((const char*)&head.length,sizeof(quint16));
        output.append((const char*)&head.device_id,sizeof(quint32));
        output.append((const char*)&head.serial_id,sizeof(quint16));
        output.append((const char*)&head.sesson_id,sizeof(quint16));
        output.append((const char*)&head.cmd_id,sizeof(quint8));

        output.append(data);
        appendSum(output);
    }
    ProtoHeader head;
    bool is_ack; //是否回应数据.
    QByteArray data; //数据.
};
//PC->Dev 发送启动发送波形命令
struct MsgStartSendWave:public ProtoMessage{

};
//一个通道的数据总和.
typedef QVector<double> ChannelData;
//class ChannelData:public QVector<float>
//{

//};
struct MsgWaveData:public ProtoMessage{
  QVector<ChannelData> channels; //一个设备包含n个通道的数据.
  QString m_dev_serial;//设备唯一序列号
  bool m_first; //是否是某次采集数据的首包数据.
};
#endif // PROTOMESSAGE_H
