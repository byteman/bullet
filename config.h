﻿#ifndef CONFIG_H
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

    bool SetFtpHost(QString host);
    bool SetFtpUser(QString user);
    bool SetFtpPwd(QString pwd);
    bool SetFtpPort(int port);
    bool SetFtpBase(QString base);
    bool SetFileForamt(int format);
    bool SetCorpNameInx(int idx);
    bool SetCorpNameList(QStringList& list);
    bool DeleteCorpName(QString name);
    bool AddCorpName(QString name);
    bool SetCorpName(QString name);
    bool SetPressDir(QString path);
    bool m_use_sys_time;
    bool m_recv_sensor_off; //是否接收传感器脱落后的重量值.
    int  m_local_port; //本机监听端口.
    int  m_rt_wave_min; //实时波形时间
    bool  m_enable_buffer; //运行buff db
    int  m_buf_num;
    int  m_buf_time;
    int  m_alarm_level;
    Lang  m_lang; //语言类型.
    int  m_save_intS; //本地保存时间间隔.
    QString m_data_dir;
    QString m_csv_dir;
    QString m_host_name; //主机名称.
    QString m_ftp_host;
    QString m_ftp_user;
    QString m_ftp_pwd;
    QString m_ftp_base;
    QString m_corp_name;
    QStringList m_corp_list;
    int     m_corp_index;
    int     m_ftp_port;
    bool    m_ftp_enable;
    QString m_report_name;
    unsigned char     m_file_format; //文件格式.

public:
    bool EnableRecvSensorOff(bool en);
};

#endif // CONFIG_H
