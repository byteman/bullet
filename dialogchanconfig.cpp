#include "dialogchanconfig.h"
#include "ui_dialogchanconfig.h"
#include "dao.h"
#include "myhelper.h"
#include <QInputDialog>
DialogChanConfig::DialogChanConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChanConfig)
{
    ui->setupUi(this);
  //  ui->edtMaxValue->setValidator(new QIntValidator(-100000, 10000, this));
   // ui->edtMinValue->setValidator(new QIntValidator(-100000, 10000, this));
    ui->edtChan->setEnabled(false);
}

DialogChanConfig::~DialogChanConfig()
{
    delete ui;
}

void DialogChanConfig::SetChanConfig(QString serialNo,QString name,int chan,DeviceChnConfig &cfg)
{
    ui->edtDevName->setText(serialNo);
    ui->edtChan->setText(QString("%1").arg(chan));
    ui->edtChanName->setText(cfg.chanName);
    devName = name;
    ui->edtMaxValue->setText(QString("%1").arg(cfg.maxValue));
    ui->edtMinValue->setText(QString("%1").arg(cfg.minValue));
}

bool DialogChanConfig::GetChanConfig(DeviceChnConfig &cfg)
{
    cfg = this->cfg;
    return  true;
}
bool DialogChanConfig::CheckPassWord()
{
    if("123456" != QInputDialog::getText(this,
                          QStringLiteral("权限验证"),
                          QStringLiteral("请输入密码")))
    {
        return false;
    }
    return true;
}
void DialogChanConfig::on_btnOK_clicked()
{
    cfg.maxValue = ui->edtMaxValue->text().toInt();
    cfg.minValue = ui->edtMinValue->text().toInt();
    cfg.chanName = ui->edtChanName->text();
    this->accept();
}

void DialogChanConfig::on_pushButton_2_clicked()
{

    this->reject();
}
bool DialogChanConfig::IsOk(QString title,QString message)
{

    int result = myHelper::ShowMessageBoxQuesion(message);
    if (result == 1) {
       return true;
    } else {
       return false;
    }
}
void DialogChanConfig::on_btnChanClear_clicked()
{


    bool ok = false;
    QString id = ui->edtDevName->text();
    int chan = ui->edtChan->text().toInt(&ok);
    if(!IsOk("",QString(QStringLiteral("确认清空设备名%1的%2通道数据吗?")).arg(devName).arg(ui->edtChan->text())))
    {
        return;
    }
    if(!CheckPassWord()){
        return;
    }
    if(ok && id.length() > 0){
        //清空数据
        QSqlError err = DAO::instance().DeviceChanDataRemove(id,chan);
        if(err.isValid()){
            myHelper::ShowMessageBoxError(err.text());
        }else{
            myHelper::ShowMessageBoxInfo(QStringLiteral("清空通道数据成功"));
        }
    }

}
