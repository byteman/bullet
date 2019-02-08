#include "mainwindow.h"
#include <QApplication>
#include "MainWnd.h"
#include "appinit.h"
#include <QTextCodec>
#include <QFile>
#include <Poco/Environment.h>
#include <QDebug>
#include "channelwidget.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    ChannelWidget w(1);
//    w.show();

    //加载样式表
    QFile file(":/qss/psblack.css");
    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        qApp->setStyleSheet(qss);
        file.close();
    }
    qDebug() <<"version=" << Poco::Environment::libraryVersion();
    a.setFont(QFont("Microsoft Yahei", 9));
    AppInit::Instance()->start();

    MainWnd w;
    w.show();


    return a.exec();
}
