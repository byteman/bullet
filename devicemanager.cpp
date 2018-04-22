#include "devicemanager.h"
#include <QSettings>
DeviceManager::DeviceManager():
    m_serial_id(0),
    m_session_id(0)
{

}

bool DeviceManager::start()
{

    QSettings config("bullet.ini", QSettings::IniFormat);


    m_dev_num = config.value("/device/num",1).toInt();
    QString str_id= config.value("/device/serial","1").toString();
    ids = str_id.split(",");
    names = config.value("/device/name","1").toString().split(",");
    if( ids.size() < 1 || ids.size() != names.size())
    {
        return false;
    }
    id_name_map.clear();
    for(int i = 0; i < ids.size();i++)
    {
        id_name_map[ids[i].toInt()] = names[i];
        Device* dev = new Device();
        connect(dev,SIGNAL(Notify(QString)),this,SLOT(onNotify(QString)));
        connect(dev,SIGNAL(ReadParam(Device*,MsgDevicePara)),this,SLOT(onReadParam(Device*,MsgDevicePara)));
        connect(dev,SIGNAL(WriteParam(Device*,bool)),this,SLOT(onWriteParam(Device*,bool)));
        connect(dev,SIGNAL(EnumFiles(Device*,MsgFileList)),this,SLOT(onEnumFiles(Device*,MsgFileList)));
        connect(dev,SIGNAL(Progress(Device*,QString)),this,SLOT(onProgress(Device*,QString)));
        connect(dev,SIGNAL(showWave(Device*,MsgWaveData)),this,SLOT(onWaveMsg(Device*,MsgWaveData)));
        dev->setId(ids[i].toInt());
        dev->setName(names[i]);
        dev_map[ids[i].toInt()] = dev;
    }
    //this->startTimer(1000);
    return true;
}

void DeviceManager::stop()
{
    for(int i = 0; i < ids.size();i++)
    {
        disconnect(dev_map[ids[i].toInt()],SIGNAL(Notify(QString)),0,0);
    }
}

bool DeviceManager::SendAllWave()
{
    bool ok = false;
    QMapIterator<quint32,Device*> i(dev_map);
    while (i.hasNext()) {
        i.next();

        if(i.value()->SendStartWave(m_session_id++) == 0)
        {
            ok = false;
        }

    }

    return ok;
}

bool DeviceManager::StartAll(bool start)
{
    bool ok = false;
    QMapIterator<quint32,Device*> i(dev_map);
    while (i.hasNext()) {
        i.next();

        if(i.value()->StartRecWave(m_session_id++,start) == 0)
        {
            ok = false;
        }

    }

    return ok;
}

bool DeviceManager::ResetAllDevice(quint8 delay_s)
{
    bool ok = true;
    QMapIterator<quint32,Device*> i(dev_map);
    while (i.hasNext()) {
        i.next();

        if(i.value()->Reset(delay_s) == false)
        {
            ok = false;
        }

    }

    return ok;
}

bool DeviceManager::ResetDevice(quint32 dev_id,quint8 delay_s)
{
    if(!dev_map.contains(dev_id))
        return false;
    return dev_map[dev_id]->Reset(delay_s);
}

bool DeviceManager::ListFiles(quint32 dev_id)
{
    if(!dev_map.contains(dev_id))
        return false;
    return dev_map[dev_id]->ListFiles();
}

void DeviceManager::ReadParam(quint32 dev_id)
{
    if(!dev_map.contains(dev_id))
        return ;
    return dev_map[dev_id]->ReadParam();
}

void DeviceManager::WriteParam(quint32 dev_id, MsgDevicePara &para)
{
    if(!dev_map.contains(dev_id))
        return ;
    return dev_map[dev_id]->WriteParam(para);
}

void DeviceManager::SetStation(QString station)
{

    QMapIterator<quint32,Device*> i(dev_map);
    while (i.hasNext()) {
        i.next();
            i.value()->setStation(station);
    }
}

void DeviceManager::ListDevice(QList<Device *> &devices)
{
    QMapIterator<quint32,Device*> i(dev_map);
    while (i.hasNext()) {
        i.next();

        devices.push_back(i.value());

    }
}

Device *DeviceManager::GetDevice(quint32 dev_id)
{
    if(!dev_map.contains(dev_id))
        return NULL;
    return dev_map[dev_id];
}

void DeviceManager::GetDeviceWaveFiles(quint32 dev_id, QStringList &files)
{
    if(!dev_map.contains(dev_id))
        return;
    return dev_map[dev_id]->listWaveFiles(files);
}

bool DeviceManager::LoadWaveFile(quint32 dev_id, QString file, MsgWaveData &wvd)
{
    if(!dev_map.contains(dev_id))
        return false;
    return dev_map[dev_id]->LoadWaveFile(file,wvd);
}

//这里获取到的通道数据必然保护通道所属的设备ID，从而能够区分设备.
void DeviceManager::onWaveMsg(Device* dev,MsgWaveData wvData)
{
     emit WaveMsg(dev,wvData);
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
        quint32 dev_id = temp_msg.head.device_id;
        if(dev_map.contains(dev_id))
        {
            //dev_map[dev_id]->setSess(msg.getSession());
            dev_map[dev_id]->setHostPort(msg.getHost(),msg.getPort());
        }
    }
    while(parser.parseData(input_msg))
    {
        quint32 dev_id = input_msg.head.device_id;
        if(dev_map.contains(dev_id))
        {
            dev_map[dev_id]->setSess(msg.getSession());
            //dev_map[dev_id]->setHostPort(msg.getHost(),msg.getPort());
            dev_map[dev_id]->onMessage(input_msg,output_msg);

            if(!input_msg.is_ack)
            {
                QByteArray arr;
                output_msg.head = input_msg.head;
                //请求包的回应，统一置位最高位.
                output_msg.head.cmd_id|=0x80;

                output_msg.toByteArray(arr);
                msg.getSession()->send(arr);
            }
        }
        input_msg.clear();

    }

}

void DeviceManager::onNotify(QString msg)
{
    emit Notify(msg);
}

void DeviceManager::onEnumFiles(Device *dev, MsgFileList files)
{
    emit EnumFiles(dev,files);
}

void DeviceManager::onProgress(Device *dev, QString progress)
{
    emit Progress(dev,progress);
}


//定时调用.
void DeviceManager::timerEvent(QTimerEvent *)
{
    QMapIterator<quint32,Device*> i(dev_map);
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
