#include "csvfile.h"
#include <QDir>
#include <QTextStream>
#include "utils.h"
#include <QDebug>
QString CSVFile::CreateDir()
{

    QString target_dir=QString("%1/csv/").arg(QDir::currentPath());
    QDir dir(target_dir);
    if(!dir.exists())
    {
        dir.mkpath(target_dir);
    }
    return target_dir;
}
QString CSVFile::GetFileName()
{
    QString dt  = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    QString dir = CreateDir();
    QString file = QString("%1/%2.csv").arg(dir).arg(dt);
    return file;
}
CSVFile::CSVFile(QString file):
    m_index(0)
{
    m_file = new QFile(file);
    //m_file->open(QIODevice::ReadWrite|QIODevice::Append);
    m_file->open(QIODevice::ReadWrite|QIODevice::Truncate);
    for(int i=0; i < 8; i++)
    {
        m_values.push_back("");
    }
}

CSVFile::CSVFile():
    m_index(0)
{


}

void CSVFile::update()
{
    m_file->write(m_output);
    m_output.clear();
}

void CSVFile::WriteHeader(QString header)
{
    //QString out = header.join(',');
    //m_output.append(out);
    //m_output.append("\r\n");
    QFile f("csv.txt");
    f.open(QIODevice::ReadOnly);

    m_file->write(f.readAll());

}
qint64 CSVFile::write(QByteArray &data)
{
    int nr = data.size() / 16;
    for(int i= 0; i < nr; i++)
    {
        qint16 value = 0;
        //m_values[0] = QString("%1").arg(m_index++);
        //m_values[1] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss");
        for(int j = 0; j < 8;j++)
        {
            memcpy(&value,data.data()+i*16+j*2,2);
            m_values[j] = QString("%1").arg(value);
        }
        QString out = m_values.join(',');
        m_output.append(out);
        m_output.append("\r\n");

    }


    if( (m_index % 100) == 0)
    {
        update();
    }
    return 0;
}

bool CSVFile::Write(MsgWaveData &data)
{
    for(int i = 0 ; i < 6; i++)
    {
        m_values[i] = QString("%1").arg(data.channels[i][0]);
    }
    QString out = m_values.join(',');
    m_output.append(out);
    m_output.append("\r\n");
    update();
    return true;
}

void CSVFile::close()
{
    update();
    m_file->close();
}



bool CSVFile::LoadWaveFile(QString file, MsgWaveData &wvd)
{

    QFile filein(file);

    if(!filein.exists()) return false;


    if (!filein.open(QIODevice::ReadOnly)) return false;

    qDebug() << "name =" << file << "file size=" << filein.size();
    QTextStream stream(&filein);
    stream.readLine();
    ChannelData cda;

    wvd.channels.reserve(8);
    wvd.channels.fill(cda,8);
    while (!stream.atEnd())
    {
        QStringList line = stream.readLine().split(",");
        if(line.size() != 8)
        {
            continue;
        }

        for(int j = 0; j < 8; j++)
        {

           bool ok = false;
           //QString item = line[j];
           double value = line[j].toDouble(&ok);
           if(!ok){
               continue;
           }
           wvd.channels[j].push_back( value );
            //cda[j]
        }




    }
    filein.close();
    return true;
}

