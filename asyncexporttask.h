#ifndef ASYNCEXPORTTASK_H
#define ASYNCEXPORTTASK_H

#include <QObject>
#include <QFuture>
#include <QFutureWatcher>
#include <models.h>
#include <QAtomicInt>
enum FileFormat{
  FF_XLSX=0,
  FF_CSV=1,

};
class AsyncExportTask:public QObject
{
    Q_OBJECT
public:
    AsyncExportTask(
                    FileFormat format,
                    QString serialNo,
                    QString name,
                    QString cellNo,
                    QVector<int> chans,
                    qint64 from,
                    qint64 to,
                    QString dest);
    ~AsyncExportTask();
    QString uuid();
    QString serialNo()
    {
        return _serialNo;
    }
    //启动任务.
    bool start();
    //结束任务.
    bool cancel();
    bool _run();
    bool writeFile(int chan, QString filePath, int format);
signals:

    void onProgress(AsyncExportTask* task,int progress);
    void onFail(AsyncExportTask* task,QString err);
    void onSucc(AsyncExportTask* task,QString err);

private:
    QString         _serialNo;
    QString         _name;
    QString         _cell;
    QVector<int>    _chans;
    qint64          _from;
    qint64          _to;
    QString         _dest;
    QString         _uuid;
    QAtomicInt      _prog;
    int             _total;
    FileFormat      _fileFormat;
    QVector<DeviceDataList> ddv;
    QMutex _mutex;
    bool writeXlsx();
    bool writeCsv();
    bool writeCsvFile(int chan, DeviceDataList ddl, QString file);
    bool writeXlsxFile(int chan, DeviceDataList ddl, QString filePath);
};

#endif // ASYNCEXPORTTASK_H
