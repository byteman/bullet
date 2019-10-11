#ifndef ASYNCEXPORT_H
#define ASYNCEXPORT_H
#include <QObject>
#include <QFuture>
#include <QFutureWatcher>
class AsyncExportTask;
/**
 * @brief The AsyncExport class
 * 异步称重数据导出管理类.
 * 将导出操作按时间分成n个线程，分别去合并n个线程查询后的数据，到一个csv文件.
 */
class AsyncExportManager:public QObject
{
    Q_OBJECT
public:
    AsyncExportManager();
    static AsyncExportManager& instance();
    //把某个序号设备下的某些通道的数据导出.
    bool AddTask(QString serialNo,
                 QString name,
                 QVector<int> chans,
                 qint64 from,
                 qint64 to,
                 QString dest);
signals:
    void onProgress(QString serialNo,int progress, int err);
    void onFail(QString serialNo,QString err);
    void onSucc(QString serialNo,QString err);
protected slots:
    void on_Progress(AsyncExportTask* task,int progress);
    void on_Fail(AsyncExportTask* task,QString err);
    void on_Succ(AsyncExportTask* task,QString err);
private:
    QMap<QString,AsyncExportTask*> tasks ;
};

#endif // ASYNCEXPORT_H
