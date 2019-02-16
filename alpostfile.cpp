#include "alpostfile.h"
#include <QUrlQuery>
#include <QFile>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

ALPostFile::ALPostFile()
{
    m_pNetWorkManager = new QNetworkAccessManager();
    connect(m_pNetWorkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));
}

ALPostFile::~ALPostFile()
{
    delete m_pNetWorkManager;
}

void ALPostFile::start()
{
    if(m_strFilePath.isEmpty() || m_strServerAddr.isEmpty()){
        return;
    }
    QFile file(m_strFilePath);
    if (!file.open(QIODevice::ReadOnly)||file.size()==0)
    {
       file.close();
       return ;
    }
    QByteArray fdata = file.readAll();
    if(fdata.isEmpty()){
        return;
    }
    file.close();
    QUrlQuery params;
    params.addQueryItem("file",fdata.toBase64());

    QNetworkRequest request;
    QString data = params.toString();

    request.setUrl(m_strServerAddr);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded;charset=utf-8");
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
    m_pNetWorkManager->post(request, params.toString().toUtf8());
}

void ALPostFile::replyFinished(QNetworkReply *reply)
{

    QByteArray ba = reply->readAll();
    qDebug() << "err code=" << reply->error();
    QJsonParseError jsonpe;
    QJsonDocument json = QJsonDocument::fromJson(ba, &jsonpe);
    if (jsonpe.error == QJsonParseError::NoError)
    {
        if (json.isObject())
        {
            QJsonObject obj = json.object();
            if (obj.contains("error_code"))
            {
                if(obj["error_code"] == 0){//成功
                    if(obj.contains("url")){
                        qDebug() << "obj[url] = " << obj["url"].toString();
                        emit sigReplyMessage(0,obj["url"].toString());
                    }
                }
                else{
                    if(obj.contains("message")){
                        qDebug() << "obj[message] = " << obj["message"];
                        emit sigReplyMessage(1,obj["message"].toString());
                    }
                }
            }
        }
        else
        {
            qDebug() << "error, shoud json object";
        }
    }
    else
    {
        qDebug() << "error:" << jsonpe.errorString();
    }

}
