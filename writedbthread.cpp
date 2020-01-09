#include "writedbthread.h"
#include <QMetaObject>
#include "singletonholder.h"
#include <QDebug>
WriteDBThread &WriteDBThread::instance()
{
    static SingletonHolder<WriteDBThread> sh;
    return *sh.get();
}

WriteDBThread::WriteDBThread()
{
    qRegisterMetaType<DeviceDataList>("DeviceDataList");
}

void WriteDBThread::startThread()
{

    this->moveToThread(&_thread);
    _thread.start();
}

void WriteDBThread::stopThread()
{
    _thread.quit();
    _thread.wait();
}

void WriteDBThread::WriteData(QString serialNo, DeviceDataList data)
{
    QMetaObject::invokeMethod(this,"_WriteData",
                              Qt :: QueuedConnection,
                              Q_ARG(QString, serialNo),
                              Q_ARG(DeviceDataList, data)
                              );
}

void WriteDBThread::DeviceChanDataRemove(QString serialNo, int chan)
{
    QMetaObject::invokeMethod(this,"_DeviceChanDataRemove",
                              Qt :: QueuedConnection,
                              Q_ARG(QString, serialNo),
                              Q_ARG(int, chan)
                              );
}

void WriteDBThread::DeviceDataTblRemove(QString serialNo)
{
    QMetaObject::invokeMethod(this,"_DeviceDataTblRemove",
                              Qt :: QueuedConnection,
                              Q_ARG(QString, serialNo)
                              );
}

void WriteDBThread::DeviceDataRemove(QString serialNo)
{
    QMetaObject::invokeMethod(this,"_DeviceDataRemove",
                              Qt :: QueuedConnection,
                              Q_ARG(QString, serialNo)
                              );
}

void WriteDBThread::DeviceDataRemove(QString serialNo, qint64 from, qint64 to)
{
    QMetaObject::invokeMethod(this,"_DeviceDataRemove",
                              Qt :: QueuedConnection,
                              Q_ARG(QString, serialNo),
                              Q_ARG(qint64, from),
                              Q_ARG(qint64, to)
                              );
}
//线程中的函数
void WriteDBThread::_WriteData(QString serialNo, DeviceDataList data)
{
    QSqlError er  = DAO::instance().DeviceDataAdd(serialNo,data);
    if(er.isValid()){
        qDebug() << er.text();
    }else{
        //qDebug() << "write ok";
    }
}

void WriteDBThread::_DeviceChanDataRemove(QString serialNo, int chan)
{
    QSqlError er  = DAO::instance().DeviceChanDataRemove(serialNo,chan);
}

void WriteDBThread::_DeviceDataTblRemove(QString serialNo)
{
    QSqlError er  = DAO::instance().DeviceDataTblRemove(serialNo);
}

void WriteDBThread::_DeviceDataRemove(QString serialNo)
{
    QSqlError er  = DAO::instance().DeviceDataRemove(serialNo);
}

void WriteDBThread::_DeviceDataRemove(QString serialNo, qint64 from, qint64 to)
{
    QSqlError er  = DAO::instance().DeviceDataRemove(serialNo,from,to);
}
