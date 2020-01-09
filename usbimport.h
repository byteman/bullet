#ifndef USBIMPORT_H
#define USBIMPORT_H
#include <QThread>
#include <QFutureWatcher>
//从u盘数据中导入数据，并提供一个进度指示.
class UsbImport:public QObject
{
    Q_OBJECT
public:
    UsbImport();
    static UsbImport* instance();
    //启动导入操作.
    bool start(QString serial,QString dir);

signals:
    void onProgress(int prog);
    void onSucc(int total);
    void onError(QString err);

private:
    QFutureWatcher<bool> *watcher;
    bool _run();
    int m_total;
    QString m_dir;
    QString m_serial_no;
    bool parseFile(QString file);
};

#endif // USBIMPORT_H
