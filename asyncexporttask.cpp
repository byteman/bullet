﻿#include "asyncexporttask.h"
#include "dao.h"
#include "qtcsv/writer.h"
#include "qtcsv/stringdata.h"
#include <QFuture>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
AsyncExportTask::AsyncExportTask(QString serialNo,
                                 QString name,
                                 QVector<int> chans,
                                 qint64 from,
                                 qint64 to,
                                 QString dest):
                                _serialNo(serialNo),
                                _name(name),
                                _chans(chans),
                                _from(from),
                                _to(to),
                                _dest(dest),
                                watcher(NULL)
{
    _uuid=QString("%1-%2").arg(serialNo).arg(QDateTime::currentMSecsSinceEpoch());
}

AsyncExportTask::~AsyncExportTask()
{
    if(watcher!=NULL){
        delete watcher;
    }
}

QString AsyncExportTask::uuid()
{
    return _uuid;
}
#if 0
bool AsyncExportTask::_run()
{

    MultiDeviceDataMap ddm;
    for(int i = 0; i < _chans.size(); i++)
    {
        DeviceDataList dll;
        ddm[_chans[i]] = dll;
    }
    emit onProgress(this,1);
    QSqlError err = DAO::instance().DeviceDataQuery(_serialNo,_from,_to, ddm);
    emit onProgress(this,10);

    QMapIterator<int,DeviceDataList> i(ddm);

    QtCSV::StringData strData;
    QStringList strHeader;
    int first = -1;
    strHeader << QStringLiteral("time");

    int count = 0;
    while(i.hasNext()){
        i.next();
        if(first == -1){
            first = i.key();
        }

        strHeader << QString("%1").arg(i.key());
        count++;
    }
    if(first == -1) return false;
    int num = ddm[first].size();

    for(int i = 0; i <num ;i++)
    {
        QStringList strList;
        for(int j = 0; j < _chans.size(); j++)
        {
            int addr = _chans[j];
            DeviceDataList& ddl = ddm[addr];
            if(i >= ddl.size()) continue;

            if(j == 0){

                strList << QString("%1").arg(QDateTime::fromMSecsSinceEpoch(ddl[i].timestamp*1000).toString("yyyy-MM-dd hh:mm:ss"));
            }
            strList << QString("%1").arg(ddl[i].value);
        }
        emit onProgress(this,10+ 80*i/num);
        strData.addRow(strList);
    }
    QString filePath = _dest;
    /**
     * @brief QtCSV::Writer::write
     *  static bool write(const QString& filePath,
                        const AbstractData& data,
                        const QString& separator = QString(","),
                        const QString& textDelimiter = QString("\""),
                        const WriteMode& mode = REWRITE,
                        const QStringList& header = QStringList(),
                        const QStringList& footer = QStringList(),
                        QTextCodec* codec = QTextCodec::codecForName("UTF-8"));

     */
    QtCSV::Writer::write(filePath, strData,QString(","),QString("\""),QtCSV::Writer::REWRITE,strHeader);
    emit onProgress(this,100);
    emit onSucc(this,"succ");
    return !err.isValid();
}
#else
bool AsyncExportTask::writeFile(int chan,DeviceDataList ddl,QString filePath)
{
    QtCSV::StringData strData;
    QStringList strHeader;
    strHeader << QStringLiteral("time");
    strHeader << QString("%1").arg(chan);

    for(int j = 0; j < ddl.size(); j++)
    {
         QStringList strList;
        strList << QString("%1").arg(QDateTime::fromMSecsSinceEpoch(ddl[j].timestamp*1000).toString("yyyy-MM-dd hh:mm:ss"));
        strList << QString("%1").arg(ddl[j].value);
        strData.addRow(strList);
    }
    //emit onProgress(this,10+ 80*i/num);



    /**
     * @brief QtCSV::Writer::write
     *  static bool write(const QString& filePath,
                        const AbstractData& data,
                        const QString& separator = QString(","),
                        const QString& textDelimiter = QString("\""),
                        const WriteMode& mode = REWRITE,
                        const QStringList& header = QStringList(),
                        const QStringList& footer = QStringList(),
                        QTextCodec* codec = QTextCodec::codecForName("UTF-8"));

     */
    QtCSV::Writer::write(filePath, strData,QString(","),QString("\""),QtCSV::Writer::REWRITE,strHeader);
    return true;
}
bool AsyncExportTask::_run()
{

    MultiDeviceDataMap ddm;
    for(int i = 0; i < _chans.size(); i++)
    {
        DeviceDataList dll;
        ddm[_chans[i]] = dll;
    }
    emit onProgress(this,1);
    QSqlError err = DAO::instance().DeviceDataQuery(_serialNo,_from,_to, ddm);
    emit onProgress(this,10);

    QMapIterator<int,DeviceDataList> i(ddm);


    int count = 0;
    while(i.hasNext()){
        i.next();

        writeFile(i.key(),i.value(),QString("%1/%2_%3.csv").
                  arg(_dest).arg(_name).arg(i.key()));
        emit onProgress(this,10+ 80*count/ddm.size());
        count++;
    }


    emit onProgress(this,100);
    emit onSucc(this,"succ");
    return !err.isValid();
}
#endif
bool AsyncExportTask::start()
{
    if(watcher==NULL){
        watcher = new QFutureWatcher<bool>(this);
        connect(watcher, SIGNAL(finished()), this, SLOT(handleLoadWaveFinished()));
    }


    const QFuture<bool> future = QtConcurrent::run(this,&AsyncExportTask::_run);
    watcher->setFuture(future);
    return true;
}
