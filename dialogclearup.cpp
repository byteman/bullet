#include "dialogclearup.h"
#include "ui_dialogclearup.h"
#include "dao.h"
#include <QDateTime>
DialogClearUp::DialogClearUp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogClearUp)
{
    ui->setupUi(this);

}

DialogClearUp::~DialogClearUp()
{
    delete ui;
}

void DialogClearUp::SetSerialNo(QString dev)
{
    qint64 start,end,count;
    QSqlError err = DAO::instance().DeviceDataInfo(dev,start,end,count);
    m_serialNo = dev;
    //
    ui->lblstart->setText(QDateTime::fromMSecsSinceEpoch(start*1000).toString("yyyy-MM-dd hh:mm:ss"));
    ui->lblend->setText(QDateTime::fromMSecsSinceEpoch(end*1000).toString("yyyy-MM-dd hh:mm:ss"));
    ui->lblcount->setText(QString("%1").arg(count/8));
    ui->dtData->setDateTime(QDateTime::currentDateTime());

}

void DialogClearUp::on_btnExit_clicked()
{
    this->accept();
}

void DialogClearUp::on_btnClearUp_clicked()
{
    DAO::instance().DeviceDataRemove(m_serialNo,0,ui->dtData->dateTime().toMSecsSinceEpoch()/1000);
    DAO::instance().DbRecycle();
}
