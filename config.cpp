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
    m_recv_sensor_off= DAO::instance().ReadBoolParam("recv_sensor_off",false);
    m_use_sys_time = false;//DAO::instance().ReadBoolParam("use_sys_time",false);
    m_rt_wave_min  = DAO::instance().ReadIntParam("rt_wave_min",20);
    m_lang = (Lang)DAO::instance().ReadIntParam("lang",(int)LANG_ZH);
    m_local_port = DAO::instance().ReadIntParam("port",8881);
    m_save_intS= DAO::instance().ReadIntParam("saveInt",3);
    m_host_name= DAO::instance().ReadStringParam("host_name","AI");
    m_data_dir= DAO::instance().ReadStringParam("data_dir",utils::GetWorkDir());
    m_ftp_host= DAO::instance().ReadStringParam("ftp_host","");
    m_ftp_user= DAO::instance().ReadStringParam("ftp_user","");
    m_ftp_pwd= DAO::instance().ReadStringParam("ftp_pwd","");
    m_ftp_port = DAO::instance().ReadIntParam("ftp_port",21);
    m_report_name =  config.value("/config/report","PressReportd").toString();
    QString ftpbase = QStringLiteral("//172.30.201.210/电芯研究院/测试数据库/2 电芯测试/22.测试数据/2.循环测试报告");

    m_ftp_base = DAO::instance().ReadStringParam("ftp_base",ftpbase);
    if(m_ftp_host.length() < 3){
        m_ftp_enable = false;

    }else{
        m_ftp_enable = true;
    }
    m_file_format = DAO::instance().ReadIntParam("format",1);
    if(m_file_format > 1 ){
        m_file_format = 1;
    }
    m_corp_index = DAO::instance().ReadIntParam("corpIdx",0);
    qDebug() << "m_corp_index = " <<m_corp_index;
    m_corp_name = DAO::instance().ReadStringParam("corpName",QStringLiteral("光明A1-实验室"));

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

bool Config::SetFtpHost(QString host)
{
    DAO::instance().WriteStringParam("ftp_host",host);
    m_ftp_host = host;
    if(m_ftp_host.length() < 3){
        m_ftp_enable = false;
    }else{
        m_ftp_enable = true;
    }
    return true;
}

bool Config::SetFtpUser(QString user)
{
    DAO::instance().WriteStringParam("ftp_user",user);
    m_ftp_user = user;
    return true;
}

bool Config::SetFtpPwd(QString pwd)
{
    DAO::instance().WriteStringParam("ftp_pwd",pwd);
    m_ftp_pwd = pwd;
    return true;
}

bool Config::SetFtpPort(int port)
{
    DAO::instance().WriteIntParam("ftp_port",port);
    m_ftp_port = port;
    return true;
}

bool Config::SetFtpBase(QString base)
{
    DAO::instance().WriteStringParam("ftp_base",base);
    m_ftp_base = base;
    return true;
}

bool Config::SetFileForamt(int format)
{
    DAO::instance().WriteIntParam("format",format);
    m_file_format = format;
    return true;
}

bool Config::SetCorpNameInx(int idx)
{
    DAO::instance().WriteIntParam("corpIdx",idx);
    m_corp_index = idx;
    return true;
}

bool Config::SetCorpNameList(QStringList &list)
{
    DAO::instance().WriteStringParam("corpList",list.join(","));
    m_corp_list = list;
    return true;
}
//删除一个公司名称.
bool Config::DeleteCorpName(QString name)
{
    //没有这个名字.
    if(!m_corp_list.contains(name)){
        return true;
    }
    m_corp_list.removeOne(name);
    return SetCorpNameList(m_corp_list);
}
//添加一个公司名
bool Config::AddCorpName(QString name)
{
    if(m_corp_list.contains(name)){
        return true;
    }
    m_corp_list.push_back(name);
    return SetCorpNameList(m_corp_list);
}

bool Config::SetCorpName(QString name)
{
    m_corp_name = name;
    DAO::instance().WriteStringParam("corpName",name);
    return true;
}
