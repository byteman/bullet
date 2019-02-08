#include "dialogdevice.h"
#include "ui_dialogdevice.h"

DialogDevice::DialogDevice(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDevice)
{
    ui->setupUi(this);
}

void DialogDevice::UpdateDeviceInfo(QString serialNo, QString name)
{
    _serialNo = serialNo;
    _deviceName=name;
    ui->edtSerialNo->setEnabled(false);
    ui->edtDevName->setText(name);
    ui->edtSerialNo->setText(serialNo);
}

bool DialogDevice::GetDeviceInfo(QString &serialNo, QString &name)
{
    serialNo = _serialNo;
    name = _deviceName;
    return true;
}

DialogDevice::~DialogDevice()
{
    delete ui;
}

void DialogDevice::on_btnOK_clicked()
{
    _deviceName = ui->edtDevName->text();
    _serialNo = ui->edtSerialNo->text();
    this->accept();
}

void DialogDevice::on_pushButton_2_clicked()
{
    this->reject();
}
