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
#include <QFuture>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
bool writeXlsxFile(int chan,QString filePath)
{

    QXlsx::Document xlsxW;
    xlsxW.setColumnWidth(1,25);
    xlsxW.write("A1", "time"); // write "Hello Qt!" to cell(A,1). it's shared string.
   // xlsxW.write("B1", QString(QStringLiteral("通道")+"%1").arg(chan));
    xlsxW.write("B1","value");
    for(int j = 0; j < 10; j++)
    {
        //xlsxW.write(j+2,1,j);
        xlsxW.write(j+2,1,QString("%1").arg(QDateTime::fromMSecsSinceEpoch(1000).toString("yyyy-MM-dd hh:mm:ss")));
        xlsxW.write(j+2,2,j);
    }

    xlsxW.saveAs(filePath); // save the document as 'Test.xlsx'

    return true;
}

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
void test()
{
    QVector<QFuture<bool>> res;
    for(int i = 0; i < 1000; i++)
    {
        res.push_back(QtConcurrent::run(writeXlsxFile,1,QString("%1.xlsx").arg(i)));
    }
    for(int i = 0; i < 10; i++)
    {
        res[i].waitForFinished();
    }
}
int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    if(!checkOne()) {
        QMessageBox::information(NULL,QStringLiteral("错误"),QStringLiteral("已经启动了一个程序了"));

       return 0;
    }

    initLogger();
    //QBreakpadInstance.setDumpPath(QLatin1String("crashes_dump"));
    //InitConsoleWindow();
    MachineChecker checker;
    if(1){
    //if(checker.CheckMac("08:57:00:F6:08:DC") ||
    //        checker.CheckMac("B0:35:9F:87:D9:FE")){
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
