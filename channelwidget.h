#ifndef CHANNEL_WIDGET_H
#define CHANNEL_WIDGET_H

#include <QWidget>
#include <QTime>
#include "wavewidget.h"
#include "models.h"
#include <QQueue>
#include "protomessage.h"
namespace Ui {
class ChannelWidget;
}

class ChannelWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChannelWidget(int addr,QWidget *parent = 0);

    ~ChannelWidget();
    void SetTitle(QString name);
    void SetOnline(bool online);
    void SetChanSetting(DeviceChnConfig& cfg);
    void SetRecState(bool paused);
    void SetTimeRange(int rangeS);
    void Timeout();
    void SetUnit(QString unit);
    void DisplayWeight(int weight, quint16 state, quint16 dot);
    void Show();
    void Reset();
    void ResetAlarm();
    bool IsZoom();
    void ClearDisplay();
    int  Addr();

    virtual void mouseDoubleClickEvent(QMouseEvent *);
signals:
    void onDoubleClick(int addr,bool zoom);
    void onConfigClick(int addr);
    void onPlayClick(int addr, bool played);
private:
    Ui::ChannelWidget *ui;

    void resetTimeout();
    void MaxAlarm(bool alarm);
    void MinAlarm(bool alarm);
    void AlarmCheck(double weight);
    QString m_name;
    int m_addr; //设备的通道地址.
    int m_timeout;
    bool m_paused;
    bool m_reset_alarm;
    int m_rt_wave_s;
    int m_min_value,m_max_value; //峰值的最大最小报警值.
    int m_mmax; //峰值
    QString m_unit; //显示的力单位
    bool m_zoom; //是否处于放大状态.


    // QObject interface

    void WriteValues(int &value);
    void disable(bool dis);
protected:
    virtual void timerEvent(QTimerEvent *);
private slots:
    void on_tbtSet_clicked();
    void on_tbtPlay_clicked(bool checked);
};

#endif // DEVWIDGET_H
