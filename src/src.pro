QT       += core multimedia network quick qml dbus

include(../lib/fontawesome.pri/fontawesome.pri)
include(../lib/o2/src/src.pri)
include(../lib/qytlib/qytlib.pri)

TARGET = ../gm-companion
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS O0_EXPORT=

CONFIG += c++14

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

SOURCES += main.cpp \
    filesystem/fileaccessgoogledrive.cpp \
    filesystem/fileaccesslocal.cpp \
    filesystem/filemanager.cpp \
    filesystem/googledrivefile.cpp \
    logging.cpp \
    services/googledrive.cpp \
    services/googledriveconnectorlocal.cpp \
    services/restserviceconnectorlocal.cpp \
    services/spotify.cpp \
    services/spotifyconnectorlocal.cpp \
    services/spotifyconnectorserver.cpp \
    tools/audio/audioelementimageprovider.cpp \
    tools/audio/audioicongenerator.cpp \
    tools/audio/audiosaveload.cpp \
    tools/audio/players/spotifyplayer.cpp \
    tools/audio/unsplash/unsplashimage.cpp \
    tools/audio/unsplash/unsplashparser.cpp \
    tools/maps/mapmarker.cpp \
    ui/colorscheme.cpp \
    tools/toolmanager.cpp \
    tools/dicetool.cpp \
    tools/notestool.cpp \
    tools/convertertool.cpp \
    tools/generators/namegenerator.cpp \
    tools/project_converter/projectconverter.cpp \
    tools/project_converter/shopconverter.cpp \
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
    tools/maps/maptool.cpp \
    tools/maps/map.cpp \
    tools/combat_tracker/effecttool.cpp \
    tools/combat_tracker/effect.cpp \
    tools/combat_tracker/combattracker.cpp \
    tools/combat_tracker/combatant.cpp \
    tools/shop/shopproject.cpp \
    tools/shop/itemshop.cpp \
    tools/characters/charactertool.cpp \
    tools/characters/character.cpp \
    tools/shop/itemeditor.cpp \
    tools/shop/shopeditor.cpp \
    tools/shop/shoptool.cpp \
    managers/updatemanager.cpp \
    managers/addonmanager.cpp \
    settings/settingstool.cpp \
    settings/settingsmanager.cpp \
    platformdetails.cpp \
    tools/characters/viewers/characterimageviewer.cpp \
    tools/characters/viewers/characterviewer.cpp \
    tools/characters/viewers/characterdsa5viewer.cpp \
    tools/characters/viewers/dsa5/dsa5listitem.cpp \
    utils/processinfo.cpp \
    utils/utils.cpp
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

HEADERS  += filesystem/fileaccess.h \
    filesystem/fileaccessgoogledrive.h \
    filesystem/fileaccesslocal.h \
    filesystem/filemanager.h \
    filesystem/googledrivefile.h \
    logging.h \
    services/googledrive.h \
    services/googledriveconnectorlocal.h \
    services/restserviceconnector.h \
    services/restserviceconnectorlocal.h \
    services/services.h \
    services/spotify.h \
    services/spotifyconnectorlocal.h \
    services/spotifyconnectorserver.h \
    settings/settings.h \
    tools/audio/audioelementimageprovider.h \
    tools/audio/audioicongenerator.h \
    tools/audio/audiosaveload.h \
    tools/audio/players/spotifyplayer.h \
    tools/audio/unsplash/unsplashimage.h \
    tools/audio/unsplash/unsplashparser.h \
    tools/maps/mapmarker.h \
    ui/colorscheme.h \
    utils/processinfo.h \
    utils/utils.h \
    version.h \
    tools/toolmanager.h \
    tools/dicetool.h \
    tools/notestool.h \
    tools/convertertool.h \
    tools/project_converter/projectconverter.h \
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
    tools/maps/map.h \
    tools/maps/maptool.h \
    tools/combat_tracker/combattracker.h \
    tools/combat_tracker/effecttool.h \
    tools/combat_tracker/effect.h \
    tools/combat_tracker/combatant.h \
    tools/shop/shopproject.h \
    tools/shop/itemshop.h \
    tools/characters/charactertool.h \
    tools/characters/character.h \
    tools/generators/namegenerator.h \
    tools/project_converter/shopconverter.h \
    tools/shop/shoptool.h \
    tools/shop/itemeditor.h \
    tools/shop/shopeditor.h \
    managers/updatemanager.h \
    managers/addonmanager.h \
    settings/settingstool.h \
    settings/settingsmanager.h \
    platformdetails.h \
    tools/characters/viewers/characterimageviewer.h \
    tools/characters/viewers/characterviewer.h \
    tools/characters/viewers/characterdsa5viewer.h \
    tools/characters/viewers/dsa5/dsa5listitem.h
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
    
    shortcutfiles.files = ../misc/lol.rophil.gm_companion.desktop
    shortcutfiles.path = $$PREFIX/share/applications/

    pixmaps.files += ../misc/gm-companion.png
    pixmaps.path = $$PREFIX/share/pixmaps/

    icons.files += ../misc/icons/*
    icons.path = $$PREFIX/share/icons/

    appdata.files = ../misc/lol.rophil.gm_companion.appdata.xml
    appdata.path = $$PREFIX/share/metainfo/

    INSTALLS += shortcutfiles
    INSTALLS += pixmaps
    INSTALLS += icons
    INSTALLS += appdata

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



