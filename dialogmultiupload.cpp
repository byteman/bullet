#include "dialogmultiupload.h"
#include "ui_dialogmultiupload.h"
#include "devicemanager.h"
#include "qftp.h"
DialogMultiUpload::DialogMultiUpload(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMultiUpload)
{
    ui->setupUi(this);
    connect(&ftp,SIGNAL(OnSuccess(QString)),this,SLOT(OnSuccess(QString)));
    connect(&ftp,SIGNAL(OnError(QString,QString)),this,SLOT(OnError(QString,QString)));

}

DialogMultiUpload::~DialogMultiUpload()
{

    delete ui;
}

void DialogMultiUpload::ListDevice(QStringList &list)
{
    m_list= list;
    ui->comboBox->clear();
    ui->comboBox->addItems(list);
}

void DialogMultiUpload::on_btnStart_clicked()
{
    ftp.Auth(21,"root","admin");
    for(int i =0 ; i <m_list.size(); i++)
    {  
        ftp.AddTask(ui->edtLocalFile->text(),"10.10.10.2",ui->edtTarget->text());
    }

}

void DialogMultiUpload::upload(QString host,QString scrName,QString dstfileName)
{


}
void DialogMultiUpload::timerEvent(QTimerEvent *event)
{
}
#include <QFileDialog>
void DialogMultiUpload::on_btnStart_2_clicked()
{
    QString file = QFileDialog::getOpenFileName(this);
    ui->edtLocalFile->setText(file);
}

void DialogMultiUpload::OnSuccess(QString host)
{

    QListWidgetItem* item = new QListWidgetItem(QString("%1 succ").arg(host));
    item->setTextColor(QColor(0x00,0xFF,0));
    ui->listWidget->addItem(item);
}

void DialogMultiUpload::OnError(QString host, QString reason)
{
    QListWidgetItem* item = new QListWidgetItem(QString("%1-> %2").arg(host).arg(reason));
    item->setTextColor(QColor(0xff,0,0));
    ui->listWidget->addItem(item);

}
