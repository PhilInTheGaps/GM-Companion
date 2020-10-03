cmake_minimum_required(VERSION 3.1.0)

project(gm-companion)

# find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include(AddFontAwesome)

# set QML import path
list(APPEND QML_IMPORT_PATH ${FONT_AWESOME_DIR})
list(APPEND QML_IMPORT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/ui")
list(REMOVE_DUPLICATES QML_IMPORT_PATH)
set(QML_IMPORT_PATH ${QML_IMPORT_PATH} CACHE STRING "Qt Creator extra qml import paths" FORCE)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Quick Qml LinguistTools)
find_library(GM-COMPANION-LIB_DIR gm-companion-lib)

# sources
set(SOURCES
  main.cpp
  logger.cpp
)

set(HEADERS
  version.h
  logger.h
  tools.h
)

# resources
set(RESOURCES
  resources/resources.qrc
  ui/qml.qrc
  ui/CustomComponents/components.qrc
)

# rc files
set(RCFILES
  gm-companion.rc
)

# translations
qt5_create_translation(TRANSLATIONS
  ${CMAKE_SOURCE_DIR}
  resources/translations/gm-companion_de_DE.ts
  resources/translations/gm-companion_en_US.ts
)

add_executable(gm-companion WIN32 ${SOURCES} ${HEADERS} ${RESOURCES} ${RCFILES})

message(${CMAKE_SOURCE_DIR}/src)

target_include_directories(gm-companion PRIVATE
  ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(
  gm-companion
  gm-companion-lib
  Qt5::Core
  Qt5::Gui
  Qt5::Quick
  Qt5::Qml
  fontawesome
)

install(
  TARGETS gm-companion
  EXPORT companion
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
  FILES ${CMAKE_CURRENT_SOURCE_DIR}/../misc/lol.rophil.gm_companion.desktop
  DESTINATION ${CMAKE_INSTALL_DATADIR}/applications/
)

install(
  FILES ${CMAKE_CURRENT_SOURCE_DIR}/../misc/gm-companion.png
  DESTINATION ${CMAKE_INSTALL_DATADIR}/pixmaps/
)

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../misc/icons/hicolor
  DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/
)

install(
  FILES ${CMAKE_CURRENT_SOURCE_DIR}/../misc/lol.rophil.gm_companion.appdata.xml
  DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo/
)
