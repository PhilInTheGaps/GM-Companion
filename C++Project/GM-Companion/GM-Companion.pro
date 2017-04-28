#-------------------------------------------------
#
# Project created by QtCreator 2017-04-10T13:52:09
#
#-------------------------------------------------

QT       += core gui \
            multimedia \
            multimediawidgets \
            winextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GM-Companion
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
           mainwindow.cpp \
    flowlayout.cpp \
    characters.cpp \
    functions.cpp \
    settingsmanager.cpp \
    dicemanager.cpp \
    music.cpp \
    sound.cpp \
    names.cpp \
    radio.cpp \
    maps.cpp \
    chareditor.cpp \
    characterpage.cpp

HEADERS  += mainwindow.h \
    flowlayout.h \
    characters.h \
    functions.h \
    settingsmanager.h \
    dicemanager.h \
    chareditor.h \
    characterpage.h

FORMS    += mainwindow.ui \
    chareditor.ui \
    characterpage.ui

DISTFILES += \

RC_FILE = GM-Companion.rc

RESOURCES += \
    resources.qrc
