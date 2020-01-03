﻿#ifndef MAIN_WND_H
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
#include "ftpupload.h"
#include "mainwindow.h"
#include "httpclient.h"
#include <QJsonDocument>
#include <QProcess>
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
        QProcess* proc;
     MyDevices* devices;
    bool bQueryOrderState;
    bool bFirst;
    FtpUploadManger m_ftp;
     MyPlotTrace *m_tracer;
    //MyPing ping;
     QFutureWatcher<bool> *watcher;
     HistWaveWidget* wave;
     QString m_cur_dev_id;
     MultiDeviceDataMap m_ddl;
     DeviceManager dvm;
     QFileSystemWatcher* fileWatcher;
     QIcon icon_device[2];
     QVector<QCheckBox*> rbChanList;
     QIcon icon_channel;
     QIcon icon_dir;
     QIcon icon_file;
     Ajax   ajax;
     QMenu* menu;
     QMenu* menu2;
     QMenu* menu3;
     QString m_cur_station;
     QTimer m_timer;
     QRubberBand *rubberBand;
     QPoint rubberOrigin;
     GPServer* srv;
     MainWindow *ftp;
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
     void loadSysConfig();
     void reloadDeviceList2();
     bool GetCurrentDeviceId2(QString &id);
     void loadChannels();
     QVector<int> GetSelectChannel();

     QTreeWidgetItem *findItemById2(QString id);
     void SelectAll(bool en);
     void loadStateFile(bool create=true);
     QJsonDocument buildReportInputJson(QString order);
     QString buildReportInput(QString order);
     void updateOrderState();
     bool CheckPassWord();
     void outputVer();
     void updateTimes();
     QVector<int> GetMainSelectChannel();
     void updateTime();
     bool GetCurrentDeviceName(QString &name);
     bool IsOk(QString title, QString message);

     QString getFtpDir(QString order);
     QString parseDateTime(QString order);
     void report_reqeust(QString order);
     QString queryOrderState();
     void refreshOrderState(QString res);
     void StartServer();
     QString GetNetFile(QString temp,QString order,QString orderKey);
     bool SaveNetFile(QString netFile, QString localFile);
     void listOrders(QString host);
private slots:
    void chan_click(int chan);
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
    void on_treeWidget2_customContextMenuRequested(const QPoint &pos);
    void on_reset_count_click(bool);
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

    void on_sbWaveMin_valueChanged(int arg1);

    void on_edtPort_textChanged(const QString &arg1);

    void onWaveShow(int addr, bool zoom);
    void on_cbUseSysTime_stateChanged(int arg1);

    void on_btnQuery_clicked();

    void on_btnShou2_clicked();

    void on_dteFrom_dateChanged(const QDate &date);

    void handleLoadWaveFinished();
    void on_cbxTimeSpan_currentIndexChanged(int index);

    void on_treeWidget2_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void myMoveEvent(QMouseEvent *);
    void mouseRelease(QMouseEvent *event);
    void mousePress(QMouseEvent *event);
    void RestoreWave();
    void on_btnRestore_clicked();

    void on_chkMeasure_clicked(bool checked);

    void mouseDoubleClick(QMouseEvent *event);
    void on_sbSaveInt_valueChanged(int arg1);

    void on_update_menu_click(bool);
    void on_chkSelAll_clicked();

    void on_btnExport_clicked();

    void on_btnMerge_clicked();

    void on_get_count_click(bool);
    void on_btnHelp_clicked();

    void on_cbxHost_currentIndexChanged(const QString &arg1);

    void on_cbxTestNo_currentIndexChanged(const QString &arg1);

    void on_btnExecReport_clicked();

    void on_btnSelFile_clicked();

    void on_edtHost_textChanged(const QString &arg1);

    void on_chkMeasure_clicked();

    void on_chkSelAll_clicked(bool checked);

    void on_btnReload_clicked();

    void on_btnLocalIP_clicked();

    void on_btnPing_clicked();

    void on_btnClear_clicked();

    void on_chkSensorOff_clicked(bool checked);

    void on_rb6_clicked();

    void on_edtFtpHost_textChanged(const QString &arg1);

    void on_edtFtpName_textChanged(const QString &arg1);

    void on_edtFtpPwd_textChanged(const QString &arg1);

    void on_edtFtpBase_textChanged(const QString &arg1);

    void on_rb1_clicked();

    void on_btnStartAll_clicked();

    void on_btnStopAll_clicked();

    void on_cbxFileFormat_currentIndexChanged(int index);

    void on_btnImport_clicked();

    void on_btnOpenReport_clicked();

    //void on_bntCorpConfig_clicked();

    //void on_cbxCorp_currentIndexChanged(int index);

    void on_edtCorpName_textChanged(const QString &arg1);



    void on_fix_menu_click(bool);
    void onUsbImportProgress(int prog);
    void on_opendata_menu_click(bool);
protected:
    void timerEvent(QTimerEvent *);

    // QWidget interface
protected:

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *);
protected slots:
    void onUsbImportSucc();
    void onResetResult(Device *, bool);
    void on_report_click(QString order);
    void fileChange(const QString &path);
    void onProgress(QString serialNo,int prog,int err);
    void onSucc(QString serialNo,QString err);
    void on_clearup_menu_click(bool);
    void onReply(QString msg);
    void on_opendir_click(QString order);
    void on_stop_menu_click(bool);
    void on_start_menu_click(bool);
    void on_clear_history_menu_click(bool);
    void on_gen_report_response(AjaxResponse r);
    void on_query_report_response(AjaxResponse r);

    // QWidget interface
protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);
};

#endif // UIDEMO01_H
