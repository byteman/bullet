#-------------------------------------------------
#
# Project created by QtCreator 2018-02-20T11:49:38
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../protomessage.cpp \
    ../protoparser.cpp

HEADERS  += mainwindow.h \
    ../protomessage.h \
    ../protoparser.h

FORMS    += mainwindow.ui
