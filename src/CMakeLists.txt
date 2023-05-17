cmake_minimum_required(VERSION 3.1.0)

# find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# compiler definitions
if(${CMAKE_VERSION} VERSION_GREATER 3.11)
  add_compile_definitions(QT_DEPRECATED_WARNINGS)
endif()

# qt modules
find_package(
  Qt5 REQUIRED
  COMPONENTS Core
             Gui
             Quick
             Qml
             Multimedia
             Network
             DBus)
find_package(QuaZip-Qt5)

# submodule libraries
include(AddPoppler)
include(AddTagLib)
include(AddGSL)

add_subdirectory(common)
add_subdirectory(services)
add_subdirectory(filesystem)

# source files
set(SRC_AUDIO
    tools/audio/audiotool.h
    tools/audio/audiotool.cpp
    tools/audio/audiosaveload.h
    tools/audio/audiosaveload.cpp
    tools/audio/editor/audioeditor.h
    tools/audio/editor/audioeditor.cpp
    tools/audio/editor/audioeditorfilebrowser.h
    tools/audio/editor/audioeditorfilebrowser.cpp
    tools/audio/editor/audioexporter.h
    tools/audio/editor/audioexporter.cpp
    tools/audio/editor/addonelementmanager.h
    tools/audio/editor/addonelementmanager.cpp
    tools/audio/editor/unsplash/unsplashimage.h
    tools/audio/editor/unsplash/unsplashimage.cpp
    tools/audio/editor/unsplash/unsplashparser.h
    tools/audio/editor/unsplash/unsplashparser.cpp
    tools/audio/players/audioplayer.h
    tools/audio/players/musicplayer.h
    tools/audio/players/musicplayer.cpp
    tools/audio/players/soundplayer.h
    tools/audio/players/soundplayer.cpp
    tools/audio/players/radioplayer.h
    tools/audio/players/radioplayer.cpp
    tools/audio/players/spotifyplayer.h
    tools/audio/players/spotifyplayer.cpp
    tools/audio/project/audioproject.h
    tools/audio/project/audioproject.cpp
    tools/audio/project/audiocategory.h
    tools/audio/project/audiocategory.cpp
    tools/audio/project/audioscenario.h
    tools/audio/project/audioscenario.cpp
    tools/audio/project/audioelement.h
    tools/audio/project/audioelement.cpp
    tools/audio/project/audiofile.h
    tools/audio/project/audiofile.cpp
    tools/audio/project/audiofilemodel.h
    tools/audio/project/audiofilemodel.cpp
    tools/audio/project/audioprojectupgrader.h
    tools/audio/project/audioprojectupgrader.cpp
    tools/audio/mpris/mprismanager.h
    tools/audio/mpris/mprismanager.cpp
    tools/audio/mpris/mprisadaptor.h
    tools/audio/mpris/mprisadaptor.cpp
    tools/audio/mpris/mprisplayeradaptor.h
    tools/audio/mpris/mprisplayeradaptor.cpp
    tools/audio/metadata/audiometadata.h
    tools/audio/metadata/metadatareader.h
    tools/audio/metadata/metadatareader.cpp
    tools/audio/thumbnails/audiothumbnail.h
    tools/audio/thumbnails/audiothumbnail.cpp
    tools/audio/thumbnails/audiothumbnailcache.h
    tools/audio/thumbnails/audiothumbnailcache.cpp
    tools/audio/thumbnails/audiothumbnailcollagegenerator.h
    tools/audio/thumbnails/audiothumbnailcollagegenerator.cpp
    tools/audio/thumbnails/audiothumbnailgenerator.h
    tools/audio/thumbnails/audiothumbnailgenerator.cpp
    tools/audio/thumbnails/audiothumbnailprovider.h
    tools/audio/thumbnails/audiothumbnailprovider.cpp
    tools/audio/thumbnails/loaders/fileimageloader.h
    tools/audio/thumbnails/loaders/fileimageloader.cpp
    tools/audio/thumbnails/loaders/spotifyimageloader.h
    tools/audio/thumbnails/loaders/spotifyimageloader.cpp
    tools/audio/thumbnails/loaders/tagimageloader.h
    tools/audio/thumbnails/loaders/tagimageloader.cpp
    tools/audio/thumbnails/loaders/webimageloader.h
    tools/audio/thumbnails/loaders/webimageloader.cpp)

set(SRC_MAPS
    tools/maps/mapmarker.h
    tools/maps/mapmarker.cpp
    tools/maps/maptool.h
    tools/maps/maptool.cpp
    tools/maps/map.h
    tools/maps/map.cpp)

set(SRC_DICE tools/dicetool.h tools/dicetool.cpp)

set(SRC_NOTES
    tools/notes/notestool.h
    tools/notes/notestool.cpp
    tools/notes/htmlgenerator.h
    tools/notes/htmlgenerator.cpp
    tools/notes/markdownhighlighter.h
    tools/notes/markdownhighlighter.cpp
    tools/notes/notebook.h
    tools/notes/notebook.cpp
    tools/notes/notebookchapter.h
    tools/notes/notebookchapter.cpp
    tools/notes/notebookpage.h
    tools/notes/notebookpage.cpp
    tools/notes/notessaveload.h
    tools/notes/notessaveload.cpp)

set(SRC_ADDONS
    addons/addon.h
    addons/addon.cpp
    addons/addonmanager.h
    addons/addonmanager.cpp
    addons/addonreleaseinfo.h
    addons/addonrepository.h
    addons/addonrepositorymanager.h
    addons/addonrepositorymanager.cpp
    addons/addon_reader/abstractaddonreader.h
    addons/addon_reader/addonreader.h
    addons/addon_reader/addonreader.cpp
    addons/addon_reader/archiveaddonreader.h
    addons/addon_reader/archiveaddonreader.cpp
    addons/addon_reader/folderaddonreader.h
    addons/addon_reader/folderaddonreader.cpp)

set(SRC_GENERATORS
    tools/generators/names/namegenerator.h
    tools/generators/names/namegenerator.cpp
    tools/generators/names/abstractnamegenerator.h
    tools/generators/names/abstractnamegenerator.cpp
    tools/generators/names/listnamegenerator.h
    tools/generators/names/listnamegenerator.cpp
    tools/generators/names/namegeneratorfactory.h
    tools/generators/names/namegeneratorfactory.cpp)

set(SRC_COMBAT
    tools/combat_tracker/effect.h
    tools/combat_tracker/effect.cpp
    tools/combat_tracker/effecttool.h
    tools/combat_tracker/effecttool.cpp
    tools/combat_tracker/combattracker.h
    tools/combat_tracker/combattracker.cpp
    tools/combat_tracker/combattrackerstate.h
    tools/combat_tracker/combattrackerstate.cpp
    tools/combat_tracker/combatant.h
    tools/combat_tracker/combatant.cpp)

set(SRC_SHOP
    tools/shop/shoptool.h
    tools/shop/shoptool.cpp
    tools/shop/shopproject.h
    tools/shop/shopproject.cpp
    tools/shop/itemshop.h
    tools/shop/itemshop.cpp
    tools/shop/itemeditor.h
    tools/shop/itemeditor.cpp
    tools/shop/shopeditor.h
    tools/shop/shopeditor.cpp)

set(SRC_CHARACTERS
    tools/characters/charactertool.h
    tools/characters/charactertool.cpp
    tools/characters/character.h
    tools/characters/character.cpp
    tools/characters/viewers/characterviewer.h
    tools/characters/viewers/characterimageviewer.h
    tools/characters/viewers/characterimageviewer.cpp
    tools/characters/viewers/characterdsa5viewer.h
    tools/characters/viewers/characterdsa5viewer.cpp
    tools/characters/viewers/dsa5/dsa5listitem.h
    tools/characters/viewers/dsa5/dsa5listitem.cpp)

set(SRC_CONVERTER
    tools/converter/convertertool.h
    tools/converter/convertertool.cpp
    tools/converter/convertereditor.h
    tools/converter/convertereditor.cpp
    tools/converter/project/converterproject.h
    tools/converter/project/converterproject.cpp
    tools/converter/project/convertercategory.h
    tools/converter/project/convertercategory.cpp
    tools/converter/project/converterunit.h
    tools/converter/project/converterunit.cpp
    tools/converter/project/converterprojectupgrader.h
    tools/converter/project/converterprojectupgrader.cpp)

set(SOURCES
    ${SRC_AUDIO}
    ${SRC_MAPS}
    ${SRC_DICE}
    ${SRC_NOTES}
    ${SRC_GENERATORS}
    ${SRC_COMBAT}
    ${SRC_SHOP}
    ${SRC_CHARACTERS}
    ${SRC_CONVERTER}
    ${SRC_ADDONS})

# resources
set(RESOURCES resources/libresources.qrc)

# target executable
add_library(gm-companion-lib STATIC ${SOURCES} ${RESOURCES})

target_link_libraries(
  gm-companion-lib
  common
  services
  filesystem
  propertyhelper
  Qt5::Core
  Qt5::Gui
  Qt5::Quick
  Qt5::Qml
  Qt5::Multimedia
  Qt5::Network
  Qt5::DBus
  QuaZip::QuaZip
  ${TAG_LIBRARY}
  ${POPPLER_QT5_LIBRARY})

install(
  TARGETS gm-companion-lib
  EXPORT gm-companion-lib
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
