#include "ftpupload.h"
#include <QFile>
#include <QNetworkAccessManager>
#include <QDebug>
class FtpUploadTask:public QObject
{
    Q_OBJECT
public slots:
    void doWork(FtpUploadManger* _ftpManager,
                UploadFileInfo& fileInfo) {


        /* ... here is the expensive or blocking operation ... */
        emit onSuccess(fileInfo.fileName);
    }

signals:
    void onSuccess(const QString &fileName);
    void onError(const QString &fileName, QString reason);
protected slots:
    void loadError(QNetworkReply::NetworkError err);
    void loadProgress(qint64 bytesSent, qint64 bytesTotal);
    void replyFinished(QNetworkReply *);

};
FtpUploadManger::FtpUploadManger()
{
    Init();
}

bool FtpUploadManger::Init()
{
    _accessManager = new QNetworkAccessManager(this);
    _accessManager->setNetworkAccessible(QNetworkAccessManager::Accessible);
    return true;
}

void FtpUploadManger::SetFtpBaseDir(QString ftpBaseDir)
{
    _ftpBaseDir = ftpBaseDir;
}

bool FtpUploadManger::Auth(int port,QString user, QString password)
{
    _ftpUser = user;
    _ftpPassword = password;
    return true;
}
bool FtpUploadManger::doUpload(QString fileName,QString ftpurl)
{

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)){
        OnError(fileName,QStringLiteral("文件打开失败"));
        return false;
    }
    QByteArray byte_file = file.readAll();

    QUrl url(ftpurl);
    url.setPort(_ftpPort);
    url.setUserName(_ftpUser);
    url.setPassword(_ftpPassword);

    qDebug() << "url===>" << url.toString();
    QNetworkRequest request(url);
    QNetworkReply* reply = _accessManager->put(request, byte_file);

    connect(_accessManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(loadError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(uploadProgress(qint64 ,qint64)), this, SLOT(loadProgress(qint64 ,qint64)));

}

void FtpUploadManger::AddTask(QString fileName, QString ftpUrl, int maxRetry)
{
    doUpload(fileName,ftpUrl);
}

void FtpUploadManger::replyFinished(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {

        qDebug() << "finished";
    }
    else
    {
        qDebug() << "error" << reply->errorString();
        //OnError();
    }
    reply->deleteLater();

}

void FtpUploadManger::loadError(QNetworkReply::NetworkError err)
{
    qDebug()<<"loadError: "<<err;
}

void FtpUploadManger::loadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    qDebug() << "loadProgress:" << bytesSent << "of" << bytesTotal;
}
