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
    tools/converter.cpp \
    tools/names.cpp \
    tools/notes.cpp \
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
    dialogs/updatedialog.cpp

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
    dialogs/updatedialog.h

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
    dialogs/updatedialog.ui

DISTFILES +=

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
