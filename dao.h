#ifndef DAO_H
#define DAO_H
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QList>
#include <QtCore>
#include "models.h"
class DAO
{
public:
    DAO();
    static DAO& instance();
    QSqlError Init(QString DataBase,QString DbDir);
    //打开设备数据库
    QSqlError InitDataDb(QString DbDir,QStringList Devices);

    QSqlError ConnectDB(QString host, int port, QString UserName, QString PassWord, QString DataBase);
    //设备管理
    QSqlError DeviceCount(int &count);
    QSqlError DeviceAdd(QString serialNo,QString name);
    QSqlError DeviceRemove(QString serialNo);
    QSqlError DeviceUpdate(QString serialNo, QString name);
    QSqlError DeviceList(DeviceInfoList& devices);
    //设备通道配置管理
    QSqlError DeviceChannalAdd(QString serialNo,int chan,DeviceChnConfig& cfg);
    //删除某个设备的所有通道配置.
    QSqlError DeviceChannalRemove(QString serialNo);
    //修改某个设备通道的通道配置
    QSqlError DeviceChannalUpdate(QString serialNo,int chan, DeviceChnConfig &cfg);
    //获取某个设备某个通道的配置数据
    QSqlError DeviceChannalGet(QString serialNo,int chan, DeviceChnConfig &cfg);
    QSqlError DeviceChannalUpdateState(QString serialNo,int chan, bool paused );

     //设备数据管理,一个设备一个表.
    //添加一条测力数据.
    QSqlError DeviceDataAdd(QString serialNo,DeviceData& data);
    //添加一组测力数据.
    QSqlError DeviceDataAdd(QString serialNo,DeviceDataList& data);
    //清空某个设备的所有数据.
    QSqlError DeviceChanDataRemove(QString serialNo,int chan);
    QSqlError DeviceDataTblRemove(QString serialNo);
    QSqlError DeviceDataRemove(QString serialNo);
    QSqlError DeviceDataRemove(QString serialNo,qint64 from,qint64 to);
    QSqlError DeviceDataInfo(QString serialNo,qint64 &start,qint64 &end, qint64 &count);

    //查询某个设备的某个通道某个时间段的历史数据.
    QSqlError DeviceDataQuery(QString serialNo,
                              int chan,
                              qint64 from,
                              qint64 to,
                              DeviceDataList& dataList);
    QSqlError DeviceDataQuery(QString serialNo,
                              qint64 from,
                              qint64 to,
                              MultiDeviceDataMap& dataList);

    QSqlError ChannelValueFix(QString serialNo,
                              qint64 from,
                              qint64 to,int chan, int maxValue,int fixValue);
    bool ExistKey(QString key);
    bool getParam(QString key, QString &value);
    QSqlError updateParam(QString key, QString value);
    QSqlError insertParam(QString key, QString value);
    //参数管理模块
    QSqlError WriteIntParam(QString key, int value);
    QSqlError WriteStringParam(QString key, QString value);
    bool ReadBoolParam(QString key, bool defValue);
    int ReadIntParam(QString key, int defValue);
    QString ReadStringParam(QString key, QString defValue);
    QSqlError CreateDataTable(QString serialNo);
    QSqlError WriteBoolParam(QString key, bool value);

    QSqlError DbRecycle(QString serialNo);

private:

    QSqlDatabase db; //配置数据库
    QMap<QString,QSqlDatabase> dataDbMap; //数据相关的数据库,一个设备名对应一个连接名.一个连接名只能同时在一个线程中使用,
    QString data_dir;
    QSqlError CreateDeviceTable();
    QSqlError CreateDeviceChannelConfigTable();
    bool DeviceChannalExists(QString serialNo, int chan);
    QSqlError CreateParamTable();
    QSqlError ConnectDataDB(QString host,
                            int port,
                            QString UserName,
                            QString PassWord,
                            QString Path,
                            QString DeviceName);
    QSqlError CreateDataTableIndex(QString serialNo);
};

#endif // DAO_H
