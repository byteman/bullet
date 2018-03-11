#include "device.h"
#include <QDir>
#include <QDebug>
#include <QDateTime>
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
    m_file(NULL)
{

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
    msg.head.device_id = m_dev_id;
    msg.head.serial_id = m_serial_id++;
    msg.head.sesson_id = m_sess_id++;
    msg.is_ack = false;
    msg.data.push_back(buf);
    QByteArray data;
    msg.toByteArray(data);
    if(m_sess)
    {
        return m_sess->send(data);
    }
    return 0;
}

bool Device::ListFiles()
{
    QByteArray data;
    WriteCmd(MSG_ENUM_FILES,data);
    return true;
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
void Device::WriteParam(MsgDevicePara &para)
{

   QByteArray data;
   para.toByteArray(data);
   WriteCmd(MSG_WRITE_PARAM,data);
}

qint64 Device::SendStartWave(quint16 sess_id)
{
    ProtoMessage msg;
    msg.head.cmd_id = MSG_START_WAVE;
    msg.head.device_id = m_dev_id;
    msg.head.serial_id = m_serial_id++;
    msg.head.sesson_id = sess_id;
    msg.is_ack = false;
    QByteArray data;
    msg.toByteArray(data);
    if(m_sess)
    {
        return m_sess->send(data);
    }
    return 0;
}

qint64 Device::StartRecWave(quint16 sess_id,bool start)
{
    ProtoMessage msg;
    if(start)
        msg.head.cmd_id = MSG_START_REC_WAVE;
    else
        msg.head.cmd_id = MSG_STOP_REC_WAVE;
    msg.head.device_id = m_dev_id;
    msg.head.serial_id = m_serial_id++;
    msg.head.sesson_id = sess_id;
    msg.is_ack = false;
    QByteArray data;
    msg.toByteArray(data);
    if(m_sess)
    {
        return m_sess->send(data);
    }
    return 0;
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
void Device::onMessage(ProtoMessage &req, ProtoMessage &resp)
{
    m_timeout = MAX_TIMEOUT;
    m_online = true;
    //启动波形回应.
    if(req.head.cmd_id == MSG_START_WAVE)
    {
        //ack
        DevNotify("start wave");

    }
    //波形数据文件.
    else if(req.head.cmd_id == MSG_WAVE_DATA)
    {
        //req
        SaveWave(req);
    }
    //注册和心跳包
    else if(req.head.cmd_id == MSG_HEART)
    {
        //req
        sDateTime dt;
        resp.data.append((const char*)&dt,sizeof(sDateTime));
        qDebug() << "heart beart";

    }
    else if(req.head.cmd_id == MSG_READ_PARAM)
    {
        //读取的参数的回应包.
        MsgDevicePara para;
        if(req.getData(&para,sizeof(MsgDevicePara)))
        {
            emit ReadParam(this,para);
        }

    }
    else if(req.head.cmd_id == MSG_WRITE_PARAM)
    {
        emit WriteParam(this,true);
        //写入参数结果的回应包.
    }
    else if(req.head.cmd_id == MSG_ENUM_FILES)
    {
        QByteArray &data = req.data;
        int num = (data[1]<<8) + data[0];
        QByteArray attr = data.mid(2,num);
        int length = data.size() - 2 - num;
        if(length > 0)
        {
            QString files = data.right(length);
            QStringList filelist = files.split(",");
            if( filelist.size()!=num)
            {
                emit Notify("file num error");
                return;
            }
            MsgFileList flist;
            for(int i = 0; i < num; i++)
            {
                MsgFileInfo finfo;

                finfo.attr = attr[i];
                finfo.name = filelist[i];
                flist.push_back(finfo);
            }
            emit EnumFiles(this,flist);

        }



    }
}
quint32 Device::id() const
{
    return m_dev_id;
}

void Device::setId(const quint32 &id)
{
    m_dev_id = id;
}

QString Device::CreateDir()
{

    QString target_dir=QString("%1/wave/%2/%3").arg(QDir::currentPath()).arg(m_station).arg(m_dev_id);
    QDir dir(target_dir);
    if(!dir.exists())
    {
        dir.mkpath(target_dir);
    }
    return target_dir;
}
QString Device::GetFileName()
{
    QString dt  = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    QString dir = CreateDir();
    QString file = QString("%1/%2.wave").arg(dir).arg(dt);
    return file;
}
void Device::sendProgress(int sample_start, int sample_total)
{
    emit Progress(this,QString("%1").arg(sample_start*100/sample_total)+"%");
}
void Device::ProcessWave(int index,QByteArray &data)
{

    //总长度 / 1个样本的长度(8通道*每个通道2字节) = 样本数
    int sample_nr = data.size() / (8*2);

    ChannelData cda;
    MsgWaveData wvd;
    wvd.channels.reserve(8);
    wvd.channels.fill(cda,8);
    if(index == 0)
    {
        wvd.m_first = true;
    }
    for(int i = 0 ; i < sample_nr; i++)
    {
        for(int j = 0; j < 8; j++)
        {
           quint16 value = (data[i*8+j*2+1]<<8) + data[i*8+j*2+0];
           wvd.channels[j].push_back( value );
            //cda[j]
        }
    }
    emit showWave(this,wvd);

}
void Device::SaveWave(ProtoMessage &msg)
{
    QByteArray &data  = msg.data;

    WaveDataHead wvh;
    memcpy(&wvh,data.data(),sizeof(WaveDataHead));

    quint8 sample_bits = wvh.samplebits;
    quint8 sample_chan = wvh.nchannel;
    quint32 sample_total = wvh.totalSamples;
    quint32 sample_start = wvh.startSample;
    quint32 sample_num = wvh.nSample;

    DevNotify(QString("total:%1,start=%2,num=%3").arg(sample_total).arg(sample_start).arg(sample_num));

#if 1
     qDebug() << "ssid" << msg.head.sesson_id << " cur_id" << m_sess_id;
    if(msg.head.sesson_id != m_sess_id)
    {
        m_sess_id = msg.head.sesson_id ;
        if(m_file!=NULL)
        {
            delete m_file;
            m_file = NULL;
        }
        QString fname = GetFileName();
        qDebug() << "create file " << fname;
        m_file = new WaveFile(fname);

    }
    if(m_file!=NULL)
    {
        int nsize = msg.data.size() - 12;
        qDebug() << "ssid" << msg.head.sesson_id << "total " << msg.data.size()  << " write " << nsize;
        QByteArray data = msg.data.mid(12, nsize);
        m_file->write(data);
        ProcessWave(sample_start, data);

    }
    if( (sample_start + sample_num) == sample_total)
    {
        emit Progress(this,"同步完成");
        m_file->close();

    }
    else{
        sendProgress(sample_start, sample_total);
    }

#else
    int nsize = msg.data.size() - 12;

    ProcessWave(msg.data.mid(12, nsize));
#endif

}
QString Device::station() const
{
    return m_station;
}

void Device::setStation(const QString &station)
{
    m_station = station;
}
bool Device::LoadWaveFile(QString file, MsgWaveData &wvd)
{
    QString filename = CreateDir() + "/" + file;
    QFile filein(filename);
    if(!filein.exists()) return false;
    filein.open(QIODevice::ReadOnly);
    QByteArray data = filein.readAll();
    //总长度 / 1个样本的长度(8通道*每个通道2字节) = 样本数
    int sample_nr = data.size() / (8*2);

    ChannelData cda;

    wvd.channels.reserve(8);
    wvd.channels.fill(cda,8);
    for(int i = 0 ; i < sample_nr; i++)
    {


        for(int j = 0; j < 8; j++)
        {

           quint16 value = 0;//(data[i*8+j*2+1]<<8) + data[i*8+j*2+0];
           memcpy(&value, data.data() + i*16 + j*2,2);

           wvd.channels[j].push_back( value );
            //cda[j]
        }
    }
    return true;

}
void Device::listWaveFiles(QStringList &files)
{
    QString dir = CreateDir();
    QDir wvDir(dir);
    files = wvDir.entryList(QDir::Files);
}
ISession *Device::sess() const
{
    return m_sess;
}

void Device::setSess(ISession *sess)
{
    m_sess = sess;
}






