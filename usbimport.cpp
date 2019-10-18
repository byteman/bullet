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
                         continue;
                     }
                }
                ddl.push_back(dd);
            }

        }

    }
    QSqlError err = DAO::instance().DeviceDataAdd(m_serial_no,ddl);
    if(err.isValid()){
        qDebug() << "DeviceDataAdd err=" << err.text();
    }
    return true;
}
//线程函数.
bool UsbImport::_run()
{
    QFileInfoList list = utils::ListDirFiles(m_dir,"*.csv");
    qDebug() << "serial=" << m_serial_no << " dir=" << m_dir;
    for(int i = 0; i < list.size(); i++)
    {
        QString file = list[i].absoluteFilePath();
        qDebug() << "parse csv=" << file;
        parseFile(file);
    }
    emit onSucc();
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


