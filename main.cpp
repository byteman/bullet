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
int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    QBreakpadInstance.setDumpPath(QLatin1String("crashes_dump"));
    //InitConsoleWindow();
    MachineChecker checker;
    if(checker.CheckMac("08:57:00:F6:08:DC") ||
            checker.CheckMac("B0:35:9F:87:D9:FE")){
    //if(checker.CheckMac("B0:35:9F:87:D9:FE")){
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
    }else{
        QMessageBox::critical(NULL,QStringLiteral("错误"),QStringLiteral("此电脑未授权，软件无法启动"));
    }


}
