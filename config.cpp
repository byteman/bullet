#include "config.h"
#include "dao.h"
#include "singletonholder.h"
#include <QDir>
Config::Config()
{

}

Config &Config::instance()
{
    static SingletonHolder<Config> sh;
    return *sh.get();
}
bool Config::Init()
{
    m_use_sys_time = false;//DAO::instance().ReadBoolParam("use_sys_time",false);
    m_rt_wave_min  = DAO::instance().ReadIntParam("rt_wave_min",20);
    m_lang = (Lang)DAO::instance().ReadIntParam("lang",(int)LANG_ZH);
    m_local_port = DAO::instance().ReadIntParam("port",8881);
    m_save_intS= DAO::instance().ReadIntParam("saveInt",3);
    m_host_name= DAO::instance().ReadStringParam("host_name","24#2");
    m_data_dir= DAO::instance().ReadStringParam("data_dir",QCoreApplication::applicationDirPath());
    return true;
}

bool Config::SetUseSysTime(bool use)
{
//    DAO::instance().WriteIntParam("use_sys_time",use);
//    m_use_sys_time = use;
    return true;
}

bool Config::SetRtWaveMin(int min)
{
    DAO::instance().WriteIntParam("rt_wave_min",min);
    m_rt_wave_min = min;
    return true;
}

bool Config::SetLocalPort(int port)
{
    DAO::instance().WriteIntParam("port",port);
    m_local_port = port;
    return true;
}

bool Config::SetSaveInt(int timeS)
{
    DAO::instance().WriteIntParam("saveInt",timeS);
    m_save_intS = timeS;
    return true;
}

bool Config::SetDataDir(QString path)
{
    DAO::instance().WriteStringParam("data_dir",path);
    m_data_dir = path;
    return true;
}

bool Config::SetHostName(QString host)
{
    DAO::instance().WriteStringParam("host_name",host);
    m_host_name = host;
    return true;
}
