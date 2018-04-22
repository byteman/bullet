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
    m_file(NULL),
    m_start_send(true)
{
    connect(&timer,SIGNAL(timeout()),this,SLOT(timeout()));
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
    return SendData(data);

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

qint64 Device::SendStartWave(quint16 sess_id,bool start)
{
    ProtoMessage msg;
    msg.head.cmd_id = MSG_START_WAVE;
    msg.head.device_id = m_dev_id;
    msg.head.serial_id = m_serial_id++;
    msg.head.sesson_id = sess_id;
    msg.is_ack = false;
    QByteArray data;
    msg.toByteArray(data);
    if(start)
        OpenFile();
    else
        CloseFile();
    return SendData(data);

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
                StartRecWave(m_serial_id++,true);
            }
        }

    }
}
void Device::OpenFile()
{
    if(m_file==NULL){
        QString fname = GetFileName();
        qDebug() << "create file " << fname;
        m_file = new CSVFile(fname);
    }
}
void Device::CloseFile()
{
    if(m_file!=NULL)
    {
        m_file->close();
        delete m_file;
        m_file = NULL;
    }
}
qint64 Device::StartRecWave(quint16 sess_id,bool start)
{
    ProtoMessage msg;
    if(start)
    {
        msg.head.cmd_id = MSG_START_REC_WAVE;


        m_packet_count = 3;
        timer.start(1000);
    }
    else
    {
        msg.head.cmd_id = MSG_STOP_REC_WAVE;
        CloseFile();
        timer.stop();

    }
    m_start_send = start;
    msg.head.device_id = m_dev_id;
    msg.head.serial_id = m_serial_id++;
    msg.head.sesson_id = sess_id;
    msg.is_ack = false;
    QByteArray data;
    msg.toByteArray(data);
    return SendData(data);

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
        if(!m_start_send)
        {
            StartRecWave(req.head.sesson_id,m_start_send);
        }
        m_packet_count++;
        SaveWave(req);
    }
    //注册和心跳包
    else if(req.head.cmd_id == MSG_HEART)
    {
        //req
        sDateTime dt;
        resp.data.append((const char*)&dt,sizeof(sDateTime));
        //qDebug() << "heart beart";

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
    QString file = QString("%1/%2.csv").arg(dir).arg(dt);
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
//           quint8 v1 = (data[i*8+j*2+1]<<8);
//           quint8 v2 = data[i*8+j*2+0];
//           qint16 value =  (v1<<8) + v2;

           quint16 value = 0;//(data[i*8+j*2+1]<<8) + data[i*8+j*2+0];
           memcpy(&value, data.data() + i*16 + j*2,2);


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


    int nsize = msg.data.size() - 12;
    //qDebug() << "ssid" << msg.head.sesson_id << "total " << msg.data.size()  << " write " << nsize;
    QByteArray wvData = msg.data.mid(12, nsize);

    if(m_file!=NULL)
    {
        m_file->write(wvData);
        if( (sample_start + sample_num) == sample_total)
        {
            emit Progress(this,"同步完成");
            CloseFile();
        }
    }

    sendProgress(sample_start, sample_total);

    ProcessWave(sample_start, wvData);


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

    CSVFile csv;
    return csv.LoadWaveFile(filename,wvd);

#if 0
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
#endif
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

void Device::setHostPort(QHostAddress host, quint16 port)
{
    //qDebug() << m_dev_id << " host:" << host.toString() << " port="<<port;
    m_host = host;
    m_port = port;
}






