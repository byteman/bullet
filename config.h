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
    bool m_use_sys_time;
    int  m_local_port; //本机监听端口.
    int  m_rt_wave_min; //实时波形时间
    Lang  m_lang; //语言类型.
};

#endif // CONFIG_H
