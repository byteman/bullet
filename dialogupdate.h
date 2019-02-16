#ifndef DIALOGUPDATE_H
#define DIALOGUPDATE_H

#include <QDialog>
#include "alpostfile.h"
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
    void on_btnStartUpdate_clicked();

    void on_btnSelectFile_clicked();

    void sigReplyMessage(int, QString);
    void onReloadFinished(AjaxResponse);
private:
    Ui::DialogUpdate *ui;
    ALPostFile* post;
    Ajax ajax;
    QString m_addr;
};

#endif // DIALOGUPDATE_H
