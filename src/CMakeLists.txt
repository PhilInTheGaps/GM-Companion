cmake_minimum_required(VERSION 3.19)

add_subdirectory(common)
add_subdirectory(services)
add_subdirectory(filesystem)

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
    tools/audio/players/audioplayer.cpp
    tools/audio/players/bufferedaudioplayer.h
    tools/audio/players/bufferedaudioplayer.cpp
    tools/audio/players/musicplayer.h
    tools/audio/players/musicplayer.cpp
    tools/audio/players/soundplayer.h
    tools/audio/players/soundplayer.cpp
    tools/audio/players/soundplayercontroller.h
    tools/audio/players/soundplayercontroller.cpp
    tools/audio/players/radioplayer.h
    tools/audio/players/radioplayer.cpp
    tools/audio/players/spotifyplayer.h
    tools/audio/players/spotifyplayer.cpp
    tools/audio/playlist/audioplaylist.h
    tools/audio/playlist/audioplaylist.cpp
    tools/audio/playlist/audioplaylistfactory.h
    tools/audio/playlist/audioplaylistfactory.cpp
    tools/audio/playlist/resolvingaudioplaylist.h
    tools/audio/playlist/resolvingaudioplaylist.cpp
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

set_source_files_properties(${SRC_AUDIO} PROPERTIES UNITY_GROUP "audio")

set(SRC_MAPS
    tools/maps/mapmarker.h
    tools/maps/mapmarker.cpp
    tools/maps/maptool.h
    tools/maps/maptool.cpp
    tools/maps/map.h
    tools/maps/map.cpp)

set_source_files_properties(${SRC_MAPS} PROPERTIES UNITY_GROUP "maps")

set(SRC_DICE
    tools/dicetool.h
    tools/dicetool.cpp)

set_source_files_properties(${SRC_DICE} PROPERTIES UNITY_GROUP "dice")

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

set_source_files_properties(${SRC_NOTES} PROPERTIES UNITY_GROUP "notes")

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

set_source_files_properties(${SRC_ADDONS} PROPERTIES UNITY_GROUP "addons")

set(SRC_GENERATORS
    tools/generators/names/namegenerator.h
    tools/generators/names/namegenerator.cpp
    tools/generators/names/abstractnamegenerator.h
    tools/generators/names/abstractnamegenerator.cpp
    tools/generators/names/listnamegenerator.h
    tools/generators/names/listnamegenerator.cpp
    tools/generators/names/namegeneratorfactory.h
    tools/generators/names/namegeneratorfactory.cpp)

set_source_files_properties(${SRC_GENERATORS} PROPERTIES UNITY_GROUP "generators")

set(SRC_COMBAT
    tools/combat_tracker/combattracker.h
    tools/combat_tracker/combattracker.cpp
    tools/combat_tracker/combattrackerstate.h
    tools/combat_tracker/combattrackerstate.cpp
    tools/combat_tracker/combatant.h
    tools/combat_tracker/combatant.cpp)

set_source_files_properties(${SRC_COMBAT} PROPERTIES UNITY_GROUP "combat")

set(SRC_SHOP
    tools/shop/baseshoptool.h
    tools/shop/baseshoptool.cpp
    tools/shop/shoptool.h
    tools/shop/shoptool.cpp
    tools/shop/itemeditor.h
    tools/shop/itemeditor.cpp
    tools/shop/shopeditor.h
    tools/shop/shopeditor.cpp
    tools/shop/project/shopproject.h
    tools/shop/project/shopproject.cpp
    tools/shop/project/shopcategory.h
    tools/shop/project/shopcategory.cpp
    tools/shop/project/itemshop.h
    tools/shop/project/itemshop.cpp
    tools/shop/project/item.h
    tools/shop/project/item.cpp
    tools/shop/project/itemgroup.h
    tools/shop/project/itemgroup.cpp)

set_source_files_properties(${SRC_SHOP} PROPERTIES UNITY_GROUP "shops")

set(SRC_CHARACTERS
    tools/characters/charactertool.h
    tools/characters/charactertool.cpp
    tools/characters/character.h
    tools/characters/character.cpp
    tools/characters/viewers/characterviewer.h
    tools/characters/viewers/characterimageviewer.h
    tools/characters/viewers/characterimageviewer.cpp)

set_source_files_properties(${SRC_CHARACTERS} PROPERTIES UNITY_GROUP "characters")

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

set_source_files_properties(${SRC_CONVERTER} PROPERTIES UNITY_GROUP "converter")

set(SRC_MESSAGES
    messages/message.h
    messages/messagemanager.h
    messages/messagemanager.cpp
    messages/messagemodel.h)

set_source_files_properties(${SRC_MESSAGES} PROPERTIES UNITY_GROUP "messages")

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
    ${SRC_ADDONS}
    ${SRC_MESSAGES})

qt_add_library(gm-companion-lib STATIC)

qt_add_qml_module(gm-companion-lib
    URI src
    SOURCES ${SOURCES}
)

qt_add_resources(gm-companion-lib "libresources"
    PREFIX "/"
    BASE resources
    FILES
        resources/units/units.json
        resources/notes/style.css
        resources/names/generic.json)

target_precompile_headers(gm-companion-lib REUSE_FROM common)

set_target_properties(gm-companion-lib PROPERTIES
    UNITY_BUILD_MODE GROUP)

target_include_directories(gm-companion-lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/addons
        ${CMAKE_CURRENT_SOURCE_DIR}/messages
        ${CMAKE_CURRENT_SOURCE_DIR}/tools
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/audio
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/audio/editor
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/audio/editor/unsplash
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/audio/metadata
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/audio/players
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/audio/project
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/audio/thumbnails
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/characters
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/characters/viewers
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/combat_tracker
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/converter
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/converter/project
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/generators/names
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/maps
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/notes
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/shop
        ${CMAKE_CURRENT_SOURCE_DIR}/tools/shop/project)

target_link_libraries(gm-companion-lib PUBLIC
    common
    commonplugin
    services
    servicesplugin
    filesystem
    filesystemplugin
    propertyhelper
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::Qml
    Qt6::Multimedia
    Qt6::Network
    Qt6::DBus
    Qt6::Concurrent
    Qt6::Core5Compat
    QuaZip::QuaZip
    ${TAG_LIBRARY}
    ${POPPLER_QT6_LIBRARY}
)

install(
    TARGETS gm-companion-lib
    EXPORT gm-companion-lib
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
