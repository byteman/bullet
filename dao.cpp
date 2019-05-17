#include "dao.h"
#include <QtDebug>
#include <QDateTime>
#include "singletonholder.h"
//Q_GLOBAL_STATIC(DAO, dao)
#define SQL_DRIVER "QSQLITE"
#define USER_ERROR_NO_TABLE 1
#define DEV_DEVICE_TABLE "tbl_device" //设备表
#define DEV_DEVICE_CHAN_CFG_TABLE "tbl_device_chan_config" //设备通道配置表
#define DEV_DEVICE_DATA_TABLE "tbl_device_data" //设备通道配置表
#define DEV_DEVICE_PARAM_TABLE "tbl_param" //设备参数
DAO::DAO()
{


}
DAO& DAO::instance()
{
    static SingletonHolder<DAO> sh;
    return *sh.get();
}
QSqlError DAO::Init(QString DataBase)
{
    QSqlError err = ConnectDB("host",8888,"UserName","PassWord",DataBase);
    if(err.isValid()) return err;

    qDebug() << "create data table =" << err.text();

    err = CreateDeviceTable();
    if(err.isValid())
    {
        qDebug() << "CreateDeviceTable =" << err.text();
        return err;
    }
    err = CreateDeviceChannelConfigTable();
    if(err.isValid())
    {
        qDebug() << "CreateDeviceChannelConfigTable =" << err.text();
        return err;
    }
    err = CreateParamTable();
    if(err.isValid())
    {
        qDebug() << "CreateParamTable =" << err.text();
        return err;
    }
    return db.lastError();
}
QSqlError DAO::ConnectDB(QString host, int port, QString UserName, QString PassWord, QString DataBase)
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

QSqlError DAO::DeviceCount(int &count)
{
    QString sql = QString("select count(*) as cout from tbl_device");

    QSqlQuery query;
    query.exec(sql);
    while (query.next())
    {
        count   = query.value("cout").toInt();
    }
    return query.lastError();
}
//添加一个新设备.
QSqlError DAO::DeviceAdd(QString serialNo, QString name)
{
    QString sql = QString("insert into %1(time,serialNo,name) values(?,?,?);").arg(DEV_DEVICE_TABLE);
    QSqlQuery query;
    query.prepare(sql);
    query.addBindValue(QDateTime::currentDateTime().toMSecsSinceEpoch());
    query.addBindValue(serialNo);
    query.addBindValue(name);

    if(!query.exec())
    {
       //qDebug()<<query.lastError();
    }
    else
    {
    }
    return query.lastError();
}
/*
删除某个设备.
1.先删除设备后，在删除设备通道配置表中的记录，然后删除设备数据表.放到一个事务中.
*/

QSqlError DAO::DeviceRemove(QString serialNo)
{
    QString sql = QString("delete from %1 where serialNo=?;").arg(DEV_DEVICE_TABLE);
    QSqlQuery query;
    qDebug() <<"sql=" <<sql;
    query.prepare(sql);

    query.addBindValue(serialNo);


    if(!query.exec())
    {
       //qDebug()<<query.lastError();
    }
    else
    {
    }
    return query.lastError();
}

QSqlError DAO::DeviceUpdate(QString serialNo, QString name)
{
    QString sql = QString("update tbl_device set name= :name where serialNo= :serialNo;");
    QSqlQuery query;
    query.prepare(sql);
    //query.exec(sql);
    query.bindValue(":name", name);
    query.bindValue(":serialNo", serialNo);


    query.exec();

    return query.lastError();
}

QSqlError DAO::DeviceList(DeviceInfoList &devices)
{

    QString sql = QString("select * from %1").arg(DEV_DEVICE_TABLE);
    QSqlQuery query;
    query.exec(sql);
    while (query.next())
    {
        DeviceInfo dev;
        //dev.id      = query.value("id").toInt();
        dev.serialNo   = query.value("serialNo").toString();
        dev.name   = query.value("name").toString();
        devices.push_back(dev);
    }
    return query.lastError();
}
//为某个设备添加参数.
QSqlError DAO::DeviceChannalAdd(QString serialNo, int chan,DeviceChnConfig& cfg)
{
    QString sql = QString("insert into %1(serialNo,chanNr,chanName,maxValue,minValue,paused) values(?,?,?,?,?,?);").arg(DEV_DEVICE_CHAN_CFG_TABLE);
    QSqlQuery query;

    query.prepare(sql);
    qDebug() << "sql = " << sql;

    query.addBindValue(serialNo);
    query.addBindValue(chan);
    query.addBindValue(cfg.chanName);
    query.addBindValue(cfg.maxValue);
    query.addBindValue(cfg.minValue);
    query.addBindValue(cfg.paused);
    query.exec();
    return query.lastError();
}
//删除某个设备的所有通道配置.
QSqlError DAO::DeviceChannalRemove(QString serialNo)
{
    QString sql = QString("delete from %1 where serialNo=?;").arg(DEV_DEVICE_CHAN_CFG_TABLE);
    QSqlQuery query;

    query.prepare(sql);

    query.addBindValue(serialNo);


    if(!query.exec())
    {
       //qDebug()<<query.lastError();
    }
    else
    {
    }
    return query.lastError();
}
//判断某个设备某个通道是否存在.
bool DAO::DeviceChannalExists(QString serialNo, int chan)
{
    QString sql = QString("select * from %1 where serialNo=? and chanNr=?;").arg(DEV_DEVICE_CHAN_CFG_TABLE);
    QSqlQuery query;
    query.prepare(sql);
    qDebug() << "DeviceChannalExists " << serialNo << " chan=" << chan << sql;
    query.addBindValue(serialNo);
    query.addBindValue(chan);

    query.exec();
    qDebug() <<" err=" << query.lastError().text();
    bool exist = query.first();
    qDebug() << "exist=" << exist;
    return exist;

}
QSqlError DAO::DeviceChannalUpdate(QString serialNo, int chan, DeviceChnConfig &cfg)
{
    if(!DeviceChannalExists(serialNo,chan)){
        qDebug() << "not exist ";
        return DeviceChannalAdd(serialNo,chan,cfg);
    }
    QString sql = QString("update %1 set chanName=:chanName,maxValue=:maxValue,minValue=:minValue,paused=:paused where serialNo=:serialNo and chanNr=:chanNr;").arg(DEV_DEVICE_CHAN_CFG_TABLE);
    QSqlQuery query;
    query.prepare(sql);
    qDebug() << "update sql" << sql;
    query.bindValue(":chanName", cfg.chanName);
    query.bindValue(":serialNo", serialNo);
    query.bindValue(":chanNr", chan);
    query.bindValue(":maxValue", cfg.maxValue);
    query.bindValue(":minValue", cfg.minValue);
    query.bindValue(":paused", cfg.paused);
    if(!query.exec())
    {
       //qDebug()<<query.lastError();
    }
    else
    {
    }
    return query.lastError();
}

QSqlError DAO::DeviceChannalGet(QString serialNo, int chan, DeviceChnConfig &cfg)
{

    QString sql = QString("select * from %1 where serialNo= :serialNo and chanNr= :chanNr;").arg(DEV_DEVICE_CHAN_CFG_TABLE);
    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":serialNo",serialNo);
    query.bindValue(":chanNr",chan);

    if(!query.exec()){
        qDebug() << "DeviceChannalGet=" << query.lastError();
    }
    if(query.first())
    {
        cfg.chanNr = chan;
        cfg.chanName = query.value("chanName").toString();
        cfg.maxValue = query.value("maxValue").toInt();
        cfg.minValue = query.value("minValue").toInt();
        cfg.paused = query.value("paused").toInt();
    }

    return query.lastError();
}

QSqlError DAO::DeviceChannalUpdateState(QString serialNo, int chan, bool paused)
{
    if(!DeviceChannalExists(serialNo,chan)){
        qDebug() << "not exist ";
        DeviceChnConfig cfg;
        cfg.paused = paused;
        return DeviceChannalAdd(serialNo,chan,cfg);
    }
    QString sql = QString("update %1 set paused=:paused where serialNo=:serialNo and chanNr=:chanNr;").arg(DEV_DEVICE_CHAN_CFG_TABLE);
    QSqlQuery query;
    query.prepare(sql);
    qDebug() << "DeviceChannalUpdateState sql=" << sql;
    qDebug() <<"serial=" << serialNo << " chan=" << chan << "paused=" << paused;
    query.bindValue(":paused", paused?1:0);
    query.bindValue(":serialNo",serialNo);
    query.bindValue(":chanNr",chan);
    query.exec();
    return query.lastError();
}

QSqlError DAO::DeviceDataAdd(QString serialNo, DeviceData &data)
{
    QString sql = QString("insert into tbl_%1_data(time,chan,value) values(?,?,?);").arg(serialNo);
    QSqlQuery query;

    query.prepare(sql);
    query.addBindValue(data.timestamp);
    query.addBindValue(data.chan);
    query.addBindValue(data.value);

    if(!query.exec())
    {
       //qDebug()<<query.lastError();
    }
    else
    {
    }
    return query.lastError();
}

QSqlError DAO::DeviceDataAdd(QString serialNo, DeviceDataList &data)
{
    QSqlError err;
    db.transaction();
    for(int i = 0; i < data.size();i++)
    {
        if(DeviceDataAdd(serialNo, data[i]).isValid()){

        }
    }
    db.commit();
    return QSqlError();
}
QSqlError DAO::DeviceDataRemove(QString serialNo,qint64 from,qint64 to)
{
    QString sql = QString("delete from tbl_%1_data where time < ?;").arg(serialNo);
    QSqlQuery query;
    query.prepare(sql);
    query.addBindValue(to);
    query.exec(sql);
    return query.lastError();
}
//删除某个设备的全部数据.
QSqlError DAO::DeviceDataRemove(QString serialNo)
{
    //清空某个表
    //delete * from tbl_xx_data;DELETE FROM sqlite_sequence WHERE name = ‘TableName’;
    QString sql = QString("DROP TABLE IF EXISTS tbl_%1_data;").arg(serialNo);
    QSqlQuery query;

    query.exec(sql);
    //sqlite_sequence 这个表示记录每个表中的自增加id的当前编号，但是我drop表以后，再新建立表格，应该就用新的id了
   // sql = QString("DELETE FROM  sqlite_sequence WHERE name=tbl_%1_data;").arg(serialNo);

   // query.exec(sql);

    return query.lastError();

}

QSqlError DAO::DeviceDataInfo(QString serialNo, qint64 &start, qint64 &end, qint64 &count)
{
    QString sql = QString("select min(time) as start,max(time) as end,count(id) as total from tbl_%1_data ; ").arg(serialNo);
    QSqlQuery query;
    query.prepare(sql);


    query.exec();
    while (query.next())
    {

        start   = query.value("start").toLongLong();
        end =   query.value("end").toLongLong();
        count = query.value("total").toLongLong();
    }
    return query.lastError();
}
QSqlError DAO::DeviceDataQuery(QString serialNo,
                          qint64 from,
                          qint64 to,
                          MultiDeviceDataMap& dataList)
{
    QMapIterator<int,DeviceDataList> i(dataList);
    while (i.hasNext()) {
        i.next();
        DeviceDataQuery(serialNo,
                        i.key(),
                        from,
                        to,
                        dataList[i.key()]);
    }
    return QSqlError();

}
//查询某个设备某个通道的历史数据.
QSqlError DAO::DeviceDataQuery(QString serialNo,
                               int chan,
                               qint64 from,
                               qint64 to,
                               DeviceDataList &dataList)
{
    QString sql = QString("select time,chan,value from tbl_%1_data where chan =? and time > ? and time < ?; ").arg(serialNo);
    QSqlQuery query;
    query.prepare(sql);
    qDebug() << sql;
    qDebug() << serialNo << chan << from << to;

    query.addBindValue(chan);
    query.addBindValue(from);
    query.addBindValue(to);

    query.exec();
    while (query.next())
    {
        DeviceData data;
        //dev.id      = query.value("id").toInt();

        data.timestamp   = query.value("time").toLongLong();
        data.chan = (quint8)query.value("chan").toInt();
        data.value = query.value("value").toInt();

        dataList.push_back(data);
    }
    return query.lastError();
}

bool DAO::ExistKey(QString key)
{
    QString sql = QString("select * from %1 where key=?;").arg(DEV_DEVICE_PARAM_TABLE);
    QSqlQuery query;
    query.prepare(sql);

    query.addBindValue(key);


    query.exec();

    bool exist = query.first();

    return exist;

}

bool DAO::getParam(QString key, QString &value)
{
    QString sql = QString("select * from %1 where key=?;").arg(DEV_DEVICE_PARAM_TABLE);
    QSqlQuery query;
    query.prepare(sql);

    query.addBindValue(key);


    query.exec();

    bool exist = query.first();
    if(exist){
        value = query.value("value").toString();
    }
    return exist;
}

QSqlError DAO::updateParam(QString key, QString value)
{
    QString sql = QString("update %1 set value= :value where key= :key;").arg(DEV_DEVICE_PARAM_TABLE);
    QSqlQuery query;
    query.prepare(sql);

    query.bindValue(":key", key);
    query.bindValue(":value", value);
    query.exec();

    return query.lastError();
}

QSqlError DAO::insertParam(QString key, QString value)
{
    QString sql = QString("insert into %1(key,value) values(?,?);").arg(DEV_DEVICE_PARAM_TABLE);
    QSqlQuery query;
    query.prepare(sql);

    query.addBindValue(key);
    query.addBindValue(value);

    query.exec();
    return query.lastError();
}

QSqlError DAO::WriteIntParam(QString key, int value)
{
    if(ExistKey(key)){
        return updateParam(key,QString("%1").arg(value));
    }
    return insertParam(key,QString("%1").arg(value));
}
QSqlError DAO::WriteBoolParam(QString key, bool value)
{
    if(ExistKey(key)){
        return updateParam(key,QString("%1").arg(value?1:0));
    }
    return insertParam(key,QString("%1").arg(value?1:0));
}

QSqlError DAO::DbRecycle()
{
    //VACUUM
    QString sql_create = "VACUUM;";

    QSqlQuery query;
    query.exec(sql_create);
    return query.lastError();
}

QSqlError DAO::WriteStringParam(QString key, QString value)
{
    if(ExistKey(key)){
        return updateParam(key,value);
    }
    return insertParam(key,value);
}

bool DAO::ReadBoolParam(QString key, bool defValue)
{
    QString value = "";
    bool ok = false;
    if(getParam(key,value)){
        int v = value.toInt(&ok);
        if(!ok) return defValue;
        return v!=0?true:false;
    }
    return defValue;
}

int DAO::ReadIntParam(QString key, int defValue)
{
    QString value = "";
    bool ok = false;
    if(getParam(key,value)){
        int v = value.toInt(&ok);
        if(!ok) return defValue;
        return v;
    }
    return defValue;
}

QString DAO::ReadStringParam(QString key, QString defValue)
{
    QString value = "";

    if(getParam(key,value)){
        return value;
    }
    return defValue;
}
QSqlError DAO::CreateDeviceTable()
{
    QString sql_create = "CREATE TABLE IF NOT EXISTS `tbl_device` ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, `time` INTEGER NOT NULL, `serialNo` TEXT NOT NULL UNIQUE, `name` TEXT NOT NULL UNIQUE )";

    QSqlQuery query;
    query.exec(sql_create);
    return query.lastError();

}

QSqlError DAO::CreateDeviceChannelConfigTable()
{
    QString sql_create = "CREATE TABLE IF NOT EXISTS `tbl_device_chan_config` ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, `serialNo` TEXT NOT NULL, `chanNr` INTEGER NOT NULL, `chanName` TEXT NOT NULL, `maxValue` INTEGER, `minValue` INTEGER,`paused` INTEGER NOT NULL )";

    QSqlQuery query;
    query.exec(sql_create);
    return query.lastError();
}

QSqlError DAO::CreateDataTable(QString serialNo)
{
    QString sql_create = "CREATE TABLE IF NOT EXISTS `" + QString("tbl_%1_data").arg(serialNo) + "` ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, `time` INTEGER NOT NULL, `chan` INTEGER NOT NULL, `value` INTEGER NOT NULL )";

    QSqlQuery query;
    query.exec(sql_create);
    return query.lastError();

}
QSqlError DAO::CreateParamTable()
{
    QString sql_create = "CREATE TABLE IF NOT EXISTS `tbl_param` ( `key` TEXT NOT NULL UNIQUE, `value` TEXT );";

    QSqlQuery query;
    query.exec(sql_create);
    return query.lastError();

}
//CREATE TABLE `tbl_param` ( `key` TEXT NOT NULL UNIQUE, `value` TEXT )
