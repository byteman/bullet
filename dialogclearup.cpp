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
    SetSerialNo(m_serialNo);
    ui->btnClearUp->setEnabled(true);
    ui->btnClearUp->setText(QStringLiteral("清理"));

}
void DialogClearUp::SetSerialNo(QString dev)
{
    qint64 start,end,count;
    QSqlError err = DAO::instance().DeviceDataInfo(dev,start,end,count);
    if(err.isValid() || (start == 0 && end == 0)){
        start = QDateTime::currentMSecsSinceEpoch() / 1000;
        end = start;
        count = 0;
    }
    m_serialNo = dev;
    //
    ui->lblstart->setText(QDateTime::fromMSecsSinceEpoch(start*1000).toString("yyyy-MM-dd hh:mm:ss"));
    ui->lblend->setText(QDateTime::fromMSecsSinceEpoch(end*1000).toString("yyyy-MM-dd hh:mm:ss"));
    ui->lblcount->setText(QString("%1").arg(count));
    ui->dtData->setDateTime(QDateTime::currentDateTime());

}

void DialogClearUp::on_btnExit_clicked()
{
    this->accept();
}
bool DialogClearUp::ClearUp(QString id,qint64 from,qint64 to)
{
    QSqlError err =DAO::instance().DeviceDataRemove(id,from,to);
    if(err.isValid()){
        qDebug() << err.text();

    }
//    err =DAO::instance().DbRecycle(id);
//    if(err.isValid()){
//        qDebug() << err.text();
//    }
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
    ui->btnClearUp->setText(QStringLiteral("正在清理，请等待.."));
}

void DialogClearUp::on_btnClearAll_clicked()
{
    DAO::instance().DeviceDataRemove(m_serialNo);
    SetSerialNo(m_serialNo);
    //DAO::instance().CreateDataTable(m_serialNo);
}
