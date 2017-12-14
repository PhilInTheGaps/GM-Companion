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

SOURCES += gm-companion/main.cpp\
    gm-companion/mainwindow.cpp \
    gm-companion/functions.cpp \
    gm-companion/tools/audiotool.cpp \
    gm-companion/tools/dicetool.cpp \
    gm-companion/tools/mapviewertool.cpp \
    gm-companion/editors/audioeditor.cpp \
    gm-companion/managers/filemanager.cpp \
    gm-companion/managers/settingsmanager.cpp \
    gm-companion/dialogs/notesdialog.cpp \
    gm-companion/dialogs/optionsdialog.cpp \
    gm-companion/dialogs/deletecharacterdialog.cpp \
    gm-companion/dialogs/whatisnewwindow.cpp \
    gm-companion/addontools/sifrp.cpp \
    gm-companion/ui/flowlayout.cpp \
    gm-companion/tools/characterviewertool.cpp \
    gm-companion/managers/generatormanager.cpp \
    gm-companion/generators/namegenerator.cpp \
    gm-companion/managers/updatemanager.cpp \
    gm-companion/dialogs/updatedialog.cpp \
    gm-companion/tools/notestool.cpp \
    gm-companion/tools/convertertool.cpp \
    gm-companion/dialogs/aboutdialog.cpp \
    gm-companion/tools/combattracker.cpp \
    gm-companion/managers/googledrive.cpp

HEADERS  += gm-companion/mainwindow.h \
    gm-companion/functions.h \
    gm-companion/version.h \
    gm-companion/tools/audiotool.h \
    gm-companion/tools/mapviewertool.h \
    gm-companion/tools/dicetool.h \
    gm-companion/editors/audioeditor.h \
    gm-companion/managers/filemanager.h \
    gm-companion/managers/settingsmanager.h \
    gm-companion/dialogs/notesdialog.h \
    gm-companion/dialogs/optionsdialog.h \
    gm-companion/dialogs/deletecharacterdialog.h \
    gm-companion/dialogs/whatisnewwindow.h \
    gm-companion/addontools/sifrp.h \
    gm-companion/ui/flowlayout.h \
    gm-companion/tools/characterviewertool.h \
    gm-companion/managers/generatormanager.h \
    gm-companion/generators/namegenerator.h \
    gm-companion/managers/updatemanager.h \
    gm-companion/dialogs/updatedialog.h \
    gm-companion/tools/notestool.h \
    gm-companion/tools/convertertool.h \
    gm-companion/dialogs/aboutdialog.h \
    gm-companion/tools/combattracker.h \
    gm-companion/managers/googledrive.h

FORMS    += gm-companion/mainwindow.ui \
    gm-companion/tools/audiotool.ui \
    gm-companion/editors/audioeditor.ui \
    gm-companion/dialogs/notesdialog.ui \
    gm-companion/dialogs/optionsdialog.ui \
    gm-companion/tools/dicetool.ui \
    gm-companion/dialogs/deletecharacterdialog.ui \
    gm-companion/dialogs/whatisnewwindow.ui \
    gm-companion/tools/mapviewertool.ui \
    gm-companion/addontools/sifrp.ui \
    gm-companion/tools/characterviewertool.ui \
    gm-companion/managers/generatormanager.ui \
    gm-companion/generators/namegenerator.ui \
    gm-companion/dialogs/updatedialog.ui \
    gm-companion/tools/notestool.ui \
    gm-companion/tools/convertertool.ui \
    gm-companion/dialogs/aboutdialog.ui \
    gm-companion/tools/combattracker.ui

DISTFILES +=

RC_FILE = gm-companion/GM-Companion.rc

RESOURCES += gm-companion/resources.qrc \
    gm-companion/styles/qdarkstyle/style.qrc

TRANSLATIONS+=  gm-companion/translations/gm-companion_en.ts
TRANSLATIONS+=  gm-companion/translations/gm-companion_de.ts

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
