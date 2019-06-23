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

    config.setIniCodec("UTF-8");//添上这句就不会出现乱码了);

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
        id_name_map[ids[i]] = names[i];
        Device* dev = new Device();
        connect(dev,SIGNAL(Notify(QString)),this,SLOT(onNotify(QString)));
        connect(dev,SIGNAL(ReadParam(Device*,MsgDevicePara)),this,SLOT(onReadParam(Device*,MsgDevicePara)));
        connect(dev,SIGNAL(WriteParam(Device*,bool)),this,SLOT(onWriteParam(Device*,bool)));
        connect(dev,SIGNAL(EnumFiles(Device*,ENUM_FILE_RESP)),this,SLOT(onEnumFiles(Device*,ENUM_FILE_RESP)));
        connect(dev,SIGNAL(Progress(Device*,QString)),this,SLOT(onProgress(Device*,QString)));
        connect(dev,SIGNAL(showWave(Device*,MsgWaveData)),this,SLOT(onWaveMsg(Device*,MsgWaveData)));

        connect(dev,SIGNAL(CalibResult(Device*,int,int,int)),this,SLOT(onCalibResult(Device*,int,int,int)));
        connect(dev,SIGNAL(RealTimeResult(Device*,RT_AD_RESULT)),this,SLOT(onRealTimeResult(Device*,RT_AD_RESULT)));

        connect(dev,SIGNAL(CalibResult(Device*,int,int,int)),this,SLOT(onCommResult(Device*,int,int)));

        dev->setId(ids[i]);
        dev->setName(names[i]);
        dev_map[ids[i]] = dev;
    }
    //this->startTimer(1000);
    return true;
}

void DeviceManager::stop()
{
    for(int i = 0; i < ids.size();i++)
    {
        disconnect(dev_map[ids[i]],SIGNAL(Notify(QString)),0,0);
    }
}

bool DeviceManager::SendAllWave(bool start)
{
    bool ok = false;
//    QMapIterator<quint32,Device*> i(dev_map);
//    while (i.hasNext()) {
//        i.next();

//        if(i.value()->SendStartWave(m_session_id++,start) == 0)
//        {
//            ok = false;
//        }

//    }

    return ok;
}

bool DeviceManager::SendWave(QString dev_id, bool start)
{
    //return dev_map[dev_id]->SendStartWave(m_session_id++,start)!=0;
    return true;
}

bool DeviceManager::StartAll(bool start)
{
    bool ok = false;
    QMapIterator<QString,Device*> i(dev_map);
    while (i.hasNext()) {
        i.next();

        if(i.value()->StartRecWave(m_session_id++,start) == 0)
        {
            ok = false;
        }

    }

    return ok;
}

bool DeviceManager::SyncFile(QString dev_id, QString file)
{
    return true;
}




bool DeviceManager::ResetDevice(QString dev_id,quint8 delay_s)
{
    if(!dev_map.contains(dev_id))
        return false;
    return dev_map[dev_id]->Reset(delay_s);
}

bool DeviceManager::ListFiles(QString dev_id, int page, int size)
{
    return true;
}

void DeviceManager::calib(QString dev_id, quint8 chan, quint8 index, int weight)
{

}

void DeviceManager::ReadParam(QString dev_id)
{

}

bool DeviceManager::RemoveDevice(QString dev_id)
{
    return true;
}

void DeviceManager::WriteParam(QString dev_id, MsgDevicePara &para)
{

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

bool DeviceManager::RemoveFile(QString dev_id, QString file)
{
    return true;
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
