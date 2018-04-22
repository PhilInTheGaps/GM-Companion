#-------------------------------------------------
#
# Project created by QtCreator 2017-04-10T13:52:09
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets network quick qml

win32:QT += winextras
include(lib/QuickDownload/quickdownload.pri)

TARGET = out/gm-companion
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += gm-companion/main.cpp \
    # UI
    gm-companion/ui/colorscheme.cpp \
    # Other
    gm-companion/functions.cpp \
    # Tools
    gm-companion/tools/maptool.cpp \
    gm-companion/tools/dicetool.cpp \
    gm-companion/tools/combattracker.cpp \
    gm-companion/tools/notestool.cpp \
    gm-companion/tools/convertertool.cpp \
    gm-companion/tools/generators/namegenerator.cpp \
    gm-companion/tools/project_converter/projectconverter.cpp \
    gm-companion/tools/project_converter/shopconverter.cpp \
    # Tools AUDIO
    gm-companion/tools/audio/audiotool.cpp \
    gm-companion/tools/project_converter/audioconverter.cpp \
    gm-companion/tools/audio/audioeditor.cpp \
    gm-companion/tools/audio/audioeditorfilebrowser.cpp \
    # Tools ADDONS
#    gm-companion/tools/addons/sifrp.cpp \
    # Tools CHARACTERS
    gm-companion/tools/characters/charactersheet.cpp \
    gm-companion/tools/characters/defaultsheet.cpp \
    gm-companion/tools/characters/charactertool.cpp \
    gm-companion/tools/characters/charactersaveload.cpp \
    gm-companion/tools/characters/dsa5sheet.cpp \
    # Tools ITEMS
    gm-companion/tools/shop/itemeditor.cpp \
    gm-companion/tools/shop/shopeditor.cpp \
    gm-companion/tools/shop/shoptool.cpp \
    # Managers
    gm-companion/managers/updatemanager.cpp \
    # Settings
    gm-companion/settings/settingstool.cpp \
    gm-companion/settings/settingsmanager.cpp \
    gm-companion/platformdetails.cpp \
    gm-companion/managers/addonmanager.cpp \
    gm-companion/managers/filemanager.cpp

lupdate_only{
SOURCES += *.qml \
    menu/*.qml \
    tools/*.qml \
    tools/audio/*.qml \
    tools/characters/*.qml \
    tools/characters/sheet_elements/*.qml \
    tools/generators/*.qml \
    tools/maps/*.qml \
    tools/notes/*.qml \
    tools/settings/*.qml \
    tools/shop/*.qml \
}

HEADERS  += gm-companion/functions.h \
    # UI
    gm-companion/ui/colorscheme.h \
    # Other
    gm-companion/version.h \
    # Tools
    gm-companion/tools/maptool.h \
    gm-companion/tools/audio/audiotool.h \
    gm-companion/tools/dicetool.h \
    gm-companion/tools/combattracker.h \
    gm-companion/tools/notestool.h \
    gm-companion/tools/convertertool.h \
    gm-companion/tools/project_converter/projectconverter.h \
    # Tools AUDIO
    gm-companion/tools/project_converter/audioconverter.h \
    gm-companion/tools/audio/audioeditor.h \
    gm-companion/tools/audio/audioeditorfilebrowser.h \
    # Tools CHARACTERS
    gm-companion/tools/characters/charactersheet.h \
    gm-companion/tools/characters/defaultsheet.h \
    gm-companion/tools/characters/charactertool.h \
    gm-companion/tools/characters/charactersaveload.h \
    gm-companion/tools/characters/dsa5sheet.h \
    # Tools ADDONS
#    gm-companion/tools/addons/sifrp.h \
    # Tools GENERATORS
     gm-companion/tools/generators/namegenerator.h \
    # Tools ITEMS
    gm-companion/tools/project_converter/shopconverter.h \
    gm-companion/tools/shop/shoptool.h \
    gm-companion/tools/shop/itemeditor.h \
    gm-companion/tools/shop/shopeditor.h \
    # Managers
    gm-companion/managers/updatemanager.h \
    # Settings
    gm-companion/settings/settingstool.h \
    gm-companion/settings/settingsmanager.h \
    gm-companion/platformdetails.h \
    gm-companion/managers/addonmanager.h \
    gm-companion/managers/filemanager.h


DISTFILES +=

QML_IMPORT_PATH =

RC_FILE = gm-companion/GM-Companion.rc

RESOURCES += gm-companion/resources/resources.qrc \
    gm-companion/ui/qml.qrc

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

unix:!macx:!android: LIBS += -ltag
