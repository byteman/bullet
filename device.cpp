#include "device.h"
#include <QDir>
#include <QDebug>
#include <QDateTime>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#define MAX_TIMEOUT 10

/*
波形保存流程
1.发送启动读取命令后,发送sess_id给设备
2.设备以收到sess_id发送波形，波形可以分成多次发送，但是每次发送的数据包中的sess_id要和启动发送时收到的sess_id一致
3.pc软件启动发送后开启一个超时定时器,如果在n秒内没有收到设备的回应，就取消掉此次接收事务.并且关闭文件.
4.如果在超时范围内收全了最后一个包,也关闭文件并完成此事务.
*/
Device::Device():
    m_online(false),
    m_timeout(MAX_TIMEOUT),
    m_serial_id(0),
    m_sess(NULL),
    m_last_save_ts(0),
    m_start_send(true),
    m_packet_count(0)
{
    for(int i = 1; i <= 12; i++)
    {
        m_channels[i] = DeviceChannel();
    }
    //connect(&timer,SIGNAL(timeout()),this,SLOT(timeout()));
}
bool Device::online() const
{
    return m_online;
}

void Device::setOnline(bool online)
{
    m_online = online;
}

qint64 Device::WriteCmd(quint8 cmd,QByteArray &buf)
{
    ProtoMessage msg;
    msg.head.cmd_id = cmd;
    memcpy(msg.head.device_id , m_dev_id.toLocal8Bit().data(),12);
    //msg.head.device_id = m_dev_id;
    //msg.head.serial_id = m_serial_id++;
    msg.head.sesson_id = m_sess_id++;
    msg.is_ack = false;
    msg.data.push_back(buf);
    QByteArray data;
    msg.toByteArray(data);
    return SendData(data);

}

void Device::ResetCount()
{
    m_packet_count = 0;
}

int Device::GetCount()
{
    return m_packet_count;
}
bool Device::calib(quint8 chan, int point,int weight)
{
    QByteArray data;
    PARA_SENSOR_CAL cal;
    cal.mCalPoint =point;
    cal.mCalWet = weight;
    cal.mSensorCh = chan;
    data.append((const char*)&cal,sizeof(PARA_SENSOR_CAL));
    WriteCmd(MSG_CALIB,data);
    return true;
}
quint32 Device::ad() const
{
    return m_ad;
}

void Device::setAd(const qint32 &ad)
{
    m_ad = ad;
}
qint32 Device::weight() const
{
    return m_weight;
}

bool Device::IsPaused(int chan)
{
    //qDebug() << "serial=" << m_dev_id << " chan=" << chan << " paused=" << m_channels[chan].config.paused;

    return m_channels[chan].config.paused;
}

void Device::ReadParam()
{
    QByteArray data;
    WriteCmd(MSG_READ_PARAM,data);
}
bool Device::Reset(quint8 delay_s)
{
    QByteArray data;
    data.append(delay_s);
    WriteCmd(MSG_RESET,data);
    return true;
}
bool Device::ClearHist()
{
    QByteArray data;

    WriteCmd(MSG_HIST_CLEAR_REQ,data);
    return true;
}
void Device::WriteParam(MsgDevicePara &para)
{

   QByteArray data;
   para.toByteArray(data);
   WriteCmd(MSG_WRITE_PARAM,data);
}

void Device::ReadRt()
{
    QByteArray data;
    WriteCmd(MSG_RT_AD,data);
    return;
}

qint64 Device::SendData(QByteArray& data)
{
    if(m_sess)
    {
        emit Notify(QString("send to %1:%2 ").arg(m_host.toString()).arg(m_port) + data.toHex());

        return m_sess->send(m_host,m_port,data);
    }
    return 0;
}

void Device::timeout()
{
    if(m_start_send )
    {
        if(m_packet_count > 0)
        {
            m_packet_count--;
            if(m_packet_count<= 0){
                //StartRecWave(m_serial_id++,true);
            }
        }

    }
}

void Device::checkOnline()
{
    if(m_timeout > 0)
    {
        m_timeout--;
    }
    if(m_timeout <=0)
    {
        m_online = false;
    }
}



bool Device::checkCanSave(qint64 time,int saveInt)
{
    if(time - m_last_save_ts > saveInt){
        m_last_save_ts = time;
        return true;
    }
    //时间往后走了
    if(time < m_last_save_ts){m_last_save_ts = time;return true;}
    return false;
}
QString Device::name() const
{
    return m_name;
}

void Device::setName(const QString &name)
{
    m_name = name;
}
void Device::DevNotify(QString msg)
{
    msg = QString("device[%1] %2").arg(m_dev_id).arg(msg);
    emit Notify(msg);
    qDebug() << msg;
}
bool Device::onMessage(ProtoMessage &req, ProtoMessage &resp)
{
    m_timeout = MAX_TIMEOUT;
    m_online = true;
    //启动波形回应.


    //实时波形数据文件.
    if(req.head.cmd_id == MSG_START_REC_WAVE)
    {
        //req
        if(!m_start_send)
        {
            //StartRecWave(req.head.sesson_id,m_start_send);
        }
        m_packet_count++;


        resp.data.append((const char*)&req.head.sesson_id,sizeof(quint32));
        return SaveWave(req);
    }
    //注册和心跳包的回应
    else if(req.head.cmd_id == MSG_HEART)
    {
        //req
        sDateTime dt;
        resp.data.append((const char*)&dt,sizeof(sDateTime));
        //qDebug() << "heart beart";

    }
    //读取参数的回应
    else if(req.head.cmd_id == MSG_READ_PARAM)
    {
        //读取的参数的回应包.
        MsgDevicePara para;
        if(req.getData(&para,sizeof(MsgDevicePara)))
        {
            emit ReadParam(this,para);
        }else{
            qDebug() << "getparam failed";
        }

    }
    //写入参数成功的回应
    else if(req.head.cmd_id == MSG_WRITE_PARAM)
    {
        emit WriteParam(this,true);
        //写入参数结果的回应包.
    }
    //标定结果的回应
    else if(req.head.cmd_id == MSG_CALIB)
    {
        SENSOR_CAL_RESULT rst;
        if(req.getData(&rst,sizeof(SENSOR_CAL_RESULT)))
        {
            emit CalibResult(this,rst.chan,rst.index,rst.result);
        }

    }
    //读取实时ad值的回应
    else if(req.head.cmd_id == MSG_RT_AD)
    {
        RT_AD_RESULT rst;
        if(req.getData(&rst,sizeof(RT_AD_RESULT)))
        {
            emit RealTimeResult(this,rst);
        }
    }
    else if(req.head.cmd_id == MSG_RESET)
    {
          qDebug() << "onResetResult----";
            emit ResetResult(this,true);
    }
    return true;

}
QString Device::id() const
{
    return m_dev_id;
}

void Device::setId(const QString &id)
{
    m_dev_id = id;
}

QQueue<SensorData>* Device::GetHistoryData(int chan)
{
    if(!m_channels.contains(chan)){
       return NULL;
    }
    return &m_channels[chan].values;
}
#include "dao.h"


bool Device::WriteValuesBuf(MsgSensorData& msg)
{


    static QTime last;
    static bool start = false;

    if(!start){
        last.start();
        start = true;
    }


    qint32 time = 0;

    Config& cfg = Config::instance();
    //qDebug() << "id=" << QThread::currentThreadId();
    if(cfg.m_use_sys_time){
        time = qint32(QDateTime::currentMSecsSinceEpoch() / 1000);
        if(!checkCanSave(time,Config::instance().m_save_intS)){
            //系统时钟模式还没有达到保存时间，就返回.
            //qDebug() << "time not reach!";
            return true;
        }
    }

    for(int i = 0; i <msg.channels.size();i++)
    {
        //如果这个设备的这个通道已经禁用了。
         if(IsPaused(msg.channels[i].addr)){
             continue;
         }
         //如果数据是传感器未接入，也不发生保存数据.
         if(msg.channels[i].weight == 65535){
             continue;
         }
         DeviceData dd;
         dd.chan = msg.channels[i].addr;
         dd.value = msg.channels[i].weight;
         dd.timestamp = msg.channels[i].time;
         if(Config::instance().m_use_sys_time){
             dd.timestamp = time;
             //判断是否可以存储了

         }
         ddl.push_back(dd);
    }
    //qDebug() << "elapsed" << last.elapsed();
    if(ddl.size() < cfg.m_buf_num &&  last.elapsed() < cfg.m_buf_time){
        //数据已经缓存了80条，或者上次接收的时间超过3s 两个条件都没有满足，
       return true;
    }
    //重新开始计时
    last.restart();

    QSqlError err=DAO::instance().DeviceDataAdd(msg.m_dev_serial,ddl);

    //写入后需要清空数据
    ddl.clear();

    if(err.isValid()){
        qDebug() << "DeviceDataAdd err=" << err.text();
        return false;
    }
    return true;
}
bool Device::WriteValues(MsgSensorData& msg)
{

    DeviceDataList ddl;


    qint32 time = 0;

     Config& cfg = Config::instance();
    //qDebug() << "id=" << QThread::currentThreadId();
    if(cfg.m_use_sys_time){
        time = qint32(QDateTime::currentMSecsSinceEpoch() / 1000);
        if(!checkCanSave(time,Config::instance().m_save_intS)){
            //系统时钟模式还没有达到保存时间，就返回.
            //qDebug() << "time not reach!";
            return true;
        }
    }

    for(int i = 0; i <msg.channels.size();i++)
    {
        //如果这个设备的这个通道已经禁用了。
         if(IsPaused(msg.channels[i].addr)){
             continue;
         }
         if(msg.channels[i].weight == 65535){
             continue;
         }
         DeviceData dd;
         dd.chan = msg.channels[i].addr;
         dd.value = msg.channels[i].weight;
         dd.timestamp = msg.channels[i].time;
         if(cfg.m_use_sys_time){
             dd.timestamp = time;
             //判断是否可以存储了

         }
         ddl.push_back(dd);
    }

    QSqlError err=DAO::instance().DeviceDataAdd(msg.m_dev_serial,ddl);

    if(err.isValid()){
        qDebug() << "DeviceDataAdd err=" << err.text();
        return false;
    }
    return true;

}
bool Device::ProcessWave(int index,QByteArray &data)
{
    MsgSensorData msd;
    msd.m_dev_serial = this->id();
    //qDebug() <<" sensor" << sizeof(SensorData);

    while(data.size() >= sizeof(SensorData)){
        SensorData value = *(SensorData*)data.left(sizeof(SensorData)).data();
        data.remove(0,sizeof(SensorData));

#ifdef CRC_SUPPORT
        quint16 crc = CRC16BigEndian((quint8*)&value,sizeof(SensorData)-2);

        if(crc != value.crc){
            qDebug("CRC ERROR %04x != %04x",crc,value.crc);
            continue;
        }
#endif
        //if(value.weight < 65535){
            WriteValues(value);
            msd.channels.push_back(value);
       // }

    }
    if(Config::instance().m_enable_buffer){
         WriteValuesBuf(msd);
    }else{
         WriteValues(msd);
    }

    //存储数据完成后，再回应.如果存储失败，则不回应数据.
    emit OnSensorData(this,msd);
    return true;
}

bool Device::SaveWave(ProtoMessage &msg)
{
    QByteArray &data  = msg.data;

    WaveDataHead wvh;
    memcpy(&wvh,data.data(),sizeof(WaveDataHead));

    quint8 sample_bits = wvh.samplebits;
    quint8 sample_chan = wvh.nchannel;
    quint32 sample_total = wvh.totalSamples;
    quint32 sample_start = wvh.startSample;
    quint32 sample_num = wvh.nSample;


    int nsize = msg.data.size() - 12;
    //qDebug() << "ssid" << msg.head.sesson_id << "total " << msg.data.size()  << " write " << nsize;
    QByteArray wvData = msg.data.mid(12, nsize);

    return ProcessWave(sample_start, wvData);


}

void Device::Sync()
{
    if(ddl.size() == 0 ) return;
    QSqlError err=DAO::instance().DeviceDataAdd(m_dev_id,ddl);
    qDebug() <<m_dev_id << " sync count=" << ddl.size();
    //写入后需要清空数据
    ddl.clear();

}

void Device::WriteValues(SensorData &data)
{
        qint32 span = Config::instance().m_rt_wave_min*60;

        m_channels[data.addr].values.push_back(data);
        do{
            SensorData& first = m_channels[data.addr].values.front();
            //如果超过20分钟*60秒，也抛弃.最多一秒一条数据.
            if( (data.time - first.time) < span && m_channels[data.addr].values.size() < span){
                //如果当前数据的时间没有大于第一个数据20min种，就说明数据还没有满,跳过,否则
                break;
            }
            m_channels[data.addr].values.pop_front();
        }while(1);

}
//修改某个设备某个通道的状态.
void Device::UpdateState(int chan,bool pause)
{
    if(m_channels.contains(chan)){
        m_channels[chan].config.paused = pause;
    }
}

void Device::UpdateChanConfig(int chan, DeviceChnConfig &cfg)
{
    //qDebug() << "UpdateChanConfig=" << m_dev_id << "chan=" << chan << "paused=" << cfg.paused;
    if(m_channels.contains(chan)){
        m_channels[chan].config = cfg;
    }
}
ISession *Device::sess() const
{
    return m_sess;
}

void Device::setSess(ISession *sess)
{
    m_sess = sess;
}

void Device::setHostPort(QHostAddress host, quint16 port)
{
    //qDebug() << m_dev_id << " host:" << host.toString() << " port="<<port;
    m_host = host;
    m_port = port;
}

bool Device::GetHostAddr(QString &ip)
{

    quint32 nip = m_host.toIPv4Address();
    ip = QString("%1.%2.%3.%4").
            arg((nip>>24)&0xff).
            arg((nip>>16)&0xff).arg((nip>>8)&0xff).arg(nip&0xff);
    //ip = m_host.toString();
    return true;
}






