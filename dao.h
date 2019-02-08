﻿#ifndef DAO_H
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
    QSqlError Init(QString DataBase);
    QSqlError ConnectDB(QString host, int port, QString UserName, QString PassWord, QString DataBase);
    //设备管理
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

     //设备数据管理,一个设备一个表.
    //添加一条测力数据.
    QSqlError DeviceDataAdd(QString serialNo,DeviceData& data);
    //添加一组测力数据.
    QSqlError DeviceDataAdd(QString serialNo,DeviceDataList& data);
    //清空某个设备的所有数据.
    QSqlError DeviceDataRemove(QString serialNo);

    //查询某个设备的某个通道某个时间段的历史数据.
    QSqlError DeviceDataQuery(QString serialNo,int chan, qint64 from, qint64 to, DeviceDataList& dataList);

private:
    QSqlError CreateDataTable(QString serialNo);
    QSqlDatabase db;
    QSqlError CreateDeviceTable();
    QSqlError CreateDeviceChannelConfigTable();
    bool DeviceChannalExists(QString serialNo, int chan);
};

#endif // DAO_H