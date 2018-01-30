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

TARGET = out/gm-companion
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += gm-companion/main.cpp \
    # UI
    gm-companion/ui/mainwindow.cpp \
    gm-companion/ui/whatisnewwindow.cpp \
    gm-companion/ui/flowlayout.cpp \
    gm-companion/ui/updatedialog.cpp \
    gm-companion/ui/aboutdialog.cpp \
    # Other
    gm-companion/functions.cpp \
    # Tools
    gm-companion/tools/dicetool.cpp \
    gm-companion/tools/mapviewertool.cpp \
    gm-companion/tools/convertertool.cpp \
    gm-companion/tools/combattracker.cpp \
    # Tools AUDIO
    gm-companion/tools/audio/audiotool.cpp \
    gm-companion/tools/audio/audioeditor.cpp \
    # Tools ADDONS
    gm-companion/tools/addons/sifrp.cpp \
    # Tools CHARACTERS
    gm-companion/tools/characters/sheets/defaultsheet.cpp \
    gm-companion/tools/characters/sheets/sheetfunctions.cpp \
    gm-companion/tools/characters/sheets/dsa5sheet.cpp \
    # Tools NOTES
    gm-companion/tools/notes/notesdialog.cpp \
    gm-companion/tools/notes/notestool.cpp \
    # Tools GENERATORS
    gm-companion/tools/generators/generatormanager.cpp \
    gm-companion/tools/generators/namegenerator.cpp \
    # Tools ITEMS
    gm-companion/tools/items/shoptool.cpp \
    gm-companion/tools/items/shopeditor.cpp \
    gm-companion/tools/items/itemeditor.cpp \
    # Managers
    gm-companion/managers/filemanager.cpp \
    gm-companion/managers/updatemanager.cpp \
    # Settings
    gm-companion/settings/settingsmanager.cpp \
    gm-companion/settings/optionsdialog.cpp \
    gm-companion/tools/addons/addonmanager.cpp \
    gm-companion/tools/characters/charactertool.cpp \
    gm-companion/tools/characters/sheets/entaria2sheet.cpp

HEADERS  += gm-companion/ui/mainwindow.h \
    # UI
    gm-companion/ui/whatisnewwindow.h \
    gm-companion/ui/flowlayout.h \
    gm-companion/ui/updatedialog.h \
    gm-companion/ui/aboutdialog.h \
    # Other
    gm-companion/functions.h \
    gm-companion/version.h \
    # Tools
    gm-companion/tools/mapviewertool.h \
    gm-companion/tools/dicetool.h \
    gm-companion/tools/convertertool.h \
    gm-companion/tools/combattracker.h \
    # Tools AUDIO
    gm-companion/tools/audio/audiotool.h \
    gm-companion/tools/audio/audioeditor.h \
    # Tools NOTES
    gm-companion/tools/notes/notesdialog.h \
    gm-companion/tools/notes/notestool.h \
    # Tools CHARACTERS
    gm-companion/tools/characters/charactertool.h \
    gm-companion/tools/characters/sheets/defaultsheet.h \
    gm-companion/tools/characters/sheets/sheetfunctions.h \
    gm-companion/tools/characters/sheets/dsa5sheet.h \
    # Tools ADDONS
    gm-companion/tools/addons/sifrp.h \
    # Tools GENERATORS
    gm-companion/tools/generators/generatormanager.h \
    gm-companion/tools/generators/namegenerator.h \
    # Tools ITEMS
    gm-companion/tools/items/shoptool.h \
    gm-companion/tools/items/shopeditor.h \
    gm-companion/tools/items/itemeditor.h \
    # Managers
    gm-companion/managers/filemanager.h \
    gm-companion/managers/updatemanager.h \
    # Settings
    gm-companion/settings/settingsmanager.h \
    gm-companion/settings/optionsdialog.h \
    gm-companion/tools/addons/addonmanager.h \
    gm-companion/tools/characters/sheets/entaria2sheet.h

FORMS    += gm-companion/ui/mainwindow.ui \
    # UI
    gm-companion/ui/whatisnewwindow.ui \
    gm-companion/ui/updatedialog.ui \
    gm-companion/ui/aboutdialog.ui \
    # Tools
    gm-companion/tools/dicetool.ui \
    gm-companion/tools/mapviewertool.ui \
    gm-companion/tools/convertertool.ui \
    gm-companion/tools/combattracker.ui \
    # Tools AUDIO
    gm-companion/tools/audio/audiotool.ui \
    gm-companion/tools/audio/audioeditor.ui \
    # Tools ITEMS
    gm-companion/tools/items/shoptool.ui \
    gm-companion/tools/items/shopeditor.ui \
    gm-companion/tools/items/itemeditor.ui \
    # Tools NOTES
    gm-companion/tools/notes/notesdialog.ui \
    gm-companion/tools/notes/notestool.ui \
    # Tools CHARACTERS
    gm-companion/tools/characters/charactertool.ui \
    gm-companion/tools/characters/sheets/defaultsheet.ui \
    gm-companion/tools/characters/sheets/dsa5sheet.ui \
    # Tools ADDONS
    gm-companion/tools/addons/sifrp.ui \
    # Tools GENERATORS
    gm-companion/tools/generators/generatormanager.ui \
    gm-companion/tools/generators/namegenerator.ui \
    # Settings
    gm-companion/settings/optionsdialog.ui \
    gm-companion/tools/addons/addonmanager.ui \
    gm-companion/tools/characters/sheets/entaria2sheet.ui

DISTFILES +=

RC_FILE = gm-companion/GM-Companion.rc

RESOURCES += gm-companion/resources/resources.qrc \
    gm-companion/resources/styles/qdarkstyle/style.qrc

TRANSLATIONS+=  gm-companion/resources/translations/gm-companion_en.ts
TRANSLATIONS+=  gm-companion/resources/translations/gm-companion_de.ts

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

unix:!macx: LIBS += -ltag
