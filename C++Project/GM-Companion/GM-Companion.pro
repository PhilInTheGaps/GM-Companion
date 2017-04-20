#-------------------------------------------------
#
# Project created by QtCreator 2017-04-10T13:52:09
#
#-------------------------------------------------

QT       += core gui \
            multimedia \
            multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GM-Companion
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
           mainwindow.cpp \
    flowlayout.cpp

HEADERS  += mainwindow.h \
    flowlayout.h

FORMS    += mainwindow.ui

DISTFILES += \
    lib/id3lib.exp \
    lib/id3lib.dll

RESOURCES += \
    resources.qrc
