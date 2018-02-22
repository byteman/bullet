#-------------------------------------------------
#
# Project created by QtCreator 2018-02-04T23:20:50
#
#-------------------------------------------------

QT       += core  gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bullet
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    server.cpp \
    device.cpp \
    devicemanager.cpp \
    tcpsession.cpp \
    sessmessage.cpp \
    protomessage.cpp \
    protoparser.cpp \
    wavefile.cpp \
    udpserver.cpp \
    udpsession.cpp \
    wavewidget.cpp \
    qwtchannel.cpp

HEADERS  += mainwindow.h \
    server.h \
    imsginterface.h \
    device.h \
    devicemanager.h \
    tcpsession.h \
    session.h \
    sessmessage.h \
    protomessage.h \
    protoparser.h \
    parser.h \
    wavefile.h \
    udpserver.h \
    udpsession.h \
    wavewidget.h \
    qwtchannel.h

FORMS    += mainwindow.ui
INCLUDEPATH += C:\Qt\Qt5.5.0\5.5\msvc2012\include\Qwt
LIBS += -LC:\Qt\Qt5.5.0\5.5\msvc2012\lib -lqwtd -lqwt

RESOURCES += \
    img.qrc

DISTFILES +=

