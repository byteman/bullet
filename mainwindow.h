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
    QMenu* menu;
    double getData(double inteval);


    QString FormatHex(QByteArray &data);
    void loadDeviceUI();
    QTreeWidgetItem *findItemById(quint32 id);
    void paintWave(MsgWaveData &wvd, int chan);
    void ShowDeviceChannel(quint32 dev_id, int chan);
private slots:
    void onNotify(QString msg);
    void on_menu_click(bool);
    void updatedataSlot();
    void Message(SessMessage msg);
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

protected:
    void timerEvent(QTimerEvent *);
    quint32 m_cur_dev_id;
    WaveWidget *m_waveWdg;
};

#endif // MAINWINDOW_H
