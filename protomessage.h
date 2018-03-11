#ifndef PROTOMESSAGE_H
#define PROTOMESSAGE_H

#include <QObject>
#include <QVector>
#include <QDateTime>
enum MessageID{
   MSG_START_WAVE=0x1, //启动发送波形
   MSG_WAVE_DATA, //设备上报的波形文件
   MSG_HEART, //设备上报心跳
   MSG_READ_PARAM, //读取设备参数
   MSG_WRITE_PARAM, //修改设备厂商
   MSG_RESET, //复位设备
   MSG_ENUM_FILES, //枚举设备sd卡内的波形文件.
   MSG_START_REC_WAVE=8,
   MSG_STOP_REC_WAVE,
};
typedef quint16 INT16U;
typedef quint8  INT8U;

#pragma pack(push)
#pragma pack(1)

/*IP地址*/
struct sIP_ADDR{
    sIP_ADDR()
    {
        addr1=192;
        addr2=168;
        addr3=0;
        addr4=1;
    }
    sIP_ADDR(quint8 a1,quint8 a2,quint8 a3,quint8 a4):
        addr1(a1),
        addr2(a2),
        addr3(a3),
        addr4(a4)
    {}
    QString toString()
    {
        return QString::asprintf("%d.%d.%d.%d",addr1,addr2,addr3,addr4);
    }
    void fromIp(quint32 ip)
    {
        addr1 = ip>>24;
        addr2 = ip>>16;
        addr3 = ip>>8;
        addr4 = ip;

    }
    quint8 addr1;
    quint8 addr2;
    quint8 addr3;
    quint8 addr4;
};

/*本机MAC地址*/
struct sMAC_ADDR{
    char addr1;
    char addr2;
    char addr3;
    char addr4;
    char addr5;
    char addr6;
};

/*本机IP地址*/
struct Glocal_IP_ADDR{
    sIP_ADDR ipaddr;                                    /*本机IP地址*/
    sIP_ADDR SubnetMask;                                /*本机掩码地址*/
    sIP_ADDR GateWay;                                   /*本机网关地址*/
};

///*服务器IP地址，和端口号*/
struct sServerADDR{
    sIP_ADDR ipaddr;
    quint32 port;
    sServerADDR()
    {
        port = 8888;
    }
};
struct sDateTime{
    unsigned char m_year;		//年
    unsigned char m_month;	//月
    unsigned char m_day;		//日
    unsigned char m_hour;		//时
    unsigned char m_minute;	//分
    unsigned char m_seconds;	//秒
    QString toString()
    {
        return QString::asprintf("%04d-%02d-%02d %02d:%02d:%02d",2000+m_year,m_month,m_day,m_hour,m_minute,m_seconds);
    }
    void now()
    {
        QDateTime dt = QDateTime::currentDateTime();
        m_year = dt.date().year()-2000;
        m_month = dt.date().month();
        m_day = dt.date().day();
        m_hour = dt.time().hour();
        m_minute = dt.time().minute();
        m_seconds = dt.time().second();
    }
    sDateTime()
    {
       now();
    }
    sDateTime(quint8 year,quint8 month,quint8 day,quint8 hour,quint8 min,quint8 sec):
        m_year(year),
        m_month(month),
        m_day(day),
        m_hour(hour),
        m_minute(min),
        m_seconds(sec)
    {

    }
};
#define MAX_WIFI_BUF 18
struct MsgDevicePara
{
    INT16U	mWorkMode;	//采集方式
    INT16U	mWetUp;		//	重量上限值
    INT16U  mWetDown;		//重量下限值

    Glocal_IP_ADDR Local_IP; //IP 地址
    sServerADDR Server_ip;	//服务器IP
    INT8U	mWifiSSID[MAX_WIFI_BUF];  //ssid 名字
    INT8U	mWifiPass[MAX_WIFI_BUF];  //pass 密码
    sDateTime mDateTime; //时间和日期.


    INT8U 	checksum;
    MsgDevicePara()
    {
        mWorkMode = 0;
        mWetUp = 0;
        mWetDown = 0;
        memset(mWifiSSID,0,MAX_WIFI_BUF);
        strcpy((char*)mWifiSSID,"ssid");
        memset(mWifiPass,0,MAX_WIFI_BUF);
        strcpy((char*)mWifiPass,"123456");
    }
    void toByteArray(QByteArray& data)
    {

        data.append((const char*)this,sizeof(MsgDevicePara));
    }
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
        quint8 sum = 0;
        for(int i = 0; i < output.size(); i++)
        {
            sum += output[i];
        }
        output.append(sum);
    }
    bool getData(void* dest, int size)
    {
        int sz = data.size();
        if(sz != size)
        {
            return false;
        }
        memcpy(dest, data.data(),size);

        return true;
    }
    bool getDateTime(sDateTime& sdt)
    {

        if(data.size() != sizeof(sDateTime))
        {
            return false;
        }
        memcpy(&sdt, data.data(),sizeof(sDateTime));

        return true;
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
    QByteArray toByteArray()
    {
        QByteArray output;
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
        return output;
    }
    void addPara(MsgDevicePara& para)
    {
        data.append((const char*)&para,sizeof(MsgDevicePara));
    }

    ProtoHeader head;
    bool is_ack; //是否回应数据.
    QByteArray data; //数据.
};
//PC->Dev 发送启动发送波形命令
struct MsgStartSendWave:public ProtoMessage{

};
struct MsgFileInfo{
    QString name;
    char    attr; //1 file 2 dir
};
typedef QList<MsgFileInfo> MsgFileList;
//一个通道的数据总和.
typedef QVector<double> ChannelData;

struct MsgWaveData:public ProtoMessage{
  QVector<ChannelData> channels; //一个设备包含n个通道的数据.
  QString m_dev_serial;//设备唯一序列号
  bool m_first; //是否是某次采集数据的首包数据.
  MsgWaveData()
  {
      m_first = false;
  }
};
#endif // PROTOMESSAGE_H
