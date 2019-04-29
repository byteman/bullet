#include "dialogupdate.h"
#include "ui_dialogupdate.h"
#include <QDebug>
#include <QMessageBox>
DialogUpdate::DialogUpdate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogUpdate)
{
    ui->setupUi(this);
    ui->pbUpdate->hide();

}

DialogUpdate::~DialogUpdate()
{
    delete ui;
}
void DialogUpdate::sigReplyMessage(int code ,QString msg)
{

}
void DialogUpdate::SetServerHost(QString addr)
{

    //m_addr = QString("ftp://%1").arg(addr);
    m_addr=addr;
    ui->edtDevIp->setText(addr);
    qDebug() << "url=" << m_addr;

}
QString DialogUpdate::GetResetURL()
{
    return QString("http://%1/reset").arg(m_addr);
}
void DialogUpdate::replyFinished(QNetworkReply *)
{
        if(reply->error() == QNetworkReply::NoError)
        {
            reply->deleteLater();
            file->flush();
            file->close();
            ui->pbUpdate->hide();
            qDebug() << "finished";
            QJsonDocument json;

            ajax.post(GetResetURL(),json,this,SLOT(onResetFinished(AjaxResponse)));

        }
        else
        {
            QMessageBox::critical(NULL, tr("Error"), "Failed!!!");
        }

}

void DialogUpdate::loadError(QNetworkReply::NetworkError err)
{
    qDebug()<<"Error: "<<err;
}

void DialogUpdate::loadProgress(qint64 bytesSent, qint64 bytesTotal)
{
        qDebug() << "loaded" << bytesSent << "of" << bytesTotal;
      ui->pbUpdate->setMaximum(bytesTotal); //最大值
      ui->pbUpdate->setValue(bytesSent);  //当前值
      if(bytesSent >= bytesTotal){

      }

}

void DialogUpdate::on_btnStartUpdate_clicked()
{
       file = new QFile(ui->edtFileName->text());
       if(!file->open(QIODevice::ReadOnly)){
           QMessageBox::information(this,"info",QStringLiteral("文件打开失败"));
           return ;
       }
       QByteArray byte_file = file->readAll();

       QNetworkAccessManager *accessManager = new QNetworkAccessManager(this);
       accessManager->setNetworkAccessible(QNetworkAccessManager::Accessible);
       QUrl url(QString("ftp://%1/fridge/fridge").arg(m_addr));
       url.setPort(21);
       url.setUserName("root");
       url.setPassword("admin");
        qDebug() << "url===>" << url.toString();
       QNetworkRequest request(url);
       reply = accessManager->put(request, byte_file);


        ui->pbUpdate->setValue(0);
        ui->pbUpdate->show();

       connect(accessManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));
       connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(loadError(QNetworkReply::NetworkError)));
       connect(reply, SIGNAL(uploadProgress(qint64 ,qint64)), this, SLOT(loadProgress(qint64 ,qint64)));

}
#include <QFileDialog>
void DialogUpdate::on_btnSelectFile_clicked()
{
    //QFileDialog dlg;

    QString file = QFileDialog::getOpenFileName(this,"open file",QString(),"fridge");

    ui->edtFileName->setText(file);

}

void DialogUpdate::onResetFinished(AjaxResponse resp)
{
    QMessageBox::information(this,
                             QString(QStringLiteral("title")),
                             QString(QStringLiteral("Successful")));
}

void DialogUpdate::on_btnExit_clicked()
{

}


void DialogUpdate::on_edtDevIp_textChanged(const QString &arg1)
{
    m_addr = arg1;
}
