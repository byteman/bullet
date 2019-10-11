#ifndef DIALOGDEVICE_H
#define DIALOGDEVICE_H

#include <QDialog>

namespace Ui {
class DialogDevice;
}

class DialogDevice : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDevice(QWidget *parent = NULL);
    void UpdateDeviceInfo(QString serialNo,QString name);
    bool GetDeviceInfo(QString &serialNo,QString &name);
    ~DialogDevice();

private slots:
    void on_btnOK_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::DialogDevice *ui;
    QString _serialNo;
    QString _deviceName;
};

#endif // DIALOGDEVICE_H
