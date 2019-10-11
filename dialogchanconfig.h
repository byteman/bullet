#ifndef DIALOGCHANCONFIG_H
#define DIALOGCHANCONFIG_H

#include <QDialog>
#include "models.h"
namespace Ui {
class DialogChanConfig;
}

class DialogChanConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChanConfig(QWidget *parent = NULL);
    ~DialogChanConfig();
    void SetChanConfig(QString serialNo,int chan,DeviceChnConfig& cfg);
    bool GetChanConfig(DeviceChnConfig& cfg);

private slots:
    void on_btnOK_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::DialogChanConfig *ui;
    DeviceChnConfig cfg;
};

#endif // DIALOGCHANCONFIG_H
