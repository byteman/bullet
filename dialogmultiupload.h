#ifndef DIALOGMULTIUPLOAD_H
#define DIALOGMULTIUPLOAD_H

#include <QDialog>
#include <QStringList>
#include "ftpupload.h"
namespace Ui {
class DialogMultiUpload;
}

class DialogMultiUpload : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMultiUpload(QWidget *parent = nullptr);
    ~DialogMultiUpload();
    void ListDevice(QStringList& list);

private slots:
    void on_btnStart_clicked();

    void on_btnStart_2_clicked();
    void OnSuccess(QString host);
    //某个文件上传失败
    void OnError(QString host,QString reason);
private:
    Ui::DialogMultiUpload *ui;
    QStringList m_list;
    //QFtp *ftp;
    FtpUploadManger ftp;
    // QObject interface
    void upload(QString host, QString scrName, QString dstfileName);
protected:
    virtual void timerEvent(QTimerEvent *event);
};

#endif // DIALOGMULTIUPLOAD_H
