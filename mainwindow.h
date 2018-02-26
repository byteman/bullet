#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QTreeWidgetItem>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <math.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_grid.h>
#include <qwt_scale_draw.h>
#include <QTcpServer>
#include <QListWidgetItem>
#include "server.h"
#include "udpserver.h"
#include "sessmessage.h"
#include "devicemanager.h"
#include "wavewidget.h"
#include "protomessage.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setupRealtimeDataDemo(QwtPlot *qwtplot);


    void ShowDeviceChannel(quint32 dev_id, QString file, int chan);
private:
    Ui::MainWindow *ui;
    QVector<double> xdata;
    QVector<double> ydata;
    QTimer updateTimer;
    QString demoName;
    QwtPlotCurve *curve ;
    UdpServer srv;
    DeviceManager dvm;
    QIcon icon_device[2];
    QIcon icon_channel;
    QIcon icon_dir;
    QIcon icon_file;
    QMenu* menu;
    double getData(double inteval);


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
private slots:
    void onNotify(QString msg);
    void on_menu_click(bool);
    void updatedataSlot();
    void Message(SessMessage msg);
    void onEnumFiles(Device* dev,MsgFileList files);
    void onReadPara(Device* dev, MsgDevicePara para);
    void onWritePara(Device* dev, bool result);
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

    void on_rb1_clicked();

    void on_rb2_clicked();

    void on_rb3_clicked();

    void on_rb4_clicked();

    void on_rb5_clicked();

    void on_rb6_clicked();

    void on_rb7_clicked();

    void on_rb8_clicked();

    void on_btnSavePara_clicked();

    void on_actionReset_triggered();

protected:
    void timerEvent(QTimerEvent *);
    quint32 m_cur_dev_id;
    WaveWidget *m_waveWdg;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *);
protected slots:
    void on_reset_menu_click(bool);
    void on_write_menu_click(bool);
    void on_list_files_menu_click(bool);
};

#endif // MAINWINDOW_H
