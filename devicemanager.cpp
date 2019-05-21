#include "devicemanager.h"
#include <QSettings>
#include "dao.h"
#include "config.h"
DeviceManager::DeviceManager():
    m_use_sys_time(true),
    m_save_int_s(3)
{

}


QString DeviceManager::GetLastError()
{
    return m_last_err;
}
bool DeviceManager::addOneDevice(QString& serialNo, QString& name)
{
    Device* dev = new Device();
    connect(dev,SIGNAL(Notify(QString)),this,SLOT(onNotify(QString)));
    connect(dev,SIGNAL(ReadParam(Device*,MsgDevicePara)),this,SLOT(onReadParam(Device*,MsgDevicePara)));
    connect(dev,SIGNAL(WriteParam(Device*,bool)),this,SLOT(onWriteParam(Device*,bool)));

    connect(dev,SIGNAL(OnSensorData(Device*,MsgSensorData)),this,SLOT(onSensorMsg(Device*,MsgSensorData)));
    connect(dev,SIGNAL(ResetResult(Device*,bool)),this,SLOT(onResetResult(Device*,bool)));
    connect(dev,SIGNAL(CalibResult(Device*,int,int,int)),this,SLOT(onCalibResult(Device*,int,int,int)));
    connect(dev,SIGNAL(CalibResult(Device*,int,int,int)),this,SLOT(onCommResult(Device*,int,int)));

    dev->setId(serialNo);
    dev->setName(name);
    dev_lock.lock();
    dev_map[serialNo] = dev;
    dev_lock.unlock();
    return true;
}
//初始化设备管理器，重新加载设备.
bool DeviceManager::Init()
{

    DeviceInfoList devList;
    DAO::instance().DeviceList(devList);
    m_save_int_s = Config::instance().m_save_intS;
    m_use_sys_time = Config::instance().m_use_sys_time;
    for(int i = 0; i < devList.size();i++)
    {
        addOneDevice(devList[i].serialNo,devList[i].name);
    }
    this->startTimer(1000);
    return true;
}
void DeviceManager::removeOneDevice(Device* dev)
{
    if(dev==NULL){
        return;
    }
    connect(dev,SIGNAL(Notify(QString)),this,SLOT(onNotify(QString)));
    connect(dev,SIGNAL(ReadParam(Device*,MsgDevicePara)),this,SLOT(onReadParam(Device*,MsgDevicePara)));
    connect(dev,SIGNAL(WriteParam(Device*,bool)),this,SLOT(onWriteParam(Device*,bool)));

    connect(dev,SIGNAL(OnSensorData(Device*,MsgSensorData)),this,SLOT(onSensorMsg(Device*,MsgSensorData)));
    connect(dev,SIGNAL(ResetResult(Device*,bool)),this,SLOT(onResetResult(Device*,bool)));
    connect(dev,SIGNAL(CalibResult(Device*,int,int,int)),this,SLOT(onCalibResult(Device*,int,int,int)));
    connect(dev,SIGNAL(CalibResult(Device*,int,int,int)),this,SLOT(onCommResult(Device*,int,int)));
    dev_map.remove(dev->id());
    delete dev;
}
void DeviceManager::Uninit()
{
     dev_lock.lock();
     QMap<QString,Device*>::const_iterator it;
     while (it != dev_map.constEnd()) {
         Device* dev = it.value();
         if(dev==NULL){
             continue;
         }
         removeOneDevice(dev);
         ++it;
       }
     dev_map.clear();
     dev_lock.unlock();
}

bool DeviceManager::ResetAllDevice(quint8 delay_s)
{
    bool ok = true;
    QMapIterator<QString,Device*> i(dev_map);
    while (i.hasNext()) {
        i.next();

        if(i.value()->Reset(delay_s) == false)
        {
            ok = false;
        }

    }

    return ok;
}

bool DeviceManager::ResetDevice(QString dev_id,quint8 delay_s)
{
    if(!dev_map.contains(dev_id))
        return false;
    return dev_map[dev_id]->Reset(delay_s);
}

void DeviceManager::ResetDeviceCount(QString dev_id)
{
    if(!dev_map.contains(dev_id))
        return ;
    return dev_map[dev_id]->ResetCount();
}

int DeviceManager::GetDeviceCount(QString dev_id)
{
    if(!dev_map.contains(dev_id))
        return 0;
    return dev_map[dev_id]->GetCount();
}

bool DeviceManager::SetSaveInt(int s)
{
    m_save_int_s = s;
    return true;
}



void DeviceManager::calib(QString dev_id,quint8 chan,quint8 index, int weight)
{
    if(!dev_map.contains(dev_id))
        return ;
    dev_map[dev_id]->calib(chan,index,weight);
}
//bool DeviceManager::UpdateRecState(QString &dev_id,int chan, bool pause)
//{
//    if(dev_map.contains(dev_id)){
//       dev_map[dev_id]->UpdateState(chan,pause);
//    }
//    return true;
//}
//设置设备通道的参数.
bool DeviceManager::SetChanConfig(QString &dev_id, int chan, DeviceChnConfig &cfg)
{
    if(dev_map.contains(dev_id)){
       dev_map[dev_id]->UpdateChanConfig(chan,cfg);
    }
    return true;
}
//单独修改通道的状态.
bool DeviceManager::ControlDeviceChan(QString dev_id, int chan, bool pause)
{
    QSqlError err =DAO::instance().DeviceChannalUpdateState(dev_id,chan,pause);
    if(err.isValid()){
        qDebug() << " ControlDeviceChan failed " << err.text();
    }
    DeviceChnConfig cfg;

    GetDeviceChan(dev_id,chan,cfg);
    cfg.paused = pause?1:0;
    SetChanConfig(dev_id,chan,cfg);

    //UpdateRecState(dev_id,chan,pause);
    return !err.isValid();
}

bool DeviceManager::RemoveDevice(QString dev_id)
{
    //1.先删除设备表中的设备
    QSqlError err = DAO::instance().DeviceRemove(dev_id);
    if(err.isValid()){
        qDebug() << " RemoveDevice failed " << err.text();
    }
    //2.在删除设备通道表中数据
    err = DAO::instance().DeviceChannalRemove(dev_id);
    if(err.isValid()){
        qDebug() << " DeviceChannalRemove failed " << err.text();
    }
    //3.删除设备数据表中数据.
    err = DAO::instance().DeviceDataRemove(dev_id);
    if(err.isValid()){
        qDebug() << " DeviceDataRemove failed " << err.text();
    }
    removeOneDevice(GetDevice(dev_id));

    return !err.isValid();
}
int DeviceManager::DeviceCount()
{
    int num = 0;
    QSqlError err = DAO::instance().DeviceCount(num);
    if(err.isValid()){
        qDebug() << err;
    }
    return num;
}

void DeviceManager::Sync()
{

    QMapIterator<QString,Device*> i(dev_map);
    while (i.hasNext()) {
        i.next();
        i.value()->Sync();
    }
}
//添加设备
//1.数据库中添加一个设备.
bool DeviceManager::AddDevice(QString dev_id, QString dev_name)
{

    //添加一个设备到设备表
    QSqlError err = DAO::instance().DeviceAdd(dev_id,dev_name);
    if(err.isValid()){
        qDebug() << " AddDevice failed " << err.text();
    }
    //在添加一个设备的数据表.
    err = DAO::instance().CreateDataTable(dev_id);
    if(err.isValid()){
        qDebug() << " CreateDataTable" << dev_id << " failed= " << err.text();
    }
    addOneDevice(dev_id,dev_name);

    return !err.isValid();
}
//修改数据库，在更新内存对象.
bool DeviceManager::UpdateDevice(QString dev_id, QString dev_name)
{
    QSqlError err = DAO::instance().DeviceUpdate(dev_id,dev_name);
    if(err.isValid()){
        qDebug() << " UpdateDevice failed " << err.text();
    }
    Device* dev= GetDevice(dev_id);
    if(dev!=NULL){
        dev->setName(dev_name);
    }
    return !err.isValid();
}
//修改了通道的配置参数.同时修改数据库和内存参数.
bool DeviceManager::UpdateDeviceChan(QString dev_id, int chan,DeviceChnConfig& cfg)
{
    QSqlError err = DAO::instance().DeviceChannalUpdate(dev_id,chan,cfg);
    if(err.isValid()){
        qDebug() << " UpdateDeviceChan failed " << err.text();
    }
    Device* dev= GetDevice(dev_id);
    if(dev!=NULL){
        dev->UpdateChanConfig(chan,cfg);
    }
    return !err.isValid();
}

bool DeviceManager::GetDeviceChan(QString dev_id, int chan, DeviceChnConfig &cfg)
{
    QSqlError err = DAO::instance().DeviceChannalGet(dev_id,chan,cfg);
    if(err.isValid()){
        qDebug() << " GetDeviceChan failed " << err.text();
    }
    return !err.isValid();
}
void DeviceManager::ReadParam(QString dev_id)
{
    if(!dev_map.contains(dev_id))
        return ;
    return dev_map[dev_id]->ReadParam();
}

void DeviceManager::WriteParam(QString dev_id, MsgDevicePara &para)
{
    if(!dev_map.contains(dev_id))
        return ;
    return dev_map[dev_id]->WriteParam(para);
}

void DeviceManager::ListDevice(QList<Device *> &devices)
{
    QMapIterator<QString,Device*> i(dev_map);
    while (i.hasNext()) {
        i.next();

        devices.push_back(i.value());

    }
}

Device *DeviceManager::GetDevice(QString dev_id)
{
    if(!dev_map.contains(dev_id))
        return NULL;
    return dev_map[dev_id];
}

void DeviceManager::onCommResult(Device *dev, int cmd, int result)
{
    emit CommResult(dev,cmd,result);
}

//这里获取到的通道数据必然保护通道所属的设备ID，从而能够区分设备.
void DeviceManager::onWaveMsg(Device* dev,MsgWaveData wvData)
{
     emit WaveMsg(dev,wvData);
}
void DeviceManager::WriteValues(Device* dev,MsgSensorData& msg)
{
    DeviceDataList ddl;
    qint32 time = 0;
   // qDebug() << "id=" << QThread::currentThreadId();
    if(Config::instance().m_use_sys_time){
        time = qint32(QDateTime::currentMSecsSinceEpoch() / 1000);
        if(!dev->checkCanSave(time,Config::instance().m_save_intS)){
            //系统时钟模式还没有达到保存时间，就返回.
            //qDebug() << "time not reach!";
            return;
        }
    }

    for(int i = 0; i <msg.channels.size();i++)
    {
        //如果这个设备的这个通道已经禁用了。
         if(dev->IsPaused(msg.channels[i].addr)){
             continue;
         }
         DeviceData dd;
         dd.chan = msg.channels[i].addr;
         dd.value = msg.channels[i].weight;
         dd.timestamp = msg.channels[i].time;
         if(Config::instance().m_use_sys_time){
             dd.timestamp = time;
             //判断是否可以存储了

         }
         ddl.push_back(dd);
    }

    QSqlError err=DAO::instance().DeviceDataAdd(msg.m_dev_serial,ddl);
    if(err.isValid()){
        qDebug() << "DeviceDataAdd err=" << err.text();
    }
}
//收到设备发过来的波形数据，存储.
void DeviceManager::onSensorMsg(Device* dev,MsgSensorData msData)
{
     emit SensorMsg(dev,msData);
     //WriteValues(dev,msData);
}
void DeviceManager::onReadParam(Device *dev, MsgDevicePara para)
{
    emit ReadParam(dev,para);
}

void DeviceManager::onWriteParam(Device *dev, bool result)
{
    emit WriteParam(dev,result);
}
//用UDP更方便点，每个包加一个确认信息.
//设备上线以后应该有一个上报设备序列号的消息包,建立了一个IP和设备ID的映射关系表.
//有一个维持心跳的命令.
//设备等待上位机发送波形请求包.回应波形请求包.波形请求包是多次发送
//有包序号，第0包表示开始包， [当前包/总包数]


//设备->PC[设备编号+会话号+序列号+包大小+当前包号+总包数+n包数据] 包数n=根据包大小推算出来.
//PC->设备[设备编号+会话号+序列号]

void DeviceManager::Message(SessMessage msg)
{
    ProtoMessage input_msg;
    ProtoMessage temp_msg;
    ProtoMessage output_msg;
    parser.pushData(msg.getData());
//    QTime time;

//    time.start();
    if(parser.parseData(msg.getData(),temp_msg))
    {
        //Q dev_id = temp_msg.head.device_id;
        //memcpy(msg.head.device_id , m_dev_id.toLocal8Bit().data(),12);

        char buf[13]={0,};
        memcpy(buf,temp_msg.head.device_id,12);
        QString dev_id = buf;
        if(dev_map.contains(dev_id))
        {
            //dev_map[dev_id]->setSess(msg.getSession());
            dev_map[dev_id]->setHostPort(msg.getHost(),msg.getPort());
        }
    }
    while(parser.parseData(input_msg))
    {
        char buffer[13]={0,};
        memcpy(buffer,input_msg.head.device_id,12);
        QString dev_id = QString(buffer);
        if(dev_map.contains(dev_id))
        {
            dev_map[dev_id]->setSess(msg.getSession());
            //dev_map[dev_id]->setHostPort(msg.getHost(),msg.getPort());
            //qDebug() << "get message cmd=" << input_msg.head.cmd_id;
            bool succ = dev_map[dev_id]->onMessage(input_msg,output_msg);
            //是否需要回应，并且成功处理数据，才进行回应.
            if(!input_msg.is_ack && succ)
            {
                QByteArray arr;
                output_msg.head = input_msg.head;
                //请求包的回应，统一置位最高位.
                output_msg.head.cmd_id|=0x80;

                output_msg.toByteArray(arr);
                dev_map[dev_id]->SendData(arr);
            }
        }
        input_msg.clear();

    }
    //qDebug()<<time.elapsed()<<"ms";
}

void DeviceManager::onNotify(QString msg)
{
    emit Notify(msg);
}

void DeviceManager::onCalibResult(Device *dev, int chan, int index, int result)
{
    emit CalibResult(dev,chan,index,result);
}

void DeviceManager::onRealTimeResult(Device *dev, RT_AD_RESULT result)
{
    emit RealTimeResult(dev,result);
}

void DeviceManager::onResetResult(Device *dev, bool result)
{
    qDebug() << "onResetResult";
    emit ResetResult(dev,result);
}


//定时调用.
void DeviceManager::timerEvent(QTimerEvent *)
{
    QMapIterator<QString,Device*> i(dev_map);
    while (i.hasNext()) {
        i.next();
        bool online = i.value()->online();
        i.value()->checkOnline();
        if(i.value()->online()!=online)
        {
            if(online)
                emit DevOffline(i.value());
            else
                emit DevOnline(i.value());
        }

    }
}
