#ifndef DIALOGPARAMS_H
#define DIALOGPARAMS_H

#include <QDialog>
#include "devicemanager.h"
#include "device.h"
namespace Ui {
class DialogParams;
}

class DialogParams : public QDialog
{
    Q_OBJECT

public:
    explicit DialogParams(DeviceManager* _dvm,QString id, QWidget *parent = 0);
    ~DialogParams();

protected slots:
    void onReadPara(Device *, MsgDevicePara);
private slots:
    void onWritePara(Device *, bool);
    void on_buttonBox_accepted();

    void on_btnSave_clicked();

    void on_btnExit_clicked();

    void on_btnRead_clicked();

private:
    Ui::DialogParams *ui;
    DeviceManager* dvm;
    QString currId;

    void toIpAddr(QString ipstr, sIP_ADDR &addr);
    void toString(QString str, INT8U *dest, int size);
    void toInt16U(QString str, INT16U &dest);
    void toInt8U(QString str, INT8U &dest);
    void toInt32U(QString str, quint32 &dest);
    void writeParam();
};

#endif // DIALOGPARAMS_H
