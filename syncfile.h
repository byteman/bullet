#ifndef SYNCFILE_H
#define SYNCFILE_H

#include <QObject>
#include <QTimer>
#include <QFile>
#include "csvfile.h"
#include "session.h"
#include "protomessage.h"

class Device;
class SyncFile:public QObject
{
    Q_OBJECT
public:
    SyncFile();
    bool StartSync(Device* sess,QString file,int total);
    bool onMessage(ProtoMessage &req, ProtoMessage &resp);
    bool StopSync();
    bool isStarted();
    float progress();
signals:
    void finished();
    void cancel();

private:
    Device  *m_device;
    QString  m_file_name;
    int      m_cur;
    int      m_total;
    int      m_timeout;
    int      m_cancel_timeout;
    int      m_want;
    bool     m_quit;
    QTimer   m_timer;
    CSVFile *m_file;
    // QObject interface
    void SaveWave(ProtoMessage &msg);
    void readPacket();

    void OpenFile(QString fname);
    void CloseFile();
    void readPacket(int index, int count);
protected slots:
    void timeout();
};

#endif // SYNCFILE_H
