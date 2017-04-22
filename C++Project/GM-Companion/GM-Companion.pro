#-------------------------------------------------
#
# Project created by QtCreator 2017-04-10T13:52:09
#
#-------------------------------------------------

QT       += core gui \
            multimedia \
            multimediawidgets \
            sql \
            network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GM-Companion
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
           mainwindow.cpp \
    flowlayout.cpp \
    database.cpp

HEADERS  += mainwindow.h \
    flowlayout.h \
    database.h

FORMS    += mainwindow.ui

DISTFILES += \

RC_FILE = GM-Companion.rc

RESOURCES += \
    resources.qrc
