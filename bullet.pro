#-------------------------------------------------
#
# Project created by QtCreator 2018-02-04T23:20:50
#
#-------------------------------------------------

QT       += core  gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = bullet
TEMPLATE = app
debug{
    message("debug");
    OBJECTS_DIR=debug
}
INCLUDEPATH+=./ext/ ./ext/
LIBS+=-L./ext/lib -lPocoNet -lPocoFoundation
DEFINES += QCUSTOMPLOT_USE_OPENGL
SOURCES += main.cpp\
        mainwindow.cpp \
    tcpserver.cpp \
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
    qcustomplot.cpp \
    igraphicchannel.cpp \
    ilinechart.cpp \
    qcustomchart.cpp \
    qcustomplotchannel.cpp \
    ifilter.cpp \
    shiftfilter.cpp \
    utils.cpp \
    gpserver.cpp \
    gpsession.cpp \
    mysocket.cpp \
    csvfile.cpp \
    syncfile.cpp \
    widgetchan.cpp \
    channelwidget.cpp \
    dialogchanconfig.cpp \
    mydevices.cpp \
    dao.cpp

HEADERS  += mainwindow.h \
    tcpserver.h \
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
    qcustomplot.h \
    igraphicchannel.h \
    ilinechart.h \
    qcustomchart.h \
    qcustomplotchannel.h \
    ifilter.h \
    shiftfilter.h \
    utils.h \
    gpserver.h \
    gpsession.h \
    mysocket.h \
    csvfile.h \
    syncfile.h \
    widgetchan.h \
    channelwidget.h \
    dialogchanconfig.h \
    mydevices.h

FORMS    += mainwindow.ui \
    widgetchan.ui \
    channelwidget.ui \
    dialogchanconfig.ui
#BASEDIR=c:\Qt\Qt5.5.0\5.5\msvc2012
##INCLUDEPATH += $$BASEDIR\include\Qwt
#LIBS += -L$$BASEDIR\lib -lqwtd
LIBS += -lOpengl32 \
                -lglu32

#支持在release模式下还能够调试
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
RESOURCES += \
    img.qrc
RC_FILE=icon.rc
DISTFILES +=

