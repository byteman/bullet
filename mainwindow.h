#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include <QLabel>
#include "mydevices.h"
namespace Ui {
class MainWindow;
}
class GPServer;
class QCustomPlot;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setupRealtimeDataDemo(QCustomPlot *qwtplot);


    void ShowDeviceChannel(quint32 dev_id, QString file, int chan);
    void removeFiles();
    void MyListFiles(int id, int page);
private:
    Ui::MainWindow *ui;
    MyDevices *devices;
    double m_max,m_min;
    QVector<double> xdata;
    QVector<double> ydata;
    QTimer updateTimer;
    QString demoName;
    //UdpServer srv;
    GPServer *srv;
    DeviceManager dvm;
    QIcon icon_device[2];
    QIcon icon_channel;
    QIcon icon_dir;
    QIcon icon_file;
    QMenu* menu;
    double getData(double inteval);
    int m_debug_bytes;
    int m_refresh_count;

    QString FormatHex(QByteArray &data);
    void loadDeviceUI();
    QTreeWidgetItem *findItemById(quint32 id);
    void paintWave(MsgWaveData &wvd, int chan);
    void ShowDeviceChannel(quint32 dev_id, int chan);

    void listFiles(quint32 dev_id);
    void readParam(quint32 dev_id);
    QString formatIpaddr(sIP_ADDR &ipaddr);
    void toIpAddr(QString ipstr, sIP_ADDR &addr);
    void toString(QString str, INT8U *dest, int size);
    void toInt16U(QString str, INT16U &dest);
    void toInt8U(QString str, INT8U &dest);
    void toInt32U(QString str, quint32 &dest);
    QString formatTime();
    void checkAll(bool checked);
    void isAllCheck();
    void setCurrentStation(int index);
    void InitDevice(QList<Device*> &devices);

    QString GetDevice(int ch);
private slots:
    void onCalibResult(Device* dev, int chan, int index, int result);
    void onRealTimeResult(Device* dev,RT_AD_RESULT result);
    void onWaveMsg(Device*dev, MsgWaveData data);
    void onNotify(QString msg);
    void on_menu_click(bool);
    void Message(SessMessage msg);
    void onEnumFiles(Device* dev,ENUM_FILE_RESP files);
    void onReadPara(Device* dev, MsgDevicePara para);
    void onWritePara(Device* dev, bool result);
    void onWaveProgress(Device* dev, QString progress);
    //void on_btnStart_clicked();
    void on_btnStop_clicked();
    void on_btnReadWave_clicked();
    void DevOffline(Device* dev);
    void DevOnline(Device* dev);

    // QObject interface
    void on_treeWidget_customContextMenuRequested(const QPoint &pos);

    void on_btnSaveWave_clicked();

    void on_btnWaveStart_clicked();

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_actionStation1_triggered();

    void on_actionStation2_triggered();

    void on_listWidget_itemClicked(QListWidgetItem *item);



    void on_btnSavePara_clicked();

    void on_actionReset_triggered();
    void on_rb1_clicked(bool checked);

    void on_rball_clicked(bool checked);

    void on_rb2_clicked(bool checked);

    void on_rb3_clicked(bool checked);

    void on_rb4_clicked(bool checked);

    void on_rb5_clicked(bool checked);

    void on_rb6_clicked(bool checked);

    void on_rb7_clicked(bool checked);

    void on_rb8_clicked(bool checked);

    void on_btnStart_clicked();

    void on_btnStop_2_clicked();



    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_btnStartSend_clicked();

    void on_btnStopRead_clicked();

    void on_btnClear_clicked();

    void on_btnPause_clicked();

    void on_btnCalibZero_clicked();

    void on_btnCalibWet_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_cbxCurPage_currentIndexChanged(int index);

    void on_btnGo_clicked();

    void on_btnDelAll_clicked();

    void on_listFile_itemDoubleClicked(QListWidgetItem *item);

    void on_treeWidget_itemActivated(QTreeWidgetItem *item, int column);

    void on_btnResetZero_clicked();

    void on_action3_triggered();

    void on_action4_triggered();

    void on_action5_triggered();

    void on_actionW_triggered();

    void on_actionW_2_triggered();

    void on_action2_triggered();

    void on_action3_2_triggered();

    void on_action3_3_triggered();

    void on_treeWidget_clicked(const QModelIndex &index);

    void on_treeWidget_itemChanged(QTreeWidgetItem *item, int column);

protected:
    void timerEvent(QTimerEvent *);
    quint32 m_cur_page;
    quint32 m_cur_dev_id;
    int m_cur_station;
    WaveWidget *m_waveWdg;
    bool pause ;
    int m_dot;
    int m_full;
    QTimer m_timer;
    QMap<int,int> m_chans;
    QMap<int,QLabel*>  chanels;
    QMap<int,QAction*> stationActions;
    // QWidget interface
protected:
    void closeEvent(QCloseEvent *);
protected slots:
    void onCommResult(Device* dev,int cmd, int result);
    void on_mytime_out();
    void on_reset_menu_click(bool);
    void on_write_menu_click(bool);
    void on_list_files_menu_click(bool);
    void on_download_wave(bool);
    void on_btnNext_clicked();
    void on_btnPrev_clicked();
    void on_remove_menu_click(bool);

    // QObject interface
    void onChannelClick(int addr);
public:
    virtual bool eventFilter(QObject *, QEvent *);
};

#endif // MAINWINDOW_H
