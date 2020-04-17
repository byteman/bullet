#ifndef FTPUPLOAD_H
#define FTPUPLOAD_H
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>

struct UploadFileInfo{
    QString fileName; //文件名称
    QString ftpUrl; //上传的文件路径
    int     fileState; //上传的状态.
    int     retryCount; //上传失败的次数.
    QDateTime fileUpTime; //文件上传时间.
};
struct UploadFileList{
     int  AddFile(QString fileName,QString  ftpUrl);
     bool RemoveFile(QString fileName);
     //获取一个未上传成功的文件
     bool PeekFirstUnUploadFile(UploadFileInfo& finfo);
     //清理多次上传未成功的文件
     bool ClearFailedFiles(int maxCount=3);
     QMap<QString,UploadFileInfo> _uploadFileList;
};
class FtpUploadManger:public QObject
{
    Q_OBJECT
public:
    FtpUploadManger();
    bool Init();
    bool UnInit();
    void SetFtpBaseDir(QString ftpBaseDir);
    //用户名和密码验证
    bool Auth(int port,QString user,QString password);
    //添加一个上传任务
    void AddTask(QString fileName,
                 QString host,QString target,int maxRetry=3);
signals:
    //某个文件上传完成.
    void OnSuccess(QString host);
    //某个文件上传失败
    void OnError(QString host,QString reason);
protected:
    bool doUpload(QString fileName,QString host,QString target);
protected slots:
    void loadError(QNetworkReply::NetworkError err);
    void loadProgress(qint64 bytesSent, qint64 bytesTotal);
    void replyFinished(QNetworkReply *);
private:
    UploadFileList _uploadFileList; //上传文件列表
    QString _ftpBaseDir;
    QString _ftpUser;
    QString _ftpPassword;
    int     _ftpPort;
    QNetworkAccessManager *_accessManager;
    //QNetworkReply *_reply;

};

#endif // FTPUPLOAD_H
