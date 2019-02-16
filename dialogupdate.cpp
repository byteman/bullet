#include "dialogupdate.h"
#include "ui_dialogupdate.h"
#include <QDebug>
DialogUpdate::DialogUpdate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogUpdate)
{
    ui->setupUi(this);
    post = new ALPostFile();
    connect(post,SIGNAL(sigReplyMessage(int,QString)),this,SLOT(sigReplyMessage(int,QString)));
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

    m_addr = QString("http://%1/upload").arg(addr);
    post->setServerAddr(m_addr);
    qDebug() << "url=" << m_addr;

}
void DialogUpdate::onReloadFinished(AjaxResponse res)
{
    if(res.error){
        qDebug() <<"code=" <<  res.status << " msg=" <<res.data;
    }
}
void DialogUpdate::on_btnStartUpdate_clicked()
{
    //QString path = ui->edtFileName->text();
    //ajax.postFile(m_addr,path,this,SLOT(onReloadFinished(AjaxResponse)));


    post->setPostFilePath(ui->edtFileName->text());
}
#include <QFileDialog>
void DialogUpdate::on_btnSelectFile_clicked()
{
    QFileDialog dlg;

    QString file = dlg.getOpenFileName(this);
    ui->edtFileName->setText(file);

}
