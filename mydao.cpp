#include "myDAO.h"
#include "singletonholder.h"
#include "Logger.h"
//Q_GLOBAL_STATIC(MyDAO, MyDAO)
#define SQL_DRIVER "QMYSQL"
#define USER_ERROR_NO_TABLE 1

const QString  DEV_DATA_TABLE= "device_data";
const QString  ALARM_DATA_TABLE= "alarm_data";
const QString  CONTACT_TABLE= "contacts";
const QString  SMS_STATE_TABLE= "sms_states";
const QString  HIST_STATE_TABLE= "hist_states";
MyDAO::MyDAO()
{


}
MyDAO& MyDAO::instance()
{
    static SingletonHolder<MyDAO> sh;
    return *sh.get();
}
QSqlError MyDAO::Init(QString host, int port, QString UserName, QString PassWord, QString DataBase)
{
    QSqlError err = ConnectDB(host,port,UserName,PassWord,DataBase);
    if(err.isValid()) return err;
    int type = err.type();
    qDebug() << "type=" << type;
    QStringList tables = db.tables();
    if(!tables.contains(DEV_DATA_TABLE))
    {


    }

    return db.lastError();
}

QSqlError MyDAO::ConnectDB(QString host, int port, QString UserName, QString PassWord, QString DataBase)
{
    db = QSqlDatabase::addDatabase(SQL_DRIVER);
    db.setHostName(host);
    db.setPort(port);
    db.setDatabaseName(DataBase);
    db.setUserName(UserName);
    db.setPassword(PassWord);
    if(db.open())
    {
       qDebug()<<"success!";
    }else{
       qDebug()<<"failure";

       return db.lastError();
    }
    qDebug() << "err=" << db.lastError().text();
    return db.lastError();

}

QSqlError MyDAO::ReConnnectDB()
{
    if(db.open())
    {
       qDebug()<<"open database success!";
    }else{
       qDebug()<<"open database failure";

       return db.lastError();
    }
    QStringList tables = db.tables();
    if(!tables.contains(DEV_DATA_TABLE))
    {


    }
    return db.lastError();

}

QSqlError MyDAO::CloseDB()
{
    db.close();
    return QSqlError();
}

QString MyDAO::GetLastError()
{
    return db.lastError().text();
}

bool MyDAO::QueryBarCode(QString barcode, BarInfo &info)
{
    QSqlQuery query;
    QString sql = QString("select * from indextable where barcode=`%1` order by StartTime desc limit 1").arg(barcode);

    query.exec(sql);
    while (query.next())
    {
        info.chan = query.value("Channel").toInt();
        info.logguid = query.value("LogGuid").toString();
        info.devip = query.value("DeviceAddr").toString();
        info.devid = query.value("DeviceID").toInt();
        info.barcode = query.value("Barcode").toString();

        //dev.id      = query.value("id").toInt();
    }
    return !query.lastError().isValid();
}
