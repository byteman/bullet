#include "config.h"
#include "dao.h"
#include "singletonholder.h"
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
    m_use_sys_time = DAO::instance().ReadBoolParam("use_sys_time",true);
    m_rt_wave_min  = DAO::instance().ReadIntParam("rt_wave_min",20);
    m_lang = (Lang)DAO::instance().ReadIntParam("lang",(int)LANG_ZH);
    m_local_port = DAO::instance().ReadIntParam("port",8881);
    return true;
}

bool Config::SetUseSysTime(bool use)
{
    DAO::instance().WriteIntParam("use_sys_time",use);
    m_use_sys_time = use;
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
