#ifndef MAIN_WND_H
#define MAIN_WND_H
//#include "myping.h"
#include <QDialog>
#include "mydevices.h"
#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QTreeWidgetItem>
#include <QRadioButton>
#include <math.h>
#include <QTcpServer>
#include <QListWidgetItem>
#include "tcpserver.h"
#include "gpserver.h"
#include "sessmessage.h"
#include "devicemanager.h"
#include "wavewidget.h"
#include "protomessage.h"
#include "gpserver.h"
#include <QLabel>
#include "histwavewidget.h"
#include "myplottrace.h"
#include <QFileSystemWatcher>

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
    bool LoadWave(QString id, QVector<int> chan, qint64 from, qint64 to);
    bool GetCurrentDeviceId2Name(QString &name);
protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::MainWnd *ui;
    MyDevices* mydev;
    bool bQueryOrderState;
    bool bFirst;

     QString m_cur_dev_id;
     MultiDeviceDataMap m_ddl;
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

     void changeDevice(QString dev_id);
     void StartReceiver();
     bool InitDvm();

     void initUI();
     void Start();
     void reloadDeviceList();
     void refreshDeviceList();
     void loadSysConfig();
     void reloadDeviceList2();
     bool GetCurrentDeviceId2(QString &id);


     QTreeWidgetItem *findItemById2(QString id);



     bool CheckPassWord();
     void outputVer();
private slots:

    void on_write_menu_click(bool);
    void on_reset_menu_click(bool);
private slots:
    void Message(SessMessage s);
    void onNotify(QString msg);

    void on_btnMenu_Min_clicked();
    void onSensorMsg(Device*dev, MsgSensorData data);
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Close_clicked();
    void on_btnExit_clicked();
    void on_treeWidget_customContextMenuRequested(const QPoint &pos);
    void on_reset_count_click(bool);
    // QWidget interface
    void DevOffline(Device *dev);
    void DevOnline(Device *dev);


    void on_btnShou_clicked();

    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_add_device_click(bool);
    void on_modify_menu_click(bool);
    void on_remove_device_click(bool);

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);


    void on_edtPort_textChanged(const QString &arg1);


    void on_btnShou2_clicked();

    void on_treeWidget2_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_update_menu_click(bool);

    void on_btnHelp_clicked();


    void on_btnLocalIP_clicked();


protected:
    void timerEvent(QTimerEvent *);

    // QWidget interface
protected:

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *);
protected slots:
    void onResetResult(Device *, bool);
    void onReply(QString msg);
    void buttonClick();
};

#endif // UIDEMO01_H
