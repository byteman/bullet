#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
//#include "apiv1.h"
#include <QMap>

typedef int (*RequestInterceptors)(QNetworkRequest* reqest);
typedef int (*ResponseInterceptors)(QNetworkReply* response);

struct AjaxResponse{
    bool error;
    int method;
    std::string method2;
    QByteArray data;
    int status;
    QNetworkReply* reply;
    QJsonParseError GetJSON(QJsonDocument& json);
    void Destory();

public:
    bool GetJsonObject(QJsonDocument &json);
};

class NotifyObject :public QObject
{
    Q_OBJECT
public:
    NotifyObject();
    ~NotifyObject();
    void Notify(AjaxResponse resp);

signals:
    void finished(AjaxResponse resp);

};
class Ajax:public QObject
{
    Q_OBJECT

public:
    Ajax();
    ~Ajax();
    QNetworkReply* post(QString url, QJsonDocument body);
    QNetworkReply* postFile(QString url,QString name, const QObject *receiver,  const char* finished);

    QNetworkReply* post(QString url, QJsonDocument body, const QObject *receiver,  const char* finished);
    QNetworkReply* get(QString url, const QObject *receiver,  const char* finished);
    QNetworkReply* remove(QString url, const QObject *receiver,  const char* finished);
    QNetworkReply *put(QString url, QJsonDocument body, const QObject *receiver, const char *finished);
signals:
    void success(AjaxResponse resp);
    void error(AjaxResponse resp);
private slots:
    void finishedSlot(QNetworkReply*);

private:
    void addEvent(QNetworkReply *r,const QObject *receiver, const char *finished);
    QNetworkAccessManager *nam;
    QMap<QNetworkReply*,NotifyObject*> notifys;

};

void SetInterceptors(RequestInterceptors reqHook,ResponseInterceptors respHook);

#endif // HTTPCLIENT_H
