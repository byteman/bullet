﻿#include "utils.h"
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
#include <QCoreApplication>
#include <Windows.h>
QString utils::GetWorkDir()
{
 //   qDebug() << "GetWorkDir";
//    char szFilePath[1024]={0,};
//    GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
    QString path = QCoreApplication::applicationDirPath();
    qDebug() << "GetWorkDir=" << path;
    return path;

}
#include <QDir>
QFileInfoList utils::ListDirFiles(QString dirName, QString ext)
{

    QDir dir(dirName);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);
    QStringList filterList;
    filterList << ext;   //设置筛选条件
    return  dir.entryInfoList(filterList);

}
bool utils::FindFiles(const QString& dirPath, QString ext,QFileInfoList &filelist)
{
    QDir dir(dirPath);
    //目录不存在
    if (!dir.exists()) {
        return false;
    }

  //取到所有的文件和文件名，但是去掉.和..的文件夹（这是QT默认有的）
    dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);

    //文件夹优先
    dir.setSorting(QDir::DirsFirst);
    QStringList filterList;
    filterList << ext;   //设置筛选条件
    //转化成一个list
    QFileInfoList list = dir.entryInfoList();
    if(list.size()< 1 ) {
        return false;
    }
    int i=0;

    //递归算法的核心部分
    do{
        QFileInfo fileInfo = list.at(i);
        //如果是文件夹，递归
        bool bisDir = fileInfo.isDir();
        if(bisDir) {
            FindFiles(fileInfo.filePath(),ext,filelist);
        }
        else{
            //bool isDll = fileInfo.fileName().endsWith(".dll");
            //qDebug() << fileInfo.filePath() << ":" <<fileInfo.fileName();
            filelist.push_back(fileInfo);
        }//end else
        i++;
    } while(i < list.size());
    return true;
}
#include <windows.h>
#include "psapi.h"
#include"stdio.h"
#include <tlhelp32.h>
bool utils::ExistProcess(const char* name)
{

        int i=0;
        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(pe32);
        HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if(hProcessSnap == INVALID_HANDLE_VALUE)
        {
            i+=0;
        }
        BOOL bMore = ::Process32First(hProcessSnap, &pe32);
        while(bMore)
        {
            //printf(" 进程名称：%s \n", pe32.szExeFile);

            //if(strcmp(name,pe32.szExeFile)==0)
            {
                //printf("进程运行中");
                i+=1;
            }
            bMore = ::Process32Next(hProcessSnap, &pe32);
        }
        if(i>1){           //大于1，排除自身
            return true;
        }else{
            return false;
        }

}
#include <QProcess>
bool utils::KillProcess(QString name)
{
   QString c = QString("taskkill /im %1 /f").arg(name);
   int pInt = QProcess::execute(c);    //关闭后台notepad.exe进程，阻塞式运行,一直占用cpu,成功返回0，失败返回1

   qDebug()<<"pInt:"<<pInt;
   return pInt;
}

bool utils::StartProcess(QString name)
{
    QProcess p;
    p.start(name);
    if(p.waitForStarted()){
        qDebug() << "wait for start failed";
        return false;
    }
    return true;
}
#include <QFile>
bool utils::ExistFile(QString file)
{
    QFile f(file);

    return f.exists();
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
