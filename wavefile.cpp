#include "wavefile.h"

WaveFile::WaveFile():
    m_file(NULL)
{

}

WaveFile::WaveFile(QString file)
{
    m_file = new QFile(file);
    m_file->open(QIODevice::ReadWrite|QIODevice::Append);
}

bool WaveFile::load(QString file, ChannelSample &samples)
{
    if(m_file != NULL)
    {
        delete m_file;
        m_file = NULL;
    }
    m_file = new QFile(file);
    return m_file->open(QIODevice::ReadWrite);

}
// sample1: 1->2bytes 2->2bytes 3->bytes
// sample2: 1->2bytes 2->2bytes 3->bytes
// sample3: 1->2bytes 2->2bytes 3->bytes
qint64 WaveFile::write(QByteArray &data)
{
    if(m_file==NULL) return -1;
    return m_file->write(data);
}

void WaveFile::close()
{
    if(m_file==NULL)return;
    m_file->close();
}

