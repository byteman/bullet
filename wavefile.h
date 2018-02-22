#ifndef WAVEFILE_H
#define WAVEFILE_H

#include <QFile>
#include <QVector>
class ChannelSample:public QVector<qint32>
{

};




class WaveFile
{
public:
    WaveFile();
    WaveFile(QString file);
    bool load(QString file,ChannelSample& samples);
    qint64 write(QByteArray& data);
    void close();
private:
    QFile *m_file;
};

#endif // WAVEFILE_H
