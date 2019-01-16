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
     QString m_cur_station;
     QTimer m_timer;
     GPServer* srv;
     void loadDeviceUI();
     void Init();
     QTreeWidgetItem *findItemById(QString id);
     void AddLog(QString msg);
     void simData();
private slots:
    void initForm();
    void buttonClick();
    void on_menu_click(bool);
    void on_write_menu_click(bool);
    void on_reset_menu_click(bool);
private slots:
    void Message(SessMessage s);
    void onNotify(QString msg);
    void on_btnMenu_Min_clicked();
    void onWaveMsg(Device*dev, MsgWaveData data);
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Close_clicked();
    void on_btnExit_clicked();
    void on_treeWidget_customContextMenuRequested(const QPoint &pos);
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
    // QWidget interface
    void DevOffline(Device *dev);
    void DevOnline(Device *dev);
    void on_mytime_out();
    void on_treeWidget_doubleClicked(const QModelIndex &index);

    void on_btnShou_clicked();

protected:
    virtual void resizeEvent(QResizeEvent *);
    void timerEvent(QTimerEvent *);

    // QWidget interface
protected:
    virtual void showEvent(QShowEvent *);

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *);
};

#endif // UIDEMO01_H
