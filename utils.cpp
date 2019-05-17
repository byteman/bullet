#include "utils.h"
#include <QDebug>
#include <QDir>
Utils::Utils()
{

}


/**
 * @brief mkMutiDir  创建多级目录
 * @param path	     未创建的目录
 * @return 	     返回已创建的目录
 */
QString utils::MkMutiDir(const QString path){
    QDir dir(path);
    if ( dir.exists(path)){
        return path;
    }
    QString parentDir = MkMutiDir(path.mid(0,path.lastIndexOf('/')));
    QString dirname = path.mid(path.lastIndexOf('/') + 1);
    QDir parentPath(parentDir);
    if ( !dirname.isEmpty() )
        parentPath.mkpath(dirname);
    return parentDir + "/" + dirname;
}

TimeStamp::TimeStamp(QString _tag):
    tag(_tag)
{
    time = QTime::currentTime();
}

TimeStamp::~TimeStamp()
{
    qDebug() << tag << " elapsed " << time.elapsed();
}


#include "utils.h"

utils::utils()
{

}

#include <QTextCodec>

QString utils::GBK2UTF8(const QString &inStr)
{
    QTextCodec *gbk = QTextCodec::codecForName("GBK");
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

    QString g2u = gbk->toUnicode(gbk->fromUnicode(inStr));              // gbk  convert utf8
    return g2u;
}

QString utils::UTF82GBK(const QString &inStr)
{
    QTextCodec *gbk = QTextCodec::codecForName("GBK");
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

//    QString utf2gbk = gbk->toUnicode(inStr.toLocal8Bit());
//    return utf2gbk;



    QString strUnicode= utf8->toUnicode(inStr.toLocal8Bit());
    QByteArray ByteGb2312= gbk->fromUnicode(strUnicode);

    //strUtf8= ByteGb2312.data();

    return QString::fromLocal8Bit(ByteGb2312.data());//注意这里要fromLocal8Bit()


}

std::string utils::gbk2utf8(const QString &inStr)
{
    return GBK2UTF8(inStr).toStdString();
}

QString utils::utf82gbk(const std::string &inStr)
{
    QString str = QString::fromStdString(inStr);

    return UTF82GBK(str);
}


QString utils::Time2String(qint64 xValue)
{
   return QDateTime::fromMSecsSinceEpoch(xValue*1000).toString("yyyy-MM-dd HH:mm:ss");
}
float utils::int2float(int wt, int dot)
{
    float wf = (float)wt;
    switch(dot)
    {

        case 1:
            return wf/10.0f;
            break;
        case 2:
            return wf/100.0f;
            break;
        case 3:
            return wf/1000.0f;
            break;
        case 4:
            return wf/10000.0f;
            break;
        default:
            return wf;
            break;
    }
    return wf;
}
QString utils::formatfloat(float wf, int dot)
{
    char buf[64] = {0,};
    switch(dot)
    {

        case 1:
            qsnprintf(buf,64,"%0.1f",wf);
            break;
        case 2:
            qsnprintf(buf,64,"%0.2f",wf);
            break;
        case 3:
            qsnprintf(buf,64,"%0.3f",wf);
            break;
        case 4:
            qsnprintf(buf,64,"%0.4f",wf);
            break;
        default:
            qsnprintf(buf,64,"%d",int(wf));
            break;
    }
    return buf;
}
QString utils::float2string(float wf, int dot)
{
    char buf[64] = {0,};
    switch(dot)
    {

        case 1:
            qsnprintf(buf,64,"%0.1f",wf/10);
            break;
        case 2:
            qsnprintf(buf,64,"%0.2f",wf/100);
            break;
        case 3:
            qsnprintf(buf,64,"%0.3f",wf/1000);
            break;
        case 4:
            qsnprintf(buf,64,"%0.4f",wf/10000);
            break;
        default:
            qsnprintf(buf,64,"%d",int(wf));
            break;
    }
    return buf;
}
QString FormatHex(QByteArray& data)
{
    return data.toHex();
}
int get_random_number()
 {
     qsrand(QTime(0,0,0).msecsTo(QTime::currentTime()));
     int a = qrand()%100;   //随机生成0到9的随机数
     return a;
}
