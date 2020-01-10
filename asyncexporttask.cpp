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
#include <QAtomicInt>

class WriteTask : public QRunnable
  {
public:
     WriteTask(int ch, DeviceDataList _ddl,QString _filePath,int _format):
         chan(ch),
         ddl(_ddl),
         filePath(_filePath),
         format(_format)
     {
     }
      void run() override
      {
          if(format == FF_XLSX){
              writeXlsxFile(chan,ddl,filePath);
          }else{
              writeCsvFile(chan,ddl,filePath);
          }
          //LeftCount--;
          //emit onProgress(this,10+ 80*c/Total);
          qDebug() << "Quit-->" << QThread::currentThread();
      }
      bool writeXlsxFile(int chan,DeviceDataList &ddl,QString filePath)
      {

          QXlsx::Document xlsxW;
          xlsxW.setColumnWidth(1,25);
          xlsxW.write("A1", "time"); // write "Hello Qt!" to cell(A,1). it's shared string.
         // xlsxW.write("B1", QString(QStringLiteral("通道")+"%1").arg(chan));
          xlsxW.write("B1","value");
          for(int j = 0; j < ddl.size(); j++)
          {
              //xlsxW.write(j+2,1,j);
              xlsxW.write(j+2,1,QString("%1").arg(QDateTime::fromMSecsSinceEpoch(ddl[j].timestamp*1000).toString("yyyy-MM-dd hh:mm:ss")));
              xlsxW.write(j+2,2,ddl[j].value);
          }

          xlsxW.saveAs(filePath); // save the document as 'Test.xlsx'

          return true;
      }
      bool writeCsvFile(int chan,DeviceDataList &ddl,QString filePath)
      {
          QtCSV::StringData strData;
          QStringList strHeader;
          strHeader << QStringLiteral("time");
          strHeader << "value";

          //strHeader << QString("%1").arg(chan);

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
private:
    int chan;
    DeviceDataList ddl;
    QString filePath;
    int format;
  };

AsyncExportTask::AsyncExportTask(FileFormat format,QString serialNo,
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
                                watcher(NULL),
                                _fileFormat(format)
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
bool AsyncExportTask::writeXlsxFile(int chan,DeviceDataList ddl,QString filePath)
{
    qDebug() << "chan" << chan << "ddl size=" << ddl.size() << QThread::currentThreadId();

    QXlsx::Document xlsxW;
    xlsxW.setColumnWidth(1,25);
    xlsxW.write("A1", "time"); // write "Hello Qt!" to cell(A,1). it's shared string.
   // xlsxW.write("B1", QString(QStringLiteral("通道")+"%1").arg(chan));
    xlsxW.write("B1","value");
    for(int j = 0; j < ddl.size(); j++)
    {    
        xlsxW.write(j+2,1,QString("%1").arg(QDateTime::fromMSecsSinceEpoch(ddl[j].timestamp*1000).toString("yyyy-MM-dd hh:mm:ss")));
        xlsxW.write(j+2,2,ddl[j].value);
    }
    //qDebug() << "chan" << chan << "ddl size222=" << ddl.size() << filePath;

    xlsxW.saveAs(filePath); // save the document as 'Test.xlsx'
    //qDebug() << "chan" << chan << "ddl size333=" << ddl.size();

    return true;
}
bool AsyncExportTask::writeCsvFile(int chan,DeviceDataList ddl,QString filePath)
{
    QTime tm;
    tm.start();
    qDebug() << QThread::currentThread() << "write csv";
    QtCSV::StringData strData;
    QStringList strHeader;
    strHeader << QStringLiteral("time");
    strHeader << "value";

    //strHeader << QString("%1").arg(chan);

    for(int j = 0; j < ddl.size(); j++)
    {
         QStringList strList;

 //        strList << "222";
//        strList << QString("%1").arg(QDateTime::fromMSecsSinceEpoch(ddl[j].timestamp*1000).toString("yyyy-MM-dd hh:mm:ss"));
          strList << QString("%1").arg(QDateTime::fromMSecsSinceEpoch(1000).toString("yyyy-MM-dd hh:mm:ss"));
        strList << QString("%1").arg(ddl[j].value);
        strData.addRow(strList);
    }
    qDebug() << QThread::currentThread() << "write csv" << tm.elapsed() << "ms";
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

    tm.restart();
    qDebug() << QThread::currentThread() << "write start";
    QtCSV::Writer::write(filePath, strData,QString(","),QString("\""),QtCSV::Writer::REWRITE,strHeader);
    qDebug() << QThread::currentThread() << "write " << tm.elapsed() << "ms";
    return true;
}
bool AsyncExportTask::writeFile(int chan, DeviceDataList ddl,QString filePath,int format)
{
    qDebug() << "chan" <<chan << "writeFile";
    bool result = false;
    if(format == FF_XLSX){
        result= writeXlsxFile(chan,ddl,filePath);
    }else{
        result= writeCsvFile(chan,ddl,filePath);
    }
    qDebug() << "chan" <<chan << "writeFile finished " << result;
    return result;
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
    QVector<QFuture<bool> > results;
     QThreadPool pool;
     pool.setMaxThreadCount(4);
    while(i.hasNext()){
        i.next();
         qDebug() << "count=" << count;
         QString filepath = QString("%1/%2_%3.csv").
                 arg(_dest).arg(_name).arg(i.key());

        if(_fileFormat == FF_XLSX){

            writeXlsxFile(i.key(),i.value(),QString("%1/%2_%3.xlsx").
                      arg(_dest).arg(_name).arg(i.key()));
            emit onProgress(this,10+ 80*count/ddm.size());
        }else{
//            writeCsvFile(i.key(),i.value(),QString("%1/%2_%3.csv").
//                      arg(_dest).arg(_name).arg(i.key()));
            QFuture<bool> future =QtConcurrent::run(&pool,this,&AsyncExportTask::writeFile,i.key(),i.value(),filepath,_fileFormat);
            results.push_back(future);

        }

        //_fileFormat = FF_CSV;

        count++;
    }
    if(_fileFormat == FF_CSV){
        for(int i = 0 ; i < results.size(); i++)
        {
            qDebug() << "i=" << i << "waitForFinished";
            results[i].waitForFinished();
            qDebug() << "i=" << i << "finished";
            emit onProgress(this,10+ 80*i/ddm.size());
        }
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
