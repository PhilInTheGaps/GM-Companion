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
    tools/audiotool.cpp \
    tools/dicetool.cpp \
    tools/mapviewertool.cpp \
    editors/audioeditor.cpp \
    managers/filemanager.cpp \
    managers/settingsmanager.cpp \
    dialogs/notesdialog.cpp \
    dialogs/optionsdialog.cpp \
    dialogs/deletecharacterdialog.cpp \
    dialogs/whatisnewwindow.cpp \
    addontools/sifrp.cpp \
    ui/flowlayout.cpp \
    tools/characterviewertool.cpp \
    managers/generatormanager.cpp \
    generators/namegenerator.cpp \
    managers/updatemanager.cpp \
    dialogs/updatedialog.cpp \
    tools/notestool.cpp \
    tools/convertertool.cpp \
    dialogs/aboutdialog.cpp \
    tools/combattracker.cpp \
    managers/googledrive.cpp

HEADERS  += mainwindow.h \
    functions.h \
    version.h \
    tools/audiotool.h \
    tools/mapviewertool.h \
    tools/dicetool.h \
    editors/audioeditor.h \
    managers/filemanager.h \
    managers/settingsmanager.h \
    dialogs/notesdialog.h \
    dialogs/optionsdialog.h \
    dialogs/deletecharacterdialog.h \
    dialogs/whatisnewwindow.h \
    addontools/sifrp.h \
    ui/flowlayout.h \
    tools/characterviewertool.h \
    managers/generatormanager.h \
    generators/namegenerator.h \
    managers/updatemanager.h \
    dialogs/updatedialog.h \
    tools/notestool.h \
    tools/convertertool.h \
    dialogs/aboutdialog.h \
    tools/combattracker.h \
    managers/googledrive.h

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
    tools/characterviewertool.ui \
    managers/generatormanager.ui \
    generators/namegenerator.ui \
    dialogs/updatedialog.ui \
    tools/notestool.ui \
    tools/convertertool.ui \
    dialogs/aboutdialog.ui \
    tools/combattracker.ui

DISTFILES +=

RC_FILE = GM-Companion.rc

RESOURCES += resources.qrc \
    styles/qdarkstyle/style.qrc

TRANSLATIONS+=  translations/gm-companion_en.ts
TRANSLATIONS+=  translations/gm-companion_de.ts

!win32 {
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
