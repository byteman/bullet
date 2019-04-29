#ifndef DIALOGUPDATE_H
#define DIALOGUPDATE_H

#include <QDialog>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "httpclient.h"
namespace Ui {
class DialogUpdate;
}

class DialogUpdate : public QDialog
{
    Q_OBJECT

public:
    explicit DialogUpdate(QWidget *parent = 0);
    ~DialogUpdate();
    void SetServerHost(QString addr);

private slots:

   void replyFinished(QNetworkReply*);
   void loadError(QNetworkReply::NetworkError);
   void loadProgress(qint64 bytesSent,qint64 bytesTotal);

    void on_btnStartUpdate_clicked();

    void on_btnSelectFile_clicked();
    void onResetFinished(AjaxResponse resp);
    void sigReplyMessage(int, QString);
    void on_btnExit_clicked();

    void on_edtDevIp_textChanged(const QString &arg1);

private:
    Ui::DialogUpdate *ui;
    QNetworkReply *reply;
    QFile *file;
    QString m_addr;
    Ajax ajax;
    QString GetResetURL();
};

#endif // DIALOGUPDATE_H
