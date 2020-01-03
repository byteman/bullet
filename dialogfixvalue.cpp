#include "dialogfixvalue.h"
#include "ui_dialogfixvalue.h"
#include "dao.h"
#include <QMessageBox>
DialogFixValue::DialogFixValue(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFixValue)
{
    ui->setupUi(this);
    chanEdit.push_back(ui->lineEdit);
    chanEdit.push_back(ui->lineEdit_2);
    chanEdit.push_back(ui->lineEdit_3);
    chanEdit.push_back(ui->lineEdit_4);
    chanEdit.push_back(ui->lineEdit_5);
    chanEdit.push_back(ui->lineEdit_6);
    chanEdit.push_back(ui->lineEdit_7);
    chanEdit.push_back(ui->lineEdit_8);
    chanEdit.push_back(ui->lineEdit_9);
    chanEdit.push_back(ui->lineEdit_10);
    chanEdit.push_back(ui->lineEdit_11);
    chanEdit.push_back(ui->lineEdit_12);

}

DialogFixValue::~DialogFixValue()
{
    delete ui;
}

void DialogFixValue::SetDeviceId(QString id)
{
    device_id = id;
}

void DialogFixValue::on_btnFix_clicked()
{
    bool maxok = false;
    int maxv = ui->editMax->text().toInt(&maxok);
    if(maxok==false)maxv = 500;
    for(int i = 0; i < chanEdit.count();i++)
    {
        bool ok = false;
        int fix = chanEdit.at(i)->text().toInt(&ok);
        if(!ok){
            continue;
        }
        DAO::instance().ChannelValueFix(device_id,-1,-1,i+1,maxv,fix);
    }
    QMessageBox::information(this,QStringLiteral("提示"),QStringLiteral("修正完成"));

}

void DialogFixValue::on_edtComm_textChanged(const QString &arg1)
{

        for(int i = 0; i < chanEdit.count();i++)
        {
             chanEdit.at(i)->setText(arg1);
        }

}

void DialogFixValue::on_btnExit_clicked()
{
    this->close();
}
