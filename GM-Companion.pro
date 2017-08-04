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
    functions.cpp \
    network.cpp \
    tools/audiotool.cpp \
    tools/dicetool.cpp \
    tools/mapviewertool.cpp \
    tools/characters.cpp \
    tools/converter.cpp \
    tools/names.cpp \
    tools/notes.cpp \
    editors/audioeditor.cpp \
    editors/chareditor.cpp \
    managers/dbmanager.cpp \
    managers/filemanager.cpp \
    managers/settingsmanager.cpp \
    dialogs/notesdialog.cpp \
    dialogs/optionsdialog.cpp \
    dialogs/deletecharacterdialog.cpp \
    dialogs/whatisnewwindow.cpp \
    addontools/sifrp.cpp \
    ui/characterpage.cpp \
    ui/flowlayout.cpp

HEADERS  += mainwindow.h \
    functions.h \
    version.h \
    tools/audiotool.h \
    tools/mapviewertool.h \
    tools/dicetool.h \
    tools/characters.h \
    editors/audioeditor.h \
    editors/chareditor.h \
    managers/dbmanager.h \
    managers/filemanager.h \
    managers/settingsmanager.h \
    dialogs/notesdialog.h \
    dialogs/optionsdialog.h \
    dialogs/deletecharacterdialog.h \
    dialogs/whatisnewwindow.h \
    addontools/sifrp.h \
    ui/characterpage.h \
    ui/flowlayout.h

FORMS    += mainwindow.ui \
    tools/audiotool.ui \
    editors/audioeditor.ui \
    dialogs/notesdialog.ui \
    dialogs/optionsdialog.ui \
    tools/dicetool.ui \
    dialogs/deletecharacterdialog.ui \
    dialogs/whatisnewwindow.ui \
    tools/mapviewertool.ui \
    addontools/sifrp.ui \
    editors/chareditor.ui \
    ui/characterpage.ui

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
