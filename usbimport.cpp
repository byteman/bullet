#include "usbimport.h"
#include "singletonholder.h"
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include "utils.h"
#include "qtcsv/reader.h"
#include "dao.h"
UsbImport::UsbImport():
    watcher(NULL)
{

}

UsbImport* UsbImport::instance()
{
    static SingletonHolder<UsbImport> sh;
    return sh.get();
}
//分析并写入一个文件的数据.
bool UsbImport::parseFile(QString file)
{
    QList<QStringList> readData = QtCSV::Reader::readToList(file);
    DeviceDataList ddl;
    //分析多行.
    for ( int i = 0; i < readData.size(); ++i )
    {
        DeviceData dd;
        if(readData.at(i).length() >= 13){
            //QStringList& l = readData.at(i);

            //分析一行数据
            for(int j = 0; j < readData.at(i).length();j++)
            {
                if(j == 0){
                    QString s = readData.at(i).at(0);
                    dd.timestamp = QDateTime::fromString(s,"yyyy-MM-dd hh:mm:ss").toMSecsSinceEpoch() /1000;

                    continue;
                }else{
                     dd.chan = j;
                     dd.value = readData.at(i).at(j).toInt();
                     if(dd.value >= 65535){
                         //跳过未连接的传感器数据.
                         continue;
                     }
                }
                ddl.push_back(dd);
            }

        }

    }
    if(ddl.size() > 0)
    {
        //在线程中开了一个新的连接来写入数据.没有使用数据的接收写入连接
        //qDebug() << "add size=" << ddl.size();

        QSqlError err = DAO::instance().DeviceDataAdd2(m_serial_no,ddl,m_total);
        if(err.isValid()){
            qDebug() << "DeviceDataAdd err=" << err.text();
        }else{
            qDebug() << "total=" << m_total;
        }
    }

    return true;
}
//线程函数.
bool UsbImport::_run()
{

    QFileInfoList list ;//= utils::ListDirFiles(m_dir,"*.csv");
    utils::FindFiles(m_dir,"*.csv",list);//递归遍历所有目录的csv文件.
    m_total = 0;
    qDebug() << "serial=" << m_serial_no << "file num=" << list.size() << " dir=" << m_dir;
    for(int i = 0; i < list.size(); i++)
    {
        QString file = list[i].absoluteFilePath();
        //qDebug() << "parse csv=" << file;
        emit onProgress((i+1)*100/list.size());
        parseFile(file);
    }
    emit onSucc(m_total);
    return true;
}
bool UsbImport::start(QString serial,QString dir)
{
    if(watcher==NULL){
        watcher = new QFutureWatcher<bool>(this);
        //connect(watcher, SIGNAL(finished()), this, SLOT(handleImportFinished()));
    }
    m_dir = dir;
    m_serial_no = serial;
    const QFuture<bool> future = QtConcurrent::run(this,&UsbImport::_run);
    watcher->setFuture(future);
    return true;
}


