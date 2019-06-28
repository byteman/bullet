#include "syncfile.h"
#include "device.h"
SyncFile::SyncFile():
    m_cur(0),
    m_total(0),
    m_device(NULL),
    m_timeout(3),
    m_cancel_timeout(0),
    m_quit(true),
    m_file(NULL)
{
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(timeout()));
}
void SyncFile::OpenFile(QString fname)
{
    if(m_file==NULL){

        qDebug() << "create file " << fname;
        m_file = new CSVFile(fname);
    }
}
void SyncFile::CloseFile()
{
    if(m_file!=NULL)
    {
        m_file->close();
        delete m_file;
        m_file = NULL;
    }
}
bool SyncFile::StartSync(Device *dev, QString file,int total)
{
    StopSync();
    m_device = dev;
    m_file_name = file;
    m_total = total;
    m_timeout = 0;
    m_cancel_timeout = 0;
    m_cur = 0;
    m_quit = false;
    OpenFile(file);
    m_want = total >5?5:total;

    m_timer.start(1000);

    return true;
}
void SyncFile::SaveWave(ProtoMessage &msg)
{
    QByteArray &data  = msg.data;
    if(m_device==NULL)return;
    WaveDataHead wvh;
    memcpy(&wvh,data.data(),sizeof(WaveDataHead));

    quint8 sample_bits = wvh.samplebits;
    quint8 sample_chan = wvh.nchannel;
    quint32 sample_total = wvh.totalSamples;
    quint32 sample_start = wvh.startSample;
    quint32 sample_num = wvh.nSample;


    m_cancel_timeout = 0;
    int nsize = msg.data.size() - 12;
    //qDebug() << "ssid" << msg.head.sesson_id << "total " << msg.data.size()  << " write " << nsize;
    QByteArray wvData = msg.data.mid(12, nsize);


    if(m_file!=NULL)
    {
        m_file->write(wvData);
    }
    qDebug() << "cur= " << sample_start <<" total=" << sample_total;
    if(sample_start+sample_num >= sample_total)
    {
        m_device->sendProgress(sample_total, sample_total);
        StopSync();
    }
    else
    {
        m_want = (sample_total - sample_start)>=5?5:sample_total - sample_start;

        readPacket(m_cur + m_want, m_want);
        m_cur += m_want;
        m_device->sendProgress(sample_start, sample_total);
    }



    //ProcessWave(sample_start, wvData);


}
//接收到波形文件的数据，进行处理
bool SyncFile::onMessage(ProtoMessage &req, ProtoMessage &resp)
{
    SaveWave(req);
    return true;
}

bool SyncFile::StopSync()
{
    CloseFile();
    m_quit = true;
    m_timer.stop();
    return true;
}

bool SyncFile::isStarted()
{
    return !m_quit;
}

float SyncFile::progress()
{
    return 0;
}
void SyncFile::readPacket(int index,int count)
{
    MsgReadFile  msg;

    msg.count = count;
    msg.index = index;
    m_timeout = 0;
    qDebug() << "read " << index << " count=" << count;
	QByteArray buf = msg.toBuffer();

    m_device->WriteCmd(MSG_WAVE_DATA,buf);
}
//1秒超时，重发当前波形.
void SyncFile::timeout()
{
    if(m_quit){
        this->StopSync();
    }
    if(++m_timeout % 3 == 0 ){
        qDebug() << "timeout retry";
        readPacket(m_cur,m_want);
        if(m_cancel_timeout++ > 2){
            qDebug() << "timeout 10 ,canceled";
            StopSync();
        }

    }
}
