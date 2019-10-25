#ifndef MY_DAO_H
#define MY_DAO_H

#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QList>

struct BarInfo{
    int devid; //设备编号
    QString devip; //设备ip
    QString logguid; //对应的唯一编号
    QString barcode;//电芯编号.
    int chan; //对应的通道编号

};
class MyDAO
{
public:
    MyDAO();

    QSqlError Init(QString host,int port, QString UserName, QString PassWord, QString DataBase);
    QSqlError ConnectDB(QString host,int port, QString UserName, QString PassWord, QString DataBase);
    QSqlError ReConnnectDB();
    QSqlError CloseDB();
    QString   GetLastError();

    //根据barcode查询一个对应的测试信息
    bool QueryBarCode(QString barcode,BarInfo& info);

    static MyDAO &instance();
private:
    QSqlError CreateDataTable();
    QSqlDatabase db;
};
//DAO& GetDao();
#endif // DAO_H
