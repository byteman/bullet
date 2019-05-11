#include "dialogclearup.h"
#include "ui_dialogclearup.h"
#include "dao.h"
#include <QDateTime>
#include <QtConcurrent/QtConcurrent>

DialogClearUp::DialogClearUp(QWidget *parent) :
    QDialog(parent),
    watcher(NULL),
    ui(new Ui::DialogClearUp)
{
    ui->setupUi(this);
    if(watcher==NULL){
        watcher = new QFutureWatcher<bool>(this);
        connect(watcher, SIGNAL(finished()), this, SLOT(clearUpFinished()));
    }
}

DialogClearUp::~DialogClearUp()
{
    delete ui;
}
void DialogClearUp::clearUpFinished()
{
    ui->btnClearUp->setEnabled(true);
    ui->btnClearUp->setText("清理");
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
bool DialogClearUp::ClearUp(QString id,qint64 from,qint64 to)
{
    DAO::instance().DeviceDataRemove(id,from,to);
    DAO::instance().DbRecycle();
    return true;
}
void DialogClearUp::on_btnClearUp_clicked()
{

    const QFuture<bool> future = QtConcurrent::run(this,
                                                   &DialogClearUp::ClearUp,
                                                   m_serialNo,
                                                   0,
                                                   ui->dtData->dateTime().toMSecsSinceEpoch()/1000);
    watcher->setFuture(future);
    ui->btnClearUp->setEnabled(false);
    ui->btnClearUp->setText("正在清理，请等待..");
}
