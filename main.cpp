#include "mainwindow.h"
#include <QApplication>
#include "MainWnd.h"
#include "appinit.h"
#include <QTextCodec>
#include <QFile>
#include <QBreakpadHandler.h>
#include <QDebug>
#include "channelwidget.h"
#include <machinechecker.h>
#include "cmddebug.h"
#include <Windows.h>
#include "Logger.h"
#include "ConsoleAppender.h"
#include "FileAppender.h"

void initLogger()
{

    QSettings config("bullet.ini", QSettings::IniFormat);
     config.setIniCodec("UTF-8");
    bool enable_log        = config.value("/config/log",false).toBool();

    if(!enable_log)
    {
        return;
    }
    ConsoleAppender* consoleAppender = new ConsoleAppender;
    consoleAppender->setFormat("[%{type:-7}] <%{Function}> %{message}\n");
    cuteLogger->registerAppender(consoleAppender);

    cuteLogger->registerAppender(new FileAppender("bullet.log"));

}
bool checkOne()
{
    //  创建互斥量
    HANDLE m_hMutex  =  CreateMutex(NULL, FALSE,  L"battery_measure" );
    //  检查错误代码
    if  (GetLastError()  ==  ERROR_ALREADY_EXISTS)  {
        //  如果已有互斥量存在则释放句柄并复位互斥量
        CloseHandle(m_hMutex);
        m_hMutex  =  NULL;
        //  程序退出
        return  false;
    }
    else
        return true;
}

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    if(!checkOne()) {
        QMessageBox::information(NULL,QStringLiteral("错误"),QStringLiteral("已经启动了一个程序了"));

       return 0;
    }

    initLogger();

    //InitConsoleWindow();

    //加载样式表
    QFile file(":/qss/psblack.css");
    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        qApp->setStyleSheet(qss);
        file.close();
    }
    //qDebug() <<"version=" << Poco::Environment::libraryVersion();
    a.setFont(QFont("Microsoft Yahei", 9));
    AppInit::Instance()->start();

    MainWnd w;
    w.show();

    return a.exec();

}
