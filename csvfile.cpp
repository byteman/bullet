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
    m_file->open(QIODevice::ReadWrite|QIODevice::Append);
    for(int i=0; i < 8; i++)
    {
        m_values.push_back("");
    }
}

CSVFile::CSVFile():
    m_index(0)
{
//    QFile file(":/csv.txt");
//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//    {
//        qDebug() << "open failed!";
//    }
//    m_header = file.readAll();
//    m_header+="\r\n";

//    for(int i=0; i < 8; i++)
//    {
//        m_values.push_back("");
//    }

}
bool CSVFile::writeHeader(QString name)
{
    m_file->setFileName(name);
    m_file->open(QIODevice::WriteOnly|QIODevice::Append);
    m_file->write(m_header);
    m_file->flush();
    return true;
}
void CSVFile::update()
{
    m_file->write(m_output);
    m_output.clear();
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
           int value = line[j].toInt(&ok);
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

