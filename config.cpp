#include "config.h"
#include "dao.h"
#include "singletonholder.h"
#include <QDir>
#include <QSettings>
#include "utils.h"
Config::Config():
    m_recv_sensor_off(true)
{

}

Config &Config::instance()
{
    static SingletonHolder<Config> sh;
    return *sh.get();
}
bool Config::Init()
{
    QSettings config("bullet.ini", QSettings::IniFormat);

    config.setIniCodec("UTF-8");//添上这句就不会出现乱码了);
    m_enable_buffer= config.value("/config/db_buf_en",false).toBool();
    m_buf_num= config.value("/config/db_buf_len",80).toInt();
    m_buf_time= config.value("/config/db_buf_time",3000).toInt();
    m_ftp_user= config.value("/config/ftp_user","root").toString();
    m_ftp_password= config.value("/config/ftp_password","12345678").toString();



    m_recv_sensor_off= DAO::instance().ReadBoolParam("recv_sensor_off",true);
    m_use_sys_time = false;//DAO::instance().ReadBoolParam("use_sys_time",false);
    m_rt_wave_min  = DAO::instance().ReadIntParam("rt_wave_min",20);
    m_lang = (Lang)DAO::instance().ReadIntParam("lang",(int)LANG_ZH);
    m_local_port = DAO::instance().ReadIntParam("port",8881);
    m_save_intS= DAO::instance().ReadIntParam("saveInt",3);
    m_host_name= DAO::instance().ReadStringParam("host_name","24#2");
    m_data_dir= DAO::instance().ReadStringParam("data_dir",utils::GetWorkDir());
    return true;
}

bool Config::SetUseSysTime(bool use)
{
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
bool Config::EnableRecvSensorOff(bool en)
{
    DAO::instance().WriteBoolParam("recv_sensor_off",en);
    m_recv_sensor_off = en;
    return true;
}
bool Config::SetHostName(QString host)
{
    DAO::instance().WriteStringParam("host_name",host);
    m_host_name = host;
    return true;
}
