cmake_minimum_required(VERSION 3.1.0)

# find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# compiler definitions
if (${CMAKE_VERSION} VERSION_GREATER 3.11)
  add_compile_definitions(QT_DEPRECATED_WARNINGS)
endif()

# qt modules
find_package(Qt5 REQUIRED COMPONENTS Core Gui Quick Qml Multimedia Network DBus)

# submodule libraries
include(AddQYTLIB)
include(AddPoppler)
include(AddTagLib)

add_subdirectory(common)
add_subdirectory(services)
add_subdirectory(filesystem_new)

# source files
set(SOURCES
  filesystem/fileaccessgoogledrive.cpp
  filesystem/fileaccesslocal.cpp
  filesystem/fileaccessnextcloud.cpp
  filesystem/filedialog.cpp
  filesystem/filemanager.cpp
  filesystem/googledrivefile.cpp
  tools/maps/mapmarker.cpp 
  tools/dicetool.cpp 
  tools/notes/htmlgenerator.cpp 
  tools/notes/markdownhighlighter.cpp 
  tools/notes/notebook.cpp 
  tools/notes/notebookchapter.cpp 
  tools/notes/notebookpage.cpp 
  tools/notes/notessaveload.cpp 
  tools/notes/notestool.cpp 
  tools/convertertool.cpp 
  tools/generators/namegenerator.cpp 
  tools/project_converter/projectconverter.cpp 
  tools/project_converter/shopconverter.cpp 
  tools/project_converter/audioconverter.cpp
  tools/audio/audiotool.cpp 
  tools/audio/audioelementimageprovider.cpp
  tools/audio/audioicongenerator.cpp
  tools/audio/audiosaveload.cpp
  tools/audio/players/discordplayer.cpp
  tools/audio/players/spotifyplayer.cpp
  tools/audio/editor/audioeditor.cpp
  tools/audio/editor/audioeditorfilebrowser.cpp
  tools/audio/editor/audioexporter.cpp
  tools/audio/editor/addonelementmanager.cpp
  tools/audio/editor/unsplash/unsplashimage.cpp
  tools/audio/editor/unsplash/unsplashparser.cpp
  tools/audio/players/musicplayer.cpp 
  tools/audio/players/soundplayer.cpp 
  tools/audio/players/radioplayer.cpp
  tools/audio/project/audioproject.cpp
  tools/audio/project/audiocategory.cpp
  tools/audio/project/audioscenario.cpp
  tools/audio/project/audioelement.cpp
  tools/audio/project/audiofilemodel.cpp
  tools/audio/project/audioicon.cpp
  tools/audio/mpris/mprisadaptor.cpp
  tools/audio/mpris/mprisplayeradaptor.cpp
  tools/audio/mpris/mprismanager.cpp
  tools/audio/metadata/metadatareader.cpp
  tools/maps/maptool.cpp 
  tools/maps/map.cpp 
  tools/combat_tracker/effecttool.cpp 
  tools/combat_tracker/effect.cpp 
  tools/combat_tracker/combattracker.cpp 
  tools/combat_tracker/combatant.cpp 
  tools/shop/shopproject.cpp 
  tools/shop/itemshop.cpp 
  tools/characters/charactertool.cpp 
  tools/characters/character.cpp 
  tools/characters/viewers/characterimageviewer.cpp
  tools/characters/viewers/characterdsa5viewer.cpp 
  tools/characters/viewers/dsa5/dsa5listitem.cpp
  tools/shop/itemeditor.cpp
  tools/shop/shopeditor.cpp
  tools/shop/shoptool.cpp
  managers/addonmanager.cpp
)

# header files
set(HEADERS
  filesystem/filerequestcontainer.h
  filesystem/fileaccess.h
  filesystem/fileaccessgoogledrive.h
  filesystem/fileaccesslocal.h
  filesystem/fileaccessnextcloud.h
  filesystem/filedialog.h
  filesystem/filemanager.h
  filesystem/googledrivefile.h
  tools/audio/audiotool.h
  tools/audio/audioelementimageprovider.h 
  tools/audio/audioicongenerator.h 
  tools/audio/audiosaveload.h 
  tools/audio/players/discordplayer.h 
  tools/audio/players/spotifyplayer.h 
  tools/audio/project/audioproject.h
  tools/audio/project/audiocategory.h
  tools/audio/project/audioscenario.h
  tools/audio/project/audioelement.h
  tools/audio/project/audiofile.h
  tools/audio/project/audiofilemodel.h
  tools/audio/project/audioicon.h
  tools/audio/editor/audioeditor.h
  tools/audio/editor/audioeditorfilebrowser.h
  tools/audio/editor/audioexporter.h
  tools/audio/editor/addonelementmanager.h
  tools/audio/editor/unsplash/unsplashimage.h
  tools/audio/editor/unsplash/unsplashparser.h
  tools/audio/players/musicplayer.h
  tools/audio/players/soundplayer.h
  tools/audio/players/radioplayer.h
  tools/audio/metadata/metadatareader.h
  tools/audio/metadata/audiometadata.h
  tools/audio/players/audioplayer.h
  tools/audio/mpris/mprisadaptor.h
  tools/audio/mpris/mprisplayeradaptor.h
  tools/audio/mpris/mprismanager.h
  tools/maps/mapmarker.h 
  tools/notes/htmlgenerator.h
  tools/notes/markdownhighlighter.h 
  tools/notes/notebook.h 
  tools/notes/notebookchapter.h 
  tools/notes/notebookpage.h 
  tools/notes/notessaveload.h 
  tools/notes/notestool.h
  tools/dicetool.h 
  tools/convertertool.h 
  tools/project_converter/projectconverter.h 
  tools/project_converter/audioconverter.h 
  tools/project_converter/shopconverter.h
  tools/maps/map.h 
  tools/maps/maptool.h 
  tools/combat_tracker/combattracker.h 
  tools/combat_tracker/effecttool.h 
  tools/combat_tracker/effect.h 
  tools/combat_tracker/combatant.h 
  tools/shop/shopproject.h 
  tools/shop/itemshop.h
  tools/shop/shoptool.h
  tools/shop/itemeditor.h
  tools/shop/shopeditor.h
  tools/characters/charactertool.h 
  tools/characters/character.h 
  tools/characters/viewers/characterimageviewer.h 
  tools/characters/viewers/characterviewer.h
  tools/characters/viewers/characterdsa5viewer.h 
  tools/characters/viewers/dsa5/dsa5listitem.h
  tools/generators/namegenerator.h
  managers/addonmanager.h
)

# resources
set(RESOURCES
  resources/libresources.qrc
)

# target executable
add_library(gm-companion-lib STATIC ${SOURCES} ${HEADERS} ${RESOURCES})

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
  qyt
  ${TAG_LIBRARY}
  ${POPPLER_QT5_LIBRARY}
)

install(
  TARGETS gm-companion-lib
  EXPORT gm-companion-lib
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)