cmake_minimum_required(VERSION 2.8.11)
project(GM-Companion)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set ( SOURCES
 main.cpp
 mainwindow.cpp
 functions.cpp
 network.cpp
 tools/audiotool.cpp
 tools/dicetool.cpp
 tools/mapviewertool.cpp
 tools/characters.cpp
 tools/converter.cpp
 tools/names.cpp
 tools/notes.cpp
 editors/audioeditor.cpp
 editors/chareditor.cpp
 managers/filemanager.cpp
 managers/settingsmanager.cpp
 dialogs/notesdialog.cpp
 dialogs/optionsdialog.cpp
 dialogs/deletecharacterdialog.cpp
 dialogs/whatisnewwindow.cpp
 addontools/sifrp.cpp
 ui/characterpage.cpp
 ui/flowlayout.cpp
 tools/characterviewertool.cpp
 editors/charactersheeteditor.cpp
)

set ( MOC_HEADERS
 mainwindow.h
 functions.h
 version.h
 tools/audiotool.h
 tools/mapviewertool.h
 tools/dicetool.h
 tools/characters.h
 editors/audioeditor.h
 editors/chareditor.h
 managers/filemanager.h
 managers/settingsmanager.h
 dialogs/notesdialog.h
 dialogs/optionsdialog.h
 dialogs/deletecharacterdialog.h
 dialogs/whatisnewwindow.h
 addontools/sifrp.h
 ui/characterpage.h
 ui/flowlayout.h
 tools/characterviewertool.h
 editors/charactersheeteditor.h
)

set ( UIS
 mainwindow.ui
 tools/audiotool.ui
 editors/audioeditor.ui
 dialogs/notesdialog.ui
 dialogs/optionsdialog.ui
 tools/dicetool.ui
 dialogs/deletecharacterdialog.ui
 dialogs/whatisnewwindow.ui
 tools/mapviewertool.ui
 addontools/sifrp.ui
 editors/chareditor.ui
 ui/characterpage.ui
 tools/characterviewertool.ui
 editors/charactersheeteditor.ui
)

set ( RESOURCES
 resources.qrc
 styles/qdarkstyle/style.qrc
)

# Next lines needed for building all Qt projects
find_package( Qt4 REQUIRED )
include( ${QT_USE_FILE} )
add_definitions( ${QT_DEFINITIONS} )
include_directories( ${CMAKE_BINARY_DIR} )

# Widgets finds its own dependencies.
find_package(Qt5 REQUIRED
    Core
    Gui
    Widgets
    Multimedia
    MultimediaWidgets
    Sql
    Network
)

# Tell CMake to create the gm-companion executable
add_executable(gm-companion main.cpp mainwindow.cpp)

qt5_use_modules(gm-companion core gui widgets multimedia multimediawidgets sql network)
