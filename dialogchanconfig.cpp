#include "dialogchanconfig.h"
#include "ui_dialogchanconfig.h"

DialogChanConfig::DialogChanConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChanConfig)
{
    ui->setupUi(this);
    ui->edtMaxValue->setValidator(new QIntValidator(-100, 10000, this));
    ui->edtMinValue->setValidator(new QIntValidator(-100, 10000, this));
    ui->edtChan->setEnabled(false);
}

DialogChanConfig::~DialogChanConfig()
{
    delete ui;
}

void DialogChanConfig::SetChanConfig(QString serialNo,int chan,DeviceChnConfig &cfg)
{
    ui->edtDevName->setText(serialNo);
    ui->edtChan->setText(QString("%1").arg(chan));
    ui->edtMaxValue->setText(QString("%1").arg(cfg.maxValue));
    ui->edtMinValue->setText(QString("%1").arg(cfg.minValue));
}

bool DialogChanConfig::GetChanConfig(DeviceChnConfig &cfg)
{
    cfg = this->cfg;
    return  true;
}

void DialogChanConfig::on_btnOK_clicked()
{
    cfg.maxValue = ui->edtMaxValue->text().toInt();
    cfg.minValue = ui->edtMinValue->text().toInt();
    this->accept();
}

void DialogChanConfig::on_pushButton_2_clicked()
{

    this->reject();
}
