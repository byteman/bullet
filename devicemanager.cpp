#include "devicemanager.h"
#include <QSettings>
#include "dao.h"
DeviceManager::DeviceManager():
    m_serial_id(0),
    m_session_id(0)
{

}

bool DeviceManager::start()
{



    DeviceInfoList devList;
    DAO::instance().DeviceList(devList);

    for(int i = 0; i < devList.size();i++)
    {

        Device* dev = new Device();
        connect(dev,SIGNAL(Notify(QString)),this,SLOT(onNotify(QString)));
        connect(dev,SIGNAL(ReadParam(Device*,MsgDevicePara)),this,SLOT(onReadParam(Device*,MsgDevicePara)));
        connect(dev,SIGNAL(WriteParam(Device*,bool)),this,SLOT(onWriteParam(Device*,bool)));

        connect(dev,SIGNAL(OnSensorData(Device*,MsgSensorData)),this,SLOT(onSensorMsg(Device*,MsgSensorData)));
        connect(dev,SIGNAL(ResetResult(Device*,bool)),this,SLOT(onResetResult(Device*,bool)));
        connect(dev,SIGNAL(CalibResult(Device*,int,int,int)),this,SLOT(onCalibResult(Device*,int,int,int)));

        connect(dev,SIGNAL(CalibResult(Device*,int,int,int)),this,SLOT(onCommResult(Device*,int,int)));

        dev->setId(devList[i].serialNo);
        dev->setName(devList[i].name);
        dev_lock.lock();
        dev_map[devList[i].serialNo] = dev;
        dev_lock.unlock();
    }
    this->startTimer(1000);
    return true;
}

void DeviceManager::stop()
{
     dev_lock.lock();
     QMap<QString,Device*>::const_iterator it;
     while (it != dev_map.constEnd()) {
           //it.value()
           ++it;
       }
//    for(int i = 0; i < ids.size();i++)
//    {
//        disconnect(dev_map[ids[i]],SIGNAL(Notify(QString)),0,0);
//    }
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


void DeviceManager::calib(QString dev_id,quint8 chan,quint8 index, int weight)
{
    if(!dev_map.contains(dev_id))
        return ;
    dev_map[dev_id]->calib(chan,index,weight);
}

bool DeviceManager::RemoveDevice(QString dev_id)
{
    QSqlError err = DAO::instance().DeviceRemove(dev_id);
    if(err.isValid()){
        qDebug() << " RemoveDevice failed " << err.text();
    }
    return !err.isValid();
}
//添加设备
//1.数据库中添加一个设备.
bool DeviceManager::AddDevice(QString dev_id, QString dev_name)
{
    QSqlError err = DAO::instance().DeviceAdd(dev_id,dev_name);
    if(err.isValid()){
        qDebug() << " AddDevice failed " << err.text();
    }
    return !err.isValid();
}

bool DeviceManager::UpdateDevice(QString dev_id, QString dev_name)
{
    QSqlError err = DAO::instance().DeviceUpdate(dev_id,dev_name);
    if(err.isValid()){
        qDebug() << " UpdateDevice failed " << err.text();
    }
    return !err.isValid();
}
bool DeviceManager::UpdateDeviceChan(QString dev_id, int chan,DeviceChnConfig& cfg)
{
    QSqlError err = DAO::instance().DeviceChannalUpdate(dev_id,chan,cfg);
    if(err.isValid()){
        qDebug() << " UpdateDeviceChan failed " << err.text();
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

void DeviceManager::SetStation(QString station)
{

    QMapIterator<QString,Device*> i(dev_map);
    while (i.hasNext()) {
        i.next();
            i.value()->setStation(station);
    }
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

void DeviceManager::GetDeviceWaveFiles(QString dev_id, QStringList &files)
{
    if(!dev_map.contains(dev_id))
        return;
    return dev_map[dev_id]->listWaveFiles(files);
}



bool DeviceManager::LoadWaveFile(QString dev_id, QString file, MsgWaveData &wvd)
{
    if(!dev_map.contains(dev_id))
        return false;
    return dev_map[dev_id]->LoadWaveFile(file,wvd);
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
void DeviceManager::onSensorMsg(Device* dev,MsgSensorData msData)
{
     emit SensorMsg(dev,msData);
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
            qDebug() << "get message cmd=" << input_msg.head.cmd_id;
            dev_map[dev_id]->onMessage(input_msg,output_msg);
            //是否需要回应，自动回应.
            if(!input_msg.is_ack )
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

}

void DeviceManager::onNotify(QString msg)
{
    emit Notify(msg);
}

void DeviceManager::onEnumFiles(Device *dev, ENUM_FILE_RESP resp)
{
    emit EnumFiles(dev,resp);
}

void DeviceManager::onProgress(Device *dev, QString progress)
{
    emit Progress(dev,progress);
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
