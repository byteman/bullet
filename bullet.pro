#-------------------------------------------------
#
# Project created by QtCreator 2018-02-04T23:20:50
#
#-------------------------------------------------

QT       += core  gui network

#CONFIG += console
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets sql printsupport

TARGET = bullet
TEMPLATE = app
debug{
    message("debug");
    OBJECTS_DIR=debug
}
INCLUDEPATH+=./qtcsv ./qtcsv/include
INCLUDEPATH+=./ext/
LIBS+=-L./ext/lib

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
    config.cpp \
    histwavewidget.cpp \
    MyTracer.cpp \
    dialogupdate.cpp \
    alpostfile.cpp \
    httpclient.cpp \
    asyncexport.cpp \
    qtcsv/sources/contentiterator.cpp \
    qtcsv/sources/reader.cpp \
    qtcsv/sources/stringdata.cpp \
    qtcsv/sources/variantdata.cpp \
    qtcsv/sources/writer.cpp \
    dialogselectchan.cpp \
    dialogmerge.cpp \
    chanselect.cpp \
    mergemanager.cpp \
    xlsx/statemanager.cpp \
    xlsx/stateinfo.cpp \
    dialogreport.cpp \
    gotypes.cpp \
    myplottrace.cpp \
    machinechecker.cpp \
    cmddebug.cpp \
    asyncexporttask.cpp \
    dialogclearup.cpp

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
    config.h \
    histwavewidget.h \
    MyTracer.h \
    dialogupdate.h \
    alpostfile.h \
    httpclient.h \
    asyncexport.h \
    qtcsv/sources/contentiterator.h \
    qtcsv/sources/filechecker.h \
    qtcsv/sources/symbols.h \
    dialogselectchan.h \
    dialogmerge.h \
    chanselect.h \
    mergemanager.h \
    xlsx/statemanager.h \
    xlsx/stateinfo.h \
    dialogreport.h \
    gotypes.h \
    myplottrace.h \
    machinechecker.h \
    cmddebug.h \
    asyncexporttask.h \
    dialogclearup.h

FORMS    += MainWnd.ui \
    channelwidget.ui \
    dialogparams.ui \
    dialogcalib.ui \
    frmmessagebox.ui \
    dialogdevice.ui \
    dialogchanconfig.ui \
    dialogupdate.ui \
    dialogselectchan.ui \
    dialogmerge.ui \
    chanselect.ui \
    dialogreport.ui \
    dialogclearup.ui
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
QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01
win32:DEFINES += _TTY_WIN_
win32:DEFINES += "WINVER=0x0501"
win32:DEFINES += "_WIN32_WINNT=0x0501"

INCLUDEPATH += ext/include/CuteLogger
INCLUDEPATH += ext/qBreakpad/handler

LIBS += -Lext/qBreakpad/handler

CONFIG(release, debug|release) {
        #LIBS+=-lqBreakpad
#支持在release模式下还能够调试
        QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
        QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
}

QXLSX_PARENTPATH=./QXlsx       # current QXlsx path is . (. means curret directory)
QXLSX_HEADERPATH=./QXlsx/header/  # current QXlsx header path is ./header/
QXLSX_SOURCEPATH=./QXlsx/source/  # current QXlsx source path is ./source/

include(./QXlsx/QXlsx.pri)

#msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8
