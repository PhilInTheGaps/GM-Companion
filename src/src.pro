QT       += core multimedia network quick qml dbus

include(../lib/fontawesome.pri/fontawesome.pri)
include(../lib/o2/src/src.pri)
include(../lib/qytlib/qytlib.pri)

TARGET = ../gm-companion
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS O0_EXPORT=

CONFIG += c++14 console

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

SOURCES += main.cpp \
    # UI
    logging.cpp \
    services/spotify.cpp \
    tools/audio/audioicongenerator.cpp \
    tools/audio/audiosaveload.cpp \
    tools/audio/filesource/localaudiofilesource.cpp \
    tools/audio/players/spotifyplayer.cpp \
    tools/audio/unsplash/unsplashimage.cpp \
    tools/audio/unsplash/unsplashparser.cpp \
    tools/maps/mapmarker.cpp \
    ui/colorscheme.cpp \
    # Other
    functions.cpp \
    # Tools
    tools/toolmanager.cpp \
    tools/dicetool.cpp \
    tools/notestool.cpp \
    tools/convertertool.cpp \
    tools/generators/namegenerator.cpp \
    tools/project_converter/projectconverter.cpp \
    tools/project_converter/shopconverter.cpp \
    # Tools AUDIO
    tools/audio/audiotool.cpp \
    tools/project_converter/audioconverter.cpp \
    tools/audio/audioeditor.cpp \
    tools/audio/audioeditorfilebrowser.cpp \
    tools/audio/audioexporter.cpp \
    tools/audio/addonelementmanager.cpp \
    tools/audio/players/musicplayer.cpp \
    tools/audio/players/soundplayer.cpp \
    tools/audio/players/radioplayer.cpp \
    tools/audio/metadatareader.cpp \
    tools/audio/audioproject.cpp \
    tools/audio/audioelement.cpp \
    tools/audio/mpris/mprisadaptor.cpp \
    tools/audio/mpris/mprisplayeradaptor.cpp \
    # Tools MAPS
    tools/maps/maptool.cpp \
    tools/maps/map.cpp \
    # Tool COMBAT TRACKER
    tools/combat_tracker/effecttool.cpp \
    tools/combat_tracker/effect.cpp \
    tools/combat_tracker/combattracker.cpp \
    tools/combat_tracker/combatant.cpp \
    # Tools Shop
    tools/shop/shopproject.cpp \
    tools/shop/itemshop.cpp \
    # Tools CHARACTERS
    tools/characters/charactertool.cpp \
    tools/characters/character.cpp \
    # Tools ITEMS
    tools/shop/itemeditor.cpp \
    tools/shop/shopeditor.cpp \
    tools/shop/shoptool.cpp \
    # Managers
    managers/updatemanager.cpp \
    managers/filemanager.cpp \
    managers/addonmanager.cpp \
    managers/file_managers/mapsfilemanager.cpp \
    managers/file_managers/shopfilemanager.cpp \
    managers/file_managers/characterfilemanager.cpp \
    # Settings
    settings/settingstool.cpp \
    settings/settingsmanager.cpp \
    platformdetails.cpp \
    # Cloud
    cloud/googledrive.cpp \
    cloud/foldertree.cpp \
    tools/characters/viewers/characterimageviewer.cpp \
    tools/characters/viewers/characterviewer.cpp \
    tools/characters/viewers/characterdsa5viewer.cpp \
    tools/characters/viewers/dsa5/dsa5listitem.cpp \
    utils/processinfo.cpp
    # Addons
#    tools/addons/sifrp.cpp \

lupdate_only{
SOURCES += *.qml \
    components/*.qml \
    main/*.qml \
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

HEADERS  += functions.h \
    # UI
    logging.h \
    services/spotify.h \
    tools/audio/audioicongenerator.h \
    tools/audio/audiosaveload.h \
    tools/audio/filesource/audiofilesource.h \
    tools/audio/filesource/localaudiofilesource.h \
    tools/audio/players/spotifyplayer.h \
    tools/audio/unsplash/unsplashimage.h \
    tools/audio/unsplash/unsplashparser.h \
    tools/maps/mapmarker.h \
    ui/colorscheme.h \
    # Other
    utils/processinfo.h \
    version.h \
    # Tools
    tools/toolmanager.h \
    tools/dicetool.h \
    tools/notestool.h \
    tools/convertertool.h \
    tools/project_converter/projectconverter.h \
    # Tools AUDIO
    tools/audio/audiotool.h \
    tools/project_converter/audioconverter.h \
    tools/audio/audioeditor.h \
    tools/audio/audioeditorfilebrowser.h \
    tools/audio/audioexporter.h \
    tools/audio/addonelementmanager.h \
    tools/audio/players/musicplayer.h \
    tools/audio/players/soundplayer.h \
    tools/audio/players/radioplayer.h \
    tools/audio/metadatareader.h \
    tools/audio/audioproject.h \
    tools/audio/audioelement.h \
    tools/audio/players/audioplayer.h \
    tools/audio/mpris/mprisadaptor.h \
    tools/audio/mpris/mprisplayeradaptor.h \
    # Tools MAPS
    tools/maps/map.h \
    tools/maps/maptool.h \
    # Tools COMBAT TRACKER
    tools/combat_tracker/combattracker.h \
    tools/combat_tracker/effecttool.h \
    tools/combat_tracker/effect.h \
    tools/combat_tracker/combatant.h \
    # Tools Shops
    tools/shop/shopproject.h \
    tools/shop/itemshop.h \
    # Tools CHARACTERS
    tools/characters/charactertool.h \
    tools/characters/character.h \
    # Tools GENERATORS
    tools/generators/namegenerator.h \
    # Tools ITEMS
    tools/project_converter/shopconverter.h \
    tools/shop/shoptool.h \
    tools/shop/itemeditor.h \
    tools/shop/shopeditor.h \
    # Managers
    managers/updatemanager.h \
    managers/addonmanager.h \
    managers/filemanager.h \
    managers/file_managers/mapsfilemanager.h \
    managers/file_managers/shopfilemanager.h \
    managers/file_managers/characterfilemanager.h \
    # Settings
    settings/settingstool.h \
    settings/settingsmanager.h \
    platformdetails.h \
    # Cloud
    cloud/googledrive.h \
    cloud/foldertree.h \
    tools/characters/viewers/characterimageviewer.h \
    tools/characters/viewers/characterviewer.h \
    tools/characters/viewers/characterdsa5viewer.h \
    tools/characters/viewers/dsa5/dsa5listitem.h
    # Tools ADDONS
#    tools/addons/sifrp.h \


QML_IMPORT_PATH += ../lib/fontawesome.pri

RC_FILE = GM-Companion.rc

RESOURCES += resources/resources.qrc \
    ui/qml.qrc

TRANSLATIONS+=  resources/translations/gm-companion_en.ts
TRANSLATIONS+=  resources/translations/gm-companion_de.ts

!win32 {
    isEmpty(PREFIX) {
        PREFIX = /usr
    }
	
    target.path = $$PREFIX/bin
    
    shortcutfiles.files = ../misc/gm-companion.desktop
    shortcutfiles.path = $$PREFIX/share/applications/

    data.files += ../misc/gm-companion.png
    data.path = $$PREFIX/share/pixmaps/‚

    appdata.files = ../misc/gm-companion.appdata.xml
    appdata.path = $$PREFIX/share/metainfo/

    INSTALLS += shortcutfiles
    INSTALLS += data
    # INSTALLS += appdata

    LIBS += -L/usr/local/lib -ltag -lqt5keychain -lpoppler-qt5
    INCLUDEPATH += /usr/local/include
}

win32 {
    LIBS += -L$$PWD/../lib/taglib/lib/ -llibtag.dll
    INCLUDEPATH += $$PWD/../lib/taglib/include
    DEPENDPATH  += $$PWD/../lib/taglib/include

    LIBS += -L$$PWD/../lib/poppler/lib -lpoppler-qt5.dll
    INCLUDEPATH += $$PWD/../lib/poppler/include
    DEPENDPATH  += $$PWD/../lib/poppler/include

    LIBS += -L$$PWD/../lib/qt5keychain/lib -lqt5keychain.dll
    INCLUDEPATH += $$PWD/../lib/qt5keychain/include
    DEPENDPATH  += $$PWD/../lib/qt5keychain/include
}

INSTALLS += target

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android



