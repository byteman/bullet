#ifndef CHANNEL_WIDGET_H
#define CHANNEL_WIDGET_H

#include <QWidget>
#include <QTime>
#include "wavewidget.h"
namespace Ui {
class ChannelWidget;
}

class ChannelWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChannelWidget(int addr,QWidget *parent = 0);

    ~ChannelWidget();
    void Timeout();
    void SetUnit(QString unit);
    QString DisplayWeight(int weight, quint16 state, quint16 dot);
    void Show();
    void Reset();
    virtual void mouseDoubleClickEvent(QMouseEvent *);
signals:
    void onDoubleClick(int addr,bool zoom);
    void onConfigClick(int addr);
private:
    void SetOnline(bool online);
    Ui::ChannelWidget *ui;
    int m_addr;
    int m_timeout;
    int m_still_time_ms;
    QTime m_still_time;
    bool m_last_still;
    QString m_unit;
    bool m_zoom;
    QFont font;
    WaveWidget* m_waveWdg;

    // QObject interface
    void clearState();
    void resetTimeout();

    // QObject interface
protected:
    virtual void timerEvent(QTimerEvent *);
private slots:
    void on_tbtSet_triggered(QAction *arg1);
    void on_tbtSet_clicked();
};

#endif // DEVWIDGET_H
