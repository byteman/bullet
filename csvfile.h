#ifndef CSVFILE_H
#define CSVFILE_H

#include <QObject>
#include <QFile>
#include <QMap>
#include <QDateTime>
#include "protomessage.h"
class CSVFile:public QObject
{
    Q_OBJECT
public:
    CSVFile();
    CSVFile(QString file);
    //存储采集一圈的数据.
    bool Append(QMap<int,QString> &values,int maxAddr);
    bool Append(int addr,QString value,int maxAddr,bool flush=false);
    bool SaveFile(QString name);
    bool LoadWaveFile(QString file, MsgWaveData &wvd);
    qint64 write(QByteArray &data);
    bool Write(MsgWaveData& data);
    void close();
    void update();
    void WriteHeader(QString header);
private:
    int m_index; //存储序号.
    QFile *m_file;
    bool m_writeHeader;
    QString CreateDir();
    QString GetFileName();
    QStringList m_values;
    QByteArray  m_output;
    QByteArray  m_header;
    QMap<int,QString >    m_csv_values;

};

#endif // CSVFILE_H
