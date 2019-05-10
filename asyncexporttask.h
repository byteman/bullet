#ifndef ASYNCEXPORTTASK_H
#define ASYNCEXPORTTASK_H

#include <QObject>
#include <QFuture>
#include <QFutureWatcher>


class AsyncExportTask:public QObject
{
    Q_OBJECT
public:
    AsyncExportTask(QString serialNo,
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
signals:

    void onProgress(AsyncExportTask* task,int progress);
    void onFail(AsyncExportTask* task,QString err);
    void onSucc(AsyncExportTask* task,QString err);

private:
    QString         _serialNo;
    QVector<int>    _chans;
    qint64          _from;
    qint64          _to;
    QString         _dest;
    QString         _uuid;
    QFutureWatcher<bool> *watcher;
};

#endif // ASYNCEXPORTTASK_H
