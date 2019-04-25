#include "asyncexport.h"
#include "singletonholder.h"
#include "dao.h"
#include "qtcsv/writer.h"
#include "qtcsv/stringdata.h"
AsyncExportManager::AsyncExportManager():
    QObject(NULL)
{

}

AsyncExportManager &AsyncExportManager::instance()
{
    static SingletonHolder<AsyncExportManager> sh;
    return *sh.get();
}

bool AsyncExportManager::AddTask(QString serialNo,
                                 QVector<int> chans,
                                 qint64 from,
                                 qint64 to,
                                 QString dest)
{

    MultiDeviceDataMap ddm;
    for(int i = 0; i < chans.size(); i++)
    {
        DeviceDataList dll;
        ddm[chans[i]] = dll;
    }
    QSqlError err = DAO::instance().DeviceDataQuery(serialNo,from,to, ddm);


    QMapIterator<int,DeviceDataList> i(ddm);

    QtCSV::StringData strData;
    QStringList strHeader;
    int first = -1;
    strHeader << "时间";
    while(i.hasNext()){
        i.next();
        if(first == -1){
            first = i.key();
        }

        strHeader << QString("%1").arg(i.key());
    }
    if(first == -1) return false;
    int num = ddm[first].size();

    for(int i = 0; i <num ;i++)
    {
        QStringList strList;
        for(int j = 0; j < chans.size(); j++)
        {
            int addr = chans[j];
            DeviceDataList& ddl = ddm[addr];
            if(i >= ddl.size()) continue;

            if(j == 0){

                strList << QString("%1").arg(QDateTime::fromMSecsSinceEpoch(ddl[i].timestamp*1000).toString("yyyy-MM-dd hh:mm:ss"));
            }
            strList << QString("%1").arg(ddl[i].value);
        }
        strData.addRow(strList);
    }
    QString filePath = dest;
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
    return !err.isValid();

}

