#ifndef CONFIG_H
#define CONFIG_H
#include <QString>
#include "models.h"

enum Lang{
    LANG_ZH=0,
    LANG_EN,
};
struct Config
{
    Config();
    static Config& instance();
    bool Init();
    bool SetUseSysTime(bool use);
    bool SetRtWaveMin(int min);
    bool SetLocalPort(int port);
    bool SetSaveInt(int timeS);
    bool SetDataDir(QString path);
    bool SetHostName(QString host);
    bool m_use_sys_time;
    bool m_recv_sensor_off; //是否接收传感器脱落后的重量值.
    int  m_local_port; //本机监听端口.
    int  m_rt_wave_min; //实时波形时间
    bool  m_enable_buffer; //运行buff db
    int  m_buf_num;
    int  m_buf_time;
    Lang  m_lang; //语言类型.
    int  m_save_intS; //本地保存时间间隔.
    QString m_data_dir;
    QString m_host_name; //主机名称.
    QString m_ftp_user;
    QString m_ftp_password;


public:
    bool EnableRecvSensorOff(bool en);
};

#endif // CONFIG_H
