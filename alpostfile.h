#ifndef ALPOSTFILE_H
#define ALPOSTFILE_H


#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class ALPostFile : public QObject
{
    Q_OBJECT
public:
    ALPostFile();
    ~ALPostFile();
    Q_INVOKABLE void setServerAddr(QString addr){m_strServerAddr = addr;}
    Q_INVOKABLE void setPostFilePath(QString filePath){m_strFilePath = filePath;this->start();}
    void start();

signals:
    void sigReplyMessage(int retCode,QString msg);//从服务器返回的数据，retCode=0 表示成功，那么msg就是图片的存储位置；retCode非0 msg就是错误信息
private slots:
    void replyFinished(QNetworkReply *reply);

private:
    QString m_strServerAddr;
    QString m_strFilePath;
    QNetworkAccessManager * m_pNetWorkManager;

};

#endif // ALPOSTFILE_H
