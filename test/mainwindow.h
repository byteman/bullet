#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include "../protoparser.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnStart_clicked();

    void on_btnStop_clicked();

private:
    Ui::MainWindow *ui;
    QUdpSocket socket;
    quint16 serial_id;
    quint16 session_id;
    ProtoParser parser;

    // QObject interface
    void sendPacket(quint8 cmd, QByteArray &data);

    void sendPacket(quint8 cmd, quint16 sess, QByteArray &data);
    void parseMsg(QByteArray &msg, QHostAddress sender, quint16 senderPort);
    void addLog(QString str);
protected:
    void timerEvent(QTimerEvent *);
protected slots:
    void readPendingDatagrams();
};

#endif // MAINWINDOW_H
