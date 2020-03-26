#include "asyncexporttask.h"
#include "dao.h"
#include "qtcsv/writer.h"
#include "qtcsv/stringdata.h"
#include <QFuture>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <QThreadPool>
#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
#include "utils.h"
#include "xlsx/statemanager.h"
AsyncExportTask::AsyncExportTask(FileFormat format,QString serialNo,
                                 QString name,
                                 QString cell,
                                 QVector<int> chans,
                                 qint64 from,
                                 qint64 to,
                                 QString dest):
                                _serialNo(serialNo),
                                _name(name),
                                _cell(cell),
                                _chans(chans),
                                _from(from),
                                _to(to),
                                _dest(dest),
                                _fileFormat(format)
{
    _uuid=QString("%1-%2").arg(serialNo).arg(QDateTime::currentMSecsSinceEpoch());
}

AsyncExportTask::~AsyncExportTask()
{

}

QString AsyncExportTask::uuid()
{
    return _uuid;
}

bool AsyncExportTask::writeXlsxFile(int chan,DeviceDataList ddl,QString filePath)
{
    qDebug() << "chan" << chan << "ddl size=" << ddl.size() << QThread::currentThreadId();

    QXlsx::Document xlsxW;
    xlsxW.setColumnWidth(1,25);
    xlsxW.write("A1", "time"); // write "Hello Qt!" to cell(A,1). it's shared string.
    xlsxW.write("B1","value");
    for(int j = 0; j < ddl.size(); j++)
    {    
        xlsxW.write(j+2,1,utils::gettm(ddl[j].timestamp*1000));
        //xlsxW.write(j+2,1,QString("%1").arg(QDateTime::fromMSecsSinceEpoch(ddl[j].timestamp*1000).toString("yyyy-MM-dd hh:mm:ss")));
        xlsxW.write(j+2,2,ddl[j].value);
    }

    xlsxW.saveAs(filePath); // save the document as 'Test.xlsx'

    return true;
}

bool AsyncExportTask::writeCsvFile(int chan,DeviceDataList ddl,QString filePath)
{
   // QTime tm;
   // tm.start();

    QtCSV::StringData strData;
    QStringList strHeader;
    strHeader << QStringLiteral("time");
    strHeader << "value";

    for(int j = 0; j < ddl.size(); j++)
    {
         QStringList strList;

//        strList << QString("%1").arg(QDateTime::fromMSecsSinceEpoch(ddl[j].timestamp*1000).toString("yyyy-MM-dd hh:mm:ss"));
         //这个的速度更快
         strList << utils::gettm(ddl[j].timestamp*1000);
         strList << QString("%1").arg(ddl[j].value);
        strData.addRow(strList);
    }
    //qDebug() << QThread::currentThread() << "write csv" << tm.elapsed() << "ms";


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

    //tm.restart();

    return QtCSV::Writer::write(filePath, strData,QString(","),QString("\""),QtCSV::Writer::REWRITE,strHeader);


}

bool AsyncExportTask::writeFile(int chan,
                                QString filePath,
                                int format)
{
    bool result = false;
    DeviceDataList ddl;
    //查询设备和对应的通道数据
    QSqlError err = DAO::instance().DeviceDataQueryByConn(_serialNo,chan,_from,_to,ddl);
    if(err.isValid())
    {
        qDebug() << "DeviceDataQueryByConn err" << err.text();
        emit onFail(this,err.text() );
    }
    //emit onFail(this,"can not find name");

    emit onProgress(this,++_prog*100 / _total );
    //写入数据.
    //qDebug() << _serialNo << _from << _to  << chan << "size="<<ddl.size();

    if(ddl.size() > 0){
        if(format == FF_XLSX){
            //多线程写xlsx会崩溃.
            //_mutex.lock();
            //result= writeXlsxFile(chan,ddl,filePath);
            //_mutex.unlock();
            ddv.push_back(ddl);

        }else{
            result= writeCsvFile(chan,ddl,filePath);
            if(!result){
                 emit onFail(this,QString("from %1 to %2 chan=%3 size=%4").arg(_from).arg(_to).arg(chan).arg(ddl.size()));
                 emit onFail(this,QString("write to file %1 failed").arg(filePath));
            }
            emit onProgress(this,++_prog*100 / _total );
        }
    }


    return result;
}
bool AsyncExportTask::writeCsv()
{
    QVector<QFuture<bool> > results;
    QThreadPool pool;
    pool.setMaxThreadCount(4);
    _total = _chans.size() * 2;
    _prog  = 0;
    emit onProgress(this,1);
    ddv.clear();
    QThread::msleep(1);
    for(int i = 0; i < _chans.size(); i++)
    {
        int chan = _chans[i];
        QString filepath = "";
        qDebug() << "_cell" << _cell << "name=" <<_name << chan;
        QString cname = StateManager::instance().GetOrderName(_cell,_name,chan);
        if(cname == ""){
            filepath = QString("%1/%2_%3.csv").
                    arg(_dest).arg(_name).arg(chan);
        }else{
            filepath = QString("%1/%2_%3_%4.csv").
                    arg(_dest).arg(cname).arg(_name).arg(chan);
        }
        qDebug() << "filepath" << filepath;
//        filepath = QString("%1/%2_%3.csv").
//                arg(_dest).arg(_name).arg(chan);


        QFuture<bool> future = QtConcurrent::run(&pool,
                                                this,
                                                &AsyncExportTask::writeFile,
                                                 chan,
                                                filepath,
                                                _fileFormat);
        results.push_back(future);
    }
    for(int i = 0 ; i < results.size(); i++)
    {
        results[i].waitForFinished();
    }
    return true;
}
bool AsyncExportTask::writeXlsx()
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
            qDebug() << "count=" << count;
            QString filepath = QString("%1/%2_%3.csv").
                    arg(_dest).arg(_name).arg(i.key());
            count++;
           if(_fileFormat == FF_XLSX){

               writeXlsxFile(i.key(),i.value(),QString("%1/%2_%3.xlsx").
                         arg(_dest).arg(_name).arg(i.key()));
               emit onProgress(this,10+ 80*count/ddm.size());
           }
       }
       return true;
}
bool AsyncExportTask::_run()
{

    QTime tm;
    tm.start();

    if(_fileFormat == FF_XLSX)
    {
        writeXlsx();
    }else{
        writeCsv();
    }

    qDebug() << "writefile elapsed = " << tm.elapsed() << "ms";
    emit onProgress(this,100);
    emit onSucc(this,"succ");
    return true;
}

bool AsyncExportTask::start()
{

    const QFuture<bool> future = QtConcurrent::run(this,&AsyncExportTask::_run);

    return true;
}
