#-------------------------------------------------
#
# Project created by QtCreator 2018-02-04T23:20:50
#
#-------------------------------------------------

QT       += core  gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets sql printsupport

TARGET = bullet
TEMPLATE = app
debug{
    message("debug");
    OBJECTS_DIR=debug
}
INCLUDEPATH+=./ext/
LIBS+=-L./ext/lib -lPocoNetd -lPocoFoundationd
#DEFINES += QCUSTOMPLOT_USE_OPENGL
SOURCES += main.cpp\
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
    appinit.cpp \
    iconhelper.cpp \
    channelwidget.cpp \
    mydevices.cpp \
    MainWnd.cpp \
    dialogparams.cpp \
    dialogcalib.cpp \
    frmmessagebox.cpp \
    dao.cpp \
    models.cpp \
    dialogdevice.cpp \
    dialogchanconfig.cpp \
    config.cpp

HEADERS  += tcpserver.h \
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
    MainWnd.h \
    appinit.h \
    iconhelper.h \
    channelwidget.h \
    mydevices.h \
    dialogparams.h \
    dialogcalib.h \
    frmmessagebox.h \
    dao.h \
    models.h \
    dialogdevice.h \
    dialogchanconfig.h \
    config.h

FORMS    += MainWnd.ui \
    channelwidget.ui \
    dialogparams.ui \
    dialogcalib.ui \
    frmmessagebox.ui \
    dialogdevice.ui \
    dialogchanconfig.ui
#BASEDIR=c:\Qt\Qt5.5.0\5.5\msvc2012
##INCLUDEPATH += $$BASEDIR\include\Qwt
#LIBS += -L$$BASEDIR\lib -lqwtd
LIBS += -lOpengl32 \
                -lglu32
RESOURCES += \
    img.qrc \
    qss.qrc \
    main.qrc
RC_FILE=icon.rc
DISTFILES +=

#msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8
