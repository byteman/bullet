#include "device.h"
#include <QDir>
#include <QDebug>
#include <QDateTime>
#define MAX_TIMEOUT 10
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
        DevNotify("start wave");

    }
    //波形数据文件.
    else if(req.head.cmd_id == MSG_WAVE_DATA)
    {
        SaveWave(req);
    }
    //注册和心跳包
    else if(req.head.cmd_id == MSG_HEART)
    {
        qDebug() << "heart beart";
    }
    else if(req.head.cmd_id == MSG_READ_PARAM)
    {

    }
    else if(req.head.cmd_id == MSG_WRITE_PARAM)
    {

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
        m_file->write(msg.data.mid(12, nsize));
    }
    if( (sample_start + sample_num) == sample_total)
    {
        m_file->close();
    }

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
           quint16 value = (data[i*8+j*2+1]<<8) + data[i*8+j*2+0];
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






