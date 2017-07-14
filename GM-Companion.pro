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
    characterpage.cpp \
    deletecharacterdialog.cpp \
    filemanager.cpp \
    network.cpp \
    windows.cpp \
    optionsdialog.cpp \
    notes.cpp \
    notesdialog.cpp \
    sifrp.cpp \
    converter.cpp \
    whatisnewwindow.cpp \
    dicetool.cpp \
    dbmanager.cpp

HEADERS  += mainwindow.h \
    flowlayout.h \
    characters.h \
    functions.h \
    settingsmanager.h \
    dicemanager.h \
    chareditor.h \
    characterpage.h \
    version.h \
    deletecharacterdialog.h \
    filemanager.h \
    optionsdialog.h \
    notesdialog.h \
    sifrp.h \
    whatisnewwindow.h \
    dicetool.h \
    dbmanager.h

FORMS    += mainwindow.ui \
    chareditor.ui \
    characterpage.ui \
    deletecharacterdialog.ui \
    optionsdialog.ui \
    notesdialog.ui \
    sifrp.ui \
    whatisnewwindow.ui \
    dicetool.ui \
    preloader.ui

DISTFILES += \

RC_FILE = GM-Companion.rc

RESOURCES += \
    resources.qrc

TRANSLATIONS+=  translations/gm-companion_en.ts
TRANSLATIONS+=  translations/gm-companion_de.ts

win32 {
    message("Building on Windows...")
}

!win32 {
    message("Not building on Windows...")
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
