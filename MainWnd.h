#ifndef MAIN_WND_H
#define MAIN_WND_H

#include <QDialog>
#include "mydevices.h"
#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QTreeWidgetItem>
#include <math.h>
#include <QTcpServer>
#include <QListWidgetItem>
#include "tcpserver.h"
#include "udpserver.h"
#include "sessmessage.h"
#include "devicemanager.h"
#include "wavewidget.h"
#include "protomessage.h"
#include "gpserver.h"
#include <QLabel>
namespace Ui {
class MainWnd;
}

class MainWnd : public QDialog
{
    Q_OBJECT

public:
    explicit MainWnd(QWidget *parent = 0);
    ~MainWnd();

    bool GetCurrentDeviceId(QString &id);
protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
        Ui::MainWnd *ui;
     MyDevices* devices;
     WaveWidget* wave;
     QString m_cur_dev_id;
     DeviceManager dvm;
     QIcon icon_device[2];
     QIcon icon_channel;
     QIcon icon_dir;
     QIcon icon_file;
     QMenu* menu;
     QMenu* menu2;
     QString m_cur_station;
     QTimer m_timer;
     GPServer* srv;
     void loadDeviceUI();
     bool Init();
     QTreeWidgetItem *findItemById(QString id);
     void AddLog(QString msg);
     void simData();
     void changeDevice(QString dev_id);
     void StartReceiver();
     bool InitDvm();
     void initDeviceChannels();
     void initUI();
     void Start();
     void reloadDeviceList();
     void refreshDeviceList();
private slots:

    void buttonClick();
    void on_menu_click(bool);
    void on_write_menu_click(bool);
    void on_reset_menu_click(bool);
private slots:
    void Message(SessMessage s);
    void onNotify(QString msg);
    void onChannelClick(int addr);
    void on_btnMenu_Min_clicked();
    void onSensorMsg(Device*dev, MsgSensorData data);
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Close_clicked();
    void on_btnExit_clicked();
    void on_treeWidget_customContextMenuRequested(const QPoint &pos);

    // QWidget interface
    void DevOffline(Device *dev);
    void DevOnline(Device *dev);


    void on_btnShou_clicked();

    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_add_device_click(bool);
    void on_modify_menu_click(bool);
    void on_remove_device_click(bool);
    void onPlayClick(int addr, bool played);
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

protected:
    void timerEvent(QTimerEvent *);

    // QWidget interface
protected:

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *);
protected slots:
    void onResetResult(Device *, bool);
};

#endif // UIDEMO01_H
