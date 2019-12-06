#include "httpclient.h"
#include <QJsonDocument>


Ajax::Ajax()
{
    nam = new QNetworkAccessManager(NULL);

    QObject::connect(nam, SIGNAL(finished(QNetworkReply*)),
                     this, SLOT(finishedSlot(QNetworkReply*)));
}

Ajax::~Ajax()
{
    delete nam;
}


static RequestInterceptors  requestHook=NULL;
static ResponseInterceptors responeHook=NULL;


QNetworkReply* Ajax::post(QString url, QJsonDocument  body)
{


    QByteArray array = body.toJson(QJsonDocument::Compact);



    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));


    if(requestHook!=NULL)
    {
        if(requestHook(&request)!=0)
        {
            return NULL;
        }
    }
    return nam->post(request,array);


}
#include <QFile>
#include <QUrlQuery>
QNetworkReply *Ajax::postFile(QString url, QString name, const QObject *receiver, const char *finished)
{
      QFile file(name);
      if (!file.open(QIODevice::ReadOnly)||file.size()==0)
      {
         file.close();
         return NULL ;
      }
      QByteArray fdata = file.readAll();
      if(fdata.isEmpty()){
          return NULL;
      }
      file.close();
      QUrlQuery params;
      params.addQueryItem("file",fdata.toBase64());

      QNetworkRequest request;
      QString data = params.toString();

      request.setUrl(url);
      request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded;charset=utf-8");
      request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
      if(requestHook!=NULL)
      {
          if(requestHook(&request)!=0)
          {
              return NULL;
          }
      }
      return nam->post(request, params.toString().toUtf8());
}

QNetworkReply *Ajax::post(QString url, QJsonDocument body,const QObject *receiver, const char* finished)
{
    QNetworkReply* reply;
    qDebug() << "post " << url;
    QByteArray array = body.toJson(QJsonDocument::Compact);



    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));


    if(requestHook!=NULL)
    {
        if(requestHook(&request)!=0)
        {
            return NULL;
        }
    }
    reply = nam->post(request,array);

    addEvent(reply,receiver,finished);

    return reply;
}
QNetworkReply *Ajax::put(QString url, QJsonDocument body,const QObject *receiver, const char* finished)
{
    QNetworkReply* reply;
    qDebug() << "put " << url;
    QByteArray array = body.toJson(QJsonDocument::Compact);



    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));


    if(requestHook!=NULL)
    {
        if(requestHook(&request)!=0)
        {
            return NULL;
        }
    }
    reply = nam->put(request,array);

    addEvent(reply,receiver,finished);

    return reply;
}
QNetworkReply *Ajax::get(QString url, const QObject *receiver, const char *finished)
{
    //qDebug() << "get " << url;
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));


    if(requestHook!=NULL)
    {
        if(requestHook(&request)!=0)
        {
            return NULL;
        }
    }
    QNetworkReply * reply = nam->get(request);
    addEvent(reply,receiver,finished);

    return reply;
}

QNetworkReply *Ajax::remove(QString url, const QObject *receiver, const char *finished)
{
    qDebug() << "remove " << url;
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));


    if(requestHook!=NULL)
    {
        if(requestHook(&request)!=0)
        {
            return NULL;
        }
    }
    QNetworkReply * reply = nam->deleteResource(request);
    addEvent(reply,receiver,finished);

    return reply;
}


void Ajax::finishedSlot(QNetworkReply *r)
{
    //qDebug() << "finshed!!";
    //qDebug() << "err=" << r->errorString();
//    for(int i =0 ;i < r->rawHeaderList().count();i++)
//    {
//        qDebug() << "header=" << i  << r->rawHeaderList()[i];
//    }
//    qDebug() << "url=" << r->url();
//    qDebug() << "oper=" << r->operation();

    AjaxResponse resp;
    resp.data=r->readAll();
    resp.method=r->operation();
    resp.reply=r;
    resp.status = r->error();
    resp.error = (r->error()!=0);
    if(responeHook!=NULL)
    {
        if(responeHook(r)!=0)
        {
            return;
        }
    }
    QMap<QNetworkReply*,NotifyObject*>::iterator it = notifys.find(r);
    if(it == notifys.end())
    {
        return;
    }
    NotifyObject* nobj = *it;

    notifys.erase(it);
    nobj->Notify(resp);

    delete nobj;
    r->deleteLater();


}

void Ajax::addEvent(QNetworkReply *r,const QObject *receiver, const char *finished)
{


    NotifyObject *no = new NotifyObject();

    connect(no,SIGNAL(finished(AjaxResponse)),receiver,finished);

    notifys[r]=no;
}

bool AjaxResponse::GetJsonObject(QJsonDocument &json)
{
    QJsonParseError json_error;
    json = QJsonDocument::fromJson(this->data, &json_error);
    return (json_error.error == QJsonParseError::NoError);

}
QJsonParseError AjaxResponse::GetJSON(QJsonDocument &json)
{
    QJsonParseError json_error;
    //qDebug() << QString(this->data);

    json = QJsonDocument::fromJson(this->data, &json_error);
    //qDebug() << "GetJson" << QString(json.toJson(QJsonDocument::Compact));
    return json_error;
}


void SetInterceptors(RequestInterceptors reqHook, ResponseInterceptors respHook)
{
    requestHook = reqHook;
    responeHook = respHook;
}


NotifyObject::NotifyObject()
{
    //qDebug() << "NotifyObject created!";
}

NotifyObject::~NotifyObject()
{
      // qDebug() << "NotifyObject destroy!";
}

void NotifyObject::Notify(AjaxResponse resp)
{
    emit finished(resp);
}


