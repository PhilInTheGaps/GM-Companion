#-------------------------------------------------
#
# Project created by QtCreator 2017-04-10T13:52:09
#
#-------------------------------------------------

QT       += core multimedia network quick qml webengine

win32:QT += winextras

include(lib/fontawesome.pri/fontawesome.pri)
include(lib/o2/src/src.pri)

TARGET = out/gm-companion
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS O0_EXPORT=

CONFIG += c++11

SOURCES += src/main.cpp \
    # UI
    src/ui/colorscheme.cpp \
    # Other
    src/functions.cpp \
    # Tools
    src/tools/maptool.cpp \
    src/tools/dicetool.cpp \
    src/tools/combat_tracker/combattracker.cpp \
    src/tools/notestool.cpp \
    src/tools/convertertool.cpp \
    src/tools/generators/namegenerator.cpp \
    src/tools/project_converter/projectconverter.cpp \
    src/tools/project_converter/shopconverter.cpp \
    # Tools AUDIO
    src/tools/audio/audiotool.cpp \
    src/tools/project_converter/audioconverter.cpp \
    src/tools/audio/audioeditor.cpp \
    src/tools/audio/audioeditorfilebrowser.cpp \
    # Tools ADDONS
#    src/tools/addons/sifrp.cpp \
    # Tools CHARACTERS
    src/tools/characters/charactertool.cpp \
    # Tools ITEMS
    src/tools/shop/itemeditor.cpp \
    src/tools/shop/shopeditor.cpp \
    src/tools/shop/shoptool.cpp \
    # Managers
    src/managers/updatemanager.cpp \
    # Settings
    src/settings/settingstool.cpp \
    src/settings/settingsmanager.cpp \
    src/platformdetails.cpp \
    src/managers/addonmanager.cpp \
    src/managers/filemanager.cpp \
    src/tools/audio/spotify.cpp \
    src/tools/audio/audioexporter.cpp \
    src/tools/combat_tracker/effecttool.cpp \
    src/tools/combat_tracker/effect.cpp \
    src/tools/audio/addonelementmanager.cpp

lupdate_only{
SOURCES += *.qml \
    components/*.qml \
    tools/*.qml \
    tools/audio/*.qml \
    tools/audio/audio_exporter/*.qml \
    tools/audio/buttons/*.qml \
    tools/audio/editor/*.qml \
    tools/characters/*.qml \
    tools/combat_tracker/*.qml \
    tools/converter/*.qml \
    tools/dice/*.qml \
    tools/generators/*.qml \
    tools/maps/*.qml \
    tools/settings/*.qml \
    tools/shop/*.qml \
}

HEADERS  += src/functions.h \
    # UI
    src/ui/colorscheme.h \
    # Other
    src/version.h \
    # Tools
    src/tools/maptool.h \
    src/tools/audio/audiotool.h \
    src/tools/dicetool.h \
    src/tools/combat_tracker/combattracker.h \
    src/tools/notestool.h \
    src/tools/convertertool.h \
    src/tools/project_converter/projectconverter.h \
    # Tools AUDIO
    src/tools/project_converter/audioconverter.h \
    src/tools/audio/audioeditor.h \
    src/tools/audio/audioeditorfilebrowser.h \
    # Tools CHARACTERS
    src/tools/characters/charactertool.h \
    # Tools ADDONS
#    src/tools/addons/sifrp.h \
    # Tools GENERATORS
     src/tools/generators/namegenerator.h \
    # Tools ITEMS
    src/tools/project_converter/shopconverter.h \
    src/tools/shop/shoptool.h \
    src/tools/shop/itemeditor.h \
    src/tools/shop/shopeditor.h \
    # Managers
    src/managers/updatemanager.h \
    # Settings
    src/settings/settingstool.h \
    src/settings/settingsmanager.h \
    src/platformdetails.h \
    src/managers/addonmanager.h \
    src/managers/filemanager.h \
    src/tools/audio/spotify.h \
    src/tools/audio/audioexporter.h \
    src/tools/combat_tracker/effecttool.h \
    src/tools/combat_tracker/effect.h \
    src/tools/audio/addonelementmanager.h


DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

QML_IMPORT_PATH += lib/fontawesome.pri

RC_FILE = src/GM-Companion.rc

RESOURCES += src/resources/resources.qrc \
    src/ui/qml.qrc

TRANSLATIONS+=  src/resources/translations/gm-companion_en.ts
TRANSLATIONS+=  src/resources/translations/gm-companion_de.ts

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

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
