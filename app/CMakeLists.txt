cmake_minimum_required(VERSION 3.19)

include(AddFontAwesome)

# set QML import path
list(APPEND QML_IMPORT_PATH ${FONT_AWESOME_DIR})
list(APPEND QML_IMPORT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/ui")
list(REMOVE_DUPLICATES QML_IMPORT_PATH)
set(QML_IMPORT_PATH ${QML_IMPORT_PATH} CACHE STRING "Qt Creator extra qml import paths" FORCE)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick QuickControls2 Qml LinguistTools)
find_library(GM-COMPANION-LIB_DIR gm-companion-lib)

# sources
set(SOURCES
  main.cpp
  logger.cpp
  messages/messagemanager.cpp
  messages/messagedispatcher.cpp
)

set(HEADERS
  version.h
  version.h.in
  logger.h
  tools.h
  messages/messagemanager.h
  messages/messagedispatcher.h
  messages/message.h
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
set(TRANSLATIONS
  resources/translations/gm-companion_de_DE.ts
  resources/translations/gm-companion_en_US.ts
)

qt_add_executable(gm-companion ${SOURCES} ${HEADERS} ${RESOURCES} ${RCFILES})

if (BUILD_TRANSLATIONS)
  qt_add_translations(gm-companion
    TS_FILES
    resources/translations/gm-companion_de_DE.ts
    resources/translations/gm-companion_en_US.ts
    INCLUDE_DIRECTORIES
    .
    ../src
  )

endif()

target_include_directories(gm-companion PRIVATE
  ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(gm-companion PRIVATE
  gm-companion-lib
  Qt6::Core
  Qt6::Gui
  Qt6::Quick
  Qt6::QuickControls2
  Qt6::Qml
  fontawesome
  sentry
)

set_target_properties(gm-companion PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
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
