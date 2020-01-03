#ifndef UTILS_H
#define UTILS_H

#include <QTime>
#include <QFileInfoList>
class Utils
{
public:
    Utils();
};
class TimeStamp{
public:
    TimeStamp(QString _tag);
    ~TimeStamp();
private:
    QTime time;
    QString tag;
};

class utils
{
public:
    utils();
    static QString formatfloat(float wf, int dot);
    static QString float2string(float wf, int dot);
    static float int2float(int wt, int dot);
    static QString GBK2UTF8(const QString &inStr);
    static QString UTF82GBK(const QString &inStr);
    static std::string gbk2utf8(const QString &inStr);
    static QString utf82gbk(const std::string &inStr);
    static QString Time2String(qint64 xValue);
    static QString MkMutiDir(const QString path);

    static QString utf8ToGb2312(QString strUtf8);
    static QString GetWorkDir();
    static QFileInfoList ListDirFiles(QString dir,QString ext);
    static bool ExistProcess(const char* name);
    static bool KillProcess(QString name);
    static bool StartProcess(QString name);
    static bool ExistFile(QString file);
    static bool FindFiles(const QString &dirPath,QString ext, QFileInfoList &filelist);
};
QString FormatHex(QByteArray& data);
int get_random_number();
#endif // UTILS_H
