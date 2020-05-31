QT       += core gui multimedia network quick qml dbus

include(../lib/fontawesome.pri/fontawesome.pri)
include(../lib/o2/src/src.pri)
include(../lib/qytlib/qytlib.pri)

TARGET = ../gm-companion
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS O0_EXPORT= NO_UPDATE_CHECK

CONFIG += c++14 qtquickcompiler

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

SOURCES += main.cpp \
    models/treeitem.cpp \
    filesystem/fileaccessgoogledrive.cpp \
    filesystem/fileaccesslocal.cpp \
    filesystem/fileaccessnextcloud.cpp \
    filesystem/filedialog.cpp \
    filesystem/filemanager.cpp \
    filesystem/googledrivefile.cpp \
    logging.cpp \
    services/google/googledrive.cpp \
    services/google/googledriveconnectorlocal.cpp \
    services/nextcloud/nextcloud.cpp \
    services/rest/restserviceconnectorlocal.cpp \
    services/service.cpp \
    services/spotify/spotify.cpp \
    services/spotify/spotifyconnectorlocal.cpp \
    services/spotify/spotifyconnectorserver.cpp \
    services/spotify/spotifyutils.cpp \
    tools/abstracttool.cpp \
    tools/audio/audioelementimageprovider.cpp \
    tools/audio/audioicongenerator.cpp \
    tools/audio/audiosaveload.cpp \
    tools/audio/players/spotifyplayer.cpp \
    tools/audio/unsplash/unsplashimage.cpp \
    tools/audio/unsplash/unsplashparser.cpp \
    tools/maps/mapmarker.cpp \
    tools/dicetool.cpp \
    tools/notes/htmlgenerator.cpp \
    tools/notes/markdownhighlighter.cpp \
    tools/notes/notebook.cpp \
    tools/notes/notebookchapter.cpp \
    tools/notes/notebookpage.cpp \
    tools/notes/notessaveload.cpp \
    tools/notes/notestool.cpp \
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
    settings/settingsmanager.cpp \
    platformdetails.cpp \
    tools/characters/viewers/characterimageviewer.cpp \
    tools/characters/viewers/characterviewer.cpp \
    tools/characters/viewers/characterdsa5viewer.cpp \
    tools/characters/viewers/dsa5/dsa5listitem.cpp \
    utils/fileutils.cpp \
    utils/markdownutils.cpp \
    utils/networkutils.cpp \
    utils/processinfo.cpp \
    utils/utils.cpp
#    tools/addons/sifrp.cpp \

lupdate_only{
SOURCES += $$files(*.qml)
}

HEADERS  += filesystem/fileaccess.h \
    models/treeitem.h \
    filesystem/fileaccessgoogledrive.h \
    filesystem/fileaccesslocal.h \
    filesystem/fileaccessnextcloud.h \
    filesystem/filedialog.h \
    filesystem/filemanager.h \
    filesystem/googledrivefile.h \
    logging.h \
    services/google/googledrive.h \
    services/google/googledriveconnectorlocal.h \
    services/nextcloud/nextcloud.h \
    services/rest/restserviceconnector.h \
    services/rest/restserviceconnectorlocal.h \
    services/service.h \
    services/services.h \
    services/spotify/spotify.h \
    services/spotify/spotifyconnectorlocal.h \
    services/spotify/spotifyconnectorserver.h \
    services/spotify/spotifyutils.h \
    settings/settings.h \
    tools/abstracttool.h \
    tools/audio/audioelementimageprovider.h \
    tools/audio/audioicongenerator.h \
    tools/audio/audiosaveload.h \
    tools/audio/players/spotifyplayer.h \
    tools/audio/unsplash/unsplashimage.h \
    tools/audio/unsplash/unsplashparser.h \
    tools/maps/mapmarker.h \
    tools/notes/htmlgenerator.h \
    tools/notes/markdownhighlighter.h \
    tools/notes/notebook.h \
    tools/notes/notebookchapter.h \
    tools/notes/notebookpage.h \
    tools/notes/notessaveload.h \
    tools/notes/notestool.h \
    utils/fileutils.h \
    utils/markdownutils.h \
    utils/networkutils.h \
    utils/processinfo.h \
    utils/utils.h \
    version.h \
    tools/dicetool.h \
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
    settings/settingsmanager.h \
    platformdetails.h \
    tools/characters/viewers/characterimageviewer.h \
    tools/characters/viewers/characterviewer.h \
    tools/characters/viewers/characterdsa5viewer.h \
    tools/characters/viewers/dsa5/dsa5listitem.h
#    tools/addons/sifrp.h \

INCLUDEPATH += $$PWD/../lib/discount
DEPENDPATH  += $$PWD/../lib/discount

RC_FILE = gm-companion.rc

RESOURCES += \
    resources/resources.qrc \
    ui/CustomComponents/components.qrc \
    ui/qml.qrc

QML_IMPORT_PATH += $$PWD/ui $$PWD/../lib/fontawesome.pri

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

    LIBS += -L/usr/local/lib -ltag -lqt5keychain -lpoppler-qt5 -lcmark-gfm -lcmark-gfm-extensions
    INCLUDEPATH += /usr/local/include
}

win32 {
    isEmpty(PREFIX) {
        PREFIX = install
    }

    target.path = $$PREFIX/bin

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

DISTFILES += \
    ui/qtquickcontrols2.conf



