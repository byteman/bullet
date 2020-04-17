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
    connect(_accessManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));

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
bool FtpUploadManger::doUpload(QString fileName,
                               QString host,
                               QString target)
{

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)){
        OnError(fileName,QStringLiteral("文件打开失败"));
        return false;
    }
    QByteArray byte_file = file.readAll();

    QUrl url(QString("ftp://%1:%2").arg(host).arg(_ftpPort));
    url.setPort(_ftpPort);
    url.setUserName(_ftpUser);
    url.setPassword(_ftpPassword);
    url.setPath(target);
    qDebug() << "url===>" << url.toString();
    QNetworkRequest request(url);
    QNetworkReply* reply = _accessManager->put(request, byte_file);


    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(loadError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(uploadProgress(qint64 ,qint64)), this, SLOT(loadProgress(qint64 ,qint64)));

}

void FtpUploadManger::AddTask(QString fileName,
                              QString host,QString target,
                              int maxRetry)
{
    doUpload(fileName,host,target);
}
//reply请求已经完毕.
void FtpUploadManger::replyFinished(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {

        emit OnSuccess(reply->url().host());
        qDebug() << reply->url().host() << "finished";
    }
    else
    {
        qDebug() << "error" << reply->errorString();
        emit OnError(reply->url().host(),reply->errorString());
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
