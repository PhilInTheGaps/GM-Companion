#-------------------------------------------------
#
# Project created by QtCreator 2017-04-10T13:52:09
#
#-------------------------------------------------

QT       += core gui \
            multimedia \
            multimediawidgets \
            network \
            sql

win32:QT += winextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gm-companion
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += main.cpp\
           mainwindow.cpp \
    flowlayout.cpp \
    characters.cpp \
    functions.cpp \
    music.cpp \
    sound.cpp \
    names.cpp \
    radio.cpp \
    maps.cpp \
    chareditor.cpp \
    characterpage.cpp \
    deletecharacterdialog.cpp \
    network.cpp \
    windows.cpp \
    notes.cpp \
    sifrp.cpp \
    converter.cpp \
    whatisnewwindow.cpp \
    tools/testtool.cpp \
    tools/audiotool.cpp \
    editors/audioeditor.cpp \
    managers/dbmanager.cpp \
    dialogs/notesdialog.cpp \
    dialogs/optionsdialog.cpp \
    managers/dicemanager.cpp \
    managers/filemanager.cpp \
    managers/settingsmanager.cpp \
    tools/dicetool.cpp

HEADERS  += mainwindow.h \
    flowlayout.h \
    characters.h \
    functions.h \
    chareditor.h \
    characterpage.h \
    version.h \
    deletecharacterdialog.h \
    sifrp.h \
    whatisnewwindow.h \
    tools/testtool.h \
    tools/audiotool.h \
    editors/audioeditor.h \
    managers/dbmanager.h \
    dialogs/notesdialog.h \
    dialogs/optionsdialog.h \
    managers/dicemanager.h \
    managers/filemanager.h \
    managers/settingsmanager.h \
    tools/dicetool.h

FORMS    += mainwindow.ui \
    chareditor.ui \
    characterpage.ui \
    deletecharacterdialog.ui \
    sifrp.ui \
    whatisnewwindow.ui \
    preloader.ui \
    tools/testtool.ui \
    tools/audiotool.ui \
    editors/audioeditor.ui \
    dialogs/notesdialog.ui \
    dialogs/optionsdialog.ui \
    tools/dicetool.ui

DISTFILES += \

RC_FILE = GM-Companion.rc

RESOURCES += resources.qrc \
    styles/qdarkstyle/style.qrc


TRANSLATIONS+=  translations/gm-companion_en.ts
TRANSLATIONS+=  translations/gm-companion_de.ts

win32 {
    message("Building on Windows...")
}

!win32 {
    message("Not building on Windows...")
    QMAKE_CXXFLAGS += -std=c++11

    target.path = /usr/bin
    INSTALLS += target

    data.path = /usr/share/gm-companion
    data.files = data/share/gm-companion/*

    resources.path = /usr/share/gm-companion/resources
    resources.files += data/share/resources/*

    desktop.path = /usr/share/applications
    desktop.files = data/share/applications/*

    INSTALLS += data
    INSTALLS += desktop
    INSTALLS += resources
}
