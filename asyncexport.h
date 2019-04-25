#ifndef ASYNCEXPORT_H
#define ASYNCEXPORT_H
#include <QObject>
#include <QFuture>
#include <QFutureWatcher>
/**
 * @brief The AsyncExport class
 * 异步称重数据导出管理类.
 */
class AsyncExportManager:public QObject
{
    Q_OBJECT
public:
    AsyncExportManager();
    static AsyncExportManager& instance();
    //把某个序号设备下的某些通道的数据导出.
    bool AddTask(QString serialNo,
                 QVector<int> chans,
                 qint64 from,
                 qint64 to,
                 QString dest);
signals:
    void onProgress(QString serialNo,int progress, int err);

private:

};

#endif // ASYNCEXPORT_H
