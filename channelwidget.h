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
    void SetChanSetting(DeviceChnConfig& cfg);
    void SetRecState(bool paused);
    void SetTimeRange(int rangeS);
    void Timeout();
    void SetUnit(QString unit);
    void DisplayWeight(int weight, quint16 state, quint16 dot);
    void Show();
    void Reset();
    bool IsZoom();
    void ClearDisplay();
    int  Addr();
    QQueue<SensorData>& GetHistoryData();
    void DisplayDataQueue(QQueue<SensorData> &dataQueue);
    virtual void mouseDoubleClickEvent(QMouseEvent *);
signals:
    void onDoubleClick(int addr,bool zoom);
    void onConfigClick(int addr);
    void onPlayClick(int addr, bool played);
private:
    Ui::ChannelWidget *ui;
    void SetOnline(bool online);
    void resetTimeout();
    void MaxAlarm(bool alarm);
    void MinAlarm(bool alarm);
    void AlarmCheck(int weight);
    QString m_name;
    int m_addr; //设备的通道地址.
    int m_timeout;
    bool m_paused;
    int m_rt_wave_s;
    int m_min_value,m_max_value; //最大最小报警值.
    QString m_unit; //显示的力单位
    bool m_zoom; //是否处于放大状态.
    WaveWidget* m_waveWdg; //波形控件.
    QQueue<SensorData> m_histData;

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
