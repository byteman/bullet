#include "asyncexport.h"
#include "singletonholder.h"
#include "dao.h"
#include "qtcsv/writer.h"
#include "qtcsv/stringdata.h"
#include <QFuture>
#include "asyncexporttask.h"
#include <QDebug>
AsyncExportManager::AsyncExportManager():
    QObject(NULL)
{

}

AsyncExportManager &AsyncExportManager::instance()
{
    static SingletonHolder<AsyncExportManager> sh;
    return *sh.get();
}

bool AsyncExportManager::AddTask(
                                int format,
                                QString serialNo,
                                 QString name,
        QString cell,
                                 QVector<int> chans,
                                 qint64 from,
                                 qint64 to,
                                 QString dest)
{

   AsyncExportTask *task = new AsyncExportTask((FileFormat)format,serialNo,name,cell,chans,from,to,dest);
   connect(task,SIGNAL(onSucc(AsyncExportTask*,QString)),this,SLOT(on_Succ(AsyncExportTask*,QString)));

   connect(task,SIGNAL(onFail(AsyncExportTask*,QString)),this,SLOT(on_Fail(AsyncExportTask*,QString)));

   connect(task,SIGNAL(onProgress(AsyncExportTask*,int)),this,SLOT(on_Progress(AsyncExportTask*,int)));

   //tasks[task->uuid()] = task;
   return task->start();
}

void AsyncExportManager::on_Progress(AsyncExportTask* task, int progress)
{
    qDebug() << "task" << task->serialNo() << "=" << progress;
    emit onProgress(task->serialNo(),progress,0);
}

void AsyncExportManager::on_Fail(AsyncExportTask* task, QString err)
{
    //task->deleteLater();
    emit onFail(task->serialNo(),err);
}

void AsyncExportManager::on_Succ(AsyncExportTask* task, QString err)
{
    emit onSucc(task->serialNo(),err);
    task->deleteLater();
}

